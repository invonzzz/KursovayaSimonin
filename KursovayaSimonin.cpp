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
#include "Header.h"
#define NumbPict 6
#define Setka 8
int main(int argc, char* argv[])
{
	int W = 800; int fW = 1920;
	int H = 800; int fH = 1080;
	SDL_Renderer* renderer = NULL;
	Mix_Chunk* Sound = NULL;
	Mix_Chunk* SoundTapN = NULL;
	Mix_Chunk* SoundTapW = NULL;
	Mix_Chunk* SoundLose = NULL;
	Mix_Music* fonmusic = NULL;
	SDL_Texture* TexturImage[6];
	SDL_Texture* SettingsBut[4];
	SDL_Texture* PauseBut[2];
	SDL_Texture* textTexture = NULL;
	SDL_Texture* ShowTimeAftGame = NULL;
	TTF_Font* my_font = NULL;
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) printf("SDL не смог запуститься! SDL_Error: %s\n", SDL_GetError());
	else
	{
		SDL_Window* window = SDL_CreateWindow(u8"Курсовая Симонин С.А. 'Три в ряд'", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, W, H, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
		if (window == NULL) printf("Окно не может быть создано! SDL_Error: %s\n", SDL_GetError());
		else
		{
			renderer = SDL_CreateRenderer(window, -1, 0);
//---------Reading Settings from file---------------------------------
			bool FullScreenChecker = 0;
			int Music = 100, tempmusic;
			int SoundTap = 50, tempsoundtap;
			std::ifstream soundin;
			soundin.open("settings.txt");
			if (soundin.is_open())
			{
				if (soundin.peek() == EOF)
				{
					Music = 100; SoundTap = 50; FullScreenChecker = 0;
				}
				else
				{
					soundin >> SoundTap;
					soundin >> Music;
					soundin >> FullScreenChecker;
				}
			}
			else
			{
				Music = 100; SoundTap = 50; FullScreenChecker = 0;
			}
			soundin.close();
//-------------------------------------------------------------------
			SDL_Surface* Fon = IMG_Load("fon.bmp");
			SDL_Rect FonRect;
			SDL_Texture* TexturFon = SDL_CreateTextureFromSurface(renderer, Fon);
			SDL_FreeSurface(Fon);
			Fon = nullptr;

//-----Buttons in Menu----------------------------------------------------------------------------

			SDL_Surface* Menu_Button = IMG_Load("menu.bmp");
			SDL_Rect MenuRect = { W / 2 - 217, H / 2 - 152, 435, 374 };
			SDL_SetColorKey(Menu_Button, SDL_TRUE, SDL_MapRGB(Menu_Button->format, 255, 255, 255));
			SDL_Texture* TexturMenu = SDL_CreateTextureFromSurface(renderer, Menu_Button);
			SDL_FreeSurface(Menu_Button);
			SDL_Rect MenuButtons[5];

			SDL_Surface* BackNumb = IMG_Load("BackNumb.bmp");
			SDL_SetColorKey(BackNumb, SDL_TRUE, SDL_MapRGB(BackNumb->format, 255, 255, 255));
			SDL_Texture* TexturBackNumb = SDL_CreateTextureFromSurface(renderer, BackNumb);
			SDL_FreeSurface(BackNumb);
			BackNumb = nullptr;

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
			
			SDL_Rect BackButtonRect;
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
			
//------------------------------------------------------------------------------------------

//-----------Grid and level-----------------------------------------------------------------
			SDL_Surface* grid = IMG_Load("grid.bmp");
			SDL_Texture* TexturGrid = SDL_CreateTextureFromSurface(renderer, grid);
			SDL_FreeSurface(grid);
			grid = nullptr;
			SDL_Rect GridRect;

			int level1[8][8];
			RandomLevelGen(level1);
			int CheckCardOpeni = 0;
			int CheckCardOpenj = 0;
			int CheckCardOpeni2 = 0;
			int CheckCardOpenj2 = 0;
			int check2try = 0;
//-----------------------------------------------------------------------------------------
			int TimeStartProgram;
			int TimeStartProgram2 = 0;
			int TimeStartGame;

			
//----------font and records menu----------------------------------------------------------
			TTF_Init();
			my_font = TTF_OpenFont("arial.ttf", 100);
			int time1 = 0, time2 = 0, time3 = 0;
			std::ifstream file;
			file.open("Records.txt");
			if (file.is_open())
			{
				while (!file.eof())
				{
					file >> time1 >> time2 >> time3;
				}
			}
			file.close();
			int RecordTime, RecordTime2 = 0, RecordVisionTime = 0;
			char time1txt[10], time2txt[10], time3txt[10];
			char BestTime[55] = "Your best times: ";
			_itoa_s(time1, time1txt, 10);
			_itoa_s(time2, time2txt, 10);
			_itoa_s(time3, time3txt, 10);
			strcat_s(BestTime, time1txt); strcat_s(BestTime, "s"); strcat_s(BestTime, " ");
			strcat_s(BestTime, time2txt); strcat_s(BestTime, "s"); strcat_s(BestTime, " ");
			strcat_s(BestTime, time3txt); strcat_s(BestTime, "s");
			SDL_Color BestTimeColor = { 0, 0, 0};
			SDL_Rect BestTimeRect;
			SDL_Texture* ResTexture = get_text_texture(renderer, BestTime, my_font, BestTimeColor);
//-------------------------------------------------------------------------------------------------
//--------------messages---------------------------------------------------------------------------
			char FileError[55] = "File not found!";
			SDL_Color FileErrorColor = { 0, 0, 0 };
			SDL_Rect FileErrorRect;
			SDL_Texture* FileErrorTexture = get_text_texture(renderer, FileError, my_font, FileErrorColor);

			char WinMessage[55] = "Graz! You Win! Your time:";
			SDL_Rect WinMessageRect;
			SDL_Color WinMessageColor = { 0, 0, 0};
			SDL_Texture* WinMessageTexture = get_text_texture(renderer, WinMessage, my_font, WinMessageColor);
//--------------------------------------------------------------------------------------------------
//------------level_info and ingame interface-------------------------------------------------------
			int numberlevel = 0;
			levels Lvl[5];
			Lvl[0].PointsLevel = 3000; Lvl[0].TimeLevel = 120; //3000
			Lvl[1].PointsLevel = 4500; Lvl[1].TimeLevel = 150; //4500
			Lvl[2].PointsLevel = 6000; Lvl[2].TimeLevel = 175; //6000
			Lvl[3].PointsLevel = 8000; Lvl[3].TimeLevel = 200; //8000
			Lvl[4].PointsLevel = 10000; Lvl[4].TimeLevel = 250; //10000

			SDL_Rect ShowTimeAGRect;

			int points = 0;
			char Points[10];
			_itoa_s(points, Points, 10);
			SDL_Rect PointsRect;
			SDL_Color PointsColor = { 49, 125, 37 };
			SDL_Texture* PointsTexture = get_text_texture(renderer, Points, my_font, PointsColor);

			int pointfn = Lvl[0].PointsLevel;
			char PointsForNext[10];
			_itoa_s(pointfn, PointsForNext, 10);
			SDL_Rect PointsFNRect;
			SDL_Color PointsFNColor = { 49, 125, 37 };
			SDL_Texture* PointsFNTexture = get_text_texture(renderer, PointsForNext, my_font, PointsFNColor);

			int level1time = Lvl[0].TimeLevel;
			char leveltime[10];
			_itoa_s(level1time, leveltime, 10);
			SDL_Rect TimerRect;
			SDL_Color TimerColor = { 252, 56, 56 };
			SDL_Texture* TimerTexture = get_text_texture(renderer, leveltime, my_font, TimerColor);

			int PointsForWin = Lvl[0].PointsLevel;
			char PointsToWin[10];
			_itoa_s(PointsForWin, PointsToWin, 10);
//---------------------------------------------------------------------------------------------------------

//--MUSIC-------------------------------------------------------
			
			Mix_Init(0);
			Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024);
			Mix_VolumeMusic(Music);
			Mix_Volume(-1, SoundTap);
			SoundTapN = Mix_LoadWAV("tap_sound.wav");
			SoundTapW = Mix_LoadWAV("wrong.wav");
			SoundLose = Mix_LoadWAV("LoseGame.wav");
			fonmusic = Mix_LoadMUS("fon_music.wav");
			loadmusic(fonmusic);
//-----------------------------------------------------------------

//------Gem_Pos&Texture--------------------------------------------
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
			
//------------------------------------------------------------------------
			if (FullScreenChecker)
			{
				UpdateRects(FullScreenChecker, fW, fH, FonRect, MenuRect, MenuButtons, SetButtons, BackButtonRect, PauseButtonsRect, GridRect, BestTimeRect, FileErrorRect, WinMessageRect, ShowTimeAGRect, PointsRect, PointsFNRect, TimerRect, gem);
				SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
				FonRect = { 0, 0, fW, fH };
			}
			if (!FullScreenChecker)
			{
				UpdateRects(FullScreenChecker, W, H, FonRect, MenuRect, MenuButtons, SetButtons, BackButtonRect, PauseButtonsRect, GridRect, BestTimeRect, FileErrorRect, WinMessageRect, ShowTimeAGRect, PointsRect, PointsFNRect, TimerRect, gem);
				SDL_SetWindowFullscreen(window, 0);
				SDL_SetWindowSize(window, W, H);
				FonRect = { 0, 0, W, H };
			}
			int Check_Window = 0;
			SDL_Event event;
			bool quit = 0;
			while (!quit)
			{	
				//Главное меню
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
									tapsound(SoundTapN);
									if (i == 4)
									{
										Check_Window = -1;
										quit = 1;
									}
									if (i == 3) Check_Window = 4;
									if (i == 2)
									{
										std::ifstream Records;
										Records.open("Records.txt");
										if (Records.is_open())
										{
											if ((time1 == 0) && (time2 == 0) && (time3 == 0)) Check_Window = 7;
											else Check_Window = 3;
										}
										else Check_Window = 7;
										Records.close();
									}
									if (i == 1)
									{
										TimeStartGame = (SDL_GetTicks() / 1000);
										std::ifstream in;
										in.open("saves.txt");
										if (in.is_open())
										{
											if (in.peek() == EOF) Check_Window = 7;
											else
											{
												for (int q = 0; q < 8; q++)
												{
													for (int j = 0; j < 8; j++)
													{
														in >> level1[j][q];
													}
												}
												in >> points;
												in >> numberlevel;
												in >> level1time;
												in >> RecordTime2;
												in >> RecordVisionTime;
												pointfn = Lvl[numberlevel].PointsLevel;
												_itoa_s(pointfn, PointsForNext, 10);
												PointsFNTexture = get_text_texture(renderer, PointsForNext, my_font, PointsFNColor);
												_itoa_s(points, Points, 10);
												PointsTexture = get_text_texture(renderer, Points, my_font, PointsColor);
												Check_Window = 1;
											}
										}
										else
										{
											Check_Window = 7;
										}
										in.close();
									}
									if (i == 0)
									{
										numberlevel = 0;
										TimeStartGame = (SDL_GetTicks() / 1000);
										level1time = Lvl[0].TimeLevel;
										RecordVisionTime = 0;
										Check_Window = 1;
										points = 0;
										_itoa_s(points, Points, 10);
										PointsTexture = get_text_texture(renderer, Points, my_font, PointsColor);
										pointfn = Lvl[0].PointsLevel;
										_itoa_s(pointfn, PointsForNext, 10);
										PointsFNTexture = get_text_texture(renderer, PointsForNext, my_font, PointsFNColor);
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
				//Игра
				while (Check_Window == 1)
				{
					SDL_RenderCopy(renderer, TexturFon, NULL, &FonRect);
					RecordTime = (SDL_GetTicks() / 1000) - TimeStartGame;
					if (RecordTime2 != RecordTime)
					{
						RecordTime2 = RecordTime;
						RecordVisionTime += 1;
					}
					TimeStartProgram = (SDL_GetTicks() / 1000) - TimeStartGame;
					if (TimeStartProgram2 != TimeStartProgram)
					{
						TimeStartProgram2 = TimeStartProgram;
						level1time -= 1;
						_itoa_s(level1time, leveltime, 10);
						SDL_DestroyTexture(TimerTexture);
						TimerTexture = get_text_texture(renderer, leveltime, my_font, TimerColor);
					}
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
									tapsound(SoundTapN);
									points = 0;
									numberlevel = 0;
									TimeStartProgram2 = 0;
									RecordVisionTime = 0;
									RecordTime2 = 0;
									SDL_DestroyTexture(PointsTexture);
									SDL_DestroyTexture(TimerTexture);
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
												tapsound(SoundTapN);
												check2try += 1;;
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
							points1 = CheckCombinationLeft(level1, CheckCardOpeni2, CheckCardOpenj2) + CheckCombinationRight(level1, CheckCardOpeni2, CheckCardOpenj2) + 1;
							int BrokenRyadBegin = CheckCardOpeni2 - CheckCombinationLeft(level1, CheckCardOpeni2, CheckCardOpenj2);
							int BrokenRyadEnd = CheckCardOpeni2 + CheckCombinationRight(level1, CheckCardOpeni2, CheckCardOpenj2);
							BrokeLeftRight(level1, CheckCardOpenj2, BrokenRyadBegin, BrokenRyadEnd);
						}
						if (CheckCombinationUp(level1, CheckCardOpeni2, CheckCardOpenj2) + CheckCombinationDown(level1, CheckCardOpeni2, CheckCardOpenj2) < 2) checkcomb1 += 1;
						else
						{
							points2 = CheckCombinationUp(level1, CheckCardOpeni2, CheckCardOpenj2) + CheckCombinationDown(level1, CheckCardOpeni2, CheckCardOpenj2) + 1;
							int BrokenStolbBegin = CheckCardOpenj2 - CheckCombinationUp(level1, CheckCardOpeni2, CheckCardOpenj2);
							int BrokenStolbEnd = CheckCardOpenj2 + CheckCombinationDown(level1, CheckCardOpeni2, CheckCardOpenj2);
							BrokeUpDown(level1, CheckCardOpeni2, BrokenStolbBegin, BrokenStolbEnd);
						}


						//Вторая нажатая фигура
						if (CheckCombinationLeft(level1, CheckCardOpeni, CheckCardOpenj) + CheckCombinationRight(level1, CheckCardOpeni, CheckCardOpenj) < 2)checkcomb2 += 1;
						else
						{
							points3 = CheckCombinationLeft(level1, CheckCardOpeni, CheckCardOpenj) + CheckCombinationRight(level1, CheckCardOpeni, CheckCardOpenj) + 1;
							int BrokenRyadBegin = CheckCardOpeni - CheckCombinationLeft(level1, CheckCardOpeni, CheckCardOpenj);
							int BrokenRyadEnd = CheckCardOpeni + CheckCombinationRight(level1, CheckCardOpeni, CheckCardOpenj);
							BrokeLeftRight(level1, CheckCardOpenj, BrokenRyadBegin, BrokenRyadEnd);
						}
						if (CheckCombinationUp(level1, CheckCardOpeni, CheckCardOpenj) + CheckCombinationDown(level1, CheckCardOpeni, CheckCardOpenj) < 2) checkcomb2 += 1;
						else
						{
							points4 = CheckCombinationUp(level1, CheckCardOpeni, CheckCardOpenj) + CheckCombinationDown(level1, CheckCardOpeni, CheckCardOpenj) + 1;
							int BrokenStolbBegin = CheckCardOpenj - CheckCombinationUp(level1, CheckCardOpeni, CheckCardOpenj);
							int BrokenStolbEnd = CheckCardOpenj + CheckCombinationDown(level1, CheckCardOpeni, CheckCardOpenj);
							BrokeUpDown(level1, CheckCardOpeni, BrokenStolbBegin, BrokenStolbEnd);
						}
						points += 100 * points1 + 100 * points2 + 100 * points3 + 100 * points4;
						if ((checkcomb1 > 1) && (checkcomb2 > 1))
						{
							wrongtapsound(SoundTapW);
							std::swap(level1[CheckCardOpeni][CheckCardOpenj], level1[CheckCardOpeni2][CheckCardOpenj2]);
						}
						else tapsound(SoundTapN);

						_itoa_s(points, Points, 10);
						SDL_DestroyTexture(PointsTexture);
						PointsTexture = get_text_texture(renderer, Points, my_font, PointsColor);
						check2try = 0;
					}
					if ((points >= Lvl[numberlevel].PointsLevel))
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
						if (numberlevel <= 4)
						{
							pointfn = Lvl[numberlevel].PointsLevel;
							_itoa_s(pointfn, PointsForNext, 10);
							SDL_DestroyTexture(PointsFNTexture);
							PointsFNTexture = get_text_texture(renderer, PointsForNext, my_font, PointsFNColor);
						}

					}
					if (level1time == 0)
					{
						losesound(SoundLose);
						pointfn = Lvl[0].PointsLevel;
						points = 0;
						numberlevel = 0;
						TimeStartProgram2 = 0;
						RecordVisionTime = 0;
						RecordTime2 = 0;
						SDL_DestroyTexture(PointsTexture);
						//SDL_DestroyTexture(TimerTexture);
						Check_Window = 0;
					}
					if (numberlevel > 4)
					{
						pointfn = Lvl[0].PointsLevel;
						numberlevel = 0;
						points = 0;
						SDL_DestroyTexture(PointsTexture);
						SDL_DestroyTexture(PointsFNTexture);
						char temptimewin[10];
						_itoa_s(RecordVisionTime, temptimewin, 10);
						strcat_s(temptimewin, "s");
						ShowTimeAftGame = get_text_texture(renderer, temptimewin, my_font, WinMessageColor);
						UpdateRec(time1, time2, time3, RecordVisionTime);
						std::ofstream Record;
						Record.open("Records.txt");
						Record << time1 << " " << time2 << " " << time3;
						Record.close();
						char BestTime[55] = "Your best times: ";
						_itoa_s(time1, time1txt, 10);
						_itoa_s(time2, time2txt, 10);
						_itoa_s(time3, time3txt, 10);
						strcat_s(BestTime, time1txt); strcat_s(BestTime, "s"); strcat_s(BestTime, " ");
						strcat_s(BestTime, time2txt); strcat_s(BestTime, "s"); strcat_s(BestTime, " ");
						strcat_s(BestTime, time3txt); strcat_s(BestTime, "s");
						ResTexture = get_text_texture(renderer, BestTime, my_font, BestTimeColor);

						Check_Window = 5;
					}
					SDL_RenderCopy(renderer, TexturBackNumb, NULL, &PointsRect);
					SDL_RenderCopy(renderer, TexturBackNumb, NULL, &TimerRect);
					SDL_RenderCopy(renderer, TexturBackNumb, NULL, &PointsFNRect);
					SDL_RenderCopy(renderer, TexturGrid, NULL, &GridRect);
					SDL_RenderCopy(renderer, SettingsBut[3], NULL, &BackButtonRect);
					DrawCells(renderer, gem, GemGame, level1);
					draw_text(renderer, PointsTexture, PointsRect);
					draw_text(renderer, PointsFNTexture, PointsFNRect);
					draw_text(renderer, TimerTexture, TimerRect);
					SDL_RenderPresent(renderer);
				}
				//Рекорды
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
								tapsound(SoundTapN);

								Check_Window = 0;
							}
						}
					}
					draw_text(renderer, ResTexture, BestTimeRect);
					SDL_RenderCopy(renderer, SettingsBut[3], NULL, &BackButtonRect);
					SDL_RenderPresent(renderer);
				}
				//Меню настроек
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
									tapsound(SoundTapN);
									if (i == 0)
									{
										if (SoundTap == 50) tempsoundtap = 0;
										if (SoundTap == 0) tempsoundtap = 50;
										SoundTap = tempsoundtap;
										std::ofstream soundfile;
										soundfile.open("settings.txt");
										soundfile << SoundTap << " " << Music << " " << FullScreenChecker;
										soundfile.close();
										Mix_Volume(-1, SoundTap);
									}
									if (i == 1)
									{
										if (Music == 100) tempmusic = 0;
										if (Music == 0) tempmusic = 100;
										Music = tempmusic;
										std::ofstream soundfile;
										soundfile.open("settings.txt");
										soundfile << SoundTap << " " << Music << " " << FullScreenChecker;
										soundfile.close();
										Mix_VolumeMusic(Music);
									}
									if (i == 2)
									{
										if (FullScreenChecker == 0)
										{
											SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
											FonRect = { 0, 0, fW, fH };
											FullScreenChecker = 1;
											UpdateRects(FullScreenChecker, fW, fH, FonRect, MenuRect, MenuButtons, SetButtons, BackButtonRect, PauseButtonsRect, GridRect, BestTimeRect, FileErrorRect, WinMessageRect, ShowTimeAGRect, PointsRect, PointsFNRect, TimerRect, gem);
											std::ofstream fsfile;
											fsfile.open("settings.txt");
											fsfile << SoundTap << " " << Music << " " << FullScreenChecker;
											fsfile.close();
											break;
										}
										if (FullScreenChecker == 1)
										{
											SDL_SetWindowFullscreen(window, 0);
											SDL_SetWindowSize(window, W, H);
											FonRect = { 0, 0, W, H };
											FullScreenChecker = 0;
											UpdateRects(FullScreenChecker, W, H, FonRect, MenuRect, MenuButtons, SetButtons, BackButtonRect, PauseButtonsRect, GridRect, BestTimeRect, FileErrorRect, WinMessageRect, ShowTimeAGRect, PointsRect, PointsFNRect, TimerRect, gem);
											std::ofstream fsfile;
											fsfile.open("settings.txt");
											fsfile << SoundTap << " " << Music << " " << FullScreenChecker;
											fsfile.close();
											break;
										}
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
				//Сообщение о победе
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
								tapsound(SoundTapN);
								Check_Window = 0;
							}
						}
					}
					draw_text(renderer, WinMessageTexture, WinMessageRect);
					draw_text(renderer, ShowTimeAftGame, ShowTimeAGRect);
					SDL_RenderCopy(renderer, SettingsBut[3], NULL, &BackButtonRect);
					SDL_RenderPresent(renderer);
				}
				//Меню паузы во время игры
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
								tapsound(SoundTapN);
								Check_Window = 0;
							}
							for (int i = 0; i < 2; i++)
							{
								if (CheckMenuHit(PauseButtonsRect[i], event.button.x, event.button.y))
								{
									tapsound(SoundTapN);
									if (i == 1) Check_Window = 1;
									if (i == 0)
									{
										std::ofstream out;
										out.open("saves.txt");
										for (int q = 0; q < 8; q++)
										{
											for (int j = 0; j < 8; j++)
											{
												out << level1[j][q];
												out << " ";
											}
										}
										out << points; out << " ";
										out << numberlevel; out << " ";
										out << level1time; out << " ";
										out << RecordTime2; out << " ";
										out << RecordVisionTime; out << " ";
										out.close();
									}
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
				//Меню отсутствия файла(-ов)
				while (Check_Window == 7)
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
								tapsound(SoundTapN);
								Check_Window = 0;
							}
						}
					}
					draw_text(renderer, FileErrorTexture, BestTimeRect);
					SDL_RenderCopy(renderer, SettingsBut[3], NULL, &BackButtonRect);
					SDL_RenderPresent(renderer);
				}
			}
			for (int i = 0; i < 4; i++) SDL_DestroyTexture(SettingsBut[i]);
			for (int i = 0; i < 6; i++) SDL_DestroyTexture(TexturImage[i]);
			for (int i = 0; i < 2; i++) SDL_DestroyTexture(PauseBut[i]);
			SDL_DestroyTexture(FileErrorTexture);
			SDL_DestroyTexture(ShowTimeAftGame);
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