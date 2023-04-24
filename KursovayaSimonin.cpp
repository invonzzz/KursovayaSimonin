#pragma comment(lib, "SDL2_mixer.lib")
#include <stdio.h>
#include <iostream>
#include <SDL.h>
#include <stdlib.h>
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include "SDL_image.h"
#include <ctime>
const int W = 700;
const int H = 700;
#define NumbPict 6

struct Gems
{
	SDL_Texture* GemTexture;
	int CardNumb;
	int CheckNumb;
	SDL_Rect CardRect;
	bool check_open = 0;
	SDL_Rect BackRect;
	bool full_open = 0;
};  


void loadmusic(Mix_Music* fon)
{
	fon = Mix_LoadMUS("fon_music.wav");
	Mix_PlayMusic(fon, -1);
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

bool CheckMenuHit(SDL_Rect MenuButtons, int x, int y)
{
	int CenterX = MenuButtons.x + (MenuButtons.w / 2);
	int CenterY = MenuButtons.y + (MenuButtons.h / 2);
	return (CenterX + MenuButtons.w / 2 > x) && (CenterX - MenuButtons.w / 2 < x) && (CenterY + MenuButtons.h / 2 > y) && (CenterY - MenuButtons.h / 2 < y);
}

int main(int argc, char* argv[])
{
	SDL_Renderer* renderer = NULL;
	Mix_Music* fonmusic = NULL;
	SDL_Texture* TexturImage[NumbPict];
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
			SDL_RenderCopy(renderer, TexturMenu, NULL, &MenuRect);
			SDL_Rect MenuButtons[5];
			for (int i = 0; i < 5; i++) MenuButtons[i] = {MenuRect.x + 6, MenuRect.y + 6 + 80 * i, MenuRect.w - 12, 50};

			Mix_Init(0);
			Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024);
			Mix_VolumeMusic(10);
			loadmusic(fonmusic);

			/*SDL_Surface* SurfImage[NumbPict];
			Gems GemGame[64];
			SurfImage[0] = IMG_Load("back.bmp");
			TexturImage[0] = SDL_CreateTextureFromSurface(renderer, SurfImage[0]);
			for (int i = 1; i < NumbPict; i++) UploadPict(renderer, SurfImage, i, TexturImage, GemGame);
			for (int i = 1; i < 64; i++) GemGame[i].GemTexture = SDL_CreateTextureFromSurface(renderer, SurfImage[i]);
			for (int i = 0; i < NumbPict; i++) SDL_FreeSurface(SurfImage[i]);*/
			
			int Check_Window = 0;

			SDL_Event event;
			bool quit = 0;
			while (!quit)
			{
				while (Check_Window == 0)
				{
					while (SDL_PollEvent(&event))
					{
						if (event.type == SDL_QUIT)
						{
							Check_Window = -1;
							quit = 1;
						}
						if (event.type == SDL_MOUSEBUTTONDOWN && (event.button.button == SDL_BUTTON_LEFT) || (event.button.button == SDL_BUTTON_RIGHT))
						{
							for (int i = 0; i < 5; i++)
							{
								if (CheckMenuHit(MenuButtons[i], event.button.x, event.button.y))
								{
									if (i == 4)
									{
										Check_Window = -1;
										quit = 1;
									}
									std::cout << i << std::endl;
								}
							}
						}
					}
					SDL_RenderPresent(renderer);
				}
			}
			Mix_FreeMusic(fonmusic);
			Mix_CloseAudio();
			SDL_DestroyRenderer(renderer);
			SDL_DestroyWindow(window);
			SDL_Quit();
		}
	}
    return 0;
}