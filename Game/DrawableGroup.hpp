#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>

class DrawableGroup : public std::vector<sf::Drawable*> {
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

	void add(sf::Drawable& drawable) {
		push_back(&drawable);
	}

	void erase(sf::Drawable& drawable) {
		auto it = std::find(std::vector<sf::Drawable*>::begin(), std::vector<sf::Drawable*>::end(), &drawable);

		if (it != end()) {
			std::vector<sf::Drawable*>::erase(it);
		}
	}

	void draw(sf::RenderTarget& window) {
		for (sf::Drawable* drawable : *this) {
			window.draw(*drawable);
		}
	}
};