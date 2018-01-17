#pragma once

#include <SFML/Graphics.hpp>
#include "PhysicsObject.hpp"

class Ball : public PhysicsObject, public sf::CircleShape {
public:
	Ball() : sf::CircleShape(), PhysicsObject(static_cast<sf::CircleShape&>(*this)) {
		sf::CircleShape::setFillColor({ 255, 255, 255 });
	}

	void update(const float elapsedTime) override {
		PhysicsObject::update(elapsedTime);
	}

	void setRadius(const float radius) {
		sf::CircleShape::setOrigin(radius / 2.0f, radius / 2.0f);
		sf::CircleShape::setRadius(radius);
	}

	sf::Vector2f getCenter() {
		sf::Vector2f center = getPosition();
		center.x = center.x + (getRadius() / 2);
		center.y = center.y + (getRadius() / 2);
		return center;
	}

	sf::FloatRect getBounds() override {
		return sf::CircleShape::getGlobalBounds();
	}

	using PhysicsObject::setPosition;
	using PhysicsObject::getPosition;
	using PhysicsObject::draw;
};