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

	using Type = Map::Type;

public:
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

		map.collisionGroup.resolveCollisions();
		map.drawableGroup.draw(statemachine.window);

		score.draw(statemachine.window);

		focus.update();
	}
};