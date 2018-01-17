#pragma once

#include <SFML\Graphics.hpp>
#include "PhysicsObject.hpp"
#include "Collision.hpp"

class Rectangle : public PhysicsObject, public sf::RectangleShape {
public:
	Rectangle() : sf::RectangleShape(), PhysicsObject(static_cast<sf::RectangleShape&>(*this)) {
		sf::RectangleShape::setFillColor({ 255, 255, 255 });
	}

	void update(const float elapsedTime) override {
		PhysicsObject::update(elapsedTime);
	}

	void setSize(sf::Vector2f size) {
		sf::RectangleShape::setOrigin(size.x / 2, size.y / 2);
		sf::RectangleShape::setSize(size);
	}

	sf::FloatRect getBounds() override {
		return getGlobalBounds();
	}

	/**
	* @brief Resolve an collision.
	* @param other Rectangle.
	*/
	void resolveCollision(Rectangle &other) {
		Collision collision = getCollision(other);

		if (collision.check()) {
			sf::Vector2f mvt;
			
			if (collision.getIntersect().x > collision.getIntersect().y) {
				if (collision.getDelta().x > 0.0f) {
					mvt = sf::Vector2f(collision.getIntersect().x * 1.0f, 0.0f);
				} else {
					mvt = sf::Vector2f(-collision.getIntersect().x * 1.0f, 0.0f);
				}
			} else {
				if (collision.getDelta().y > 0.0f) {
					mvt = sf::Vector2f(0.0f, collision.getIntersect().y);
				} else {
					mvt = sf::Vector2f(0.0f, -collision.getIntersect().y);
					setVelocity({ getVelocity().x, 0 });
				}
			}

			if (getVelocity().y > 0) {
				setVelocity({ getVelocity().x, 0 });
			}

			setPosition(getPosition() + mvt);
		}
	}

	/**
	* @brief Get an collision instance containing the delta and intersect position between two rectangles.
	* @param other Rectangle.
	* @return Collision class instance.
	*/
	Collision getCollision(Rectangle &other) {
		sf::Vector2f otherPosition = other.getPosition();
		sf::Vector2f otherHalfSize = other.getSize() / 2.0f;
		sf::Vector2f thisPosition = getPosition();
		sf::Vector2f thisHalfSize = getSize() / 2.0f;

		sf::Vector2f delta(otherPosition.x - thisPosition.x, otherPosition.y - thisPosition.y);
		sf::Vector2f intersect(abs(delta.x) - (otherHalfSize.x + thisHalfSize.x), abs(delta.y) - (otherHalfSize.y + thisHalfSize.y));

		return Collision(delta, intersect);
	}

	/**
	* @brief Check if two rectangles intersects with each other.
	* @param other Rectangle.
	* @return Result.
	*/
	bool intersects(Rectangle &other) {
		Collision collision = getCollision(other);

		return collision.check();
	}

	using PhysicsObject::setPosition;
	using PhysicsObject::getPosition;
	using PhysicsObject::draw;
};
