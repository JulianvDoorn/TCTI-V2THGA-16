#pragma once

#include <SFML/Graphics.hpp>

#include "GameObject.hpp"
#include "Collision.hpp"

class Collidable : public GameObject {
	bool sideCollision = false;

protected:
	virtual void onCollide(Collidable& other) { }

public:
	virtual sf::Vector2f getVelocity() const {
		return { 0, 0 };
	}

	virtual void setVelocity(const sf::Vector2f& velocity) { }

	void resolveCollision(Collidable& other, Collision collision) {
		onCollide(other);

		sf::Vector2f mtv;

		if ((collision.getIntersect().x - collision.getIntersect().y) > 8) {//8 is the treshold of detecting if the collision is worth resolving.
			sideCollision = true;

			if (collision.getDelta().x > 0.0f) {
				mtv = sf::Vector2f(collision.getIntersect().x, 0.0f);
			} else {
				mtv = sf::Vector2f(-collision.getIntersect().x, 0.0f);
			}

			setPosition(getPosition() + mtv);
			return;
		}

		if (sideCollision) { // to check if the last collision was with the side
			sideCollision = false;

			if (getVelocity().y >= 40) { //40 is to check if the player is not falling
				return;
			}
		}

		if (collision.getDelta().y > 0.0f) { //resols a collision of y type
			mtv = sf::Vector2f(0.0f, collision.getIntersect().y);
		} else {
			mtv = sf::Vector2f(0.0f, -collision.getIntersect().y);
			setVelocity({ getVelocity().x, 0 });
		}

		if (getVelocity().y > 0) {
			setVelocity({ getVelocity().x, 0 });
		}

		setPosition(getPosition() + mtv);
	}

	Collision getCollision(Collidable& other) {
		return Collision(*this, other);
	}
};