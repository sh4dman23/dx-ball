#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

#include "mapeditor.h"
#include "audio.h"
#include "bricks.h"
#include "maps.h"
#include "mainmenu.h"
#include "coregame.h"

// Map editor buttons
Rectangle mapEditorButtons[NUM_MAP_EDITOR_BUTTONS];
Texture2D mapEditorButtonTextures[NUM_MAP_EDITOR_BUTTONS];

// Map selections
Vector2 mapSelectionPos[2] = {(Vector2) {-1, -1}, (Vector2) {-1, -1}};
Rectangle mapSelectionRegion = (Rectangle){0, 0, 0, 0};

bool brickSelected[MAX_NUMBER_OF_BRICKS] = { false };
int selectedBricksCount = 0;


/* Function Definitions */

// Add new map file
void addNewMap()
{
    if (numberOfMaps == MAX_NUMBER_OF_MAPS)
        return;
    numberOfMaps++;
    currentMap = numberOfMaps - 1;
    setEmptyMap();
    saveCurrentMap();
}

// Delete current map (if more than one maps remain)
void deleteCurrentMap()
{
    if (numberOfMaps <= 1)
        return;

    int lastMap = currentMap;
    while (currentMap < numberOfMaps - 1)
    {
        // copy (i + 1)th map to the ith map
        switchToMap(currentMap + 1);
        currentMap--;
        saveCurrentMap();
        currentMap++;
    }
    numberOfMaps--;

    char mapFilePath[20];
    sprintf(mapFilePath, "%s/%d.txt", MAP_FILES_PATH, currentMap);

    // delete extra file
    FileRemove(mapFilePath);

    // switch to map originally succeeding the deleted map
    if (lastMap < numberOfMaps)
        currentMap = lastMap;
    else
        currentMap = numberOfMaps - 1;
    switchToMap(currentMap);
}

// Setup buttons in map editor
void setMapEditor()
{
    int px = 0, py = 20;

    const int fontSize = MAP_EDITOR_BUTTON_SIZE;
    const Vector2 mapNameBoxDimensions = {150, fontSize * 2};
    const Vector2 otherBoxDimensions = {fontSize * 2, fontSize * 2};
    const int spacing = 5;

    px = (GetScreenWidth() - mapNameBoxDimensions.x - otherBoxDimensions.x * 5 - spacing * 5) / 2;
    int orPx = px;
    px += 2 * (otherBoxDimensions.x + spacing);

    // map name box
    mapEditorButtons[0] = (Rectangle){px, py, mapNameBoxDimensions.x, mapNameBoxDimensions.y};
    px += mapNameBoxDimensions.x + spacing;

    // main 4
    for (int i = 0; i < 4; i++)
    {
        mapEditorButtons[i + 1] = (Rectangle){
            px + i * otherBoxDimensions.x + i * spacing, py,
            otherBoxDimensions.x, otherBoxDimensions.y
        };
    }

    px = orPx;

    // switch order buttons
    for (int i = 0; i < 2; i++) {
        mapEditorButtons[i + 5] = (Rectangle){
            px + i * otherBoxDimensions.x + i * spacing, py,
            otherBoxDimensions.x, otherBoxDimensions.y
        };
    }
}

// Map selections
void manageMapSelections() {
    Vector2 mousePos = GetMousePosition();
    bool mouseInMapArea = CheckCollisionPointRec(mousePos, (Rectangle){
        bricks[0].rect.x,
        bricks[0].rect.y,
        bricks[0].rect.x + BRICK_WIDTH * maxBrickCols,
        bricks[0].rect.y + BRICK_HEIGHT * maxBrickRows
    });

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !mouseInMapArea)
        clearMapEditorSelection();

    if (IsKeyDown(KEY_LEFT_CONTROL)) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && mouseInMapArea) {
            clearSelectionRegion();
            mapSelectionPos[0] = mousePos;
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && !Vector2Equals(mapSelectionPos[0], (Vector2) {-1, -1}))
            mapSelectionPos[1] = mousePos;

        // update map selection region
        mapSelectionRegion = (Rectangle) {
            min(mapSelectionPos[0].x, mapSelectionPos[1].x),
            min(mapSelectionPos[0].y, mapSelectionPos[1].y),
            fabs(mapSelectionPos[1].x - mapSelectionPos[0].x),
            fabs(mapSelectionPos[1].y - mapSelectionPos[0].y)
        };

        // finish selection
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && !Vector2Equals(mapSelectionPos[0], (Vector2) {-1, -1})) {
            // set bricks selections here
            updateBrickSelections();

            clearSelectionRegion();
        }
    }

    if (IsKeyUp(KEY_LEFT_CONTROL))
        clearSelectionRegion();
}

