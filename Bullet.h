#pragma once

// File to contain all bullet implementation

// Game projectiles
class Bullet : public sf::Drawable { // Abstract base class
protected:
	sf::Shape* sprite; // Base sprite. Faces right by default (rotation 0).
	char flag; // Flag code that will be used for various purposes
	float xVelocity, yVelocity;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
		target.draw(*sprite, states);
	}
public:
	Bullet(float speed = 0, float angleDegrees = 0) {
		sprite = nullptr;
		xVelocity = speed * cos(angleDegrees * PI / 180);
		yVelocity = speed * sin(angleDegrees * PI / 180);
		flag = NEUTRAL;
	}
	~Bullet() {
		delete sprite;
	}
	// Some bullet types with specific variables will use this
	virtual void resetBullet() {
		return;
	}
	// Called to move every frame
	virtual void processMovement() {
		sprite->move(xVelocity, yVelocity);
	}
	// Rotate bullet direction
	virtual void rotateBullet(float angleDegrees) {
		// Edge cases, if variables are 0
		if (yVelocity == 0 && xVelocity == 0) {
			sprite->rotate(angleDegrees);
			return;
		}
		if (angleDegrees == 0) return;
		float currentSpeed = sqrt(pow(xVelocity, 2) + pow(yVelocity, 2));
		float currentAngle = atan(yVelocity / xVelocity) * 180 / PI; 
		if (xVelocity < 0)
			currentAngle += 180;
		xVelocity = cos((currentAngle + angleDegrees) * PI / 180) * currentSpeed;
		yVelocity = sin((currentAngle + angleDegrees) * PI / 180) * currentSpeed;
		sprite->rotate(angleDegrees);
	}
	// Given a target radius, rotate a bullet so that it will form a circle of that radius
	virtual void rotateArc(float targetRadius, float speed) {
		// No need to execute if any arguments are 0
		if (speed == 0 || targetRadius == 0) return;
		// Speed is passed in so it wouldn't have to be calculated manually.
		rotateBullet(speed * 360 / (2 * PI * (targetRadius)));
	}
	// Given a rectangular coordinate, aim bullet towards it
	virtual void aimBullet(sf::Vector2f targetPos) {
		sf::Vector2f bulletPos = sprite->getPosition();
		if (bulletPos == targetPos) // Point is directly on bullet. No rotation.
			return;
		float angleToPoint = atan((targetPos.y - bulletPos.y) / (targetPos.x - bulletPos.x)) * 180 / PI;
		if (targetPos.x < bulletPos.x)
			angleToPoint += 180;
		rotateBullet(angleToPoint - sprite->getRotation());
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
	virtual void setPosition(float x, float y) {
		sprite->setPosition(x, y);
	}
	// Sets velocity but does not update bullet orientation. Polar version will be used more often
	virtual void setVelocity(float x, float y) {
		xVelocity = x;
		yVelocity = y;
	}
	// Set velocity with polar coordinates
	virtual void setVelocityR(float speed, float angleDegrees) {
		xVelocity = speed * cos(angleDegrees * PI / 180);
		yVelocity = speed * sin(angleDegrees * PI / 180);
		sprite->setRotation(angleDegrees);
	}
	// setVelocityR but keeps current rotation
	virtual void setSpeed(float speed) {
		xVelocity = speed * cos(sprite->getRotation() * PI / 180);
		yVelocity = speed * sin(sprite->getRotation() * PI / 180);
	}
	// Only sets the rotation of the sprite and nothing else
	virtual void setRotation(float angleDegrees) {
		sprite->setRotation(angleDegrees);
	}
	// Adds a multiplier to the velocity. Will update bullet orientation
	void scaleVelocity(float x, float y) {
		xVelocity *= x;
		yVelocity *= y;
		alignAngle();
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
	sf::Vector2f getVelocity() {
		return sf::Vector2f(xVelocity, yVelocity);
	}
	float getRotation() {
		return sprite->getRotation();
	}
	// Flip the x velocity (reflection along the y axis)
	virtual void flipX() {
		rotateBullet(180 - sprite->getRotation() * 2);
	}
	// Flip the y velocity (reflection along the x axis)
	virtual void flipY() {
		rotateBullet(-sprite->getRotation() * 2);
	}
	// Sync sprite orientation with actual velocity. Optionally add an offset.
	void alignAngle() {
		if (yVelocity == 0 && xVelocity == 0)
			return;
		float angle;
		angle = atan(yVelocity / xVelocity) * 180 / PI; // C++ handles atan(y/0), but not atan(0/0)
		if (xVelocity < 0)
			angle += 180;
		sprite->setRotation(angle);
	}
	void alignAngle(float xOffset, float yOffset) {
		if (yVelocity + yOffset == 0 && xVelocity + xOffset == 0)
			return;
		float angle;
		angle = atan((yVelocity + yOffset) / (xVelocity + xOffset)) * 180 / PI; // C++ handles atan(y/0), but not atan(0/0)
		if (xVelocity + xOffset < 0)
			angle += 180;
		sprite->setRotation(angle);
	}
	void skipFrames(int frameCount) {
		for (int i = 0; i < frameCount; i++)
			processMovement();
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
	CircleBullet(sf::Vector2f position = SCREENPOS, float speed = 0, float angleDegrees = 0, sf::Color color = WHITE, int radius = 0) : Bullet(speed, angleDegrees) {
		sprite = new SfCircleAtHome(WHITE, radius, position, true, color, STANDARDCIRCLEOUTLINE);
		hitBoxRadius = radius;
		sprite->setRotation(angleDegrees);
	}
	// Circular hitbox compares distance with sum of radius
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
		Bullet::rotateBullet(angleDegrees);
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
	float activeDuration; // Duration in seconds before deactivation phase. Permanent if 0;

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
	Laser(sf::Vector2f centerPos = SCREENPOS, float angleDegrees = 0, float maxWidth = 0, float growthSpeed = 1, float activationDelay = 0, float activeDuration = 0, sf::Color color = WHITE)
		: Bullet(), ComplexBullet() {
		this->growthSpeed = growthSpeed;
		this->maxWidth = maxWidth; // SHOULD have a max width of at least 2;
		this->centerPos = centerPos;
		this->activationDelay = activationDelay;
		this->activeDuration = activeDuration;
		frameCounter = 0;
		hitboxActive = false;

		color.a = 150;
		currentWidth = 1;
		rect = new SfRectangleAtHome(WHITE, { WINDOWWIDTH, currentWidth }, centerPos, false, color, 1);
		sprite = rect;
		rotateBullet(angleDegrees);
		cir = new SfCircleAtHome(WHITE, 2, centerPos, true, color, SMALLBULLETOUTLINE);
		extraSprites.push_back(cir);
	}
	void rotateBullet(float angleDegrees) {
		sprite->rotate(angleDegrees);
		alignSprite();
	}
	// Function to set the laser width and align sprites
	void setWidth(float targetWidth) {
		rect->setSize({ rect->getSize().x, targetWidth });
		cir->setRadius(targetWidth / 1.4);
		alignSprite();
		// Align circle only during laser size change
		cir->alignCenter();
		cir->setPosition(centerPos);
		currentWidth = targetWidth;
		if (targetWidth > 0) 
			rect->setOutlineThickness(max(targetWidth / 5, STARTINGLASEROUTLINE));
		else // No outline if width is zero
			rect->setOutlineThickness(0);
	}
	// Process the active status and growth of laser
	virtual void processMovement() {
		frameCounter++;
		
		// Deactivate laser
		if (frameCounter / FPS > activationDelay + activeDuration)
		{
			hitboxActive = false;
			// Laser shrinks at twice the growth speed
			float targetWidth = maxWidth - (frameCounter / FPS - activationDelay - activeDuration) * growthSpeed * 2;
			if (targetWidth > 0)
				if (maxWidth - ((frameCounter + 1) / FPS - activationDelay - activeDuration) * growthSpeed * 2 < 0)
					setWidth(0);
				else
					setWidth(targetWidth);
		}
		// Activate laser
		else if (frameCounter / FPS > activationDelay) {
			hitboxActive = true;
			// Process laser growth
			float targetWidth = (frameCounter / FPS - activationDelay) * growthSpeed;
			if (targetWidth < maxWidth)
				// If laser exceeds max width on the next frame, "snap" to max width
				if (((frameCounter + 1) / FPS - activationDelay) * growthSpeed > maxWidth)
					setWidth(maxWidth);
				else
					setWidth(targetWidth);
		}
		// Process movement only if velocity is not zero.
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
		ComplexBullet::adjustPosition(x, y);
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
		
		// Both the hitbox of the rectangle and the circle
		return bounds.contains(newX, newY) || (sqrt(pow(hitboxPos.x - centerPos.x, 2) + pow(hitboxPos.y - centerPos.y, 2)) <= hitbox.getRadius() + maxWidth / 2);
	}
};

class RiceBullet : public CircleBullet {
public:
	RiceBullet(sf::Vector2f position = SCREENPOS, float speed = 0, float angleDegrees = 0, sf::Color color = WHITE, int radius = 0)
		: CircleBullet(position, xVelocity, yVelocity, color, radius), Bullet(speed, angleDegrees) {
		sprite->setOutlineThickness(SMALLBULLETOUTLINE);
		sprite->scale(2, 1);
		sprite->setRotation(angleDegrees);
	}
};
class DotBullet : public CircleBullet {
public:
	DotBullet(sf::Vector2f position = SCREENPOS, float speed = 0, float angleDegrees = 0, sf::Color color = WHITE, int radius = 0)
		: CircleBullet(position, xVelocity, yVelocity, color, radius), Bullet(speed, angleDegrees) {
		sprite->setOutlineThickness(SMALLBULLETOUTLINE);
		sprite->setRotation(angleDegrees);
	}
};
class TalismanBullet : public CircleBullet {
public:
	TalismanBullet(sf::Vector2f position = SCREENPOS, float speed = 0, float angleDegrees = 0, sf::Color color = WHITE, int radius = 0)
		: CircleBullet(position, xVelocity, yVelocity, color, radius), Bullet(speed, angleDegrees) {
		delete sprite;
		sprite = new SfRectangleAtHome(TRANSPARENTWHITE, { 4.f * radius, 3.f * radius }, position, true, color, STANDARDCIRCLEOUTLINE);
		sprite->setRotation(angleDegrees);
	}
};
class BubbleBullet : public ComplexBullet, public CircleBullet {
public:
	BubbleBullet(sf::Vector2f position = SCREENPOS, float speed = 0, float angleDegrees = 0, sf::Color color = WHITE, int radius = 0)
		: CircleBullet(position, xVelocity, yVelocity, color, radius), Bullet(speed, angleDegrees) {
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
		sprite->setRotation(angleDegrees);
	}
};
// Special type of bullet that optionally has a hitbox. Used as a transformable sprite to track bullet spawn positions.
class Spawner : public CircleBullet{
	bool visible; // Determines if spawner is visible. Cannot be changed after construction.
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
		if (visible)
			target.draw(*sprite, states);
	}
public:
	Spawner(sf::Vector2f position = SCREENPOS, float speed = 0, float angleDegrees = 0, bool visible = false, sf::Color color = WHITE, int radius = 0)
		: CircleBullet(position, xVelocity, yVelocity, color, radius), Bullet(speed, angleDegrees) {
		sprite->setFillColor(color);
		sprite->setOutlineThickness(SMALLBULLETOUTLINE);
		sprite->setRotation(angleDegrees);
		this->visible = visible;
	}
	bool checkPlayerCollision(sf::CircleShape& playerHitbox) {
		if (flag != ACTIVESPAWNERHITBOX) // Flag indicates whether hitbox is active
			return false;
		return CircleBullet::checkPlayerCollision(playerHitbox);
	}
};

#pragma endregion

