#pragma once

// File to contain all bullet implementation
#pragma region Bullets
// Game projectiles
class Bullet : public sf::Drawable { // Abstract base class
protected:
	sf::Shape* sprite; // Base sprite
	char flag; // Flag code that will be used for various purposes
	float xVelocity, yVelocity;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
		target.draw(*sprite, states);
	}
public:
	Bullet(float xVelocity = 0, float yVelocity = 0) {
		sprite = nullptr;
		this->xVelocity = xVelocity;
		this->yVelocity = yVelocity;
		flag = NEUTRAL;
	}
	~Bullet() {
		delete sprite;
	}
	// Called to move every frame
	virtual void processMovement() {
		sprite->move(xVelocity, yVelocity);
	}
	// Rotate bullet direction
	virtual void rotate(float angleDegrees) {
		float currentSpeed = sqrt(pow(xVelocity, 2) + pow(yVelocity, 2));
		float currentAngle = sprite->getRotation() - 90; // Default orientation is facing down. Setting current angle to have 0 at facing right
		xVelocity = cos((currentAngle + angleDegrees) / 180 * PI) * currentSpeed;
		yVelocity = sin((currentAngle + angleDegrees) / 180 * PI) * currentSpeed;
		sprite->rotate(angleDegrees);
	}
	virtual void setPosition(sf::Vector2f position) {
		sprite->setPosition(position);
	}
	// Flip the x coordinate (reflection along the y axis)
	virtual void flipX() {
		sprite->rotate(-sprite->getRotation() * 2);
		xVelocity *= -1;
	}
	// Flip the y coordinate (reflection along the x axis)
	virtual void flipY() {
		sprite->rotate(-sprite->getRotation() * 2);
		yVelocity *= -1;
	}
	void setVelocity(float x, float y) {
		xVelocity = x;
		yVelocity = y;
	}
	// Set flags used for certain patterns
	void setFlag(char val) {
		flag = val;
	}
	
	char getFlag() {
		return flag;
	}
	sf::Vector2f getPosition() {
		return sprite->getPosition();
	}
	
	// Circular hitboxes will override this. 
	virtual bool checkPlayerCollision(sf::CircleShape& hitbox) = 0;
};

// Basic bullet with circular hitbox (sprite does not have to be circular). Can be a bullet type by itself.
class CircleBullet : virtual public Bullet {
protected:
	int hitBoxRadius; // Used for collision detection;
public:
	CircleBullet(sf::Vector2f position = SCREENPOS, float xVelocity = 0, float yVelocity = 0, sf::Color color = WHITE, int radius = 0) : Bullet(xVelocity, yVelocity) {
		sprite = new SfCircleAtHome(WHITE, radius, position, true, color, STANDARDCIRCLEOUTLINE);
		hitBoxRadius = radius;
	}
	bool checkPlayerCollision(sf::CircleShape& playerHitbox) {
		sf::Vector2f hitBoxPos = playerHitbox.getPosition();
		sf::Vector2f bulletPos = sprite->getPosition();
		return sqrt(pow(hitBoxPos.x - bulletPos.x, 2) + pow(hitBoxPos.y - bulletPos.y, 2)) <= playerHitbox.getRadius() + hitBoxRadius;
	}

};

// Bullets that require multiple shapes to draw
class ComplexBullet : virtual public Bullet {
protected:
	vector<sf::Shape*> extraSprites;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
		target.draw(*sprite, states);
		for(sf::Shape* extraSprite : extraSprites)
			target.draw(*extraSprite, states);
	}
