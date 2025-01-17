// Ruobin Chen
// Made with sfml 2.5.1
// Game inspired by Touhou Project
// Total line count as of 12/26/2023 : 2118
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <numeric>
#include <cmath>
#include "Constants.h"
#include "Drawings.h"
#include "Mechanisms.h"
#include "Bullet.h"
#include "Pattern.h"
#include "GameScreen.h"
#include "Characters.h"
using namespace std;
using namespace Constants;
// Add a list of static bullets to the general pattern
void addTestBullets(Pattern* generalBullets) {
    generalBullets->addCircleBullet({ 350, 400 }, 0, 0);
    generalBullets->addRiceBullet({ 400, 400 }, 0, 90);
    generalBullets->addDotBullet({ 450, 400 }, 0, 0);
    generalBullets->addTalismanBullet({ 500, 400 }, 0, 90);
    generalBullets->addBubbleBullet({ 550, 400 }, 0, 0);
    generalBullets->addLaser({ 400, 200 }, 0, 10, 20, 0.25, 99, BLUE);
    generalBullets->addArrowheadBullet({600, 400}, 0, 90);
    generalBullets->addSpawner({ 300, 400 }, 0, 0, true);
}
int main(){
    srand(time(NULL));
    // Load sprite textures
    sf::Texture playerTexture;
    if (!playerTexture.loadFromFile(PLAYERTEXTUREFILEPATH)) {
        cout << "Failed to load player texture file\n";
        return -1;
    }
    sf::Texture enemyTexture;
    if (!enemyTexture.loadFromFile(ENEMYTEXTUREFILEPATH)) {
        cout << "Failed to load enemy texture file\n";
        return -1;
    }
    sf::Font font;
    if (!font.loadFromFile(FONTFILEPATH)) {
        cout << "Failed to load font file\n";
        return -1;
    }
    sf::ContextSettings windowSettings;
    windowSettings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(1600, 900), "ShootEmUp", sf::Style::Close | sf::Style::Titlebar, windowSettings);
    window.setKeyRepeatEnabled(false);
    window.setFramerateLimit(FPS);

    SfTextAtHome hitText(font, WHITE, "Pichuun", 40, { 1000, 500 }, true, false, true, true);
    FadeText hitFade(hitText, 0, 1);

    PatternManager manager;
    GameScreen gameScreen(&manager, &hitFade, playerTexture, enemyTexture);

    sfClockAtHome fpsTimer;
    int fpsCounter = 0;
    Pattern* generalBullets = new Pattern();
    addTestBullets(generalBullets);

    manager.addPattern(generalBullets);
    manager.addPattern(new Bowap({ 400, 400 }, 8, 30, 6));
    manager.addPattern(new QedRipples({ 400, 200 }, 80, 0.75, 3));
    manager.addPattern(new FlyingSaucer({ 400, 250 }, 40, 0.35, 2));
    manager.addPattern(new GengetsuTime({ 400, 200 }, 48, 10, 10));
    manager.addPattern(new WindGod({ 400, 300 }, 0.3, 4));
    manager.addPattern(new MercuryPoison({ 400, 200 }, 32, 3, 2.5));
    manager.addPattern(new SeamlessCeiling({ 400, 200 }, 4, 2, 3));
    manager.deactivateAllPatterns();

    sf::CircleShape* cursor = new sf::CircleShape(15.f, 3); // Triangle shaped cursor
    cursor->rotate(90.f);
    vector<string> menuText = { "Test", "BOWAP", "QED", "UFO", "GRT", "MOF", "HGP", "SCOKJ"};
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
        gameScreen.update();
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
                    manager.deactivateAllPatterns();
                    if (event.key.code - 26 < manager.getPatternCount()) {
                        manager[event.key.code - 26]->setActive(true);
                    }   
                }

                else if (event.key.code == sf::Keyboard::Space) 
                    manager.rotateAllBullets(30);
                else if (event.key.code == sf::Keyboard::Z) {
                    break;
                }

                break;
            case sf::Event::MouseMoved:
                danmaku.onMouseMove(event.mouseMove.x, event.mouseMove.y);
                break;
            case sf::Event::MouseButtonPressed:
                if (danmaku.onMouseClick(event.mouseButton.x, event.mouseButton.y)) {
                    manager.deactivateAllPatterns();
                    manager[danmaku.getCursorPos()]->setActive(true); // Index 0 is generalBullets
                }
                break;
            default:
                break;
            }
        }
        window.clear();
        window.draw(gameScreen);
        window.draw(fpsText);
        window.draw(danmaku);
        hitFade.drawAnimation(window);
        window.display();

    }

    return 0;
}
