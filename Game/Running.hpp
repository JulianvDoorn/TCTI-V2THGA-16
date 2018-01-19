#pragma once

#include <array>
#include "State.hpp"
#include "Statemachine.hpp"
#include "Characters.hpp"
#include "ViewFocus.hpp"

class Running : public State {
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
	Rectangle wall1;

	bool gameOver = false;
	float gameOverCounter = 3.0f;

public:
	Running(Statemachine& statemachine) :
		State("running"),
		statemachine(statemachine),
		focus(statemachine.window),
		player(statemachine.window)
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

		wall1.setSize({ 20, 100 });
		wall1.setPosition({ -200, 450 });
		objects.add(wall1);





		//KeyScheme& scheme = new KeyScheme(sf::Keyboard::Key::D, sf::Keyboard::Key::A, sf::Keyboard::Key::S, sf::Keyboard::Key::W);

		statemachine.addState(*this);
	}

	void entry() override {
		focus.setFocus(player);
		focus.update();

		keyReleasedConnection = game.keyboard.keyReleased.connect([this](sf::Keyboard::Key key) {
			if (key == sf::Keyboard::Key::Escape) {
				statemachine.doTransition("game-pauze-menu");
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
		wall1.draw(statemachine.window);

		focus.update();
	}
};