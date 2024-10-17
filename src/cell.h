#pragma once

class Cell{
public:
	Cell(int x = 0, int y = 0, bool alive = false);
	int x;
	int y;
	int alive;
};
