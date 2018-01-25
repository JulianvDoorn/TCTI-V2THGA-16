#pragma once

#include <SFML/Graphics.hpp>

#include "Drawable.hpp"
#include "CollisionDetection.hpp"
#include "Events.hpp"

class PhysicsObject : public Drawable, public Collidable {
	EventConnection<sf::Vector2i> masterMouseLeftButtonDown;
	EventConnection<sf::Vector2i> masterMouseLeftButtonUp;
	EventConnection<sf::Vector2i> masterMouseRightButtonDown;
	EventConnection<sf::Vector2i> masterMouseRightButtonUp;

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

	EventSource<> mouseLeftButtonDown;
	EventSource<> mouseLeftButtonUp;
	EventSource<> mouseRightButtonDown;
	EventSource<> mouseRightButtonUp;
	
	void bindMouseEvents() {
		masterMouseLeftButtonDown = game.mouse.mouseLeftButtonDown.connect([this](sf::Vector2i pos) {
			if (getBounds().contains(game.window->mapPixelToCoords(pos))) {
				mouseLeftButtonDown.fire();
			}
		});

		masterMouseRightButtonDown = game.mouse.mouseRightButtonDown.connect([this](sf::Vector2i pos) {
			if (getBounds().contains(game.window->mapPixelToCoords(pos))) {
				mouseRightButtonDown.fire();
			}
		});

		masterMouseLeftButtonUp = game.mouse.mouseLeftButtonUp.connect([this](sf::Vector2i pos) {
			if (getBounds().contains(game.window->mapPixelToCoords(pos))) {
				mouseLeftButtonUp.fire();
			}
		});

		masterMouseRightButtonUp = game.mouse.mouseRightButtonUp.connect([this](sf::Vector2i pos) {
			if (getBounds().contains(game.window->mapPixelToCoords(pos))) {
				mouseRightButtonUp.fire();
			}
		});
	}

	void unbindMouseEvents() {
		masterMouseLeftButtonDown.disconnect();
		masterMouseLeftButtonUp.disconnect();
		masterMouseRightButtonDown.disconnect();
		masterMouseRightButtonUp.disconnect();
	}

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