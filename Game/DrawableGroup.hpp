#pragma once

#include <SFML/Graphics.hpp>
#include "Drawable.hpp"
#include <vector>
#include <algorithm>

class DrawableGroup : public std::vector<Drawable*> {
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
		push_back(&drawable);
	}

	void erase(Drawable& drawable) {
		std::vector<Drawable*>::erase(std::remove(begin(), end(), &drawable), end());
	}

	void draw(sf::RenderTarget& window) {
		for (Drawable* drawable : *this) {
			drawable->draw(window);
		}
	}
};