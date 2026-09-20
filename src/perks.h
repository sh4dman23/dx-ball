#ifndef PERKS_H
#define PERKS_H

#define PERKS_IMG_PATH "./assets/perks/"
extern const Vector2 PERK_IMG_SIZE;

extern const Vector2 PERK_SPEED;                // speed at which perk falls down
extern const double DELAY_AFTER_PERK_SPAWN;     // in seconds
extern const double TIMED_PERK_DURATION;        // base duration for perk that has timer
extern const int BASE_PERK_ACTIVATE_SCORE;      // base score gained on perk activation

extern bool canSpawnPerk;                       // flag for perk spawn

typedef struct Perk
{
    char filename[30];                          // name without .png
    double spawnChance;                         // in %
    bool timed;                                 // whether or not perk has timer
    Vector2 pos;                                // position of perk image when falling
    double duration;                            // remaining duration
    Texture2D img;
} Perk;

#define NUMBER_OF_PERKS 10
extern Perk perks[NUMBER_OF_PERKS];


/* Function Prototypes */
void resetPerks();                                  // Reset all perks to be deactivated and despawned
void updatePerks();                                 // Manage perk positions and durations
void spawnPerk(int brickIndex);                     // Spawn perk after ball hits a brick
bool isPerkSpawnable(int perkIndex);                // Check if perk can be spawned
void delayPerkSpawn();                              // Manage delay for perk spawn
void activatePerk(int perkIndex);                   // Activate effects of perk
void deactivatePerk(int perkIndex);                 // Remove effects of perk
void checkPerkAndPaddleCollision(int perkIndex);    // Check when falling perk hits paddle

#endif
