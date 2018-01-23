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

	Rectangle rockFloor1;
	Rectangle rockFloor2;
	Rectangle grassFloor;
	Rectangle wall;
	Rectangle wall1;
	Rectangle crate;
	Rectangle bush;

	sf::Texture brickTexture;
	sf::Texture groundTexture;
	sf::Texture grassTexture;
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
		groundTexture.loadFromFile("ground2.png");
		grassTexture.loadFromFile("ground3.png");
		grassTexture.setRepeated(true);
		bushTexture.loadFromFile("bush.png");
		collisionGroup.add(death);

		rockFloor1.setSize({ 800, 200 });
		rockFloor1.setPosition({ 0, 600 });
		rockFloor1.setTexture(&groundTexture);
		collisionGroup.add(rockFloor1);

		rockFloor2.setSize({ 800, 200 });
		rockFloor2.setPosition({ 800, 600 });
		rockFloor2.setTexture(&groundTexture);
		collisionGroup.add(rockFloor2);

		grassFloor.setSize({ 1600, 20 });
		grassFloor.setTextureRect({0,0, 1600, 20 });
		grassFloor.setPosition({ 400,500 });
		grassFloor.setTexture(&grassTexture);
		collisionGroup.add(grassFloor);

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
		bush.setPosition({ 150, 486 });
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

		//player.resolveCollision(rockFloor1);
		//player.resolveCollision(rockFloor2);
		//player.resolveCollision(wall);
		//player.deathByAntagonist(death);

		player.draw(statemachine.window);
		death.draw(statemachine.window);

		rockFloor1.draw(statemachine.window);
		rockFloor2.draw(statemachine.window);
		grassFloor.draw(statemachine.window);
		wall.draw(statemachine.window);
		wall1.draw(statemachine.window);
		crate.draw(statemachine.window);
		bush.draw(statemachine.window);

		focus.update();
	}
};