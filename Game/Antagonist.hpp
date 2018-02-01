#pragma once

#include <SFML/Graphics.hpp>
#include "Body.hpp"

/**
 * @class	Antagonist
 *
 * @brief	An displayable antagonist.
 *
 * @author	Wiebe
 * @date	25-1-2018
 */

class Antagonist : public Body {
	/** @brief	The walk direction */
	int32_t walkDirection = 1;

	/** @brief	The walkspeed */
	float walkspeed = 40;

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
		setSize({ 200, 400 });
		setVelocity({ walkDirection * walkspeed, getVelocity().y });
		setGravity({ 0, 0 });
	}
};