// Checks changes to map in map editor
void checkMapEdit()
{
    //* So that brick type doesnt change automatically as soon as editor opens
    if (IsMouseButtonUp(MOUSE_BUTTON_LEFT))
        justMouseClicked = false;
    if (justMouseClicked)
        return;

    Vector2 mousePos = GetMousePosition();

    if (IsKeyPressed(KEY_ENTER))
        saveCurrentMap();                       // save map
    else if (IsKeyPressed(KEY_BACKSPACE)) {
        initializeCurrentMap();                 // reset all changes
        clearMapEditorSelection();
    }

    manageMapSelections();

    //* edit selected bricks
    if (GetMouseWheelMove() > 0 || IsKeyPressed(KEY_UP)) {
        for (int i = 0; i < numBricks; i++)
            if (brickSelected[i])
                changeBrickType(i, +1);
    }
    else if (GetMouseWheelMove() < 0 || IsKeyPressed(KEY_DOWN)) {
        for (int i = 0; i < numBricks; i++)
            if (brickSelected[i])
                changeBrickType(i, -1);
    }
    else if (IsKeyPressed(KEY_DELETE)) {
        for (int i = 0; i < numBricks; i++)
            if (brickSelected[i])
                bricks[i].type = BRICK_EMPTY;
    }

    //* cycle brick type right
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !IsKeyDown(KEY_LEFT_CONTROL))
    {
        clearSelectionRegion();
        if (selectedBricksCount > 0) {
            clearMapEditorSelection();
            return;
        }

        for (int i = 0; i < numBricks; i++)
        {
            if (CheckCollisionPointRec(mousePos, bricks[i].rect))
            {
                // playSfx(SFX_ME_BRICK_CHANGE);
                changeBrickType(i, +1);
                break;
            }
        }
    }

    //* cycle brick type left
    else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && !IsKeyDown(KEY_LEFT_CONTROL))
    {
        clearSelectionRegion();
        if (selectedBricksCount > 0) {
            clearMapEditorSelection();
            return;
        }

        for (int i = 0; i < numBricks; i++)
        {
            if (CheckCollisionPointRec(mousePos, bricks[i].rect))
            {
                // playSfx(SFX_ME_BRICK_CHANGE);
                changeBrickType(i, -1);
                break;
            }
        }
    }

    //* set empty brick
    else if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE) && !IsKeyDown(KEY_LEFT_CONTROL))
    {
        clearSelectionRegion();
        if (selectedBricksCount > 0) {
            clearMapEditorSelection();
            return;
        }

        for (int i = 0; i < numBricks; i++)
        {
            if (CheckCollisionPointRec(mousePos, bricks[i].rect) && bricks[i].type != 0)
            {
                if (!isSfxPlaying(SFX_ME_BRICK_CHANGE))
                    playSfx(SFX_ME_BRICK_CHANGE);
                bricks[i].type = 0;
            }
        }
    }

    //* interaction with map editor buttons
    manageMapEditorButtonInteractions();

    // arrow keys to change map
    if (IsKeyPressed(KEY_LEFT))
    {
        playSfx(SFX_ME_BUTTON_CLICK);
        saveCurrentMap();
        switchToMap(currentMap - 1);
        clearMapEditorSelection();
    }
    else if (IsKeyPressed(KEY_RIGHT))
    {
        playSfx(SFX_ME_BUTTON_CLICK);
        saveCurrentMap();
        switchToMap(currentMap + 1);
        clearMapEditorSelection();
    }
}

