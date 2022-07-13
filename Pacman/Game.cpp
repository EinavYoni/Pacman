#include "Game.h"

string Game::mode = "simple";


bool Game::playGame(char& menuChoice)
{
	// '\n' means game not started
	char userChoice = '\n', lastValidDirection = '\n', temp;
	bool stayInPlace = true, pause = false, ghostsMove = true;
	bool lose = false;
	int stepsCounter = 0, x, y, fruitmove = 0;
	bool saveMode, loadMode, silentMode, first = true;
	Directions_upper step;
	char playerChar, direction;
	fstream saveFile;
	fstream resFile;

	saveMode = (mode.compare("save") == 0);
	loadMode = (mode.compare("load") == 0) || (mode.compare("silent") == 0);
	silentMode = (mode.compare("silent") == 0);
	
	if (!setGameStart(menuChoice))
	{
		return false;
	}

	int numOfGhost = Board::getLogSizeGhost();

	if(!loadMode)
	{ 
		fruit.set();
	}

	else
	{
		playByLoad();
		fileIndex++;
		stepsFile >> playerChar;
	}

	if (!openFiles(resFile, saveFile))
	{
		return false;
	}

	while (!lose)
	{
		pacman.tube(silentMode);
		ghostsMove = !ghostsMove;
		Sleep(SPEED);

		if (!loadMode)
		{
			if (_kbhit())
			{
				temp = _getch();

				if (temp == ESC)
				{
					pause = !pause;

					if (pause)
					{
						stayInPlace = true;

						// invalid key
						userChoice = '\n';
						printPause(pause);
					}

					else
					{
						userChoice = lastValidDirection;
						printPause(pause);
					}
				}

				else if (temp == QUIT_L || temp == QUIT_U)
				{
					quitGame(menuChoice);
					return false;
				}

				else if (pacman.isKeyValid(temp) && !pause)
				{
					if (temp != 'S' && temp != 's')
					{
						first = false;
					}
					
					stayInPlace = false;
					userChoice = temp;
					lastValidDirection = userChoice;
				}
			}
		}

		else
		{
			if (stepsFile.eof())
			{
				system("cls");
				stepsFile.close();
				Reset(menuChoice);
				if (silentMode)
				{
					checkTest(stepsCounter);
				}
				return true;
			}

			if (playerChar == PACMAN_CHAR)
			{
				stepsFile >> direction;
				step = convertDirection(direction);
				userChoice = (char)step;
				stepsFile >> playerChar;
			}

		    if (playerChar != GHOST_CHAR && playerChar != PACMAN_CHAR) // fruit char
			{
				stepsFile >> direction;
				if (direction == needToSetFruitChar)
				{
					fruit.setPlayerChar(playerChar);
					stepsFile >> x;
					stepsFile >> y;
					fruit.set(x, y);
					stepsFile >> direction;

				}

				fruitmove = (int)convertDirection(direction);
				stepsFile >> playerChar;
			}
		}
		if (!first || (silentMode))
		{
			stepsCounter++;
		}
	
		pacman.move(mode, userChoice);

		if (FruitHitPacman())
		{
			pacmanHitFruit();
		}

		if (isPacmanHitGhost(userChoice))
			lose = true;

		if (pacman.getLifes() == 0)
		{
			if (saveMode)
			{
				ResultFile(resFile, stepsCounter, false);
			}

			loseInGame(menuChoice, stepsCounter);;
			return false;
		}

		if (userChoice != '\n')
		{
			fruit.move(mode, fruitmove);

			if (fruit.isFruitHitGhost())
			{
				fruit.setAppear(TIME_TO_DISAPPEAR);
				fruit.timeToDisappear(mode);
			}

			if (FruitHitPacman())
			{
				pacmanHitFruit();
			}
			if (ghostsMove)
			{
				int movePP = pacman.getPoint().getX();
				for (int i = 0; i < numOfGhost; i++)
				{
					if (loadMode)
					{
						stepsFile >> direction;
						movePP = (int)convertDirection(direction);
					}
				    	ghosts[i].move(mode,movePP, pacman.getPoint().getY());
					if (loadMode)
					{
						stepsFile >> playerChar;
					}
				}
			}

			if (fruit.isFruitHitGhost())
			{
				fruit.setAppear(TIME_TO_DISAPPEAR);
				fruit.timeToDisappear(mode);
			}
		}

		else
		{
			fruit.stay();
			for (int i = 0; i < numOfGhost; i++)
			{
				ghosts[i].stay();
			}
		}

		if (isPacmanHitGhost(userChoice))
			lose = true;

		if (userChoice == (int)Directions_upper::STAY || userChoice == (int)Directions_lower::STAY_L)
			stayInPlace = true;

		if (pacman.isHitTheWall())
		{
			stayInPlace = true;
			pacman.setPoint(pacman.getLastMove().getX(), pacman.getLastMove().getY());
		}

		pacman.prindAndSetBoard(silentMode);

		if (pacman.ateFood())
		{
			pacman.updateScore();
			pacman.printLifeAndScore(silentMode);
		}

		pacman.prindAndSetBoard(silentMode);

		if (stayInPlace)
			pacman.stay(silentMode);

		if (pacman.eatAll())
		{
			if (saveMode)
			{
				ResultFile(resFile, stepsCounter, true);
			}

			printWin(stepsCounter);
			break;
		}

		if (pacman.getLifes() == 0)
		{
			if (saveMode)
			{
				ResultFile(resFile, stepsCounter, false);
			}

			loseInGame(menuChoice);
			return false;
		}

		SaveGameToFile(saveFile, saveMode, ghostsMove, first);
	}

	if (saveMode)
	{
		saveFile.close();
	}
	Reset(menuChoice);

	return false;
}

