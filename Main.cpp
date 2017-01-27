/*  This file is part of NEMIGABTL.
    NEMIGABTL is free software: you can redistribute it and/or modify it under the terms
of the GNU Lesser General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.
    NEMIGABTL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU Lesser General Public License for more details.
    You should have received a copy of the GNU Lesser General Public License along with
NEMIGABTL. If not, see <http://www.gnu.org/licenses/>. */

// Main.cpp

#include "stdafx.h"
#include "Emulator.h"
#include "emubase\\Emubase.h"

void Test1_SystemMonitor_303()
{
    Test_Init(_T("TEST 1: System Monitor 3.03"), EMU_CONF_NEMIGA303);

    Emulator_Run(35);  // Boot: 1.4 seconds
    Test_CheckScreenshot(_T("data\\test01_303_01.bmp"));
    Emulator_KeyboardPressRelease('S');  // Show registers
    Emulator_Run(5);
    Emulator_KeyboardPressRelease('/');  // Open address 000000
    Emulator_Run(5);
    Emulator_KeyboardPressRelease('\n');  // Next address
    Emulator_Run(5);
    Emulator_KeyboardPressRelease('\r');  // Close
    Emulator_Run(5);
    Emulator_KeyboardSequence("160000/");  // Open address 160000
    Emulator_Run(5);
    Emulator_KeyboardPressRelease('\n');  // Next address
    Emulator_Run(5);
    Emulator_KeyboardPressRelease('\r');  // Close
    Emulator_Run(5);
    Emulator_KeyboardSequence("177776/");  // Open address 177776
    Emulator_Run(5);
    Emulator_KeyboardPressRelease('\n');  // Next address
    Emulator_Run(5);
    Emulator_KeyboardPressRelease('\r');  // Close
    Emulator_Run(5);
    Test_CheckScreenshot(_T("data\\test01_303_02.bmp"));

    Test_Done();
}
void Test1_SystemMonitor_405()
{
    Test_Init(_T("TEST 1: System Monitor 4.05"), EMU_CONF_NEMIGA405);

    Emulator_Run(35);  // Boot: 1.4 seconds
    Test_CheckScreenshot(_T("data\\test01_405_01.bmp"));
    Emulator_KeyboardPressRelease('S');  // Show registers
    Emulator_Run(5);
    Emulator_KeyboardPressRelease('/');  // Open address 000000
    Emulator_Run(5);
    Emulator_KeyboardPressRelease('\n');  // Next address
    Emulator_Run(5);
    Emulator_KeyboardPressRelease('\r');  // Close
    Emulator_Run(5);
    Emulator_KeyboardSequence("160000/");  // Open address 160000
    Emulator_Run(5);
    Emulator_KeyboardPressRelease('\n');  // Next address
    Emulator_Run(5);
    Emulator_KeyboardPressRelease('\r');  // Close
    Emulator_Run(5);
    Emulator_KeyboardSequence("177776/");  // Open address 177776
    Emulator_Run(5);
    Emulator_KeyboardPressRelease('\n');  // Next address
    Emulator_Run(5);
    Emulator_KeyboardPressRelease('\r');  // Close
    Emulator_Run(5);
    Test_CheckScreenshot(_T("data\\test01_405_02.bmp"));

    Test_Done();
}
void Test1_SystemMonitor_406()
{
    Test_Init(_T("TEST 1: System Monitor 4.06"), EMU_CONF_NEMIGA406);

    Emulator_Run(35);  // Boot: 1.4 seconds
    Test_CheckScreenshot(_T("data\\test01_406_01.bmp"));
    Emulator_KeyboardPressRelease('S');  // Show registers
    Emulator_Run(5);
    Emulator_KeyboardPressRelease('/');  // Open address 000000
    Emulator_Run(5);
    Test_CheckScreenshot(_T("data\\test01_406_02.bmp"));
    //NOTE: В прошивке 4.06 команда пульта '/' (работа с памятью) работает как-то странно --
    //      не позволяет работать с адресами выше адреса, лежащего в 177744, а по умолчанию там лежит 0.

    //Test_SaveScreenshotSeria(_T("video\\test01_%04u.bmp"), 50, 5);
    //g_pBoard->SetTrace(TRACE_CPU);

    Test_Done();
}

