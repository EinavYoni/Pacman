#include "Ghost.h"
#include "Fruit.h"

void Ghost::move(string mode,int x, int y)
{
	int pathExist = 0;
	Coord nextMove;
	bool isRandomMove = false;
	bool silent = (mode.compare("silent") == 0);
	if (mode.compare("load") == 0 || silent)
	{
		moveLoad(x);
	}
	else {
		if (mood != ghostMood::DUMB)
		{
			if (mood == ghostMood::SUPER_SMART)
			{
				isRandomMove = superSmartMove(x, y);
			}

			if (mood == ghostMood::SMART)
			{
				//most time choose radom
				isRandomMove = SmartMove();
			}

			if (isRandomMove == false)
			{
				pathExist = BFSsearch(nextMove, x, y);

				if (pathExist == -1)
				{
					isRandomMove = true;
				}
				else
				{
					setLastMove(getPoint().getX(), getPoint().getY());
					setPoint(nextMove.getX(), nextMove.getY());
				}
			}

			else if (pathExist == -1)
			{
				//There is no path between Pacman and ghost
				isRandomMove = true;
			}
		}

		if (mood == ghostMood::DUMB || isRandomMove)
		{
			dumbMove();
		}
	}

		prindAndSetBoard(silent);

}

bool const Ghost::isValidMove(const ghostDirections& direction)
{
	int x = 0, y = 0, newX, newY;
	int width = Board::getWidth(), length = Board::getLength();
	Cell status;
	vector <Cell>& board = Board::getBoardGame();

	switch (direction)
	{
	case ghostDirections::G_RIGHT:
		y++;
		break;

	case ghostDirections::G_DOWN:
		x++;
		break;

	case ghostDirections::G_LEFT:
		y--;
		break;

	case ghostDirections::G_UP:
		x--;
		break;
	}

	newX = getPoint().getX() + x;
	newY = getPoint().getY() + y;

	if (validCoord(newX, newY))
	{
		status = board[newX * width + newY];

		if (status.getCellStatus() != CellStatus::WALL && status.getCellStatus() != CellStatus::TUBE)
		{
			if (board[newX * width + newY].getPlayerStatus() == PlayerInCell::FRUIT)
			{
				Fruit::setAppear(TIME_TO_DISAPPEAR);
			}

			return true;
		}
	}

	return false;
}

void const Ghost::updateAfterHitPacman(Ghost ghosts[], bool& silent)
{
	int status = (int)Board::getBoardGame()[(getPoint().getX()) * Board::getWidth() + getPoint().getY()].getCellStatus();

	Board::getBoardGame()[getPoint().getX() * Board::getWidth() + getPoint().getY()].setPlayerInCell((int)PlayerInCell::NONE);

	gotoxy(getPoint().getX(), getPoint().getY());

	Board::printWithColor(getColor(), status, (int)colors::Obj);

	if (!silent)
	{
		cout << StatusChars[status];
	}
	gotoxy(24, 79);
}

void Ghost::dumbMove()
{
	int random = rand() % 4;

	setLastMove(getPoint().getX(), getPoint().getY());

	while (!isValidMove((ghostDirections)random))
		random = rand() % 4;

	setMove(directionASCII[random]);
}

bool Ghost::superSmartMove(int& PacX, int& PacY)
{
	bool isitDumb = false;

	if (CollaborationGhost == Board::getLogSizeGhost())
	{
		CollaborationGhost = 0;
	}

	// the first ghost chases the pacman location

	else
	{
		findLocNearPacman(PacX, PacY, isitDumb);
	}

	CollaborationGhost++;

	return isitDumb;	
}

Coord Ghost::nextMovePoint(int loc)
{
	int width = Board::getWidth();
	vector <Cell>& board = Board::getBoardGame();
	Coord nextMove;
	int currX, currY, lastY = board[loc].getPoint().getY(), lastX = board[loc].getPoint().getX();
	int ghostpoint = getPoint().getX() * width + getPoint().getY();

	while (board[loc].getPoint().getX() != board[ghostpoint].getPoint().getX() || board[loc].getPoint().getY() != board[ghostpoint].getPoint().getY())
	{
		lastX = board[loc].getPoint().getX();
		lastY = board[loc].getPoint().getY();

		currX = board[loc].getPrevX();
		currY = board[loc].getPrevY();

		loc = currX * width + currY;
	}

	nextMove.setCoord(lastX, lastY);

	return (nextMove);
}

bool Ghost::validCoord(int x, int y)
{
	int width = Board::getWidth(), length = Board::getLength();
	return(y >= 0 && x >= 0 && y < width && x < length);
}

