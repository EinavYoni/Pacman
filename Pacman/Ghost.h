#pragma once

#include "Cell.h"
#include "Pacman.h"
#include <time.h>
#include <queue>
#include <cstdlib>

const int MAX_GHOSTS = 4;

enum class ghostDirections { G_UP, G_DOWN, G_RIGHT, G_LEFT };
const int directionASCII[4] = { (int)Directions_upper::UP, (int)Directions_upper::DOWN, (int)Directions_upper::RIGHT, (int)Directions_upper::LEFT };
enum class ghostMood { DUMB, SMART, SUPER_SMART };

static int CollaborationGhost = 0;

class Ghost : public Player
{
	ghostMood mood;

public:
	Ghost() { mood = ghostMood::DUMB, setPlayerChar(GHOST_CHAR); };
	void setMode(ghostMood mood_) { mood = mood_; }
	virtual void move(string mode,int userChoice = 0, int y = 0) override;
	bool const isValidMove(const ghostDirections& direction);
	void const updateAfterHitPacman(Ghost ghosts[], bool& silent);
	void dumbMove();
	bool SmartMove();
	bool superSmartMove(int& PacX, int& PacY);
	int BFSsearch(Coord& nextMove, int& PacX, int& PacY);
	Coord nextMovePoint(int loc);
	bool validCoord(int x, int y);
	void findLocNearPacman(int& x, int&y, bool& isitSmart);
	void stay();
	void moveLoad(int step);
};