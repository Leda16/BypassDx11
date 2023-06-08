#include <iostream>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>

std::string execute_command(const std::string& command) {
    std::string result;
    std::shared_ptr<FILE> pipe(_popen(command.c_str(), "r"), _pclose);
    if (!pipe) {
        throw std::runtime_error("Erro ao executar o comando.");
    }
    char buffer[128];
    while (!feof(pipe.get())) {
        if (fgets(buffer, 128, pipe.get()) != nullptr) {
            result += buffer;
        }
    }
    return result;
}

std::string GetHWID() {
    std::string command = "wmic csproduct get uuid";
    std::string output = execute_command(command);

    // Remove qualquer espaço em branco no início da string
    output.erase(output.begin(), std::find_if(output.begin(), output.end(), [](unsigned char ch) {
        return !std::isspace(ch);
        }));

    // Verifica se a string começa com "UUID"
    std::string uuidPrefix = "UUID";
    if (output.compare(0, uuidPrefix.size(), uuidPrefix) == 0) {
        // Remove o prefixo "UUID" da string
        output.erase(0, uuidPrefix.size());

        // Remove qualquer espaço em branco novamente
        output.erase(output.begin(), std::find_if(output.begin(), output.end(), [](unsigned char ch) {
            return !std::isspace(ch);
            }));
    }

    return output;
}