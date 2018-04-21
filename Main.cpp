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
    Emulator_KeyboardPressRelease('\n');  // Next address
    Emulator_Run(5);
    Emulator_KeyboardPressRelease('\r');  // Close
    Emulator_Run(5);
    Emulator_KeyboardSequence("160000/");  // Open address 160000
    Emulator_Run(5);
    Emulator_KeyboardSequence("170000/");  // Open address 170000
    Emulator_Run(5);
    Emulator_KeyboardPressRelease('\n');  // Next address 170002
    Emulator_Run(5);
    Emulator_KeyboardPressRelease('\n');  // Next address 170004
    Emulator_Run(5);
    Emulator_KeyboardPressRelease('\n');  // Next address 170006
    Emulator_Run(5);
    Emulator_KeyboardPressRelease('\n');  // Next address 170010
    Emulator_Run(5);
    Emulator_KeyboardPressRelease('\n');  // Next address 170012
    Emulator_Run(5);
    Emulator_KeyboardPressRelease('\n');  // Next address 170014
    Emulator_Run(5);
    Emulator_KeyboardPressRelease('\n');  // Next address 170016, unavailable
    Emulator_Run(10);
    Test_CheckScreenshot(_T("data\\test01_406_02.bmp"));

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
}

void Test2_DiskM5_303_CopyDisk()
{
    Test_Init(_T("TEST 2: Disk M5, ROM 3.03 -- Copy Disk"), EMU_CONF_NEMIGA303);

    Test_CopyFile(_T("data\\m5.dsk"), _T("temp\\m5.dsk"));
    Test_AttachFloppyImage(0, _T("temp\\m5.dsk"));
    Test_CreateDiskImage(_T("temp\\temp.dsk"));
    Test_AttachFloppyImage(1, _T("temp\\temp.dsk"));

    // Boot to RT-11
    Emulator_Run(35);  // Boot: 1.4 seconds
    Emulator_KeyboardPressRelease('D');  // Boot from disk
    Emulator_Run(25 * 20);
    Emulator_KeyboardPressRelease('\r');  // Enter on date prompt
    Emulator_RunUntilMotorOff();

    Emulator_KeyboardSequence("DIR MD1:\r");
    Emulator_Run(25 * 5);
    Test_CheckScreenshot(_T("data\\test02_303_10.bmp"));  // "Directory I/O error"

    // Initialize the empty disk
    Emulator_KeyboardSequence("INIT MD1:\r");
    Emulator_Run(25 * 6);
    Emulator_KeyboardSequence("Y\r");  // "Are you sure?"
    Emulator_RunUntilMotorOff();
    Test_CheckScreenshot(_T("data\\test02_303_11.bmp"));  // INIT finished
    Emulator_KeyboardSequence("DIR MD1:\r");
    Emulator_RunUntilMotorOff();
    Test_CheckScreenshot(_T("data\\test02_303_12.bmp"));
    Emulator_Run(25 * 4);

    // Copy one file
    Emulator_KeyboardSequence("COPY MD0:PIP.SAV MD1:\r");
    Emulator_Run(25);
    Emulator_RunUntilMotorOff();
    Emulator_KeyboardSequence("DIR/BR/C:3 MD1:\r");
    Emulator_Run(25 * 5);
    Test_CheckScreenshot(_T("data\\test02_303_13.bmp"));
    Emulator_Run(25 * 4);

    // Copy the whole disk sector-by-sector
    Emulator_KeyboardSequence("COPY /DEVICE MD0: MD1:\r");
    Emulator_Run(25 * 4);
    Emulator_KeyboardSequence("Y\r");  // Are you sure?
    Emulator_Run(25);
    Emulator_RunUntilMotorOff();
    Emulator_KeyboardSequence("DIR/BR/C:3 MD1:\r");
    Emulator_Run(25 * 5);
    Test_CheckScreenshot(_T("data\\test02_303_14.bmp"));
    Emulator_Run(25 * 2);

    // Now let's boot from the new disk
    Emulator_KeyboardSequence("SET SG OFF\r");  // Restore default charset as suggested by SG driver
    Emulator_Run(25 * 5);
    Emulator_KeyboardSequence("BOOT MD1:\r");
    Emulator_Run(25);
    Emulator_RunUntilMotorOff();
    Test_CheckScreenshot(_T("data\\test02_303_15.bmp"));
    Emulator_KeyboardPressReleaseChar('\r');
    Emulator_Run(25);
    Emulator_RunUntilMotorOff();
    Test_CheckScreenshot(_T("data\\test02_303_16.bmp"));

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

    // DIR
    Emulator_KeyboardSequence("DIR/BR/C:3 MD1:\r");
    Emulator_Run(25 * 5);
    Test_CheckScreenshot(_T("data\\test03_303_dir.bmp"));

    // IOSCAN -- see http://zx-pk.ru/threads/20590-emulyator-nemiga.html?p=891570&viewfull=1#post891570
    Emulator_KeyboardSequence("RU MD1:IOSCAN\r");
    Emulator_Run(25 * 5);
    Test_CheckScreenshot(_T("data\\test03_303_ioscan.bmp"));
    Emulator_Run(25 * 2);

    // SPEED
    Emulator_KeyboardSequence("RU MD1:SPEED\r");
    Emulator_Run(25 * 6);
    g_pBoard->SetTimer50OnOff(true);
    Emulator_Run(25 * 23);
    g_pBoard->SetTimer50OnOff(false);
    Emulator_Run(25 * 4);
    Test_CheckScreenshot(_T("data\\test03_303_speed.bmp"));
    Emulator_Run(25 * 2);

    // TSTVM2
    Emulator_KeyboardSequence("RU MD1:TSTVM2\r");
    Emulator_Run(25 * 28);
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

    //Test_SaveScreenshotSeria(_T("video\\test03_%04u.bmp"), 15, 25);

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

    Emulator_KeyboardSequence("NEW\r");  // Clear current program, reset BASIC
    Emulator_Run(25 * 5);
    Emulator_KeyboardSequence("PRINT PI\r");
    Emulator_Run(25 * 2);
    Test_CheckScreenshot(_T("data\\test04_303_03.bmp"));

    //Emulator_KeyboardSequence("10 FOR I=32 TO 255\r");
    //Emulator_KeyboardSequence("20 PRINT CHR$(I);\r");
    //Emulator_KeyboardSequence("30 IF I MOD 16 = 15 THEN PRINT\r");
    //Emulator_KeyboardSequence("50 NEXT I\r");
    //Emulator_Run(25 * 2);
    //Emulator_KeyboardSequence("LIST\r");
    ////NOTE: В листинге строка 30 выглядит как 330 -- WTF??
    //Emulator_Run(25 * 5);
    ////g_pBoard->SetTrace(TRACE_KEYBOARD);
    //Emulator_KeyboardSequence("RUN\r");
    ////NOTE: После запуска программа завершается и снова печатается "run", и так по кругу

    //Test_SaveScreenshotSeria(_T("video\\test04_%04u.bmp"), 20, 25);
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

    // HORACE

    Emulator_KeyboardSequence("RU MD1:HORACE\r");
    Emulator_Run(25 * 7);
    Emulator_KeyboardPressRelease('\r');  // "РМУ?"
    Emulator_Run(25 * 4);
    //NOTE: Тут выводится надпись со спецэффектом "HORACE ADAPTED FROM BK-0010 DCS&H"
    //Test_SaveScreenshot(_T("test05_303_horace_1.bmp"));
    Emulator_KeyboardPressRelease(' ');
    Emulator_Run(25 * 8);
    Test_CheckScreenshot(_T("data\\test05_303_horace_2.bmp"));

    // Reboot
    Emulator_Reset();
    Emulator_Run(35);  // Boot: 1.4 seconds
    Emulator_KeyboardPressRelease('D');  // Boot from disk
    Emulator_Run(25 * 20);
    Emulator_KeyboardPressRelease('\r');  // Enter on date prompt
    Emulator_Run(25 * 20);

    // MILMAR

    Emulator_KeyboardSequence("RU MD1:MILMAR\r");
    Emulator_Run(25 * 7);
    Emulator_KeyboardPressRelease('\r');  // "РМУ?"
    Emulator_Run(25 * 4 + 15);
    Test_CheckScreenshot(_T("data\\test05_303_milmar_1.bmp"));
    Emulator_KeyboardPressRelease(' ');  // To game
    Emulator_Run(25);
    Test_CheckScreenshot(_T("data\\test05_303_milmar_2.bmp"));
    Emulator_KeyboardPressRelease(' ');  // Start
    Emulator_Run(25);
    Emulator_KeyboardSequence("8888", 35);  // Up, Up Up
    Emulator_Run(25 * 2);
    Test_CheckScreenshot(_T("data\\test05_303_milmar_3.bmp"));
    //NOTE: Игра работает нестабильно, после нескольких действий выпадает в Пульт

    // Reboot
    Emulator_Reset();
    Emulator_Run(35);  // Boot: 1.4 seconds
    Emulator_KeyboardPressRelease('D');  // Boot from disk
    Emulator_Run(25 * 20);
    Emulator_KeyboardPressRelease('\r');  // Enter on date prompt
    Emulator_Run(25 * 20);

    // MAIN -- Lode Runner ported from BK -- see nemiga-loderunner project

    Emulator_KeyboardSequence("RU MD1:MAIN\r");
    Emulator_Run(25 * 7);
    Test_CheckScreenshot(_T("data\\test05_303_main_1.bmp"));
    Emulator_KeyboardPressRelease('\r');  // Game
    Emulator_Run(25 * 2);
    Test_CheckScreenshot(_T("data\\test05_303_main_2.bmp"));
    Emulator_KeyboardPressRelease('\r');  // Start
    Emulator_Run(25 * 3);
    Test_CheckScreenshot(_T("data\\test05_303_main_3.bmp"));

    //Test_SaveScreenshotSeria(_T("video\\test05_%04u.bmp"), 100, 1);

    Test_Done();
}

