#pragma once

#include "Coord.h"

const char StatusChars[5] = { ' ', '#', '-', ' ', ' '};
const char PlayersChars[2] = { '@', '$' };

enum class CellStatus { EMPTY, WALL, FOOD, LEGEND , TUBE};
enum class PlayerInCell { PACMAN, GHOST, NONE, FRUIT};

class Cell
{
	int status;
	int playerInCell;
	Coord point;
	Coord tubeExit;
	Coord prev;

public:
	void setCell(int cell_stat, int player_stat) { status = cell_stat, playerInCell = player_stat; };
	void setCellStatus(int cell_stat) { status = cell_stat; };
	const CellStatus getCellStatus() { return (CellStatus)status; };
	void setPlayerInCell(int player_stat) { playerInCell = player_stat; };
	const PlayerInCell getPlayerStatus() { return (PlayerInCell)playerInCell; };
	void setPointt(int newX, int newY) { point.setCoord(newX, newY); };
	Coord& const getPoint() { return point; };
	void settubeExit(int x, int y) { tubeExit.setCoord(x, y); }
	int const gettubeExitX() { return tubeExit.getX(); }
	int const gettubeExitY() { return tubeExit.getY(); }
	int const getPrevX() { return prev.getX(); }
	int const getPrevY() { return prev.getY(); }
	void setPrev(int x, int y) { prev.setCoord(x, y); }
};