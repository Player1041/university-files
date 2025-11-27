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

struct Circle
{
	int x;
	int y;
	int radius;
	int velx;
	int vely;
	int upOrDown;
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
};

int numOfCircles = 1;



Circle createCircle() {
	Circle newCircle;
	newCircle.radius = 10 + rand() % 11;
	newCircle.x = rand() % gScreenWidth - (2 * newCircle.radius);
	newCircle.y = rand() % gScreenHeight - (2 * newCircle.radius);
	newCircle.velx = 1 + rand() % 10;
	newCircle.vely = 1 + rand() % 10;
	newCircle.r = rand() % 256;
	newCircle.g = rand() % 256;
	newCircle.b = rand() % 256;
	newCircle.upOrDown = rand() % 2;

	return newCircle;
}

using namespace std;
int main()
{
	vector<Circle> circles;
	circles.push_back(createCircle());
	srand(time(0));



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

		for (size_t i = 0; i < circles.size(); i++) {
			ChangeColour(circles[i].r, circles[i].g, circles[i].b, circles[i].a);
			DrawCircle(circles[i].x, circles[i].y, circles[i].radius);

			if (circles[i].r <= 0 || circles[i].g <= 0 || circles[i].b <= 0) {
				circles[i].upOrDown = 1;
			}
			else if (circles[i].r >= 255 || circles[i].g >= 255 || circles[i].b >= 255) {
				circles[i].upOrDown = 0;
			}

			if (circles[i].upOrDown == 0) {
				circles[i].r = circles[i].r - 1;
				circles[i].g = circles[i].g - 1;
				circles[i].b = circles[i].b - 1;
				circles[i].a = 255;
			}
			else if (circles[i].upOrDown == 1) {
				circles[i].r = circles[i].r + 1;
				circles[i].g = circles[i].g + 1;
				circles[i].b = circles[i].b + 1;
				circles[i].a = 255;
			};

			if (circles[i].x > gScreenWidth - (2 * circles[i].radius) || circles[i].x < 0) {
				circles[i].velx = -circles[i].velx;
				circles.push_back(createCircle());
			}
			if (circles[i].y > gScreenHeight - (2 * circles[i].radius) || circles[i].y < 0) {
				circles[i].vely = -circles[i].vely;
				circles.push_back(createCircle());
			}

				circles[i].x += circles[i].velx;
				circles[i].y += circles[i].vely;
			}
		}

		cout << "Bye bye" << endl;

		return 0;
	}
