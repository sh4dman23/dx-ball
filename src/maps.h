#ifndef MAPS_H
#define MAPS_H

// paddings for map area
extern const double PADDING_ABOVE_MAP;
extern const double PADDING_BELOW_MAP;
extern const double PADDING_ON_MAP_SIDES;

#define MAX_NUMBER_OF_MAPS 1000
#define MAP_FILES_PATH "./maps"

int currentMap;
int numberOfMaps;


/* Function Prototypes */
void setEmptyMap();                         // Set all bricks in map to empty bricks (type 0)
void initializeAllMaps();                   // Find number of maps and data related to each map
void initializeCurrentMap();                // Initializes (read and reset) current map
void readMapFromFile(FILE *mapFile);        // Read map from file to memory
void saveCurrentMap();                      // Save current map (as it is in memory)
void writeMapToFile(FILE *outputFile);      // Write map from memory to file
void switchToMap(int mapIndex);             // Switch to another map and initializes it

#endif
