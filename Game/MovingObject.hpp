#pragma once

#include <SFML/Graphics.hpp>

/**
 * @class	MovingObject
 *
 * @brief	A moving object.
 *
 * @author	Jeffrey
 * @date	1/31/2018
 */

class MovingObject {
protected:
	/** @brief	The velocity */
	sf::Vector2f velocity;

public:

	/**
	 * @fn	sf::Vector2f MovingObject::getVelocity() const
	 *
	 * @brief	Gets the velocity of the movingobject
	 *
	 * @author	Jeffrey
	 * @date	1/31/2018
	 *
	 * @return	The velocity.
	 */

	sf::Vector2f getVelocity() const {
		return velocity;
	}

	/**
	 * @fn	void MovingObject::setVelocity(const sf::Vector2f& velocity)
	 *
	 * @brief	Sets a velocity
	 *
	 * @author	Jeffrey
	 * @date	1/31/2018
	 *
	 * @param	velocity	The velocity.
	 */

	void setVelocity(const sf::Vector2f& velocity) {
		this->velocity = velocity;
	}
};