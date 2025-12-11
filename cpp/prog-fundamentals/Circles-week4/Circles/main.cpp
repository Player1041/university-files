/*
	Start shape drawing file
*/

#include <iostream>
#include <vector>
#include <time.h>
#include <stddef.h>
#include "Framework.h"

// Variables with global scope - do not rename them, feel free to change values though


// Screen dimensions
int gScreenWidth{ 800 };
int gScreenHeight{ 600 };

// Delay to slow things down
int gTimeDelayMS{ 5 };

// variables for circle location
//int x = 0;
//int y = 0;
//int radius = 50;

//int velx = 5;
//int vely = 5;
const int numOfCircles = 1000;

int xs[numOfCircles];
int ys[numOfCircles];
int velxs[numOfCircles];
int velys[numOfCircles];
int radii[numOfCircles];
int upOrDown[numOfCircles];

unsigned char rs[numOfCircles];
unsigned char gs[numOfCircles];
unsigned char bs[numOfCircles];
unsigned char as[numOfCircles];



using namespace std;

int main()
{
	srand(time(0));
	for (size_t i = 0; i < numOfCircles; i++) {
		radii[i] = 10 + rand() % 11;
		xs[i] = rand() % gScreenWidth - (2 * radii[i]);
		ys[i] = rand() % gScreenHeight - (2 * radii[i]);
		velxs[i] = 1 + rand() % 10;
		velys[i] = 1 + rand() % 10;
		rs[i] = rand() % 256;
		gs[i] = rand() % 256;
		bs[i] = rand() % 256;
		upOrDown[i] = rand() % 2;

	}

	cout << "Hello circles" << endl;

	while (UpdateFramework())
	{

		// Draws a circle at numOfCircles,200 with radius 20
		//if (x > gScreenWidth - (2 * radius) || x < 0) velx = -velx;
		//if (y > gScreenHeight - (2 * radius) || y < 0) vely = -vely;

		//x += velx;
		//y += vely;

		//unsigned char r = rand() % 256;
		//unsigned char g = rand() % 256;
		//unsigned char b = rand() % 256;
		//unsigned char a = 255;


		//ChangeColour(r, g, b, a);
		//DrawCircle(x, y, radius);

		for (size_t i = 0; i < numOfCircles; i++) {
			ChangeColour(rs[i], gs[i], bs[i], as[i]);
			DrawCircle(xs[i], ys[i], radii[i]);

			if (rs[i] <= 0 || gs[i] <= 0 || bs[i] <= 0) {
				upOrDown[i] = 1;
			}
			else if (rs[i] >= 255 || gs[i] >= 255 || bs[i] >= 255) {
				upOrDown[i] = 0;
			}
			
			if (upOrDown[i] == 0) {
				rs[i] = rs[i] - 1;
				gs[i] = gs[i] - 1;
				bs[i] = bs[i] - 1;
				as[i] = 255;
			}
			else if (upOrDown[i] == 1) {
			rs[i] = rs[i] + 1;
			gs[i] = gs[i] + 1;
			bs[i] = bs[i] + 1;
			as[i] = 255;
			};
			if (xs[i] > gScreenWidth - (2 * radii[i]) || xs[i] < 0) velxs[i] = -velxs[i];
			if (ys[i] > gScreenHeight - (2 * radii[i]) || ys[i] < 0) velys[i] = -velys[i];

			xs[i] += velxs[i];
			ys[i] += velys[i];
		}
	}

	cout << "Bye bye" << endl;

	return 0;
}
