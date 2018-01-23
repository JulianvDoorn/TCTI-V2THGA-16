#pragma once

#include <SFML/Graphics.hpp>
#include "Drawable.hpp"
#include <vector>
#include <algorithm>

class DrawableGroup {

	/** @brief	The other collidables to compare with the primary collidable */
	std::vector<Drawable*> drawableGroup;

public:
	/**
	* @fn	void CollisionGroup::add(Collidable& collidable)
	*
	* @brief	Adds collidable to the vector of collidables to compare with primaryCollidable.
	*
	* @author	Julian
	* @date	2018-01-19
	*
	* @param [in,out]	collidable	The collidable to add.
	*/

	void add(Drawable& drawable) {
		drawableGroup.push_back(&drawable);
	}

	void erase(Drawable& drawable) {
		drawableGroup.erase(std::remove(drawableGroup.begin(), drawableGroup.end(), &drawable), drawableGroup.end());
	}

	void draw(sf::RenderTarget& window) {
		for (Drawable* drawable : drawableGroup) {
			drawable->draw(window);
		}
	}
};