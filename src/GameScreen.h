#pragma once
#include <SFML/Graphics.hpp>
#include "Characters.h"
// Class to handle everything on the game screen
class GameScreen : public sf::Drawable {
	sf::FloatRect gameBounds;
	Player* reimoo;
	sf::Sprite background;
	BorderRects* borderRects;
	PatternManager* bulletManager;
	FadeText* hitIndicator; // Delete once I implement death

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
		target.draw(borderRects->getBackground(), states);
		target.draw(*reimoo, states);
		target.draw(*bulletManager, states);
		if (reimoo->getFocused()) // Hitbox is drawn above bullets
			target.draw(reimoo->getHitbox(), states);
		target.draw(borderRects->getBorder1(), states);
		target.draw(borderRects->getBorder2(), states);

	}
public:
	GameScreen(Player* player, BorderRects* borderRects, PatternManager* bulletManager, FadeText* hit) {
		reimoo = player;
		this->borderRects = borderRects;
		reimoo->setbounds(borderRects->getBackground().getGlobalBounds());
		this->bulletManager = bulletManager;
		hitIndicator = hit;
	}
	// Checks every frame
	void doStuff() {
		reimoo->checkMovement();
		bulletManager->doStuff();
		if (bulletManager->checkPlayerCollision(reimoo->getHitbox()))
			hitIndicator->restart();
	}
};