#pragma once
#include <iostream>
#include <windows.h>
#include <string>
#include <random>
#include <chrono>
#include <thread>

std::string generateRandomString(int length) {
    std::string result;
    static const char characters[] =
        "0123456789"
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    std::mt19937 generator(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> distribution(0, sizeof(characters) - 2);

    for (int i = 0; i < length; ++i) {
        result += characters[distribution(generator)];
    }

    return result;
}

void updateConsoleTitle() {
    while (true) {
        std::string title = generateRandomString(8);
        SetConsoleTitleA(title.c_str());
        Sleep(1000);
    }
}