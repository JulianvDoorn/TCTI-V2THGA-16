#pragma once

#include <vector>
#include <exception>
#include <memory>
#include <SFML/Graphics.hpp>
#include "Rectangle.hpp"
#include "Ball.hpp"
#include "EventSource.hpp"
#include "Keyboard.hpp"
#include "KeyScheme.hpp"

using KeySchemes = std::array<KeyScheme, 100>;

class KeySchemeNotFoundException : public std::exception {
public:
	const char* what() const noexcept {
		return "KeyScheme with given difficuly cannot been found!";
	}
};

class Player : public Rectangle {
private:
	sf::RenderWindow &window;
	KeyScheme activeKeyScheme = KeyScheme(sf::Keyboard::Key::A, sf::Keyboard::Key::D, sf::Keyboard::Key::W, sf::Keyboard::Key::S);
	KeySchemes keySchemes = {
		KeyScheme(sf::Keyboard::Key::D, sf::Keyboard::Key::A, sf::Keyboard::Key::S, sf::Keyboard::Key::W, KeyScheme::Difficulty::MODERATE),
		KeyScheme(sf::Keyboard::Key::J, sf::Keyboard::Key::L, sf::Keyboard::Key::I, sf::Keyboard::Key::J, KeyScheme::Difficulty::MODERATE)
	};

	int32_t walkDirection = 0;
	float walkspeed = 50;
	float jumpForce = 500;

	bool jump = false;

	int deathcase = 0;

public:
	Player(sf::RenderWindow &window) : window(window) {
		setSize({ 20, 20 });
		setPosition({ 150, 450 });
		setFillColor(sf::Color(0, 255, 0));

		game.keyboard.keyPressed.connect([this](const sf::Keyboard::Key key) {
			if (key == activeKeyScheme.jump) {
				doJump();
			}
			else if (key == activeKeyScheme.moveLeft) {
				walk(walkDirection - 1);
			}
			else if (key == activeKeyScheme.moveRight) {
				walk(walkDirection + 1);
			}
		});

		game.keyboard.keyReleased.connect([this](const sf::Keyboard::Key key) {
			if (key == activeKeyScheme.moveLeft) {
				walk(walkDirection + 1);
			}
			else if (key == activeKeyScheme.moveRight) {
				walk(walkDirection - 1);
			}
		});
	}

	void update(const float elapsedTime) override {
		PhysicsObject::update(elapsedTime);

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

	void checkDeath() {
		if (getPosition().y > 2000) {
			game.fellOffMap.fire();
		}
	}


	sf::FloatRect getBounds() override {
		return getGlobalBounds();
	}

	KeyScheme findKeyScheme(const KeyScheme::Difficulty difficulty) {
		std::vector<KeyScheme*> schemes;

		for (unsigned int i = 0; i < keySchemes.size(); i++) {
			if (keySchemes[i].difficulty == difficulty) {
				schemes.push_back(&keySchemes.at(i));
			}
		}

		if (!schemes.size()) {
			throw KeySchemeNotFoundException();
		}

		std::random_shuffle(schemes.begin(), schemes.end());

		return *schemes.at(0);
	}

	void setActiveKeyScheme(KeyScheme s) {
		activeKeyScheme = s;

		std::cout << "Controls: " << std::endl;
		std::cout << "Jump: " << s.jump << std::endl;
		std::cout << "Roll: " << s.roll << std::endl;
		std::cout << "Move left: " << s.moveLeft << std::endl;
		std::cout << "Move right: " << s.moveRight << std::endl;
	}

	KeyScheme& getActiveKeyScheme() {
		return activeKeyScheme;
	}

	using Rectangle::getCollision;
	using Rectangle::setPosition;
	using Rectangle::getPosition;
	using Rectangle::setSize;
	using Rectangle::draw;
};