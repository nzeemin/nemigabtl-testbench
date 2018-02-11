/*  This file is part of NEMIGABTL.
    NEMIGABTL is free software: you can redistribute it and/or modify it under the terms
of the GNU Lesser General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.
    NEMIGABTL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU Lesser General Public License for more details.
    You should have received a copy of the GNU Lesser General Public License along with
NEMIGABTL. If not, see <http://www.gnu.org/licenses/>. */

// Emulator.cpp

#include "stdafx.h"
#include <stdio.h>
#include <Share.h>
#include "Emulator.h"
#include "emubase\Board.h"
#include "emubase\Processor.h"


//////////////////////////////////////////////////////////////////////


CMotherboard* g_pBoard = NULL;
int g_nEmulatorConfiguration;  // Current configuration

BOOL g_okEmulatorRunning = FALSE;

WORD m_wEmulatorCPUBreakpoint = 0177777;

BOOL m_okEmulatorSound = FALSE;

BOOL m_okEmulatorParallel = FALSE;
BOOL m_okEmulatorSerial = FALSE;
HANDLE m_hEmulatorComPort = INVALID_HANDLE_VALUE;

FILE* m_fpEmulatorParallelOut = NULL;

long m_nFrameCount = 0;
DWORD m_dwTickCount = 0;
DWORD m_dwEmulatorUptime = 0;  // Machine uptime, seconds, from turn on or reset, increments every 25 frames
long m_nUptimeFrameCount = 0;

BYTE* g_pEmulatorRam;  // RAM values - for change tracking
BYTE* g_pEmulatorChangedRam;  // RAM change flags
WORD g_wEmulatorCpuPC = 0177777;      // Current PC value
WORD g_wEmulatorPrevCpuPC = 0177777;  // Previous PC value


void CALLBACK Emulator_SoundGenCallback(unsigned short L, unsigned short R);

//////////////////////////////////////////////////////////////////////
//Прототип функции преобразования экрана
// Input:
//   pVideoBuffer   Исходные данные, биты экрана БК
//   pPalette       Палитра
//   pImageBits     Результат, 32-битный цвет, размер для каждой функции свой
typedef void (CALLBACK* PREPARE_SCREEN_CALLBACK)(const BYTE* pVideoBuffer, const DWORD* pPalette, void* pImageBits);

void CALLBACK Emulator_PrepareScreenBW512x256(const BYTE* pVideoBuffer, const DWORD* palette, void* pImageBits);
void CALLBACK Emulator_PrepareScreenBW512x312(const BYTE* pVideoBuffer, const DWORD* palette, void* pImageBits);

const DWORD ScreenView_Palette[4] =
{
    0x000000, 0xB0B0B0, 0x404040, 0xFFFFFF
};
const DWORD Emulator_DiffPalette[16] =
{
    0x000000, 0xB0B0B0, 0x404040, 0xFFFFFF,
    0x00403F, 0x00B03F, 0x00FF3F,
    0x40003F, 0x40B03F, 0x40FF3F,
    0xB0003F, 0xB0403F, 0xB0FF3F,
    0xFF003F, 0xFF403F, 0xFFB03F
};


//////////////////////////////////////////////////////////////////////


const LPCTSTR FILENAME_ROM_303 = _T("nemiga-303.rom");
const LPCTSTR FILENAME_ROM_405 = _T("nemiga-405.rom");
const LPCTSTR FILENAME_ROM_406 = _T("nemiga-406.rom");


//////////////////////////////////////////////////////////////////////

BOOL Emulator_LoadRomFile(LPCTSTR strFileName, BYTE* buffer, DWORD fileOffset, DWORD bytesToRead)
{
    FILE* fpRomFile = ::_tfsopen(strFileName, _T("rb"), _SH_DENYWR);
    if (fpRomFile == NULL)
        return FALSE;

    ::memset(buffer, 0, bytesToRead);

    if (fileOffset > 0)
    {
        ::fseek(fpRomFile, fileOffset, SEEK_SET);
    }

    DWORD dwBytesRead = ::fread(buffer, 1, bytesToRead, fpRomFile);
    if (dwBytesRead != bytesToRead)
    {
        ::fclose(fpRomFile);
        return FALSE;
    }

    ::fclose(fpRomFile);

    return TRUE;
}

