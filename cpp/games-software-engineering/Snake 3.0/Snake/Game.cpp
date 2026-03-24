#include "Game.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>
#include "Constants.h"
#include "ItemManager.h"
#include "PlayerManager.h"
#include "QuestionManager.h"




sf::Vector2f gridToPixel(int col, int row)
{
    return sf::Vector2f(
        static_cast<float>(col * CELL_SIZE),
        static_cast<float>(row * CELL_SIZE)
    );
}