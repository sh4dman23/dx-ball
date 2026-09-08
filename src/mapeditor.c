#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

#include <mapeditor.h>
#include <bricks.h>
#include <maps.h>
#include <mainmenu.h>

Rectangle mapEditorButtons[NUM_MAP_EDITOR_BUTTONS];
Texture2D mapEditorButtonTextures[NUM_MAP_EDITOR_BUTTONS];

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

// Checks changes to map in map editor
void checkMapEdit()
{
    //* So that the map doesn't update automatically
    if (IsMouseButtonUp(MOUSE_BUTTON_LEFT))
        justMouseClicked = false;
    if (justMouseClicked)
        return;

    Vector2 mousePos = GetMousePosition();

    // save map
    if (IsKeyPressed(KEY_ENTER))
    {
        saveCurrentMap();
    }

    // reset all changes
    else if (IsKeyPressed(KEY_BACKSPACE))
    {
        initializeCurrentMap();
    }

    // cycle brick type right
    else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        for (int i = 0; i < maxBrickCols * maxBrickRows; i++)
        {
            if (CheckCollisionPointRec(mousePos, bricks[i].rect))
            {
                bricks[i].type = bricks[i].type + 1;
                if (bricks[i].type > MAX_BRICK_TYPE)
                    bricks[i].type = MIN_BRICK_TYPE;
            }
        }
    }

    // cycle brick type left
    else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
    {
        for (int i = 0; i < maxBrickCols * maxBrickRows; i++)
        {
            if (CheckCollisionPointRec(mousePos, bricks[i].rect))
            {
                bricks[i].type = bricks[i].type - 1;
                if (bricks[i].type < MIN_BRICK_TYPE)
                    bricks[i].type = MAX_BRICK_TYPE;
            }
        }
    }

    // set empty brick
    else if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE))
    {
        for (int i = 0; i < maxBrickCols * maxBrickRows; i++)
        {
            if (CheckCollisionPointRec(mousePos, bricks[i].rect))
            {
                bricks[i].type = 0;
            }
        }
    }

    // interaction with map editor buttons
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        // cycle maps left
        if (CheckCollisionPointRec(mousePos, mapEditorButtons[1]))
        {
            saveCurrentMap();
            switchToMap(currentMap - 1);
        }

        // cycle maps right
        else if (CheckCollisionPointRec(mousePos, mapEditorButtons[2]))
        {
            saveCurrentMap();
            switchToMap(currentMap + 1);
        }

        // add new map
        else if (CheckCollisionPointRec(mousePos, mapEditorButtons[3]))
        {
            addNewMap();
        }

        // delete current map
        else if (CheckCollisionPointRec(mousePos, mapEditorButtons[4]))
        {
            deleteCurrentMap();
        }
    }

    // arrow keys to change map
    if (IsKeyPressed(KEY_LEFT))
    {
        saveCurrentMap();
        switchToMap(currentMap - 1);
    }
    else if (IsKeyPressed(KEY_RIGHT))
    {
        saveCurrentMap();
        switchToMap(currentMap + 1);
    }
}
