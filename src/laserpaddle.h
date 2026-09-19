#ifndef LASERPADDLE_H
#define LASERPADDLE_H

#define MAX_LASERS_PER_SCREEN 3
#define LASER_TEXTURE_FILEPATH "./assets/particles/laser.png"

extern const double INTERVAL_BETWEEN_LASERS;

extern Texture2D laserTexture;

typedef struct {
    bool leftOnScreen;
    bool rightOnScreen;
    Vector2 left;
    Vector2 right;
} Laser;

extern Laser lasers[MAX_LASERS_PER_SCREEN];

extern Vector2 laserSpeed;

extern bool laserActivated;

/* Function Prototypes */
void resetLasers();
void activateLaser();
void deactivateLaser();
void updateLasers();                // Update laser positions
void checkLaserControls();          // Spawn laser if lmb pressed and laser spawnable
void checkLaserCollisions();        // Check when laser collides with stuff
bool spawnLaser();                  // Spawns a laser if it can be spawned and returns true if spawned

#endif
