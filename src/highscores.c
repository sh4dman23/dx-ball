#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <string.h>
#include <highscores.h>
#include <stats.h>

// High Scores
HSEntry highScores[MAX_HIGH_SCORES];
int numHighScores = 0; // store how many high scores are present in memory

Texture2D highScoresTitleImage; // image for high scores title

/* Function Definitions */

// Read high scores from file
void readHighScores()
{
    FILE *hsFile = fopen(HIGH_SCORES_FILE_PATH, "r");
    if (hsFile == NULL)
    {
        TraceLog(LOG_ERROR, "Leaderboards data has been lost.\n");

        // open empty file
        hsFile = fopen(HIGH_SCORES_FILE_PATH, "w");
        if (hsFile != NULL)
            fclose(hsFile);
        return;
    }

    // read entries line by line
    for (int i = 0; i < MAX_HIGH_SCORES; i++)
    {
        if (fscanf(hsFile, "%d %d", &highScores[i].score, &highScores[i].playtime) != 2)
            break;

        char c = '\0';
        fscanf(hsFile, "%c", &c); // read extra space
        for (int j = 0; j < MAX_PLAYER_NAME_LENGTH && fscanf(hsFile, "%c", &c) == 1 && c != '\n'; j++)
        {
            highScores[i].name[j] = c;
            highScores[i].name[j + 1] = '\0';
        }

        // exhaust current line
        while (c != '\n' && fscanf(hsFile, "%c", &c) == 1);
        numHighScores++;
    }

    // sort high scores
    sortHighScores();

    fclose(hsFile);
}

// Sort high score entries based on score and playtime
void sortHighScores()
{
    for (int i = 0; i < numHighScores; i++)
    {
        for (int j = i + 1; j < numHighScores; j++)
        {
            if (highScores[j].score > highScores[i].score ||
                highScores[j].score == highScores[i].score && highScores[j].playtime < highScores[i].playtime)
            {
                HSEntry tmp = highScores[i];
                highScores[i] = highScores[j];
                highScores[j] = tmp;
            }
        }
    }
}

// Save score (called after entering name in end game screen)
void saveNewScore()
{
    // no need to save score with empty name
    if (strlen(nameInputStr) == 0)
        return;

    // find index to store
    int i = 0;
    while (i < numHighScores && (highScores[i].score > playerScore ||
        (highScores[i].score == playerScore && highScores[i].playtime <= playtime))
    )
    {
        i++;
    }

    // cannot be placed
    if (i >= MAX_HIGH_SCORES)
        return;

    // shift and replace
    for (int j = MAX_HIGH_SCORES - 1; j > i; j--)
    {
        highScores[j] = highScores[j - 1];
    }
    highScores[i].score = playerScore;
    highScores[i].playtime = playtime;
    strcpy(highScores[i].name, nameInputStr);

    // increase count
    if (numHighScores + 1 < MAX_HIGH_SCORES)
        numHighScores++;

    // save to file
    writeHighScores();
}

// Write high scores list from memory to file
void writeHighScores()
{
    FILE *hsFile = fopen(HIGH_SCORES_FILE_PATH, "w");
    if (hsFile == NULL)
    {
        TraceLog(LOG_ERROR, "High scores file could not be opened.");
    }

    // Format: {score} {playtime} {name}
    for (int i = 0; i < numHighScores; i++)
        fprintf(hsFile, "%d %d %s\n", highScores[i].score, highScores[i].playtime, highScores[i].name);

    fclose(hsFile);
}
