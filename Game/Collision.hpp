#pragma once

#include <SFML/Graphics.hpp>

class Collision {
private:
	sf::Vector2f delta;
	sf::Vector2f intersect;

public:
	/**
	* @brief ADT holding collision information.
	* @param delta Delta between two objects.
	* @param intersect Intersection vector between two objects.
	*/
	Collision(const sf::Vector2f delta, const sf::Vector2f intersect) :
		delta(delta),
		intersect(intersect)
	{};

	/**
	* @brief Check if there has been a collision.
	* @return Result.
	*/
	bool check() {
		if (getIntersect().x < 0.0f && getIntersect().y < 0.0f) {
			return true;
		}

		return false;
	}

	/**
	* @brief Get collision delta between two objects.
	* @return Delta.
	*/
	sf::Vector2f getDelta() const { return delta; };

	/**
	* @brief Get intersect between two objects.
	* @return Intersect.
	*/
	sf::Vector2f getIntersect() const { return intersect; };
};