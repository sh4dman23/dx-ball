#ifndef AUDIO_H
#define AUDIO_H

//? Sfx

// Music
#define MUSIC_FILES_PATH "./audio/music"
#define NUMBER_OF_MUSIC_FILES 5

extern int currMusicIndex;
extern Music currMusic;

// flag for checking if music has just been stopped,
// and next music has NOT yet loaded
extern bool musicStopped;


/* Function Prototypes */
void updateAudio();                     // Update all audio and music streams (called in every frame)
void rotateMusic();                     // Rotate music, switching to next track
void switchMusic(int musicIndex);       // Switch to new music (unload previous and load new)
void checkMusicChange();                // Check user input for changing music
void unloadAudio();                     // Unload all sfx and music

#endif
