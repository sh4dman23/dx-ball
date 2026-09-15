#ifndef AUDIO_H
#define AUDIO_H

//! All sfx files must be of type .wav (add filenames in audio.c)
// Sfx
typedef enum {
    SFX_BALL_DROP = 0,
    SFX_BRICK_COLLIDE,
    SFX_UNBREAKABLE_BRICK_COLLIDE,
    SFX_BOING,
    SFX_BRICK_EXPLOSION,
    SFX_LASER,
    SFX_PERK_ACTIVATION,
    SFX_WALL_COLLIDE,
    SFX_ME_BUTTON_CLICK,
    SFX_ME_BRICK_CHANGE,

    // add sfx above this
    NUMBER_OF_SFX
} SfxID;

#define SFX_FILES_PATH "./audio/sfx"

// Music
#define MUSIC_FILES_PATH "./audio/music"
#define NUMBER_OF_MUSIC_FILES 5

extern int currMusicIndex;
extern Music currMusic;

// flag for checking if music has just been stopped,
// and next music has NOT yet loaded
extern bool musicStopped;


/* Function Prototypes */
void loadAllAudio();                    // Load all sfx
void unloadAllAudio();                  // Unload all sfx and music
void updateAudio();                     // Update all audio and music streams (called in every frame)
void rotateMusic();                     // Rotate music, switching to next track
void switchMusic(int musicIndex);       // Switch to new music (unload previous and load new)
void checkMusicChange();                // Check user input for changing music

void playSfx(SfxID id);                 // Play sound effect
bool isSfxPlaying(SfxID id);            // Check if sound effect is playing

#endif
