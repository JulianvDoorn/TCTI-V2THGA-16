#pragma once

#include <SFML/Graphics.hpp>
#include "VectorMath.hpp"

class Body {
protected:
	bool anchored = false;
	sf::Vector2f velocity = { 0, 0 };
	const sf::Vector2f gravityConstant = { 0, 9.81 };
	float mass;
	int bodyType;
public:
	void setAnchored(bool anchored) {
		this->anchored = anchored;
	}
	
	bool getAnchored() {
		return anchored;
	}
	
	void applyForce(sf::Vector2f force) {
		velocity += force;
	}
	
	virtual void updatePhysics() =0;
	
	virtual void resolveCollision(const Body& b) =0;
	
	virtual sf::Vector2f getClosestPointTo(const sf::Vector2f v) const =0;
	
	virtual void draw(sf::RenderWindow& w) =0;
	
	virtual bool checkIntersect(const Body& b) const =0;
	
	virtual sf::FloatRect getGlobalBounds() const =0;
	
	virtual sf::Vector2f getPosition() const =0;
	
	virtual void setPosition(sf::Vector2f position) =0;
	
	sf::Vector2f getVelocity() const {
		return velocity;
	}
	
	virtual void getNormalFromOffset(sf::Vector2f offset) {};
};