void Game::quitGame(char& menuChoice)
{
	pacman.setLifes(3);
	Board::setX(0);
	Reset(menuChoice);
	system("cls");
}

bool const Game::validModeChoice(const char& choice)
{
	return (choice == '1' || choice == '2' || choice == '3');
}

void Game::loseInGame(char& menuChoice, int stepsCounter)
{
	bool silent = (mode.compare("silent") == 0);
	
	pacman.printLifeAndScore(silent);

	Reset(menuChoice);
	printLose();
	if (mode.compare("silent") == 0)
	{
		checkTest(stepsCounter);
	}
	Board::setX(0);
}

bool const Game::compareCoords(Coord& c1, Coord& c2)
{
	return(c1.getX() == c2.getX() && c1.getY() == c2.getY());
}

void const Game::printPause(const bool& pause)
{
	bool silentMode = (mode.compare("silent") == 0);

	if (pause && !silentMode)
	{
		Pacman::gotoxy(Board::getLegend().getX(), Board::getLegend().getY());
		Board::printWithColor(pacman.getColor(), (int)PlayerInCell::PACMAN, (int)colors::Obj);
		cout << "   GAME PAUSED    ";
		Pacman::gotoxy(24, 79);
	}
	else
	{
		pacman.printLifeAndScore(silentMode);
	}
	
}

bool Game::isPacmanHitGhost(char& userChoice)
{
	bool initGhosts = false, silent;
	silent = (mode.compare("silent")==0);

	for (int i = 0; i < Board::getLogSizeGhost(); i++)
	{
		if (Game::compareCoords(pacman.getPoint(), ghosts[i].getPoint()))
		{
			Sleep(700);
			initGhosts = true;
			pacman.updateAfterHitGhost(silent);
			userChoice = (int)Directions_upper::STAY;
			if (pacman.getLifes() == 0)
				return true;
		}
	}

	if (initGhosts)
	{
		for (int i = 0; i < Board::getLogSizeGhost(); i++)
			ghosts[i].updateAfterHitPacman(ghosts, silent);
	}

	return false;
}

void Game::setDefaultColor()
{ 
	pacman.setColor(0);

	for (int i = 0; i < Board::getLogSizeGhost(); i++)
	{
		ghosts[i].setColor(0);
	}

	fruit.setColor(0);
};

void Game::Reset(char& menuChoice)
{
	setDefaultColor();
	pacman.setScore(0);
	pacman.setFruitBonus(0);
	Board::setBreadcrumbs(0);
	Board::getBoardGame().clear();
	Board::setLogSizeGhost(0);
	fruit.setAppear(0);

	if (pacman.getLifes() == 0)
	{
		pacman.setLifes(3);
	}

	else if (menuChoice == '3' || menuChoice == '4')
	{
		// if there are more boards to play
		if (Board::getX() != 0)
		{
			playGame(menuChoice);
		}

	}
}