public:
	ComplexBullet() : Bullet(){}
	~ComplexBullet() {
		for (sf::Shape* extraSprite : extraSprites)
			delete extraSprite;
	}
	// Called to move every frame
	virtual void processMovement() {
		sprite->move(xVelocity, yVelocity);
		for (sf::Shape* extraSprite : extraSprites)
			extraSprite->move(xVelocity, yVelocity);
	}
	// Rotate bullet direction
	virtual void rotate(float angleDegrees) {
		float currentSpeed = sqrt(pow(xVelocity, 2) + pow(yVelocity, 2));
		float currentAngle = sprite->getRotation() - 90; // Default orientation is facing down. Setting current angle to have 0 at facing right
		xVelocity = cos((currentAngle + angleDegrees) / 180 * PI) * currentSpeed;
		yVelocity = sin((currentAngle + angleDegrees) / 180 * PI) * currentSpeed;
		sprite->rotate(angleDegrees);
		for (sf::Shape* extraSprite : extraSprites)
			extraSprite->rotate(angleDegrees);
	}
	virtual void setPosition(sf::Vector2f position) {
		sprite->setPosition(position);
		for (sf::Shape* extraSprite : extraSprites)
			extraSprite->setPosition(position);

	}
	// Flip the x coordinate (reflection along the y axis)
	virtual void flipX() {
		sprite->rotate(-sprite->getRotation() * 2);
		for (sf::Shape* extraSprite : extraSprites)
			extraSprite->rotate(-extraSprite->getRotation() * 2);
		xVelocity *= -1;
	}
	// Flip the y coordinate (reflection along the x axis)
	virtual void flipY() {
		sprite->rotate(-sprite->getRotation() * 2);
		for (sf::Shape* extraSprite : extraSprites)
			extraSprite->rotate(-extraSprite->getRotation() * 2);
		yVelocity *= -1;
	}
};

class RiceBullet : public CircleBullet {
public:
	RiceBullet(sf::Vector2f position = SCREENPOS, float xVelocity = 0, float yVelocity = 0, sf::Color color = WHITE, int radius = 0) 
		: CircleBullet(position, xVelocity, yVelocity, color, radius), Bullet(xVelocity, yVelocity) {
		sprite->setOutlineThickness(SMALLCIRCLEOUTLINE);
		if (xVelocity != 0)
			sprite->rotate(atan(yVelocity / xVelocity) * 180 / PI + 90); // Rotate based on shot angle
		sprite->setScale(1, 2);
	}
};
class DotBullet : public CircleBullet {
public:
	DotBullet(sf::Vector2f position = SCREENPOS, float xVelocity = 0, float yVelocity = 0, sf::Color color = WHITE, int radius = 0) 
		: CircleBullet(position, xVelocity, yVelocity, color, radius), Bullet(xVelocity, yVelocity) {
		sprite->setOutlineThickness(SMALLCIRCLEOUTLINE);
	}
};
class TalismanBullet : public CircleBullet {
public:
	TalismanBullet(sf::Vector2f position = SCREENPOS, float xVelocity = 0, float yVelocity = 0, sf::Color color = WHITE, int radius = 0) 
		: CircleBullet(position, xVelocity, yVelocity, color, radius), Bullet(xVelocity, yVelocity) {
		delete sprite;
		sprite = new SfRectangleAtHome(WHITE, { 3.f * radius, 4.f * radius }, position, true, MAGENTA, STANDARDCIRCLEOUTLINE);
		if (xVelocity != 0)
			sprite->rotate(atan(yVelocity / xVelocity) * 180 / PI + 90);
	}
};
class BubbleBullet : public ComplexBullet, public CircleBullet {
public:
	BubbleBullet(sf::Vector2f position = SCREENPOS, float xVelocity = 0, float yVelocity = 0, sf::Color color = WHITE, int radius = 0)
		: CircleBullet(position, xVelocity, yVelocity, color, radius), Bullet(xVelocity, yVelocity) {
		// Four concentric circles with colors: transparent, color darkened and partially transparent, original color, white
		sprite->setFillColor(TRANSPARENT);
		sprite->setOutlineThickness(radius * 0.6);
		sf::Color copyColor = color;
		color.a = 200;
		color.r *= 0.9;
		color.g *= 0.9;
		color.b *= 0.9;
		sprite->setOutlineColor(color);
		extraSprites.push_back(new SfCircleAtHome(TRANSPARENT, radius * 1.3, position, true, copyColor, radius * 0.6));
		extraSprites.push_back(new SfCircleAtHome(TRANSPARENT, radius * 1.9, position, true, TRANSPARENTWHITE, radius * 0.75));
	}
};
#pragma endregion

// Class to store bullet pattern templates. The base class is for random bullets and children have specific patterns.
class Pattern : public sf::Drawable {
protected:
	vector<Bullet*> bullets;
	// Timing
	float duration; // How long the pattern is active in seconds. Does not deactivate if zero
	int frameCounter; // Used as a timer
	bool active;

