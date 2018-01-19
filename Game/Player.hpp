#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "Rectangle.hpp"
#include "Ball.hpp"
#include "CollisionObjects.hpp"
#include "EventSource.hpp"
#include "Keyboard.hpp"
#include "KeyScheme.hpp"

class Player : public Rectangle {
private:
	sf::View &view;
	sf::RenderWindow &window;
	KeyScheme activeScheme;
	std::vector<KeyScheme*> keySchemes;

	int32_t walkDirection = 0;
	float walkspeed = 50;
	float jumpForce = 500;

	bool jump = false;

	int deathcase = 0;
	//bool roll = false;

	sf::Keyboard::Key keyJump = sf::Keyboard::Key::W;
	sf::Keyboard::Key keyRoll = sf::Keyboard::Key::S;
	sf::Keyboard::Key keyMoveLeft = sf::Keyboard::Key::A;
	sf::Keyboard::Key keyMoveRight = sf::Keyboard::Key::D;

public:
	Player(sf::View &view, sf::RenderWindow &window, Keyboard &keyboard) :
		view(view),
		window(window)
	{
		setSize({ 20, 20 });
		setPosition({ 150, 450 });
		setFillColor(sf::Color(0, 255, 0));



		keyboard.keyPressed.connect([this](const sf::Keyboard::Key key) {
			if (key == keyJump) {
				doJump();
			}
			else if (key == keyMoveLeft) {
				walk(walkDirection - 1);
			}
			else if (key == keyMoveRight) {
				walk(walkDirection + 1);
			}
		});

		keyboard.keyReleased.connect([this](const sf::Keyboard::Key key) {
			if (key == keyMoveLeft) {
				walk(walkDirection + 1);
			}
			else if (key == keyMoveRight) {
				walk(walkDirection - 1);
			}
		});
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
		checkDeath();

	}

	void walk(int32_t direction) {
		walkDirection = direction;
	}

	void doJump() {
		sf::Vector2f velocity = getVelocity();
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
				keyMoveLeft = sf::Keyboard::D;
				keyMoveRight = sf::Keyboard::A;
				//std::cout << "/!\\ death got you /!\\ " << std::endl;
				break;
		}
		//if (deathcase != 0) {
		if (deathcase == 1) {
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

	void addKeyScheme(KeyScheme &s) {
		schemes.push_back(&s);
	}

	void findKeyScheme(KeyScheme::Difficulty difficulty) {
		std::vector<KeyScheme*>::iterator it = std::find_if(keySchemes.begin(), keySchemes.end(), [difficulty](const KeyScheme* scheme) { scheme->difficulty == difficulty; });

		
	}

	void setActiveKeyScheme(KeyScheme &s) {
		activeScheme = s;
	}

	using Rectangle::getCollision;
	using Rectangle::setPosition;
	using Rectangle::getPosition;
	using Rectangle::setSize;
	using Rectangle::draw;
	using PhysicsObject::intersects;
};