#ifndef TEXTS_H
#define TEXTS_H

extern Font mainFont;
extern Font subFont;

#define TEXT_FILES_PATH "./text/"

typedef struct {
    char *text;
    char **textLines;
    int lineCount;
} Text;

/* Function Prototypes */
void loadAllFonts();            // Load fonts
void unloadAllFonts();          // Unload fonts
void loadAllTexts();            // Load all text resource files required
void unloadAllTexts();          // Unload all text resource files required

bool stringStartsWith(const char *s1, const char *s2);      // Check if a string starts with another substring

// draw text with font
void drawTextFont(Font font, const char *text, double posX, double posY, int fontSize, int spacing, Color color);
void drawTextFontBold(Font font, const char *text, double posX, double posY, int fontSize, int spacing, int offset, Color color);

// draw text block with supported formatting:
//      start with "#" for header
//      enclose with "**" for *bold* text
void drawFormattedTextBlock(Font font, Text text, double posX, double posY, double fontSize, double spacing, Color color);

void drawControlsText();        // Draw text in controls screen

#endif
