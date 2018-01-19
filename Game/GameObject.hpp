#pragma once

#include <SFML/Graphics.hpp>

class GameObject {
public:
	virtual sf::Vector2f getSize() const = 0;

	virtual sf::Vector2f getPosition() const = 0;

	virtual void setPosition(const sf::Vector2f& position) = 0;

	bool operator== (const GameObject& other) {
		return this == &other;
	}
};