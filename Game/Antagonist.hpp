#pragma once

#include <SFML/Graphics.hpp>
#include "Rectangle.hpp"

class Antagonist : public Rectangle {
	int32_t walkDirection = 1;
	float walkspeed = 50;

public:
	Antagonist() {
		setSize({ 200, 200 });
		setFillColor(sf::Color(255, 0, 0));
		setVelocity({ walkDirection * walkspeed, getVelocity().y });
		setGravity({ 0, 0 });
	}

	sf::FloatRect getBounds() override {
		return getGlobalBounds();
	}

	using Rectangle::setPosition;
	using Rectangle::getPosition;
	using Rectangle::setSize;
	using Rectangle::draw;
};