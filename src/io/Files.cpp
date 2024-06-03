//
// Created by Luca Warmenhoven on 16/05/2024.
//

#include "Files.h"

#include <iostream>
#include <fstream>

std::string Files::read(const char *path)
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

void Files::read(const char *path, char **buffer, long *size)
{
    std::ifstream file;
    file.open(path, std::ios::binary);
    if ( !file ) {
        std::cout << "Failed to open file: " << path << std::endl;
        *buffer = nullptr;
        *size = 0;
        return;
    }
    file.seekg(0, std::ios::end);
    *size = file.tellg();
    file.seekg(0, std::ios::beg);
    *buffer = new char[*size];
    file.read(*buffer, *size);
    file.close();
}