#pragma once
#include "include.h"

void salvarStringsMemoria(DWORD processId)
{
    if (!SetPrivilege(SE_DEBUG_NAME, TRUE))
    {
        std::cout << "Falha ao elevar os privilégios do programa." << std::endl;
        return;
    }

    HANDLE processHandle = OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, FALSE, processId);
    if (processHandle == NULL)
    {
        std::cout << "Falha ao abrir o processo com PID: " << processId << std::endl;
        return;
    }

    MEMORY_BASIC_INFORMATION memInfo;
    DWORD_PTR baseAddress = 0;
    DWORD_PTR endAddress = 0;
    char buffer[1024];

    while (VirtualQueryEx(processHandle, reinterpret_cast<LPCVOID>(baseAddress), &memInfo, sizeof(memInfo)) != 0)
    {
        if (memInfo.State == MEM_COMMIT && (memInfo.Protect == PAGE_READWRITE || memInfo.Protect == PAGE_READONLY))
        {
            baseAddress = reinterpret_cast<DWORD_PTR>(memInfo.BaseAddress);
            endAddress = baseAddress + memInfo.RegionSize;

            std::string filename = std::to_string(processId) + ".txt";
            std::ofstream file(filename, std::ios::out | std::ios::app);

            for (DWORD_PTR address = baseAddress; address < endAddress; address += sizeof(buffer))
            {
                DWORD oldProtect;
                if (VirtualProtectEx(processHandle, reinterpret_cast<LPVOID>(address), sizeof(buffer), PAGE_READONLY, &oldProtect))
                {
                    SIZE_T bytesRead;
                    if (ReadProcessMemory(processHandle, reinterpret_cast<LPCVOID>(address), buffer, sizeof(buffer), &bytesRead))
                    {
                        std::string data(buffer, bytesRead);
                        file << data;
                    }
                    else
                    {
                        std::cout << "Falha ao ler a memória do processo com PID: " << processId << std::endl;
                        break;
                    }

                    VirtualProtectEx(processHandle, reinterpret_cast<LPVOID>(address), sizeof(buffer), oldProtect, &oldProtect);
                }
                else
                {
                    std::cout << "Falha ao modificar as permissões de acesso à memória do processo com PID: " << processId << std::endl;
                    break;
                }
            }

            file.close();
        }

        baseAddress += memInfo.RegionSize;
    }

    CloseHandle(processHandle);
}