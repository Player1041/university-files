#include "Circle.h"

extern int gScreenWidth;
extern int gScreenHeight;

// Default constructor makes a random circle (same as your createCircle())
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
