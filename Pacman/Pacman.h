#pragma once

#include "Player.h"
#include "Ghost.h"

class Pacman :public Player
{
	int lifes = 3;
	int score = 0;
	int fruitBonus = 0;

public:
	Pacman() {
		lifes = 3; score = 0; setPlayerChar(PACMAN_CHAR);
	}
	void setLifes(int newLifes) { lifes = newLifes; };
	int const getLifes() { return lifes; };
	bool const isHitTheWall();
	bool const isKeyValid(const char& key);
	void const printLifeAndScore(bool silent =false);
	void setScore(int newScore) { score = newScore; };
	int const getScore() { return score; };
	void updateScore();
	void updateLifes(bool& silent);
	void const stay(bool& silent);
	bool const eatAll();
	void updateAfterHitGhost(bool& silent);
	virtual void move(string mode,int userChoice, int y = 0)final;
	int const getFruitBonus() { return fruitBonus; }
	void setFruitBonus(int newFruitBonus) { fruitBonus = newFruitBonus; }
	 bool const ateFood();
};