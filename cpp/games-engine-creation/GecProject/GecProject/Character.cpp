#include "Character.h"
#include <SFML/Graphics.hpp>
#include <iostream>


Character::Character(std::string characterName, int characterID, int width, int height, float loc_x, float loc_y, int maxFrames)
{
	std::string filePath = "";
	switch (characterID) {
		case (0) : 
			filePath = "spritesheet.png";
	}
	if (!m_loadedTexture.loadFromFile(filePath)) {
		std::cout << "it failed";
	}
	else {
		sf::Vector2u texXY = m_loadedTexture.getSize();

		m_width = width;
		m_height = height;

		m_loc_x = loc_x;
		m_loc_y = loc_y;

		m_maxFrames = texXY.x / m_width;
		m_sprite = new sf::Sprite(m_loadedTexture);
		m_sprite->setTextureRect(sf::IntRect({ m_currentFrame * m_width, 0 }, { m_width, m_height }));
		m_sprite->setPosition(sf::Vector2f(m_loc_x, m_loc_y));
	}
}

void Character::draw(sf::RenderWindow* window) {
	window->draw(*m_sprite);
}

void Character::update(sf::Time dt) {
	m_timer += dt;
	sf::Time animationSpeed = sf::seconds(0.1f);

	if (m_timer >= animationSpeed) {
		m_timer = -animationSpeed;
		m_sprite->setTextureRect(sf::IntRect({ m_currentFrame * m_width, 0 }, { m_width, m_height }));
		nextFrame();
	}
}

void Character::nextFrame() {
	m_currentFrame++;
	if (m_currentFrame >= m_maxFrames) {
		m_currentFrame = 0;
	}
	m_sprite->setTextureRect(sf::IntRect({ m_currentFrame * m_width, 0 }, { m_width, m_height }));
}

void Character::move(sf::RenderWindow* window) {
	sf::Vector2u size = window->getSize();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
		std::cout << "\nW\n";
		if (m_loc_y >= 0) {
			std::cout << "entered loop\n";
			m_loc_y -= move_speed;

			std::cout << m_loc_y;
			if (m_loc_y < size.y) {
				m_loc_y = 0;
			}
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
		if (m_loc_y >= 0) {
			m_loc_y += move_speed;
			if (m_loc_y > size.y) {
				m_loc_y = 0;
			}
		}
	}
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
		if (m_loc_x >= 0) {
			m_loc_x += move_speed;
			if (m_loc_x < size.x) {
				m_loc_x = 0;
			}
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
		if (m_loc_x >= 0) {
			m_loc_x += move_speed;
			if (m_loc_x > size.x) {
				m_loc_x = 0;
			}
		}
	}
}
