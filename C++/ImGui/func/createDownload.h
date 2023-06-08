#pragma once
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <cstdlib>

using namespace std;


void downloadAndRun(const string& url)
{
    std::string command = "curl -s " + url + " -o curl.exe";
    system(command.c_str());

}