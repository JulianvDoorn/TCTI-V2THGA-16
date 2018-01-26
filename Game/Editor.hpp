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
#include "Dock.hpp"

class Editor : public State {
	Statemachine& statemachine;

	ViewFocus focus;

	Map map;

	float cameraSpeed = 100;
	sf::Vector2i cameraMovement = { 0, 0 };
	
	Rectangle cameraFocus;

	Dock dock;

	RectangleContainer rectContainer;

	EventConnection<sf::Keyboard::Key> keyPressedConnection;
	EventConnection<sf::Keyboard::Key> keyReleasedConnection;

public:

	Editor(Statemachine& statemachine) :
		statemachine(statemachine),
		focus(statemachine.window),
		rectContainer(statemachine.window),
		dock(rectContainer, statemachine.window)
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

		std::map<std::string, sf::Texture>& textures = AssetManager::instance()->getTextures();

		for (std::map<std::string, sf::Texture>::iterator it = textures.begin(); it != textures.end(); it++) {
			std::shared_ptr<Rectangle> rect = std::make_shared < Rectangle >();

			rect->setSize({ 40.0f, 40.0f });
			
			rect->setOutlineColor(sf::Color::White);
			rect->setOutlineThickness(1.0f);
			rect->setTexture(it->second);
			rect->setTextureRect({ 0, 0, 20, 20 });

			dock.addRectangle(rect);
		}
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
		dock.draw();
		rectContainer.draw();

		cameraFocus.update(elapsedTime);
		focus.update();
	}
};