BOOL Emulator_Init()
{
    ASSERT(g_pBoard == NULL);

    CProcessor::Init();

    g_pBoard = new CMotherboard();

    // Allocate memory for old RAM values
    g_pEmulatorRam = (BYTE*) ::malloc(65536);  ::memset(g_pEmulatorRam, 0, 65536);
    g_pEmulatorChangedRam = (BYTE*) ::malloc(65536);  ::memset(g_pEmulatorChangedRam, 0, 65536);

    g_pBoard->Reset();

    //if (m_okEmulatorSound)
    //{
    //    SoundGen_Initialize(Settings_GetSoundVolume());
    //    g_pBoard->SetSoundGenCallback(Emulator_SoundGenCallback);
    //}

    return TRUE;
}

void Emulator_Done()
{
    ASSERT(g_pBoard != NULL);

    CProcessor::Done();

    //g_pBoard->SetSoundGenCallback(NULL);
    //SoundGen_Finalize();

    //g_pBoard->SetSerialCallbacks(NULL, NULL);
    //if (m_hEmulatorComPort != INVALID_HANDLE_VALUE)
    //{
    //    ::CloseHandle(m_hEmulatorComPort);
    //    m_hEmulatorComPort = INVALID_HANDLE_VALUE;
    //}

    delete g_pBoard;
    g_pBoard = NULL;

    // Free memory used for old RAM values
    ::free(g_pEmulatorRam);
    ::free(g_pEmulatorChangedRam);
}

BOOL Emulator_InitConfiguration(WORD configuration)
{
    g_pBoard->SetConfiguration(configuration);

    LPCTSTR szRomFileName = NULL;
    switch (configuration)
    {
    case EMU_CONF_NEMIGA303:
        szRomFileName = FILENAME_ROM_303;
        break;
    case EMU_CONF_NEMIGA405:
        szRomFileName = FILENAME_ROM_405;
        break;
    case EMU_CONF_NEMIGA406:
        szRomFileName = FILENAME_ROM_406;
        break;
    default:
        szRomFileName = FILENAME_ROM_303;
        break;
    }

    BYTE buffer[4096];

    // Load ROM file
    if (!Emulator_LoadRomFile(szRomFileName, buffer, 0, 4096))
    {
        AlertWarning(_T("Failed to load the ROM file."));
        return FALSE;
    }
    g_pBoard->LoadROM(buffer);

    g_nEmulatorConfiguration = configuration;

    g_pBoard->Reset();

    m_nUptimeFrameCount = 0;
    m_dwEmulatorUptime = 0;

    return TRUE;
}

void Emulator_Start()
{
    g_okEmulatorRunning = TRUE;

    //// Set title bar text
    //SetWindowText(g_hwnd, _T("NEMIGA Back to Life [run]"));
    //MainWindow_UpdateMenu();

    //m_nFrameCount = 0;
    //m_dwTickCount = GetTickCount();
}
void Emulator_Stop()
{
    g_okEmulatorRunning = FALSE;
    m_wEmulatorCPUBreakpoint = 0177777;

    //if (m_fpEmulatorParallelOut != NULL)
    //    ::fflush(m_fpEmulatorParallelOut);

    //// Reset title bar message
    //SetWindowText(g_hwnd, _T("NEMIGA Back to Life [stop]"));
    //MainWindow_UpdateMenu();
    //// Reset FPS indicator
    //MainWindow_SetStatusbarText(StatusbarPartFPS, _T(""));

    //MainWindow_UpdateAllViews();
}

void Emulator_Reset()
{
    ASSERT(g_pBoard != NULL);

    g_pBoard->Reset();

    m_nUptimeFrameCount = 0;
    //m_dwEmulatorUptime = 0;

    //MainWindow_UpdateAllViews();
}

void Emulator_SetCPUBreakpoint(WORD address)
{
    m_wEmulatorCPUBreakpoint = address;
}

BOOL Emulator_IsBreakpoint()
{
    WORD wCPUAddr = g_pBoard->GetCPU()->GetPC();
    if (wCPUAddr == m_wEmulatorCPUBreakpoint)
        return TRUE;
    return FALSE;
}

