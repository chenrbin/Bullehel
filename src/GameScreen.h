#pragma once
#include <SFML/Graphics.hpp>
#include "Characters.h"
// Class to handle everything on the game screen
class GameScreen : public sf::Drawable {
	sf::FloatRect gameBounds;
	Player* player;
	vector<Enemy*> enemies;
	PatternManager* bulletManager;
	FadeText* hitIndicator; // Delete once I implement death
	SfRectangleAtHome background, border1, border2;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
		target.draw(background, states);
		target.draw(*player, states);
		target.draw(*bulletManager, states);

		// Player is drawn in front of bullets, but hitbox is drawn above bullets
		if (player->getFocused()) 
			target.draw(player->getHitbox(), states);

		for (Enemy* enemy : enemies){
			target.draw(*enemy, states);
		}

		target.draw(border1, states);
		target.draw(border2, states);

	}
public:
	GameScreen(PatternManager* bulletManager, FadeText* hit, sf::Texture& playerTexture, sf::Texture& enemyTexture) {

		// Build game screen rectangles
		background = SfRectangleAtHome(GRAY, { SCREENWIDTH, SCREENHEIGHT }, { SCREENLEFT, SCREENTOP });
		border1 = SfRectangleAtHome(TRANSPARENT, { SCREENWIDTH, SCREENHEIGHT }, { SCREENLEFT, SCREENTOP }, false, WHITE, 1);
		border2 = SfRectangleAtHome(TRANSPARENT, { SCREENWIDTH + 2, SCREENHEIGHT + 2 }, { SCREENLEFT - 1, SCREENTOP - 1 }, false, BLACK, BORDERMARGIN);

		// Make player
		player = new Player(PLAYERSTANDARDSPEED, PLAYERHITBOXRADIUS, playerTexture);
		player->setbounds(background.getGlobalBounds());

		enemies.push_back(new Enemy(ENEMYHITBOXRADIUS, 1000, enemyTexture));

		this->bulletManager = bulletManager;
		hitIndicator = hit;
	}
	~GameScreen(){
		delete player;
		for (Enemy* enemy : enemies){
			delete enemy;
		}
	}
	// Checks every frame
	void update() {
		player->onKeyPress();
		bulletManager->update();
		if (bulletManager->checkPlayerCollision(player->getHitbox()))
			hitIndicator->restart();
	}
};