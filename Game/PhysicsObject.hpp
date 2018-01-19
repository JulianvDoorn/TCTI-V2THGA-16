#pragma once

#include <SFML/Graphics.hpp>

#include "Drawable.hpp"
#include "CollisionDetection.hpp"
#include "Events.hpp"

class PhysicsObject : public Drawable, public Collidable {
protected:
	sf::Transformable& transformable;
	sf::Vector2f velocity = sf::Vector2f(0, 0);
	sf::Vector2f gravity = sf::Vector2f(0, 981.0f);

	PhysicsObject(sf::Shape& shape) : Drawable(shape), transformable(shape) { }
	PhysicsObject(sf::Sprite& sprite) : Drawable(sprite), transformable(sprite) { }

	bool sideCollision;

	void onCollide(Collidable& other) override {
		collided.fire(other);
	}

public:
	EventSource<Collidable&> collided;

	virtual void update(const float elapsedTime) {
		applyForce(gravity * elapsedTime);
		setPosition(getPosition() + velocity * elapsedTime);
	}

	virtual void setGravity(const sf::Vector2f& gravity) {
		this->gravity = gravity;
	}

	virtual void setPosition(const sf::Vector2f& position) {
		transformable.setPosition(position);
	}
	
	virtual sf::Vector2f getSize() const = 0;

	virtual sf::Vector2f getPosition() const {
		return transformable.getPosition();
	}

	virtual sf::FloatRect getBounds() = 0;

	virtual void applyForce(const sf::Vector2f& force) {
		velocity += force;
	}

	virtual void setVelocity(const sf::Vector2f& velocity) {
		this->velocity = velocity;
	}

	virtual sf::Vector2f getVelocity() const {
		return velocity;
	}
};