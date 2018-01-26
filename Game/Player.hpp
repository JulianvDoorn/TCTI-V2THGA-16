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

/** @brief	An (fixed-size) array holding key schemes. Maximum of 100 key schemes. */
using KeySchemes = std::array<KeyScheme, 100>;

/**
 * @class	Player
 *
 * @brief	A displayable player.
 *
 * @author	Wiebe
 * @date	25-1-2018
 */

class Player : public Rectangle {
private:
    sf::RenderWindow &window;

	/** @brief	The current active key scheme */
	KeyScheme activeKeyScheme = KeyScheme(sf::Keyboard::Key::A, sf::Keyboard::Key::D, sf::Keyboard::Key::W, sf::Keyboard::Key::S,sf::Keyboard::LShift);

	/** @brief	The predefined player key schemes */
	KeySchemes keySchemes = {
		KeyScheme(sf::Keyboard::Key::D, sf::Keyboard::Key::A, sf::Keyboard::Key::S, sf::Keyboard::Key::W, sf::Keyboard::Key::RShift ,KeyScheme::Difficulty::MODERATE),
		KeyScheme(sf::Keyboard::Key::J, sf::Keyboard::Key::L, sf::Keyboard::Key::I, sf::Keyboard::Key::J, sf::Keyboard::Key::LShift, KeyScheme::Difficulty::MODERATE)
	};


	int32_t walkDirection = 0;

	float defaultWalkingSpeed = 100;
    float walkspeed = 100;
	float jumpForce = 500;
    bool spammingRunKey = false;
    float runningSpammingFactor = 1;
	bool jump = false;
	bool roll = false;
    sf::Time lastKeyPressTime;
    bool runKeyPressed = false;
	int deathcase = 0;

    sf::Clock runClock;
	sf::Clock rollClock;

	EventConnection<sf::Keyboard::Key> keyPressedConn;
	EventConnection<sf::Keyboard::Key> keyReleasedConn;

 /** @brief These variable are used to set a bodypart to display **/
    bool torsoDisplay = true;
    bool leftLegDisplay = true;
    bool rightLegDisplay = true;
    bool headDisplay = true;
    bool leftArmDisplay = true;
    bool rightArmDisplay = true;

    Rectangle torso;
    Rectangle leftLeg;
    Rectangle rightLeg;
    Rectangle head;
    Rectangle leftArm;
    Rectangle rightArm;
    Rectangle rollRectangle;

    int defaultAnimationTimeInMiliseconds = 50;
    int animationTimeInMiliseconds = defaultAnimationTimeInMiliseconds;
	int animationCyle = 0;
    sf::Clock animationClock;

public:

	/**
	 * @fn	Player::Player()
	 *
	 * @brief	Default constructor
	 *
	 * @author	Wiebe
	 * @date	25-1-2018
	 */
	Player(sf::RenderWindow &window) : window(window){
		sf::Vector2f playersize = {20,40};
        setSize(playersize);

        setFillColor(sf::Color::Transparent);
        torso.setSize(playersize);
        head.setSize(playersize);
        leftLeg.setSize(playersize);
        rightLeg.setSize(playersize);
        leftArm.setSize(playersize);
        rightArm.setSize(playersize);
        rollRectangle.setSize({20,20});

		keyPressedConn = game.keyboard.keyPressed.connect([this](const sf::Keyboard::Key key) {
			if (key == activeKeyScheme.jump) {
				doJump();
			}
			else if (key == activeKeyScheme.roll) {
                roll = true;
				rollClock.restart();
			}
			else if (key == activeKeyScheme.run){
				runKeyPressed = true;
				if (runClock.getElapsedTime().asMilliseconds() - lastKeyPressTime.asMilliseconds() <200){
					spammingRunKey = true;
					runningSpammingFactor *= 1.5;
				}
				else if (spammingRunKey){
					spammingRunKey = false;
					runningSpammingFactor =1;
					walkspeed = defaultWalkingSpeed;
				}
				walkspeed *= 2 * runningSpammingFactor;
                animationTimeInMiliseconds /= 2;
                animationTimeInMiliseconds /= runningSpammingFactor;
				if (walkspeed > 299){ //max running speed without glitching
					walkspeed = 299;
				}
				lastKeyPressTime = runClock.getElapsedTime();
			}
			else if (key == activeKeyScheme.moveLeft) {
				setWalkDirection(walkDirection - 1);
			}
			else if (key == activeKeyScheme.moveRight) {
				setWalkDirection(walkDirection + 1);
			}
		});

		keyReleasedConn = game.keyboard.keyReleased.connect([this](const sf::Keyboard::Key key) {
			if (key == activeKeyScheme.moveLeft) {
				setWalkDirection(walkDirection + 1);
			}
			else if (key ==activeKeyScheme.run){
				runKeyPressed = false;
				if (spammingRunKey){
					if (runClock.restart().asMilliseconds() > 200){
						walkspeed = defaultWalkingSpeed;
						runningSpammingFactor = 1;
						animationTimeInMiliseconds = defaultAnimationTimeInMiliseconds;
					}
				}
				else{
					walkspeed = 100;
                    animationTimeInMiliseconds = defaultAnimationTimeInMiliseconds;
				}
			}
			else if (key == activeKeyScheme.moveRight) {
				setWalkDirection(walkDirection - 1);
			}
		});
	}

