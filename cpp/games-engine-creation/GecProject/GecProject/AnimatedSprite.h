#pragma once

#include <SFML/Graphics.hpp>


class AnimatedSprite
{

public:
	AnimatedSprite(std::string filePath, int width, int height, int maxFrames);
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

	sf::Time m_timer{ sf::Time::Zero };

};

