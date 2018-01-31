#pragma once

#include <SFML/Graphics.hpp>
#include <map>

#include "State.hpp"
#include "Statemachine.hpp"
#include "MapLoader.hpp"
#include "MapEditor.hpp"

/**
 * @class	Editor
 *
 * @brief	An editor for game maps.
 *
 * @author	Wiebe
 * @date	29-1-2018
 */

class Editor : public State {

	/** @brief	The statemachine */
	Statemachine& statemachine;

	/** @brief	The view camera */
	FreeCamera camera;

	/** @brief	The dock displaying available shapes */
	Dock dock;

	/** @brief	The map file generator */
	MapFileGenerator mapFileGenerator;
	
	/** @brief	The file out stream */
	std::ofstream fileOut;

	/** @brief	The game map */
	Map map;

	EventConnection keyPressedConnection;
	EventConnection keyReleasedConnection;
	EventConnection mouseLeftButtonUp;
	EventConnection mouseMovedConn;
	EventConnection objectAddedConn;
	EventConnection objectRemovedConn;

	/**
	 * @struct	EditorEventBinding
	 *
	 * @brief	An editor event binding.
	 *
	 * @author	Wiebe
	 * @date	29-1-2018
	 */

	struct EditorEventBinding {
		/** @brief	The mouse left button down connection */
		EventConnection mouseLeftButtonDown;

		/** @brief	The mouse left button up connection */
		EventConnection mouseLeftButtonUp;

		/**
		 * @fn	EditorEventBinding(EventConnection mouseLeftButtonDown, EventConnection mouseLeftButtonUp)
		 *
		 * @brief	Constructor
		 *
		 * @author	Wiebe
		 * @date	29-1-2018
		 *
		 * @param	mouseLeftButtonDown	The mouse left button down.
		 * @param	mouseLeftButtonUp  	The mouse left button up.
		 */

		EditorEventBinding(EventConnection mouseLeftButtonDown, EventConnection mouseLeftButtonUp) :
			mouseLeftButtonDown(mouseLeftButtonDown),
			mouseLeftButtonUp(mouseLeftButtonUp)
		{ }
	};


	/** @brief	The editor event connection map */
	std::map<Body*, EditorEventBinding> editorEventConnMap;


	/** @brief	Used for object selection */
	ObjectSelector selection;

	/**
	 * @property	Body player, death
	 *
	 * @brief	Represents the player and the death
	 */

	Body player, death;

	/** @brief	True if left control pressed */
	bool lControlPressed = false;

	/**
	 * @fn	void Editor::bindEditorEvents(Body& object)
	 *
	 * @brief	Bind editor events
	 *
	 * @author	Wiebe
	 * @date	29-1-2018
	 *
	 * @param [in,out]	object	The physic object subject.
	 */

	void bindEditorEvents(Body& object) {
		object.bindMouseEvents();

		editorEventConnMap.insert(std::pair<Body*, EditorEventBinding>(&object, EditorEventBinding {
			object.mouseLeftButtonDown.connect([&object, this]() {
				selection.select(object);
			}),

			object.mouseLeftButtonUp.connect([&object, this]() {
				selection.stopDrag();
			})
		}));
	}

	/**
	 * @fn	void Editor::unbindEditorEvents(Body& object)
	 *
	 * @brief	Unbind editor events
	 *
	 * @author	Wiebe
	 * @date	29-1-2018
	 *
	 * @param [in,out]	object	The physic object subject.
	 */

	void unbindEditorEvents(Body& object) {
		auto it = editorEventConnMap.find(&object);

		if (it != editorEventConnMap.end()) {
			it->first->unbindMouseEvents();
			it->second.mouseLeftButtonDown.disconnect();
			it->second.mouseLeftButtonUp.disconnect();

			editorEventConnMap.erase(it);
		}
	}

public:

	/**
	 * @fn	Editor::Editor(Statemachine& statemachine)
	 *
	 * @brief	Constructor
	 *
	 * @author	Wiebe
	 * @date	29-1-2018
	 *
	 * @param [in,out]	statemachine	The statemachine.
	 */

