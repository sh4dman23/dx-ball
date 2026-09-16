#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

#include <mapeditor.h>
#include <audio.h>
#include <bricks.h>
#include <maps.h>
#include <mainmenu.h>
#include <coregame.h>

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

    const int fontSize = 16;
    const Vector2 mapNameBoxDimensions = {150, fontSize * 2};
    const Vector2 otherBoxDimensions = {fontSize * 2, fontSize * 2};
    const int spacing = 5;

    px = (GetScreenWidth() - mapNameBoxDimensions.x - otherBoxDimensions.x * 3 - spacing * 3) / 2;

    // map name box
    mapEditorButtons[0] = (Rectangle){px, py, mapNameBoxDimensions.x, mapNameBoxDimensions.y};
    px += mapNameBoxDimensions.x + spacing;

    // other 4
    for (int i = 0; i < 4; i++)
    {
        mapEditorButtons[i + 1] = (Rectangle){px + i * otherBoxDimensions.x + i * spacing, py, otherBoxDimensions.x, otherBoxDimensions.y};
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
                playSfx(SFX_ME_BRICK_CHANGE);
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
                playSfx(SFX_ME_BRICK_CHANGE);
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
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        // cycle maps left
        if (CheckCollisionPointRec(mousePos, mapEditorButtons[1]))
        {
            playSfx(SFX_ME_BUTTON_CLICK);
            saveCurrentMap();
            switchToMap(currentMap - 1);
        }

        // cycle maps right
        else if (CheckCollisionPointRec(mousePos, mapEditorButtons[2]))
        {
            playSfx(SFX_ME_BUTTON_CLICK);
            saveCurrentMap();
            switchToMap(currentMap + 1);
        }

        // add new map
        else if (CheckCollisionPointRec(mousePos, mapEditorButtons[3]))
        {
            playSfx(SFX_ME_BUTTON_CLICK);
            addNewMap();
        }

        // delete current map
        else if (CheckCollisionPointRec(mousePos, mapEditorButtons[4]))
        {
            playSfx(SFX_ME_BUTTON_CLICK);
            deleteCurrentMap();
        }
    }

    // arrow keys to change map
    if (IsKeyPressed(KEY_LEFT))
    {
        playSfx(SFX_ME_BUTTON_CLICK);
        saveCurrentMap();
        switchToMap(currentMap - 1);
    }
    else if (IsKeyPressed(KEY_RIGHT))
    {
        playSfx(SFX_ME_BUTTON_CLICK);
        saveCurrentMap();
        switchToMap(currentMap + 1);
    }
}

// Increase or decrease brick type in map editor
void changeBrickType(int brickIndex, int change) {
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
