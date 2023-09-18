#pragma once

struct BulletSprite : public sf::Drawable, public sf::Transformable{};

// File to contain all bullet implementation
// Game projectiles
struct Bullet : public sf::Drawable {
	sf::Shape* hitbox; // Hitbox will be a circle or a rectangle. Rectangles will use getGlobalBounds().intersects(). Circles will store a separate radius variable
	sf::Shape* sprite; // Base sprite
	float xVelocity, yVelocity;
	
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
		target.draw(*sprite, states);
	}
	Bullet(float xVelocity = 0, float yVelocity = 0) {
		sprite = nullptr;
		hitbox = nullptr;
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
		hitbox->setPosition(position);
	}
	sf::Vector2f getPosition() {
		return sprite->getPosition();
	}
	sf::Shape* getHitbox() {
		return hitbox;
	}
	// Circular hitboxes will override this
	virtual bool checkPlayerCollision(sf::CircleShape& hitbox) {
		print(1);
		// Fix this. Recognize player hitbox as circle
		return (hitbox.getGlobalBounds().intersects(hitbox.getGlobalBounds()));
	}
};

// Basic circular bullet with circular hitbox
class CircleBullet : public Bullet {
protected:
	int hitBoxRadius; // Used for collision detection;
public:
	CircleBullet(sf::Vector2f position = SCREENPOS, float xVelocity = 0, float yVelocity = 0, sf::Color color = WHITE, int radius = 0) : Bullet(xVelocity, yVelocity) {
		sprite = new SfCircleAtHome(WHITE, radius, position, true, color, STANDARDCIRCLEOUTLINE);
		hitBoxRadius = radius;
		SfCircleAtHome* circle = new SfCircleAtHome();
		circle->setRadius(radius);
		circle->alignCenter();
		circle->setPosition(position);
		hitbox = circle;
	}
	bool checkPlayerCollision(sf::CircleShape& playerHitbox) {
		sf::Vector2f hitBoxPos = playerHitbox.getPosition();
		sf::Vector2f bulletPos = sprite->getPosition();
		return sqrt(pow(hitBoxPos.x - bulletPos.x, 2) + pow(hitBoxPos.y - bulletPos.y, 2)) <= playerHitbox.getRadius() + hitBoxRadius - playerHitbox.getOutlineThickness();
	}

};

class RiceBullet : public CircleBullet {
public:
	RiceBullet(sf::Vector2f position = SCREENPOS, float xVelocity = 0, float yVelocity = 0, sf::Color color = WHITE, int radius = 0) : CircleBullet(position, xVelocity, yVelocity, color, radius) {
		sprite->setOutlineThickness(SMALLCIRCLEOUTLINE);
		sprite->setScale(0.8, 2);
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
	void addCircleBullet(sf::Vector2f position, sf::Vector2f velocity, sf::Color color = RED, int radius = STANDARDCIRCLEBULLETRADIUS) {
		bullets.push_back(new CircleBullet(position, velocity.x, velocity.y, color, radius));
	}
	// Add circle bullet with velocity in polar coordinates
	void addCircleBulletR(sf::Vector2f position, float speed = 0, float angleDegrees = 0, sf::Color color = RED, int radius = STANDARDCIRCLEBULLETRADIUS) {
		bullets.push_back(new CircleBullet(position, speed * cos(angleDegrees * 3.14159f / 180), speed * -sin(angleDegrees * 3.14159f / 180), color, radius));
	}
	// Add rice bullet with velocity in polar coordinates
	void addRiceBulletR(sf::Vector2f position, float speed = 0, float angleDegrees = 0, sf::Color color = RED, int radius = STANDARDRICEBULLETRADIUS) {
		bullets.push_back(new RiceBullet(position, speed * cos(angleDegrees * 3.14159f / 180), speed * -sin(angleDegrees * 3.14159f / 180), color, radius));
	}
	// Add rice bullet with velocity in polar coordinates
	void addDotBulletR(sf::Vector2f position, float speed = 0, float angleDegrees = 0, sf::Color color = RED, int radius = STANDARDDOTBULLETRADIUS) {
		bullets.push_back(new RiceBullet(position, speed * cos(angleDegrees * 3.14159f / 180), speed * -sin(angleDegrees * 3.14159f / 180), color, radius));
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
	// Check if player hitbox has collided with any bullets
	bool checkPlayerCollision(sf::CircleShape& hitbox) {
		for (Bullet* bullet : bullets) {
			if (bullet->checkPlayerCollision(hitbox)) {
				return true;
			}
		}
		return false;
	}
};
