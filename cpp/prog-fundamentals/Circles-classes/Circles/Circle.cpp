#include "Circle.h"
#include "Framework.h"

extern int gScreenWidth;
extern int gScreenHeight;

Circle::Circle()
{
    radius = 10 + rand() % 11;

    x = rand() % (gScreenWidth - 2 * radius);
    y = rand() % (gScreenHeight - 2 * radius);

    velx = 1 + rand() % 10;
    vely = 1 + rand() % 10;

    r = rand() % 256;
    g = rand() % 256;
    b = rand() % 256;
    a = 255;

    upOrDown = rand() % 2;
}

void Circle::Update()
{
    if (GetRed() <= 0 || GetGreen() <= 0 || GetBlue() <= 0) {
		SetUpOrDown(1);
    }
    else if (GetRed() >= 255 || GetGreen() >= 255 || GetBlue() >= 255) {
		SetUpOrDown(0);
    }

    if (upOrDown == 0) {
        SetRed(GetRed() - 1);
        SetGreen(GetGreen() - 1);
        SetBlue(GetBlue() - 1);
        SetAlpha(255);
    }
    else if (upOrDown == 1) {
        SetRed(GetRed() + 1);
        SetGreen(GetGreen() + 1);
        SetBlue(GetBlue() + 1);
        SetAlpha(255);
    };
	SetX(GetX() + GetVelX());
	SetY(GetY() + GetVelY());
}

void Circle::Draw()
{
    DrawCircle(x, y, radius);
}

unsigned char Circle::GetRed()
{
    return r;
}

unsigned char Circle::GetGreen()
{
    return g;
}

unsigned char Circle::GetBlue()
{
    return b;
}

unsigned char Circle::GetAlpha()
{
    return a;
}


void Circle::SetRed(int value)
{
    r = value;
}

void Circle::SetGreen(int value)
{
    g = value;
}

void Circle::SetBlue(int value)
{
    b = value;
}

void Circle::SetAlpha(int value)
{
    a = value;
}

int Circle::GetX()
{
    return x;
}

int Circle::GetY()
{
    return y;
}

int Circle::GetRadius()
{
    return radius;
}

int Circle::GetVelX()
{
    return velx;
}

int Circle::GetVelY()
{
    return vely;
}

int Circle::GetUpOrDown()
{
    return upOrDown;
}

void Circle::SetX(int value)
{
	x = value;
}

void Circle::SetY(int value)
{
    y = value;
}

void Circle::SetRadius(int value)
{
    radius = value;
}

void Circle::SetVelX(int value)
{
    velx = value;
}

void Circle::SetVelY(int value)
{
    vely = value;
}

void Circle::SetUpOrDown(int value)
{
    upOrDown = value;
}




  
