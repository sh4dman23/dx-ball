#include "raylib.h"
#include "raymath.h"
#include <stdlib.h>
#include <string.h>

#include "texts.h"
#include "maps.h"

Font peepoFont;
Font sprint2Font;
Font ccRedAlertFont;

// accessible by functions using fonts.h
Font mainFont;
Font subFont;

Text controlsText = {0};

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

// Load all text resource files required
void loadAllTexts() {
    controlsText.text = LoadFileText(TextFormat("%s/controls.txt", TEXT_FILES_PATH));
    controlsText.textLines = LoadTextLines(controlsText.text, &controlsText.lineCount);
}

// Unload text resource files required
void unloadAllTexts() {
    UnloadFileText(controlsText.text);
    UnloadTextLines(controlsText.textLines, controlsText.lineCount);
}

void drawTextFont(Font font, const char *text, double posX, double posY, int fontSize, int spacing, Color color) {
    DrawTextEx(font, text, (Vector2) {posX, posY}, fontSize, spacing, color);
}

void drawTextFontBold(Font font, const char *text, double posX, double posY, int fontSize, int spacing, int offset, Color color) {
    drawTextFont(font, text, posX - offset, posY, fontSize, spacing, color);
    drawTextFont(font, text, posX + offset, posY, fontSize, spacing, color);
    // drawTextFont(font, text, posX, posY - offset, fontSize, spacing, color);
    // drawTextFont(font, text, posX, posY + offset, fontSize, spacing, color);
    drawTextFont(font, text, posX, posY, fontSize, spacing, color);
}

void drawFormattedTextBlock(Font font, Text text, double posX, double posY, double fontSize, double spacing, Color color) {
    char **lines = text.textLines;
    if (lines == NULL)
        return;

    bool boldStarted = false;

    double px = posX, py = posY;

    for (int i = 0; i < text.lineCount; i++) {
        double textHeight = 0;
        if (strcmp(lines[i], "") == 0)
            textHeight = MeasureTextEx(font, " ", fontSize, spacing).y;
        else
            textHeight = MeasureTextEx(font, lines[i], fontSize, spacing).y;

        // headers start with #
        if (stringStartsWith(lines[i], "#")) {
            char *s = MemAlloc((strlen(lines[i]) + 1) * sizeof(char));
            strcpy(s, lines[i]);
            drawTextFontBold(font, strtok(s, "#"), px, py, fontSize * 28 / 24, spacing * 3 / 2, 1, color);
            MemFree(s);
            py += textHeight * 0.5;
            // reset bold status after header
            boldStarted = false;
        }

        // normal text
        else {
            // drawTextFont(mainFont, lines[i], px, py, fontSize, spacing, RAYWHITE);
            for (char *s = lines[i]; *s != '\0'; s++) {
                // bold text is enclosed by *
                if (*s == '*') {
                    boldStarted = !boldStarted;
                    continue;
                }

                // draw formatted character string, making it bold when needed
                char *formattedChar = TextFormat("%c", *s);
                Vector2 regularSize = MeasureTextEx(font, formattedChar, fontSize, spacing);
                Vector2 boldSize = MeasureTextEx(font, formattedChar, fontSize * 1.1, spacing * 2);

                if (boldStarted) {
                    drawTextFontBold(font, formattedChar, px, py - (boldSize.y - regularSize.y) / 2, fontSize * 1.1, spacing * 2, 1, color);
                    px += boldSize.x + spacing;
                }
                else {
                    drawTextFont(font, formattedChar, px, py, fontSize, spacing, color);
                    px += regularSize.x + spacing;
                }

            }
        }

        px = posX;
        py += textHeight * 1.2;
    }
}

// Draw text in controls screen
void drawControlsText() {
    int px = 100, py = PADDING_BELOW_MAP * 1.5;
    drawFormattedTextBlock(mainFont, controlsText, px, py, 24, 2, GetColor(0xE9E9E9FF));
}

bool stringStartsWith(const char *s1, const char *s2) {
    if (s1 == NULL || s2 == NULL)
        return false;

    while (*s1 && *s2 && *s1 == *s2) {
        s1++, s2++;
    }

    return (*s2 == '\0');
}

