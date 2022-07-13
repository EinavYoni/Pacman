#include "Board.h"

int Board::Width = 0;
vector<Cell> Board::BoardGame;
string Board::currScreen;;
Coord Board::Legend;
int Board::Length = 0;
int Board::breadCrumbs = 0;
int Board::LogSizeGhost = 0;
int Board::x = 0;

void const Board::print(const bool color, bool silentMode)
{
	int loc;

	system("CLS");

	for (int i = 0; i < Length; i++)
	{
		for (int j = 0; j < Width; j++)
		{
			 loc = i * Width + j;

			 if (BoardGame[loc].getCellStatus() == CellStatus::FOOD)
			 {
				 breadCrumbs++;
			 }

			if (BoardGame[loc].getPlayerStatus() == PlayerInCell::NONE)
			{
				if (!silentMode)
				{
					printWithColor(color, (int)BoardGame[loc].getCellStatus(), (int)colors::Obj);
					cout << StatusChars[(int)BoardGame[loc].getCellStatus()];
				}
			}

			else
			{
				if (!silentMode)
				{
					printWithColor(color, (int)BoardGame[loc].getPlayerStatus(), (int)colors::NotObj);
					cout << PlayersChars[(int)BoardGame[loc].getPlayerStatus()];
				}
			}

			BoardGame[loc].setPointt(i, j);
		}
		cout << "\n";
	}
}

bool Board::convertBoard(char& menuChoice)
{
	ifstream file;
	bool res = true;

	if (menuChoice == '1' || menuChoice == '2')
	{
		if (getFileFromUser(file))
		{
			if (!loadFileToVector(file))
			{
				BoardGame.clear();
				res = false;
			}
			file.close();
		}

		else
		{
			res = false;
		}
	}

	else if (menuChoice == '3' || menuChoice == '4')
	{
		vector<string> boardsFiles = readAllFiles();

		bool loadScreen = true;

		if (x < boardsFiles.size())
		{
			while (loadScreen)
			{
				file.open(boardsFiles[x], ios::in);
				currScreen= boardsFiles[x];
				if (!loadFileToVector(file))
				{
					BoardGame.clear();
					if (x == boardsFiles.size() - 1)
					{
						loadScreen = false;
						res = false;
					}
				}
				else
				{
					loadScreen = false;
				}
				file.close();
				x++;
			}
		}

		else
		{
			cout << "no files.\n\n";
			res = false;
		}

		if (x == boardsFiles.size())
		{
			boardsFiles.clear();
			x = 0;
		}
	}

	return res;
}

void Board::printWithColor(int color, int statColor, int cellORPlayer)
{
	if (color)
	{
		if (cellORPlayer == (int)colors::Obj)
		{
			if (statColor == (int)CellStatus::FOOD)
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (int)colors::FoodColor);
			else if (statColor == (int)CellStatus::WALL)
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (int)colors::WallColor);
			else
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		}

		else
		{
			if (statColor == (int)PlayerInCell::GHOST)
			{
				colors color = colors::GhostColor1;

				ghostColorINDEX++;

				if (ghostColorINDEX == getLogSizeGhost() + 1)
				{
					ghostColorINDEX = 1;
				}
				if (ghostColorINDEX == 1)
				{
					color = colors::GhostColor1;
				}
				if (ghostColorINDEX == 2)
				{
					color = colors::GhostColor2;
				}
				if (ghostColorINDEX == 3)
				{
					color = colors::GhostColor3;
				}
				if (ghostColorINDEX == 4)
				{
					color = colors::GhostColor4;
				}
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (int)color);
			}
			else if (statColor == (int)PlayerInCell::PACMAN)
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (int)colors::pacmanColor);
			else
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (int)colors::FruitColor);
			}
		}
	}
}

bool const Board::getFileFromUser(ifstream& file)
{
	cout << "Please enter file name: " << endl;
	string inFileName;
	cin >> inFileName;
	file.open(inFileName, ios::in);
	currScreen = inFileName;
	if (file.fail())
	{
		cout << "\nfile not found!\n\n";
		return false;
	}

	return true;
}

