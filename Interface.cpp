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
#define NumbPict 6
#define Setka 8
SDL_Texture* get_text_texture(SDL_Renderer*& renderer, char* text, TTF_Font* font, SDL_Color fore_color)
{
	SDL_Surface* textSurface = NULL;
	textSurface = TTF_RenderText_Blended(font, text, fore_color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_FreeSurface(textSurface);
	return texture;
}
void draw_text(SDL_Renderer*& renderer, SDL_Texture* texture, SDL_Rect rect)
{
	SDL_RenderCopy(renderer, texture, NULL, &rect);
}
void UploadPict(SDL_Renderer*& render, SDL_Surface* SurfImage[], int number, SDL_Texture* TexturImage[], Gems gems[])
{
	char numb[10];
	_itoa_s(number, numb, 10);
	strcat_s(numb, ".bmp");
	SurfImage[number] = IMG_Load(numb);
	TexturImage[number] = SDL_CreateTextureFromSurface(render, SurfImage[number]);
	gems[number].GemTexture = TexturImage[number];
}
void DrawCells(SDL_Renderer*& renderer, Gems gem[][8], Gems gems[], int level[][8])
{
	srand(time(NULL));
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			SDL_RenderCopy(renderer, gems[level[i][j]].GemTexture, NULL, &gem[i][j].CardRect);
		}
	}
}
bool CheckMenuHit(SDL_Rect MenuButtons, int x, int y)
{
	int CenterX = MenuButtons.x + (MenuButtons.w / 2);
	int CenterY = MenuButtons.y + (MenuButtons.h / 2);
	return (CenterX + MenuButtons.w / 2 > x) && (CenterX - MenuButtons.w / 2 < x) && (CenterY + MenuButtons.h / 2 > y) && (CenterY - MenuButtons.h / 2 < y);
}
void SetBut(SDL_Renderer* render, SDL_Rect SettingsButtons[])
{
	for (int i = 0; i < 3; i++)
	{
		SettingsButtons[i] = { 80 + i * 200, 50 + i * 200, 50, 50 };
	}
}
void UpdateRec(int& time1, int& time2, int& time3, int& RecordVisionTime)
{
	bool CheckRecConf = 0;
	if (((RecordVisionTime < time1) || (time1 == 0)) && (CheckRecConf == 0))
	{
		time3 = time2;
		time2 = time1;
		time1 = RecordVisionTime;
		CheckRecConf = 1;
	}
	if (((RecordVisionTime < time2) || (time2 == 0)) && (CheckRecConf == 0))
	{
		time3 = time2;
		time2 = RecordVisionTime;
		CheckRecConf = 1;
	}
	if (((RecordVisionTime < time3) || (time3 == 0)) && (CheckRecConf == 0))
	{
		time3 = RecordVisionTime;
		CheckRecConf = 1;
	}
}