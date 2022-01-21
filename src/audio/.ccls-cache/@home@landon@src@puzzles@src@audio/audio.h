#ifndef __AUDIO__
#define __AUDIO__


#include <SDL2/SDL_mixer.h>

#include <stdbool.h>


typedef Mix_Music music_t;

typedef Mix_Chunk sound_t;


bool audio_init();

music_t* music_new(char* path);

sound_t* sound_new(char* path);

void music_play_inf(music_t* m);

void music_play_loops(music_t* m, int loops);

void music_play_faded_inf(music_t* m, int ms);

void music_play_faded_loops(music_t* m, int loops, int ms);

void music_fade_out(int ms);

void music_resume();

void music_pause();

void music_stop();

bool music_is_playing();

bool music_is_paused();

void music_set_volume(int v);

int sound_play(sound_t* s);

int sound_play_channel(sound_t* s, int ch);

void sound_set_volume(sound_t* s, int v);

void music_free(music_t* m);

void sound_free(sound_t* s);

void audio_free();


#endif
