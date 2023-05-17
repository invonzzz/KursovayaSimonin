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
void UpdateRects(bool fc, int W, int H, SDL_Rect& FonRect, SDL_Rect& MenuRect, SDL_Rect MenuButtons[], SDL_Rect SetButtons[], SDL_Rect& BackButtonRect, SDL_Rect PauseButtonsRect[], SDL_Rect& GridRect, SDL_Rect& BestTimeRect, SDL_Rect& FileErrorRect, SDL_Rect& WinMessageRect, SDL_Rect& ShowTimeAGRect, SDL_Rect& PointsRect, SDL_Rect& PointsFNRect, SDL_Rect& TimerRect, Gems gem[][8])
{
	MenuRect = { W / 2 - 217, H / 2 - 152, 435, 374 };
	for (int i = 0; i < 5; i++) MenuButtons[i] = { MenuRect.x + 6, MenuRect.y + 6 + 80 * i, MenuRect.w - 12, 50 };
	SetButtons[0] = { W / 2 - 175, H / 2, 50, 50 };
	SetButtons[1] = { W / 2 - 75, H / 2, 50, 50 };
	SetButtons[2] = { W / 2 + 25, H / 2, 50, 50 };
	SetButtons[3] = { W / 2 + 125, H / 2, 50, 50 };
	BackButtonRect = { W - SetButtons[3].w, H - SetButtons[3].h, 50, 50 };
	PauseButtonsRect[0] = { BackButtonRect.x - BackButtonRect.w - 25, H - SetButtons[3].h, 50, 50 };
	PauseButtonsRect[1] = { PauseButtonsRect[0].x - PauseButtonsRect[0].w - 25, H - SetButtons[3].h, 50, 50 };
	GridRect = { W / 2 - 240, H / 4 - 25, 480, 480 };
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			gem[i][j].CardRect = { W / 2 - 240 + 60 * i, H / 4 - 25 + 60 * j, 60, 60 };
		}
	}
	if (fc == 0)
	{
		PointsRect = { W - 750, H - 750, 150, 50 };
		PointsFNRect = { W - 200, H - 750, 150, 50 };
		TimerRect = { W / 2 - 75, H - 750, 150, 50 };
	}
	else
	{
		PointsRect = { W / 2 - 500, H - 150, 150, 50 };
		PointsFNRect = { W / 2 + 500, H - 150, 150, 50 };
		TimerRect = { W / 2 - 75, H - 150, 150, 50 };

	}
	BestTimeRect = { W / 2 - 250, H / 2 - 50, 500, 100 };
	FileErrorRect = { W / 2 - 250, H / 2 - 50, 500, 100 };
	WinMessageRect = { W / 2 - 250, H / 2 - 50, 500, 100 };
	ShowTimeAGRect = { WinMessageRect.x + WinMessageRect.w + 20, WinMessageRect.y, 50, 100 };
}