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
bool CheckSecondTap(int i, int j, int i1, int j1)
{
	if (i == i1 + 1 && j == j1) return 1;
	if (i == i1 - 1 && j == j1) return 1;
	if (j == j1 + 1 && i == i1) return 1;
	if (j == j1 - 1 && i == i1) return 1;
	else return 0;
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
void BrokeLeftRight(int level[][8], int stolb, int begin, int end)
{
	int temp[8][8];
	for (int i = 0; i < Set; i++)
	{
		for (int j = 0; j < Set; j++)
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
	for (int i = 0; i < Set; i++)
	{
		for (int j = 0; j < Set; j++)
		{
			level[j][i] = temp[i][j];
		}
	}
}
void BrokeUpDown(int level[][8], int stolb, int begin, int end)
{
	int temp[Set][Set];
	int freegems = begin;
	for (int i = 0; i < Set; i++)
	{
		for (int j = 0; j < Set; j++)
		{
			temp[i][j] = level[j][i];
		}
	}
	for (int i = 0; i <= end; i++)
	{
		if (i < freegems) temp[end - i][stolb] = temp[begin - i - 1][stolb];
		else temp[end - i][stolb] = rand() % (5 - 1 + 1) + 1;
	}
	for (int i = 0; i < Set; i++)
	{
		for (int j = 0; j < Set; j++)
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
	//ÏĞÎÂÅĞÊÀ 3 ÎÄÈÍÀÊÎÂÛÕ
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
	//ÏĞÎÂÅĞÊÀ 3 ÎÄÈÍÀÊÎÂÛÕ
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
void RandomLevelGen(int level[][8])
{
	srand(time(NULL));
	for (int i = 0; i < Set; i++)
	{
		for (int j = 0; j < Set; j++)
		{
			level[i][j] = rand() % 6;
		}
	}
}
