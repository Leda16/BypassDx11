#pragma once
#include "createNetwork.h"
namespace fs = std::filesystem;

#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "ntdll.lib")

int opcao;
int maxTries = 2;
int numTries = 0;
bool userFound = false;
