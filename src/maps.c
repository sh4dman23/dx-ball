#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <string.h>

#include <maps.h>
#include <coregame.h>
#include <bricks.h>

int currentMap = 0;
int numberOfMaps = 0;


/* Function Definitions */

// Set all bricks in map to empty bricks
void setEmptyMap()
{
    double px = PADDING_ON_MAP_SIDES;
    double py = PADDING_ABOVE_MAP;

    for (int i = 0; i < maxBrickRows; i++)
    {
        for (int j = 0; j < maxBrickCols; j++)
        {
            bricks[i * maxBrickCols + j] = (Brick){
                (Rectangle){
                    px,
                    py,
                    BRICK_WIDTH,
                    BRICK_HEIGHT
                },
                BRICK_EMPTY
            };

            px += BRICK_WIDTH;
        }

        px = PADDING_ON_MAP_SIDES;
        py += BRICK_HEIGHT;
    }

    breakableBricksLeft = 0;
}

// Find number of maps and data related to that map
void initializeAllMaps()
{
    currentMap = 0;
    numberOfMaps = 0;

    while (true)
    {
        char mapFilePath[50] = {'\0'};
        sprintf(mapFilePath, "%s/%d.txt", MAP_FILES_PATH, currentMap);

        FILE *mapFile = fopen(mapFilePath, "r");
        if (mapFile == NULL)
        {
            break;
        }

        readMapFromFile(mapFile);
        fclose(mapFile);
        numberOfMaps++;
        currentMap++;
    }

    currentMap = 0;
}

// Initializes (read) current map
void initializeCurrentMap()
{
    setEmptyMap();

    char mapFilePath[20];
    sprintf(mapFilePath, "%s/%d.txt", MAP_FILES_PATH, currentMap);

    FILE *mapInputFile = fopen(mapFilePath, "r");
    if (mapInputFile == NULL) {
        switchToMap(currentMap + 1);
        return;
    }

    readMapFromFile(mapInputFile);
    fclose(mapInputFile);
}

// Read map from file to memory
void readMapFromFile(FILE *mapFile)
{
    setEmptyMap();

    int brickRows = 0, brickCols = 0;
    fscanf(mapFile, "%d %d ", &brickRows, &brickCols);

    int numBreakableBricks = 0;
    for (int i = 0; i < brickRows; i++)
    {
        for (int j = 0; j < brickCols; j++)
        {
            int brickType = 0;
            fscanf(mapFile, "%d ", &brickType);

            bricks[i * maxBrickCols + j].type = brickType;

            //* count number of bricks in level that can be broken
            if (isBrickBreakable(i * maxBrickCols + j))
                numBreakableBricks++;
        }
    }

    breakableBricksLeft = numBreakableBricks;
}

// Save current map
void saveCurrentMap()
{
    char mapFilePath[20];
    sprintf(mapFilePath, "%s/%d.txt", MAP_FILES_PATH, currentMap);

    FILE *mapFile = fopen(mapFilePath, "w");
    writeMapToFile(mapFile);
    fclose(mapFile);
}

// Write map from memory to file
void writeMapToFile(FILE *outputFile)
{
    fprintf(outputFile, "%d %d\n", maxBrickRows, maxBrickCols);
    for (int i = 0; i < maxBrickRows; i++)
    {
        for (int j = 0; j < maxBrickCols; j++)
        {
            fprintf(outputFile, "%d ", bricks[i * maxBrickCols + j].type);
        }
        fprintf(outputFile, "\n");
    }
}

// Switch to another map and initializes it
void switchToMap(int mapIndex)
{
    if (mapIndex < 0)
        currentMap = numberOfMaps + mapIndex % numberOfMaps;
    else
        currentMap = mapIndex;

    currentMap %= numberOfMaps;

    initializeCurrentMap();
}

