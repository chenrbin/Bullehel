#pragma once
#include <SFML/Graphics.hpp>
#include "Characters.h"
// Class to handle everything on the game screen
class GameScreen {
	sf::FloatRect gameBounds;
	Player reimoo;
	sf::Sprite background;
	BorderRects* borderRects;
	BulletManager* bulletManager;
public:
	GameScreen(Player player, BorderRects* borderRects, BulletManager* bulletManager) {
		reimoo = player;
		this->borderRects = borderRects;
		reimoo.setbounds(borderRects->getBackground().getGlobalBounds());
		this->bulletManager = bulletManager;
	}
	// Checks every frame
	void doStuff() {
		reimoo.checkMovement();
		bulletManager->doStuff();
	}
	void drawScreen(sf::RenderWindow& window) {
		window.draw(borderRects->getBackground());
		reimoo.drawCharacter(window);
		window.draw(*bulletManager);
		window.draw(borderRects->getBorder1());
		window.draw(borderRects->getBorder2());
	}
};