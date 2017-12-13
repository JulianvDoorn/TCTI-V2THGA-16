#pragma once

#include <SFML/Graphics.hpp>
#include "Body.hpp"
#include "VectorMath.hpp"

class RectangleBody : public virtual sf::RectangleShape, public Body {
public:
	RectangleBody(sf::Vector2f size) : sf::RectangleShape(size) { }
	
	void updatePhysics() override { }
	
	void resolveCollision(const Body& b) override { }
	
	void draw(sf::RenderWindow& w) override {
		w.draw(*this);
	}
	
	bool checkIntersect(const Body& b) const override {
		return false;
	}
	
	sf::Vector2f getClosestPointTo(const sf::Vector2f v) const override {
	
	}
	
	sf::FloatRect getGlobalBounds() const override {
		return sf::RectangleShape::getGlobalBounds();
	}
	
	sf::Vector2f getPosition() const override {
		return sf::RectangleShape::getPosition() + sf::RectangleShape::getSize() / 2;
	}
	
	void setPosition(sf::Vector2f position) override {
		sf::RectangleShape::setPosition(position - sf::RectangleShape::getSize() / 2);
	}
};