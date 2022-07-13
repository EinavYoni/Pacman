#pragma once

#include "Ghost.h"
#include "Board.h"
#include "Pacman.h"

class Fruit : public Ghost
{ 
	static int appear;

public:
	Fruit() { appear = 0; }
	static const int getAppear() { return appear; }
	void set();
	bool validCell(int i, int j, int width);
	static void setAppear(int appear_) { appear = appear_; }
	virtual void move(string mode,int userChoice = 0, int y = 0) override;
	void set(int x, int y);
	bool timeToDisappear(string mode);
	bool isFruitHitPacman();
	bool isFruitHitGhost();
};