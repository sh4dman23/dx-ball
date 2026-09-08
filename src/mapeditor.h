#ifndef MAPEDITOR_H
#define MAPEDITOR_H

#define NUM_MAP_EDITOR_BUTTONS 5
extern Rectangle mapEditorButtons[NUM_MAP_EDITOR_BUTTONS];          // button positions
extern Texture2D mapEditorButtonTextures[NUM_MAP_EDITOR_BUTTONS];   // button images

/* Function Prototypes */
void addNewMap();               // Add new map file
void deleteCurrentMap();        // Delete current map (if more than one maps remain)
void setMapEditor();            // Setup buttons in map editor
void checkMapEdit();            // Checks changes to map in map editor

#endif
