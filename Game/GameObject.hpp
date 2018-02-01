#pragma once

#include <SFML/Graphics.hpp>

/**
 * @class	GameObject
 *
 * @brief	A game object.
 *
 * @author	Jeffrey
 * @date	1/31/2018
 */

class GameObject {
public:

	/**
	 * @fn	virtual void GameObject::update(const float elapsedTime) = 0;
	 *
	 * @brief	Updates the gameobject using elapsedTime
	 *
	 * @author	Jeffrey
	 * @date	1/31/2018
	 *
	 * @param	elapsedTime	The elapsed time.
	 */

	virtual void update(const float elapsedTime) = 0;
};