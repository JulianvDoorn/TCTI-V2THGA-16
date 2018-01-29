#pragma once

#include <SFML/Graphics.hpp>
#include "PhysicsObject.hpp"
#include "Collision.hpp"
#include "VectorMath.hpp"

/**
 * @class	Rectangle
 *
 * @brief	A drawable rectangle.
 *
 * @author	Wiebe
 * @date	29-1-2018
 */

class Rectangle : public PhysicsObject, public sf::RectangleShape {
public:

	/**
	 * @fn	Rectangle::Rectangle() : sf::RectangleShape(), PhysicsObject(static_cast<sf::RectangleShape&>(*this))
	 *
	 * @brief	Default constructor
	 *
	 * @author	Wiebe
	 * @date	29-1-2018
	 */

	Rectangle() : sf::RectangleShape(), PhysicsObject(static_cast<sf::RectangleShape&>(*this)) {
		sf::RectangleShape::setFillColor({ 255, 255, 255 });
	}

	/**
	 * @fn	void Rectangle::resize(float resize, ResizeFace face) override
	 *
	 * @brief	Resizes the rectangle
	 *
	 * @author	Wiebe
	 * @date	29-1-2018
	 *
	 * @param	resize	The resize factor.
	 * @param	face  	The resize face.
	 */

	void resize(float resize, ResizeFace face) override {
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
	 * @fn	void Rectangle::setSize(sf::Vector2f size)
	 *
	 * @brief	Sets the size of the rectangle
	 *
	 * @author	Wiebe
	 * @date	29-1-2018
	 *
	 * @param	size	The size.
	 */

	void setSize(sf::Vector2f size) {
		sf::RectangleShape::setOrigin(size.x / 2, size.y / 2);
		sf::RectangleShape::setSize(size);
	}

	/**
	 * @fn	sf::Vector2f Rectangle::getSize() const override
	 *
	 * @brief	Gets the size of the rectangle
	 *
	 * @author	Wiebe
	 * @date	29-1-2018
	 *
	 * @return	The size.
	 */

	sf::Vector2f getSize() const override {
		return sf::RectangleShape::getSize();
	}

	/**
	 * @fn	sf::FloatRect Rectangle::getBounds() override
	 *
	 * @brief	Gets the global bounds of an rectangle
	 *
	 * @author	Wiebe
	 * @date	29-1-2018
	 *
	 * @return	The bounds.
	 */

	sf::FloatRect getBounds() override {
		return getGlobalBounds();
	}

	/**
	 * @fn	bool Rectangle::contains(const sf::Vector2f pos)
	 *
	 * @brief	Query if this object contains the given position
	 *
	 * @author	Wiebe
	 * @date	29-1-2018
	 *
	 * @param	pos	The const sf::Vector2f to test for containment.
	 *
	 * @return	True if the object is in this collection, false if not.
	 */

	bool contains(const sf::Vector2f pos) {
		return sf::RectangleShape::getGlobalBounds().contains(pos);
	}

	/**
	 * @fn	void Rectangle::setTexture(const sf::Texture& texture)
	 *
	 * @brief	Sets a texture
	 *
	 * @author	Wiebe
	 * @date	29-1-2018
	 *
	 * @param	texture	The texture.
	 */

	void setTexture(const sf::Texture& texture) {
		sf::RectangleShape::setTexture(&texture);
	}

	/**
	 * @fn	std::ofstream& Rectangle::writeToFile(std::ofstream& of) override
	 *
	 * @brief	Writes an rectangle to an output file stream
	 *
	 * @author	Wiebe
	 * @date	29-1-2018
	 *
	 * @param [in,out]	of	The outout file stream
	 *
	 * @return	A reference to a std::ofstream.
	 */

	std::ofstream& writeToFile(std::ofstream& of) override {
		of << "rectangle {\n";
		of << " Size = Vector2(" << getSize().x << ", " << getSize().y << ")\n";
		of << " Position = Vector2(" << getPosition().x << ", " << getPosition().y << ")\n";

		if (getTexture() != nullptr) {
			of << " TextureId = \"" << AssetManager::instance()->resolveTextureID(*getTexture()) << "\"\n";
			of << " TextureRect = Rect(" << getTextureRect().left << ", " << getTextureRect().top << ", " << getTextureRect().width << ", " << getTextureRect().height << ")\n";
		}

		of << " Color = #" << std::uppercase << std::hex << getFillColor().toInteger() << "\n";
		of << std::dec;
		of << " CanCollide = true\n";
		of << "}\n";

		return of;
	}

	using PhysicsObject::update;
	using PhysicsObject::setPosition;
	using PhysicsObject::getPosition;
	using PhysicsObject::draw;
};
