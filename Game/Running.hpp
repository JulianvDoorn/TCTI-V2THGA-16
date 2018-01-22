#pragma once

#include <array>
#include "State.hpp"
#include "Statemachine.hpp"
#include "Characters.hpp"
#include "ViewFocus.hpp"
#include <SFML\Audio.hpp>

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

	sf::Texture brickTexture;
	sf::Texture groundTexture;
	sf::Texture bushTexture;

	bool gameOver = false;
	float gameOverCounter = 3.0f;

public:
	Running(Statemachine& statemachine) :
		State("running"),
		statemachine(statemachine),
		focus(statemachine.window),
		player(statemachine.window),
		collisionGroup(player)
	{

		brickTexture.loadFromFile("brickWall.png");
		groundTexture.loadFromFile("ground.png");
		bushTexture.loadFromFile("bush.png");
		collisionGroup.add(death);

		floor0.setSize({ 400, 200 });
		floor0.setPosition({ 0, 600 });
		floor0.setTexture(&groundTexture);
		collisionGroup.add(floor0);

		wall.setSize({ 30, 60 });
		wall.setPosition({ 250, 550 });
		collisionGroup.add(wall);

		wall1.setSize({ 30, 60 });
		wall1.setPosition({ -200, 450 });
		collisionGroup.add(wall1);

		crate.setSize({ 30, 30 });
		crate.setPosition({ 0, 450 });
		crate.setTexture(&brickTexture);
		collisionGroup.add(crate);

		bush.setSize({ 14, 14 });
		bush.setPosition({ 150, 494 });
		bush.setTexture(&bushTexture);

		statemachine.addState(*this);
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
		}
		else if (gameOverCounter > 0) {
			gameOverCounter -= elapsedTime;
		}
		else {
			statemachine.doTransition("game-over");
			return;
		}

		death.update(elapsedTime);

		collisionGroup.resolveCollisions();

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
		crate.draw(statemachine.window);
		bush.draw(statemachine.window);

		focus.update();
	}
};