int Emulator_SystemFrame()
{
    g_pBoard->SetCPUBreakpoint(m_wEmulatorCPUBreakpoint);

    //ScreenView_ScanKeyboard();
    //ScreenView_ProcessKeyboard();

    if (!g_pBoard->SystemFrame())
        return 0;

    //// Calculate frames per second
    //m_nFrameCount++;
    //DWORD dwCurrentTicks = GetTickCount();
    //long nTicksElapsed = dwCurrentTicks - m_dwTickCount;
    //if (nTicksElapsed >= 1200)
    //{
    //    double dFramesPerSecond = m_nFrameCount * 1000.0 / nTicksElapsed;
    //    double dSpeed = dFramesPerSecond / 25.0 * 100;
    //    TCHAR buffer[16];
    //    swprintf_s(buffer, 16, _T("%03.f%%"), dSpeed);
    //    MainWindow_SetStatusbarText(StatusbarPartFPS, buffer);

    //    BOOL floppyEngine = g_pBoard->IsFloppyEngineOn();
    //    MainWindow_SetStatusbarText(StatusbarPartFloppyEngine, floppyEngine ? _T("Motor") : NULL);

    //    m_nFrameCount = 0;
    //    m_dwTickCount = dwCurrentTicks;
    //}

    // Calculate emulator uptime (25 frames per second)
    m_nUptimeFrameCount++;
    if (m_nUptimeFrameCount >= 25)
    {
        m_dwEmulatorUptime++;
        m_nUptimeFrameCount = 0;

        //int seconds = (int) (m_dwEmulatorUptime % 60);
        //int minutes = (int) (m_dwEmulatorUptime / 60 % 60);
        //int hours   = (int) (m_dwEmulatorUptime / 3600 % 60);

        //TCHAR buffer[20];
        //swprintf_s(buffer, 20, _T("Uptime: %02d:%02d:%02d"), hours, minutes, seconds);
        //MainWindow_SetStatusbarText(StatusbarPartUptime, buffer);
    }

    return 1;
}

void Emulator_PrepareScreenRGB32(void* pImageBits)
{
    if (pImageBits == NULL) return;

    const BYTE* pVideoBuffer = g_pBoard->GetVideoBuffer();
    ASSERT(pVideoBuffer != NULL);

    // Render to bitmap
    PREPARE_SCREEN_CALLBACK callback = Emulator_PrepareScreenBW512x312;
    callback(pVideoBuffer, ScreenView_Palette, pImageBits);
}

const DWORD * Emulator_GetPalette()
{
    return ScreenView_Palette;
}

#define AVERAGERGB(a, b)  ( (((a) & 0xfefefeffUL) + ((b) & 0xfefefeffUL)) >> 1 )

void CALLBACK Emulator_PrepareScreenBW512x256(const BYTE* pVideoBuffer, const DWORD* palette, void* pImageBits)
{
    for (int y = 0; y < 256; y++)
    {
        const WORD* pVideo = (WORD*)(pVideoBuffer + y * 512 / 4);
        DWORD* pBits = (DWORD*)pImageBits + (256 - 1 - y) * 512;
        for (int x = 0; x < 512 / 8; x++)
        {
            WORD src = *pVideo;

            for (int bit = 0; bit < 8; bit++)
            {
                int colorindex = (src & 0x80) >> 7 | (src & 0x8000) >> 14;
                DWORD color = palette[colorindex];
                *pBits = color;
                pBits++;
                src = src << 1;
            }

            pVideo++;
        }
    }
}

void CALLBACK Emulator_PrepareScreenBW512x312(const BYTE* pVideoBuffer, const DWORD* palette, void* pImageBits)
{
    DWORD * pImageStart = ((DWORD *)pImageBits) + 512 * 28;
    Emulator_PrepareScreenBW512x256(pVideoBuffer, palette, pImageStart);
}


//////////////////////////////////////////////////////////////////////

DWORD Emulator_GetUptime()
{
    return m_dwEmulatorUptime;
}

