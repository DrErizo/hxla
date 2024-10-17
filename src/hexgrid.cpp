#include "SFML/Graphics/Texture.hpp"
#include "SFML/System/Err.hpp"
#include "SFML/Window/VideoMode.hpp"
#include "cell.h"
#include "hexgrid.h"
#include <X11/Xmd.h>
#include <cstdint>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <cstring>

int width;
int height;
int cellSize;
sf::Texture texture;	
sf::VideoMode screen = sf::VideoMode::getDesktopMode();
Hexgrid::Hexgrid(sf::Texture txt ,int w, int h, int size) : width(w), height(h), texture(txt), cellSize(size){};
Hexgrid::~Hexgrid(){
	delete pixels;
	delete cellsNew;
	delete cells;

};
uint8_t *pixels; 
Cell *cells;
Cell *cellsNew;
int amountOfCells;
int amountOfCellsInARow;
int amountOfCellsInAColumn;
int cell;
int pivY = 0;
int pivX = 0;
void Hexgrid::hexgridInit(){
  cell = 0;
  amountOfCellsInARow = (width / (int)(cellSize*2));
  amountOfCellsInAColumn = (height / (cellSize * 2));
  amountOfCells = amountOfCellsInARow * amountOfCellsInAColumn;
	pixels = new uint8_t[width * height * 4];
	cells = new Cell[amountOfCells];
	cellsNew = new Cell[amountOfCells];

	printf("%ix%i",width,height);
	if(!texture.create({(uint16_t)width, (uint16_t)height})){
		printf("Failed to Create Texture: %s", (char*)sf::err);
	}else{
      for(int y = 0; y<amountOfCellsInAColumn;++y){
		for(int x = 0; x<amountOfCellsInARow;++x){
            cell = y * amountOfCellsInARow + x;
			cells[cell].y = y; 
			cells[cell].x = x;
            cells[cell].alive = (rand() % 30 ? false : true);
		}
      }
	}
}
void Hexgrid::applyRules(){
  memcpy(cellsNew,cells,sizeof(Cell) * amountOfCells);

  for(int i = 0;i<amountOfCells;++i){
    int neighbours = countNeighbours(cells,cells[i]);
    
    //Overpopulation
  	if(cells[i].alive && neighbours > 2) cellsNew[i].alive = false;
    //Isolation
  	else if(cells[i].alive && neighbours < 2) cellsNew[i].alive = false;
    //Survival
  	else if(cells[i].alive && (neighbours == 3 || neighbours == 3)) cellsNew[i].alive = true;
    //Reproduction
  	else if(cells[i].alive == false && neighbours == 2) cellsNew[i].alive = true;
  }

  memcpy(cells,cellsNew,sizeof(Cell) * amountOfCells);
}
uint8_t Hexgrid::countNeighbours(Cell cells[], Cell cell){
    int neighbours = 0;
    int cellIndex = (cell.y * amountOfCellsInARow + cell.x);

    for(int y = 0 ; y < 3 ; ++y){
        pivY = (y == 1) ? -1: ((y == 2) ? 0 : 1);
        for(int x = 0 ; x < 3 ; ++x){
            pivX = (x == 1) ? -1: ((x == 2) ? 0 : 1);
            if(!(pivX == 0 && pivY == 0)){
                if(!((cellIndex > amountOfCells) || (cellIndex <  0))){
                    if(!((cell.x % amountOfCellsInARow) == 0 || (cell.y % amountOfCellsInAColumn) == 0)){
                        if(cells[(cell.y + pivY) * amountOfCellsInARow + (cell.x + pivX)].alive) neighbours++; 
                    }
                }
            }
        }
    }

    return neighbours;
    
}
void Hexgrid::renderGrid(){
  int stage = 1;
  for(int y = 0 ; y < amountOfCellsInAColumn ; ++y){
      for(int x = 0; x < amountOfCellsInARow ; ++x){
          if(stage){
            if(cells[amountOfCellsInARow * y + x].alive){
              drawHexagon((x * (cellSize*2)),(y * cellSize*2),cellSize);
            }          }else{
            if(cells[amountOfCellsInARow * y + x].alive){
              drawHexagon((x * (cellSize*2)),(y * cellSize * 2 + (cellSize)),cellSize);
            } 
          }
      stage = !stage;
      }
  }
}
void Hexgrid::drawPixel(int x, int y, uint8_t color){
    for(int j=0;j<=3;j++) pixels[y * screen.size.x + x  + j] = color;
}
void Hexgrid::drawLine(int x0, int y0, int x1, int y1, uint8_t color){
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        int pixelIndex = (y0 * screen.size.x + x0) * 4 ;
        if (pixelIndex >= 0 && pixelIndex < screen.size.x * screen.size.y * 4) {
        	drawPixel((pixelIndex - (((int)(pixelIndex / width)) * width)), ((int)(pixelIndex / width)), 255);
        }

        if (x0 == x1 && y0 == y1) {
            break;
        }

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}
void Hexgrid::drawHexagon(int x, int y,int size){
//    for (int i = 0; i < 6; i++) {
//        drawLine(round(x + size * cos((2 * M_PI / 6) * i) + size),round( y + size * sin((2 * M_PI / 6) * i)+ size),round(x + size * cos((2 * M_PI / 6) * i+1)+ size),round( y + size * sin((2 * M_PI / 6) * i+1)+ size));
//    }

    for (int i = 0; i < 6; i++) {
        int x0 = round(x + size * cos((2 * M_PI / 6) * i) + size);
        int y0 = round( y + size * sin((2 * M_PI / 6) * i)+ size);
        int x1 = round(x + size * cos((2 * M_PI / 6) * i+1)+ size);
        int y1 = round( y + size * sin((2 * M_PI / 6) * i+1)+ size);

        int dx = abs(x1 - x0);
        int dy = abs(y1 - y0);
        int sx = (x0 < x1) ? 1 : -1;
        int sy = (y0 < y1) ? 1 : -1;
        int err = dx - dy;

        while (true) {
            int pixelIndex = (y0 * screen.size.x + x0) * 4 ;
            if (pixelIndex >= 0 && pixelIndex < screen.size.x * screen.size.y * 4) {
                drawLine(x0,y0,x+cellSize,y+cellSize,255); 
            }

            if (x0 == x1 && y0 == y1) {
                break;
            }

            int e2 = 2 * err;
            if (e2 > -dy) {
                err -= dy;
                x0 += sx;
            }
            if (e2 < dx) {
                err += dx;
                y0 += sy;
            }
        }
    }

}
void Hexgrid::clear(){
	for(int i = 0; i<width * height * 4; i++) pixels[i] = 0;
}
void Hexgrid::reset(){
    cell = 0;
    for(int y = 0; y<amountOfCellsInAColumn;++y){
	    for(int x = 0; x<amountOfCellsInARow;++x){
            cell = y * amountOfCellsInARow + x;
            cells[cell].alive = (rand() % 30 ? false : true);
		}
    }
}
