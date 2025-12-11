#ifndef CIRCLE_H
#define CIRCLE_H

#include <cstdlib>

class Circle
{
public:
    // Constructors
    Circle();
    unsigned char GetRed();
    unsigned char GetGreen();
    unsigned char GetBlue();
    unsigned char GetAlpha();

    void SetRed(int value);
    void SetGreen(int value);
    void SetBlue(int value);
    void SetAlpha(int value);

	int GetX();
	int GetY();
	int GetRadius();
	int GetVelX();
	int GetVelY();
	int GetUpOrDown();

	void SetX(int value);
	void SetY(int value);
	void SetRadius(int value);
	void SetVelX(int value);
	void SetVelY(int value);
	void SetUpOrDown(int value);

    void Update();
    void Draw();

private:
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;

    int x;
    int y;
    int radius;
    int velx;
    int vely;
    int upOrDown;


};

#endif
