#ifndef CIRCLE_H
#define CIRCLE_H

#include <cstdlib>

class Circle
{
public:
    // Constructors
    Circle();
    // Public properties (or make private + getters/setters if you want)
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

#endif