void const Game::setPlayerLoc()
{
	int i = 0, j = 0;
	int locX, locY;
	for (Cell x : Board::getBoardGame())
	{
		i++;

		if (x.getPlayerStatus() == PlayerInCell::PACMAN)
		{
			locX = Board::getBoardGame()[i].getPoint().getX();
			locY = Board::getBoardGame()[i].getPoint().getY();
			pacman.setLastMove(locX, locY - 1);
			pacman.setStartPos(locX, locY - 1);
			pacman.setPoint(locX, locY - 1);
			x.setCellStatus((int)CellStatus::EMPTY);
		}

		if (x.getPlayerStatus() == PlayerInCell::GHOST)
		{
			locX = Board::getBoardGame()[i].getPoint().getX();
			locY = Board::getBoardGame()[i].getPoint().getY();
			ghosts[j].setStartPos(locX, locY - 1);
			ghosts[j].setPoint(locX, locY - 1);
			ghosts[j].setLastMove(locX, locY - 1);
			j++;
		}
	}
}

bool Game::setGameStart(char& menuChoice)
{
	system("cls");

	if (!gameBoard.convertBoard(menuChoice))
	{
		Reset(menuChoice);
		return false;
	}

	if (menuChoice == '2' || menuChoice == '4')
	{
		pacman.setColor((int)colors::pacmanColor);

		for (int i = 0; i < Board::getLogSizeGhost(); i++)
		{
			ghosts[i].setColor((int)colors::GhostColor1);
		}

		fruit.setColor((int)colors::FruitColor);
	}

	if (mode.compare("simple") == 0 || mode.compare("save") == 0)
	{
		char mood = chooseGhostMode();
		setGhostMood(mood);
	}

	if (mode.compare("silent") == 0)
	{
		gameBoard.print(pacman.getColor(), true);
	}

	else
	{
		gameBoard.print(pacman.getColor());
		pacman.printLifeAndScore();
	}

	setPlayerLoc();

	return true;
}

void  Game::pacmanHitFruit()
{
	pacman.setFruitBonus(pacman.getFruitBonus() + fruit.getPlayerCharint());

	if (mode.compare("silent") != 0)
	{
		Pacman::gotoxy(Board::getLegend().getX(), Board::getLegend().getY());
		Board::printWithColor(pacman.getColor(), (int)PlayerInCell::PACMAN, (int)colors::Obj);
		cout << "  FRUIT BONUS!    ";
		Pacman::gotoxy(24, 79);
		Sleep(4 * SPEED);

		pacman.printLifeAndScore();
	}
	fruit.setPoint(Board::getLegend().getX(), Board::getLegend().getY());
	Fruit::setAppear(COUNTDOWM_TO_APPEAR);
}

bool Game::FruitHitPacman()
{
	if (fruit.getAppear() > 0)
	{
		return(pacman.getPoint().getX() == fruit.getPoint().getX() && pacman.getPoint().getY() == fruit.getPoint().getY());
	}

	else
	{
		return false;
	}
}

void const Game::printLose()
{
	system("cls");
	cout << "**********************************************\n"
		<< "*                                            *\n"
		<< "*              YOU LOSE !!! : (              *\n"
		<< "*                                            *\n"
		<< "**********************************************\n\n";

	Menu::printMessage();
}

void const Game::printWin(int stepsCounter)
{
	system("cls");
	cout << "**********************************************\n"
		<< "*                                            *\n"
		<< "*              YOU WIN !!! : )               *\n"
		<< "*                                            *\n"
		<< "**********************************************\n\n";

	Menu::printMessage();

	if (mode.compare("silent") == 0)
	{
		checkTest(stepsCounter);
	}
}

char& Game::chooseGhostMode()
{
	char userChoice;

	system("cls");
	cout << "Please choose ghost mode:\n"
		<< "(1) Dumb\n"
		<< "(2) Smart\n"
		<< "(3) Super smart\n" << endl;

	userChoice = _getch();

	while (!validModeChoice(userChoice))
	{
		cout << "\nInvalid choice. Plese enter a valid number";
		userChoice = _getch();
	}

	return userChoice;
}

void Game::setGhostMood(const char& mood)
{
	int const ghostNum = Board::getLogSizeGhost();

	for (int i = 0; i < ghostNum; i++)
	{
		ghosts[i].setMode((ghostMood)(mood - '1'));
	}
}

