#include "AnimatedSprite.h"
#include <iostream>
AnimatedSprite::AnimatedSprite(std::string filePath, int width, int height, float loc_x, float loc_y, int maxFrames) {

	if (!m_loadedTexture.loadFromFile(filePath)) {
		std::cout << "Shit failed";
	}
	else {
		sf::Vector2u texXY = m_loadedTexture.getSize();


		m_width = width;
		m_height = height;

		m_x = loc_x;
		m_y = loc_y;

		m_maxFrames = texXY.x / m_width;
		m_sprite = new sf::Sprite(m_loadedTexture);
		m_sprite->setTextureRect(sf::IntRect({ m_currentFrame * m_width, 0 }, { m_width, m_height }));
		m_sprite->setPosition(sf::Vector2f(m_x, m_y));

	}
}


void AnimatedSprite::draw(sf::RenderWindow* window) {
	window->draw(*m_sprite);
}

void AnimatedSprite::update(sf::Time dt) {
	m_timer += dt;
	sf::Time animationSpeed = sf::seconds(0.1f);

	if (m_timer >= animationSpeed) {
		m_timer = -animationSpeed;
		m_sprite->setTextureRect(sf::IntRect({ m_currentFrame * m_width, 0 }, { m_width, m_height }));
		NextFrame();
	}
}

void AnimatedSprite::NextFrame() {
	m_currentFrame++;
	if (m_currentFrame >= m_maxFrames) {
		m_currentFrame = 0;
	}
	m_sprite->setTextureRect(sf::IntRect({ m_currentFrame * m_width, 0 }, { m_width, m_height }));
}