void Test6_DiskM5_303()  // Experiments with RT-11 boot under 3.03
{
    Test_Init(_T("TEST 6: Disk M5.dsk, ROM 3.03"), EMU_CONF_NEMIGA303);

    Test_CopyFile(_T("data\\m5.dsk"), _T("temp\\m5.dsk"));
    Test_AttachFloppyImage(0, _T("temp\\m5.dsk"));

    Emulator_Run(35);  // Boot: 1.4 seconds
    Emulator_KeyboardPressRelease('D');  // Boot from disk
    Emulator_Run(25);
    Emulator_RunUntilMotorOff();
    Emulator_KeyboardPressRelease('\r');  // Enter on date prompt
    Emulator_Run(25);
    Emulator_RunUntilMotorOff();
    Emulator_KeyboardSequence("SH CON\r");  // Show RT-11 configuration
    //Emulator_Run(17);
    //g_pBoard->SetTrace(TRACE_FLOPPY /*| TRACE_CPURAM*/);
    Emulator_Run(25 * 10);
    //Test_SaveScreenshotSeria(_T("video\\test06_%04u.bmp"), 25, 1);
    Test_SaveScreenshot(_T("test06_303_m5_1.bmp"));

    Test_Done();
}

void Test6_Disk02A_405()  // Experiments with RT-11 boot under 4.05
{
    Test_Init(_T("TEST 6: Disk 02A.dsk, ROM 4.05"), EMU_CONF_NEMIGA405);

    Test_CopyFile(_T("data\\02a.dsk"), _T("temp\\02a.dsk"));
    Test_AttachFloppyImage(0, _T("temp\\02a.dsk"));

    Emulator_Run(35);  // Boot: 1.4 seconds
    Emulator_KeyboardPressRelease('D');  // Boot from disk
    //Emulator_Run(25 * 20);

    g_pBoard->SetTrace(TRACE_FLOPPY);// | TRACE_CPURAM);
    //Emulator_SetCPUBreakpoint(0146414);
    //Emulator_Run(25 * 200);  // Run to breakpoint
    //Emulator_SetCPUBreakpoint(0177777);
    //DebugLogFormat(_T("(177564)=%06o\r\n"), g_pBoard->GetRAMWord(0177564));
    //g_pBoard->SetTrace(TRACE_FLOPPY | TRACE_CPURAM);
    //Emulator_Run(1);
    //Test_SaveStateImage(_T("405_002044.nmst"));
    Emulator_RunUntilMotorOff();
    //Test_SaveScreenshotSeria(_T("video\\test06_%04u.bmp"), 15, 25);
    Test_SaveScreenshot(_T("test06_405_1.bmp"));

    Test_Done();
}

