#pragma once

#include <SFML\Graphics.hpp>
#include "Rectangle.hpp"

class Player : public Rectangle {
private:
	sf::View &view;
	sf::RenderWindow &window;

	int32_t walkDirection = 0;
	float walkspeed = 50;
	float jumpForce = 500;

	bool jump = false;
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

		death();

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
		if (getPosition().y > 2000) {
			std::cout << "/!\\ fell out of the world /!\\ " << std::endl;
			sf::sleep(sf::milliseconds(5000));
			window.close();
		}
	}

	sf::FloatRect getBounds() override {
		return getGlobalBounds();
	}

	using Rectangle::setPosition;
	using Rectangle::getPosition;
	using Rectangle::setSize;
	using Rectangle::draw;
};