#pragma once

#include "Cell.h"
#include <vector>
#include <windows.h>
#include <iostream>
#include <stdexcept>
#include <stdlib.h>
#include <string>
#include <cstdio>
#include <fstream>
#include <filesystem>
#include <conio.h>

using namespace std;

int const MAX_CHARS_IN_ROW = 80;
int const MAX_ROWS = 25;

enum class colors { WallColor = 24, FoodColor = 8, EmptyColor = 15,  pacmanColor = 14,
	                GhostColor1 = 4, GhostColor2 = 2, GhostColor3 = 13, GhostColor4 = 6,
	                Obj = 0, NotObj = 1, FruitColor = 29};

static bool GhostaColor = false;

const char FOOD_CHAR = '-';
const char WALL_CHAR = '#';
const char EMPTY_CHAR = ' ';
const char PACMAN_CHAR = '@';
const char GHOST_CHAR = '$';
const char LEGEND_CHAR = '&';
const char NOFOOD_CHAR = '%';

static int ghostColorINDEX = 0;

class Board 
{
private:
	static int Length;
	static int Width;
	static vector<Cell> BoardGame;
	static Coord Legend;
	static int breadCrumbs;
	static int LogSizeGhost;
	static int x;
	static string currScreen;

public:
	static  string& getCurrScreen() { return currScreen; }
	static vector<Cell>& getBoardGame() { return BoardGame; }
	static int getWidth() { return Width;}
	static int getLength() { return Length; }
	void const print(const bool color, bool silentMode = false);
	bool convertBoard(char& menuChoice);
    static void printWithColor(int color, int statColor, int cellORPlayer);
	bool const getFileFromUser(ifstream& file);
	bool loadFileToVector(istream& file);
	void setLegend();
	void const setTube();
	static Coord getLegend() { return Legend; }
	static const int getBreadcrumbs() { return breadCrumbs; }
	static void setBreadcrumbs(int breadCrumbs_) { breadCrumbs = breadCrumbs_; }
	static const int getLogSizeGhost() { return LogSizeGhost; }
	static void setLogSizeGhost(int size) { LogSizeGhost = size; }
	vector<string> readAllFiles();
	static void setX(int x_) { x = x_; }
	static int getX() { return x; }
	void invalidCharError();
	void noLegendError();
	void noPacmanError();
	void over25rowsError();
	void moreThanOneLegendError();
	void noPlaceForLegendError();
	void overFourGhostsError();
	void overOnePacmanError();
	void over80charsError();
	void emptyFileError();
	void noFirstLineError();
};