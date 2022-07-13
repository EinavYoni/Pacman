#include "Menu.h"

void const Menu::printMenu()
{
	cout << "**********************************************************************\n"
		<< "*  Please choose one of the following options:                       *\n"
		<< "*  (1) Start a new game and choose board by yourself                 *\n"
		<< "*  (2) Start a new game with colors and choose board by yourself     *\n"
		<< "*  (3) Start a new game                                              *\n"
		<< "*  (4) Start a new game with colors                                  *\n"
		<< "*  (8) Present instructions and keys                                 *\n"
		<< "*  (9) EXIT	                                                     *\n"
		<< "**********************************************************************\n" << endl;
}

bool const Menu::validMenuChoice(const char& choice)
{
	return (choice == '1' || choice == '2' || choice == '3' || choice == '4'
		|| choice == '8' || choice == '9');
}

void const Menu::printInstructionsAndKeys()
{
	system("cls");
	cout << "   Instructions and keys\n  ***********************\n"
		<< "The player needs to use the arrows\nand eat all the food (-) on the screen\n"
		<< "without touching the ghosts or the walls\n\n LEFT ---- a / A\n"
		<< " RIGHT --- d / D \n UP ------ w / W \n Down ---- x / X\n STAY ---- s / S\n\n"
		<< "To quit game - press 'o' or 'O'\n" << endl;
}

void const Menu::startGame()
{
	char userChoice = '\n';
	Game game;

	if (Game::getMode().compare("load") == 0
		|| Game::getMode().compare("silent") == 0)
	{
		userChoice = '3'; // show game without colors
		game.playGame(userChoice);
		cout << "GoodBye!\n";
		return;
	}

	do
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		printMenu();
		userChoice = _getch();

		while (!validMenuChoice(userChoice))
		{
			cout << "\nInvalid choice. Plese enter a valid number";
			userChoice = _getch();
		}

		switch (userChoice - '0')
		{
		case 1:
		case 2:
		case 3:
		case 4:
			game.playGame(userChoice);
			break;
		case 8:
			printInstructionsAndKeys();
			printMessage();
			break;
		}
	} while (userChoice != '9');

	system("cls");
	cout << "GoodBye!\n";
}

void const Menu::printMessage()
{
	cout << "\nPress any key to continue";

	if (_getch())
		system("cls");
}

void Menu::validInput(int argc, char* argv[])
{
	bool load = false;

	if (argc == 0)
	{
		startGame();
		return;
	}

	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], " ") != 0)
		{
			if (strcmp(argv[i], "-save") == 0)
			{
				if (Game::getMode().compare("simple") == 0)
				{
					Game::setMode("save");
				}
				else
				{
					invalidInputMessage();
					return;
				}
			}
			else if (strcmp(argv[i], "-load") == 0)
			{
				if (!isFileExist(".steps") || Game::getMode().compare("simple") != 0)
				{
					invalidInputMessage();
					return;
				}
				else
				{
					Game::setMode("load");
					load = true;
				}
			}
			else if (strcmp(argv[i], "[-silent]") == 0)
			{
				if (!isFileExist(".result") || !load)
				{
					invalidInputMessage();
					return;
				}
				else
				{
					Game::setMode("silent");
				}
			}
			else
			{
				invalidInputMessage();
				return;
			}
		}
	}

	startGame();
	return;
}

bool Menu::isFileExist(string end)
{
	string path = ".";
	int screens = 0, steps = 0, results = 0;


	for (const auto& file : std::filesystem::directory_iterator(path))
	{
		if (file.path().filename().extension() == ".screen")
		{
			screens++;
		}
	}

	for (const auto& file : std::filesystem::directory_iterator(path))
	{
		if (file.path().filename().extension() == ".steps")
		{
			steps++;
		}
	}

	for (const auto& file : std::filesystem::directory_iterator(path))
	{
		if (file.path().filename().extension() == ".result")
		{
			results++;
		}
	}

	if (end == ".screen")
	{
		return (screens > 0);
	}

	else if (end == ".steps")
	{
		return (screens == steps && screens > 0);
	}

	else
	{
		return (screens == steps && steps == results && screens > 0);
	}

}
void Menu::invalidInputMessage()
{
	cout << "Invalid input!\n";
}