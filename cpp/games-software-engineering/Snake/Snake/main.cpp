/*
    Snake - Start Project

    This project is set up to link to SFML (a library supporting 2D graphics, sound, user input and networing)
    There is a subdirectory in the project called SFML-3.0.0 containing SFML headers and libraries
    The project settings are configured to look for these files

    SFML reference: https://www.sfml-dev.org/documentation/3.0.0/
    SFML Tutorials: https://www.sfml-dev.org/tutorials/3.0/
*/

#include <SFML/Graphics.hpp>
#include <list>

using namespace std;
using namespace sf;

int main()
{
    list<RectangleShape> rectangles = {};
    float x = 0;
    float y = 0;

    bool canInsertNewRect = true;

    sf::RenderWindow window(VideoMode({ 800, 600 }), "My window");

    while (window.isOpen()) {
        while (const optional event = window.pollEvent()) {
            if (event->is<Event::Closed>()) {
                window.close();
            }

            window.clear(sf::Color::Black);

            if (sf::Keyboard::isKeyPressed(Keyboard::Key::Space) && sf::Keyboard::isKeyPressed(Keyboard::Key::Left)); {
            if (canInsertNewRect) {
                RectangleShape rect({ 50.0f, 150.0f });
                rect.setPosition({ x, y });
                rect.setFillColor(Color::Green);
                rectangles.push_front(rect);
                x += 50;
                //y += 150;
                canInsertNewRect = false;
            }
            else {
                canInsertNewRect = true;
            }
        }

            for (const auto& rectangle : rectangles) {
                window.draw(rectangle);
			}

            window.display();
        }
	}
} 