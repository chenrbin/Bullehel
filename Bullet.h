#pragma once


// File to contain all bullet implementation
// Game projectiles
class Bullet : public sf::Drawable {
	sf::Shape* hitbox;
	sf::Shape* sprite;
	float xVelocity, yVelocity;
	

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
		target.draw(*sprite, states);
	}
public:
	Bullet(sf::Vector2f position = { SCREENLEFT, SCREENTOP }, float xVelocity = 0, float yVelocity = 0) {
		sprite = new SfCircleAtHome(WHITE, 8, position, true, RED, -2);
		hitbox = new SfCircleAtHome(WHITE, 8, position, true, RED, -2);
		this->xVelocity = xVelocity;
		this->yVelocity = yVelocity;
	}
	~Bullet() {
		//print("Deleted bullet");
	}
	void processMovement() {
		hitbox->move(xVelocity, yVelocity);
		sprite->move(xVelocity, yVelocity);
	}
	void setVelocity(float x, float y) {
		xVelocity = x;
		yVelocity = y;
	}
	void setPosition(sf::Vector2f position) {
		sprite->setPosition(position);
	}
	sf::Vector2f getPosition() {
		return sprite->getPosition();
	}
};

class CircleBullet : public Bullet {
public:
	CircleBullet() {

	}
};

// Manager for all bullet objects. Will be called by main, GameScreen, and others.
class BulletManager : public sf::Drawable {
	vector<Bullet*> bullets;
	sf::FloatRect bulletBounds; // Determines the bound where bullets can exist

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
		for (Bullet* bullet : bullets)
			target.draw(*bullet, states);
	}
public:
	BulletManager() {
		bulletBounds = SCREENBOUNDS;
		bulletBounds.left -= BORDERMARGIN;
		bulletBounds.top -= BORDERMARGIN;
		bulletBounds.width += BORDERMARGIN * 2;
		bulletBounds.height += BORDERMARGIN * 2;
	}
	~BulletManager() {
		for (Bullet* bullet : bullets)
			delete bullet;
	}
	// Checks that will occur every frame
	void doStuff() {
		deleteOutOfBoundsBullets();
		processMovement();
	}
	// Add circle bullet with velocity in rectangulat coordinates
	void addCircleBullet(sf::Vector2f position, float xVelocity = 0, float yVelocity = 0) {
		bullets.push_back(new Bullet(position, xVelocity, yVelocity));
	}
	// Add circle bullet with velocity in polar coordinates
	void addCircleBulletR(sf::Vector2f position, float speed, float angleDegrees) {
		bullets.push_back(new Bullet(position, speed * cos(angleDegrees * 3.14159 / 180), speed * -sin(angleDegrees * 3.14159 / 180)));
	}
	// Call this function every frame to move bullets
	void processMovement() {
		for (Bullet* bullet : bullets)
			bullet->processMovement();
	}
	void deleteOutOfBoundsBullets() {
		for (int i = 0; i < bullets.size(); i++)
			if (!bulletBounds.contains(bullets[i]->getPosition())) {
				Bullet* temp = bullets[i];
				bullets.erase(bullets.begin() + i);
				delete temp;
				i--; // Reiterate current index since vector has shrunk;
			}
	}
	sf::FloatRect& getbulletBounds() {
		return bulletBounds;
	}
};
