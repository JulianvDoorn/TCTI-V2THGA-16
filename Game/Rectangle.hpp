#pragma once

#include <SFML/Graphics.hpp>
#include "PhysicsObject.hpp"
#include "Collision.hpp"
#include "VectorMath.hpp"

class Rectangle : public PhysicsObject, public sf::RectangleShape {
public:
	Rectangle() : sf::RectangleShape(), PhysicsObject(static_cast<sf::RectangleShape&>(*this)) {
		sf::RectangleShape::setFillColor({ 255, 255, 255 });
	}

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

	void setSize(sf::Vector2f size) {
		sf::RectangleShape::setOrigin(size.x / 2, size.y / 2);
		sf::RectangleShape::setSize(size);
	}

	sf::Vector2f getSize() const override {
		return sf::RectangleShape::getSize();
	}

	sf::FloatRect getBounds() override {
		return getGlobalBounds();
	}

	bool contains(const sf::Vector2f pos) {
		return sf::RectangleShape::getGlobalBounds().contains(pos);
	}

	void setTexture(const sf::Texture& texture) {
		sf::RectangleShape::setTexture(&texture);
	}

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