	// Bullet properties
	int streamCount;
	float baseSpeed;
	sf::Vector2f sourcePos;
	// Amount of shots per second, up to the framerate. FPS / shotFrequency = frames per shot
	// Shoots only once if set to zero
	float shotFrequency;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
		for (Bullet* bullet : bullets)
			target.draw(*bullet, states);
	}
public:
	Pattern(float duration = 0, int streamCount = 0, float shotFrequency = 0, float baseSpeed = 0, sf::Vector2f sourcePos = SCREENPOS) {
		frameCounter = 0;
		active = true;
		this->duration = duration;
		this->streamCount = streamCount;
		this->shotFrequency = shotFrequency;
		this->baseSpeed = baseSpeed;
		this->sourcePos = sourcePos;
		if (shotFrequency > FPS)
			shotFrequency = FPS;
	}
	~Pattern() {
		for (Bullet* bullet : bullets)
			delete bullet;
	}
	virtual void processMovement() {
		for (Bullet* bullet : bullets)
			bullet->processMovement();
	}
	// Increment frame counter
	void incrementFrame() {
		if(active)
			frameCounter++;
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

#pragma region Add bullets
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
	// Add bubble bullet with velocity in polar coordinates
	void addBubbleBulletR(sf::Vector2f position, float speed = 0, float angleDegrees = 0, sf::Color color = DEFAULTBUBBLEBULLETCOLOR, int radius = STANDARDBUBBLEBULLETRADIUS) {
		bullets.push_back(new BubbleBullet(position, speed * cos(angleDegrees * 3.14159f / 180), speed * -sin(angleDegrees * 3.14159f / 180), color, radius));
	}

#pragma endregion

};

// Direct stream with accelerating angle velocity
class Bowap : public Pattern {
	float offset;
	float angleVelocity;
	float angleAcceleration;
public:
	Bowap(float offset, float angleVelocity, float angleAcceleration, float duration, int streamCount, float shotFrequency, float baseSpeed, sf::Vector2f sourcePos) : Pattern(duration, streamCount, shotFrequency, baseSpeed, sourcePos) {
		this->offset = offset;
		this->angleVelocity = angleVelocity;
		this->angleAcceleration = angleAcceleration;
	}
	void spawnBullets() {
		if (!active)
			return;
		if (isGoodToshoot()) {
			int num = offset + angleVelocity * frameCounter / FPS + angleAcceleration * pow(frameCounter / FPS, 2);
			for (int i = 0; i < streamCount; i++)
				addRiceBulletR(sourcePos, baseSpeed, num + i * 360 / streamCount);
		}
	}

};

// Ring of bullets, bounces off top left right walls once.
class QedRipples : public Pattern{ // Todo: speed up phases
	sf::FloatRect bounds;
public:
	QedRipples(float duration, int streamCount, sf::Vector2f centerPos, float shotFrequency, float baseSpeed, sf::FloatRect bounds = SCREENBOUNDS) : Pattern(duration, streamCount, shotFrequency, baseSpeed, centerPos) {
		this->bounds = bounds;
	}
	void processMovement() {
		for (Bullet* bullet : bullets) {
			bullet->processMovement();
			sf::Vector2f pos = bullet->getPosition();
			if (!bounds.contains(pos)) {
				if (pos.y > bounds.top + bounds.height)
					continue;
				if (bullet->getFlag() != BOUNCED && (pos.x < bounds.left || pos.x > bounds.left + bounds.width)) {
					bullet->flipX();
					bullet->setFlag(BOUNCED);
				}
				else if (pos.y < bounds.top) {
					bullet->flipY();
					bullet->setFlag(BOUNCED);
				}
			}
		}
	}
	void spawnBullets() {
		if (!active)
			return;
		if (isGoodToshoot()) {
			// Random angle and position
			int shotAngle = rand() % 360;
			sf::Vector2f shotSource;
			if (frameCounter != 0) 
				shotSource = { sourcePos.x + rand() % 400 - 200, sourcePos.y + rand() % 200 - 100 };
			else // Fixed position for first shot
				shotSource = sourcePos;
			for (int i = 0; i < streamCount; i++)
				addRiceBulletR(shotSource, baseSpeed, shotAngle + i * 360 / streamCount, BLUE);
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
			pattern->processMovement();
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



