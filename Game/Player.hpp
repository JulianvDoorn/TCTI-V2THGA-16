#pragma once

#include <vector>
#include <exception>
#include <memory>
#include <SFML/Graphics.hpp>
#include "Body.hpp"
#include "EventSource.hpp"
#include "Keyboard.hpp"
#include "KeyScheme.hpp"
#include "Label.hpp"
#include "KeyToString.hpp"
#include "VectorStreamOperators.hpp"

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

class Player : public Body {
private:

    /** @brief	The window */
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
    float walkspeed = defaultWalkingSpeed;
	/** @brief	The jump force */
	float jumpForce = 500;
    /** @brief	True to spamming run key */
    bool spammingRunKey = false;
    /** @brief	The running spamming factor */
    float runningSpammingFactor = 1;
	/** @brief	True to jump */
	bool jump = false;
	/** @brief	True to roll */
	bool roll = false;
    /** @brief	The last key press time */
    sf::Time lastKeyPressTime;
    /** @brief	True if run key pressed */
    bool runKeyPressed = false;

    /** @brief	The run clock */
    sf::Clock runClock;
	/** @brief	The roll clock */
	sf::Clock rollClock;

	/** @brief	The key pressed connection */
	EventConnection keyPressedConn;

	/** @brief	The key released connection */
	EventConnection keyReleasedConn;

    /** @brief	These variable are used to set a bodypart to display * */
    bool torsoDisplay = true;
    bool leftLegDisplay = true;
    bool rightLegDisplay = true;
    bool headDisplay = true;
    bool leftArmDisplay = true;
    bool rightArmDisplay = true;
    bool leftArmDraw = true;
    bool rightArmDraw = true;
    bool rollRectangleDisplay = false;
    /** @brief	The torso */
    Body torso;
    /** @brief	The left leg */
    Body leftLeg;
    /** @brief	The right leg */
    Body rightLeg;
    /** @brief	The head */
    Body head;
    /** @brief	The left arm */
    Body leftArm;
    /** @brief	The right arm */
    Body rightArm;
    /** @brief	The roll rectangle */
    Body rollRectangle;

    /** @brief	The playersize */
    sf::Vector2f playersize = {20,40};

    /** @brief	The default animation time in miliseconds */
    int defaultAnimationTimeInMiliseconds = 50;
    /** @brief	The animation time in miliseconds */
    int animationTimeInMiliseconds = defaultAnimationTimeInMiliseconds;
	/** @brief	The animation cyle */
	int animationCyle = 0;
    /** @brief	The animation clock */
    sf::Clock animationClock;

