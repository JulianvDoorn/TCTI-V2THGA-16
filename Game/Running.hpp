#pragma once

#include <array>
#include <SFML/Audio.hpp>
#include "State.hpp"
#include "Statemachine.hpp"
#include "Characters.hpp"
#include "ViewFocus.hpp"
#include "Label.hpp"
#include "MapFactory.hpp"
#include "DrawableGroup.hpp"

#include <fstream>

/**
 * @class	Running
 *
 * @brief	The running state, this is the state in which the maingame plays.
 *
 * @author	Jeffrey de Waal
 * @date	1/25/2018
 */

class Running : public State {

	/** @brief	The statemachine */
	Statemachine& statemachine;

	/** @brief	The dynamic focus */
	ViewFocus focus;
	/** @brief	The map */
	Map map;

	/** @brief	The background music */
	sf::Music backgroundMusic;

	/** @brief	The key released connection */
	EventConnection<sf::Keyboard::Key> keyReleasedConnection;
	/** @brief	The died connection */
	EventConnection<> diedConnection;
	/** @brief	The fell off map connection */
	EventConnection<> fellOffMapConnection;

	/** @brief	The player */
	Player player;
	/** @brief	The death */
	Antagonist death;

	/** @brief	The score label */
	Label score;

	/** @brief	Gameover boolean */
	bool gameOver = false;
	/** @brief	The game over counter */
	float gameOverCounter = 3.0f;

	using Type = Map::Type;

public:

	/**
	 * @fn	Running::Running(Statemachine& statemachine) : statemachine(statemachine), focus(statemachine.window), score(AssetManager::instance()->getFont("arial"))
	 *
	 * @brief	Running constructor, This constructor sets-up all the objects using The map file and adds collision. Creates the player and death with a default position and texture.
	 *
	 * @author	Jeffrey de Waal
	 * @date	1/25/2018
	 *
	 * @param [in,out]	statemachine	A reference to the statemachine build in the main.cpp file.
	 */

	Running(Statemachine& statemachine) :
		statemachine(statemachine),
		focus(statemachine.window),
		score(AssetManager::instance()->getFont("arial"))
	{
		map.registerCreateMethod("player", [&](std::istream& is) {
			CurlyBracketList<KeyValuePair> curlyBracketList;

			is >> curlyBracketList;

			for (const KeyValuePair& pair : curlyBracketList) {
				if (pair.key == "Position") {
					player.setPosition(*pair.value.vectorValue);
				}
				else if (pair.key == "TextureId") {
					sf::Texture& texture = AssetManager::instance()->getTexture(*pair.value.stringValue);
					player.setTexture(texture);
				}
			}

			map.drawableGroup.add(player);
			map.collisionGroup.setPrimaryCollidable(player);
		});

		map.registerCreateMethod("death", [&](std::istream& is) {
			CurlyBracketList<KeyValuePair> curlyBracketList;

			is >> curlyBracketList;

			for (const KeyValuePair& pair : curlyBracketList) {
				if (pair.key == "Position") {
					death.setPosition(*pair.value.vectorValue);
				}
				else if (pair.key == "TextureId") {
					sf::Texture& texture = AssetManager::instance()->getTexture(*pair.value.stringValue);
					death.setTexture(texture);
				}
			}

			map.drawableGroup.add(death);
			map.collisionGroup.add(death);
		});

		std::ifstream file;
		file.open("map.txt");
		map.readFile(file);
	}

	/**
	 * @fn	void Running::entry() override
	 *
	 * @brief	On entry sets-up, connects all the eventconnections to the correct event and sets the on event action. Sets the beackground music. Places the dynamic focus on the player.
	 *
	 * @author	Jeffrey de Waal
	 * @date	1/25/2018
	 */

	void entry() override {
		backgroundMusic.openFromFile("sound.wav");
		backgroundMusic.setLoop(true);
		backgroundMusic.play();
		focus.setFocus(player);
        focus.setLeftBorder(500);
        focus.setRightBorder(500);
        focus.setTopBorder(320);
        focus.setBottomBorder(50);
		focus.update();

		player.collided.connect([this](Collidable& other) {
			if (other == death) {
				game.died.fire();
			}
		});

		keyReleasedConnection = game.keyboard.keyReleased.connect([this](sf::Keyboard::Key key) {
			if (key == sf::Keyboard::Key::Escape) {
				statemachine.doTransition("game-pauze");
			}
		});

		diedConnection = game.died.connect([this]() {
			std::cout << "/!\\ death got you /!\\" << std::endl;
			gameOver = true;
		});

		fellOffMapConnection = game.fellOffMap.connect([this]() {
			std::cout << "/!\\ fell out of the world /!\\" << std::endl;
			gameOver = true;
		});
	}

	/**
	 * @fn	void Running::exit() override
	 *
	 * @brief	Disconnects all the eventconnections used in this state and sets the focus to the default position
	 *
	 * @author	Jeffrey de Waal
	 * @date	1/25/2018
	 */

	void exit() override {
		focus.unsetFocus();
		focus.update();
		backgroundMusic.stop();

		keyReleasedConnection.disconnect();

		if (!gameOver) {
			diedConnection.disconnect();
			fellOffMapConnection.disconnect();
		}
	}

	/**
	 * @fn	void Running::update(const float elapsedTime) override
	 *
	 * @brief	When not gameover updates the player and deaths position. Checks for collision between the player and the objects and the death. Updates the score and focus.
	 * 			When gameover transitions to the game-over state.
	 *
	 * @author	Jeffrey de Waal
	 * @date	1/25/2018
	 *
	 * @param	elapsedTime	The elapsed time.
	 */

	void update(const float elapsedTime) override {
		if (!gameOver) {
			player.update(elapsedTime);
			death.update(elapsedTime);
		}
		else if (gameOverCounter > 0) {
			gameOverCounter -= elapsedTime;
		}
		else {			
			statemachine.doTransition("game-over");
			return;
		}

		map.collisionGroup.resolveCollisions();
		map.drawableGroup.draw(statemachine.window);

		score.draw(statemachine.window);

		focus.update();
	}
};