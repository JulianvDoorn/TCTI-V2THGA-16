#pragma once

#include <SFML/Graphics.hpp>

#include "GameObject.hpp"
#include "Collision.hpp"

class Collidable : public GameObject {
	bool sideCollision = false;

protected:

	/**
	 * @fn	virtual void Collidable::onCollide(Collidable& other)
	 *
	 * @brief	Executes when two collidables have collided
	 *
	 * @author	Julian
	 * @date	2018-01-19
	 *
	 * @param [in,out]	other	Collidable with which has been collided
	 */

	virtual void onCollide(Collidable& other) { }

	friend class IntersectionGroup;
	
public:

	/**
	 * @fn	virtual sf::Vector2f Collidable::getVelocity() const
	 *
	 * @brief	Returns the velocity of this collidable.
	 * 			Required for some collision resolving but not for collision detection.
	 *
	 * @author	Julian
	 * @date	2018-01-19
	 *
	 * @return	velocity
	 */

	virtual sf::Vector2f getVelocity() const {
		return { 0, 0 };
	}

	/**
	 * @fn	virtual void Collidable::setVelocity(const sf::Vector2f& velocity)
	 *
	 * @brief	Sets the velocity of this collidable.
	 * 			Required for some collision resolving but not for collision detection.
	 *
	 * @author	Julian
	 * @date	2018-01-19
	 *
	 * @param	velocity	velocity
	 */

	virtual void setVelocity(const sf::Vector2f& velocity) { }

	/**
	 * @fn	void Collidable::resolveCollision(Collidable& other, Collision collision)
	 *
	 * @brief	Resolves collision and invokes Collidable::onCollide(Collidable&)
	 *
	 * @author	Julian
	 * @date	2018-01-19
	 *
	 * @param [in,out]	other	 	Collidable& with which has been collided
	 * @param 		  	collision	Collision information required for resolving the collision
	 */

	void resolveCollision(Collidable& other, Collision collision) {
		onCollide(other);

		sf::Vector2f mtv;
		if ((collision.getDelta().y > 30 && collision.getDelta().y < -20) || getVelocity().y == 0) {//to check if the bottom type of resolving is needed
			if ((collision.getIntersect().x - collision.getIntersect().y) > -10) {//-10 is the treshold of detecting if the horizontal collision is worth resolvin
				//for the bottom of an object
				sideCollision = true;

				if (collision.getDelta().x > 0.0f) {
					mtv = sf::Vector2f(collision.getIntersect().x, 0.0f);
				}
				else {
					mtv = sf::Vector2f(-collision.getIntersect().x, 0.0f);
				}

				setPosition(getPosition() + mtv);

				if (getVelocity().x != 0) { // set horizontal velocity to 0
					setVelocity({ 0, getVelocity().y });
				}

				return;
			}
		}
			if ((collision.getIntersect().x - collision.getIntersect().y) > 10) {//10 is the treshold of detecting if the horizontal collision is worth resolving
			//for the top of an object
				sideCollision = true;

				if (collision.getDelta().x > 0.0f) {
					mtv = sf::Vector2f(collision.getIntersect().x, 0.0f);
				}
				else {
					mtv = sf::Vector2f(-collision.getIntersect().x, 0.0f);
				}

				setPosition(getPosition() + mtv);

				if (getVelocity().x > 0) { // set horizontal velocity to 0
					setVelocity({ 0, getVelocity().y });
				}

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
		}
		else {
			mtv = sf::Vector2f(0.0f, -collision.getIntersect().y);
			setVelocity({ getVelocity().x, 0 });
		}

		if (getVelocity().y > 0) { //set vertical velocity to 0
			setVelocity({ getVelocity().x, 0 });
		}

		setPosition(getPosition() + mtv);
	}

	/**
	 * @fn	Collision Collidable::getCollision(Collidable& other)
	 *
	 * @brief	Generates a Collision instance using @code Collision(*this, other) @endcode
	 *
	 * @author	Julian
	 * @date	2018-01-19
	 *
	 * @param [in,out]	other	Collidable with which has been collided
	 *
	 * @return	Collision information
	 */

	Collision getCollision(Collidable& other) {
		return Collision(*this, other);
	}
};