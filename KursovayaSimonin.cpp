#pragma comment(lib, "SDL2_mixer.lib")
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
#define Setka 8

void RandomLevelGen(int level[][8])
{
	srand(time(NULL));
	for (int i = 0; i < Setka; i++)
	{
		for (int j = 0; j < Setka; j++)
		{
			level[i][j] = rand() % 6;
		}
	}
}
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

bool CheckSecondTap(int i, int j, int i1, int j1)
{
	if (i == i1 + 1 && j == j1) return 1;
	if (i == i1 - 1 && j == j1) return 1;
	if (j == j1 + 1 && i == i1) return 1;
	if (j == j1 - 1 && i == i1) return 1;
	else return 0;
}
int CheckCombinationLeftRight(int level[][8], int j1, int i1)
{
	int countcombright = 0;
	int countcombleft = 0;
	for (int q = 1; q <= 7-j1; q++)
	{
		if ((level[j1 + q][i1] == level[j1][i1]) || (level[j1 + q][i1] == 0))
		{
			countcombright += 1;
		}
		if ((level[j1 + q][i1] != level[j1][i1]) && (level[j1 + q][i1] != 0))
		{
			break;
		}
	}
	for (int q = 1; q <= j1; q++)
	{
		if ((level[j1 - q][i1] == level[j1][i1]) || (level[j1 - q][i1] == 0))
		{
			countcombleft += 1;
		}
		if ((level[j1 - q][i1] != level[j1][i1]) && (level[j1 - q][i1] != 0))
		{
			break;
		}
	}
	return countcombleft + countcombright + 1;
}
int CheckCombinationLeft(int level[][8], int j1, int i1)
{
	int countcombleft = 0;
	for (int q = 1; q <= j1; q++)
	{
		if ((level[j1 - q][i1] == level[j1][i1]) || (level[j1 - q][i1] == 0))
		{
			countcombleft += 1;
		}
		if ((level[j1 - q][i1] != level[j1][i1]) && (level[j1 - q][i1] != 0))
		{
			break;
		}
	}
	return countcombleft;
}
int CheckCombinationRight(int level[][8], int j1, int i1)
{
	int countcombright = 0;
	for (int q = 1; q <= 7 - j1; q++)
	{
		if ((level[j1 + q][i1] == level[j1][i1]) || (level[j1 + q][i1] == 0))
		{
			countcombright += 1;
		}
		if ((level[j1 + q][i1] != level[j1][i1]) && (level[j1 + q][i1] != 0))
		{
			break;
		}
	}
	return countcombright;
}
int CheckCombinationDown(int level[][8], int j1, int i1)
{
	int countcombup = 0;
	for (int q = 1; q <= 7 - i1; q++)
	{
		if ((level[j1][i1 + q] == level[j1][i1]) || (level[j1][i1 + q] == 0))
		{
			countcombup += 1;
		}
		if ((level[j1][i1 + q] != level[j1][i1]) && (level[j1][i1 + q] != 0))
		{
			break;
		}
	}
	return countcombup;
}
int CheckCombinationUp(int level[][8], int j1, int i1)
{
	int countcombdown = 0;
	for (int q = 1; q <= i1; q++)
	{
		if ((level[j1][i1 - q] == level[j1][i1]) || (level[j1][i1 - q] == 0))
		{
			countcombdown += 1;
		}
		if ((level[j1][i1 - q] != level[j1][i1]) && (level[j1][i1 - q] != 0))
		{
			break;
		}
	}
	return countcombdown;
}
int CheckCombinationUpDown(int level[][8], int j1, int i1)
{
	int countcombup = 0;
	int countcombdown = 0;
	for (int q = 1; q <= 7 - i1; q++)
	{
		if ((level[j1][i1+q] == level[j1][i1]) || (level[j1][i1 + q] == 0))
		{
			countcombup += 1;
		}
		if ((level[j1][i1+q] != level[j1][i1]) && (level[j1][i1 + q] != 0))
		{
			break;
		}
	}
	for (int q = 1; q <= i1; q++)
	{
		if ((level[j1][i1-q] == level[j1][i1]) || (level[j1][i1 - q] == 0))
		{
			countcombdown += 1;
		}
		if ((level[j1][i1-q] != level[j1][i1]) && (level[j1][i1 - q] != 0))
		{
			break;
		}
	}
	return countcombdown + countcombup + 1;
}
void BrokeLeftRight(int level[][8], int stolb, int begin, int end)
{
	int temp[8][8];
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			temp[i][j] = level[j][i];
		}
	}
	for (int i = stolb; i > 0; i--)
	{
		for (int j = begin; j <= end; j++)
		{
			temp[i][j] = temp[i - 1][j];
		}
	}
	for (int i = begin; i <= end; i++)
	{
		temp[0][i] = rand() % (5 - 1 + 1) + 1;
	}
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			level[j][i] = temp[i][j];
		}
	}
}
void BrokeUpDown(int level[][8], int stolb, int begin, int end)
{
	int temp[8][8];
	int freegems = begin; 
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			temp[i][j] = level[j][i];
		}
	}
	for (int i = 0; i <= end; i++)
	{
		if (i < freegems) temp[end - i][stolb] = temp[begin - i - 1][stolb];
		else temp[end - i][stolb] = rand() % (5 - 1 + 1) + 1;
	}
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			level[j][i] = temp[i][j];
		}
	}
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
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			if ((level[i][j] == 0) && (level[i][j + 1] == level[i][j + 2]))
			{
				int temp = level[i][j + 1];
				while (temp == level[i][j + 1]) level[i][j + 1] = rand() % (5 - 1 + 1) + 1;
			}
		}
	}
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if ((level[i][j] == 0) && (level[i + 1][j] == level[i + 2][j]))
			{
				int temp = level[i + 1][j];
				while (temp == level[i + 1][j]) level[i + 1][j] = rand() % (5 - 1 + 1) + 1;
			}
		}
	}
	for (int i = 0; i < 8; i++)
	{
		for (int j = 1; j < 7; j++)
		{
			if ((level[i][j] == 0) && (level[i][j + 1] == level[i][j - 1]))
			{
				int temp = level[i][j - 1];
				while (temp == level[i][j - 1]) level[i][j - 1] = rand() % (5 - 1 + 1) + 1;
			}
		}
	}
	for (int i = 0; i < 7; i++)
	{
		for (int j = 1; j < 8; j++)
		{
			if ((level[i][j] == 0) && (level[i + 1][j] == level[i - 1][j]))
			{
				int temp = level[i - 1][j];
				while (temp == level[i - 1][j]) level[i - 1][j] = rand() % (5 - 1 + 1) + 1;
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
	SDL_Texture* PauseBut[2];
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
			Fon = nullptr;
			SDL_RenderCopy(renderer, TexturFon, NULL, &FonRect);

			SDL_Surface* Menu_Button = IMG_Load("menu.bmp");
			SDL_Rect MenuRect = { W / 2 - 217, H / 2 - 152, 435, 374 };
			SDL_SetColorKey(Menu_Button, SDL_TRUE, SDL_MapRGB(Menu_Button->format, 255, 255, 255));
			SDL_Texture* TexturMenu = SDL_CreateTextureFromSurface(renderer, Menu_Button);
			SDL_FreeSurface(Menu_Button);
			SDL_Rect MenuButtons[5];
			for (int i = 0; i < 5; i++) MenuButtons[i] = {MenuRect.x + 6, MenuRect.y + 6 + 80 * i, MenuRect.w - 12, 50};

			SDL_Surface* BackNumb = IMG_Load("BackNumb.bmp");
			SDL_SetColorKey(BackNumb, SDL_TRUE, SDL_MapRGB(BackNumb->format, 255, 255, 255));
			SDL_Texture* TexturBackNumb = SDL_CreateTextureFromSurface(renderer, BackNumb);
			SDL_FreeSurface(BackNumb);
			BackNumb = nullptr;

			SDL_Surface* grid = IMG_Load("grid.bmp");
			SDL_Texture* TexturGrid = SDL_CreateTextureFromSurface(renderer, grid);
			SDL_FreeSurface(grid);
			grid = nullptr;
			SDL_Rect GridRect = { W / 4 - 25, H / 4 - 25, 400, 400 };

			int level1[8][8];
			RandomLevelGen(level1);
			int CheckCardOpeni = 0;
			int CheckCardOpenj = 0;
			int CheckCardOpeni2 = 0;
			int CheckCardOpenj2 = 0;
			int check2try = 0;

			int TimeStartProgram;
			int TimeStartProgram2 = 0;
			int TimeStartGame;

			SDL_Surface* SettingsButtons[6];
			SettingsButtons[0] = IMG_Load("SoundButton.bmp");
			SettingsButtons[1] = IMG_Load("MusicButton.bmp");
			SettingsButtons[2] = IMG_Load("ScreenButton.bmp");
			SettingsButtons[3] = IMG_Load("BackButton.bmp");
			for (int i = 0; i < 4; i++)SettingsBut[i] = SDL_CreateTextureFromSurface(renderer, SettingsButtons[i]);
			for (int i = 0; i < 4; i++)
			{
				SDL_FreeSurface(SettingsButtons[i]);
				SettingsButtons[i] = nullptr;
			}
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

			SDL_Surface* PauseButtons[2];
			SDL_Rect PauseButtonsRect[2];
			PauseButtons[0] = IMG_Load("save.bmp");
			PauseButtons[1] = IMG_Load("cont.bmp");
			SDL_SetColorKey(PauseButtons[0], SDL_TRUE, SDL_MapRGB(PauseButtons[0]->format, 255, 255, 255));
			SDL_SetColorKey(PauseButtons[1], SDL_TRUE, SDL_MapRGB(PauseButtons[1]->format, 255, 255, 255));
			for (int i = 0; i < 2; i++)PauseBut[i] = SDL_CreateTextureFromSurface(renderer, PauseButtons[i]);
			for (int i = 0; i < 2; i++)
			{
				SDL_FreeSurface(PauseButtons[i]);
				PauseButtons[i] = nullptr;
			}
			PauseButtonsRect[0] = { BackButtonRect.x - BackButtonRect.w - 25, H - SetButtons[3].h, 50, 50};
			PauseButtonsRect[1] = { PauseButtonsRect[0].x - PauseButtonsRect[0].w - 25, H - SetButtons[3].h, 50, 50};

			char WinMessage[55] = "Graz!!! You Win! ";
			SDL_Rect WinMessageRect = { W / 2 - 250, H / 2 - 50, 500, 100 };
			SDL_Color WinMessageColor = { 0, 0, 0};
			SDL_Texture* WinMessageTexture = get_text_texture(renderer, WinMessage, my_font, WinMessageColor);
			
			int numberlevel = 0;
			levels Lvl[5];
			Lvl[0].PointsLevel = 3000; Lvl[0].TimeLevel = 50; //3000
			Lvl[1].PointsLevel = 4000; Lvl[1].TimeLevel = 50; //4500
			Lvl[2].PointsLevel = 600; Lvl[2].TimeLevel = 175; //6000
			Lvl[3].PointsLevel = 800; Lvl[3].TimeLevel = 200; //8000
			Lvl[4].PointsLevel = 1000; Lvl[4].TimeLevel = 250; //10000

			int points = 0;
			char Points[10];
			_itoa_s(points, Points, 10);
			SDL_Rect PointsRect = { 50, 50, 150, 50 };
			SDL_Color PointsColor = { 49, 125, 37 };
			SDL_Texture* PointsTexture = get_text_texture(renderer, Points, my_font, PointsColor);

			int level1time = Lvl[0].TimeLevel;
			char leveltime[10];
			_itoa_s(level1time, leveltime, 10);
			SDL_Rect TimerRect = { W/2-75, 50, 150, 50 };
			SDL_Color TimerColor = { 252, 56, 56 };
			SDL_Texture* TimerTexture = get_text_texture(renderer, leveltime, my_font, TimerColor);

			int PointsForWin = Lvl[0].PointsLevel;
			char PointsToWin[10];
			_itoa_s(PointsForWin, PointsToWin, 10);
			int Music = 100, tempmusic;
			int SoundTap = 50, tempsoundtap;
			Mix_Init(0);
			Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024);
			Mix_VolumeMusic(Music);
			loadmusic(fonmusic);

			SDL_Surface* SurfImage[6];
			Gems GemGame[6];
			for (int i = 0; i < 6; i++) UploadPict(renderer, SurfImage, i, TexturImage, GemGame);
			for (int i = 0; i < 6; i++) SDL_SetColorKey(SurfImage[i], SDL_TRUE, SDL_MapRGB(SurfImage[i]->format, 255, 255, 255));
			for (int i = 0; i < 6; i++) GemGame[i].GemTexture = SDL_CreateTextureFromSurface(renderer, SurfImage[i]);
			for (int i = 0; i < 6; i++)
			{
				SDL_FreeSurface(SurfImage[i]);
				SurfImage[i] = nullptr;
			}

			Gems gem[8][8];
			for (int i = 0; i < 8; i++)
			{
				for (int j = 0; j < 8; j++)
				{
					gem[i][j].CardRect = { W/4 - 25 + 50 * i, H/4 - 25 + 50 * j, 50, 50 };
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
										TimeStartGame = (SDL_GetTicks() / 1000);
										level1time = Lvl[0].TimeLevel;
										Check_Window = 1;
										points = 0;
										_itoa_s(points, Points, 10);
										PointsTexture = get_text_texture(renderer, Points, my_font, PointsColor);
										RandomLevelGen(level1);
										CheckGeneration(level1);
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
					TimeStartProgram = (SDL_GetTicks() / 1000) - TimeStartGame;
					if (TimeStartProgram2 != TimeStartProgram)
					{
						TimeStartProgram2 = TimeStartProgram;
						level1time -= 1;
						_itoa_s(level1time, leveltime, 10);
						SDL_DestroyTexture(TimerTexture);
						TimerTexture = get_text_texture(renderer, leveltime, my_font, TimerColor);
					}
					SDL_RenderCopy(renderer, TexturFon, NULL, &FonRect);
					SDL_RenderCopy(renderer, TexturBackNumb, NULL, &PointsRect);
					SDL_RenderCopy(renderer, TexturBackNumb, NULL, &TimerRect);
					SDL_RenderCopy(renderer, TexturGrid, NULL, &GridRect);
					while (SDL_PollEvent(&event))
					{
						if (event.type == SDL_QUIT)
						{
							Check_Window = -1;
							quit = 1;
						}
						if ((event.type == SDL_KEYDOWN) && (event.key.keysym.sym == SDLK_ESCAPE))
						{
							Check_Window = 6;
						}
						if (event.type == SDL_MOUSEBUTTONDOWN && (event.button.button == SDL_BUTTON_LEFT))
						{
								if (CheckMenuHit(BackButtonRect, event.button.x, event.button.y))
								{
									tapsound(Sound);
									points = 0;
									numberlevel = 0;
									TimeStartProgram2 = 0;
									SDL_DestroyTexture(PointsTexture);
									//SDL_DestroyTexture(TimerTexture);
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
											if (check2try == 0)
											{
												tapsound(Sound);
												check2try += 1;
											}
										}
									}
								}
						}
					}
					if (check2try == 2)
					{
						int points1 = 0, points2 = 0, points3 = 0, points4 = 0;
						int checkcomb1 = 0;
						int checkcomb2 = 0;
						std::swap(level1[CheckCardOpeni][CheckCardOpenj], level1[CheckCardOpeni2][CheckCardOpenj2]);
						//Первая нажатая фигура
						if (CheckCombinationLeft(level1, CheckCardOpeni2, CheckCardOpenj2) + CheckCombinationRight(level1, CheckCardOpeni2, CheckCardOpenj2) < 2) checkcomb1 += 1;
						else
						{
							points1 = CheckCombinationLeftRight(level1, CheckCardOpeni2, CheckCardOpenj2);
							int BrokenRyadBegin = CheckCardOpeni2 - CheckCombinationLeft(level1, CheckCardOpeni2, CheckCardOpenj2);
							int BrokenRyadEnd = CheckCardOpeni2 + CheckCombinationRight(level1, CheckCardOpeni2, CheckCardOpenj2);
							BrokeLeftRight(level1, CheckCardOpenj2, BrokenRyadBegin, BrokenRyadEnd);
						}
						if (CheckCombinationUp(level1, CheckCardOpeni2, CheckCardOpenj2) + CheckCombinationDown(level1, CheckCardOpeni2, CheckCardOpenj2) < 2) checkcomb1 += 1;
						else
						{
							points2 = CheckCombinationUpDown(level1, CheckCardOpeni2, CheckCardOpenj2);
							int BrokenStolbBegin = CheckCardOpenj2 - CheckCombinationUp(level1, CheckCardOpeni2, CheckCardOpenj2);
							int BrokenStolbEnd = CheckCardOpenj2 + CheckCombinationDown(level1, CheckCardOpeni2, CheckCardOpenj2);
							BrokeUpDown(level1, CheckCardOpeni2, BrokenStolbBegin, BrokenStolbEnd);
						}


						//Вторая нажатая фигура
						if (CheckCombinationLeft(level1, CheckCardOpeni, CheckCardOpenj) + CheckCombinationRight(level1, CheckCardOpeni, CheckCardOpenj) < 2)checkcomb2 += 1;
						else
						{
							points3 = CheckCombinationLeftRight(level1, CheckCardOpeni, CheckCardOpenj);
							int BrokenRyadBegin = CheckCardOpeni - CheckCombinationLeft(level1, CheckCardOpeni, CheckCardOpenj);
							int BrokenRyadEnd = CheckCardOpeni + CheckCombinationRight(level1, CheckCardOpeni, CheckCardOpenj);
							BrokeLeftRight(level1, CheckCardOpenj, BrokenRyadBegin, BrokenRyadEnd);
						}
						if (CheckCombinationUp(level1, CheckCardOpeni, CheckCardOpenj) + CheckCombinationDown(level1, CheckCardOpeni, CheckCardOpenj) < 2) checkcomb2 += 1;
						else
						{
							points4 = CheckCombinationUpDown(level1, CheckCardOpeni, CheckCardOpenj);
							int BrokenStolbBegin = CheckCardOpenj - CheckCombinationUp(level1, CheckCardOpeni, CheckCardOpenj);
							int BrokenStolbEnd = CheckCardOpenj + CheckCombinationDown(level1, CheckCardOpeni, CheckCardOpenj);
							BrokeUpDown(level1, CheckCardOpeni, BrokenStolbBegin, BrokenStolbEnd);
						}
						points += 100* points1 + 100*points2 + 100*points3 + 100*points4;
						if ((checkcomb1 > 1) && (checkcomb2 > 1))
						{
							wrongtapsound(Sound);
							std::swap(level1[CheckCardOpeni][CheckCardOpenj], level1[CheckCardOpeni2][CheckCardOpenj2]);
						}
						else tapsound(Sound);

						_itoa_s(points, Points, 10);
						SDL_DestroyTexture(PointsTexture);
						PointsTexture = get_text_texture(renderer, Points, my_font, PointsColor);
						check2try = 0;
					}
					if (points >= Lvl[numberlevel].PointsLevel)
					{
						numberlevel++;
						level1time = Lvl[numberlevel].TimeLevel;
						points = 0;
						_itoa_s(points, Points, 10);
						SDL_DestroyTexture(PointsTexture);
						//SDL_DestroyTexture(TimerTexture);
						PointsTexture = get_text_texture(renderer, Points, my_font, PointsColor);
						RandomLevelGen(level1);
						CheckGeneration(level1);
					}
					if (level1time == 0)
					{
						losesound(Sound);
						points = 0;
						numberlevel = 0;
						TimeStartProgram2 = 0;
						SDL_DestroyTexture(PointsTexture);
						//SDL_DestroyTexture(TimerTexture);
						Check_Window = 0;
					}
					if (numberlevel > 4)
					{
						numberlevel = 0;
						points = 0;
						SDL_DestroyTexture(PointsTexture);
						Check_Window = 5;
					}
					DrawCells(renderer, gem, GemGame, level1);
					SDL_RenderCopy(renderer, SettingsBut[3], NULL, &BackButtonRect);
					draw_text(renderer, PointsTexture, PointsRect);
					draw_text(renderer, TimerTexture, TimerRect);
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
										if (SoundTap == 50) tempsoundtap = 0;
										if (SoundTap == 0) tempsoundtap = 50;
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
				while (Check_Window == 5)
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
					draw_text(renderer, WinMessageTexture, WinMessageRect);
					SDL_RenderCopy(renderer, SettingsBut[3], NULL, &BackButtonRect);
					SDL_RenderPresent(renderer);
				}
				while (Check_Window == 6)
				{
					while (SDL_PollEvent(&event))
					{

						if (event.type == SDL_QUIT)
						{
							Check_Window = -1;
							quit = 1;
						}
						if ((event.type == SDL_KEYDOWN) && (event.key.keysym.sym == SDLK_ESCAPE))
						{
							Check_Window = 1;
						}
						if (event.type == SDL_MOUSEBUTTONDOWN && (event.button.button == SDL_BUTTON_LEFT))
						{
							if (CheckMenuHit(BackButtonRect, event.button.x, event.button.y))
							{
								tapsound(Sound);
								Check_Window = 0;
							}
							for (int i = 0; i < 2; i++)
							{
								if (CheckMenuHit(PauseButtonsRect[i], event.button.x, event.button.y))
								{
									if (i == 1) Check_Window = 1;
								}
							}
						}
					}
					for (int i = 0; i < 2; i++)
					{
						SDL_RenderCopy(renderer, PauseBut[i], NULL, &PauseButtonsRect[i]);
					}
					SDL_RenderCopy(renderer, SettingsBut[3], NULL, &BackButtonRect);
					SDL_RenderPresent(renderer);
				}
			}
			for (int i = 0; i < 4; i++) SDL_DestroyTexture(SettingsBut[i]);
			for (int i = 0; i < 6; i++) SDL_DestroyTexture(TexturImage[i]);
			SDL_DestroyTexture(ResTexture);
			SDL_DestroyTexture(TexturMenu);
			SDL_DestroyTexture(TexturBackNumb);
			SDL_DestroyTexture(TexturGrid);
			SDL_DestroyTexture(PointsTexture);
			SDL_DestroyTexture(TimerTexture);
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