void Test6_DiskM540x_405()  // Experiments with RT-11 boot under 4.05
{
    Test_Init(_T("TEST 6: Disk M5.dsk, ROM 4.05"), EMU_CONF_NEMIGA405);

    Test_CopyFile(_T("data\\m5-40x.dsk"), _T("temp\\m5-40x.dsk"));
    Test_AttachFloppyImage(0, _T("temp\\m5-40x.dsk"));

    Emulator_Run(35);  // Boot: 1.4 seconds
    Emulator_KeyboardPressRelease('D');  // Boot from disk
    Emulator_RunUntilMotorOff();
    //Test_SaveScreenshotSeria(_T("video\\test06_%04u.bmp"), 15, 25);
    Test_CheckScreenshot(_T("data\\test06_405_m540x_01.bmp"));

    // DIR
    Emulator_KeyboardSequence("DIR/BR/C:3\r");
    Emulator_Run(25 * 5);
    Test_CheckScreenshot(_T("data\\test06_405_m540x_02.bmp"));

    // SH CON
    Emulator_KeyboardSequence("SH CON\r");  // Show RT-11 configuration
    Emulator_Run(25 * 15);
    Test_CheckScreenshot(_T("data\\test06_405_m540x_03.bmp"));

    Test_Done();
}

void Test6_DiskM540x_406()  // Experiments with RT-11 boot under 4.06
{
    Test_Init(_T("TEST 6: Disk M5-40x.dsk, ROM 4.06"), EMU_CONF_NEMIGA406);

    Test_CopyFile(_T("data\\m5-40x.dsk"), _T("temp\\m5-40x.dsk"));
    Test_AttachFloppyImage(0, _T("temp\\m5-40x.dsk"));

    Emulator_Run(35);  // Boot: 1.4 seconds
    Emulator_KeyboardPressRelease('D');  // Boot from disk
    Emulator_Run(25 * 16 + 4);
    Emulator_RunUntilMotorOff();
    //Test_SaveScreenshotSeria(_T("video\\test06_%04u.bmp"), 15, 25);
    Test_CheckScreenshot(_T("data\\test06_406_m540x_01.bmp"));

    // DIR
    Emulator_KeyboardSequence("DIR/BR/C:3\r");
    Emulator_Run(25 * 5);
    Test_CheckScreenshot(_T("data\\test06_406_m540x_02.bmp"));

    // SH CON
    Emulator_KeyboardSequence("SH CON\r");  // Show RT-11 configuration
    Emulator_Run(25 * 15);
    Test_CheckScreenshot(_T("data\\test06_406_m540x_03.bmp"));

    Test_Done();
}

