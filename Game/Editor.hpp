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
#include "AssetFileGenerator.hpp"
#include "ShapeFileGenerator.hpp"
#include "VectorMath.hpp"

#include "VectorStreamOperators.hpp"

class Selection : public Drawable {
	PhysicsObject* selection;

	sf::Vector2f resizeOrigin;

	sf::Vector2i offset;
	bool dragging;

	sf::RectangleShape boundingBox;

	sf::CircleShape topResizeHandle;
	sf::CircleShape bottomResizeHandle;
	sf::CircleShape leftResizeHandle;
	sf::CircleShape rightResizeHandle;

	EventConnection mouseMovedConn;
	EventConnection mouseLeftButtonDown;
	EventConnection mouseLeftButtonUp;

	ResizeFace resizeDirection;

public:
	Selection() :
		Drawable(boundingBox),
		selection(nullptr)
	{
		boundingBox.setFillColor(sf::Color::Transparent);
		boundingBox.setOutlineColor(sf::Color::Cyan);
		boundingBox.setOutlineThickness(2.0f);

		topResizeHandle.setFillColor(sf::Color::Cyan);
		topResizeHandle.setRadius(6.0f);
		topResizeHandle.setOrigin({ 6.0f, 6.0f + 10.0f });

		bottomResizeHandle.setFillColor(sf::Color::Cyan);
		bottomResizeHandle.setRadius(6.0f);
		bottomResizeHandle.setOrigin({ 6.0f, 6.0f - 10.0f });

		leftResizeHandle.setFillColor(sf::Color::Cyan);
		leftResizeHandle.setRadius(6.0f);
		leftResizeHandle.setOrigin({ 6.0f + 10.0f, 6.0f });

		rightResizeHandle.setFillColor(sf::Color::Cyan);
		rightResizeHandle.setRadius(6.0f);
		rightResizeHandle.setOrigin({ 6.0f - 10.0f, 6.0f });
	}

	void select(PhysicsObject* s) {
		selection = s;
		startDrag();
	}

	void deselect() {
		selection = nullptr;
	}

	void startDrag() {
		dragging = true;

		if (selection != nullptr) {
			offset = game.window->mapCoordsToPixel(selection->getPosition()) - sf::Mouse::getPosition(*game.window);
		}
	}

	void stopDrag() {
		dragging = false;
	}

	void startResize(ResizeFace dir, sf::Vector2f origin) {
		resizeDirection = dir;
		resizeOrigin = origin;
	}

	void stopResize() {
		resizeDirection = ResizeFace::None;
	}

	void connect() {
		mouseLeftButtonDown = game.mouse.mouseLeftButtonDown.connect([this](sf::Vector2i pos) {
			sf::Vector2f posCoords = game.window->mapPixelToCoords(pos);

			if (boundingBox.getGlobalBounds().contains(posCoords)) {
				startDrag();
			}
			else if (topResizeHandle.getGlobalBounds().contains(posCoords)) {
				startResize(ResizeFace::Top, posCoords);
			}
			else if (bottomResizeHandle.getGlobalBounds().contains(posCoords)) {
				startResize(ResizeFace::Bottom, posCoords);
			}
			else if (leftResizeHandle.getGlobalBounds().contains(posCoords)) {
				startResize(ResizeFace::Left, posCoords);
			}
			else if (rightResizeHandle.getGlobalBounds().contains(posCoords)) {
				startResize(ResizeFace::Right, posCoords);
			}
			else {
				deselect();
			}
		});

		mouseLeftButtonUp = game.mouse.mouseLeftButtonUp.connect([this](sf::Vector2i pos) {
			stopDrag();
			stopResize();
		});

		mouseMovedConn = game.mouse.mouseMoved.connect([this](sf::Vector2i pos) {
			if (selection != nullptr) {
				if (dragging == true) {
					selection->setPosition(game.window->mapPixelToCoords(sf::Mouse::getPosition(*game.window)) + static_cast<sf::Vector2f>(offset));
				}
				else if (resizeDirection != ResizeFace::None) {
					sf::Vector2f mousePos = game.window->mapPixelToCoords(sf::Mouse::getPosition(*game.window));

					if (resizeDirection == ResizeFace::Top) {
						selection->resize(resizeOrigin.y - mousePos.y, ResizeFace::Top);
					}
					else if (resizeDirection == ResizeFace::Bottom) {
						selection->resize(mousePos.y - resizeOrigin.y, ResizeFace::Bottom);
					}
					else if (resizeDirection == ResizeFace::Left) {
						selection->resize(resizeOrigin.x - mousePos.x, ResizeFace::Left);
					}
					else if (resizeDirection == ResizeFace::Right) {
						selection->resize(mousePos.x - resizeOrigin.x, ResizeFace::Right);
					}
					
					resizeOrigin = mousePos;
				}
			}
		});
	}

	void disconnect() {
		mouseMovedConn.disconnect();
	}

