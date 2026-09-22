#ifndef FONTS_H
#define FONTS_H

extern Font mainFont;
extern Font subFont;

void loadAllFonts();            // Load fonts
void unloadAllFonts();          // Unload fonts
void drawTextFont(Font font, const char *text, int posX, int posY, int fontSize, int spacing, Color color);

#endif
