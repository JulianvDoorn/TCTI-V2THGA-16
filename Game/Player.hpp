#pragma once

#include <SFML\Graphics.hpp>
#include "Rectangle.hpp"
#include "Ball.hpp"
#include "CollisionObjects.hpp"

class Player : public Rectangle {
private:
	sf::View &view;
	sf::RenderWindow &window;

	int32_t walkDirection = 0;
	float walkspeed = 50;
	float jumpForce = 500;

	bool jump = false;

	int deathcase = 0;
	//bool roll = false;

public:
	Player(sf::View &view, sf::RenderWindow &window) :
		view(view),
		window(window)
	{
		setSize({ 20, 20 });
		setPosition({ 150, 450 });
		setFillColor(sf::Color(0, 255, 0));
	}
	void update(const float elapsedTime) override {
		PhysicsObject::update(elapsedTime);

		view.setCenter(getPosition());
		window.setView(view);

		if (walkDirection != 0) {
			setVelocity({ walkDirection * walkspeed, getVelocity().y });
		} else {
			setVelocity({ 0, getVelocity().y });
		}


		if (jump) {
			applyForce({ 0, -jumpForce });
			jump = false;
		}

		//if (roll) {
		//roll
		//}

		checkDeath();

	}


	void walk(int32_t direction) {
		walkDirection = direction;
	}

	void doJump() {
		sf::Vector2f velocity = getVelocity();
		//if ((velocity.y > 16 && velocity.y < 17) || velocity.y == 0) {
		if (velocity.y == 0) {
			jump = true;
		}
	}

	void doRoll() {
		//roll = true;
	}

	void death() {
		switch (deathcase) {
			case 0:
				break;
			case 1:
				std::cout << "/!\\ fell out of the world /!\\ " << std::endl;
				break;
			case 2:
				std::cout << "/!\\ death got you /!\\ " << std::endl;
				break;
		}
		if (deathcase != 0) {
			sf::sleep(sf::milliseconds(5000));
			window.close();
		}
	}

	void checkDeath() {
		if (getPosition().y > 2000) {
			deathcase = 1;
			death();
		}
	}

	void detectCollision(CollisionObjects &objects) {
		std::cout << objects.getSize() << std::endl;
		if (intersects(*(objects.at(0)))) {
			deathcase = 2;
			death();
		}
		for (unsigned int i = 0; i < objects.getSize(); i++) {
			PhysicsObject* object = objects.at(i);
			if (intersects(*object)) {
				resolveCollision(*object);
			}
		}
	}

	sf::FloatRect getBounds() override {
		return getGlobalBounds();
	}

	using Rectangle::getCollision;
	using Rectangle::setPosition;
	using Rectangle::getPosition;
	using Rectangle::setSize;
	using Rectangle::draw;
	using PhysicsObject::intersects;
};