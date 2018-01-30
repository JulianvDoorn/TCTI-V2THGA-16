#pragma once

#include <SFML/Graphics.hpp>

class MovingObject {
protected:
	sf::Vector2f velocity;

public:
	sf::Vector2f getVelocity() const {
		return velocity;
	}

	void setVelocity(const sf::Vector2f& velocity) {
		this->velocity = velocity;
	}
};