#pragma once
#include "include.h"

BOOL SetPrivilege(LPCSTR privilegeName, BOOL enable)
{
    HANDLE tokenHandle;
    TOKEN_PRIVILEGES privileges;
    LUID privilegeLUID;

    // Abrir o token de acesso do processo atual
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &tokenHandle))
    {
        std::cout << "Falha ao abrir o token de acesso do processo." << std::endl;
        return FALSE;
    }

    // Obter o LUID (Locally Unique Identifier) do privilégio solicitado
    if (!LookupPrivilegeValue(NULL, privilegeName, &privilegeLUID))
    {
        std::cout << "Falha ao obter o LUID do privilégio." << std::endl;
        CloseHandle(tokenHandle);
        return FALSE;
    }

    // Definir as informações de privilégio
    privileges.PrivilegeCount = 1;
    privileges.Privileges[0].Luid = privilegeLUID;
    privileges.Privileges[0].Attributes = enable ? SE_PRIVILEGE_ENABLED : 0;

    // Solicitar a alteração de privilégios
    if (!AdjustTokenPrivileges(tokenHandle, FALSE, &privileges, sizeof(TOKEN_PRIVILEGES), NULL, NULL))
    {
        std::cout << "Falha ao ajustar os privilégios do token." << std::endl;
        CloseHandle(tokenHandle);
        return FALSE;
    }

    // Verificar se a alteração de privilégios foi bem-sucedida
    if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)
    {
        std::cout << "Não foi possível atribuir todos os privilégios solicitados." << std::endl;
        CloseHandle(tokenHandle);
        return FALSE;
    }

    CloseHandle(tokenHandle);
    return TRUE;
}

std::vector<DWORD> listarPIDs()
{



    if (!SetPrivilege(SE_DEBUG_NAME, TRUE))
    {
        std::cout << "Falha ao elevar os privilégios do programa." << std::endl;
        return {};
    }

    SC_HANDLE scmHandle = OpenSCManager(NULL, NULL, SC_MANAGER_ENUMERATE_SERVICE);
    if (scmHandle == NULL)
    {
        std::cout << "Falha ao abrir o gerenciador de serviços." << std::endl;
        return {};
    }

    DWORD bufferSize = 0;
    DWORD requiredBufferSize = 0;
    DWORD numServices = 0;
    DWORD resumeHandle = 0;

    EnumServicesStatusEx(scmHandle, SC_ENUM_PROCESS_INFO, SERVICE_WIN32, SERVICE_STATE_ALL, NULL, bufferSize, &requiredBufferSize, &numServices, &resumeHandle, NULL);

    if (requiredBufferSize == 0)
    {
        std::cout << "Nenhum serviço encontrado." << std::endl;
        CloseServiceHandle(scmHandle);
        return {};
    }

    bufferSize = requiredBufferSize;
    LPENUM_SERVICE_STATUS_PROCESS servicesBuffer = reinterpret_cast<LPENUM_SERVICE_STATUS_PROCESS>(new char[bufferSize]);

    if (!EnumServicesStatusEx(scmHandle, SC_ENUM_PROCESS_INFO, SERVICE_WIN32, SERVICE_STATE_ALL, reinterpret_cast<LPBYTE>(servicesBuffer), bufferSize, &requiredBufferSize, &numServices, &resumeHandle, NULL))
    {
        std::cout << "Falha ao enumerar os serviços." << std::endl;
        delete[] reinterpret_cast<char*>(servicesBuffer);
        CloseServiceHandle(scmHandle);
        return {};
    }

    std::cout << "Lista de serviços em execução:" << std::endl;

    std::vector<DWORD> pids;

    for (DWORD i = 0; i < numServices; i++)
    {
        std::string serviceName(servicesBuffer[i].lpServiceName);
        std::string displayName(servicesBuffer[i].lpDisplayName);
        DWORD processId = servicesBuffer[i].ServiceStatusProcess.dwProcessId;

        if (serviceName == "DPS" || serviceName == "Dnscache")
        {
            std::cout << "Nome do serviço: " << serviceName << std::endl;
            std::cout << "Nome de exibição: " << displayName << std::endl;
            std::cout << "PID: " << processId << std::endl;
            std::cout << std::endl;

            pids.push_back(processId);
        }
    }

    delete[] reinterpret_cast<char*>(servicesBuffer);
    CloseServiceHandle(scmHandle);

    return pids;
}

std::vector<DWORD> listarProcessos()
{
    if (!SetPrivilege(SE_DEBUG_NAME, TRUE))
    {
        std::cout << "Falha ao elevar os privilégios do programa." << std::endl;
        return {};
    }

    std::vector<DWORD> pids;

    HANDLE snapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshotHandle == INVALID_HANDLE_VALUE)
    {
        std::cout << "Falha ao criar o snapshot dos processos." << std::endl;
        return pids;
    }

    PROCESSENTRY32 processEntry;
    processEntry.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(snapshotHandle, &processEntry))
    {
        std::cout << "Falha ao obter a primeira entrada do processo." << std::endl;
        CloseHandle(snapshotHandle);
        return pids;
    }

    std::cout << "Lista de processos em execução:" << std::endl;

    do
    {
        std::string processName(processEntry.szExeFile);
        DWORD processId = processEntry.th32ProcessID;

        if (processName == "lsass.exe" || processName == "explorer.exe" || processName == "Discord.exe" || processName == "csrss.exe" || processName == "Registry")
        {
            std::cout << "Nome do processo: " << processName << std::endl;
            std::cout << "PID: " << processId << std::endl;
            std::cout << std::endl;

            pids.push_back(processId);
        }
    } while (Process32Next(snapshotHandle, &processEntry));

    CloseHandle(snapshotHandle);

    return pids;
}