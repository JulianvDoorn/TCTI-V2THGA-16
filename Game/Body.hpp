#pragma once

#include <SFML/Graphics.hpp>
#include "VectorMath.hpp"

#include "Sprite.hpp"
#include "Serializable.hpp"
#include "Collidable.hpp"

/**
 * @class	Body
 *
 * @brief	A drawable rectangle.
 *
 * @author	Wiebe
 * @date	29-1-2018
 */

class Body : public virtual Collidable, public Sprite {
	sf::Vector2f gravity = { 0, 981.0f };

protected:
	void onCollide(Collidable& other) override {
		collided.fire(other);
	}

public:
	EventSource<Collidable&> collided;

	/**
	 * @fn	Body::Body() : sf::RectangleShape(), Body(static_cast<sf::RectangleShape&>(*this))
	 *
	 * @brief	Default constructor
	 *
	 * @author	Wiebe
	 * @date	29-1-2018
	 */

	Body() { }

	void setGravity(const sf::Vector2f& gravity) {
		this->gravity = gravity;
	}
	
	sf::Vector2f getGravity() const {
		return gravity;
	}

	virtual void update(const float elapsedType) {
		applyForce(gravity * elapsedType);
		setPosition(getPosition() + velocity * elapsedType);
	}

	void applyForce(const sf::Vector2f& force) {
		velocity += force;
	}

	/**
	 * @fn	std::ofstream& Body::writeToFile(std::ofstream& of) override
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

	virtual std::ofstream& writeToFile(std::ofstream& of) const override {
		of << "rectangle {\n";
		of << " Size = Vector2(" << getSize().x << ", " << getSize().y << ")\n";
		of << " Position = Vector2(" << getPosition().x << ", " << getPosition().y << ")\n";

		if (getTexture() != nullptr) {
			of << " TextureId = \"" << AssetManager::instance()->resolveTextureID(*getTexture()) << "\"\n";

			if (getTexture()->isRepeated()) {
				of << " TextureRect = Rect(" << getTextureRect().left << ", " << getTextureRect().top << ", " << getSize().x << ", " << getSize().y << ")\n";
			}
			else {
				of << " TextureRect = Rect(" << getTextureRect().left << ", " << getTextureRect().top << ", " << getTextureRect().width << ", " << getTextureRect().height << ")\n";
			}

		}

		of << " Color = #" << std::uppercase << std::hex << getFillColor().toInteger() << "\n";
		of << std::dec;
		of << " CanCollide = true\n";
		of << "}\n";

		return of;
	}
};
