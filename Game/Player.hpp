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


	/** @brief	The walk direction */
	int32_t walkDirection = 0;

	/** @brief	The default walking speed */
	float defaultWalkingSpeed = 100;

    /** @brief	The walkspeed */
    float walkspeed = 100;

	/** @brief	The jump force */
	float jumpForce = 500;

    /** @brief	True to enable the spamming run key functionality */
    bool spammingRunKey = false;

    /** @brief	The running spamming factor */
    float runningSpammingFactor = 1;

	/** @brief	Jump flag */
	bool jump = false;

	/** @brief	Roll flag */
	bool roll = false;

    /** @brief	The last key press time */
    sf::Time lastKeyPressTime;

    /** @brief	True if run key pressed */
    bool runKeyPressed = false;

	/** @brief	The deathcase */
	int deathcase = 0;

    /** @brief	The run clock */
    sf::Clock runClock;

	/** @brief	The roll clock */
	sf::Clock rollClock;

	/** @brief	The key pressed connection */
	EventConnection<sf::Keyboard::Key> keyPressedConn;

	/** @brief	The key released connection */
	EventConnection<sf::Keyboard::Key> keyReleasedConn;

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
				doRoll();
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
				if (walkspeed > 299){ //max running speed without glitching
					walkspeed = 299;
				}
				lastKeyPressTime = runClock.getElapsedTime();
			}
			else if (key == activeKeyScheme.moveLeft) {
				walk(walkDirection - 1);
			}
			else if (key == activeKeyScheme.moveRight) {
				walk(walkDirection + 1);
			}
		});

		keyReleasedConn = game.keyboard.keyReleased.connect([this](const sf::Keyboard::Key key) {
			if (key == activeKeyScheme.moveLeft) {
				walk(walkDirection + 1);
			}
			else if (key ==activeKeyScheme.run){
				runKeyPressed = false;
				if (spammingRunKey){
					if (runClock.restart().asMilliseconds() > 200){
						walkspeed = defaultWalkingSpeed;
						runningSpammingFactor = 1;
					}
				}
				else{
					walkspeed = 100;
				}
			}
			else if (key == activeKeyScheme.moveRight) {
				walk(walkDirection - 1);
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
        if (walkDirection == 0){
            if (!roll) {
                if (torsoDisplay) {
                    torso.setTexture(AssetManager::instance()->getTexture("fimmyStandingBody"));
                    torso.draw(window);
                }
                if (headDisplay) {
                    head.setTexture(AssetManager::instance()->getTexture("fimmyStandingHead"));
                    head.draw(window);
                }
                if (leftLegDisplay) {
                    leftLeg.setTexture(AssetManager::instance()->getTexture("fimmyStandingLeftLeg"));
                    leftLeg.draw(window);
                }
                if (rightLegDisplay) {
                    rightLeg.setTexture(AssetManager::instance()->getTexture("fimmyStandingRightLeg"));
                    rightLeg.draw(window);
                }
                if (leftArmDisplay) {
                    leftArm.setTexture(AssetManager::instance()->getTexture("fimmyStandingLeftArm"));
                    leftArm.draw(window);
                }
                if (rightArmDisplay) {
                    rightArm.setTexture(AssetManager::instance()->getTexture("fimmyStandingRightArm"));
                    rightArm.draw(window);
                }
            }
        }
		if (walkDirection != 0) {
			if (walkDirection > 0) {
                if (!roll) {
                    if (torsoDisplay) {
                        torso.setTexture(AssetManager::instance()->getTexture("fimmyRightBody"));
                        torso.draw(window);
                    }
                    if (headDisplay) {
                        head.setTexture(AssetManager::instance()->getTexture("fimmyRightHead"));
                        head.draw(window);
                    }
                    if (rightLegDisplay) {
                        rightLeg.setTexture(AssetManager::instance()->getTexture("fimmyRightLeg"));
                        rightLeg.draw(window);
                    }
                    if (rightArmDisplay) {
                        rightArm.setTexture(AssetManager::instance()->getTexture("fimmyRightArm"));
                        rightArm.draw(window);
                    }
                }
			}
			if (walkDirection < 0) {
                if (!roll) {
                    if (torsoDisplay) {
                        torso.setTexture(AssetManager::instance()->getTexture("fimmyLeftBody"));
                        torso.draw(window);
                    }
                    if (headDisplay) {
                        head.setTexture(AssetManager::instance()->getTexture("fimmyLeftHead"));
                        head.draw(window);
                    }
                    if (leftLegDisplay) {
                        leftLeg.setTexture(AssetManager::instance()->getTexture("fimmyLeftLeg"));
                        leftLeg.draw(window);
                    }
                    if (leftArmDisplay) {
                        leftArm.setTexture(AssetManager::instance()->getTexture("fimmyLeftArm"));
                        leftArm.draw(window);
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

		if (jump) {
			applyForce({ 0, -jumpForce });
			jump = false;
		}
		if (roll) {
			if (walkDirection > 0) {
                    rollRectangle.setPosition(getPosition());
					rollRectangle.setTexture(AssetManager::instance()->getTexture("fimmyRollRight"));
					rollRectangle.draw(window);

				setVelocity({ 299, jumpForce });
			}
			else if (walkDirection < 0) {
				setVelocity({ -299, jumpForce });
                    rollRectangle.setPosition(getPosition());
					rollRectangle.setTexture(AssetManager::instance()->getTexture("fimmyRollLeft"));
					rollRectangle.draw(window);

			}
			else {
				setVelocity({ 0, jumpForce });
			}
			setSize({ 20, 20 });
			roll = true;
			if (((rollClock.getElapsedTime().asSeconds()) > 1) || getVelocity().x == 0) {
				setSize({ 20, 40 });
				roll = false;
			}
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

	void walk(int32_t direction) {
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
	 * @fn	void Player::doRoll()
	 *
	 * @brief	Executes an roll action
	 *
	 * @author	Wiebe
	 * @date	25-1-2018
	 */

	void doRoll() {
		roll = true;
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

	using Rectangle::getCollision;
	using Rectangle::setPosition;
	using Rectangle::getPosition;
	using Rectangle::setSize;
	using Rectangle::draw;
};