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

class Editor : public State {
	Statemachine& statemachine;

	ViewFocus focus;

	Map map;

	float cameraSpeed = 100;
	sf::Vector2i cameraMovement = { 0, 0 };
	
	Rectangle cameraFocus;

	EventConnection keyPressedConnection;
	EventConnection keyReleasedConnection;

public:

	Editor(Statemachine& statemachine) :
		statemachine(statemachine),
		focus(statemachine.window)
	{
		focus.setFocus(cameraFocus);
		cameraFocus.setGravity({ 0, 0 });

		using Type = MapFactory::Type;
		using Value = MapFactory::Value;

		std::ifstream file("map.txt");
		MapFactory mapFactory(file);

		mapFactory.registerCreateMethod("player", [&](Map& map, const MapItemProperties& properties) {
			properties.read({
				{ "Position", Type::Vector, [&](Value value) { cameraFocus.setPosition(*value.vectorValue); } }
			});
			// do nothing
		});

		mapFactory.registerCreateMethod("death", [&](Map& map, const MapItemProperties& properties) {
			// do nothing
		});

		map = mapFactory.buildMap();
	}

	void entry() override {
		focus.setFocus(cameraFocus);
		focus.setLeftBorder(500);
		focus.setRightBorder(500);
		focus.setTopBorder(320);
		focus.setBottomBorder(50);
		focus.update();
		focus.setDynamicCameraEnabled(false);

		keyPressedConnection = game.keyboard.keyPressed.connect([this](sf::Keyboard::Key key) {
			if (key == sf::Keyboard::Key::W) {
				cameraMovement += { 0, -1 };
				cameraFocus.setVelocity({ cameraFocus.getVelocity().x, cameraMovement.y * cameraSpeed });
			}
			else if (key == sf::Keyboard::Key::A) {
				cameraMovement += { -1, 0 };
				cameraFocus.setVelocity({ cameraMovement.x * cameraSpeed, cameraFocus.getVelocity().y });
			}
			else if (key == sf::Keyboard::Key::S) {
				cameraMovement += { 0, 1 };
				cameraFocus.setVelocity({ cameraFocus.getVelocity().x, cameraMovement.y * cameraSpeed });

			}
			else if (key == sf::Keyboard::Key::D) {
				cameraMovement += { 1, 0 };
				cameraFocus.setVelocity({ cameraMovement.x * cameraSpeed, cameraFocus.getVelocity().y });
			}
		});

		keyReleasedConnection = game.keyboard.keyReleased.connect([this](sf::Keyboard::Key key) {
			if (key == sf::Keyboard::Key::W) {
				cameraMovement -= { 0, -1 };
				cameraFocus.setVelocity({ cameraFocus.getVelocity().x, cameraMovement.y * cameraSpeed });
			}
			else if (key == sf::Keyboard::Key::A) {
				cameraMovement -= { -1, 0 };
				cameraFocus.setVelocity({ cameraMovement.x * cameraSpeed, cameraFocus.getVelocity().y });

			}
			else if (key == sf::Keyboard::Key::S) {
				cameraMovement -= { 0, 1 };
				cameraFocus.setVelocity({ cameraFocus.getVelocity().x, cameraMovement.y * cameraSpeed });

			}
			else if (key == sf::Keyboard::Key::D) {
				cameraMovement -= { 1, 0 };
				cameraFocus.setVelocity({ cameraMovement.x * cameraSpeed, cameraFocus.getVelocity().y });
			}
			else if (key == sf::Keyboard::Key::Escape) {
				statemachine.doTransition("main-menu");
			}
		});
	}

	void exit() override {
		focus.unsetFocus();
		focus.update();
		focus.setDynamicCameraEnabled(true);

		keyPressedConnection.disconnect();
		keyReleasedConnection.disconnect();
	}

	void update(const float elapsedTime) override {
		map.resolveCollisions();
		map.draw(statemachine.window);

		cameraFocus.update(elapsedTime);
		focus.update();
	}
};