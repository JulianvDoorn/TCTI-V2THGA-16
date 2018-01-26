#pragma once

#include <SFML/Graphics.hpp>

class GameObject {
public:

	/**
	 * @fn	virtual sf::Vector2f GameObject::getSize() const = 0;
	 *
	 * @brief	Return the size of this GameObject.
	 *
	 * @author	Julian
	 * @date	2018-01-19
	 *
	 * @return	size
	 */

	virtual sf::Vector2f getSize() const = 0;

	/**
	 * @fn	virtual sf::Vector2f GameObject::getPosition() const = 0;
	 *
	 * @brief	Returns the position of this GameObject.
	 *
	 * @author	Julian
	 * @date	2018-01-19
	 *
	 * @return	position
	 */

	virtual sf::Vector2f getPosition() const = 0;

	/**
	 * @fn	virtual void GameObject::setPosition(const sf::Vector2f& position) = 0;
	 *
	 * @brief	Sets the position of this GameObject.
	 *
	 * @author	Julian
	 * @date	2018-01-19
	 *
	 * @param	position	The position to set this GameObject to.
	 */

	virtual void setPosition(const sf::Vector2f& position) = 0;

	/**
	 * @fn	bool GameObject::operator== (const GameObject& other)
	 *
	 * @brief	Equality operator
	 *
	 * @author	Julian
	 * @date	2018-01-19
	 *
	 * @param	other	Other GameObject to compare with.
	 *
	 * @return	True if this == &other (when lhs is the same object as rhs)
	 */

	bool operator== (const GameObject& other) {
		return this == &other;
	}
};