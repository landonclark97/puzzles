#include <SDL2/SDL_mixer.h>

#include <stdlib.h>
#include <stdbool.h>

#include "./audio.h"


bool audio_init() {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 2048)) {
        printf("Error initializing audio device, SDL_Error: %s\n", Mix_GetError());
        return false;
    }
    return true;
}

music_t* music_new(char* path) {
    music_t* m = Mix_LoadMUS(path);
    if (!m) {
        return NULL;
    }
    return m;
}

sound_t* sound_new(char* path) {
    sound_t* s = Mix_LoadWAV(path);
    if (!s) {
        return NULL;
    }
    return s;
}

void music_play_inf(music_t* m) {
    Mix_PlayMusic(m, -1);
}

void music_play_loops(music_t* m, int loops) {
    Mix_PlayMusic(m, loops);
}

void music_play_faded_inf(music_t* m, int ms) {
    Mix_FadeInMusic(m, -1, ms);
}

void music_play_faded_loops(music_t* m, int loops, int ms) {
    Mix_FadeInMusic(m, loops, ms);
}

void music_fade_out(int ms) {
    Mix_FadeOutMusic(ms);
}

void music_resume() {
    Mix_ResumeMusic();
}

void music_pause() {
    Mix_PauseMusic();
}

void music_stop() {
    Mix_HaltMusic();
}

bool music_is_playing() {
    return Mix_PlayingMusic();
}

bool music_is_paused() {
    return Mix_PausedMusic();
}

void music_set_volume(int v) {
    Mix_VolumeMusic(v);
}

int sound_play(sound_t* s) {
    return Mix_PlayChannel(-1, s, 0);
}

int sound_play_channel(sound_t* s, int ch) {
    return Mix_PlayChannel(ch, s, 0);
}

void sound_set_volume(sound_t* s, int v) {
    Mix_VolumeChunk(s, v);
}

void music_free(music_t* m) {
    Mix_FreeMusic(m);
}

void sound_free(sound_t* s) {
    Mix_FreeChunk(s);
}

void audio_free() {
    Mix_Quit();
}
