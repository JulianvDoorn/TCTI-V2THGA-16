#pragma once

#include <SFML/Graphics.hpp>

#include "Drawable.hpp"
#include "PhysicsObject.hpp"

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

		sf::FloatRect bounds = selection->getBounds();

		boundingBox.setSize({ bounds.width, bounds.height });
		boundingBox.setPosition({ bounds.left, bounds.top });
		boundingBox.setOutlineColor(sf::Color::Cyan);
	}

	void select(PhysicsObject& s) {
		selection = &s;
		startDrag();

		sf::FloatRect bounds = selection->getBounds();

		boundingBox.setSize({ bounds.width, bounds.height });
		boundingBox.setPosition({ bounds.left, bounds.top });
		boundingBox.setOutlineColor(sf::Color::Cyan);
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

			if (!dragging) {
				topResizeHandle.setFillColor(sf::Color::Cyan);
				bottomResizeHandle.setFillColor(sf::Color::Cyan);
				leftResizeHandle.setFillColor(sf::Color::Cyan);
				rightResizeHandle.setFillColor(sf::Color::Cyan);

				topResizeHandle.setPosition({ bounds.left + bounds.width / 2, bounds.top });
				bottomResizeHandle.setPosition({ bounds.left + bounds.width / 2, bounds.top + bounds.height });
				leftResizeHandle.setPosition({ bounds.left, bounds.top + bounds.height / 2 });
				rightResizeHandle.setPosition({ bounds.left + bounds.width, bounds.top + bounds.height / 2 });
			}
			else {
				topResizeHandle.setFillColor(sf::Color::Transparent);
				bottomResizeHandle.setFillColor(sf::Color::Transparent);
				leftResizeHandle.setFillColor(sf::Color::Transparent);
				rightResizeHandle.setFillColor(sf::Color::Transparent);
			}
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