#pragma once

#include "GameState.hpp"
#include "Statemachine.hpp"
#include "Characters.hpp"

class Running : public GameState {
	Statemachine& statemachine;

	EventConnection<sf::Keyboard::Key> keyReleasedConnection;

	sf::View view;

	Player player;
	Antagonist death;
	CollisionObjects objects;

	Rectangle floor0;
	Rectangle floor1;
	Rectangle wall;

public:
	Running(Statemachine& statemachine) :
		GameState("running"),
		statemachine(statemachine),
		view(sf::Vector2f(150, 10), sf::Vector2f(1280, 720)), player(statemachine.window, statemachine.keyboard, view)
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
		keyReleasedConnection = statemachine.keyboard.keyReleased.connect([this](sf::Keyboard::Key key) {
			if (key == sf::Keyboard::Key::Escape) {
				statemachine.doTransition("main-menu");
			}
		});
	}

	void exit() override {
		keyReleasedConnection.disconnect();
	}

	void update(const float elapsedTime) override {
		player.update(elapsedTime);
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
	}
};