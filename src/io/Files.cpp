//
// Created by Luca Warmenhoven on 16/05/2024.
//

#include "Files.h"

#include <iostream>
#include <fstream>

std::string Files::readFile(const char *path)
{
    std::ifstream file;
    file.open(path);
    if ( !file ) {
        std::cout << "Failed to open file: " << path << std::endl;
        return "";
    }
    std::string source;
    std::string line;
    while ( std::getline(file, line)) {
        source += line + "\n";
    }
    file.close();
    return source;
}