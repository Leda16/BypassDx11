#pragma once
#include <iostream>
#include <Windows.h>
#include <wininet.h>

#pragma comment(lib, "wininet.lib")

bool checkSiteStatus(const std::string& url) {
    HINTERNET hInternet = InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hInternet) {
        return false;
    }

    HINTERNET hUrl = InternetOpenUrlA(hInternet, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (!hUrl) {
        InternetCloseHandle(hInternet);
        return false;
    }

    InternetCloseHandle(hUrl);
    InternetCloseHandle(hInternet);
    return true;
}