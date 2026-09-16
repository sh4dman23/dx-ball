#ifndef BRICKS_H
#define BRICKS_H

// Bricks
#define MAX_NUMBER_OF_BRICKS 1000

#define BRICK_WIDTH 60.0
#define BRICK_HEIGHT 20.0

extern int maxBrickRows;
extern int maxBrickCols;
extern int numBricks;

extern int breakableBricksLeft;         // bricks remaining until level ends

typedef struct Brick
{
    Rectangle rect;                     // brick position and dimensions
    int type;                           // brick type
} Brick;

// Brick types
#define BRICK_EMPTY         0           // empty brick
#define BRICK_STD1          1           // standard brick 1
#define BRICK_STD2          2           // standard brick 2
#define BRICK_STD3          3           // standard brick 3
#define BRICK_UNBREAKABLE   -1          // unbreakable brick
#define BRICK_EXPLOSIVE     -2          // explosive brick

//* REQUIRED for map editor brick changes
#define MAX_BRICK_TYPE 3        //* edit this after adding brick types
#define MIN_BRICK_TYPE -2       //* edit this after adding brick types

// brick textures
#define BRICK_TEXTURES_PATH             "./assets/bricks"               // textures for normal bricks

#define NUM_BRICK_TEXTURES (MAX_BRICK_TYPE - MIN_BRICK_TYPE)            // excluding empty brick

extern Texture2D brickTextures[NUM_BRICK_TEXTURES + 1];                 // stored as: 0 1 2 3 ... -3 -2 -1

extern Brick bricks[MAX_NUMBER_OF_BRICKS];


/* Function Prototypes */
void degradeBrick(int brickIndex);              // Degrade brick, based on its type

//!* update this when new brick type added
bool isBrickBreakable(int brickIndex);           // Check if brick is breakable

#endif