void Test2_DiskM5_303()
{
    Test_Init(_T("TEST 2: Disk M5, ROM 3.03"), EMU_CONF_NEMIGA303);

    Test_CopyFile(_T("data\\m5.dsk"), _T("temp\\m5.dsk"));
    Test_AttachFloppyImage(0, _T("temp\\m5.dsk"));

    // Boot to RT-11
    Emulator_Run(35);  // Boot: 1.4 seconds
    Emulator_KeyboardPressRelease('D');  // Boot from disk
    Emulator_Run(25 * 20);
    Emulator_KeyboardPressRelease('\r');  // Enter on date prompt
    Emulator_Run(25 * 20);
    Test_CheckScreenshot(_T("data\\test02_303_01.bmp"));

    // DIR
    Emulator_KeyboardSequence("DIR/BR/C:3\r");
    Emulator_Run(25 * 5);
    Test_CheckScreenshot(_T("data\\test02_303_02.bmp"));

    // SH CON
    Emulator_KeyboardSequence("SH CON\r");  // Show RT-11 configuration
    Emulator_Run(25 * 15);
    Test_CheckScreenshot(_T("data\\test02_303_03.bmp"));

    // SH DEV
    Emulator_KeyboardSequence("SH DEV\r");  // Show RT-11 device handlers
    Emulator_Run(25 * 15);
    Test_CheckScreenshot(_T("data\\test02_303_04.bmp"));

    // SH MEM
    Emulator_KeyboardSequence("SH MEM\r");  // Show memory organization
    Emulator_Run(25 * 15);
    Test_CheckScreenshot(_T("data\\test02_303_05.bmp"));

    // Create empty disk, initialize, copy system disk 1-to-1 to the new disk
    Test_CreateDiskImage(_T("temp\\temp.dsk"));
    Test_AttachFloppyImage(1, _T("temp\\m5.dsk"));
    Emulator_KeyboardSequence("DIR MD1:\r");
    Emulator_Run(25 * 5);
    Test_CheckScreenshot(_T("data\\test02_303_10.bmp"));  // "Directory I/O error"
    Emulator_KeyboardSequence("INIT MD1:\r");
    Emulator_Run(25 * 6);
    Emulator_KeyboardSequence("Y\r");  // "Are you sure?"
    Emulator_Run(25 * 10);
    Test_CheckScreenshot(_T("data\\test02_303_11.bmp"));  // INIT finished
    Emulator_KeyboardSequence("DIR MD1:\r");
    //NOTE: Выводит ошибку "File not found SY:DIR.SAV" -- всё плохо

    //Test_SaveScreenshotSeria(_T("video\\test02_%04u.bmp"), 15, 25);

    Test_Done();
}

void Test3_Tests_303()
{
    Test_Init(_T("TEST 3: Tests, ROM 3.03"), EMU_CONF_NEMIGA303);

    Test_CopyFile(_T("data\\m5.dsk"), _T("temp\\m5.dsk"));
    Test_AttachFloppyImage(0, _T("temp\\m5.dsk"));
    Test_CopyFile(_T("data\\tests.dsk"), _T("temp\\tests.dsk"));
    Test_AttachFloppyImage(1, _T("temp\\tests.dsk"));

    Emulator_Run(35);  // Boot: 1.4 seconds
    Emulator_KeyboardPressRelease('D');  // Boot from disk
    Emulator_Run(25 * 20);
    Emulator_KeyboardPressRelease('\r');  // Enter on date prompt
    Emulator_Run(25 * 20);

    Emulator_KeyboardSequence("DIR/BR/C:3 MD1:\r");
    Emulator_Run(25 * 5);
    Test_CheckScreenshot(_T("data\\test03_303_dir.bmp"));

    Emulator_KeyboardSequence("RU MD1:TSTVM2\r");
    Emulator_Run(25 * 22);
    Test_CheckScreenshot(_T("data\\test03_303_tstvm2_1.bmp"));
    Emulator_KeyboardPressRelease('\r');
    Emulator_Run(25 * 295);
    Test_CheckScreenshot(_T("data\\test03_303_tstvm2_2.bmp"));
    Emulator_KeyboardPressRelease('\r');
    Emulator_Run(25 * 120);
    Test_CheckScreenshot(_T("data\\test03_303_tstvm2_3.bmp"));
    Emulator_KeyboardPressRelease('\r');
    Emulator_Run(25 * 32);
    Test_CheckScreenshot(_T("data\\test03_303_tstvm2_4.bmp"));

    //Test_SaveScreenshotSeria(_T("video\\test03_%04u.bmp"), 30, 25);

    Test_Done();
}

