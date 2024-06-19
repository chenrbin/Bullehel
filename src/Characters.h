#pragma once
#include <iostream>
#include <algorithm>
using namespace std;
using namespace Constants;
class Player {
	SfCircleAtHome hitbox; // Actual hitbox is invisible and slightly smaller.
	sf::Sprite playerSprite;
	sf::FloatRect movementBounds;
	float moveSpeed, hitboxRadius;
	bool focused;

public:
	Player() {
		moveSpeed = 0;
		hitboxRadius = 0;
		focused = false;
	};
	Player(float moveSpeed, float hitboxRadius, sf::Texture& texture) {
		this->moveSpeed = moveSpeed;
		this->hitboxRadius = hitboxRadius;
		hitbox = SfCircleAtHome(WHITE, hitboxRadius, { 800, 800 }, true, RED, STANDARDCIRCLEOUTLINE);
		playerSprite.setTexture(texture);
		playerSprite.setScale(0.6, 0.6);
		sf::FloatRect bound = playerSprite.getLocalBounds();
		playerSprite.setOrigin(bound.width / 2, bound.height / 2);
	}
	void move(sf::Vector2f vec) {
		hitbox.move(vec.x, vec.y);
		// Bound check
		if (getPosition().x < movementBounds.left)
			hitbox.setPosition(movementBounds.left, getPosition().y);
		else if (getPosition().x > movementBounds.left + movementBounds.width)
			hitbox.setPosition(movementBounds.left + movementBounds.width, getPosition().y);
		if (getPosition().y < movementBounds.top)
			hitbox.setPosition(getPosition().x, movementBounds.top);
		else if (getPosition().y > movementBounds.top + movementBounds.height)
			hitbox.setPosition(getPosition().x, movementBounds.top + movementBounds.height);
		playerSprite.setPosition(hitbox.getPosition());
	}
	// Check where to move based on current keyboard presses
	void checkMovement(){
		sf::Vector2f nextMove(0, 0);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			nextMove.x -= moveSpeed;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			nextMove.x += moveSpeed;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			nextMove.y += moveSpeed;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			nextMove.y -= moveSpeed;
		if (nextMove.x != 0 && nextMove.y != 0)
			nextMove *= float(sqrt(2) / 2);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
			focused = true;
			nextMove *= FOCUSSPEEDMODIFIER;
		}
		else focused = false;
		if (nextMove != sf::Vector2f{0, 0})
			move(nextMove);
	}
	void drawCharacter(sf::RenderWindow& window) {
		window.draw(playerSprite);
	}
	sf::Vector2f getPosition() {
		return hitbox.getPosition();
	}
	void setbounds(sf::FloatRect bounds){
		movementBounds = bounds;
		movementBounds.height -= hitboxRadius * 2, movementBounds.width -= hitboxRadius * 2;
		movementBounds.top += hitboxRadius, movementBounds.left += hitboxRadius;
		// If hitbox is outside of bounds when setting, move in bound.
		if (!bounds.contains(getPosition())) {
			hitbox.setPosition(movementBounds.left + movementBounds.width * 0.5f, movementBounds.top + movementBounds.height * 0.8f);
			playerSprite.setPosition(hitbox.getPosition());
		}
	}
	SfCircleAtHome& getHitbox() {
		return hitbox;
	}
	bool getFocused() {
		return focused;
	}
};