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
		return "KeyScheme with given difficulty cannot been found!";
	}
};

class Player : public Rectangle {
private:
	KeyScheme activeKeyScheme = KeyScheme(sf::Keyboard::Key::A, sf::Keyboard::Key::D, sf::Keyboard::Key::W, sf::Keyboard::Key::S,sf::Keyboard::LShift);
	KeySchemes keySchemes = {
		KeyScheme(sf::Keyboard::Key::D, sf::Keyboard::Key::A, sf::Keyboard::Key::S, sf::Keyboard::Key::W, sf::Keyboard::Key::RShift ,KeyScheme::Difficulty::MODERATE),
		KeyScheme(sf::Keyboard::Key::J, sf::Keyboard::Key::L, sf::Keyboard::Key::I, sf::Keyboard::Key::J, sf::Keyboard::Key::LShift, KeyScheme::Difficulty::MODERATE)
	};

	int32_t walkDirection = 0;
	float defaultWalkingSpeed = 50;
    float walkspeed = 50;
	float jumpForce = 500;
    bool spammingRunKey = false;
    float runningSpammingFactor = 1;
	bool jump = false;
	bool roll = false;
    sf::Time lastKeyPressTime;
    bool runKeyPressed = false;
	int deathcase = 0;
	//bool roll = false;
    sf::Clock clock;

	EventConnection<sf::Keyboard::Key> keyPressedConn;
	EventConnection<sf::Keyboard::Key> keyReleasedConn;

public:
	Player() {
		setSize({ 20, 40 });
		setFillColor(sf::Color(0, 255, 0));

		keyPressedConn = game.keyboard.keyPressed.connect([this](const sf::Keyboard::Key key) {
			if (key == activeKeyScheme.jump) {
				doJump();
			}
			else if (key == activeKeyScheme.run){
				runKeyPressed = true;
				if (clock.getElapsedTime().asMilliseconds() - lastKeyPressTime.asMilliseconds() <200){
					spammingRunKey = true;
					runningSpammingFactor *= 1.5;
				}
				else if (spammingRunKey){
					spammingRunKey = false;
					runningSpammingFactor =1;
					walkspeed = defaultWalkingSpeed;
				}
				walkspeed *= 3 * runningSpammingFactor;
				if (walkspeed > 500){
					walkspeed = 500;
				}
				lastKeyPressTime = clock.getElapsedTime();
			}
			else if (key == activeKeyScheme.moveLeft) {
				walk(walkDirection - 1);
			}
			else if (key == activeKeyScheme.moveRight) {
				walk(walkDirection + 1);
			}
			else if (key == activeKeyScheme.roll) {
				doRoll();
				clock.restart();
			}
		});

		keyReleasedConn = game.keyboard.keyReleased.connect([this](const sf::Keyboard::Key key) {
			if (key == activeKeyScheme.moveLeft) {
				walk(walkDirection + 1);
			}
			else if (key ==activeKeyScheme.run){
				runKeyPressed = false;
				if (spammingRunKey){
					if (clock.restart().asMilliseconds() > 200){
						walkspeed = defaultWalkingSpeed;
						runningSpammingFactor = 1;
					}
				}
				else{
					walkspeed = 50;
				}
			}
			else if (key == activeKeyScheme.moveRight) {
				walk(walkDirection - 1);
			}
		});
	}

	~Player() {
		keyPressedConn.disconnect();
		keyReleasedConn.disconnect();
	}

	void update(const float elapsedTime) override {
		PhysicsObject::update(elapsedTime);
		
		if (walkDirection != 0) {
			if (!roll) {
				setVelocity({ walkDirection * walkspeed, getVelocity().y });
			}
		} else {
			if (!roll) {
				setVelocity({ 0, getVelocity().y });
			}
		}

		if (jump) {
			applyForce({ 0, -jumpForce });
			jump = false;
		}
		if (roll) {
			if (getVelocity().x > 0) {
				applyForce({ 1, jumpForce });
			}
			else if(getVelocity().x < 0) {
				applyForce({ -1, jumpForce });
			}
			else {
				applyForce({ 0, jumpForce });
			}
			setSize({ 20, 20 });
			roll = true;
			if (((clock.getElapsedTime().asSeconds()) > 2) || getVelocity().x == 0) {
				setSize({ 20, 40 });
				roll = false;
			}
		}
		checkDeath();
        if (clock.getElapsedTime().asMilliseconds() -lastKeyPressTime.asMilliseconds() > 250 && !runKeyPressed ){
            spammingRunKey = false;
            walkspeed = defaultWalkingSpeed;
        }
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
		roll = true;
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