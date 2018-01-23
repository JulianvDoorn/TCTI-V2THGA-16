#pragma once

#include <array>
#include <SFML\Audio.hpp>
#include "State.hpp"
#include "Statemachine.hpp"
#include "Characters.hpp"
#include "ViewFocus.hpp"
#include "Label.hpp"
#include "MapFactory.hpp"
#include "DrawableGroup.hpp"

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
	DrawableGroup drawableGroup;

	PhysicsObject* temp;

	//Rectangle floor0;
	//Rectangle floor1;
	//Rectangle wall;
	//Rectangle wall1;
	//Rectangle crate;
	//Rectangle bush;
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
		drawableGroup.add(player);
		death.setPosition({ -200, 200 });
		drawableGroup.add(death);



		std::ifstream file;
		file.exceptions(std::istream::failbit);
		file.open("map.txt");

		MapFactory mapFactory;

		while (!file.eof()) {
			std::string name;
			file >> name;

			PhysicsObject* ptr = mapFactory.create(name, file);

			if (ptr != nullptr) {
				drawableGroup.add(*ptr);
				collisionGroup.add(*ptr);
			}

			file >> std::ws;
		}



		

		collisionGroup.add(death);

		Rectangle* floor0 = new Rectangle();
		floor0->setSize({ 400, 200 });
		floor0->setPosition({ 0, 600 });
		floor0->setTexture(AssetManager::instance()->getTexture("ground"));
		collisionGroup.add(*floor0);
		drawableGroup.add(*floor0);

		Rectangle* floor1 = new Rectangle();
		floor1->setSize({ 60, 10 });
		floor1->setPosition({ 0, 500 });
		collisionGroup.add(*floor1);
		drawableGroup.add(*floor1);

		Rectangle* wall1 = new Rectangle();
		wall1->setSize({ 30, 60 });
		wall1->setPosition({ -200, 450 });
		collisionGroup.add(*wall1);
		drawableGroup.add(*wall1);

		Rectangle* crate = new Rectangle();
		crate->setSize({ 30, 30 });
		crate->setPosition({ 0, 450 });
		crate->setTexture(AssetManager::instance()->getTexture("brick"));
		collisionGroup.add(*crate);
		drawableGroup.add(*crate);

		Rectangle* bush = new Rectangle();
		bush->setSize({ 14, 14 });
		bush->setPosition({ 150, 494 });
		bush->setTexture(AssetManager::instance()->getTexture("bush"));
		drawableGroup.add(*bush);

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
		drawableGroup.draw(statemachine.window);

		//player.draw(statemachine.window);
		//death.draw(statemachine.window);

		//temp->draw(statemachine.window);

		//floor0.draw(statemachine.window);
		//floor1.draw(statemachine.window);
		//wall.draw(statemachine.window);
		//wall1.draw(statemachine.window);
		//crate.draw(statemachine.window);
		//bush.draw(statemachine.window);

		score.draw(statemachine.window);

		focus.update();
	}
};