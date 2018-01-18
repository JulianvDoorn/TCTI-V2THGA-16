#pragma once

#include <SFML/Graphics.hpp>
#include "Rectangle.hpp"
#include "CollisionObjects.hpp"

class Antagonist : public Rectangle {
private:
	sf::RenderWindow &window;

	int32_t walkDirection = 1;
	float walkspeed = 50;

public:
	Antagonist(sf::RenderWindow &window) : window(window)
	{
		setSize({ 200, 200 });
		setPosition({ -200, 200 });
		setFillColor(sf::Color(255, 0, 0));
		setVelocity({ walkDirection * walkspeed, getVelocity().y });
	}

	void update(const float elapsedTime) override {
		setPosition(getPosition() + velocity * elapsedTime);
	}


	sf::FloatRect getBounds() override {
		return getGlobalBounds();
	}

	using Rectangle::setPosition;
	using Rectangle::getPosition;
	using Rectangle::setSize;
	using Rectangle::draw;
};