bool Board::loadFileToVector(istream& file)
{
	int status = (int)CellStatus::FOOD, currplayer = (int)PlayerInCell::NONE;
	int pacmanCounter = 0, legendCounter = 0;
	int charsInRow = 0, rowsCounter = 0;
	Cell curr;
	char input = file.get();
	bool firstLine = true;
	string in;

	LogSizeGhost = 0;

	if (input == '\n')
	{
		noFirstLineError();
		return false;
	}

	if (file.eof())
	{
		emptyFileError();
		return false;
	}

	while (!file.eof())
	{
		if (!(file.good()))
		{
			throw runtime_error("File corrupt!");
		}

		charsInRow++;
		if (charsInRow > MAX_CHARS_IN_ROW)
		{
			over80charsError();
			return false;
		}

		if (input == WALL_CHAR) status = (int)CellStatus::WALL;
		else if (input == NOFOOD_CHAR) status = (int)CellStatus::EMPTY;
		else if (input == PACMAN_CHAR)
		{
			pacmanCounter++;
			if (pacmanCounter > 1)
			{
				overOnePacmanError();
				return false;
			}
			status = (int)CellStatus::EMPTY;
			currplayer = (int)PlayerInCell::PACMAN;
		}
		else if (input == GHOST_CHAR)
		{
			LogSizeGhost++;
			if (LogSizeGhost > 4)
			{
				overFourGhostsError();
				return false;
			}
			currplayer = (int)PlayerInCell::GHOST;
		}
		else if (input == LEGEND_CHAR)
		{
			legendCounter++;
			if (charsInRow + 20 > MAX_CHARS_IN_ROW)
			{
				noPlaceForLegendError();
				return false;
			}
			if (legendCounter > 1)
			{
				moreThanOneLegendError();
				return false;
			}
			status = (int)CellStatus::LEGEND;
			currplayer = (int)PlayerInCell::NONE;
		}
		else if (input != EMPTY_CHAR && input != FOOD_CHAR && input != '\n')
		{
			invalidCharError();
			return false;
		}
		if (input == '\n')
		{
			charsInRow = 0;
			rowsCounter++;
			if (rowsCounter > MAX_ROWS)
			{
				over25rowsError();
				return false;
			}
			if (firstLine)
			{
				Width = BoardGame.size();
			}
			firstLine = false;
		}
		else
		{
			curr.setCellStatus(status);
			curr.setPlayerInCell(currplayer);
			BoardGame.push_back(curr);
		}

		input = file.get();
		status = (int)CellStatus::FOOD;
		currplayer = (int)PlayerInCell::NONE;
	}

	if (pacmanCounter == 0)
	{
		noPacmanError();
		return false;
	}

	if (legendCounter == 0)
	{
		noLegendError();
		return false;
	}

	Length = BoardGame.size() / Width;
	setLegend();
	setTube();

	return true;
}

void Board::invalidCharError()
{
	system("cls");
	cout << "Invalid file. Includes invalid char.\n";
	cout << "Press any key to continue";
	if (_getch())
	{
		system("cls");
	}
}

void Board::noLegendError()
{
	system("cls");
	cout << "Invalid file. No legend sign.\n";
	cout << "Press any key to continue";
	if (_getch())
	{
		system("cls");
	}
}

void Board::noPacmanError()
{
	system("cls");
	cout << "Invalid file. No pacman sign.\n";
	cout << "Press any key to continue";
	if (_getch())
	{
		system("cls");
	}
}

void Board::over25rowsError()
{
	system("cls");
	cout << "Invalid file. More than 25 rows.\n";
	cout << "Press any key to continue";
	if (_getch())
	{
		system("cls");
	}
}

void Board::moreThanOneLegendError()
{
	system("cls");
	cout << "Invalid file. More than one legend sign.\n";
	cout << "Press any key to continue";
	if (_getch())
	{
		system("cls");
	}
}

