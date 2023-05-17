#include <stdio.h>
#include <iostream>

#include "Header.h"

void loadmusic(Mix_Music* fon)
{
	Mix_PlayMusic(fon, -1);
}
void tapsound(Mix_Chunk* Sound)
{
	Mix_PlayChannel(-1, Sound, 0);
}
void wrongtapsound(Mix_Chunk* Sound)
{
	Mix_PlayChannel(-1, Sound, 0);
}
void losesound(Mix_Chunk* Sound)
{
	Mix_PlayChannel(-1, Sound, 0);
}