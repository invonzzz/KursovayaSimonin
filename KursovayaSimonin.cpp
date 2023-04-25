﻿#pragma comment(lib, "SDL2_mixer.lib")
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
const int W = 700;
const int H = 700;
#define NumbPict 6

void RandomLevelGen(int level[][8])
{
	srand(time(NULL));
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			level[i][j] = rand() % 6;
		}
	}
}
struct Gems
{
	SDL_Texture* GemTexture;
	int CardNumb;
	int CheckNumb;
	SDL_Rect CardRect;
	bool check_open = 0;
	bool full_open = 0;
};  

SDL_Texture* get_text_texture(SDL_Renderer*& renderer, char* text, TTF_Font* font, SDL_Color fore_color)
{
	SDL_Surface* textSurface = NULL;
	SDL_Color back_color = { 40, 94, 42};
	textSurface = TTF_RenderText_Blended(font, text, fore_color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_FreeSurface(textSurface);
	return texture;
}

void draw_text(SDL_Renderer*& renderer, SDL_Texture* texture, SDL_Rect rect)
{
	SDL_RenderCopy(renderer, texture, NULL, &rect);
}

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

bool CheckSecondTap(int i, int j, int i1, int j1)
{
	if (i == i1 + 1 && j == j1) return 1;
	if (i == i1 - 1 && j == j1) return 1;
	if (j == j1 + 1 && i == i1) return 1;
	if (j == j1 - 1 && i == i1) return 1;
	else return 0;
}
bool CheckCombination(int level[][8], int i, int j, int i1, int j1)
{
	if (level[j][i] == level[j - 1][i] == level[j + 1][i]) return 1;
	if (level[j][i] == level[j + 1][i] == level[i + 2][i]) return 1;
	if (level[j][i] == level[j - 1][i] == level[j - 2][i]) return 1;

	if (level[j1][i1] == level[j1 - 1][i1] == level[j1 + 1][i1]) return 1;
	if (level[j1][i1] == level[j1 + 1][i1] == level[i1 + 2][i1]) return 1;
	if (level[j1][i1] == level[j1 - 1][i1] == level[j1 - 2][i1]) return 1;

	/*if (level[i1][j1] == level[i1 - 1][j1] == level[i1 + 1][j1]) return 1;
	if (level[i1][j1] == level[i1 + 1][j1] == level[i1 + 2][j1]) return 1;
	if (level[i1][j1] == level[i1 - 1][j1] == level[i1 - 2][j1]) return 1;*/

	else return 0;
}

void CheckGeneration(int level[][8])
{
	int count = 0;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (level[i][j] == 0)
			{
				count += 1;
			}
		}
	}
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (level[i][j] == 0)
			{
				if (count > 1)
				{
					level[i][j] = rand() % (5 - 1 + 1) + 1;
					count--;
				}
			}
		}
	}
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			if (level[i][j] == 0 && level[i][j+1] == level[i][j+2])
			{
				int temp = level[i][j+1];
				while (temp == level[i][j+1]) level[i][j+1] = rand() % (5 - 1 + 1) + 1;
			}
		}
	}
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (level[i][j] == 0 && level[i+1][j] == level[i+2][j])
			{
				int temp = level[i+1][j];
				while (temp == level[i+1][j]) level[i+1][j] = rand() % (5 - 1 + 1) + 1;
			}
		}
	}
	//ПРОВЕРКА 3 ОДИНАКОВЫХ
	for (int i = 0; i < 8; i++)
	{
		for (int j = 1; j < 7; j++)
		{
			if ((level[i][j] == level[i][j - 1]) && (level[i][j] == level[i][j + 1]))
			{
				int temp = level[i][j];
				while (temp == level[i][j]) level[i][j] = rand() % (5 - 1 + 1) + 1;
			}
		}
	}
	//ПРОВЕРКА 3 ОДИНАКОВЫХ
	for (int i = 1; i < 7; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if ((level[i][j] == level[i - 1][j]) && (level[i][j] == level[i + 1][j]))
			{
				int temp = level[i][j];
				while (temp == level[i][j]) level[i][j] = rand() % (5 - 1 + 1) + 1;
			}
		}
	}
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