void Test4_Basic_303()
{
    Test_Init(_T("TEST 4: BASIC, ROM 3.03"), EMU_CONF_NEMIGA303);

    Test_CopyFile(_T("data\\m5.dsk"), _T("temp\\m5.dsk"));
    Test_AttachFloppyImage(0, _T("temp\\m5.dsk"));
    Test_CopyFile(_T("data\\basic.dsk"), _T("temp\\basic.dsk"));
    Test_AttachFloppyImage(1, _T("temp\\basic.dsk"));

    Emulator_Run(35);  // Boot: 1.4 seconds
    Emulator_KeyboardPressRelease('D');  // Boot from disk
    Emulator_Run(25 * 20);
    Emulator_KeyboardPressRelease('\r');  // Enter on date prompt
    Emulator_Run(25 * 20);

    Emulator_KeyboardSequence("RU MD1:BASVN\r");
    Emulator_Run(25 * 11);
    Test_CheckScreenshot(_T("data\\test04_303_01.bmp"));

    // BASIC speed test by Sergey Frolov, see http://www.leningrad.su/calc/speed.php
    Emulator_KeyboardSequence("4 FOR I = 1 TO 10\r");
    Emulator_KeyboardSequence("5 A = 1.0000001\r");
    Emulator_KeyboardSequence("10 B = A\r");
    Emulator_KeyboardSequence("15 FOR J = 1 TO 27\r");
    Emulator_KeyboardSequence("20 A = A * A\r");
    Emulator_KeyboardSequence("25 B = B ^ 2.01\r");
    Emulator_KeyboardSequence("30 NEXT J\r");
    Emulator_KeyboardSequence("35 NEXT I\r");
    Emulator_KeyboardSequence("40 PRINT A, B\r");
    Emulator_Run(25 * 2);
    Emulator_KeyboardSequence("RUN\r");
    Emulator_Run(25 * 3);  //NOTE: Тест выполняется слишком быстро, на реале это заняло 8 секунд (на Немиге 4.06)
    Test_CheckScreenshot(_T("data\\test04_303_02.bmp"));
    // Результаты теста совпадают с реалом: http://zx-pk.ru/threads/20590-emulyator-nemiga.html?p=892828&viewfull=1#post892828

    //Emulator_KeyboardSequence("PRINT PI\r");
    //Emulator_Run(25 * 2);
    //Emulator_KeyboardSequence("10 FOR I=32 TO 255\r");
    //Emulator_KeyboardSequence("20 PRINT CHR$(I);\r");
    //Emulator_KeyboardSequence("30 IF I MOD 16 = 15 THEN PRINT\r");
    //Emulator_KeyboardSequence("50 NEXT I\r");
    //Emulator_Run(25 * 2);
    //Emulator_KeyboardSequence("LIST\r");
    //NOTE: В листинге строка 30 выглядит как 330 -- WTF??
    //Emulator_Run(25 * 2);
    //g_pBoard->SetTrace(TRACE_KEYBOARD);
    //Emulator_KeyboardSequence("RUN\r");
    //NOTE: После запуска программа завершается и снова печатается "run", и так по кругу

    //Test_SaveScreenshotSeria(_T("video\\test04_%04u.bmp"), 15, 25);
    //Emulator_SaveImage(_T("test04.nmst"));

    Test_Done();
}

