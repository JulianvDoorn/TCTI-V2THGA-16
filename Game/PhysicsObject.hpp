#pragma once

#include <SFML/Graphics.hpp>
#include "Drawable.hpp"
#include "Collision.hpp"

class PhysicsObject : public Drawable {
protected:
	sf::Transformable& transformable;
	sf::Vector2f velocity = sf::Vector2f(0, 0);
	sf::Vector2f gravity = sf::Vector2f(0, 981.0f);

	PhysicsObject(sf::Shape& shape) : Drawable(shape), transformable(shape) { }
	PhysicsObject(sf::Sprite& sprite) : Drawable(sprite), transformable(sprite) { }

	bool sideCollision;
public:
	virtual void update(const float elapsedTime) {
		applyForce(gravity * elapsedTime);
		setPosition(getPosition() + velocity * elapsedTime);
	}

	virtual void setGravity(const sf::Vector2f& gravity) {
		this->gravity = gravity;
	}

	virtual void setPosition(const sf::Vector2f& position) {
		transformable.setPosition(position);
	}
	
	virtual sf::Vector2f getSize() const = 0;

	virtual sf::Vector2f getPosition() const {
		return transformable.getPosition();
	}

	virtual sf::FloatRect getBounds() = 0;

	virtual void applyForce(const sf::Vector2f& force) {
		velocity += force;
	}

	virtual void setVelocity(const sf::Vector2f& velocity) {
		this->velocity = velocity;
	}

	virtual sf::Vector2f getVelocity() {
		return velocity;
	}

	/**
	* @brief Resolve an collision.
	* @param other Rectangle.
	*/
	void resolveCollision(PhysicsObject &other) {
		Collision collision = getCollision(other);

		/*if (collision.check()) {
			sf::Vector2f mvt;
			std::cout << "x: " << collision.getIntersect().x << "	y:" << collision.getIntersect().y << std::endl;
			if (collision.getIntersect().x > collision.getIntersect().y) {
				std::cout << "zijkant: x: "<< collision.getIntersect().x << "	y:" <<collision.getIntersect().y << std::endl;
				if (collision.getDelta().x > 0.0f) {
					mvt = sf::Vector2f(collision.getIntersect().x, 0.0f);
				}
				else {
					mvt = sf::Vector2f(-collision.getIntersect().x, 0.0f);
				}
			}
			else {
				std::cout << "onderkant: x: " << collision.getIntersect().x << "	y:" << collision.getIntersect().y << std::endl;
				if (collision.getDelta().y > 0.0f) {
					mvt = sf::Vector2f(0.0f, collision.getIntersect().y);
				}
				else {
					mvt = sf::Vector2f(0.0f, -collision.getIntersect().y);
					setVelocity({ getVelocity().x, 0 });
				}
				if (getVelocity().y > 0) {
					setVelocity({ getVelocity().x, 0 });
				}
			}

			setPosition(getPosition() + mvt);
		}*/

		if (collision.check()) {
			sf::Vector2f mvt;
			if ((collision.getIntersect().x - collision.getIntersect().y) > 8) {//8 is the treshold of detecting if the collision is worth resolving.
				sideCollision = true;
				if (collision.getDelta().x > 0.0f) {
					mvt = sf::Vector2f(collision.getIntersect().x, 0.0f);
				}
				else {
					mvt = sf::Vector2f(-collision.getIntersect().x, 0.0f);
				}
				setPosition(getPosition() + mvt);
				return;
			}
			if (sideCollision) { // to check if the last collision was with the side
				sideCollision = false;
				if (getVelocity().y >= 40) { //40 is to check if the player is not falling
					return;
				}
			}
			if (collision.getDelta().y > 0.0f) { //resols a collision of y type
				mvt = sf::Vector2f(0.0f, collision.getIntersect().y);
			}
			else {
				mvt = sf::Vector2f(0.0f, -collision.getIntersect().y);
				setVelocity({ getVelocity().x, 0 });
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
	Collision getCollision(PhysicsObject &other) {
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
	virtual bool intersects(PhysicsObject &other) {
		Collision collision = getCollision(other);
		return collision.check();
	}

};