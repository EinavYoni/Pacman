#include "Pacman.h"

bool const Pacman::isKeyValid(const char& key)
{
	return (key == (int)Directions_upper::RIGHT || key == (int)Directions_upper::LEFT ||
		key == (int)Directions_upper::UP || key == (int)Directions_upper::DOWN ||
		key == (int)Directions_lower::RIGHT_L || key == (int)Directions_lower::LEFT_L ||
		key == (int)Directions_lower::UP_L || key == (int)Directions_lower::DOWN_L ||
		key == (int)Directions_upper::STAY || key == (int)Directions_lower::STAY_L);
}

bool const Pacman::isHitTheWall()
{
	return (Board::getBoardGame()[getPoint().getX() * Board::getWidth() + getPoint().getY()].getCellStatus() == CellStatus::WALL);
}

 bool const Pacman::ateFood()
{
	 return(Board::getBoardGame()[getPoint().getX() * Board::getWidth() + getPoint().getY()].getCellStatus() == CellStatus::FOOD);
}

void const Pacman::printLifeAndScore(bool silent)
{
	if (!silent)
	{
		Board::printWithColor(getColor(), (int)CellStatus::FOOD, (int)colors::Obj);
		gotoxy(Board::getLegend().getX(), Board::getLegend().getY());
		cout << "LIVES:" << getLifes() << " SCORE:" << getScore() + getFruitBonus();
	}
}

void Pacman::updateScore()
{
	setScore(++score);
}

void Pacman::updateLifes(bool& silent)
{
	setLifes(--lifes);
	
	if (!silent)
	{
		gotoxy(Board::getLegend().getX(), Board::getLegend().getY() + 6);
		Board::printWithColor(getColor(), (int)CellStatus::FOOD, (int)colors::Obj);
		cout << lifes;
		gotoxy(24, 79);
	}

}

bool const Pacman::eatAll()
{
	return (score == Board::getBreadcrumbs());
}

void const Pacman::stay(bool& silent)
{
	if(silent)
	{
		return;
	}
	gotoxy(getPoint().getX(), getPoint().getY());
	Board::printWithColor(getColor(), (int)PlayerInCell::PACMAN, (int)colors::NotObj);
	cout << PACMAN_CHAR;
	gotoxy(24, 79);
}

void Pacman::updateAfterHitGhost(bool& silent)
{
	updateLifes(silent);

	Board::getBoardGame()[getPoint().getX() * Board::getWidth() + getPoint().getY()].setPlayerInCell((int)PlayerInCell::NONE);

	setPoint(getStartPosX(), getStartPosY());
	Board::getBoardGame()[getStartPosX() * Board::getWidth() + getStartPosY()].setPlayerInCell((int)PlayerInCell::PACMAN);

	gotoxy(getStartPosX(), getStartPosY());
	Board::printWithColor(getColor(), (int)PlayerInCell::PACMAN, (int)colors::NotObj);
	if (!silent)
	{
		cout << PACMAN_CHAR;

	}
}

void Pacman::move(string mode,int userChoice, int y)
{
	setLastMove(getPoint().getX(), getPoint().getY());
	setMove(userChoice);
}


