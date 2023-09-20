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

// Add a list of static bullets to the general pattern
void addTestBullets(Pattern* generalBullets) {
    generalBullets->addCircleBulletR({ 350, 400 }, 0, 0);
    generalBullets->addRiceBulletR({ 400, 400 }, 0, 0);
    generalBullets->addDotBulletR({ 450, 400 }, 0, 0);
    generalBullets->addTalismanBulletR({ 500, 400 }, 0, 0);
    generalBullets->addBubbleBulletR({ 550, 400 }, 0, 0);
}
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
    Pattern* generalBullets = new Pattern();
    bulletManager.addPattern(generalBullets);

    addTestBullets(generalBullets);

    Bowap* bowap = new Bowap(5, 150, -100, 0, 8, 10, 6, {400, 400});
    bulletManager.addPattern(bowap);
    bowap->setActive(false);
    QedRipples* qed = new QedRipples(0, 80, { 400, 200 }, 0.75, 3);
    bulletManager.addPattern(qed);
    //qed->setActive(false);


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
                if (event.key.code == sf::Keyboard::Space)
                    bulletManager.rotateAllBullets(90);
                if (event.key.code == sf::Keyboard::Z)
                    bowap->resetCounter();
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
