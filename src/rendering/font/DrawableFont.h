//
// Created by Luca Warmenhoven on 20/05/2024.
//

#ifndef GRAPHICS_TEST_DRAWABLEFONT_H
#define GRAPHICS_TEST_DRAWABLEFONT_H

#include <cstdint>

#define GLYPH_FLAG_ON_CURVE 0x01
#define GLYPH_FLAG_X_SHORT 0x02
#define GLYPH_FLAG_Y_SHORT 0x04
#define GLYPH_FLAG_REPEAT 0x08
#define GLYPH_FLAG_X_IS_SAME_OR_POSITIVE_X_SHORT_VECTOR 0x10
#define GLYPH_FLAG_Y_IS_SAME_OR_POSITIVE_Y_SHORT_VECTOR 0x20
#define GLYPH_FLAG_OVERLAP_SIMPLE 0x40
#define GLYPH_FLAG_RESERVED 0x80

typedef struct {
    uint32_t tag;
    uint32_t checkSum;
    uint32_t offset;
    uint32_t length;
} ttf_table_header_t;


typedef struct {
    int16_t numberOfCountours;
    uint16_t *endPtsOfContours;

    uint16_t instructionLength;
    uint8_t *instructions;

    uint8_t *flags;
    int16_t *xCoordinates;
    int16_t *yCoordinates;

    int16_t xMin, xMax;
    int16_t yMin, yMax;
    uint16_t width, height;

} ttf_glyph_t;

typedef struct {
    uint32_t *glyph_table;
    ttf_glyph_t *glyphs;


    ttf_table_header_t *cmap;
    ttf_table_header_t *head;
    ttf_table_header_t *hhea;
    ttf_table_header_t *hmtx;
    ttf_table_header_t *maxp;
    ttf_table_header_t *name;
} ttf;



class DrawableFont
{
public:

    float ascent;
    float descent;
    float body;
    float height;

    virtual void draw() = 0;
};

class TrueTypeFont : public DrawableFont
{

private:
    ttf_table_header_t *tables;

    uint32_t scalerType;
    uint16_t tableNumbers;
    uint16_t searchRange;
    uint16_t entrySelector;
    uint16_t rangeShift;

    TrueTypeFont(uint32_t scalerType, uint16_t numTables, uint16_t searchRange, uint16_t entrySelector, uint16_t rangeShift, ttf_table_header_t *tables);

public:


    static TrueTypeFont parse(const char *fontPath);

    void draw() override;

};



#endif //GRAPHICS_TEST_DRAWABLEFONT_H
