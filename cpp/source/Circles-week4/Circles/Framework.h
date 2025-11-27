#pragma once

bool UpdateFramework();
void DrawCircle(int x,int y, int radius);
void DrawRectangle(int x, int y, int width, int height);
void ChangeColour(unsigned char red, unsigned char green,
	unsigned char blue, unsigned char alpha = 255);

enum class EButton
{
	eLeft,
	eRight,
	eMiddle
};

bool IsButtonPressed(EButton whichButton);
void GetMousePosition(int& x, int& y);

