#pragma once

#include "GameState.hpp"
#include "Statemachine.hpp"
#include "Characters.hpp"
#include "ViewFocus.hpp"

class Running : public GameState {
	Statemachine& statemachine;

	ViewFocus focus;

	EventConnection<sf::Keyboard::Key> keyReleasedConnection;
	EventConnection<> diedConnection;
	EventConnection<> fellOffMapConnection;

	Player player;
	Antagonist death;
	CollisionObjects objects;

	Rectangle floor0;
	Rectangle floor1;
	Rectangle wall;

	bool gameOver = false;
	float gameOverCounter = 3.0f;

public:
	Running(Statemachine& statemachine) :
		GameState("running"),
		statemachine(statemachine),
		focus(statemachine.window),
		player(statemachine.window, statemachine.keyboard, statemachine.gameEvents)
	{
		objects.add(death);
		
		floor0.setSize({ 600, 100 });
		floor0.setPosition({ 0, 600 });
		objects.add(floor0);

		floor1.setSize({ 60, 10 });
		floor1.setPosition({ 0, 500 });
		objects.add(floor1);

		wall.setSize({ 20, 50 });
		wall.setPosition({ 250, 550 });
		objects.add(wall);

		statemachine.addState(*this);
	}

	void entry() override {
		focus.setFocus(player);
		focus.update();

		keyReleasedConnection = statemachine.keyboard.keyReleased.connect([this](sf::Keyboard::Key key) {
			if (key == sf::Keyboard::Key::Escape) {
				statemachine.doTransition("main-menu");
			}
		});

		diedConnection = statemachine.gameEvents.died.connect([this]() {
			std::cout << "/!\\ death got you /!\\" << std::endl;
			gameOver = true;
		});

		fellOffMapConnection = statemachine.gameEvents.fellOffMap.connect([this]() {
			std::cout << "/!\\ fell out of the world /!\\" << std::endl;
			gameOver = true;
		});
	}

	void exit() override {
		focus.unsetFocus();
		focus.update();

		keyReleasedConnection.disconnect();
		diedConnection.disconnect();
		fellOffMapConnection.disconnect();
	}

	void update(const float elapsedTime) override {
		if (!gameOver) {
			player.update(elapsedTime);
		} else if (gameOverCounter > 0) {
			gameOverCounter -= elapsedTime;
		} else {
			statemachine.doTransition("game-over");
		}

		death.update(elapsedTime);

		player.detectCollision(objects);

		//player.resolveCollision(floor0);
		//player.resolveCollision(floor1);
		//player.resolveCollision(wall);
		//player.deathByAntagonist(death);

		player.draw(statemachine.window);
		death.draw(statemachine.window);

		floor0.draw(statemachine.window);
		floor1.draw(statemachine.window);
		wall.draw(statemachine.window);

		focus.update();
	}
};