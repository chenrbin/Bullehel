#include <iostream>
#include <SFML/Graphics.hpp>
#include "Constants.h"
#include "Drawings.h"
#include "Characters.h"

using namespace std;
using namespace Constants;

int main(){
    sf::Texture texture;
    if (!texture.loadFromFile("reimoo.png"))
        return -1;
    sf::ContextSettings windowSettings;
    windowSettings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(1600, 900), "Bullet Hell", sf::Style::Close | sf::Style::Titlebar, windowSettings);
    window.setKeyRepeatEnabled(false);
    window.setFramerateLimit(60);

    Player reimoo(PLAYERSTANDARDSPEED, PLAYERHITBOXRADIUS, texture);
    SfRectangleAtHome screen(GRAY, { SCREENWIDTH, SCREENHEIGHT }, { SCREENLEFT, SCREENTOP });
    SfRectangleAtHome border1(TRANSPARENT, { SCREENWIDTH, SCREENHEIGHT }, { SCREENLEFT, SCREENTOP }, false, WHITE, 1);
    SfRectangleAtHome border2(TRANSPARENT, { SCREENWIDTH + 2, SCREENHEIGHT + 2 }, { SCREENLEFT - 1, SCREENTOP - 1 }, false, BLACK, 50);
    reimoo.setbounds(screen.getGlobalBounds());
    while (window.isOpen())
    {
        sf::Event event;
        sf::Vector2f movement();
        float shiftScale = 1;

        reimoo.checkMovement();
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                break;
            default:
                break;
            }
        }

        
        window.clear();
        window.draw(screen);
        reimoo.drawCharacter(window);
        window.draw(border1);
        window.draw(border2);
        window.display();
    }

    return 0;
}