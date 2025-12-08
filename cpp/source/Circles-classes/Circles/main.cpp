/*
	Start shape drawing file
*/

#include <iostream>
#include <vector>
#include <time.h>
#include <stddef.h>
#include "Framework.h"
#include "Circle.h"
// Variables with global scope - do not rename them, feel free to change values though


// Screen dimensions
int gScreenWidth{ 800 };
int gScreenHeight{ 600 };

// Delay to slow things down
int gTimeDelayMS{ 5 };

int numOfCircles = 1;

using namespace std;
int main()
{

	srand(time(0));

	vector<Circle> circles;
	circles.push_back(Circle());
	



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
				circles.push_back(Circle());
			}
			if (circles[i].y > gScreenHeight - (2 * circles[i].radius) || circles[i].y < 0) {
				circles[i].vely = -circles[i].vely;
				circles.push_back(Circle());
			}

				circles[i].x += circles[i].velx;
				circles[i].y += circles[i].vely;
			}
		}

		cout << "Bye bye" << endl;

		return 0;
	}
