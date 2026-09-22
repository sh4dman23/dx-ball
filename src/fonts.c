#include "raylib.h"
#include "raymath.h"
#include <stdlib.h>

#include "fonts.h"

Font peepoFont;
Font sprint2Font;
Font ccRedAlertFont;

// accessible by functions using fonts.h
Font mainFont;
Font subFont;

// modify these pointers
const Font *mainFontP = &peepoFont;
const Font *subFontP = &ccRedAlertFont;

void loadAllFonts() {
    peepoFont = LoadFontEx("./fonts/peepo/Peepo.ttf", 100, NULL, 0);
    sprint2Font = LoadFontEx("./fonts/sprint2/sprint-2.ttf", 100, NULL, 0);
    ccRedAlertFont = LoadFontEx("./fonts/c&c-redalert/c&c-redalert.ttf", 100, NULL, 0);

    mainFont = *mainFontP;
    subFont = *subFontP;
}

void unloadAllFonts() {
    UnloadFont(peepoFont);
    UnloadFont(sprint2Font);
    UnloadFont(ccRedAlertFont);
}

void drawTextFont(Font font, const char *text, int posX, int posY, int fontSize, int spacing, Color color) {
    DrawTextEx(font, text, (Vector2) {posX, posY}, fontSize, spacing, color);
}
