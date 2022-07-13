#include "Fruit.h"

 int Fruit::appear = 0;

void Fruit::set()
{
	vector<Cell>& board = Board::getBoardGame();
	int width = Board::getWidth(), Length = Board::getLength();
	int i = rand() % Length;
	int j = rand() % width;
	int fruitVal = rand() % 5 + 5;
	setPlayerChar(fruitVal + '0');

	while (!validCell(i, j, width))
	{
		j = rand() % width;
		i = rand() % Length;
	}

	setPoint(i, j);
	board[i * width + j].setPlayerInCell((int)PlayerInCell::FRUIT);
}

void Fruit::set(int x, int y)
{
	vector<Cell>& board = Board::getBoardGame();
	int width = Board::getWidth();
	setPoint(x, y);
	board[x * width + y].setPlayerInCell((int)PlayerInCell::FRUIT);
	appear++;
}

bool Fruit::validCell(int i, int j, int width)
{
	vector<Cell>& board = Board::getBoardGame();
	int loc = i * width + j;

	return (board[loc].getPlayerStatus() == PlayerInCell::NONE &&
		    board[loc].getCellStatus() != CellStatus::WALL &&
		    board[loc].getCellStatus() != CellStatus::LEGEND &&
		    board[loc].getCellStatus() != CellStatus::TUBE);
}	

 void Fruit::move(string mode,int userChoice, int y)
{
	 if (timeToDisappear(mode))
		 return;

	 appear++;
	 Ghost::move(mode, userChoice);
}


 bool Fruit::timeToDisappear(string mode)
 {
	 if (appear < 0)
	 {
		 appear++;
		 return true;
	 }

	 if (appear == TIME_TO_DISAPPEAR)
	 {
		 vector <Cell>& board = Board::getBoardGame();
		 int x = getPoint().getX(), y = getPoint().getY(), width = Board::getWidth();

		 // erase fruit from board
		 board[x * width + y].setPlayerInCell((int)PlayerInCell::NONE);

		 if (mode.compare("silent") != 0)
		 {
			 Pacman::gotoxy(x, y);
			 Board::printWithColor(getColor(), (int)board[x * width + y].getCellStatus(), static_cast<int>(colors::Obj));
			 cout << StatusChars[(int)board[x * width + y].getCellStatus()];
		 }

		 appear = COUNTDOWM_TO_APPEAR;
		 return true;
	 }

	 if (appear == 0)
	 {
		 if (mode.compare("load") != 0 && mode.compare("silent") !=0)
		 {
			 set();
		 }
		 else
		 {
			 return true;
		 }
	 }

	 return false;
 }

 bool Fruit::isFruitHitPacman()
 {
	 vector <Cell>& board = Board::getBoardGame();
	 int x = getPoint().getX(), y = getPoint().getY(), width = Board::getWidth();

	 if (board[x * width + y].getPlayerStatus() == PlayerInCell::PACMAN)
	 {
		 return true;
	 }

	 return false;
 }

 bool Fruit::isFruitHitGhost()
 {
	 vector <Cell>& board = Board::getBoardGame();
	 int x = getPoint().getX(), y = getPoint().getY(), width = Board::getWidth();

	 if (board[x * width + y].getPlayerStatus() == PlayerInCell::GHOST)
	 {
		 return true;
	 }

	 return false;
 }