BOOL Emulator_AttachFloppyImage(int slot, LPCTSTR sFilePath)
{
    return g_pBoard->AttachFloppyImage(slot, sFilePath);
}
void Emulator_DetachFloppyImage(int slot)
{
    return g_pBoard->DetachFloppyImage(slot);
}

BOOL Emulator_Run(int frames)
{
    for (int i = 0; i < frames; i++)
    {
        int res = Emulator_SystemFrame();
        if (!res)
            return FALSE;
    }

    return TRUE;
}

BOOL Emulator_RunUntilMotorOff()
{
    for (;;)
    {
        int res = Emulator_SystemFrame();
        if (!res)
            return FALSE;
        if (!g_pBoard->IsFloppyEngineOn())
            break;
    }
    return TRUE;
}

BOOL Emulator_SaveScreenshot(LPCTSTR sFileName, const DWORD * bits, const DWORD * palette)
{
    ASSERT(bits != NULL);
    ASSERT(sFileName != NULL);

    // Create file
    HANDLE hFile = ::CreateFile(sFileName,
            GENERIC_WRITE, FILE_SHARE_READ, NULL,
            CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
        return FALSE;

    BITMAPFILEHEADER hdr;
    ::ZeroMemory(&hdr, sizeof(hdr));
    hdr.bfType = 0x4d42;  // "BM"
    BITMAPINFOHEADER bih;
    ::ZeroMemory(&bih, sizeof(bih));
    bih.biSize = sizeof(BITMAPINFOHEADER);
    bih.biWidth = 512;
    bih.biHeight = 312;
    bih.biSizeImage = bih.biWidth * bih.biHeight;
    bih.biPlanes = 1;
    bih.biBitCount = 8;
    bih.biCompression = BI_RGB;
    bih.biXPelsPerMeter = bih.biXPelsPerMeter = 2000;
    hdr.bfSize = (DWORD) sizeof(BITMAPFILEHEADER) + bih.biSize + bih.biSizeImage;
    hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) + bih.biSize + sizeof(RGBQUAD) * 256;

    DWORD dwBytesWritten = 0;

    BYTE * pData = (BYTE *) ::malloc(bih.biSizeImage);

    // Prepare the image data
    const DWORD * psrc = bits;
    BYTE * pdst = pData;
    for (int i = 0; i < 512 * 312; i++)
    {
        DWORD rgb = *psrc;
        psrc++;
        BYTE color = 0;
        for (BYTE c = 0; c < 128; c++)
        {
            if (palette[c] == rgb)
            {
                color = c;
                break;
            }
        }
        *pdst = color;
        pdst++;
    }

    WriteFile(hFile, &hdr, sizeof(BITMAPFILEHEADER), &dwBytesWritten, NULL);
    if (dwBytesWritten != sizeof(BITMAPFILEHEADER))
    {
        ::free(pData);
        return FALSE;
    }
    WriteFile(hFile, &bih, sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL);
    if (dwBytesWritten != sizeof(BITMAPINFOHEADER))
    {
        ::free(pData);
        return FALSE;
    }
    WriteFile(hFile, palette, sizeof(RGBQUAD) * 128, &dwBytesWritten, NULL);
    if (dwBytesWritten != sizeof(RGBQUAD) * 128)
    {
        ::free(pData);
        return FALSE;
    }
    //NOTE: Write the palette for the second time, to fill colors #128-255
    WriteFile(hFile, palette, sizeof(RGBQUAD) * 128, &dwBytesWritten, NULL);
    if (dwBytesWritten != sizeof(RGBQUAD) * 128)
    {
        ::free(pData);
        return FALSE;
    }

    WriteFile(hFile, pData, bih.biSizeImage, &dwBytesWritten, NULL);
    ::free(pData);
    if (dwBytesWritten != bih.biSizeImage)
        return FALSE;

    // Close file
    CloseHandle(hFile);

    return TRUE;
}

BOOL Emulator_SaveScreenshot(LPCTSTR sFileName)
{
    DWORD * bits = (DWORD *) ::malloc(512 * 312 * 4);

    Emulator_PrepareScreenRGB32(bits);

    BOOL result = Emulator_SaveScreenshot(sFileName, bits, Emulator_GetPalette());

    ::free(bits);

    return result;
}

