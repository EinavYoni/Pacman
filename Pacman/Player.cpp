#include "Player.h"

void Player::setMove(const int& direction)
{
	Coord tempMove;
	int width = Board::getWidth();
	vector <Cell>& board = Board::getBoardGame();
	tempMove = lastMove;
	int x = point.getX(), y = point.getY();

	switch (direction)
	{
	case (int)Directions_upper::RIGHT:
	case (int)Directions_lower::RIGHT_L:
		tempMove.setCoord(x, y + 1);
		break;

	case (int)Directions_upper::DOWN:
	case (int)Directions_lower::DOWN_L:
		tempMove.setCoord(x + 1, y);
		break;

	case (int)Directions_upper::LEFT:
	case (int)Directions_lower::LEFT_L:
		tempMove.setCoord(x, y - 1);
		break;

	case (int)Directions_upper::UP:
	case (int)Directions_lower::UP_L:
		tempMove.setCoord(x - 1, y);
		break;
	}

	x = tempMove.getX(), y = tempMove.getY();

	if (x >= 0 && y < width && y >= 0 && x < Board::getLength())
	{
		if (board[x * width + y].getCellStatus() != CellStatus::WALL)
		{
			point.setCoord(x, y);
		}
	}
}

void Player::gotoxy(int x, int y)
{
	static HANDLE hConsoleOutput = NULL;
	if (NULL == hConsoleOutput)
		hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);

	COORD dwCursorPosition = { y, x };
	cout.flush();
	SetConsoleCursorPosition(hConsoleOutput, dwCursorPosition);
}

void Player::tube(bool& silentMode)
{
	int i = point.getX(), j = point.getY();
	int width = Board::getWidth(), length = Board::getLength();
	int loc = i * width + j;

	if (Board::getBoardGame()[loc].getCellStatus() == CellStatus::TUBE)
	{
		setLastMove(i, j);
		setPoint(Board::getBoardGame()[loc].gettubeExitX(), Board::getBoardGame()[loc].gettubeExitY());
		prindAndSetBoard(silentMode);
	}
}

void Player::prindAndSetBoard(bool mode)
{
	vector <Cell>& board = Board::getBoardGame();
	int width = Board::getWidth(), LastX = lastMove.getX(), LastY = lastMove.getY();
	int x = getPoint().getX(), y = getPoint().getY();

	if (playerChar == PACMAN_CHAR)
	{
		if (!mode)
		{
			gotoxy(LastX, LastY);
			Board::printWithColor(getColor(), (int)colors::EmptyColor, (int)colors::Obj);
			cout << EMPTY_CHAR;
		}

		if (board[LastX * width + LastY].getCellStatus() != CellStatus::TUBE)
		{
			board[LastX * width + LastY].setCellStatus((int)CellStatus::EMPTY); // make the status empty
		}
		board[LastX * width + LastY].setPlayerInCell((int)PlayerInCell::NONE);
		board[x * width + y].setPlayerInCell((int)PlayerInCell::PACMAN); // change player
	}

	else // print the last char
	{
		board[LastX * width + LastY].setPlayerInCell((int)PlayerInCell::NONE); // make the status empty
		if (!mode)
		{
				gotoxy(LastX, LastY);
				Board::printWithColor(getColor(), (int)board[LastX * width + LastY].getCellStatus(), (int)colors::Obj);
				cout << StatusChars[(int)board[LastX * width + LastY].getCellStatus()];
			
		}

		if (playerChar == GHOST_CHAR)
		{
			board[x * width + y].setPlayerInCell((int)PlayerInCell::GHOST); // change player
		}
		else
		{
			board[x * width + y].setPlayerInCell((int)PlayerInCell::FRUIT); // change player
		}

	}

	if (playerChar == PACMAN_CHAR)
		Board::printWithColor(getColor(), (int)PlayerInCell::PACMAN, (int)colors::NotObj);
	else if(playerChar == GHOST_CHAR)
		Board::printWithColor(getColor(), (int)PlayerInCell::GHOST, (int)colors::NotObj);
	else
	{
		Board::printWithColor(getColor(), (int)PlayerInCell::FRUIT, (int)colors::NotObj);
	}

	if (!mode)
	{
		gotoxy(x, y);
		cout << playerChar;
		gotoxy(24, 79);
	}

}