void Board::noPlaceForLegendError()
{
	system("cls");
	cout << "Invalid file. Not enough place for legend.\n";
	cout << "Press any key to continue";
	if (_getch())
	{
		system("cls");
	}
}

void Board::overFourGhostsError()
{
	system("cls");
	cout << "Invalid file. More than 4 ghosts.\n";
	cout << "Press any key to continue";
	if (_getch())
	{
		system("cls");
	}
}

void Board::overOnePacmanError()
{
	system("cls");
	cout << "Invalid file. More than one pacman.\n";
	cout << "Press any key to continue";
	if (_getch())
	{
		system("cls");
	}
}

void Board::over80charsError()
{
	system("cls");
	cout << "Invalid file. More than 80 characters in a row.\n";
	cout << "Press any key to continue";
	if (_getch())
	{
		system("cls");
	}
}

void Board::emptyFileError()
{
	system("cls");
	cout << "Invalid file. Empty file.\n";
	cout << "Press any key to continue";
	if (_getch())
	{
		system("cls");
	}
}

void Board::noFirstLineError()
{
	system("cls");
	cout << "Invalid file. First line doesn't exist.\n";
	cout << "Press any key to continue";
	if (_getch())
	{
		system("cls");
	}
}

void const Board::setTube()
{
	int i;
	int width = Board::getWidth(), length = Board::getLength();
	vector <Cell>& board = Board::getBoardGame();

	for (i = 0; i < width; i++)
	{
		if (board[i].getCellStatus() != CellStatus::WALL)
		{
			if (board[(width) * (length - 1) + i].getCellStatus() != CellStatus::WALL)
			{
				board[i].setCellStatus((int)CellStatus::TUBE);
				board[(width) * (length - 1) + i].setCellStatus((int)CellStatus::TUBE);
				//set tube exit 
				board[i].settubeExit((length - 1), i);
				board[(width) * (length - 1) + i].settubeExit(0, i);
			}
		}
	}
	// side line
	for (i = 0; i < length; i++)
	{
		if (board[i * width].getCellStatus() != CellStatus::WALL)
		{
			if (board[(i * width) + width - 1].getCellStatus() != CellStatus::WALL)
			{
				board[i * width].setCellStatus((int)CellStatus::TUBE);
				board[(i * width) + width - 1].setCellStatus((int)CellStatus::TUBE);

				board[i * width].settubeExit(i, width - 1);
				board[(i * width) + width - 1].settubeExit(i, 0);
			}
		}
	}


}

void Board::setLegend()
{
	int legendX = 0, legendY = 0;
	int x, y, status = (int)CellStatus::LEGEND;

	for (int i = 0; i < Length; i++)
	{
		for (int j = 0; j < Width; j++)
		{
			if (BoardGame[i * Width + j].getCellStatus() == CellStatus::LEGEND)
			{
				legendX = i;
				legendY = j;
			}
		}
	}

	for (int i = 0; i < 3; i++)
	{
		x = legendX + i;
		for (int j = 0; j < 20; j++)
		{
			y = legendY + j;
			// put wall on frame
			if (i == 0 || i == 2 || j == 0 || j == 19)
			{
				status = (int)CellStatus::WALL;
			}

			BoardGame[x * Width + y].setCellStatus(status);
			status = (int)CellStatus::LEGEND;
		}
	}

	BoardGame[(legendX + 1) * Width + (legendY + 1)].setCellStatus((int)CellStatus::LEGEND);
	BoardGame[x * Width + y].setPlayerInCell((int)PlayerInCell::NONE);
	Legend.setCoord((legendX + 1) , (legendY + 1));
}

vector<string> Board::readAllFiles()
{
	vector<string> boardsFiles;
	string path = ".";

	for (const auto& file : std::filesystem::directory_iterator(path))
	{
		if (file.path().filename().extension() == ".screen")
		{
			boardsFiles.push_back(file.path().string());
		}
	}

	sort(boardsFiles.begin(), boardsFiles.end());

	return boardsFiles;
}