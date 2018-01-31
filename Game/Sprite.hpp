#pragma once

#include <SFML/Graphics.hpp>

#include "Serializable.hpp"
#include "ClickableRectangle.hpp"

/**
 * @enum	ResizeFace
 *
 * @brief	Values that represent resize faces
 */

enum class ResizeFace {
	Top,
	Bottom,
	Left,
	Right,
	None
};

/**
 * @class	Sprite
 *
 * @brief	A sprite.
 *
 * @author	Jeffrey
 * @date	1/31/2018
 */

class Sprite : public ClickableRectangle, public Serializable {
public:
	void setSize(const sf::Vector2f& size) {
		ClickableRectangle::setOrigin(size / 2.0f);
		ClickableRectangle::setSize(size);
	}

	/**
	* @fn	void Body::resize(float resize, ResizeFace face) override
	*
	* @brief	Resizes the rectangle
	*
	* @author	Wiebe
	* @date	29-1-2018
	*
	* @param	resize	The resize factor.
	* @param	face  	The resize face.
	*/

	void resize(float resize, ResizeFace face) {
		if (face == ResizeFace::Top) {
			sf::Vector2f newSize = getSize() + sf::Vector2f(0, resize);

			if (newSize.y >= 0) {
				setSize(newSize);
				setPosition(getPosition() + sf::Vector2f(0, -resize / 2));
			}
		}
		else if (face == ResizeFace::Bottom) {
			sf::Vector2f newSize = getSize() + sf::Vector2f(0, resize);

			if (newSize.y >= 0) {
				setSize(newSize);
				setPosition(getPosition() + sf::Vector2f(0, resize / 2));
			}
		}
		else if (face == ResizeFace::Left) {
			sf::Vector2f newSize = getSize() + sf::Vector2f(resize, 0);

			if (newSize.x >= 0) {
				setSize(newSize);
				setPosition(getPosition() + sf::Vector2f(-resize / 2, 0));
			}
		}
		else if (face == ResizeFace::Right) {
			sf::Vector2f newSize = getSize() + sf::Vector2f(resize, 0);

			if (newSize.x >= 0) {
				setSize(newSize);
				setPosition(getPosition() + sf::Vector2f(resize / 2, 0));
			}
		}
	}

	/**
	 * @fn	virtual std::ofstream& Sprite::writeToFile(std::ofstream &of) const override
	 *
	 * @brief	Writes to file
	 *
	 * @author	Jeffrey
	 * @date	1/31/2018
	 *
	 * @param [in,out]	of	The of.
	 *
	 * @return	A reference to a std::ofstream.
	 */

	virtual std::ofstream& writeToFile(std::ofstream &of) const override {
		throw "Not implemented";
		return of;
	}
};