#pragma once

#include <windows.h>
#include <conio.h>
#include "Coord.h"
#include "Board.h"

/* this class define a player in the game */

const int SPEED = 100;

enum class Directions_upper { UP = 87, DOWN = 88, LEFT = 65, RIGHT = 68, STAY = 83 };
enum class Directions_lower { UP_L = 119, DOWN_L = 120, LEFT_L = 97, RIGHT_L = 100, STAY_L = 115 };

const int ESC = 27;
const int TIME_TO_DISAPPEAR = 30;
const int COUNTDOWM_TO_APPEAR = -20;

class Player 
{
	Coord point;
	char playerChar;
	Coord lastMove;
	int color = 0;
	int startPos[2];

public:
	void setPoint(int newX, int newY) { point.setCoord(newX, newY); };
	void setLastMove(int newX, int newY) { lastMove.setCoord(newX, newY); };
	void setMove(const int& direction);
    static void gotoxy(int x, int y);
	void setPlayerChar(char c) { playerChar = c; };
	int getPlayerCharint() { return playerChar - '0'; }
	char& getPlayerChar() { return playerChar; }
	Coord& const getPoint() { return point; };
	Coord& const getLastMove() { return lastMove; };
	void tube(bool& silentMode);
	void prindAndSetBoard(bool mode );
	void setColor(int c) { color = c; };
	int const getColor() { return color; };
	void const setStartPos(int x, int y) { startPos[0] = x, startPos[1] = y; }
	int getStartPosX() { return startPos[0];}
	int getStartPosY() { return startPos[1]; }
	virtual void move(string mode, int userChoice = 0, int y=0 ) = 0 ;
};