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
		for (size_t i = 0; i < circles.size(); i++) {

			ChangeColour(
				circles[i].GetRed(),
				circles[i].GetGreen(),
				circles[i].GetBlue(),
				circles[i].GetAlpha()
			);
			circles[i].Draw();
			circles[i].Update();

			if (circles[i].GetX() > gScreenWidth - (2 * circles[i].GetRadius()) || circles[i].GetX() < 0) {
				circles[i].SetVelX(-circles[i].GetVelX());
				circles.push_back(Circle());
			}

			if (circles[i].GetY() > gScreenHeight - (2 * circles[i].GetRadius()) || circles[i].GetY() < 0) {
				circles[i].SetVelY(-circles[i].GetVelY());
				circles.push_back(Circle());
			}
			}
		}
		cout << "Bye bye" << endl;
		return 0;
	}
