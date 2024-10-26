#pragma once
#include <iostream>
#include <algorithm>
using namespace std;
using namespace Constants;
class Player : public sf::Drawable {
	SfCircleAtHome hitbox; // Actual hitbox is invisible and slightly smaller.
	sf::Sprite playerSprite;
	sf::FloatRect movementBounds; // Boundary limits for the player's movement
	float moveSpeed, hitboxRadius;
	bool focused;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
		target.draw(playerSprite, states);
	}
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
	// Process key controls, such as movement and shooting
	void onKeyPress(){
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
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
			shoot();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
			focused = true;
			nextMove *= FOCUSSPEEDMODIFIER;
		}
		else focused = false;
		if (nextMove != sf::Vector2f{0, 0})
			move(nextMove);
	}
	void shoot(){
		cout << "pew\n";
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

class Enemy : public sf::Drawable {
	sf::Sprite sprite;
	sf::FloatRect movementBounds;
	float hitboxRadius;
	float hitpoint;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
		target.draw(sprite, states);
	}
public:
	Enemy() {
		hitboxRadius = 0;
		hitpoint = 0;
	};
	Enemy(float hitboxRadius, float hitpoint, sf::Texture& texture) {
		this->hitboxRadius = hitboxRadius;
		this->hitpoint = hitpoint;
		sprite.setTexture(texture);
		sprite.setScale(0.6, 0.6);
		sprite.setPosition(400 , 100);
		sf::FloatRect bound = sprite.getLocalBounds();
		sprite.setOrigin(bound.width / 2, bound.height / 2);
	}
};