    /** @brief	The keyscheme text */
    Label keyschemeText;
    /** @brief	The key scheme show clock */
    sf::Clock keySchemeShowClock;
    /** @brief	The key scheme show time in milliseconds */
    int keySchemeShowTimeInMilliseconds = 3000;


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
					doRun();
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
				checkStillRunning();
			}
			else if (key == activeKeyScheme.moveRight) {
				setWalkDirection(walkDirection - 1);
			}
            else if (key == activeKeyScheme.roll){
                roll = false;
				unRoll();
            }
		});
	}

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
        doWalk();
		if (jump) {
			applyForce({ 0, -jumpForce });
			jump = false;
		}
		if (roll) {
			doRoll();
		}
		checkDeath();
		if (runClock.getElapsedTime().asMilliseconds() - lastKeyPressTime.asMilliseconds() > 250 && !runKeyPressed) {
			spammingRunKey = false;
			walkspeed = defaultWalkingSpeed;
		}

		Body::update(elapsedTime);
        torso.setPosition(getPosition());
        leftLeg.setPosition(getPosition());
        rightLeg.setPosition(getPosition());
        leftArm.setPosition(getPosition());
        rightArm.setPosition(getPosition());
        head.setPosition(getPosition());

        updateKeySchemeDisplay();
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
        showKeySchemeUsed();
		walkDirection = direction;
	}

	/**
	 * @fn	void Player::doJump()
	 *
	 * @brief	Executes an jump action and play the jump sound.
	 *
	 * @author	Wiebe
	 * @date	25-1-2018
	 */

	void doJump() {
		sf::Vector2f velocity = getVelocity();
		if (velocity.y == 0) {
            AssetManager::instance()->getSound("jump").play();
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

    /**
     * @fn	void Player::doWalk()
     *
     * @brief	Executes the walk operation
     *
     * @author	Jeffrey
     * @date	1/31/2018
     */

    void doWalk(){

        if (walkDirection == 0){
            if (!roll) {
                if (torsoDisplay) {
                    torso.setTexture(&AssetManager::instance()->getTexture("fimmyStandingBody"));
                }
                if (headDisplay) {
                    head.setTexture(&AssetManager::instance()->getTexture("fimmyStandingHead"));
                }
                if (leftLegDisplay) {
                    leftLeg.setTexture(&AssetManager::instance()->getTexture("fimmyStandingLeftLeg"));
                }
                if (rightLegDisplay) {
                    rightLeg.setTexture(&AssetManager::instance()->getTexture("fimmyStandingRightLeg"));
                }
                if (leftArmDisplay) {
                    leftArm.setTexture(&AssetManager::instance()->getTexture("fimmyStandingLeftArm"));
                    leftArmDraw =true;
                }
                if (rightArmDisplay) {
                    rightArm.setTexture(&AssetManager::instance()->getTexture("fimmyStandingRightArm"));
                    rightArmDraw = true;
                }
            }
        }
        if (walkDirection != 0) {
            if (walkDirection > 0) {
                if (!roll) {
                    if (torsoDisplay) {
                        torso.setTexture(&AssetManager::instance()->getTexture("fimmyRightBody"));
                    }
                    if (headDisplay) {
                        head.setTexture(&AssetManager::instance()->getTexture("fimmyRightHead"));
                    }
                    if (rightLegDisplay) {
                        if (animationClock.getElapsedTime().asMilliseconds() > animationTimeInMiliseconds) {
                            switch (animationCyle) {
                                case 1:
                                    rightLeg.setTexture(&AssetManager::instance()->getTexture("fimmyRightLeg"));
                                    break;
                                case 2:
                                    rightLeg.setTexture(&AssetManager::instance()->getTexture("fimmyRightLeg2"));
                                    break;
                                case 3:
                                    rightLeg.setTexture(&AssetManager::instance()->getTexture("fimmyRightLeg3"));
                                    animationCyle = 0;
                                    break;
                            }
                            animationClock.restart();
                            animationCyle++;
                        }
                    }
                    if (rightArmDisplay) {
                        rightArm.setTexture(&AssetManager::instance()->getTexture("fimmyRightArm"));
                    }
                    leftArmDraw = false;
                }
            }
            if (walkDirection < 0) {
                if (!roll) {
                    if (torsoDisplay) {
                        torso.setTexture(&AssetManager::instance()->getTexture("fimmyLeftBody"));
                    }
                    if (headDisplay) {
                        head.setTexture(&AssetManager::instance()->getTexture("fimmyLeftHead"));
                    }
                    if (leftLegDisplay) {
                        if (animationClock.getElapsedTime().asMilliseconds() > animationTimeInMiliseconds) {
                            switch (animationCyle) {
                                case 1:
                                    leftLeg.setTexture(&AssetManager::instance()->getTexture("fimmyLeftLeg"));
                                    break;
                                case 2:
                                    leftLeg.setTexture(&AssetManager::instance()->getTexture("fimmyLeftLeg2"));
                                    break;
                                case 3:
                                    leftLeg.setTexture(&AssetManager::instance()->getTexture("fimmyLeftLeg3"));
                                    animationCyle = 0;
                                    break;
                            }
                            animationClock.restart();
                            animationCyle++;
                        }
                    }
                    if (leftArmDisplay) {
                        leftArm.setTexture(&AssetManager::instance()->getTexture("fimmyLeftArm"));
                    }
                    rightArmDraw = false;
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

	/**
	 * @fn	void Player::doRoll()
	 *
	 * @brief	Executes the roll operation
	 *
	 * @author	Jeffrey de Waal
	 * @date	1/31/2018
	 */

	void doRoll(){
        rollRectangleDisplay = true;
		if (walkDirection > 0) {
			rollRectangle.setPosition(getPosition());
			rollRectangle.setTexture(&AssetManager::instance()->getTexture("fimmyRollRight"));
			setVelocity({ 299, jumpForce });
		}
		else if (walkDirection < 0) {
			setVelocity({ -299, jumpForce });
			rollRectangle.setPosition(getPosition());
			rollRectangle.setTexture(&AssetManager::instance()->getTexture("fimmyRollLeft"));
		}
		else {
			setVelocity({ 0, jumpForce });
		}
        setSize({ 20, 20 });
        roll = true;
        if (((rollClock.getElapsedTime().asSeconds()) > 1) || getVelocity().x == 0) {
            setSize({ 20, 40 });
			roll = false;
            rollRectangleDisplay = false;
		}
	}

	/**
	 * @fn	void Player::doRun()
	 *
	 * @brief	Executes the run operation
	 *
	 * @author	Jeffrey de Waal
	 * @date	1/31/2018
	 */

	void doRun(){
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
		animationTimeInMiliseconds /= int(runningSpammingFactor);
		if (walkspeed > 299){ //max running speed without glitching
			walkspeed = 299;
		}
		lastKeyPressTime = runClock.getElapsedTime();
	}

	/**
	 * @fn	void Player::checkStillRunning()
	 *
	 * @brief	Check still running
	 *
	 * @author	Jeffrey de Waal
	 * @date	1/31/2018
	 */

	void checkStillRunning(){
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

	/**
	 * @fn	void Player::unRoll()
	 *
	 * @brief	Un roll
	 *
	 * @author	Jeffrey
	 * @date	1/31/2018
	 */

	void unRoll(){
        roll = false;
        rollRectangleDisplay = false;
        setSize(playersize);
        setPosition({getPosition().x + 20, getPosition().y});
	}

    /**
     * @fn	void Player::showKeySchemeUsed()
     *
     * @brief	Shows the key scheme used
     *
     * @author	Jeffrey de Waal
     * @date	1/31/2018
     */

    void showKeySchemeUsed(){
        keyschemeText.setFont(AssetManager::instance()->getFont("arial"));
        keyschemeText.setCharSize(16);
        keyschemeText.setColor(sf::Color::White);
        std::string moveLeft = "Left: " + keyToString(activeKeyScheme.moveLeft);
        std::string moveRight = "Right: " + keyToString(activeKeyScheme.moveRight);
        std::string jump = "Jump: " + keyToString(activeKeyScheme.jump);
        std::string run = "Run: " + keyToString(activeKeyScheme.run);
        std::string roll = "Roll: " + keyToString(activeKeyScheme.roll);
        keyschemeText.setText(moveLeft+ " "+ moveRight +" "+jump + " "+ run + " " + roll );
        keySchemeShowClock.restart();
    }

    /**
     * @fn	void Player::updateKeySchemeDisplay()
     *
     * @brief	Updates the key scheme display
     *
     * @author	Jeffrey de Waal
     * @date	1/31/2018
     */

    void updateKeySchemeDisplay(){
        sf::Vector2f position = window.mapPixelToCoords(static_cast<sf::Vector2i>(window.getView().getSize()), window.getView());
        sf::Vector2f offset = {100,-100};
        keyschemeText.setPosition(position - offset);
        if(keySchemeShowClock.getElapsedTime().asMilliseconds() > keySchemeShowTimeInMilliseconds){
            keyschemeText.setColor(sf::Color::Transparent);
        }
    }

    /**
     * @fn	void Player::draw(sf::RenderTarget &window, sf::RenderStates renderStates) const override
     *
     * @brief	Draws
     *
     * @author	Jeffrey de Waal
     * @date	1/31/2018
     *
     * @param [in,out]	window			The window.
     * @param 		  	renderStates	List of states.
     */

    void draw(sf::RenderTarget &window, sf::RenderStates renderStates) const override {
		if (!roll){
			window.draw(head);
			window.draw(torso);
			window.draw(leftLeg);
			window.draw(rightLeg);

            if (leftArmDraw) {
				window.draw(leftArm);
            }

            if (rightArmDraw) {
				window.draw(rightArm);
            }
        }

        if (rollRectangleDisplay){
			window.draw(rollRectangle);
        }

		window.draw(keyschemeText);
    }

	/**
	 * @fn	void Player::loseLeftLeg()
	 *
	 * @brief	Lose left leg
	 *
	 * @author	Jeffrey de Waal
	 * @date	1/31/2018
	 */

	void loseLeftLeg(){
		leftLegDisplay = false;
	}

	/**
	 * @fn	void Player::loseRightLeg()
	 *
	 * @brief	Lose right leg
	 *
	 * @author	Jeffrey de Waal
	 * @date	1/31/2018
	 */

	void loseRightLeg(){
		rightLegDisplay = false;
	}

	/**
	 * @fn	void Player::loseLeftArm()
	 *
	 * @brief	Lose left arm
	 *
	 * @author	Jeffrey de Waal
	 * @date	1/31/2018
	 */

	void loseLeftArm(){
		leftArmDisplay = false;
	}

	/**
	 * @fn	void Player::loseRightArm()
	 *
	 * @brief	Lose right arm
	 *
	 * @author	Jeffrey de Waal
	 * @date	1/31/2018
	 */

	void loseRightArm(){
		rightArmDisplay = false;
	}

	/**
	 * @fn	void Player::loseHead()
	 *
	 * @brief	Lose head
	 *
	 * @author	Jeffrey de Waal
	 * @date	1/31/2018
	 */

	void loseHead(){
		headDisplay = false;
	}

	/**
	 * @fn	void Player::gainLeftLeft()
	 *
	 * @brief	Gain left
	 *
	 * @author	Jeffrey de Waal
	 * @date	1/31/2018
	 */

	void gainLeftLeft(){
		leftLegDisplay =true;
	}

	/**
	 * @fn	void Player::gainRightLeg()
	 *
	 * @brief	Gain right leg
	 *
	 * @author	Jeffrey de Waal
	 * @date	1/31/2018
	 */

	void gainRightLeg(){
		rightLegDisplay =true;
	}

	/**
	 * @fn	void Player::gainLeftArm()
	 *
	 * @brief	Gain left arm
	 *
	 * @author	Jeffrey de Waal
	 * @date	1/31/2018
	 */

	void gainLeftArm(){
		leftArmDisplay = true;
	}

	/**
	 * @fn	void Player::gainRightArm()
	 *
	 * @brief	Gain right arm
	 *
	 * @author	Jeffrey de Waal
	 * @date	1/31/2018
	 */

	void gainRightArm(){
		rightArmDisplay = true;
	}

	/**
	 * @fn	void Player::gainHead()
	 *
	 * @brief	Gain head
	 *
	 * @author	Jeffrey de Waal
	 * @date	1/31/2018
	 */

	void gainHead(){
		headDisplay = true;
	}
};