#pragma once

#include <SFML/Graphics.hpp>

#include "GameObject.hpp"

class Collision {
private:

	/** @brief	Difference between the positions of two GameObjects */
	sf::Vector2f delta;

	/** @brief	Intersection field between the two GameObjects */
	sf::Vector2f intersect;

public:

	/**
	 * @fn	Collision::Collision(const GameObject& lhs, const GameObject& rhs)
	 *
	 * @brief	Constructor
	 *
	 * @author	Julian
	 * @date	2018-01-19
	 *
	 * @param	lhs	GameObject1 to calculate a collision for relative to GameObject2
	 * @param	rhs	GameObject2 to calculate a collision for relative to GameObject1
	 */

	Collision(const GameObject& lhs, const GameObject& rhs) {
		sf::Vector2f otherPosition = rhs.getPosition();
		sf::Vector2f otherHalfSize = rhs.getSize() / 2.0f;
		sf::Vector2f thisPosition = lhs.getPosition();
		sf::Vector2f thisHalfSize = lhs.getSize() / 2.0f;

		delta = { otherPosition.x - thisPosition.x, otherPosition.y - thisPosition.y };
		intersect = { abs(delta.x) - (otherHalfSize.x + thisHalfSize.x), abs(delta.y) - (otherHalfSize.y + thisHalfSize.y) };
	};

	/**
	 * @fn	Collision::Collision(const sf::Vector2f delta, const sf::Vector2f intersect)
	 *
	 * @brief	ADT holding collision information.
	 *
	 * @author	Julian
	 * @date	2018-01-19
	 *
	 * @param	delta	 	Delta between two objects.
	 * @param	intersect	Intersection vector between two objects.
	 */

	Collision(const sf::Vector2f delta, const sf::Vector2f intersect) :
		delta(delta),
		intersect(intersect)
	{ };

	/**
	 * @fn	bool Collision::intersects()
	 *
	 * @brief	Calculate if there this collision's collision area is positive
	 *
	 * @author	Julian
	 * @date	2018-01-19
	 *
	 * @return	True when the two objects intersect and false when they do not
	 */

	bool intersects() {
		if (getIntersect().x < 0.0f && getIntersect().y < 0.0f) {
			return true;
		}

		return false;
	}

	/**
	 * @fn	sf::Vector2f Collision::getDelta() const
	 *
	 * @brief	Get collision delta between two objects.
	 *
	 * @author	Julian
	 * @date	2018-01-19
	 *
	 * @return	Delta position between the two objects.
	 */

	sf::Vector2f getDelta() const { return delta; };

	/**
	 * @fn	sf::Vector2f Collision::getIntersect() const
	 *
	 * @brief	Get intersection between two objects.
	 *
	 * @author	Julian
	 * @date	2018-01-19
	 *
	 * @return	Intersection area
	 */

	sf::Vector2f getIntersect() const { return intersect; };
};