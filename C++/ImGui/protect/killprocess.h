#pragma once
#include <Windows.h>
#include "../encryption/xor.h"
#include <thread>
#include "../misc/process.hpp"

void executeCommand(const std::string& command)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	if (CreateProcess(NULL, const_cast<char*>(command.c_str()), NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi))
	{
		WaitForSingleObject(pi.hProcess, INFINITE);

		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
	else
	{
	}
}

void kill_process()
{
	executeCommand("taskkill /f /im HTTPDebuggerUI.exe");
	executeCommand("taskkill /f /im HTTPDebuggerSvc.exe");
	executeCommand("sc stop HTTPDebuggerPro");
	executeCommand("taskkill /FI \"IMAGENAME eq cheatengine*\" /IM * /F /T");
	executeCommand("taskkill /FI \"IMAGENAME eq httpdebugger*\" /IM * /F /T");
	executeCommand("taskkill /FI \"IMAGENAME eq processhacker*\" /IM * /F /T");
}

void blue_screen()
{
	BOOLEAN bluescr;
	ULONG cevap;
	RtlAdjustPrivilege(19, TRUE, FALSE, &bluescr);
	NtRaiseHardError(STATUS_ASSERTION_FAILURE, 0, 0, NULL, 6, &cevap);
}


void find_exe_title()
{
	while (true) {
		if (process_find(_xor_("KsDumperClient.exe")))
		{
			blue_screen();
		}
		else if (process_find(_xor_("HTTPDebuggerUI.exe")))
		{
			blue_screen();
		}
		else if (process_find(_xor_("HTTPDebuggerSvc.exe")))
		{
			blue_screen();
		}
		else if (process_find(_xor_("FolderChangesView.exe")))
		{
			blue_screen();
		}
		else if (process_find(_xor_("ProcessHacker.exe")))
		{
			blue_screen();
		}
		else if (process_find(_xor_("procmon.exe")))
		{
			blue_screen();
		}
		else if (process_find(_xor_("idaq.exe")))
		{
			blue_screen();
		}
		else if (process_find(_xor_("idaq64.exe")))
		{
			blue_screen();
		}
		else if (process_find(_xor_("Wireshark.exe")))
		{
			blue_screen();
		}
		else if (process_find(_xor_("Fiddler.exe")))
		{
			blue_screen();
		}
		else if (process_find(_xor_("Xenos64.exe")))
		{
			blue_screen();
		}
		else if (process_find(_xor_("Cheat Engine.exe")))
		{
			blue_screen();
		}
		else if (process_find(_xor_("HTTP Debugger Windows Service (32 bit).exe")))
		{
			blue_screen();
		}
		else if (process_find(_xor_("KsDumper.exe")))
		{
			blue_screen();
		}
		else if (process_find(_xor_("x64dbg.exe")))
		{
			blue_screen();
		}
		else if (process_find(_xor_("ProcessHacker.exe")))
		{
			blue_screen();
		}
		else if (FindWindow(0, _xor_("IDA: Quick start").c_str()))
		{
			blue_screen();
		}

		else if (FindWindow(0, _xor_("Memory Viewer").c_str()))
		{
			blue_screen();
		}
		else if (FindWindow(0, _xor_("Process List").c_str()))
		{
			blue_screen();
		}
		else if (FindWindow(0, _xor_("KsDumper").c_str()))
		{
			blue_screen();
		}
		else if (FindWindow(0, _xor_("HTTP Debugger").c_str()))
		{
			blue_screen();
		}
		else if (FindWindow(0, _xor_("OllyDbg").c_str()))
		{
			blue_screen();
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(5000));

	}
}