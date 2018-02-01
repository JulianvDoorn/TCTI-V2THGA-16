#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>

/**
 * @class	DrawableGroup
 *
 * @brief	A drawable group.
 *
 * @author	Jeffrey
 * @date	2/1/2018
 */

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

	/**
	 * @fn	void DrawableGroup::erase(sf::Drawable& drawable)
	 *
	 * @brief	Erases the given drawable
	 *
	 * @author	Jeffrey
	 * @date	1/31/2018
	 *
	 * @param [in,out]	drawable	The drawable.
	 */

	void erase(sf::Drawable& drawable) {
		auto it = std::find(std::vector<sf::Drawable*>::begin(), std::vector<sf::Drawable*>::end(), &drawable);

		if (it != end()) {
			std::vector<sf::Drawable*>::erase(it);
		}
	}

	/**
	 * @fn	void DrawableGroup::draw(sf::RenderTarget& window)
	 *
	 * @brief	Draws the given window
	 *
	 * @author	Jeffrey
	 * @date	1/31/2018
	 *
	 * @param [in,out]	window	The window.
	 */

	void draw(sf::RenderTarget& window) {
		for (sf::Drawable* drawable : *this) {
			window.draw(*drawable);
		}
	}
};