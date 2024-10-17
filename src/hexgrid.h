#pragma once
#include "SFML/Graphics/Texture.hpp"
#include "cell.h"
#include <cstdint>

class Hexgrid {
	int width;
	int height;
	int cellSize;
	sf::Texture texture;	
	Cell *cells;
	Cell *cellsNew;
public:
	uint8_t *pixels;
	Hexgrid(sf::Texture txt ,int width, int height, int cellSize);
	~Hexgrid();
	void hexgridInit();
	void applyRules();
	void clear();
	void drawHexagon(int x, int y,int size);
	void drawLine(int x0, int y0, int x1, int y1, uint8_t color);
	void drawPixel(int x, int y, uint8_t color);
	void renderGrid();
	void reset();
	uint8_t countNeighbours(Cell cells[], Cell cell);
};
