#ifndef MAPEDITOR_H
#define MAPEDITOR_H

#include <bricks.h>

#define NUM_MAP_EDITOR_BUTTONS 5
extern Rectangle mapEditorButtons[NUM_MAP_EDITOR_BUTTONS];          // button positions
extern Texture2D mapEditorButtonTextures[NUM_MAP_EDITOR_BUTTONS];   // button images

// Selections
extern Rectangle mapSelectionRegion;                    // Region selected in map editor
extern bool brickSelected[MAX_NUMBER_OF_BRICKS];        // Check whether brick is selected


/* Function Prototypes */
void addNewMap();                                       // Add new map file
void deleteCurrentMap();                                // Delete current map (if more than one maps remain)
void setMapEditor();                                    // Setup buttons in map editor
void checkMapEdit();                                    // Checks changes to map in map editor
void changeBrickType(int brickIndex, int change);       // Increase or decrease brick type in map editor

void manageMapSelections();                             // Manage selection region in map editor
void updateBrickSelections();                           // Update toggles for brick selections in map editor
void clearSelectionRegion();                            // Clear selection region in map editor
void clearMapEditorSelection();                         // Clear all selections in map editor

#endif
