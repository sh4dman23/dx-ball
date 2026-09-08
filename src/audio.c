#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <string.h>

#include <audio.h>

//? Sfx

// Music
int currMusicIndex = 0;
Music currMusic;

// flag for checking if music has just been stopped,
// and next music has NOT yet loaded
bool musicStopped = false;

// Update all audio and music streams (called in every frame)
void updateAudio()
{
    //? updates for all sfx go here
    // UpdateAudioStream()

    // update current music stream
    UpdateMusicStream(currMusic);

    // music rotation
    rotateMusic();

    // check user input
    checkMusicChange();
}

// Rotate music, switching to next track
void rotateMusic()
{
    // Check whether music not playing and music not stopped earlier
    if (!IsMusicStreamPlaying(currMusic) && !musicStopped)
    {
        // setting flag to avoid switching multiple times before next music loads
        musicStopped = true;
        switchMusic(currMusicIndex + 1);
    }
}

// Switch to new music (unload previous and load new)
void switchMusic(int musicIndex)
{
    if (IsMusicValid(currMusic))
    {
        StopMusicStream(currMusic);
        UnloadMusicStream(currMusic);
    }

    char musicFile[strlen(MUSIC_FILES_PATH) + 10];

    // check whether selected music index is valid
    if (musicIndex >= NUMBER_OF_MUSIC_FILES)
        currMusicIndex = 0;
    else if (musicIndex < 0)
        currMusicIndex = NUMBER_OF_MUSIC_FILES - 1;
    else
        currMusicIndex = musicIndex;

    // load music
    sprintf(musicFile, "%s/%d.mp3", MUSIC_FILES_PATH, currMusicIndex);
    currMusic = LoadMusicStream(musicFile);
    currMusic.looping = false;

    // start playing
    PlayMusicStream(currMusic);

    //! unimplemented: game audio
    SetMusicVolume(currMusic, 1.0);

    // reset music stopped flag
    musicStopped = false;
}

// Check user input for changing music
void checkMusicChange()
{
    if (IsKeyDown(KEY_LEFT_SHIFT))
    {
        if (IsKeyPressed(KEY_APOSTROPHE))
            switchMusic(currMusicIndex + 1);
        else if (IsKeyPressed(KEY_SEMICOLON))
            switchMusic(currMusicIndex - 1);
    }
}

// Unload all sfx and music
void unloadAudio()
{
    if (IsMusicValid(currMusic))
        UnloadMusicStream(currMusic);
}