	void update(const float elapsedTime) override {
		if (selection != nullptr) {
			sf::FloatRect bounds = selection->getBounds();

			boundingBox.setSize({ bounds.width, bounds.height });
			boundingBox.setPosition({ bounds.left, bounds.top });
			boundingBox.setOutlineColor(sf::Color::Cyan);

			topResizeHandle.setFillColor(sf::Color::Cyan);
			bottomResizeHandle.setFillColor(sf::Color::Cyan);
			leftResizeHandle.setFillColor(sf::Color::Cyan);
			rightResizeHandle.setFillColor(sf::Color::Cyan);

			topResizeHandle.setPosition({ bounds.left + bounds.width / 2, bounds.top });
			bottomResizeHandle.setPosition({ bounds.left + bounds.width / 2, bounds.top + bounds.height });
			leftResizeHandle.setPosition({ bounds.left, bounds.top + bounds.height / 2 });
			rightResizeHandle.setPosition({ bounds.left + bounds.width, bounds.top + bounds.height / 2});
		}
		else {
			boundingBox.setOutlineColor(sf::Color::Transparent);
			topResizeHandle.setFillColor(sf::Color::Transparent);
			bottomResizeHandle.setFillColor(sf::Color::Transparent);
			leftResizeHandle.setFillColor(sf::Color::Transparent);
			rightResizeHandle.setFillColor(sf::Color::Transparent);
		}
	};

	void draw(sf::RenderTarget& renderTarget) override {
		Drawable::draw(renderTarget);

		renderTarget.draw(topResizeHandle);
		renderTarget.draw(bottomResizeHandle);
		renderTarget.draw(leftResizeHandle);
		renderTarget.draw(rightResizeHandle);
	}
};















class FreeCamera {
	sf::View view;
	sf::RenderWindow& window;

	float speed;
	sf::Vector2i movement = { 0, 0 };
	sf::Vector2f position = { 0, 0 };

	EventConnection keyPressedConnection;
	EventConnection keyReleasedConnection;

public:
	FreeCamera(sf::RenderWindow& window) : view(sf::Vector2f(), static_cast<sf::Vector2f>(window.getSize())), window(window) { }

	FreeCamera(sf::RenderWindow& window, float cameraSpeed) : view(sf::Vector2f(), static_cast<sf::Vector2f>(window.getSize())), window(window), speed(cameraSpeed) { }

	void connect() {
		keyPressedConnection = game.keyboard.keyPressed.connect([this](sf::Keyboard::Key key) {
			if (key == sf::Keyboard::Key::W) {
				movement += { 0, -1 };
			}
			else if (key == sf::Keyboard::Key::A) {
				movement += { -1, 0 };
			}
			else if (key == sf::Keyboard::Key::S) {
				movement += { 0, 1 };

			}
			else if (key == sf::Keyboard::Key::D) {
				movement += { 1, 0 };
			}
		});

		keyReleasedConnection = game.keyboard.keyReleased.connect([this](sf::Keyboard::Key key) {
			if (key == sf::Keyboard::Key::W) {
				movement -= { 0, -1 };
			}
			else if (key == sf::Keyboard::Key::A) {
				movement -= { -1, 0 };

			}
			else if (key == sf::Keyboard::Key::S) {
				movement -= { 0, 1 };

			}
			else if (key == sf::Keyboard::Key::D) {
				movement -= { 1, 0 };
			}
		});
	}

	void disconnect() {
		keyPressedConnection.disconnect();
		keyReleasedConnection.disconnect();
	}

	void setCameraSpeed(float cameraSpeed) {
		speed = cameraSpeed;
	}

	void setPosition(sf::Vector2f pos) {
		position = pos;
	}

	void update(const float elapsedTime) {
		position += static_cast<sf::Vector2f>(movement) * speed * elapsedTime;

		view.setCenter(position);
		window.setView(view);
	}

	void reset() {
		view.setCenter(window.getDefaultView().getCenter());
		window.setView(view);
	}
};

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
	EventConnectionVector physicsObjectConnVector;

	Selection selection;

public:

	Editor(Statemachine& statemachine) :
		statemachine(statemachine),
		rectContainer(statemachine.window),
		dock(rectContainer, statemachine.window),
		assetFileGenerator(fileOut),
		shapeFileGenerator(fileOut),
		camera(statemachine.window, 100)
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

		mapFactory.registerCreateMethod("power-up", [&](Map& map, const MapItemProperties& properties) {
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

		for (PhysicsObject* physicsObject : map) {
			physicsObject->bindMouseEvents();

			physicsObjectConnVector.connect(physicsObject->mouseLeftButtonDown, [physicsObject, this]() {
				selection.select(physicsObject);
			});

			physicsObjectConnVector.connect(physicsObject->mouseLeftButtonUp, [&physicsObject, this]() {
				selection.stopDrag();
			});
		}
	}

	void exit() override {
		camera.disconnect();
		camera.reset();

		selection.disconnect();

		physicsObjectConnVector.disconnect();

		for (PhysicsObject* physicsObject : map) {
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