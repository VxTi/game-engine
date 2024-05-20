//
// Created by Luca Warmenhoven on 20/05/2024.
//

#include "DrawableFont.h"
#include <fstream>
#include <iostream>

TrueTypeFont::TrueTypeFont(uint32_t scalerType, uint16_t numTables, uint16_t searchRange, uint16_t entrySelector, uint16_t rangeShift, ttf_table_header_t *tables)
    : scalerType(scalerType), tableNumbers(numTables), searchRange(searchRange), entrySelector(entrySelector), rangeShift(rangeShift)
{

}

// Calculate the checksum of a table
uint32_t calculateChecksum(uint32_t *tableData, size_t length)
{
    uint32_t sum = 0;
    for ( uint32_t nLongs = (length + 3) / 4; nLongs-- > 0; )
        sum += *tableData++;
    return sum;
}

uint32_t getUint32(uint8_t *data, size_t offset){ return (data[offset] << 24) | (data[offset + 1] << 16) | (data[offset + 2] << 8) | data[offset + 3]; }
uint16_t getUint16(uint8_t *data, size_t offset) { return (data[offset] << 8) | data[offset + 1]; }
uint8_t getUint8(uint8_t *data, size_t offset) { return data[offset]; }


TrueTypeFont TrueTypeFont::parse(const char *fontPath)
{
    std::ifstream file(fontPath, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file" << std::endl;
        exit(1);
    }

    // Read the TTF header
    uint32_t scalerType;
    uint16_t numTables;
    uint16_t searchRange;
    uint16_t entrySelector;
    uint16_t rangeShift;

    file.read(reinterpret_cast<char*>(&scalerType), sizeof(scalerType));
    file.read(reinterpret_cast<char*>(&numTables), sizeof(numTables));
    file.read(reinterpret_cast<char*>(&searchRange), sizeof(searchRange));
    file.read(reinterpret_cast<char*>(&entrySelector), sizeof(entrySelector));
    file.read(reinterpret_cast<char*>(&rangeShift), sizeof(rangeShift));

    // Convert from big-endian to host-endian
    scalerType = __builtin_bswap32(scalerType);
    numTables = __builtin_bswap16(numTables);
    searchRange = __builtin_bswap16(searchRange);
    entrySelector = __builtin_bswap16(entrySelector);
    rangeShift = __builtin_bswap16(rangeShift);

    auto tagToString = [](uint32_t tag) {
        char str[5];
        str[0] = (tag >> 24) & 0xFF;
        str[1] = (tag >> 16) & 0xFF;
        str[2] = (tag >> 8) & 0xFF;
        str[3] = tag & 0xFF;
        str[4] = '\0';
        return std::string(str);
    };

    std::vector<ttf_table_header_t> tableDirectory;

    for (int i = 0; i < numTables; ++i) {
        ttf_table_header_t entry;
        file.read(reinterpret_cast<char*>(&entry.tag), sizeof(entry.tag));
        file.read(reinterpret_cast<char*>(&entry.checkSum), sizeof(entry.checkSum));
        file.read(reinterpret_cast<char*>(&entry.offset), sizeof(entry.offset));
        file.read(reinterpret_cast<char*>(&entry.length), sizeof(entry.length));

        // Convert from big-endian to host-endian
        entry.tag = __builtin_bswap32(entry.tag);
        entry.checkSum = __builtin_bswap32(entry.checkSum);
        entry.offset = __builtin_bswap32(entry.offset);
        entry.length = __builtin_bswap32(entry.length);

        std::cout << "Table name: " << tagToString(entry.tag) << std::endl;
        std::cout << "CheckSum: " << entry.checkSum << std::endl;

        tableDirectory.push_back(entry);
    }

    return TrueTypeFont(
            scalerType,
            numTables,
            searchRange,
            entrySelector,
            rangeShift,
            nullptr
            );
}

void TrueTypeFont::draw()
{
    // Draw the font
}