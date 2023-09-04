#pragma once
#include <iostream>

using namespace std;
using namespace Constants;
class Player {
	SfCircleAtHome hitbox;
public:
	Player() {
		hitbox = SfCircleAtHome(RED, PLAYERHITBOXRADIUS, { 800, 800 }, true);
	}
	void move(float x = 0, float y = 0) {
		hitbox.move(x, y);
	}
	void display(sf::RenderWindow& window) {
		window.draw(hitbox);
	}
};