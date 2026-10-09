#pragma once

#include <SFML/Graphics.hpp>


class AnimatedSprite
{

public:
	AnimatedSprite(std::string filePath, int width, int height, float loc_x, float loc_y, int maxFrames);
	void update(sf::Time dt);
	void draw(sf::RenderWindow* window);


private:
	void NextFrame();

	sf::Sprite* m_sprite = nullptr;

	sf::Texture m_loadedTexture = sf::Texture();

	int m_maxFrames = 0;
	int m_currentFrame = 0;

	int m_width = 0;
	int m_height = 0;

	float m_x = 0;
	float m_y = 0;

	sf::Time m_timer{ sf::Time::Zero };

};