	Editor(Statemachine& statemachine) :
		statemachine(statemachine),
		dock(map, statemachine.window, selection),
		mapFileGenerator(fileOut),
		camera(statemachine.window, 275),
		selection(map)
	{
		using Type = MapFactory::Type;
		using Value = MapFactory::Value;

		// Create new file input stream which we'ill use to read out an map file.
		std::ifstream file("map_generated.txt");

		// Factory used for generating Body instances.
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

		mapFactory.registerCreateMethod("power-up", [&](Map& map, const MapItemProperties& properties) {
			// do nothing
		});

		// Build the map.
		map = mapFactory.buildMap();

		// Load the textures.
		std::map<std::string, sf::Texture>& textures = AssetManager::instance()->getTextures();

		// Add those textures into dummy rectangles and place them into the dock.
		for (std::map<std::string, sf::Texture>::iterator it = textures.begin(); it != textures.end(); it++) {
			Body rect;

			rect.setSize({ 40.0f, 40.0f });
			
			rect.setOutlineColor(sf::Color::White);
			rect.setOutlineThickness(1.0f);
			rect.setTexture(&it->second);
			rect.setTextureRect({ 0, 0, 20, 20 });

			dock.addRectangle(rect);
		}
	}

	/**
	 * @fn	void Editor::entry() override
	 *
	 * @brief	Entry for the Editor state
	 *
	 * @author	Wiebe
	 * @date	29-1-2018
	 */

	void entry() override {
		camera.connect();

		// Bind events for each Body instance.
		for (const std::unique_ptr<Body>& physicsObject : map) {
			bindEditorEvents(*physicsObject);
		}

		objectAddedConn = map.objectAdded.connect([this](Body& physicsObject) {
			bindEditorEvents(physicsObject);
		});

		objectRemovedConn = map.objectRemoving.connect([this](Body& physicsObject) {
			unbindEditorEvents(physicsObject);
		});

		keyPressedConnection = game.keyboard.keyPressed.connect([this](sf::Keyboard::Key key) {
			if (key == sf::Keyboard::Key::LControl) {
				lControlPressed = true;
			}
		});

		keyReleasedConnection = game.keyboard.keyReleased.connect([this](sf::Keyboard::Key key) {
			if (key == sf::Keyboard::Key::Escape) {
				// Go to the main menu.
				statemachine.doTransition("main-menu");
			}
			else if (key == sf::Keyboard::Key::S && lControlPressed) {
				// Save the map file.
				std::cout << "Writing map to file...\n";
				mapFileGenerator.generate("map_generated.txt", map, player, death);

				lControlPressed = false;
			}
			else if (key == sf::Keyboard::Key::LControl) {
				// Left control key released
				lControlPressed = false;
			}
		});

		selection.deselect();
		selection.connect();
	}

	void exit() override {
		// Remove the camera from the window.
		camera.disconnect();
		camera.reset();

		selection.disconnect();

		objectAddedConn.disconnect();
		objectRemovedConn.disconnect();

		// Unbind all events for each Body.
		for (const std::unique_ptr<Body>& physicsObject : map) {
			unbindEditorEvents(*physicsObject);
		}

		keyReleasedConnection.disconnect();

		std::cout << "Writing map to file...\n";
		mapFileGenerator.generate("map_generated.txt", map, player, death);
	}

	/**
	 * @fn	void Editor::update(const float elapsedTime) override
	 *
	 * @brief	Updates the editor window.
	 *
	 * @author	Wiebe
	 * @date	29-1-2018
	 *
	 * @param	elapsedTime	The elapsed time.
	 */

	void update(const float elapsedTime) override {
		camera.update(elapsedTime);

		map.resolve();
		map.draw(statemachine.window);
		dock.draw();

		selection.update(elapsedTime);
		statemachine.window.draw(selection);

		// Draw mouse pointer.
		sf::RectangleShape rectShape;
		rectShape.setSize({ 10, 10 });
		rectShape.setPosition(game.window->mapPixelToCoords(sf::Mouse::getPosition(*game.window)));
		statemachine.window.draw(rectShape);
	}
};