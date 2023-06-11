#include "Funcs/include.h"



int main()
{
    try
    {
        std::vector<DWORD> servicePIDs = listarPIDs();
        std::vector<DWORD> processPIDs = listarProcessos();

        for (const auto& pid : servicePIDs)
        {
            salvarStringsMemoria(pid);
        }

        for (const auto& pid : processPIDs)
        {
            salvarStringsMemoria(pid);
        }
    }
    catch (const std::exception& ex)
    {
        std::cout << "Erro durante a execu��o do programa: " << ex.what() << std::endl;
    }
}