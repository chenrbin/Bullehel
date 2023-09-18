#pragma once

struct BulletSprite : public sf::Drawable, public sf::Transformable{};

// File to contain all bullet implementation
// Game projectiles
class Bullet : public sf::Drawable {
protected:
	sf::Shape* hitbox; // Hitbox will be a circle or a rectangle. Rectangles will use getGlobalBounds().intersects(). Circles will store a separate radius variable
	sf::Shape* sprite; // Base sprite

	float xVelocity, yVelocity;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
		target.draw(*sprite, states);
	}
public:
	Bullet(float xVelocity = 0, float yVelocity = 0) {
		sprite = nullptr;
		hitbox = nullptr;
		this->xVelocity = xVelocity;
		this->yVelocity = yVelocity;
	}
	~Bullet() {
		delete hitbox;
		delete sprite;
	}
	void processMovement() {
		hitbox->move(xVelocity, yVelocity);
		sprite->move(xVelocity, yVelocity);
	}
	void rotate(float angleDegrees) {
		float currentSpeed = sqrt(pow(xVelocity, 2) + pow(yVelocity, 2));
		float currentAngle = atan(yVelocity / xVelocity); // atan covers 90 degrees
		if (xVelocity < 0)
			currentAngle += PI;
		float newAngle = angleDegrees * PI / 180 + currentAngle;
		xVelocity = cos(newAngle) * currentSpeed;
		yVelocity = sin(newAngle) * currentSpeed;
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

// Basic bullet with circular hitbox (sprite does not have to be circular). Can be a bullet type by itself.
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
		return sqrt(pow(hitBoxPos.x - bulletPos.x, 2) + pow(hitBoxPos.y - bulletPos.y, 2)) <= playerHitbox.getRadius() + hitBoxRadius;
	}

};

class RiceBullet : public CircleBullet {
public:
	RiceBullet(sf::Vector2f position = SCREENPOS, float xVelocity = 0, float yVelocity = 0, sf::Color color = WHITE, int radius = 0) : CircleBullet(position, xVelocity, yVelocity, color, radius) {
		sprite->setOutlineThickness(SMALLCIRCLEOUTLINE);
		sprite->setScale(1, 2);
	}
};
class DotBullet : public CircleBullet {
public:
	DotBullet(sf::Vector2f position = SCREENPOS, float xVelocity = 0, float yVelocity = 0, sf::Color color = WHITE, int radius = 0) : CircleBullet(position, xVelocity, yVelocity, color, radius) {
		sprite->setOutlineThickness(STANDARDCIRCLEOUTLINE);
	}
};
class TalismanBullet : public CircleBullet {
public:
	TalismanBullet(sf::Vector2f position = SCREENPOS, float xVelocity = 0, float yVelocity = 0, sf::Color color = WHITE, int radius = 0) : CircleBullet(position, xVelocity, yVelocity, color, radius) {
		sprite = new SfRectangleAtHome(WHITE, { 3.f * radius, 4.f * radius }, position, true, MAGENTA, STANDARDCIRCLEOUTLINE);
	}
};
// Class to store bullet pattern templates. The base class is for random bullets and children have specific patterns.
class Pattern : public sf::Drawable {
protected:
	vector<Bullet*> bullets;
	// When to shoot
	float duration; // In seconds
	int frameCounter; // Used as a timer
	bool active;

	// How much to shoot
	float streamCount;
	sf::Vector2f sourcePos;
	// Amount of shots per second, up to the framerate. FPS / shotFrequency = frames per shot
	// Shoots only once if set to zero
	float shotFrequency;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
		for (Bullet* bullet : bullets)
			target.draw(*bullet, states);
	}
