#pragma once

#include <SFML/Audio.hpp>
#include <array>
#include <fstream>
#include "State.hpp"
#include "Statemachine.hpp"
#include "Characters.hpp"
#include "ViewFocus.hpp"
#include "Label.hpp"
#include "MapLoader.hpp"
#include "PowerUps.hpp"
#include "IntersectionGroup.hpp"

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
	EventConnection keyReleasedConnection;
	/** @brief	The died connection */
	EventConnection diedConnection;
	/** @brief	The fell off map connection */
	EventConnection fellOffMapConnection;

	/** @brief	The player */
    Player player;
	/** @brief	The death */
	Antagonist death;
	Antagonist deathSikkel;

	/** @brief	The score label */
	Label score;

	/** @brief	Gameover boolean */
	bool gameOver = false;
	/** @brief	The game over counter */
	float gameOverCounter = 3.0f;

	std::vector<PowerUp*> powerUps;

	std::array<int, 5> bodyRemoveToggles = {500, 600, 700, 800, 900}; // 2000, 5000, 8000, 11000, 18000
	int bodyRemoveToggleIndex = 0;

	sf::Sprite background;
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
		score(AssetManager::instance()->getFont("arial")),
        player(statemachine.window)
	{
		using Type = MapFactory::Type;
		using Value = MapFactory::Value;

		std::ifstream file("map_generated.txt");
		MapFactory mapFactory(file);


		mapFactory.registerCreateMethod("player", [&](Map& map, const MapItemProperties& properties) {
            properties.read({
				{ "Position", Type::Vector, [&](Value value) { player.setPosition(*value.vectorValue); } },
				{ "TextureId", Type::String, [&](Value value) { player.setTexture(&AssetManager::instance()->getTexture(*value.stringValue)); } }
			});
			map.addDrawable(player);
			map.setPrimaryCollidable(player);
		});

		mapFactory.registerCreateMethod("death", [&](Map& map, const MapItemProperties& properties) {
			properties.read({
				{ "Position", Type::Vector, [&](Value value) { death.setPosition(*value.vectorValue); } },
				{ "TextureId", Type::String, [&](Value value) { death.setTexture(&AssetManager::instance()->getTexture(*value.stringValue)); } }
			});
			death.setTexture(&AssetManager::instance()->getTexture("death"));
			deathSikkel.setPosition({ -50, 285});
			deathSikkel.setTexture(&AssetManager::instance()->getTexture("deathsikkel"));
			deathSikkel.setSize({ 100,400 });
			map.addDrawable(death);
			map.addDrawable(deathSikkel);
			map.addCollidable(death);
		});

		mapFactory.registerCreateMethod("heal-power-up", [&](Map& map, const MapItemProperties& properties) {
			IntersectionGroup* powerUpIntersectionGroup = new IntersectionGroup();
			Heal* heal = new Heal(powerUpIntersectionGroup);

			map.addDrawable(heal);
			map.addObject(heal);

			properties.read({
				{ "Color", Type::Color, [&](Value value) { heal->setFillColor(*value.colorValue); } },
				{ "Position", Type::Vector, [&](Value value) { heal->setPosition(*value.vectorValue); } },
				{ "Value", Type::Float, [&](Value value) { heal->setHealValue(value.floatValue); } },
				{ "TextureId", Type::String, [&](Value value) { heal->setTexture(&AssetManager::instance()->getTexture(*value.stringValue)); } }
			});

			powerUpIntersectionGroup->setPrimary(heal);
			powerUpIntersectionGroup->add(player);

			map.addObjectGroup(*powerUpIntersectionGroup);

			powerUps.emplace_back(heal);
		});

		map = mapFactory.buildMap();
		
		background.setTexture(AssetManager::instance()->getTexture("background"));
		background.setTextureRect({ 0, 0, 1280, 720 });
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
		backgroundMusic.setVolume(15);
		backgroundMusic.play();
		focus.setFocus(player);
        focus.setLeftBorder(500);
        focus.setRightBorder(500);
        focus.setTopBorder(270);
        focus.setBottomBorder(0);
		focus.update();

		player.collided.connect([this](Collidable& other) {
			if (&other == &death) {
				game.died.fire();
			}
		});

		for (PowerUp* powerUp : powerUps) {
			powerUp->connect(player, map);
		}

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

		for (PowerUp* powerUp : powerUps) {
			powerUp->disconnect();
		}

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
		background.setPosition(statemachine.window.mapPixelToCoords({ 0, 0 }));
		statemachine.window.draw(background);
		map.resolve();

		if (!gameOver) {
			player.update(elapsedTime);
			//death.update(elapsedTime);
			//deathSikkel.update(elapsedTime);
		}
		else if (gameOverCounter > 0) {
			gameOverCounter -= elapsedTime;
		}
		else {			
			statemachine.doTransition("game-over");
			return;
		}

		if (bodyRemoveToggleIndex != -1 && player.getPosition().x >= bodyRemoveToggles.at(bodyRemoveToggleIndex) ) {
			player.removeBodyPart(bodyRemoveToggleIndex);
			player.setNextKeyScheme();

			if (bodyRemoveToggleIndex < 4) {
				bodyRemoveToggleIndex++;
			}
			else {
				bodyRemoveToggleIndex = -1;
			}
		}

		//death.update(elapsedTime);
		//deathSikkel.update(elapsedTime);

		map.resolve();
		map.draw(statemachine.window);

		statemachine.window.draw(score);

		focus.update();
	}
};