void manageMapEditorButtonInteractions() {
    Vector2 mousePos = GetMousePosition();

    bool clicked = false;

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        clicked = true;

        // cycle maps left
        if (CheckCollisionPointRec(mousePos, mapEditorButtons[1]) && numberOfMaps > 1) {
            saveCurrentMap();
            switchToMap(currentMap - 1);
        }

        // cycle maps right
        else if (CheckCollisionPointRec(mousePos, mapEditorButtons[2]) && numberOfMaps > 1) {
            saveCurrentMap();
            switchToMap(currentMap + 1);
        }

        // add new map
        else if (CheckCollisionPointRec(mousePos, mapEditorButtons[3]) && numberOfMaps < MAX_NUMBER_OF_MAPS) {
            addNewMap();
        }

        // delete current map
        else if (CheckCollisionPointRec(mousePos, mapEditorButtons[4]) && numberOfMaps > 1) {
            deleteCurrentMap();
        }

        // move map up
        else if (CheckCollisionPointRec(mousePos, mapEditorButtons[5]) && currentMap < numberOfMaps - 1) {
            swapMap(currentMap, currentMap + 1);
        }

        // move map down
        else if (CheckCollisionPointRec(mousePos, mapEditorButtons[6]) && currentMap > 0) {
            swapMap(currentMap, currentMap - 1);
        }

        else {
            clicked = false;
        }
    }

    if (clicked) {
        playSfx(SFX_ME_BUTTON_CLICK);
        clearMapEditorSelection();
    }
}

// Increase or decrease brick type in map editor
void changeBrickType(int brickIndex, int change) {
    if (!isSfxPlaying(SFX_ME_BRICK_CHANGE))
        playSfx(SFX_ME_BRICK_CHANGE);
    bricks[brickIndex].type = bricks[brickIndex].type + change % (NUM_BRICK_TEXTURES + 1);
    if (bricks[brickIndex].type > MAX_BRICK_TYPE)
        bricks[brickIndex].type = MIN_BRICK_TYPE;
    if (bricks[brickIndex].type < MIN_BRICK_TYPE)
        bricks[brickIndex].type = MAX_BRICK_TYPE;

    if (bricks[brickIndex].type == BRICK_EXPLODING)
        changeBrickType(brickIndex, change);
}

// Toggle selection states for bricks when selected again
void updateBrickSelections() {
    int selected = 0;
    for (int i = 0; i < numBricks; i++) {
        // toggle selection
        if (CheckCollisionRecs(bricks[i].rect, mapSelectionRegion))
            brickSelected[i] = !brickSelected[i];
        selected += brickSelected[i];
    }

    selectedBricksCount = selected;
}

void clearSelectionRegion() {
    mapSelectionPos[0] = (Vector2) {-1, -1};
    mapSelectionPos[1] = (Vector2) {-1, -1};
    mapSelectionRegion = (Rectangle) {0, 0, 0, 0};
}

void clearMapEditorSelection() {
    clearSelectionRegion();
    for (int i = 0; i < numBricks; i++)
        brickSelected[i] = false;
    selectedBricksCount = 0;
}

void swapMap(int mi1, int mi2) {
    if (mi1 < 0 || mi1 >= numberOfMaps) {
        mi1 = (mi1 % numberOfMaps + numberOfMaps) % numberOfMaps;
    }
    if (mi2 < 0 || mi2 >= numberOfMaps) {
        mi2 = (mi2 % numberOfMaps + numberOfMaps) % numberOfMaps;
    }

    // save changes
    saveCurrentMap();

    // map to switch to after swap
    int mapToSwitch = currentMap;
    if (mi1 == currentMap)
        mapToSwitch = mi2;
    else if (mi2 == currentMap)
        mapToSwitch = mi1;


    // store 1st map in tmp file
    switchToMap(mi1);
    FILE *tmp1 = tmpfile();
    writeMapToFile(tmp1);

    // store 2nd map in tmp file
    switchToMap(mi2);
    FILE *tmp2 = tmpfile();
    writeMapToFile(tmp2);


    // read 1st map from tmp1 into 2nd map
    rewind(tmp1);
    readMapFromFile(tmp1);
    saveCurrentMap();

    // read 2nd map from tmp2 into 1st map
    switchToMap(mi1);
    rewind(tmp2);
    readMapFromFile(tmp2);
    saveCurrentMap();

    fclose(tmp1);
    fclose(tmp2);

    switchToMap(mapToSwitch);
}
