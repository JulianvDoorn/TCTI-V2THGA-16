#pragma once

#include <SFML/Graphics.hpp>
#include "Rectangle.hpp"
#include "Ball.hpp"
#include "CollisionObjects.hpp"
#include "EventSource.hpp"
#include "Keyboard.hpp"

class Player : public Rectangle {
	sf::RenderWindow& window;
	GameEvents& gameEvents;

	int32_t walkDirection = 0;
	float defaultWalkingSpeed = 50;
    float walkspeed = 50;
	float jumpForce = 500;
    bool spammingShift = false;
    int runningSpammingFactor = 1;
	bool jump = false;
    sf::Time lastKeyPressTime;
    bool shiftpressed = false;
	int deathcase = 0;
	//bool roll = false;
    sf::Clock clock;

public:
	Player(sf::RenderWindow &window, Keyboard &keyboard, GameEvents& gameEvents) : window(window), gameEvents(gameEvents) {
		setSize({ 20, 20 });
		setPosition({ 150, 450 });
		setFillColor(sf::Color(0, 255, 0));

		keyboard.keyPressed.connect([this](const sf::Keyboard::Key key) {
			switch (key) {
                case sf::Keyboard::Key::Space:
			case sf::Keyboard::Key::W:
				doJump();
				break;
                case sf::Keyboard::Key::LShift:
                case sf::Keyboard::Key::RShift:
                    shiftpressed = true;
                    if (clock.getElapsedTime().asMilliseconds() - lastKeyPressTime.asMilliseconds() <200){
                        spammingShift = true;
                        runningSpammingFactor *= 1.5;
                    }
                    else if (spammingShift){
                        spammingShift = false;
                        runningSpammingFactor =1;
                        walkspeed = defaultWalkingSpeed;
                    }
                    walkspeed *= 3 * runningSpammingFactor;
                    if (walkspeed > 500){
                        walkspeed = 500;
                    }
                    lastKeyPressTime = clock.getElapsedTime();
                    break;
			case sf::Keyboard::Key::D:
				walk(walkDirection + 1);
				break;
			case sf::Keyboard::Key::A:
				walk(walkDirection - 1);
				break;
			}
		});

		keyboard.keyReleased.connect([this](const sf::Keyboard::Key key) {
			switch (key) {
            case sf::Keyboard::Key::LShift:
            case sf::Keyboard::Key::RShift:
               shiftpressed = false;
                if (spammingShift){
                   if (clock.restart().asMilliseconds() > 200){
                       spammingShift  = false;
                       walkspeed = defaultWalkingSpeed;
                       runningSpammingFactor = 1;
                   }
               }
               else{
                   walkspeed = 50;
               }
                break;
			case sf::Keyboard::Key::D:
				walk(walkDirection - 1);
				break;
			case sf::Keyboard::Key::A:
				walk(walkDirection + 1);
				break;
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
    if (clock.getElapsedTime().asMilliseconds() -lastKeyPressTime.asMilliseconds() > 250 && shiftpressed ){
        spammingShift = false;
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
		//roll = true;
	}

	void checkDeath() {
		if (getPosition().y > 2000) {
			gameEvents.fellOffMap.fire();
		}
	}

	void detectCollision(CollisionObjects &objects) {
		if (intersects(*(objects.at(0)))) {
			gameEvents.died.fire();
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