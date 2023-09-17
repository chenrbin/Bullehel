#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Constants.h"
#include "Drawings.h"
#include "Mechanisms.h"
#include "Bullet.h"
#include "GameScreen.h"
#include "Characters.h"

using namespace std;
using namespace Constants;

int main(){
    srand(time(NULL));
    sf::Texture texture;
    if (!texture.loadFromFile("reimoo2.png"))
        return -1;
    sf::ContextSettings windowSettings;
    windowSettings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(1600, 900), "Bullet Hell", sf::Style::Close | sf::Style::Titlebar, windowSettings);
    window.setKeyRepeatEnabled(false);
    window.setFramerateLimit(60);

    Player reimoo(PLAYERSTANDARDSPEED, PLAYERHITBOXRADIUS, texture);
    BorderRects borderRects;
    BulletManager bulletManager;
    GameScreen screen(reimoo, &borderRects, &bulletManager);

    sf::Clock fpsTimer;
    int fpsCounter = 0;

    while (window.isOpen())
    {
        /*
        fpsCounter++;
        if (fpsCounter == 60) {
            print(1 / fpsTimer.restart().asSeconds());
            fpsCounter = 0;
        }*/

        screen.doStuff();
        bulletManager.addCircleBulletR({ 400, 300 }, 2, rand() % 180);
        bulletManager.addCircleBulletR({ 400, 300 }, 2, rand() % 180 - 180);

        sf::Event event;
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
        screen.drawScreen(window);
        window.display();
    }

    return 0;
}