	/**
	 * @fn	Player::~Player()
	 *
	 * @brief	Destructor
	 *
	 * @author	Wiebe
	 * @date	25-1-2018
	 */

	~Player() {
		keyPressedConn.disconnect();
		keyReleasedConn.disconnect();
	}

	/**
	 * @fn	void Player::update(const float elapsedTime) override
	 *
	 * @brief	Updates the window with the elapsed time in mind.
	 *
	 * @author	Wiebe
	 * @date	25-1-2018
	 *
	 * @param	elapsedTime	The elapsed time.
	 */
	void update(const float elapsedTime) override {
		PhysicsObject::update(elapsedTime);
        torso.setPosition(getPosition());
        leftLeg.setPosition(getPosition());
        rightLeg.setPosition(getPosition());
        leftArm.setPosition(getPosition());
        rightArm.setPosition(getPosition());
        head.setPosition(getPosition());
        doWalk();
		if (jump) {
			applyForce({ 0, -jumpForce });
			jump = false;
		}
		if (roll) {
				doRoll();
		}
		checkDeath();
        if (runClock.getElapsedTime().asMilliseconds() -lastKeyPressTime.asMilliseconds() > 250 && !runKeyPressed ){
            spammingRunKey = false;
            walkspeed = defaultWalkingSpeed;
        }
	}

	/**
	 * @fn	void Player::walk(int32_t direction)
	 *
	 * @brief	Walk the given direction
	 *
	 * @author	Wiebe
	 * @date	25-1-2018
	 *
	 * @param	direction	The direction.
	 */

	void setWalkDirection(int32_t direction) {
		walkDirection = direction;
	}

	/**
	 * @fn	void Player::doJump()
	 *
	 * @brief	Executes an jump action
	 *
	 * @author	Wiebe
	 * @date	25-1-2018
	 */

	void doJump() {
		sf::Vector2f velocity = getVelocity();
		if (velocity.y == 0) {
			jump = true;
		}
	}

	/**
	 * @fn	void Player::checkDeath()
	 *
	 * @brief	Check if the player is death
	 * 			
	 * @detailed Fire the fellOfMap event if the player is death.
	 *
	 * @author	Wiebe
	 * @date	25-1-2018
	 */

	void checkDeath() {
		if (getPosition().y > 2000) {
			game.fellOffMap.fire();
		}
	}

	/**
	 * @fn	sf::FloatRect Player::getBounds() override
	 *
	 * @brief	Gets the player visual bounds
	 *
	 * @author	Wiebe
	 * @date	25-1-2018
	 *
	 * @return	The bounds.
	 */

	sf::FloatRect getBounds() override {
		return getGlobalBounds();
	}

	/**
	 * @fn	KeyScheme Player::findKeyScheme(const KeyScheme::Difficulty difficulty)
	 *
	 * @brief	Searches randomly for a key scheme which matches the required difficulty.
	 *
	 * @author	Wiebe
	 * @date	25-1-2018
	 *
	 * @exception	KeySchemeNotFoundException	Thrown when a Key Scheme Not Found error condition
	 * 											occurs.
	 *
	 * @param	difficulty	The difficulty.
	 *
	 * @return	The found key scheme.
	 */

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

	/**
	 * @fn	void Player::setActiveKeyScheme(KeyScheme s)
	 *
	 * @brief	Sets active key scheme
	 *
	 * @author	Wiebe
	 * @date	25-1-2018
	 *
	 * @param	s	A KeyScheme to process.
	 */

	void setActiveKeyScheme(KeyScheme s) {
		activeKeyScheme = s;
	}

	/**
	 * @fn	KeyScheme& Player::getActiveKeyScheme()
	 *
	 * @brief	Gets active key scheme
	 *
	 * @author	Wiebe
	 * @date	25-1-2018
	 *
	 * @return	The active key scheme.
	 */

	KeyScheme& getActiveKeyScheme() {
		return activeKeyScheme;
	}

	void loseLeftLeg(){
		leftLegDisplay = false;
	}

	void loseRightLeg(){
		rightLegDisplay = false;
	}

	void loseLeftArm(){
		leftArmDisplay = false;
	}
	void loseRightArm(){
		rightArmDisplay = false;
	}
	void loseHead(){
		headDisplay = false;
	}

