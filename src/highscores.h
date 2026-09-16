#ifndef HIGHSCORES_H
#define HIGHSCORES_H
#include <stats.h>

// High Scores
#define MAX_HIGH_SCORES 10
#define HIGH_SCORES_FILE_PATH "./data/highscores.txt"

typedef struct HSEntry
{
    int score;
    int playtime;
    char name[MAX_PLAYER_NAME_LENGTH + 1];
} HSEntry;

extern HSEntry highScores[MAX_HIGH_SCORES];
extern int numHighScores;                   // number of high scores present in memory

extern Texture2D highScoresTitleImage;      // image for high scores title

/* Function Prototypes */
void readHighScores();                      // Read high scores from file
void sortHighScores();                      // Sort high score entries based on score and playtime
void saveNewScore();                        // Save score
void writeHighScores();                     // Write high scores list from memory to file

#endif
