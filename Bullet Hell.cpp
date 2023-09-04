#include <iostream>
#include <SFML/Graphics.hpp>
#include "Constants.h"
#include "Drawings.h"
#include "Characters.h"

using namespace std;
using namespace Constants;

int main(){

    sf::ContextSettings windowSettings;
    windowSettings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(1600, 900), "Bullet Hell", sf::Style::Close | sf::Style::Titlebar, windowSettings);
    Player reimoo;
    window.setKeyRepeatEnabled(false);
    window.setFramerateLimit(60);
    while (window.isOpen())
    {
        sf::Event event;
        sf::Vector2f movement();
        float shiftScale = 1;
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
            shiftScale = 0.5;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            reimoo.move(-PLAYERSTANDARDSPEED * shiftScale, 0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            reimoo.move(0, PLAYERSTANDARDSPEED * shiftScale);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            reimoo.move(PLAYERSTANDARDSPEED * shiftScale, 0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            reimoo.move(0, -PLAYERSTANDARDSPEED * shiftScale);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            reimoo.move(-PLAYERSTANDARDSPEED * shiftScale, 0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            reimoo.move(0, PLAYERSTANDARDSPEED * shiftScale);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            reimoo.move(PLAYERSTANDARDSPEED * shiftScale, 0);
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
        reimoo.display(window);
        window.display();
    }

    return 0;
}