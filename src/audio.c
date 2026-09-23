#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <string.h>

#include "audio.h"
#include "gamestates.h"

//* Sfx
const float sfxVol = 0.7f;

//? All sfx files must be of type .wav
// file name of audio files in filepath
char *sfxFileNames[NUMBER_OF_SFX] = {
    "BallDrop",
    "BrickCollide",
    "UnbreakableBrickCollide",
    "Boing",
    "BrickExplosion",
    "Laser",
    "PerkActivation",
    "WallCollide",
    "Click",
    "Click2",
};

// container for audio files
Sound sounds[NUMBER_OF_SFX];

//* Music
int currMusicIndex = 0;
Music currMusic;

const float musicVol = 1.0f;

bool musicAudible = true;           // whether music should be audible

// flag for checking if music has just been stopped,
// and next music has NOT yet loaded
bool musicStopped = false;

// Update all audio and music streams (called in every frame)
void updateAudio()
{
    // update audio device if disconnected
    if (!IsAudioDeviceReady()) {
        CloseAudioDevice();
        InitAudioDevice();
    }


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

    SetMusicVolume(currMusic, musicAudible ? musicVol : 0.0);

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

    if (gameState == GS_MAIN_MENU && IsKeyPressed(KEY_M))
        toggleMusic();
}

// Toggle whether music should be on or off
// (only turns sound off, music still plays)
void toggleMusic() {
    musicAudible = !musicAudible;
    SetMusicVolume(currMusic, musicAudible ? musicVol : 0.0);
}

// Play sfx
void playSfx(SfxID id) {
    if (id < 0 || id >= NUMBER_OF_SFX)
        return;
    SetSoundVolume(sounds[id], sfxVol);
    PlaySound(sounds[id]);
}

// Check if sfx is playing
bool isSfxPlaying(SfxID id) {
    return IsSoundPlaying(sounds[SFX_ME_BRICK_CHANGE]);
}

// Load all sfx
void loadAllAudio() {
    for (int i = 0; i < NUMBER_OF_SFX; i++) {
        char filename[50];
        sprintf(filename, "%s/%s.wav", SFX_FILES_PATH, sfxFileNames[i]);
        sounds[i] = LoadSound(filename);
    }
}

// Unload all sfx and music
void unloadAllAudio()
{
    for (int i = 0; i < NUMBER_OF_SFX; i++) {
        UnloadSound(sounds[i]);
    }

    if (IsMusicValid(currMusic))
        UnloadMusicStream(currMusic);
}