void Test5_Games_303()
{
    Test_Init(_T("TEST 5: Games, ROM 3.03"), EMU_CONF_NEMIGA303);

    Test_CopyFile(_T("data\\m5.dsk"), _T("temp\\m5.dsk"));
    Test_AttachFloppyImage(0, _T("temp\\m5.dsk"));
    Test_CopyFile(_T("data\\games.dsk"), _T("temp\\games.dsk"));
    Test_AttachFloppyImage(1, _T("temp\\games.dsk"));

    Emulator_Run(35);  // Boot: 1.4 seconds
    Emulator_KeyboardPressRelease('D');  // Boot from disk
    Emulator_Run(25 * 20);
    Emulator_KeyboardPressRelease('\r');  // Enter on date prompt
    Emulator_Run(25 * 20);

    Emulator_KeyboardSequence("DIR/BR/C:3 MD1:\r");
    Emulator_Run(25 * 5);
    Test_CheckScreenshot(_T("data\\test05_303_dir.bmp"));

    // KLAD2

    Emulator_KeyboardSequence("RU MD1:KLAD2\r");
    Emulator_Run(25 * 7);
    Emulator_KeyboardPressRelease('\r');  // "РМУ?"
    Emulator_Run(25 * 4);
    Test_CheckScreenshot(_T("data\\test05_303_klad2_1.bmp"));
    Emulator_KeyboardPressRelease('\r');
    Emulator_Run(50);
    Emulator_KeyboardPressRelease('1');  // Speed selection
    Emulator_Run(25 * 4);
    Test_CheckScreenshot(_T("data\\test05_303_klad2_2.bmp"));

    // Reboot
    Emulator_Reset();
    Emulator_Run(35);  // Boot: 1.4 seconds
    Emulator_KeyboardPressRelease('D');  // Boot from disk
    Emulator_Run(25 * 20);
    Emulator_KeyboardPressRelease('\r');  // Enter on date prompt
    Emulator_Run(25 * 20);

    // HORROR

    Emulator_KeyboardSequence("RU MD1:HORROR\r");
    Emulator_Run(25 * 7);
    Emulator_KeyboardPressRelease('\r');  // "РМУ?"
    Emulator_Run(25 * 4);
    Test_CheckScreenshot(_T("data\\test05_303_horror_1.bmp"));
    Emulator_KeyboardPressRelease('\r');  // Select "Game"
    Emulator_Run(50);
    Test_CheckScreenshot(_T("data\\test05_303_horror_2.bmp"));

    // Reboot
    Emulator_Reset();
    Emulator_Run(35);  // Boot: 1.4 seconds
    Emulator_KeyboardPressRelease('D');  // Boot from disk
    Emulator_Run(25 * 20);
    Emulator_KeyboardPressRelease('\r');  // Enter on date prompt
    Emulator_Run(25 * 20);

    // DIG

    Emulator_KeyboardSequence("RU MD1:DIG\r");
    Emulator_Run(25 * 7);
    Emulator_KeyboardPressRelease('\r');  // "РМУ?"
    Emulator_Run(25 * 4);
    Test_CheckScreenshot(_T("data\\test05_303_dig_1.bmp"));
    Emulator_KeyboardPressRelease(' ');
    Emulator_KeyboardSequence("WADS");  // Key mappings
    Emulator_Run(50);
    Emulator_KeyboardPressRelease('1');  // Rang
    Emulator_Run(25 * 4);
    Test_CheckScreenshot(_T("data\\test05_303_dig_2.bmp"));
    //NOTE: Игра не заработала, выводит только часть спрайтов и всё

    // Reboot
    Emulator_Reset();
    Emulator_Run(35);  // Boot: 1.4 seconds
    Emulator_KeyboardPressRelease('D');  // Boot from disk
    Emulator_Run(25 * 20);
    Emulator_KeyboardPressRelease('\r');  // Enter on date prompt
    Emulator_Run(25 * 20);

    //// HORACE

    Emulator_KeyboardSequence("RU MD1:HORACE\r");
    Emulator_Run(25 * 7);
    Emulator_KeyboardPressRelease('\r');  // "РМУ?"
    Emulator_Run(25 * 4);
    //NOTE: Тут выводится надпись со спецэффектом "HORACE ADAPTED FROM BK-0010 DCS&H"
    //Test_SaveScreenshot(_T("test05_303_horace_1.bmp"));
    Emulator_KeyboardPressRelease(' ');
    Emulator_Run(25 * 8);
    Test_CheckScreenshot(_T("data\\test05_303_horace_2.bmp"));

    //Test_SaveScreenshotSeria(_T("video\\test05_%04u.bmp"), 20, 5);

    Test_Done();
}

int _tmain(int /*argc*/, _TCHAR* /*argv*/[])
{
    SYSTEMTIME timeFrom;  ::GetLocalTime(&timeFrom);
    Test_LogInfo(_T("Initialization..."));

    Test1_SystemMonitor_303();
    Test1_SystemMonitor_405();
    Test1_SystemMonitor_406();
    Test2_DiskM5_303();
    Test3_Tests_303();
    Test4_Basic_303();
    Test5_Games_303();

    Test_LogInfo(_T("Finalization..."));

    SYSTEMTIME timeTo;  ::GetLocalTime(&timeTo);

    FILETIME fileTimeTo;
    SystemTimeToFileTime(&timeTo, &fileTimeTo);
    ULARGE_INTEGER ulTimeTo;
    ulTimeTo.LowPart = fileTimeTo.dwLowDateTime;
    ulTimeTo.HighPart = fileTimeTo.dwHighDateTime;

    FILETIME fileTimeFrom;
    SystemTimeToFileTime(&timeFrom, &fileTimeFrom);
    ULARGE_INTEGER ulTimeFrom;
    ulTimeFrom.LowPart = fileTimeFrom.dwLowDateTime;
    ulTimeFrom.HighPart = fileTimeFrom.dwHighDateTime;

    ULONGLONG ulDiff = ulTimeTo.QuadPart - ulTimeFrom.QuadPart;

    float diff = (float)ulDiff;  // number of 100-nanosecond intervals
    Test_LogFormat('i', _T("Time spent: %.3f seconds"), diff / 10000000.0);

    Test_LogSummary();

    return 0;
}
