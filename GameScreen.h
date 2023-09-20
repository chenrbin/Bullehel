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
	FadeText* hitIndicator; // Delete once I implement death
public:
	GameScreen(Player player, BorderRects* borderRects, BulletManager* bulletManager, FadeText* hit) {
		reimoo = player;
		this->borderRects = borderRects;
		reimoo.setbounds(borderRects->getBackground().getGlobalBounds());
		this->bulletManager = bulletManager;
		hitIndicator = hit;
	}
	// Checks every frame
	void doStuff() {
		reimoo.checkMovement();
		bulletManager->doStuff();
		if (bulletManager->checkPlayerCollision(reimoo.getHitbox()))
			hitIndicator->restart();
	}
	void drawScreen(sf::RenderWindow& window) {
		window.draw(borderRects->getBackground());
		reimoo.drawCharacter(window);
		window.draw(*bulletManager);
		if (reimoo.getFocused()) // Hitbox is drawn above bullets
			window.draw(reimoo.getHitbox());
;		window.draw(borderRects->getBorder1());
		window.draw(borderRects->getBorder2());
	}
};