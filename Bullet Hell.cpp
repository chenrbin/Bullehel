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
    sf::Font font;
    if (!font.loadFromFile("font.ttf"))
        return -1;
    sf::ContextSettings windowSettings;
    windowSettings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(1600, 900), "Bullet Hell", sf::Style::Close | sf::Style::Titlebar, windowSettings);
    window.setKeyRepeatEnabled(false);
    window.setFramerateLimit(FPS);

    SfTextAtHome hitText(font, WHITE, "Pichuun", 40, { 1000, 500 }, true, false, true, true);
    FadeText hitFade(hitText, 0, 1);

    Player reimoo(PLAYERSTANDARDSPEED, PLAYERHITBOXRADIUS, texture);
    BorderRects borderRects;
    BulletManager bulletManager;
    GameScreen screen(reimoo, &borderRects, &bulletManager, &hitFade);

    sfClockAtHome fpsTimer;
    int fpsCounter = 0;
    int num = 0;
   
    bulletManager.addRiceBulletR({ 400, 400 }, 0, 0, BLUE);

    sfClockAtHome bulletTimer;
    int bulletCounter = 0;
    SfTextAtHome fpsText(font, WHITE, "60", 20, FPSTEXTPOS);
    while (window.isOpen())
    {
        // Read fps
        if (fpsTimer.getTimeSeconds() > 1) {
            fpsTimer.restart();
            fpsText.setString(to_string(fpsCounter));
            fpsCounter = 0;
        }
        fpsCounter++;

        bulletCounter++;
        if (bulletCounter % 2 == 0)
        {
            int vel = 150;
            int accel = -100;
            num = 270 + vel * bulletCounter / FPS + accel * pow(bulletCounter / FPS, 2);
            
            int streamCount = 8;
            for (int i = 0; i < streamCount; i++)
                bulletManager.addRiceBulletR({ 400, 400 }, 4, num + i * 360 / streamCount, RED, STANDARDRICEBULLETRADIUS);
        }
       

        screen.doStuff();
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
            case sf::Event::LostFocus:
                break;
            default:
                break;
            }
        }
        window.clear();
        screen.drawScreen(window);
        window.draw(fpsText);
        hitFade.drawAnimation(window);
        window.display();

    }

    return 0;
}