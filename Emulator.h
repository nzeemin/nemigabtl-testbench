/*  This file is part of NEMIGABTL.
    NEMIGABTL is free software: you can redistribute it and/or modify it under the terms
of the GNU Lesser General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.
    NEMIGABTL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU Lesser General Public License for more details.
    You should have received a copy of the GNU Lesser General Public License along with
NEMIGABTL. If not, see <http://www.gnu.org/licenses/>. */

// Emulator.h

#pragma once

#include "emubase\Board.h"
#include "util\BitmapFile.h"

//////////////////////////////////////////////////////////////////////

enum EmulatorConfiguration
{
    EMU_CONF_NEMIGA303 = 303,
    EMU_CONF_NEMIGA405 = 405,
    EMU_CONF_NEMIGA406 = 406,
};

//////////////////////////////////////////////////////////////////////


extern CMotherboard* g_pBoard;

extern int g_nEmulatorConfiguration;  // Current configuration
extern BOOL g_okEmulatorRunning;


//////////////////////////////////////////////////////////////////////


BOOL Emulator_Init();
BOOL Emulator_InitConfiguration(WORD configuration);
void Emulator_Done();
void Emulator_SetCPUBreakpoint(WORD address);
BOOL Emulator_IsBreakpoint();
void Emulator_Start();
void Emulator_Stop();
void Emulator_Reset();
int  Emulator_SystemFrame();
DWORD Emulator_GetUptime();  // NEMIGA uptime, in seconds

void Emulator_PrepareScreenRGB32(void* pImageBits);

BOOL Emulator_AttachFloppyImage(int slot, LPCTSTR sFilePath);
void Emulator_DetachFloppyImage(int slot);

BOOL Emulator_Run(int frames);
BOOL Emulator_RunUntilMotorOff();
BOOL Emulator_SaveScreenshot(LPCTSTR sFileName);
int  Emulator_CheckScreenshot(LPCTSTR sFileName);
void Emulator_KeyboardPressRelease(BYTE scan, int timeout = 5);
void Emulator_KeyboardPressReleaseChar(char ch, int timeout = 5);
void Emulator_KeyboardSequence(const char * str, int timeout = 5);

BOOL Emulator_SaveImage(LPCTSTR sFilePath);
BOOL Emulator_LoadImage(LPCTSTR sFilePath);


//////////////////////////////////////////////////////////////////////