int Ghost::BFSsearch(Coord& nextMove, int& PacX, int& PacY)
{
	int width = Board::getWidth(), length = Board::getLength(), Y, X, loc = 0, lastY, lastX;
	const int boardSize = Board::getBoardGame().size();
	int GhostY = getPoint().getY(), GhostX = getPoint().getX();
	int directionsX[4] = { -1,1,0,0 };
	int directionsY[4] = { 0,0,1,-1 };
	queue<int> queueX, queueY;
	int tempY, tempX;
	int moveCount = 0, stepsLeftInLayer = 1, stepsInNextLayer = 0;
	bool reachedEnd = false;

	vector <Cell>& board = Board::getBoardGame();
	vector <bool> boardVisit;

	for (int i = 0; i < boardSize; i++) { boardVisit.push_back(false); }

	queueY.push(GhostY);
	queueX.push(GhostX);
	boardVisit[GhostX * width + GhostY] = true;

	Y = queueY.front();
	X = queueX.front();

	while (queueY.size() > 0)
	{
		lastX = X;
		lastY = Y;
		Y = queueY.front();
		X = queueX.front();
		queueY.pop();
		queueX.pop();

		if (X == PacX && Y == PacY)
		{
			loc = X * width + Y;
			reachedEnd = true;
			break;
		}

		else
		{
			//find neighbours 
			for (int i = 0; i < 4; i++)
			{
				tempX = X + directionsX[i];
				tempY = Y + directionsY[i];

				if (validCoord(tempX, tempY))
				{
					if (boardVisit[tempX * width + tempY] == false
						&& board[tempX * width + tempY].getCellStatus() != CellStatus::WALL)
					{
						queueY.push(tempY);
						queueX.push(tempX);
						boardVisit[tempX * width + tempY] = true;
						board[tempX * width + tempY].setPrev(X, Y);
						stepsInNextLayer++;
					}
				}
			}
			stepsLeftInLayer--;
		}

		if (stepsLeftInLayer == 0)
		{
			stepsLeftInLayer = stepsInNextLayer;
			stepsInNextLayer = 0;
			moveCount++;
		}
	}

	boardVisit.clear();

	if (reachedEnd)
	{
		Coord nextStep = nextMovePoint(loc);
		nextMove.setCoord(nextStep.getX(), nextStep.getY());
		return moveCount;
	}

	//path not found
	else return -1;
}

void Ghost::findLocNearPacman(int& x, int& y, bool& isitDumb)
{
	vector <Cell> board = Board::getBoardGame();
	int widht = Board::getWidth();
	int newX, newY, pacLocX = x, pacLocY = y, ghostX = getPoint().getX(), ghostY = getPoint().getY();
	CellStatus status;
	int directionsX[4] = { -1,1,0,0 };
	int directionsY[4] = { 0,0,1,-1 };
	int random = rand() % 3;
	int radiosX = pacLocX - ghostX;
	int radiosY = pacLocX - ghostX;

	for (int i = 0; i < 4; i++)
	{
		newX = x + directionsX[i];
		newY = y + directionsY[i];

		if (validCoord(newX, newY))
		{
			status = board[newX * widht + newY].getCellStatus();

			if (status != CellStatus::WALL && status != CellStatus::TUBE)
			{

				if (CollaborationGhost == 1)
				{
					// sometimes step dumb
					if (random == 2)
					{
						isitDumb = true;
						break;
					}
				}

				else if (CollaborationGhost == 2)
				{
					// sometimes step dumb and sometimes if pacman is near go smart
					if (radiosX < 5 && radiosX > -5 && radiosY < widht && radiosY > -widht)
					{
						isitDumb = false;
					}
					else
					{
						isitDumb = true;
					}
					break;
				}

				else if (CollaborationGhost == 3)
				{
					// if pacman is near go smart
					if (radiosX < 5 && radiosX > -5 && radiosY < widht && radiosY > -widht)
					{
						isitDumb = false;
					}
					else
					{
						// increase the search distance
						x = newX + 20;
						y = newY + 20;
					}
				}
			}
		}
	}
}

bool Ghost::SmartMove()
{
	bool notsmartMove =false ;

	if (CollaborationGhost == 3)
	{
		notsmartMove= true;
	}

	CollaborationGhost++;

	if(CollaborationGhost == 7 )
	{
		CollaborationGhost = 0;
	}

	return(notsmartMove);
}

void Ghost::stay()
{
	setLastMove(getPoint().getX(), getPoint().getY());
}

void Ghost::moveLoad(int step)
{
	setLastMove(getPoint().getX(), getPoint().getY());
	setMove(step);
}