int main(int argc, char* argv[])
{
	SDL_Renderer* renderer = NULL;
	Mix_Chunk* Sound = NULL;
	Mix_Music* fonmusic = NULL;
	SDL_Texture* TexturImage[6];
	SDL_Texture* SettingsBut[4];
	SDL_Texture* textTexture = NULL;
	TTF_Font* my_font = NULL;
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) printf("SDL не смог запуститься! SDL_Error: %s\n", SDL_GetError());
	else
	{
		SDL_Window* window = SDL_CreateWindow(u8"Курсовая Симонин С.А. 'Три в ряд'", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, W, H, SDL_WINDOW_SHOWN);
		if (window == NULL) printf("Окно не может быть создано! SDL_Error: %s\n", SDL_GetError());
		else
		{
			renderer = SDL_CreateRenderer(window, -1, 0);


			SDL_Surface* Fon = IMG_Load("fon.bmp");
			SDL_Rect FonRect = { 0, 0, 0 + W, 0 + H };
			SDL_Texture* TexturFon = SDL_CreateTextureFromSurface(renderer, Fon);
			SDL_FreeSurface(Fon);
			SDL_RenderCopy(renderer, TexturFon, NULL, &FonRect);

			SDL_Surface* Menu_Button = IMG_Load("menu.bmp");
			SDL_Rect MenuRect = { W / 2 - 217, H / 2 - 152, 435, 374 };
			SDL_SetColorKey(Menu_Button, SDL_TRUE, SDL_MapRGB(Menu_Button->format, 255, 255, 255));
			SDL_Texture* TexturMenu = SDL_CreateTextureFromSurface(renderer, Menu_Button);
			SDL_FreeSurface(Menu_Button);
			SDL_Rect MenuButtons[5];
			for (int i = 0; i < 5; i++) MenuButtons[i] = {MenuRect.x + 6, MenuRect.y + 6 + 80 * i, MenuRect.w - 12, 50};

			int level1[8][8];
			RandomLevelGen(level1);
			int CheckCardOpeni = 0;
			int CheckCardOpenj = 0;
			int CheckCardOpeni2 = 0;
			int CheckCardOpenj2 = 0;
			int check2try = 0;
			int temped[2];

			SDL_Surface* SettingsButtons[4];

			SettingsButtons[0] = IMG_Load("SoundButton.bmp");
			SettingsButtons[1] = IMG_Load("MusicButton.bmp");
			SettingsButtons[2] = IMG_Load("ScreenButton.bmp");
			SettingsButtons[3] = IMG_Load("BackButton.bmp");
			for (int i = 0; i < 4; i++)SettingsBut[i] = SDL_CreateTextureFromSurface(renderer, SettingsButtons[i]);

			for (int i = 0; i < 4; i++) SDL_FreeSurface(SettingsButtons[i]);
			SDL_Rect SetButtons[4];
			for (int i = 0; i < 4; i++) SetButtons[i] = { W/4 + 100*i, H / 2, 50, 50};

			TTF_Init();
			my_font = TTF_OpenFont("arial.ttf", 100);
			int time1 = 0, time2 = 0, time3 = 0;
			std::ifstream file;
			file.open("Records.txt");
			while (!file.eof()) 
			{
				file >> time1 >> time2 >> time3;
			}
			file.close();
			char time1txt[10], time2txt[10], time3txt[10];
			char BestTime[55] = "Your best times: ";
			_itoa_s(time1, time1txt, 10);
			_itoa_s(time2, time2txt, 10);
			_itoa_s(time3, time3txt, 10);
			strcat_s(BestTime, time1txt); strcat_s(BestTime, "s"); strcat_s(BestTime, " ");
			strcat_s(BestTime, time2txt); strcat_s(BestTime, "s"); strcat_s(BestTime, " ");
			strcat_s(BestTime, time3txt); strcat_s(BestTime, "s");
			SDL_Color BestTimeColor = { 0, 0, 0};
			SDL_Rect BestTimeRect = { W / 2 - 250, H / 2 - 50, 500, 100 };
			SDL_Texture* ResTexture = get_text_texture(renderer, BestTime, my_font, BestTimeColor);
			SDL_Rect BackButtonRect = { W - SetButtons[3].w, H - SetButtons[3].h, 50, 50 };
			
			int Music = 100, tempmusic;
			int SoundTap = 100, tempsoundtap;
			Mix_Init(0);
			Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024);
			Mix_VolumeMusic(Music);
			loadmusic(fonmusic);

			SDL_Surface* SurfImage[6];
			Gems GemGame[6];
			for (int i = 0; i < 6; i++) UploadPict(renderer, SurfImage, i, TexturImage, GemGame);
			for (int i = 0; i < 6; i++) SDL_SetColorKey(SurfImage[i], SDL_TRUE, SDL_MapRGB(SurfImage[i]->format, 255, 255, 255));
			for (int i = 0; i < 6; i++) GemGame[i].GemTexture = SDL_CreateTextureFromSurface(renderer, SurfImage[i]);
			for (int i = 0; i < 6; i++) SDL_FreeSurface(SurfImage[i]);

			Gems gem[8][8];
			for (int i = 0; i < 8; i++)
			{
				for (int j = 0; j < 8; j++)
				{
					gem[i][j].CardRect = { W/4 - 25 + 50 * i, W/4 - 25 + 50 * j, 50, 50 };
				}
			}
			
			int Check_Window = 0;
			SDL_Event event;
			bool quit = 0;
			while (!quit)
			{			
				while (Check_Window == 0)
				{
					SDL_RenderCopy(renderer, TexturFon, NULL, &FonRect);
					while (SDL_PollEvent(&event))
					{
						if (event.type == SDL_QUIT)
						{
							Check_Window = -1;
							quit = 1;
						}
						if (event.type == SDL_MOUSEBUTTONDOWN && (event.button.button == SDL_BUTTON_LEFT))
						{
							for (int i = 0; i < 5; i++)
							{
								if (CheckMenuHit(MenuButtons[i], event.button.x, event.button.y))
								{
									tapsound(Sound);
									if (i == 4)
									{
										Check_Window = -1;
										quit = 1;
									}
									if (i == 3) Check_Window = 4;
									if (i == 2) Check_Window = 3;
									//if (i == 1) Check_Window = 2;
									if (i == 0)
									{
										Check_Window = 1;
										RandomLevelGen(level1);
										CheckGeneration(level1);
									}
								}
							}
						}
					}
					SDL_RenderCopy(renderer, TexturMenu, NULL, &MenuRect);
					SDL_RenderPresent(renderer);
				}
				while (Check_Window == 1)
				{
					SDL_RenderCopy(renderer, TexturFon, NULL, &FonRect);
					while (SDL_PollEvent(&event))
					{
						if (event.type == SDL_QUIT)
						{
							Check_Window = -1;
							quit = 1;
						}
						if (event.type == SDL_MOUSEBUTTONDOWN && (event.button.button == SDL_BUTTON_LEFT))
						{
							if (event.type == SDL_MOUSEBUTTONDOWN && (event.button.button == SDL_BUTTON_LEFT))
							{
								if (CheckMenuHit(BackButtonRect, event.button.x, event.button.y))
								{
									tapsound(Sound);
									Check_Window = 0;
								}
								for (int i = 0; i < 8; i++)
								{
									for (int j = 0; j < 8; j++)
									{
										if (CheckMenuHit(gem[i][j].CardRect, event.button.x, event.button.y))
										{
											if (check2try == 1 && CheckSecondTap(CheckCardOpeni, CheckCardOpenj, i, j))
											{
												CheckCardOpeni2 = i;
												CheckCardOpenj2 = j;
												check2try += 1;
											}
											if (check2try == 1 && !CheckSecondTap(CheckCardOpeni, CheckCardOpenj, i, j))
											{
												check2try = 0;
											}
											if (check2try == 0)
											{
												CheckCardOpeni = i;
												CheckCardOpenj = j;
											}
											if (check2try == 0) check2try += 1;
										}
									}
								}
							}
						}
					}
					if (check2try == 2)
					{
						std::swap(level1[CheckCardOpeni][CheckCardOpenj], level1[CheckCardOpeni2][CheckCardOpenj2]);
						for (int i = 0; i < 8; i++)
						{
							for (int j = 0; j < 8; j++)
							{
								std::cout << level1[j][i] << " ";
							}
							std::cout << "\n";
						}
						std::cout << CheckCombination(level1, CheckCardOpeni, CheckCardOpenj, CheckCardOpeni2, CheckCardOpenj2) << std::endl;
						check2try = 0;
					}
					DrawCells(renderer, gem, GemGame, level1);
					SDL_RenderCopy(renderer, SettingsBut[3], NULL, &BackButtonRect);
					SDL_RenderPresent(renderer);
				}
				while (Check_Window == 3)
				{
					SDL_RenderCopy(renderer, TexturFon, NULL, &FonRect);
					while (SDL_PollEvent(&event))
					{

						if (event.type == SDL_QUIT)
						{
							Check_Window = -1;
							quit = 1;
						}
						if (event.type == SDL_MOUSEBUTTONDOWN && (event.button.button == SDL_BUTTON_LEFT))
						{
							if (CheckMenuHit(BackButtonRect, event.button.x, event.button.y))
							{
								tapsound(Sound);
								Check_Window = 0;
							}
						}
					}
					draw_text(renderer, ResTexture, BestTimeRect);
					SDL_RenderCopy(renderer, SettingsBut[3], NULL, &BackButtonRect);
					SDL_RenderPresent(renderer);
				}
				while (Check_Window == 4)
				{
					SDL_RenderCopy(renderer, TexturFon, NULL, &FonRect);
					while (SDL_PollEvent(&event))
					{
						if (event.type == SDL_QUIT)
						{
							Check_Window = -1;
							quit = 1;
						}
						if (event.type == SDL_MOUSEBUTTONDOWN && (event.button.button == SDL_BUTTON_LEFT))
						{
							for (int i = 0; i < 4; i++)
							{
								if (CheckMenuHit(SetButtons[i], event.button.x, event.button.y))
								{
									tapsound(Sound);
									if (i == 0)
									{
										if (SoundTap == 100) tempsoundtap = 0;
										if (SoundTap == 0) tempsoundtap = 100;
										SoundTap = tempsoundtap;
										Mix_Volume(-1, SoundTap);
									}
									if (i == 1)
									{
										if (Music == 100) tempmusic = 0;
										if (Music == 0) tempmusic = 100;
										Music = tempmusic;
										Mix_VolumeMusic(Music);
									}
									if (i == 2)
									{

									}
									if (i == 3)	Check_Window = 0;
								}
							}
						}
					}
					for (int i = 0; i < 4; i++)
					{
						SDL_RenderCopy(renderer, SettingsBut[i], NULL, &SetButtons[i]);
					}
					SDL_RenderPresent(renderer);
				}
			}
			for (int i = 0; i < 4; i++) SDL_DestroyTexture(SettingsBut[i]);
			SDL_DestroyTexture(ResTexture);
			SDL_DestroyTexture(TexturMenu);
			TTF_CloseFont(my_font);
			TTF_Quit();
			Mix_FreeMusic(fonmusic);
			Mix_FreeChunk(Sound);
			Mix_CloseAudio();
			SDL_DestroyRenderer(renderer);
			SDL_DestroyWindow(window);
			SDL_Quit();
		}
	}
    return 0;
}