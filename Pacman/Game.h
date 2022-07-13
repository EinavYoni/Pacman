#pragma once

#include "Pacman.h"
#include "Ghost.h"
#include "Fruit.h"
#include "Menu.h"
#include <iostream>

#define rcastc reinterpret_cast<char*>
#define rcastcc reinterpret_cast<const char*> 

using namespace std;

const char QUIT_L = 'o';
const char QUIT_U = 'O';
const char needToSetFruitChar = 'N';

class Game 
{
	Board gameBoard;
	Pacman pacman;
    Ghost ghosts[MAX_GHOSTS];
	Fruit fruit;
	static string mode;
	fstream stepsFile;
	fstream resFile;
	int fileIndex = 0;

public:

	static string getMode() { return mode; }
	static void setMode(string newMode) { mode = newMode; }
	bool playGame(char& menuChoice);
	bool const compareCoords(Coord& c1, Coord& c2);
	void const printPause(const bool& pause);
	bool isPacmanHitGhost(char& userChoice);
	void setDefaultColor();
	void Reset(char& menuChoice);
	void const setPlayerLoc();
	bool setGameStart(char& menuChoice);
	void pacmanHitFruit();
	bool FruitHitPacman();
	void const printLose();
	void const printWin(int stepsCounter = 0);
	void loseInGame(char& menuChoice, int stepsCounter = 0);
	bool const validModeChoice(const char& choice);
	char& chooseGhostMode();
	void setGhostMood(const char& mood);
	void quitGame(char& menuChoice);
	void SaveGameToFile(fstream& saveFile, bool& save, bool ghostmove, bool first);
	char& findDirection( Coord& point,  Coord& lastPoint);
	bool ResultFile(fstream& r, int& stepsCounter, bool win);
	bool fileTest(fstream& file);
	string renameScreen();
	bool openFiles(fstream& resFile, fstream& saveFile);
	void playByLoad();
	Directions_upper convertDirection(char dir);
	string returnLoadFile(string end);
	void checkTest(int& stepsCounter);
};




