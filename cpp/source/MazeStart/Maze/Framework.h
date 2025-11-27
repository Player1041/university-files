#if !defined(FRAMEWORK_H)
#define FRAMEWORK_H

bool UpdateFramework();
void DrawCircle(int x,int y, int radius);
void DrawRectangle(int x, int y, int width, int height);
void ChangeColour(unsigned char red, unsigned char green,
	unsigned char blue, unsigned char alpha = 255);
void StartClock();
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


#endif
