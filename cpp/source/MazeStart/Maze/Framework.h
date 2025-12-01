#if !defined(FRAMEWORK_H)
#define FRAMEWORK_H
#include <string.h>
#include <SFML/Graphics.hpp>
extern sf::RenderWindow gWindow;

bool UpdateFramework();
void DrawCircle(int x,int y, int radius);
void DrawRectangle(int x, int y, int width, int height);
void ChangeColour(unsigned char red, unsigned char green,
	unsigned char blue, unsigned char alpha = 255);
void StartClock();
int StopClock();
float GetElapsedTime();

enum class EKeyPressed
{
	eNone,
	eLeft,
	eUp,
	eRight,
	eDown
};

enum class EButton
{
	eLeft,
	eRight,
	eMiddle
};

bool IsButtonPressed(EButton whichButton);
void GetMousePosition(int& x, int& y);
EKeyPressed GetLastKeyPressed();

void drawText(const std::string& textToDisplay);
void initFont(const std::string& fontName = "arial.ttf");

#endif