void Game::SaveGameToFile(fstream& saveFile, bool& save, bool ghostmove, bool first)
{
	if (!save )
	{
		return;
	}
	if (first)
	{
		return;
	}

	int ghostSize = Board::getLogSizeGhost();
	Coord lastMove;
	char direction;

	// print pacman 
	saveFile << pacman.getPlayerChar();
	direction = findDirection(pacman.getPoint(), pacman.getLastMove());
		
	// delete the player char
	if (direction == -1)
	{
		saveFile.seekp(-1, std::ios_base::end);
	}
	else
	{
		saveFile << direction;
		saveFile << '\n';
	}
	if (fruit.getAppear() > 0)
	{
		saveFile << fruit.getPlayerChar();
		if (fruit.getAppear() == 1)
		{
			saveFile << ' ' << needToSetFruitChar;
			saveFile << ' ' << fruit.getPoint().getX();
			saveFile << ' ' << fruit.getPoint().getY();
			saveFile << ' ';
		}

		direction = findDirection(fruit.getPoint(), fruit.getLastMove());
		saveFile << direction;
		saveFile << '\n';
	}
	if (ghostmove)
	{
		// print ghost move 
		for (int i = 0; i < ghostSize; i++)
		{
			saveFile << ghosts[i].getPlayerChar();
			direction = findDirection(ghosts[i].getPoint(), ghosts[i].getLastMove());
			saveFile << direction;
		}
		saveFile << '\n';
	}	
}

char& Game::findDirection( Coord& point, Coord& lastPoint)
{
	int x = point.getX() - lastPoint.getX();
	int y = point.getY() - lastPoint.getY();
	char direction;
	if (x == 1)
	{
		direction = 'D';
	}
	else if (x == -1)
	{
		direction = 'U';
	}
	else if (y == -1)
	{
		direction = 'L';
	}
	else if (y == 1)
	{
		direction = 'R';
	}
	else if (x == 0 && y == 0)
	{
		direction = 'S';
	}
	else
	{
		direction = -1 ;
	}
	return direction;
}

bool Game::ResultFile(fstream& resFile, int& stepsCounter, bool win)
{
	
	resFile << stepsCounter;
	
	resFile.close();
	return true;
}

bool Game::fileTest(fstream& file)
{
	if (!file)
	{
		cout << "Error with file" << endl;
		return false;
	}
	return true;
}

string Game::renameScreen()
{
	string screenName = Board::getCurrScreen();
	screenName.resize(screenName.size() - 7);
	return screenName;
}

bool Game::openFiles(fstream& resFile, fstream& saveFile)
{
	if ((mode.compare("save") == 0))
	{
		string screenName = renameScreen();
		resFile.open(screenName + ".result", ios::out);
		if (!fileTest(resFile))
		{
			return false;
		}

		saveFile.open(screenName + ".steps", ios::out);
		if (!fileTest(saveFile))
		{
			return false;
		}

	}
	return true;

}

void Game::playByLoad()
{
	string filename = returnLoadFile(".steps");
	char playerChar, direction, nullchar;
	Directions_upper step;
	int x, y, indexGhost, logSizeGhost=Board::getLogSizeGhost();
	 stepsFile.open(filename, ios::in);
}

Directions_upper Game::convertDirection(char dir)
{
	Directions_upper res;
	switch (dir)
	{
	case 'R':
		res = Directions_upper::RIGHT;
		break;

	case 'D':
		res = Directions_upper::DOWN;
		break;

	case 'L':
		res = Directions_upper::LEFT;
		break;

	case 'U':
			res = Directions_upper::UP;
		break;
	case 'S':
		res= Directions_upper::STAY;
		break;

	}
	return res;
}

string Game::returnLoadFile(string end)
{
	int x = 0;
	string path = ".";
	fstream stepsFileload;

	for (const auto& file : std::filesystem::directory_iterator(path))
	{
		if (file.path().filename().extension() == end)
		{
			if (x >= fileIndex)
			{
				return (file.path().filename().string());
			}
			x++;
		}
	}
}

void Game::checkTest(int& stepsCounter)
{
	int result;
	string filename = returnLoadFile(".result");
	resFile.open(filename, ios::in);

	resFile >> result;

	system("cls");

	if (stepsCounter == result)
	{
		cout << "Test passed.\n";
	}

	else
	{
		cout << "Test failed.\n";
	}

	Menu::printMessage();
}


