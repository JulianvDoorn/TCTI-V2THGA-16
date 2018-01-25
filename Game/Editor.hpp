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
	
	Rectangle cameraFocus;

	EventConnection<sf::Keyboard::Key> keyPressedConnection;
	EventConnection<sf::Keyboard::Key> keyReleasedConnection;

public:

	Editor(Statemachine& statemachine) :
		statemachine(statemachine),
		focus(statemachine.window)
	{
		focus.setFocus(cameraFocus);

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

		keyPressedConnection = game.keyboard.keyPressed.connect([this](sf::Keyboard::Key key) {
		
		});

		keyReleasedConnection = game.keyboard.keyReleased.connect([this](sf::Keyboard::Key key) {
			if (key == sf::Keyboard::Key::Escape) {
				statemachine.doTransition("main-menu");
			}
		});
	}

	void exit() override {
		focus.unsetFocus();
		focus.update();

		keyPressedConnection.disconnect();
		keyReleasedConnection.disconnect();
	}

	void update(const float elapsedTime) override {
		map.resolveCollisions();
		map.draw(statemachine.window);

		focus.update();
	}
};