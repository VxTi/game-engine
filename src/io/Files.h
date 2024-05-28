//
// Created by Luca Warmenhoven on 16/05/2024.
//

#ifndef GRAPHICS_TEST_FILES_H
#define GRAPHICS_TEST_FILES_H

#include <cstdio>
#include <string>

/**
 * A struct that represents a file
 */
typedef struct
{
    const char *path;
    const char *content;
    const size_t size;
} File;

class Files
{
public:

    /**
     * Reads a file and returns a File struct
     * @param path The path to the file
     * @return A File struct
     */
    static std::string readFile(const char *path);

};


#endif //GRAPHICS_TEST_FILES_H