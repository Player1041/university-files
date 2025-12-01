/*
	GPF Week 6 - Maze Start Code
*/

#include <iostream>
#include <vector>
#include <time.h>
#include <stdio.h>

#include "Framework.h"
using namespace std;

// Screen dimensions
int gScreenWidth{ 800 };
int gScreenHeight{ 600 };

// Delay to slow things down
int gTimeDelayMS{ 100 };

// Maze size as constants
constexpr int kMazeColumnsX{ 20 };
constexpr int kMazeRowsY{ 20 };

/*
	2 Dimensional Arrays
	You can think of these as being an array of arrays
	
	The maze has kMazeColumnsX columns across (20 by default) represented by an array
	We then have kMazeRowsY (20) of these arrays, one for each row
	
	Columns and rows can be confusing so we tend to prefer working with x and y
	x is the horizontal axis (columns) across and y is the vertical axis (rows) down.

	Each single item is called a cell. 
	
	E.g. to output the value of the cell at 16 X (column 16) and 1 Y (row 1) we would write:
	cout << map[1][16] << endl; // outputs G
*/

char gMap[kMazeRowsY][kMazeColumnsX] = {
	// 20 columns (x axis) by 20 rows (y axis)
	// X0   X1   X2   X3   X4   X5   X6   X7   X8   X9   X10  X11  X12  X13  X14  X15  X16  X17  X18  X19 
	{ 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W' },	//Y0
	{ 'W', '.', '.', '.', '.', 'W', '.', '.', '.', 'W', 'W', '.', '.', '.', '.', 'W', '.', '.', '.', 'W' },	//Y1
	{ 'W', '.', 'W', 'W', 'W', 'W', 'W', 'W', '.', 'W', '.', '.', 'W', 'W', 'W', 'W', 'W', 'W', '.', 'W' },	//Y2
	{ 'W', '.', 'W', '.', '.', '.', 'W', '.', '.', 'W', '.', 'W', 'W', '.', 'W', '.', '.', '.', '.', 'W' },	//Y3
	{ 'W', '.', 'W', '.', 'W', '.', 'W', '.', 'W', 'W', '.', 'W', 'W', '.', 'W', 'W', 'W', '.', 'W', 'W' },	//Y4
	{ 'W', '.', 'W', '.', 'W', '.', '.', '.', '.', '.', '.', '.', 'W', '.', 'W', '.', '.', '.', '.', 'W' },	//Y5
	{ 'W', '.', 'W', '.', 'W', 'W', 'W', '.', '.', 'W', 'W', '.', 'W', '.', 'W', '.', 'W', '.', '.', 'W' },	//Y6
	{ 'W', '.', 'W', '.', 'W', '.', 'W', 'W', 'W', 'W', 'W', '.', 'W', '.', 'W', '.', 'W', 'W', 'W', 'W' },	//Y7
	{ 'W', '.', '.', '.', '.', '.', '.', '.', '.', '.', 'W', '.', '.', '.', 'W', '.', '.', '.', '.', 'W' },	//Y8
	{ 'W', '.', 'W', '.', 'W', '.', 'W', 'W', '.', 'W', 'W', 'W', '.', 'W', 'W', 'W', 'W', '.', 'W', 'W' },	//Y9
	{ 'W', '.', 'W', 'W', 'W', 'W', 'W', 'W', '.', 'W', 'W', 'W', '.', 'W', '.', 'W', 'W', '.', 'W', 'W' },	//Y10
	{ 'W', '.', '.', '.', '.', 'W', '.', '.', '.', 'W', 'W', '.', '.', '.', '.', 'W', '.', '.', '.', 'W' },	//Y11
	{ 'W', '.', 'W', 'W', 'W', 'W', 'W', 'W', '.', '.', '.', '.', 'W', '.', 'W', 'W', 'W', 'W', '.', 'W' },	//Y12
	{ 'W', '.', 'W', '.', '.', '.', 'W', '.', '.', 'W', 'W', '.', 'W', '.', 'W', '.', '.', '.', '.', 'W' },	//Y13
	{ 'W', '.', 'W', '.', 'W', '.', 'W', '.', 'W', 'W', 'W', '.', 'W', '.', 'W', '.', 'W', 'W', 'W', 'W' },	//Y14
	{ 'W', '.', 'W', '.', 'W', '.', '.', '.', '.', '.', 'W', '.', 'W', '.', 'W', '.', '.', '.', 'W', 'W' },	//Y15
	{ 'W', 'G', 'W', '.', 'W', 'W', 'W', '.', '.', 'W', 'W', '.', 'W', '.', '.', '.', 'W', '.', '.', 'W' },	//Y16
	{ 'W', '.', 'W', '.', 'W', '.', 'W', 'W', 'W', 'W', '.', '.', 'W', 'W', 'W', '.', 'W', 'W', '.', 'W' },	//Y17
	{ 'W', 'P', '.', '.', '.', '.', '.', '.', '.', 'W', '.', '.', '.', '.', '.', '.', '.', '.', '.', 'W' },	//Y18
	{ 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W' },	//Y19
};

int playerX = 1;
int playerY = 18;
bool goalReached = false;
bool mazeFullyDrawn = false;

void DrawMaze() {
	const int cellWidth = gScreenWidth / kMazeColumnsX;
	const int cellHeight = gScreenHeight / kMazeRowsY;

	// Colours:
	// 0  ,0  ,0	- Black
	// 255,255,255  - White
	// 255,0  ,0	- Red
	// 0,  0,  255	- Blue

	for (int y = 0; y < kMazeRowsY; y++) {
		for (int x = 0; x < kMazeColumnsX; x++) {
				char cell = gMap[y][x];
				switch (cell) {
				case 'W': // Wall
					ChangeColour(0, 0, 0);
					break;
				case '.': // Path
					ChangeColour(255, 255, 255);
					break;
				case 'G': //Goal
					ChangeColour(255, 0, 0);
					break;
				case 'P': //Player
					ChangeColour(0, 0, 255);
					playerX = x;
					playerY = y;
					break;
				}
				DrawRectangle(x * cellWidth, y * cellHeight, cellWidth, cellHeight);
		}
	}
	mazeFullyDrawn = true;
}

void HandleMovement() {
	int newPlayerX = playerX;
	int newPlayerY = playerY;
	switch (GetLastKeyPressed()) {
	case EKeyPressed::eLeft:
		newPlayerX = playerX - 1;
		if (newPlayerX >= 0 && gMap[playerY][newPlayerX] != 'W') {
			if (gMap[playerY][newPlayerX] == 'G') {
				goalReached = true;
			}
			if (!goalReached) {
				gMap[playerY][playerX] = '.';
				gMap[playerY][newPlayerX] = 'P';
			}
			playerX = newPlayerX;
		}
		break;
	case EKeyPressed::eRight:
		newPlayerX = playerX + 1;
		if (newPlayerX >= 0 && gMap[playerY][newPlayerX] != 'W') {
			if (gMap[playerY][newPlayerX] == 'G') {
				goalReached = true;
			}
			if (!goalReached) {
				gMap[playerY][playerX] = '.';
				gMap[playerY][newPlayerX] = 'P';
			}
			playerX = newPlayerX;
		}
		break;
	case EKeyPressed::eUp:
		newPlayerY = playerY - 1;
		if (newPlayerY >= 0 && gMap[newPlayerY][playerX] != 'W') {
			if (gMap[newPlayerY][playerX] == 'G') {
				goalReached = true;
			}
			if (!goalReached) {
				gMap[playerY][playerX] = '.';
				gMap[newPlayerY][playerX] = 'P';
			}
			playerY = newPlayerY;
		}
		break;
	case EKeyPressed::eDown:
		newPlayerY = playerY + 1;
		if (newPlayerY >= 0 && gMap[newPlayerY][playerX] != 'W') {
			if (gMap[newPlayerY][playerX] == 'G') {
				goalReached = true;
			}
			if (!goalReached) {
				gMap[playerY][playerX] = '.';
				gMap[newPlayerY][playerX] = 'P';
			}
			playerY = newPlayerY;
		}
		break;
	}
}

int main()
{
	// failed to get it to work, will look into it later
	// initFont(); 
	DrawMaze();
	if (mazeFullyDrawn == true)
		StartClock();
	bool hasWon = false;
	while (UpdateFramework())
	{
		
		DrawMaze();
		if (goalReached == false)
		{
			std::cout << "Elapsed Time: " << GetElapsedTime() << " seconds\r";
			HandleMovement();
		} else {
			if (!hasWon) {
				int finalTime = StopClock();
				std::cout << "You win!" << std::endl;
				std::cout << "Finish time: " << finalTime << " seconds\r";
				hasWon = true;
			}

		}

	}

	return 0;
}
