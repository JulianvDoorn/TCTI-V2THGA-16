#pragma once

#include <array>
#include <SFML/Audio.hpp>
#include "State.hpp"
#include "Statemachine.hpp"
#include "Characters.hpp"
#include "ViewFocus.hpp"
#include "Label.hpp"

class Running : public State {
	Statemachine& statemachine;

	ViewFocus focus;

	sf::Music backgroundMusic;

	EventConnection<sf::Keyboard::Key> keyReleasedConnection;
	EventConnection<> diedConnection;
	EventConnection<> fellOffMapConnection;

	Player player;
	Antagonist death;
	CollisionGroup collisionGroup;

	Rectangle floor0;
	Rectangle floor1;
	Rectangle wall;
	Rectangle wall1;
	Rectangle crate;
	Rectangle bush;
	Label score;

	bool gameOver = false;
	float gameOverCounter = 3.0f;

public:
	Running(Statemachine& statemachine) :
		statemachine(statemachine),
		focus(statemachine.window),
		player(statemachine.window),
		collisionGroup(player),
		score(AssetManager::instance()->getFont("arial"))
	{
		player.setPosition({ 150, 450 });
		death.setPosition({ -200, 200 });

		collisionGroup.add(death);

		floor0.setSize({ 400, 200 });
		floor0.setPosition({ 0, 600 });
		floor0.setTexture(AssetManager::instance()->getTexture("ground"));
		collisionGroup.add(floor0);

		floor1.setSize({ 60, 10 });
		floor1.setPosition({ 0, 500 });
		collisionGroup.add(floor1);

		collisionGroup.add(wall);

		wall1.setSize({ 30, 60 });
		wall1.setPosition({ -200, 450 });
		collisionGroup.add(wall1);

		crate.setSize({ 30, 30 });
		crate.setPosition({ 0, 450 });
		crate.setTexture(AssetManager::instance()->getTexture("brick"));
		collisionGroup.add(crate);

		bush.setSize({ 14, 14 });
		bush.setPosition({ 150, 494 });
		bush.setTexture(AssetManager::instance()->getTexture("bush"));

	}

	void entry() override {
		backgroundMusic.openFromFile("sound.wav");
		backgroundMusic.setLoop("true");
		backgroundMusic.play();
		focus.setFocus(player);
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

		game.keyboard.keyPressed.connect([this](sf::Keyboard::Key key) {
			if (key == sf::Keyboard::Key::Z) {
				player.setActiveKeyScheme(player.findKeyScheme(KeyScheme::Difficulty::MODERATE));
			}
		});

		diedConnection = game.died.connect([this]() {
			diedConnection.disconnect();
			std::cout << "/!\\ death got you /!\\" << std::endl;
			gameOver = true;
		});

		fellOffMapConnection = game.fellOffMap.connect([this]() {
			fellOffMapConnection.disconnect();
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
		} else if (gameOverCounter > 0) {
			gameOverCounter -= elapsedTime;
		} else {
			statemachine.doTransition("game-over");
			return;
		}

		death.update(elapsedTime);

		collisionGroup.resolveCollisions();

		player.draw(statemachine.window);
		death.draw(statemachine.window);

		floor0.draw(statemachine.window);
		floor1.draw(statemachine.window);
		wall.draw(statemachine.window);
		wall1.draw(statemachine.window);
		crate.draw(statemachine.window);
		bush.draw(statemachine.window);

		score.draw(statemachine.window);

		focus.update();
	}
};