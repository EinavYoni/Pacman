#pragma once

#include <iostream>
#include <conio.h>
#include "Game.h"

using std::cout;
using std::endl;
using std::system;

class Menu
{
private:
	void const printMenu();
	void const printInstructionsAndKeys();
	bool const validMenuChoice(const char& choice);
	bool isFileExist(string end);
	void invalidInputMessage();

public:
	static void const printMessage();
	void const startGame();
	void validInput(int argc, char* argv[]);
};