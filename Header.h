#pragma once
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
#define NumbP 6
#define Set 8
//------STRUCTURES-----------------------------------------
struct levels
{
	int TimeLevel;
	int PointsLevel;
};
struct Gems
{
	SDL_Texture* GemTexture;
	SDL_Rect CardRect;
};

//------GAME_LOGIC------------------------------------------
bool CheckSecondTap(int i, int j, int i1, int j1);
int CheckCombinationLeft(int level[][8], int j1, int i1);
int CheckCombinationRight(int level[][8], int j1, int i1);
int CheckCombinationDown(int level[][8], int j1, int i1);
int CheckCombinationUp(int level[][8], int j1, int i1);
void BrokeLeftRight(int level[][8], int stolb, int begin, int end);
void BrokeUpDown(int level[][8], int stolb, int begin, int end);
void CheckGeneration(int level[][8]);
void RandomLevelGen(int level[][8]);
void ZeroElementLogic(int level[][8], int CheckCardOpeni2, int CheckCardOpenj2);

//------MUSIC------------------------------------------------
void loadmusic(Mix_Music* fon);
void tapsound(Mix_Chunk* Sound);
void wrongtapsound(Mix_Chunk* Sound);
void losesound(Mix_Chunk* Sound);

//------INTERFACE--------------------------------------------
SDL_Texture* get_text_texture(SDL_Renderer*& renderer, char* text, TTF_Font* font, SDL_Color fore_color);
void draw_text(SDL_Renderer*& renderer, SDL_Texture* texture, SDL_Rect rect);
void UploadPict(SDL_Renderer*& render, SDL_Surface* SurfImage[], int number, SDL_Texture* TexturImage[], Gems gems[]);
void DrawCells(SDL_Renderer*& renderer, Gems gem[][8], Gems gems[], int level[][8]);
bool CheckMenuHit(SDL_Rect MenuButtons, int x, int y);
void SetBut(SDL_Renderer* render, SDL_Rect SettingsButtons[]);
void UpdateRec(int& time1, int& time2, int& time3, int& RecordVisionTime);
void UpdateRects(bool fc, int W, int H, SDL_Rect& FonRect, SDL_Rect& MenuRect, SDL_Rect MenuButtons[], SDL_Rect SetButtons[], SDL_Rect& BackButtonRect, SDL_Rect PauseButtonsRect[], SDL_Rect& GridRect, SDL_Rect& BestTimeRect, SDL_Rect& FileErrorRect, SDL_Rect& WinMessageRect, SDL_Rect& ShowTimeAGRect, SDL_Rect& PointsRect, SDL_Rect& PointsFNRect, SDL_Rect& TimerRect, Gems gem[][8]);