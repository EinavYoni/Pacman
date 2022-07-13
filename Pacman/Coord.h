#pragma once

class Coord
{
	int x, y;

public:
	Coord() { x = 0, y = 0; };
	void setCoord(int _x, int _y) { x = _x, y = _y; };
	int& const getX() { return x; };
	int& const getY() { return y; };
};