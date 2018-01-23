#pragma once

#include <SFML/Graphics.hpp>
#include "PhysicsObject.hpp"
#include "Collision.hpp"

class Rectangle : public PhysicsObject, public sf::RectangleShape {
public:
	Rectangle() : sf::RectangleShape(), PhysicsObject(static_cast<sf::RectangleShape&>(*this)) {
		sf::RectangleShape::setFillColor({ 255, 255, 255 });
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

	using PhysicsObject::update;
	using PhysicsObject::setPosition;
	using PhysicsObject::getPosition;
	using PhysicsObject::draw;
};
