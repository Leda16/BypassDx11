#pragma once
#include <iostream>
#include <string>
#include <windows.h>
#include <d3d9.h>
#include <tchar.h>

std::string GetProcessorInfo() {
    std::string processorInfo;

    char buffer[256];
    DWORD bufferSize = sizeof(buffer);

    if (RegGetValueA(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", "ProcessorNameString", RRF_RT_ANY, nullptr, buffer, &bufferSize) == ERROR_SUCCESS) {
        processorInfo = buffer;
    }

    return processorInfo;
}

std::string GetGPUInfo() {
    std::string gpuInfo;

    IDirect3D9* pD3D = Direct3DCreate9(D3D_SDK_VERSION);
    if (pD3D) {
        D3DADAPTER_IDENTIFIER9 adapterIdentifier;
        if (SUCCEEDED(pD3D->GetAdapterIdentifier(D3DADAPTER_DEFAULT, 0, &adapterIdentifier))) {
            gpuInfo = adapterIdentifier.Description;
        }

        pD3D->Release();
    }

    return gpuInfo;
}

std::string GetMotherboardInfo() {
    std::string motherboardInfo;

    HKEY hKey;
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("HARDWARE\\DESCRIPTION\\System\\BIOS"), 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        TCHAR value[256];
        DWORD dataSize = sizeof(value);

        if (RegQueryValueEx(hKey, _T("BaseBoardProduct"), NULL, NULL, reinterpret_cast<LPBYTE>(value), &dataSize) == ERROR_SUCCESS) {
            value[dataSize / sizeof(TCHAR) - 1] = _T('\0');
            motherboardInfo = reinterpret_cast<TCHAR*>(value);
        }

        RegCloseKey(hKey);
    }

    return motherboardInfo;
}