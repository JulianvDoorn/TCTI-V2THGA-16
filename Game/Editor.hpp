#pragma once

#include <SFML/Graphics.hpp>
#include <map>

#include "State.hpp"
#include "Statemachine.hpp"
#include "PhysicsObject.hpp"
#include "MapLoader.hpp"
#include "MapEditor.hpp"

class Editor : public State {
	Statemachine& statemachine;

	FreeCamera camera;
	Dock dock;

	RectangleContainer rectContainer;

	MapFileGenerator mapFileGenerator;
	
	std::ofstream fileOut;

	Map map;

	EventConnection keyPressedConnection;
	EventConnection keyReleasedConnection;
	EventConnection mouseLeftButtonUp;
	EventConnection mouseMovedConn;
	EventConnectionVector physicsObjectConnVector;

	Selection selection;

	Rectangle player, death;

public:

	Editor(Statemachine& statemachine) :
		statemachine(statemachine),
		rectContainer(statemachine.window),
		dock(map, statemachine.window, selection),
		mapFileGenerator(fileOut),
		camera(statemachine.window, 100)
	{
		using Type = MapFactory::Type;
		using Value = MapFactory::Value;

		std::ifstream file("map_generated.txt");
		MapFactory mapFactory(file);

		mapFactory.registerCreateMethod("player", [&](Map& map, const MapItemProperties& properties) {
			properties.read({
				{ "Position", Type::Vector, [&](Value value) { camera.setPosition(*value.vectorValue); player.setPosition(*value.vectorValue); } }
			});
		});

		mapFactory.registerCreateMethod("death", [&](Map& map, const MapItemProperties& properties) {
			properties.read({
				{ "Position", Type::Vector, [&](Value value) { death.setPosition(*value.vectorValue); } }
			});
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
		camera.connect();

		map.objectAdded.connect([this](PhysicsObject& physicsObject) {
		
		});

		keyReleasedConnection = game.keyboard.keyReleased.connect([this](sf::Keyboard::Key key) {
			if (key == sf::Keyboard::Key::Escape) {
				statemachine.doTransition("main-menu");
			}
			else if (key == sf::Keyboard::Key::LControl) {
				std::cout << "Writing map to file...\n";
				mapFileGenerator.generate("map_generated.txt", map, player, death);
			}
		});

		selection.deselect();
		selection.connect();

		for (const std::unique_ptr<PhysicsObject>& temp : map) {
			PhysicsObject* physicsObject = &(*temp);

			physicsObject->bindMouseEvents();

			physicsObjectConnVector.connect(physicsObject->mouseLeftButtonDown, [physicsObject, this]() {
				selection.select(*physicsObject);
			});

			physicsObjectConnVector.connect(physicsObject->mouseLeftButtonUp, [physicsObject, this]() {
				selection.stopDrag();
			});
		}
	}

	void exit() override {
		camera.disconnect();
		camera.reset();

		selection.disconnect();

		physicsObjectConnVector.disconnect();

		for (const std::unique_ptr<PhysicsObject>& temp : map) {
			PhysicsObject* physicsObject = &(*temp);

			physicsObject->unbindMouseEvents();
		}

		keyReleasedConnection.disconnect();
	}

	void update(const float elapsedTime) override {
		camera.update(elapsedTime);

		map.resolveCollisions();
		map.draw(statemachine.window);
		dock.draw();
		rectContainer.draw();

		selection.update(elapsedTime);
		selection.draw(statemachine.window);



		sf::RectangleShape rectShape;
		rectShape.setSize({ 10, 10 });
		rectShape.setPosition(game.window->mapPixelToCoords(sf::Mouse::getPosition(*game.window)));
		statemachine.window.draw(rectShape);
	}
};