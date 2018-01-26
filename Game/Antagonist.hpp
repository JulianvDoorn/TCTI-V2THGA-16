#pragma once

#include <SFML/Graphics.hpp>
#include "Rectangle.hpp"

/**
 * @class	Antagonist
 *
 * @brief	An displayable antagonist.
 *
 * @author	Wiebe
 * @date	25-1-2018
 */

class Antagonist : public Rectangle {
private:
	/** @brief	The walk direction */
	int32_t walkDirection = 1;

	/** @brief	The walkspeed */
	float walkspeed = 50;

public:

	/**
	 * @fn	Antagonist::Antagonist()
	 *
	 * @brief	Default constructor
	 *
	 * @author	Wiebe
	 * @date	25-1-2018
	 */

	Antagonist() {
		setSize({ 300, 400 });
		setVelocity({ walkDirection * walkspeed, getVelocity().y });
		setGravity({ 0, 0 });
	}

	/**
	 * @fn	sf::FloatRect Antagonist::getBounds() override
	 *
	 * @brief	Gets the bounds
	 *
	 * @author	Wiebe
	 * @date	25-1-2018
	 *
	 * @return	The bounds.
	 */

	sf::FloatRect getBounds() override {
		return getGlobalBounds();
	}

	using Rectangle::setPosition;
	using Rectangle::getPosition;
	using Rectangle::setSize;
	using Rectangle::draw;
};