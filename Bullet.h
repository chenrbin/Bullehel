#pragma once

// File to contain all bullet implementation

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
	virtual void rotateBullet(float angleDegrees) {
		if (yVelocity == 0 && xVelocity == 0) {
			sprite->rotate(angleDegrees);
			return;
		}
		float currentSpeed = sqrt(pow(xVelocity, 2) + pow(yVelocity, 2));
		float currentAngle = atan(yVelocity / xVelocity) * 180 / PI; 
		if (xVelocity < 0)
			currentAngle += 180;
		xVelocity = cos((currentAngle + angleDegrees) * PI / 180) * currentSpeed;
		yVelocity = sin((currentAngle + angleDegrees) * PI / 180) * currentSpeed;
		sprite->rotate(angleDegrees);
	}
	// Adds an offset to position instead of setting it
	virtual void adjustPosition(float x, float y) {
		sprite->move(x, y);
	}
	// Adds an offset to velocity
	virtual void adjustVelocity(float x, float y) {
		xVelocity += x;
		yVelocity += y;
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
	// Align sprite rotation based on velocity. Optionally add an offset
	void alignAngle() {
		float angle;
		if (yVelocity != 0 || xVelocity != 0)
			angle = atan(yVelocity / xVelocity) * 180 / PI + 90; // C++ handles atan(y/0), but not atan(0/0)
		else
			angle = 0;
		if (xVelocity < 0)
			angle += 180;
		sprite->setRotation(angle);
	}
	// Align sprite rotation based on velocity. Optionally add an offset
	void alignAngle(float xOffset, float yOffset) {
		float angle;
		if (yVelocity + yOffset != 0 || xVelocity + xOffset != 0)
			angle = atan((yVelocity + yOffset) / (xVelocity + xOffset)) * 180 / PI + 90; // C++ handles atan(y/0), but not atan(0/0)
		else
			angle = 0;
		if (xVelocity + xOffset < 0)
			angle += 180;
		sprite->setRotation(angle);
	}
	void setVelocity(float x, float y) {
		xVelocity = x;
		yVelocity = y;
	}
	void scaleVelocity(float x, float y) {
		xVelocity *= x;
		yVelocity *= y;
	}
	sf::Vector2f getVelocity() {
		return sf::Vector2f(xVelocity, yVelocity);
	}
	// Some bullet types with specific variables will use this
	virtual void resetBullet() {
		return;
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
	
	// Actual bullet types will override this. 
	virtual bool checkPlayerCollision(sf::CircleShape& hitbox) = 0;
};
#pragma region Bullet types
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
	virtual void rotateBullet(float angleDegrees) {
		float currentSpeed = sqrt(pow(xVelocity, 2) + pow(yVelocity, 2));
		float currentAngle = sprite->getRotation() - 90; // Default orientation is facing down. Setting current angle to have 0 at facing right
		xVelocity = cos((currentAngle + angleDegrees) / 180 * PI) * currentSpeed;
		yVelocity = sin((currentAngle + angleDegrees) / 180 * PI) * currentSpeed;
		sprite->rotate(angleDegrees);
		for (sf::Shape* extraSprite : extraSprites)
			extraSprite->rotate(angleDegrees);
	}
	void setPosition(sf::Vector2f position) {
		sprite->setPosition(position);
		for (sf::Shape* extraSprite : extraSprites)
			extraSprite->setPosition(position);

	}
	void adjustPosition(float x, float y) {
		sprite->move(x, y);
		for (sf::Shape* extraSprite : extraSprites)
			extraSprite->move(x, y);
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

// Laser with instantaneous travel time. Rectangular hitbox;
class Laser : public ComplexBullet {
protected:
	// Basic attributes
	float growthSpeed; // pixel per second. If growth speed = maxWidth, laser grows to max width in one second
	float maxWidth, currentWidth;
	sf::Vector2f centerPos;
	bool hitboxActive; // Activates collision
	float activationDelay; // Seconds before collision becomes active

	// Internal calculation variables
	int frameCounter;

	// Operation pointers
	SfRectangleAtHome* rect; // Pointer shared with sprite for rectangle specific functions
	SfCircleAtHome* cir; // Same with circle
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
		target.draw(*sprite, states);
		for (sf::Shape* extraSprite : extraSprites)
			target.draw(*extraSprite, states);
	}
public:
	// Default orientation is straight right, therefore the laser width is actually the rectangle height
	Laser(sf::Vector2f centerPos = SCREENPOS, float angleDegrees = 0, float maxWidth = 0, float growthSpeed = 1, float activationDelay = 0, sf::Color color = WHITE)
		: Bullet(), ComplexBullet() {
		this->growthSpeed = growthSpeed;
		this->maxWidth = maxWidth;
		this->centerPos = centerPos;
		this->activationDelay = activationDelay;
		frameCounter = 0;
		hitboxActive = false;

		color.a = 150;
		currentWidth = 1;
		rect = new SfRectangleAtHome(WHITE, { WINDOWWIDTH, currentWidth }, centerPos, false, color, 1);
		sprite = rect;
		rotateBullet(angleDegrees);
		cir = new SfCircleAtHome(WHITE, 2, centerPos, true, TRANSPARENTWHITE, SMALLBULLETOUTLINE);
		extraSprites.push_back(cir);
	}
	void rotateBullet(float angleDegrees) {
		sprite->rotate(angleDegrees);
		alignSprite();
	}
	// Process the active status and growth of laser
	virtual void processMovement() {
		frameCounter++;
		rotateBullet(1);

		// Return if laser should not be active
		if (frameCounter / FPS < activationDelay)
			return;
		// Activate laser
		hitboxActive = true;
		// Process laser growth
		float targetWidth = (frameCounter / FPS - activationDelay) * growthSpeed;
		if (targetWidth < maxWidth) {
			rect->setSize({ rect->getSize().x, targetWidth });
			rect->setOutlineThickness(max(targetWidth / 5, 1.f));
			cir->setRadius(targetWidth / 1.4);
			alignSprite();
			// Align circle only during laser size change
			cir->alignCenter();
			cir->setPosition(centerPos);
			currentWidth = targetWidth;
		}
		else
			currentWidth = maxWidth;
		if (xVelocity != 0 || yVelocity != 0) {
			centerPos.x += xVelocity;
			centerPos.y += yVelocity;
			sprite->move(xVelocity, yVelocity);
			for (sf::Shape* extraSprite : extraSprites)
				extraSprite->move(xVelocity, yVelocity);
		}
	}
	// Make sure the rectangle laser is aligned with the circle
	void alignSprite() {
		rect->alignX();
		rect->setPosition(centerPos.x - sin(rect->getRotation() * PI / 180) * rect->getOutlineThickness(), centerPos.y + cos(rect->getRotation() * PI / 180) * rect->getOutlineThickness());
	}
	// Adds an offset to position instead of setting it
	virtual void adjustPosition(float x, float y) {
		sprite->move(x, y);
	}
	void resetBullet() {
		frameCounter = 0;
		currentWidth = 1;
		rect->setSize({ rect->getSize().x, currentWidth});
		rect->setOutlineThickness(SMALLBULLETOUTLINE);
		cir->setRadius(2);
		alignSprite();
		// Align circle only during laser size change
		cir->alignCenter();
		cir->setPosition(centerPos);
	}
	bool checkPlayerCollision(sf::CircleShape& hitbox) {
		if (!hitboxActive)
			return false;
		// Compare the relative position of the hitbox to an upright rect
		sf::Vector2f hitboxPos = hitbox.getPosition();
		SfRectangleAtHome newRec = *rect;
		newRec.setRotation(0);
		float angle = rect->getRotation() * PI / 180;
		sf::Vector2f dist = { hitboxPos.x - centerPos.x, hitboxPos.y - centerPos.y };
		float mag = sqrt(pow(dist.x, 2) + pow(dist.y, 2));
		float angle2 = atan(dist.y / dist.x);
		if (dist.x < 0)
			angle2 += PI;
		float newX = centerPos.x + (mag) * cos(angle2 - angle);
		float newY = centerPos.y + (mag) * sin(angle2 - angle);
		newRec.alignX();
		newRec.setPosition(centerPos);
		sf::FloatRect bounds = newRec.getGlobalBounds();
		bounds.top += rect->getOutlineThickness();
		
		return bounds.contains(newX, newY) || (sqrt(pow(hitboxPos.x - centerPos.x, 2) + pow(hitboxPos.y - centerPos.y, 2)) <= hitbox.getRadius() + maxWidth / 2);
	}
};

class RiceBullet : public CircleBullet {
public:
	RiceBullet(sf::Vector2f position = SCREENPOS, float xVelocity = 0, float yVelocity = 0, sf::Color color = WHITE, int radius = 0) 
		: CircleBullet(position, xVelocity, yVelocity, color, radius), Bullet(xVelocity, yVelocity) {
		sprite->setOutlineThickness(SMALLBULLETOUTLINE);
		alignAngle();
		sprite->scale(1, 2);
	}
};
class DotBullet : public CircleBullet {
public:
	DotBullet(sf::Vector2f position = SCREENPOS, float xVelocity = 0, float yVelocity = 0, sf::Color color = WHITE, int radius = 0) 
		: CircleBullet(position, xVelocity, yVelocity, color, radius), Bullet(xVelocity, yVelocity) {
		sprite->setOutlineThickness(SMALLBULLETOUTLINE);
	}
};
class TalismanBullet : public CircleBullet {
public:
	TalismanBullet(sf::Vector2f position = SCREENPOS, float xVelocity = 0, float yVelocity = 0, sf::Color color = WHITE, int radius = 0) 
		: CircleBullet(position, xVelocity, yVelocity, color, radius), Bullet(xVelocity, yVelocity) {
		delete sprite;
		sprite = new SfRectangleAtHome(TRANSPARENTWHITE, { 3.f * radius, 4.f * radius }, position, true, color, STANDARDCIRCLEOUTLINE);
		alignAngle();
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

