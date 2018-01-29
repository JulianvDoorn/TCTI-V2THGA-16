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

	AssetFileGenerator assetFileGenerator;
	ShapeFileGenerator shapeFileGenerator;
	
	std::ofstream fileOut;


	Map map;

	EventConnection keyPressedConnection;
	EventConnection keyReleasedConnection;
	EventConnection mouseLeftButtonUp;
	EventConnection mouseMovedConn;
	EventConnection objectAddedConn;
	EventConnection objectRemovedConn;
	
	struct EditorEventBinding {
		EventConnection mouseLeftButtonDown;
		EventConnection mouseLeftButtonUp;

		EditorEventBinding(EventConnection mouseLeftButtonDown, EventConnection mouseLeftButtonUp) :
			mouseLeftButtonDown(mouseLeftButtonDown),
			mouseLeftButtonUp(mouseLeftButtonUp)
		{ }
	};

	std::map<PhysicsObject*, EditorEventBinding> editorEventConnMap;

	Selection selection;

	void bindEditorEvents(PhysicsObject& object) {
		object.bindMouseEvents();

		editorEventConnMap.insert(std::pair<PhysicsObject*, EditorEventBinding>(&object, EditorEventBinding {
			object.mouseLeftButtonDown.connect([&object, this]() {
				selection.select(object);
			}),

			object.mouseLeftButtonUp.connect([&object, this]() {
				selection.stopDrag();
			})
		}));
	}

	void unbindEditorEvents(PhysicsObject& object) {
		auto it = editorEventConnMap.find(&object);

		if (it != editorEventConnMap.end()) {
			it->first->unbindMouseEvents();
			it->second.mouseLeftButtonDown.disconnect();
			it->second.mouseLeftButtonUp.disconnect();

			editorEventConnMap.erase(it);
		}
	}

public:

	Editor(Statemachine& statemachine) :
		statemachine(statemachine),
		rectContainer(statemachine.window),
		dock(map, statemachine.window, selection),
		assetFileGenerator(fileOut),
		shapeFileGenerator(fileOut),
		camera(statemachine.window, 100),
		selection(map)
	{
		using Type = MapFactory::Type;
		using Value = MapFactory::Value;

		std::ifstream file("map.txt");
		MapFactory mapFactory(file);

		mapFactory.registerCreateMethod("player", [&](Map& map, const MapItemProperties& properties) {
			properties.read({
				{ "Position", Type::Vector, [&](Value value) { camera.setPosition(*value.vectorValue); } }
			});
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
		camera.connect();

		for (const std::unique_ptr<PhysicsObject>& physicsObject : map) {
			bindEditorEvents(*physicsObject);
		}

		objectAddedConn = map.objectAdded.connect([this](PhysicsObject& physicsObject) {
			bindEditorEvents(physicsObject);
		});

		objectRemovedConn = map.objectRemoving.connect([this](PhysicsObject& physicsObject) {
			unbindEditorEvents(physicsObject);
		});

		keyReleasedConnection = game.keyboard.keyReleased.connect([this](sf::Keyboard::Key key) {
			if (key == sf::Keyboard::Key::Escape) {
				statemachine.doTransition("main-menu");
			}
			else if (key == sf::Keyboard::Key::LControl) {
				std::cout << "Writing assets to file...\n";
				assetFileGenerator.generate("assets_generated.txt");
				std::cout << "Writing map to file...\n";
				shapeFileGenerator.generate("map_generated.txt", rectContainer);
			}
		});

		selection.deselect();
		selection.connect();
	}

	void exit() override {
		camera.disconnect();
		camera.reset();

		selection.disconnect();

		objectAddedConn.disconnect();
		objectRemovedConn.disconnect();

		for (const std::unique_ptr<PhysicsObject>& physicsObject : map) {
			unbindEditorEvents(*physicsObject);
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