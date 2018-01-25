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

class Running : public State {
	Statemachine& statemachine;

	ViewFocus focus;
	Map map;

	sf::Music backgroundMusic;

	EventConnection<sf::Keyboard::Key> keyReleasedConnection;
	EventConnection<> diedConnection;
	EventConnection<> fellOffMapConnection;

	Player player;
	Antagonist death;

	Label score;

	bool gameOver = false;
	float gameOverCounter = 3.0f;

public:
	Running(Statemachine& statemachine) :
		statemachine(statemachine),
		focus(statemachine.window),
		score(AssetManager::instance()->getFont("arial"))
	{
		using Type = MapFactory::Type;
		using Value = MapFactory::Value;

		std::ifstream file("map.txt");
		MapFactory mapFactory(file);

		mapFactory.registerCreateMethod("player", [&](Map& map, const MapItemProperties& properties) {
			properties.read({
				{ "Position", Type::Vector, [&](Value value) { player.setPosition(*value.vectorValue); } },
				{ "TextureId", Type::String, [&](Value value) { player.setTexture(AssetManager::instance()->getTexture(*value.stringValue)); } }
			});

			map.addDrawable(player);
			map.setPrimaryCollidable(player);
		});

		mapFactory.registerCreateMethod("death", [&](Map& map, const MapItemProperties& properties) {
			properties.read({
				{ "Position", Type::Vector, [&](Value value) { death.setPosition(*value.vectorValue); } },
				{ "TextureId", Type::String, [&](Value value) { death.setTexture(AssetManager::instance()->getTexture(*value.stringValue)); } }
			});

			map.addDrawable(death);
			map.addCollidable(death);
		});

		map = mapFactory.buildMap();
	}

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

	void update(const float elapsedTime) override {
		if (!gameOver) {
			player.update(elapsedTime);
			death.update(elapsedTime);
		}
		else if (gameOverCounter > 0) {
			gameOverCounter -= elapsedTime;
		}
		else {
			diedConnection.disconnect();
			fellOffMapConnection.disconnect();
			
			statemachine.doTransition("game-over");
			return;
		}

		death.update(elapsedTime);

		map.resolveCollisions();
		map.draw(statemachine.window);

		score.draw(statemachine.window);

		focus.update();
	}
};