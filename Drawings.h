#pragma once
#include "Constants.h"
#include "Mechanisms.h"

#pragma region Sf Sprites At Home
using namespace std;
using namespace Constants;
// Class for easy sf::Text generation. Replaces the setText function
class SfTextAtHome : public sf::Text {
public:
	SfTextAtHome() {}
	SfTextAtHome(sf::Font& font, sf::Color color, string message, unsigned int textSize, sf::Vector2f position, bool bold = true, bool underlined = false, bool centered = false, bool outline = false) {
		setFont(font);
		setFillColor(color);
		setString(message);
		setCharacterSize(textSize);
		if (centered)
			alignCenter();
		setPosition(position);
		if (bold)
			setStyle(sf::Text::Bold);
		if (underlined)
			setStyle(sf::Text::Underlined);
		if (outline)
		{
			setOutlineColor(BLACK);
			setOutlineThickness(1);
		}
	}
	void alignCenter() {
		const sf::FloatRect box = getLocalBounds();
		setOrigin(box.width / 2.0f, box.height / 2.0f);
	}
	bool contains(float x, float y) {
		return getGlobalBounds().contains(x, y);
	}
};

// Class for easy sf::RectangleShape generation.
class SfRectangleAtHome : public sf::RectangleShape {
public:
	SfRectangleAtHome() {}
	SfRectangleAtHome(sf::Color color, sf::Vector2f size, sf::Vector2f position, bool centered = false, sf::Color outlineColor = sf::Color(), float outlineThickness = 0) {
		setSize(size);
		setFillColor(color);
		if (centered)
			alignCenter();
		setPosition(position);
		setOutlineColor(outlineColor);
		setOutlineThickness(outlineThickness);
	}
	void alignCenter() {
		const sf::FloatRect box = getLocalBounds();
		setOrigin(box.width / 2.0f, box.height / 2.0f);
	}
	bool contains(float x, float y) {
		return getGlobalBounds().contains(x, y);
	}
};

// Class for easy sf::Circle generation
class SfCircleAtHome : public sf::CircleShape {
public:
	SfCircleAtHome() {}
	SfCircleAtHome(sf::Color color, float radius, sf::Vector2f position, bool centered = false, sf::Color outlineColor = sf::Color(), float outlineThickness = 0) {
		setRadius(radius);
		setFillColor(color);
		if (centered)
			alignCenter();
		setPosition(position);
		setOutlineColor(outlineColor);
		setOutlineThickness(outlineThickness);
	}
	void alignCenter() {
		setOrigin(getRadius(), getRadius());
	}
	bool contains(float x, float y) {
		return getGlobalBounds().contains(x, y);
	}
};
#pragma endregion

// Animations are meant to be each created once and restarted when played
class Animation {
protected:
	sfClockAtHome startTime;
	float duration;

public:
	Animation() {
		duration = 0;
	}
	// Draws the animation to the window
	virtual void drawAnimation(sf::RenderWindow& window) = 0;
	virtual ~Animation() {};
	// Turns on animation
	virtual void restart() = 0;
	// Return true if animation has expired
	virtual bool isOver() {
		return startTime.getTimeSeconds() > duration;
	}
};

// Text that fades after a set duration
class FadeText : public Animation {
	float fadeDuration; // In seconds
	SfTextAtHome text; // Text to display. 
	sf::Color textColor;
public:
	FadeText(SfTextAtHome text, float duration, float fadeDuration) {
		this->text = text;
		this->duration = duration;
		this->fadeDuration = fadeDuration;
		startTime.restart();
		textColor = text.getFillColor();
		textColor.a = 0; // This disables the animation when constructed
		this->text.setFillColor(textColor);
	}
	// Draws the animation to the window
	void drawAnimation(sf::RenderWindow& window) {
		float elapsedTime = startTime.getElapsedTime().asSeconds();
		if (elapsedTime > duration + fadeDuration)
			return; // Returns nothing if animation is past duration
		if (elapsedTime > duration && textColor.a > 0) { // Begin fading
			textColor.a = ((1 - (elapsedTime - duration) / fadeDuration) * 255);
			text.setFillColor(textColor);
		}
		window.draw(text);
	}
	// Turns on animation
	void restart() {
		startTime.restart();
		textColor.a = 255;
		text.setFillColor(textColor);
	}
	// Change the text to display
	void setString(string str) {
		text.setString(str);
	}
};

// Organize drawable rectangle elements
class BorderRects {
	SfRectangleAtHome background;
	SfRectangleAtHome border1;
	SfRectangleAtHome border2;
public:
	BorderRects() {
		background = SfRectangleAtHome(GRAY, { SCREENWIDTH, SCREENHEIGHT }, { SCREENLEFT, SCREENTOP });
		border1 = SfRectangleAtHome(TRANSPARENT, { SCREENWIDTH, SCREENHEIGHT }, { SCREENLEFT, SCREENTOP }, false, WHITE, 1);
		border2 = SfRectangleAtHome(TRANSPARENT, { SCREENWIDTH + 2, SCREENHEIGHT + 2 }, { SCREENLEFT - 1, SCREENTOP - 1 }, false, BLACK, BORDERMARGIN * 2);
	}
	SfRectangleAtHome& getBackground() {
		return background;
	}
	SfRectangleAtHome& getBorder1() {
		return border1;
	}
	SfRectangleAtHome& getBorder2() {
		return border2;
	}
};
