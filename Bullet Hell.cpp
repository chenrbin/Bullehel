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
    generalBullets->addLaser({ 400, 200 }, 90, 10, RED);
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
    addTestBullets(generalBullets);

    bulletManager.addPattern(generalBullets);
    bulletManager.addPattern(new Bowap(5, 150, -100, 8, { 400, 400 }, 30, 6));
    bulletManager.addPattern(new QedRipples(80, { 400, 200 }, 0.75, 3));
    bulletManager.addPattern(new FlyingSaucer(36, { 400, 250 }, 0.35, 2, 0));
    bulletManager.addPattern(new GengetsuTime(40, { 400, 200 }, 10, 10));
    bulletManager.deactivateAllPatterns();
    print(bulletManager[0]->getBullets().size());

    sf::CircleShape* cursor = new sf::CircleShape(15.f, 3); // Triangle shaped cursor
    cursor->rotate(90.f);
    vector<string> menuText = { "Test", "BOWAP", "QED", "UFO", "GRT"};
    ClickableMenu danmaku(font, WHITE, menuText, 30, {850, 200}, 30, *cursor);

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
        bool menuClicked = false;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                if (event.key.code >= 26 && event.key.code <= 35) {
                    bulletManager.deactivateAllPatterns();
                    if (event.key.code - 26 < bulletManager.getPatternCount()) {
                        bulletManager[event.key.code - 26]->setActive(true);
                    }   
                }

                if (event.key.code == sf::Keyboard::Space)
                    bulletManager.rotateAllBullets(90);

                break;
            case sf::Event::MouseMoved:
                danmaku.updateMouseMove(event.mouseMove.x, event.mouseMove.y);
                break;
            case sf::Event::MouseButtonPressed:
                if (danmaku.updateMouseClick(event.mouseButton.x, event.mouseButton.y)) {
                    bulletManager.deactivateAllPatterns();
                    bulletManager[danmaku.getCursorPos()]->setActive(true); // Index 0 is generalBullets
                }
            default:
                break;
            }
        }
        window.clear();
        screen.drawScreen(window);
        window.draw(fpsText);
        window.draw(danmaku);
        hitFade.drawAnimation(window);
        window.display();

    }

    return 0;
}
