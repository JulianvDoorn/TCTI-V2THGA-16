#pragma once

#include <SFML/Graphics.hpp>
#include "Body.hpp"
#include "VectorMath.hpp"

#include <iostream>

const float pi = 3.14159265358979323846f;

std::ostream& operator<< (std::ostream& os, sf::FloatRect r) {
	return os << "Width: " << r.width << " Height: " << r.height << " : Left: " << r.left << " Top: " << r.top << std::endl;
}

class BallBody : public sf::CircleShape, public Body {
public:
	BallBody(float radius) : sf::CircleShape(radius) {
		mass = pi * radius * radius;
	}
	
	void updatePhysics() override {
		if (!anchored) {
			velocity += gravityConstant;
			setPosition(getPosition() + velocity / 30);
		}
	}
	
	void resolveCollision(const Body& b) override {
		if (!anchored) {
			sf::Vector2f v0 = b.getClosestPointTo(getPosition()); // contact point on b's surface
			
			if (magnitude(v0 - getPosition()) < getRadius()) {
				sf::Vector2f mtv = b.getClosestPointTo(getPosition()) - getClosestPointTo(b.getPosition()); // minimum translation vector to counter penetration
				move(mtv);
				
				sf::Vector2f v1 = getClosestPointTo(b.getPosition()) - getPosition(); // contact point on this's surface relative to getPosition()
				sf::Vector2f n1 = normalize(v0 - b.getPosition()); // magnitude == 1, normal vector of b's surface
				
				if (dot(velocity, v1) > 0) {
					applyForce(n1 * (magnitude(velocity) * 1.5));
				}
			}
		}
	}
	
	sf::Vector2f getClosestPointTo(const sf::Vector2f v) const override {
		return normalize(v - getPosition()) * getRadius() + getPosition();
	}
	
	void draw(sf::RenderWindow& w) override {
		w.draw(*this);
	}
	
	bool checkIntersect(const Body& b) const override {
		return b.getGlobalBounds().intersects(getGlobalBounds());
	}
	
	sf::FloatRect getGlobalBounds() const override {
		return sf::CircleShape::getGlobalBounds();
	}
	
	sf::Vector2f getPosition() const override {
		return sf::CircleShape::getPosition() + sf::Vector2f(getRadius(), getRadius());
	}
	
	void setPosition(sf::Vector2f position) override {
		sf::CircleShape::setPosition(position - sf::Vector2f(getRadius(), getRadius()));
	}
};