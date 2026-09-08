#ifndef STATS_H
#define STATS_H

// Core game statistics
extern int playerScore;
extern double scoreMultiplier;
extern int playtime;
extern int lives;
extern const int STARTING_LIVES;

extern const int BASE_BRICK_HIT_SCORE;

#define MAX_PLAYER_NAME_LENGTH 16               // Maximum length for player name

extern Texture2D lifeTexture;                   // Texture for life

/* Function prototypes */
void increaseLives();                           // Increase lives by +1
void updatePlayTime();                          // Update playtime counter
void resetStats();                              // Reset stats for new game
void increaseScore(int change);                 // Increase player score

#endif
