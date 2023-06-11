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

    // Obter o LUID (Locally Unique Identifier) do privil�gio solicitado
    if (!LookupPrivilegeValue(NULL, privilegeName, &privilegeLUID))
    {
        std::cout << "Falha ao obter o LUID do privil�gio." << std::endl;
        CloseHandle(tokenHandle);
        return FALSE;
    }

    // Definir as informa��es de privil�gio
    privileges.PrivilegeCount = 1;
    privileges.Privileges[0].Luid = privilegeLUID;
    privileges.Privileges[0].Attributes = enable ? SE_PRIVILEGE_ENABLED : 0;

    // Solicitar a altera��o de privil�gios
    if (!AdjustTokenPrivileges(tokenHandle, FALSE, &privileges, sizeof(TOKEN_PRIVILEGES), NULL, NULL))
    {
        std::cout << "Falha ao ajustar os privil�gios do token." << std::endl;
        CloseHandle(tokenHandle);
        return FALSE;
    }

    // Verificar se a altera��o de privil�gios foi bem-sucedida
    if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)
    {
        std::cout << "N�o foi poss�vel atribuir todos os privil�gios solicitados." << std::endl;
        CloseHandle(tokenHandle);
        return FALSE;
    }

    CloseHandle(tokenHandle);
    return TRUE;
}