    void doWalk(){
        if (walkDirection == 0){
            if (!roll) {
                if (torsoDisplay) {
                    torso.setTexture(AssetManager::instance()->getTexture("fimmyStandingBody"));
                }
                if (headDisplay) {
                    head.setTexture(AssetManager::instance()->getTexture("fimmyStandingHead"));
                }
                if (leftLegDisplay) {
                    leftLeg.setTexture(AssetManager::instance()->getTexture("fimmyStandingLeftLeg"));
                }
                if (rightLegDisplay) {
                    rightLeg.setTexture(AssetManager::instance()->getTexture("fimmyStandingRightLeg"));
                }
                if (leftArmDisplay) {
                    leftArm.setTexture(AssetManager::instance()->getTexture("fimmyStandingLeftArm"));
                }
                if (rightArmDisplay) {
                    rightArm.setTexture(AssetManager::instance()->getTexture("fimmyStandingRightArm"));
                }
            }
        }
        if (walkDirection != 0) {
            if (walkDirection > 0) {
                if (!roll) {
                    if (torsoDisplay) {
                        torso.setTexture(AssetManager::instance()->getTexture("fimmyRightBody"));
                    }
                    if (headDisplay) {
                        head.setTexture(AssetManager::instance()->getTexture("fimmyRightHead"));
                    }
                    if (rightLegDisplay) {
                        if (animationClock.getElapsedTime().asMilliseconds() > animationTimeInMiliseconds) {
                            switch (animationCyle) {
                                case 1:
                                    rightLeg.setTexture(AssetManager::instance()->getTexture("fimmyRightLeg"));
                                    break;
                                case 2:
                                    rightLeg.setTexture(AssetManager::instance()->getTexture("fimmyRightLeg2"));
                                    break;
                                case 3:
                                    rightLeg.setTexture(AssetManager::instance()->getTexture("fimmyRightLeg3"));
                                    animationCyle = 0;
                                    break;
                            }
                            animationClock.restart();
                            animationCyle++;
                        }
                    }
                    if (rightArmDisplay) {
                        rightArm.setTexture(AssetManager::instance()->getTexture("fimmyRightArm"));
                    }
                }
            }
            if (walkDirection < 0) {
                if (!roll) {
                    if (torsoDisplay) {
                        torso.setTexture(AssetManager::instance()->getTexture("fimmyLeftBody"));
                    }
                    if (headDisplay) {
                        head.setTexture(AssetManager::instance()->getTexture("fimmyLeftHead"));
                    }
                    if (leftLegDisplay) {
                        if (animationClock.getElapsedTime().asMilliseconds() > animationTimeInMiliseconds) {
                            switch (animationCyle) {
                                case 1:
                                    leftLeg.setTexture(AssetManager::instance()->getTexture("fimmyLeftLeg"));
                                    break;
                                case 2:
                                    leftLeg.setTexture(AssetManager::instance()->getTexture("fimmyLeftLeg2"));
                                    break;
                                case 3:
                                    leftLeg.setTexture(AssetManager::instance()->getTexture("fimmyLeftLeg3"));
                                    animationCyle = 0;
                                    break;
                            }
                            animationClock.restart();
                            animationCyle++;
                        }
                    }
                    if (leftArmDisplay) {
                        leftArm.setTexture(AssetManager::instance()->getTexture("fimmyLeftArm"));
                    }
                }
            }
            if (!roll) {
                setVelocity({ walkDirection * walkspeed, getVelocity().y });
            }
        } else {
            if (!roll) {
                setVelocity({ 0, getVelocity().y });
            }
        }
    }
	void doRoll(){
		if (walkDirection > 0) {
			rollRectangle.setPosition(getPosition());
			rollRectangle.setTexture(AssetManager::instance()->getTexture("fimmyRollRight"));

			setVelocity({ 299, jumpForce });
		}
		else if (walkDirection < 0) {
			setVelocity({ -299, jumpForce });
			rollRectangle.setPosition(getPosition());
			rollRectangle.setTexture(AssetManager::instance()->getTexture("fimmyRollLeft"));
		}
		else {
			setVelocity({ 0, jumpForce });
		}
		setSize({ 20, 20 });
		roll = true;
		if (((rollClock.getElapsedTime().asSeconds()) > 1) || getVelocity().x == 0) {
			setSize({ 20, 40 });
			roll = false;
            rollRectangle.setFillColor(sf::Color::Transparent);
		}
	}

    void draw(sf::RenderTarget &window){
        Drawable::draw(window);
        head.draw(window);
        torso.draw(window);
        leftLeg.draw(window);
        rightLeg.draw(window);
        leftArm.draw(window);
        rightArm.draw(window);
        rollRectangle.draw(window);
    }
	using Rectangle::getCollision;
	using Rectangle::setPosition;
	using Rectangle::getPosition;
	using Rectangle::setSize;
	using Rectangle::draw;
};