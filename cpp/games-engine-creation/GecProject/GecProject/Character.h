#pragma once
#include <SFML/Graphics.hpp>
#include <string.h>



class Character
{
public:
	Character(std::string characterName, int characterID, int width, int height, float loc_x, float loc_y, int maxFrames);

	// Sprite Updating
	void update(sf::Time dt);
	void draw(sf::RenderWindow* window);
	void nextFrame();
	void move(sf::RenderWindow* window);
private:

	// Generic Variables
	std::string characterName = "Generic Character";
	// Enemy List 
	// 0x00 - Default Enemy
	// 0x01 - Player Character
	//
	int characterID = 0;

	int health = 3;
	int move_speed = 1;

	float m_loc_x = 1;
	float m_loc_y = 1;

	// Sprite
	sf::Sprite* m_sprite = nullptr;
	sf::Texture m_loadedTexture = sf::Texture();

	int m_width = 0;
	int m_height = 0;

	int m_currentFrame = 0;
	int m_maxFrames = 0;

	sf::Time m_timer{ sf::Time::Zero };
	

};