// Returns: amount of different pixels
int Emulator_CompareScreens(const DWORD * scr1, const DWORD * scr2)
{
    const DWORD * p1 = scr1;
    const DWORD * p2 = scr2;

    int result = 0;
    for (int i = 512 * 312; i > 0; i--)
    {
        if (*p1 != *p2)
            result++;
        p1++;  p2++;
    }

    return result;
}

// Returns: amount of different pixels
int Emulator_CheckScreenshot(LPCTSTR sFileName, const DWORD * bits, const DWORD * palette, DWORD * tempbits)
{
    ASSERT(sFileName != NULL);
    ASSERT(bits != NULL);

    // Open file for reading
    HANDLE hFile = ::CreateFile(sFileName,
            GENERIC_READ, FILE_SHARE_READ, NULL,
            OPEN_EXISTING, 0, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
        return -1;

    BITMAPFILEHEADER hdr;
    BITMAPINFOHEADER bih;
    DWORD dwBytesRead = 0;

    ReadFile(hFile, &hdr, sizeof(BITMAPFILEHEADER), &dwBytesRead, NULL);
    if (dwBytesRead != sizeof(BITMAPFILEHEADER))
        return -1;
    //TODO: Check the header
    ReadFile(hFile, &bih, sizeof(BITMAPINFOHEADER), &dwBytesRead, NULL);
    if (dwBytesRead != sizeof(BITMAPINFOHEADER))
        return -1;
    //TODO: Check the header
    if (bih.biSizeImage != 512 * 312)
        return -1;
    // Skip the palette
    SetFilePointer(hFile, sizeof(RGBQUAD) * 256, 0, FILE_CURRENT);

    BYTE * pData = (BYTE *) ::malloc(bih.biSizeImage);

    ReadFile(hFile, pData, bih.biSizeImage, &dwBytesRead, NULL);
    if (dwBytesRead != bih.biSizeImage)
    {
        ::free(pData);
        return -1;
    }

    // Decode the image data
    BYTE * psrc = pData;
    DWORD * pdst = tempbits;
    for (int i = 0; i < 512 * 312; i++)
    {
        BYTE color = *psrc;
        psrc++;
        *pdst = palette[color];
        pdst++;
    }

    ::free(pData);

    // Compare the screenshots
    int result = Emulator_CompareScreens(bits, tempbits);

    // Close file
    CloseHandle(hFile);

    return result;
}

void Emulator_PrepareDiffScreenshot(const DWORD * scr1, const DWORD * scr2, DWORD * dest)
{
    const DWORD * p1 = scr1;
    const DWORD * p2 = scr2;
    DWORD * pdest = dest;
    for (int i = 512 * 312; i > 0; i--)
    {
        if (*p1 == *p2)
            *pdest = *p1;
        else
            *pdest = 0x00003f | (*p1 & 0x00ff00) | (*p2 & 0xff0000);
        p1++;  p2++;  pdest++;
    }
}

int Emulator_CheckScreenshot(LPCTSTR sFileName)
{
    DWORD * bits = (DWORD *) ::malloc(512 * 312 * 4);
    DWORD * tempbits = (DWORD *) ::malloc(512 * 312 * 4);

    Emulator_PrepareScreenRGB32(bits);

    const DWORD * palette = Emulator_GetPalette();
    int result = Emulator_CheckScreenshot(sFileName, bits, palette, tempbits);
    if (result != 0)
    {
        DWORD * diffbits = (DWORD *) ::malloc(512 * 312 * 4);
        Emulator_PrepareDiffScreenshot(bits, tempbits, diffbits);

        // Make diff fike name like "diff" + file name without a path from sFileName
        TCHAR sDiffFileName[MAX_PATH];
        const TCHAR * sSubFileName = _tcsrchr(sFileName, _T('\\'));
        sSubFileName = (sSubFileName == NULL) ? sFileName : sSubFileName + 1;
        wsprintf(sDiffFileName, _T("diff_%s"), sSubFileName);

        Emulator_SaveScreenshot(sDiffFileName, diffbits, Emulator_DiffPalette);

        ::free(diffbits);
    }

    ::free(tempbits);
    ::free(bits);

    return result;
}

void Emulator_KeyboardPressRelease(BYTE scan, int timeout)
{
    g_pBoard->KeyboardEvent(scan, TRUE);
    Emulator_Run(timeout);
    g_pBoard->KeyboardEvent(scan, FALSE);
}

void Emulator_KeyboardPressReleaseChar(char ch, int timeout)
{
    Emulator_KeyboardPressRelease(ch, timeout); //STUB
}

void Emulator_KeyboardSequence(const char * str, int timeout)
{
    const char * p = str;
    while (*p != 0)
    {
        Emulator_KeyboardPressReleaseChar(*p, timeout);
        p++;
        if (*p != 0)
            Emulator_Run(5);  // Pause between keys
    }
}


//////////////////////////////////////////////////////////////////////
//
// Emulator image format - see CMotherboard::SaveToImage()
// Image header format (32 bytes):
//   4 bytes        NEMIGAIMAGE_HEADER1
//   4 bytes        NEMIGAIMAGE_HEADER2
//   4 bytes        NEMIGAIMAGE_VERSION
//   4 bytes        NEMIGAIMAGE_SIZE
//   4 bytes        NEMIGA uptime
//   12 bytes       Not used

BOOL Emulator_SaveImage(LPCTSTR sFilePath)
{
    // Create file
    FILE* fpFile = ::_tfsopen(sFilePath, _T("w+b"), _SH_DENYWR);
    if (fpFile == NULL)
        return false;

    // Allocate memory
    BYTE* pImage = (BYTE*) ::malloc(NEMIGAIMAGE_SIZE);
    if (pImage == NULL)
    {
        ::fclose(fpFile);
        return false;
    }
    memset(pImage, 0, NEMIGAIMAGE_SIZE);
    // Prepare header
    DWORD* pHeader = (DWORD*) pImage;
    *pHeader++ = NEMIGAIMAGE_HEADER1;
    *pHeader++ = NEMIGAIMAGE_HEADER2;
    *pHeader++ = NEMIGAIMAGE_VERSION;
    *pHeader++ = NEMIGAIMAGE_SIZE;
    // Store emulator state to the image
    g_pBoard->SaveToImage(pImage);
    *(DWORD*)(pImage + 16) = m_dwEmulatorUptime;

    // Save image to the file
    size_t dwBytesWritten = ::fwrite(pImage, 1, NEMIGAIMAGE_SIZE, fpFile);
    ::free(pImage);
    ::fclose(fpFile);
    if (dwBytesWritten != NEMIGAIMAGE_SIZE)
        return false;

    return TRUE;
}

BOOL Emulator_LoadImage(LPCTSTR sFilePath)
{
    Emulator_Stop();

    // Open file
    FILE* fpFile = ::_tfsopen(sFilePath, _T("rb"), _SH_DENYWR);
    if (fpFile == NULL)
        return false;

    // Read header
    DWORD bufHeader[NEMIGAIMAGE_HEADER_SIZE / sizeof(DWORD)];
    DWORD dwBytesRead = ::fread(bufHeader, 1, NEMIGAIMAGE_HEADER_SIZE, fpFile);
    if (dwBytesRead != NEMIGAIMAGE_HEADER_SIZE)
    {
        ::fclose(fpFile);
        return false;
    }

    //TODO: Check version and size

    // Allocate memory
    BYTE* pImage = (BYTE*) ::malloc(NEMIGAIMAGE_SIZE);
    if (pImage == NULL)
    {
        ::fclose(fpFile);
        return false;
    }

    // Read image
    ::fseek(fpFile, 0, SEEK_SET);
    dwBytesRead = ::fread(pImage, 1, NEMIGAIMAGE_SIZE, fpFile);
    if (dwBytesRead != NEMIGAIMAGE_SIZE)
    {
        ::free(pImage);
        ::fclose(fpFile);
        return false;
    }

    // Restore emulator state from the image
    g_pBoard->LoadFromImage(pImage);

    m_dwEmulatorUptime = *(DWORD*)(pImage + 16);

    // Free memory, close file
    ::free(pImage);
    ::fclose(fpFile);

    return TRUE;
}


//////////////////////////////////////////////////////////////////////
