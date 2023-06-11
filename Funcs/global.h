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