public:
	Pattern() {
		duration = 0;
		frameCounter = 0;
		shotFrequency = 0;
		streamCount = 0;
		sourcePos = SCREENPOS;
		shotFrequency = 0;
		active = true;
	}
	~Pattern() {
		for (Bullet* bullet : bullets)
			delete bullet;
	}
	// Increment frame counter
	void incrementFrame() {
		frameCounter++;
	}
	// All addBullet functions return a pointer to the bullet create
	// Add circle bullet with velocity in rectangular coordinates
	void addCircleBullet(sf::Vector2f position, sf::Vector2f velocity, sf::Color color = DEFAULTCIRCLEBULLETCOLOR, int radius = STANDARDCIRCLEBULLETRADIUS) {
		bullets.push_back(new CircleBullet(position, velocity.x, velocity.y, color, radius));
	}
	// Add circle bullet with velocity in polar coordinates
	void addCircleBulletR(sf::Vector2f position, float speed = 0, float angleDegrees = 0, sf::Color color = DEFAULTCIRCLEBULLETCOLOR, int radius = STANDARDCIRCLEBULLETRADIUS) {
		bullets.push_back(new CircleBullet(position, speed * cos(angleDegrees * 3.14159f / 180), speed * -sin(angleDegrees * 3.14159f / 180), color, radius));
	}
	// Add rice bullet with velocity in polar coordinates
	void addRiceBulletR(sf::Vector2f position, float speed = 0, float angleDegrees = 0, sf::Color color = DEFAULTRICEBULLETCOLOR, int radius = STANDARDRICEBULLETRADIUS) {
		bullets.push_back(new RiceBullet(position, speed * cos(angleDegrees * 3.14159f / 180), speed * -sin(angleDegrees * 3.14159f / 180), color, radius));
	}
	// Add rice bullet with velocity in polar coordinates
	void addDotBulletR(sf::Vector2f position, float speed = 0, float angleDegrees = 0, sf::Color color = DEFAULTDOTBULLETCOLOR, int radius = STANDARDDOTBULLETRADIUS) {
		bullets.push_back(new DotBullet(position, speed * cos(angleDegrees * 3.14159f / 180), speed * -sin(angleDegrees * 3.14159f / 180), color, radius));
	}
	// Add talisman bullet with velocity in polar coordinates
	void addTalismanBulletR(sf::Vector2f position, float speed = 0, float angleDegrees = 0, sf::Color color = DEFAULTDOTBULLETCOLOR, int radius = STANDARDDOTBULLETRADIUS) {
		bullets.push_back(new TalismanBullet(position, speed * cos(angleDegrees * 3.14159f / 180), speed * -sin(angleDegrees * 3.14159f / 180), color, radius));
	}
	vector<Bullet*>& getBullets() {
		return bullets;
	}
	void setActive(bool val) {
		active = val;
	}
	// Not used by base class
	virtual void spawnBullets() {
		return;
	}
	// Compares shotFrequency with frameCounter and is used by spawnBullets to determine when to shoot.
	bool isGoodToshoot() {
		int framesPerShot;
		if (shotFrequency == 0) {
			framesPerShot = 1;
			active = false;
		}
		else
			framesPerShot = FPS / shotFrequency;
		return frameCounter % framesPerShot == 0;
	}
	// Reset frame counter
	void resetCounter() {
		frameCounter = 0;
	}
};

class Bowap : public Pattern {
	float offset;
	float angleVelocity;
	float angleAcceleration;
public:
	Bowap(float offset, float angleVelocity, float angleAcceleration, float streamCount, float duration, sf::Vector2f sourcePos, float shotFrequency) {
		this->offset = offset;
		this->angleVelocity = angleVelocity;
		this->angleAcceleration = angleAcceleration;
		this->streamCount = streamCount;
		this->duration = duration;
		this->sourcePos = sourcePos;
		if (shotFrequency > FPS)
			shotFrequency = FPS;
		this->shotFrequency = shotFrequency;
	}
	void spawnBullets() {
		if (!active)
			return;
		if (isGoodToshoot()) {
			int num = offset + angleVelocity * frameCounter / FPS + angleAcceleration * pow(frameCounter / FPS, 2);
			for (int i = 0; i < streamCount; i++)
				addRiceBulletR(sourcePos, 6, num + i * 360 / streamCount);
		}
	}

};
// Manager for all bullet objects. Will be called by main, GameScreen, and others.
class BulletManager : public sf::Drawable {
	vector<Pattern*> activePatterns;
	sf::FloatRect bulletBounds; // Determines the bound where bullets can exist

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
		for (Pattern* pattern :activePatterns)
			target.draw(*pattern, states);
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
		for (Pattern* pattern : activePatterns)
			delete pattern;
	}
	// Checks that will occur every frame
	void doStuff() {
		deleteOutOfBoundsBullets();
		spawnPatterns();
		processMovement();
	}
	// Adds a pattern to the manager. Patterns can either spawn bullets from an algorithm or function call.
	void addPattern(Pattern* pattern) {
		activePatterns.push_back(pattern);
	}
	void spawnPatterns() {
		for (Pattern* pattern : activePatterns)
			pattern->spawnBullets();
	}
	
	// Call this function every frame to move bullets
	void processMovement() {
		for (Pattern* pattern : activePatterns) {
			pattern->incrementFrame();
			for (Bullet* bullet : pattern->getBullets())
				bullet->processMovement();
		}
	}
	void rotateAllBullets(float angleDegrees) {
		for (Pattern* pattern : activePatterns)
			for (Bullet* bullet : pattern->getBullets()) 
				bullet->rotate(angleDegrees);
	}
	// Delete any bullets that are out of bounds
	void deleteOutOfBoundsBullets() {
		for (Pattern* pattern : activePatterns) {
			vector<Bullet*>& bullets = pattern->getBullets();
			for (int i = 0; i < bullets.size(); i++) 
				if (!bulletBounds.contains(bullets[i]->getPosition())) {
					Bullet* temp = bullets[i];
					bullets.erase(bullets.begin() + i);
					delete temp;
					i--; // Reiterate current index since vector has shrunk;
				}
		}
	}
	sf::FloatRect& getbulletBounds() {
		return bulletBounds;
	}
	// Check if player hitbox has collided with any bullets
	bool checkPlayerCollision(sf::CircleShape& hitbox) {
		for (Pattern* pattern : activePatterns)
			for (Bullet* bullet : pattern->getBullets())
				if (bullet->checkPlayerCollision(hitbox))
					return true;
		return false;
	}
};



