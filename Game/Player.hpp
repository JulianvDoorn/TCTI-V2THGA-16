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
#include "Label.hpp"
#include "KeyToString.hpp"
#include "Bodypart.hpp"
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
    float walkspeed = defaultWalkingSpeed;
	float jumpForce = 500;
    bool spammingRunKey = false;
    float runningSpammingFactor = 1;
	bool jump = false;
	bool roll = false;
    sf::Time lastKeyPressTime;
    bool runKeyPressed = false;

    sf::Clock runClock;
	sf::Clock rollClock;

	/** @brief	The key pressed connection */
	EventConnection keyPressedConn;

	/** @brief	The key released connection */
	EventConnection keyReleasedConn;

    Bodypart torso;
    Bodypart leftLeg;
    Bodypart rightLeg;
    Bodypart head;
    Bodypart leftArm;
    Bodypart rightArm;
    Bodypart rollRectangle;
    std::array<Bodypart * ,7> bodyparts;

    sf::Vector2f playersize = {20,40};

    sf::Clock animationClock;

    Label keyschemeText;
    sf::Clock keySchemeShowClock;
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

        bodyparts[0] = &torso;
        bodyparts[1] = &head;
        bodyparts[2] = &leftLeg;
        bodyparts[3] = &rightLeg;
        bodyparts[4] = &leftArm;
        bodyparts[5] = &rightArm;
        bodyparts[6] = &rollRectangle;

        for(auto &object : bodyparts){
            object->setSize(playersize);
        }

        rollRectangle.setSize({20,20});
        rollRectangle.setRollable();

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

    void setTextures(){
        // set all texutres for the standing position
        torso.setStandingTexture(AssetManager::instance()->getTexture("fimmyStandingBody"));
        head.setStandingTexture(AssetManager::instance()->getTexture("fimmyStandingHead"));
        leftArm.setStandingTexture(AssetManager::instance()->getTexture("fimmyStandingLeftArm"));
        rightArm.setStandingTexture(AssetManager::instance()->getTexture("fimmyStandingRightArm"));
        leftLeg.setStandingTexture(AssetManager::instance()->getTexture("fimmyStandingLeftLeg"));
        rightLeg.setStandingTexture(AssetManager::instance()->getTexture("fimmyStandingRightLeg"));

        // set all textures for the run right postion
        torso.setRunRightTexture(AssetManager::instance()->getTexture("fimmyRightBody"));
        head.setRunRightTexture(AssetManager::instance()->getTexture("fimmyRightHead"));
        rightArm.setRunRightTexture(AssetManager::instance()->getTexture("fimmyRightArm"));
        // set the texture to empty to display nothing.
        leftArm.setRunRightTexture();
        rightLeg.setRunRightTexture(AssetManager::instance()->getTexture("fimmyRightLeg"));
        leftLeg.setRunRightTexture();

        torso.setRunLeftTexture(AssetManager::instance()->getTexture("fimmyLeftBody"));
        head.setRunLeftTexture(AssetManager::instance()->getTexture("fimmyLeftHead"));
        leftArm.setRunLeftTexture(AssetManager::instance()->getTexture("fimmyLeftArm"));
        rightArm.setRunLeftTexture();
        leftLeg.setRunLeftTexture(AssetManager::instance()->getTexture("fimmyLeftLeg"));
        rightLeg.setRunLeftTexture();

        rollRectangle.setRollLeftTexture(AssetManager::instance()->getTexture("fimmyRollLeft"));
        rollRectangle.setRollRightTexture(AssetManager::instance()->getTexture("fimmyRollRight"));

        std::array<sf::Texture,3> runLeftAnnimation = {AssetManager::instance()->getTexture("fimmyLeftLeg"),AssetManager::instance()->getTexture("fimmyLeftLeg2"),AssetManager::instance()->getTexture("fimmyLeftLeg3")};
        leftLeg.setRunAnnimationTextures(runLeftAnnimation);
        leftLeg.setVisibleDirections(-1);

        std::array<sf::Texture,3> runRightAnnimation = {AssetManager::instance()->getTexture("fimmyRightLeg"),AssetManager::instance()->getTexture("fimmyRightLeg2"),AssetManager::instance()->getTexture("fimmyRightLeg3")};
        rightLeg.setRunAnnimationTextures(runRightAnnimation);
        rightLeg.setVisibleDirections(1);
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
        setTextures();
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

		PhysicsObject::update(elapsedTime);
        for (auto &object: bodyparts){
            object->setPosition(getPosition());
        }
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

    void doWalk(){
        if (walkDirection == 0){
            if (!roll) {
                for(auto &object: bodyparts){
                    object->useStandingTexture();
                }
            }
        }
        if (walkDirection != 0) {
            if (walkDirection > 0) {
                if (!roll) {
                    for(auto &object: bodyparts){
                        object->useRunRightTexture();
						object->updateAnimation(animationClock,walkDirection);
                    }
                }
            }
            if (walkDirection < 0) {
                if (!roll) {
                    for(auto &object: bodyparts){
                        object->useRunLeftTexture();
						object->updateAnimation(animationClock,walkDirection);
                    }
                }
            }
            if (!roll) {
                setVelocity({ walkDirection * walkspeed, getVelocity().y });
            }
        }
        else {
            if (!roll) {
                setVelocity({ 0, getVelocity().y });
            }
        }
    }
	void doRoll(){
        rollRectangle.enableDraw();
		if (walkDirection > 0) {
            for (auto &object : bodyparts){
                object->roll(walkDirection);
            }
			setVelocity({ 299, jumpForce });
		}
		else if (walkDirection < 0) {
            for (auto &object : bodyparts){
                object->roll(walkDirection);
            }
			setVelocity({ -299, jumpForce });
		}
		else {
			setVelocity({ 0, jumpForce });
		}
        setSize({ 20, 20 });
        roll = true;
        if (((rollClock.getElapsedTime().asSeconds()) > 1) || getVelocity().x == 0) {
            setSize({ 20, 40 });
			roll = false;
            rollRectangle.disalbeDraw();
		}
	}

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
		if (walkspeed > 299){ //max running speed without glitching
			walkspeed = 299;
		}
		lastKeyPressTime = runClock.getElapsedTime();
	}

	void checkStillRunning(){
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
	void unRoll(){
        roll = false;
        rollRectangle.disalbeDraw();
        for (auto &object : bodyparts){
            object->unRoll();
        }
        setSize(playersize);
        setPosition({getPosition().x + 20, getPosition().y});
	}
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

    void updateKeySchemeDisplay(){
        sf::Vector2f position = window.mapPixelToCoords(static_cast<sf::Vector2i>(window.getView().getSize()), window.getView());
        sf::Vector2f offset = {100,-100};
        keyschemeText.setPosition(position - offset);
        if(keySchemeShowClock.getElapsedTime().asMilliseconds() > keySchemeShowTimeInMilliseconds){
            keyschemeText.setColor(sf::Color::Transparent);
        }
    }
    void draw(sf::RenderTarget &window){
        Drawable::draw(window);
            for (auto &object : bodyparts){
                object->draw(window);
            }
        keyschemeText.draw(window);
    }

	void loseLeftLeg(){
		leftLeg.remove();
	}
	void loseRightLeg(){
		rightLeg.remove();
	}
	void loseLeftArm(){
		leftArm.remove();
	}
	void loseRightArm(){
		rightArm.remove();
	}
	void loseHead(){
		head.remove();
	}

	void gainLeftLeft(){
		leftLeg.show();
	}
	void gainRightLeg(){
		rightLeg.show();
	}
	void gainLeftArm(){
		leftArm.show();
	}
	void gainRightArm(){
		rightArm.show();
	}
	void gainHead(){
		head.show();
	}

	using Rectangle::getCollision;
	using Rectangle::setPosition;
	using Rectangle::getPosition;
	using Rectangle::setSize;
	using Rectangle::draw;
};