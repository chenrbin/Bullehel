#pragma once

// File to contain all bullet implementation

// Game projectiles
class Bullet : public sf::Drawable { // Abstract base class
protected:
	sf::Shape* sprite; // Base sprite. Faces right by default (rotatfion 0).
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
#pragma region Rotational transformation
	// Rotate bullet direction. Optionally provide speed so it does not have to be calculated
	virtual void rotateBullet(float angleDegrees, float speed = 0) {
		if (angleDegrees == 0) return;
		// Edge cases, if variables are 0
		if (yVelocity == 0 && xVelocity == 0) {
			sprite->rotate(angleDegrees);
			return;
		}
		float currentSpeed = (speed != 0) ? speed : sqrt(pow(xVelocity, 2) + pow(yVelocity, 2));

		float currentAngle = sprite->getRotation();
		xVelocity = cos((currentAngle + angleDegrees) * PI / 180) * currentSpeed;
		yVelocity = sin((currentAngle + angleDegrees) * PI / 180) * currentSpeed;
		sprite->rotate(angleDegrees);
	}
	// Given a target radius and speed, rotate a bullet so that it will form a circle of that radius.
	// Positive speed for clockwise rotation, negative for counterclockwise
	virtual void rotateArc(float targetRadius, float speed) {
		// No need to execute if any arguments are 0
		if (speed == 0 || targetRadius == 0) return;
		// Speed is passed in so it wouldn't have to be calculated manually
		rotateBullet(speed * 360 / (2 * PI * targetRadius), abs(speed));
	}
	// Sets the rotation and velocity to a specified angle
	virtual void setRotation(float angleDegrees, float speed = 0) {
		sprite->setRotation(angleDegrees);
		float currentSpeed = (speed != 0) ? speed : sqrt(pow(xVelocity, 2) + pow(yVelocity, 2));
		xVelocity = cos((angleDegrees)*PI / 180) * currentSpeed;
		yVelocity = sin((angleDegrees)*PI / 180) * currentSpeed;
	}
	// Given a rectangular coordinate, aim bullet towards it
	virtual void aimBullet(sf::Vector2f targetPos) {
		setRotation(getAngleToPos(sprite->getPosition(), targetPos));
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
	void alignAngle(float xOffset = 0, float yOffset = 0) {
		if (yVelocity + yOffset == 0 && xVelocity + xOffset == 0)
			return;
		float angle;
		angle = atan2f((yVelocity + yOffset), (xVelocity + xOffset)) * 180 / PI;
		sprite->setRotation(angle);
	}
#pragma endregion


#pragma region Position and Velocity Transformations
	// Adds an offset to position instead of setting it
	virtual void adjustPosition(float x, float y) {
		sprite->move(x, y);
	}
	virtual void setPosition(float x, float y) {
		sprite->setPosition(x, y);
	}
	// Adjust position of a bullet rotating in an arc such that its origin point remains the same
	virtual void alignArc(float deltaRadius) {
		float angleDelta = (sprite->getRotation() - 90) / 180 * PI;
		adjustPosition(deltaRadius * cos(angleDelta), deltaRadius * sin(angleDelta));
	}
	// Sets velocity. Polar version will be used more often
	virtual void setVelocity(float x, float y) {
		xVelocity = x;
		yVelocity = y;
		alignAngle();
	}
	// Set velocity with polar coordinates
	virtual void setVelocityR(float speed, float angleDegrees) {
		xVelocity = speed * cos(angleDegrees * PI / 180);
		yVelocity = speed * sin(angleDegrees * PI / 180);
		sprite->setRotation(angleDegrees);
	}
	// Adds an offset to velocity
	virtual void adjustVelocity(float x, float y) {
		xVelocity += x;
		yVelocity += y;
		alignAngle();
	}
	// Adds a multiplier to the velocity
	void scaleVelocity(float x, float y) {
		xVelocity *= x;
		yVelocity *= y;
		alignAngle();
	}
	// Set velocity facing current angle
	virtual void setSpeed(float speed) {
		xVelocity = speed * cos(sprite->getRotation() * PI / 180);
		yVelocity = speed * sin(sprite->getRotation() * PI / 180);
	}
	// Adds an offset to velocity and maintains rotation
	virtual void adjustSpeed(float speed) {
		xVelocity += speed * cos(sprite->getRotation() * PI / 180);
		yVelocity += speed * sin(sprite->getRotation() * PI / 180);
	}
#pragma endregion

	// Set flags used for certain patterns
	void setFlag(char val) {
		flag = val;
	}
	// Get the angle from source facing target in degrees.
	float getAngleToPos(sf::Vector2f sourcePos, sf::Vector2f targetPos) {
		return atan2(targetPos.y - sourcePos.y, targetPos.x - sourcePos.x) * 180 / PI;
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
	float getSpeed() {
		return sqrt(pow(xVelocity, 2) + pow(yVelocity, 2));
	}
	float getRotation() {
		return sprite->getRotation();
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
	CircleBullet(sf::Vector2f position = SCREENPOS, float speed = 0, float angleDegrees = 0, sf::Color color = WHITE, int radius = 0, bool useCircleSprite = true)
		: Bullet(speed, angleDegrees) {
		if (useCircleSprite) { // Slight optimization. Use false for child classes that don't use the circle
			sprite = new SfCircleAtHome(WHITE, radius, position, true, color, max(STANDARDCIRCLEOUTLINE, radius / 3.f));
			sprite->setRotation(angleDegrees);
		}
		hitBoxRadius = radius;
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
		for (sf::Shape* extraSprite : extraSprites)
			target.draw(*extraSprite, states);
	}
public:
	ComplexBullet() : Bullet() {}
	~ComplexBullet() {
		for (sf::Shape* extraSprite : extraSprites)
			delete extraSprite;
	}
	// Called to move every frame
	virtual void processMovement() {
		sprite->move(xVelocity, yVelocity);
		for (sf::Shape* extraSprite : extraSprites){
			// Sync extra sprites with main sprite
			extraSprite->setRotation(sprite->getRotation());
			extraSprite->setPosition(sprite->getPosition());
		}
	
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
		cir = new SfCircleAtHome(WHITE, 2, centerPos, true, color, SMALLBULLETOUTLINE);
		rotateBullet(angleDegrees);
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
		rect->alignY();
		rect->setPosition(centerPos);
	}
	// Adds an offset to position instead of setting it
	virtual void adjustPosition(float x, float y) {
		ComplexBullet::adjustPosition(x, y);
	}
	void resetBullet() {
		frameCounter = 0;
		currentWidth = 1;
		rect->setSize({ rect->getSize().x, currentWidth });
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
		float angle2 = atan2f(dist.y, dist.x);
		float newX = centerPos.x + (mag)*cos(angle2 - angle);
		float newY = centerPos.y + (mag)*sin(angle2 - angle);
		newRec.alignY();
		newRec.setPosition(centerPos);
		sf::FloatRect bounds = newRec.getGlobalBounds();

		// Both the hitbox of the rectangle and the circle
		return bounds.contains(newX, newY) || (sqrt(pow(hitboxPos.x - centerPos.x, 2) + pow(hitboxPos.y - centerPos.y, 2)) <= hitbox.getRadius() + maxWidth / 2);
	}
};


class RiceBullet : public CircleBullet {
public:
	RiceBullet(sf::Vector2f position = SCREENPOS, float speed = 0, float angleDegrees = 0, sf::Color color = WHITE, int radius = 0)
		: CircleBullet(position, speed, angleDegrees, color, radius), Bullet(speed, angleDegrees) {
		sprite->setOutlineThickness(SMALLBULLETOUTLINE);
		sprite->scale(2, 1); // Stretch horizonally to look like an ellipse
		sprite->setRotation(angleDegrees);
	}
};
class DotBullet : public CircleBullet {
public:
	DotBullet(sf::Vector2f position = SCREENPOS, float speed = 0, float angleDegrees = 0, sf::Color color = WHITE, int radius = 0)
		: CircleBullet(position, speed, angleDegrees, color, radius), Bullet(speed, angleDegrees) {
		sprite->setOutlineThickness(SMALLBULLETOUTLINE);
		sprite->setRotation(angleDegrees);
	}
};
class TalismanBullet : public CircleBullet {
public:
	TalismanBullet(sf::Vector2f position = SCREENPOS, float speed = 0, float angleDegrees = 0, sf::Color color = WHITE, int radius = 0)
		: CircleBullet(position, speed, angleDegrees, color, radius, false), Bullet(speed, angleDegrees) {
		sprite = new SfRectangleAtHome(TRANSPARENTWHITE, { 4.f * radius, 3.f * radius }, position, true, color, STANDARDCIRCLEOUTLINE);
		sprite->setRotation(angleDegrees);
	}
};
class BubbleBullet : public ComplexBullet, public CircleBullet {
public:
	BubbleBullet(sf::Vector2f position = SCREENPOS, float speed = 0, float angleDegrees = 0, sf::Color color = WHITE, int radius = 0)
		: CircleBullet(position, speed, angleDegrees, color, radius), Bullet(speed, angleDegrees) {
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
// Arrowhead-shaped bullet
class ArrowheadBullet : public ComplexBullet, public RiceBullet {
	int hitBoxRadius;
public:
	ArrowheadBullet(sf::Vector2f position = SCREENPOS, float speed = 0, float angleDegrees = 0, sf::Color color = WHITE, int radius = 0)
		: RiceBullet(position, speed, angleDegrees, WHITE, radius) {
		hitBoxRadius *= 1.2; // Slightly increase hitbox size
		for (float i = -1; i <= 1; i += 2)
		{
			sf::ConvexShape* arrowPart = new sf::ConvexShape(7);
			arrowPart->setPoint(0, { -radius * 5.f, 0 });
			arrowPart->setPoint(1, { -radius * 4.5f, 0 });
			arrowPart->setPoint(2, { 0, radius * 1.6f * i });
			arrowPart->setPoint(3, { radius * 3.6f, radius * 1.6f * i });
			arrowPart->setPoint(4, { radius * 3.6f, radius * 2.5f * i});
			arrowPart->setPoint(5, { 0, radius * 2.5f * i });
			arrowPart->setPoint(6, { -radius * 5.f, radius * 0.8f * i, });
			arrowPart->setFillColor(color);
			arrowPart->setPosition(position);
			arrowPart->setRotation(angleDegrees);
			extraSprites.push_back(arrowPart);
		}
	}
};
// Special type of bullet that optionally has a hitbox. Used as a transformable sprite to track bullet spawn positions.
class Spawner : public CircleBullet {
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

