#pragma once
#include "Constants.h"

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



// Organize drawable rectangle elements
class BorderRects : sf::Drawable {
	SfRectangleAtHome background;
	SfRectangleAtHome border1;
	SfRectangleAtHome border2;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
		target.draw(background, states);
		target.draw(border1, states);
		target.draw(border2, states);
	}
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