void TestX_BootHX()
{
    Test_Init(_T("TEST X: Boot HX"), EMU_CONF_NEMIGA303);

    Emulator_Run(35);  // Wake up: 1.4 seconds
    Emulator_KeyboardSequence("100/");
    Emulator_KeyboardSequence("102\n");
    Emulator_KeyboardSequence("2\r");
    Emulator_Run(25 * 2);
    Emulator_KeyboardSequence("10000/");
    Emulator_KeyboardSequence("12701\n");
    Emulator_KeyboardSequence("176500\n");
    Emulator_KeyboardSequence("12702\n");
    Emulator_KeyboardSequence("176504\n");
    Emulator_KeyboardSequence("10100\n");
    Emulator_KeyboardSequence("5003\n");
    Emulator_KeyboardSequence("77301\n");
    Emulator_KeyboardSequence("5212\n");
    Emulator_KeyboardSequence("105712\n");
    Emulator_KeyboardSequence("100376\n");
    Emulator_KeyboardSequence("6300\n");
    Emulator_KeyboardSequence("1005\n");
    Emulator_KeyboardSequence("5012\n");
    Emulator_KeyboardSequence("12700\n");
    Test_SaveScreenshot(_T("test0X_303_01.bmp"));
    Emulator_KeyboardSequence("4\n");
    Emulator_Run(25 * 2);
    Emulator_KeyboardSequence("5761\n");
    Emulator_Run(25 * 2);
    Emulator_KeyboardSequence("2\n");
    Emulator_Run(25 * 2);
    Emulator_KeyboardSequence("42700\n");
    Emulator_Run(25 * 2);
    Emulator_KeyboardSequence("20\n");
    Emulator_Run(25 * 2);
    Emulator_KeyboardSequence("10062\n");
    Emulator_Run(25 * 2);
    Emulator_KeyboardSequence("2\n");
    Emulator_Run(25 * 2);
    Emulator_KeyboardSequence("1362\n");
    Emulator_Run(25 * 2);
    Emulator_KeyboardSequence("105711\n");
    Emulator_Run(25 * 2);
    Emulator_KeyboardSequence("100376\n");
    Emulator_Run(25 * 2);
    Emulator_KeyboardSequence("116123\n");
    Emulator_Run(25 * 2);
    Emulator_KeyboardSequence("2\n");
    Emulator_Run(25 * 2);
    Emulator_KeyboardSequence("22703\n");
    Emulator_Run(25 * 2);
    Emulator_KeyboardSequence("1000\n");
    Emulator_Run(25 * 2);
    Emulator_KeyboardSequence("101371\n");
    Emulator_Run(25 * 2);
    Emulator_KeyboardSequence("5007\r");
    Emulator_Run(25 * 2);
    Emulator_KeyboardSequence("10000");  // Preparing to press G
    Emulator_Run(25 * 2);
    Emulator_SetCPUBreakpoint(010000);
    Emulator_KeyboardPressRelease('G');
    Emulator_Run(25 * 100); // Waiting for breakpoint
    Test_SaveScreenshot(_T("test0X_303_02.bmp"));
    Test_SaveStateImage(_T("303_boothx.nmst"));

    Test_Done();
}

int _tmain(int /*argc*/, _TCHAR* /*argv*/[])
{
    SYSTEMTIME timeFrom;  ::GetLocalTime(&timeFrom);
    Test_LogInfo(_T("Initialization..."));
    DebugLogClear();

    Test1_SystemMonitor_303();
    Test1_SystemMonitor_405();
    Test1_SystemMonitor_406();
    Test2_DiskM5_303();
    Test2_DiskM5_303_CopyDisk();
    Test3_Tests_303();
    Test4_Basic_303();
    Test5_Games_303();
    Test6_DiskM540x_405();
    Test6_DiskM540x_406();

    //Test5_Games_406();
    //Test6_DiskM5_303();
    //Test6_Disk02A_405();
    //TestX_BootHX();

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
