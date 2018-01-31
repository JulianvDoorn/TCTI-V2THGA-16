#pragma once

#include <SFML/Graphics.hpp>

#include "GameObject.hpp"
#include "MovingObject.hpp"
#include "Collision.hpp"
#include "ClickableRectangle.hpp"

/**
 * @class	Collidable
 *
 * @brief	A collidable.
 *
 * @author	Jeffrey
 * @date	1/31/2018
 */

class Collidable : public GameObject, public MovingObject, public virtual sf::RectangleShape {
	/** @brief	True to side collision */
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

	/**
	 * @class	IntersectionGroup
	 *
	 * @brief	An intersection group.
	 *
	 * @author	Jeffrey
	 * @date	1/31/2018
	 */

	friend class IntersectionGroup;

	/**
	 * @class	CollisionGroup
	 *
	 * @brief	A collision group.
	 *
	 * @author	Jeffrey
	 * @date	1/31/2018
	 */

	friend class CollisionGroup;

public:

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