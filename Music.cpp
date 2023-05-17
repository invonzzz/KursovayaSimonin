#include <stdio.h>
#include <iostream>
#include <SDL.h>
#include <stdlib.h>
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include "SDL_image.h"
#include <ctime>
#include <fstream>
#include <string>
#include "Header.h"

void loadmusic(Mix_Music* fon)
{
	fon = Mix_LoadMUS("fon_music.wav");
	Mix_PlayMusic(fon, -1);
}
void tapsound(Mix_Chunk* Sound)
{
	Sound = Mix_LoadWAV("tap_sound.wav");
	Mix_PlayChannel(-1, Sound, 0);
}
void wrongtapsound(Mix_Chunk* Sound)
{
	Sound = Mix_LoadWAV("wrong.wav");
	Mix_PlayChannel(-1, Sound, 0);
}
void losesound(Mix_Chunk* Sound)
{
	Sound = Mix_LoadWAV("LoseGame.wav");
	Mix_PlayChannel(-1, Sound, 0);
}