#pragma once

#include <SFML/Graphics.hpp>

#include "Map.hpp"
#include "GameObject.hpp"

/**
 * @class	ObjectSelector
 *
 * @brief	Class responsible for selecting and manipulating objects.
 *
 * @author	Julian
 * @date	2018-01-30
 */

class ObjectSelector : public sf::Drawable, public GameObject {
	Body* selection;

	sf::Vector2f resizeOrigin;

	sf::Vector2i offset;
	bool dragging;

	sf::RectangleShape boundingBox;

	sf::CircleShape topResizeHandle;
	sf::CircleShape bottomResizeHandle;
	sf::CircleShape leftResizeHandle;
	sf::CircleShape rightResizeHandle;

	EventConnection keyReleasedConn;
	EventConnection mouseMovedConn;
	EventConnection mouseLeftButtonDown;
	EventConnection mouseLeftButtonUp;

	ResizeFace resizeDirection;

	Map& map;

public:

	/**
	 * @fn	ObjectSelector::ObjectSelector(Map& map)
	 *
	 * @brief	Constructor
	 * 
	 * Constructs an ObjectSelector for the given map.
	 *
	 * @author	Julian
	 * @date	2018-01-30
	 *
	 * @param [in,out]	map	The map.
	 */

	ObjectSelector(Map& map) :
		selection(nullptr),
		map(map)
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

	/**
	 * @fn	void ObjectSelector::select(Body* s)
	 *
	 * @brief	Selects the given Body*
	 *
	 * @author	Julian
	 * @date	2018-01-30
	 *
	 * @param [in,out]	s	If non-null, a Body to select.
	 */

	void select(Body* s) {
		selection = s;
		startDrag();

		sf::FloatRect bounds = selection->getGlobalBounds();

		boundingBox.setSize({ bounds.width, bounds.height });
		boundingBox.setPosition({ bounds.left, bounds.top });
		boundingBox.setOutlineColor(sf::Color::Cyan);
	}

	/**
	 * @fn	void ObjectSelector::select(Body& s)
	 *
	 * @brief	Selects the given Body&
	 *
	 * @author	Julian
	 * @date	2018-01-30
	 *
	 * @param [in,out]	s	A Body to select.
	 */

	void select(Body& s) {
		selection = &s;
		startDrag();

		sf::FloatRect bounds = selection->getGlobalBounds();

		boundingBox.setSize({ bounds.width, bounds.height });
		boundingBox.setPosition({ bounds.left, bounds.top });
		boundingBox.setOutlineColor(sf::Color::Cyan);
	}

	/**
	 * @fn	void ObjectSelector::deselect()
	 *
	 * @brief	Deselects the current selection
	 *
	 * @author	Julian
	 * @date	2018-01-30
	 */

	void deselect() {
		selection = nullptr;
	}

	/**
	 * @fn	void ObjectSelector::startDrag()
	 *
	 * @brief	Starts dragging the current selection.
	 *
	 * @author	Julian
	 * @date	2018-01-30
	 */

	void startDrag() {
		dragging = true;

		if (selection != nullptr) {
			offset = game.window->mapCoordsToPixel(selection->getPosition()) - sf::Mouse::getPosition(*game.window);
		}
	}

	/**
	 * @fn	void ObjectSelector::stopDrag()
	 *
	 * @brief	Stops dragging the current selection.
	 *
	 * @author	Julian
	 * @date	2018-01-30
	 */

	void stopDrag() {
		dragging = false;
	}

	/**
	 * @fn	void ObjectSelector::startResize(ResizeFace dir, sf::Vector2f origin)
	 *
	 * @brief	Starts resizing the current selection.
	 *
	 * @author	Julian
	 * @date	2018-01-30
	 *
	 * @param	dir   	The dir.
	 * @param	origin	The origin.
	 */

	void startResize(ResizeFace dir, sf::Vector2f origin) {
		resizeDirection = dir;
		resizeOrigin = origin;
	}

	/**
	 * @fn	void ObjectSelector::stopResize()
	 *
	 * @brief	Stops resizing the current selection.
	 *
	 * @author	Julian
	 * @date	2018-01-30
	 */

	void stopResize() {
		resizeDirection = ResizeFace::None;
	}

	/**
	 * @fn	void ObjectSelector::connect()
	 *
	 * @brief	Connects all the mouse and keyboard events of this object.
	 * 
	 * Connects game.keyboard.keyPressed. When Delete is pressed, delete the current selection.
	 * Mouse click and move events for selecting and dragging objects.
	 *
	 * @author	Julian
	 * @date	2018-01-30
	 */

	void connect() {
		keyReleasedConn = game.keyboard.keyPressed.connect([this](sf::Keyboard::Key key) {
			if (key == sf::Keyboard::Key::Delete) {
				if (selection != nullptr) {
					map.removeObject(selection);
					selection = nullptr;
				}
			}
		});

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

	/**
	 * @fn	void ObjectSelector::disconnect()
	 *
	 * @brief	Disconnects all the connected EventSources.
	 *
	 * @author	Julian
	 * @date	2018-01-30
	 */

	void disconnect() {
		mouseLeftButtonDown.disconnect();
		mouseLeftButtonUp.disconnect();
		mouseMovedConn.disconnect();
	}

	/**
	* @fn	void ObjectSelector::update(const float elapsedTime)
	*
	* @brief	Updates the object selecter with a given elapsedTime.
	*
	* Sets the colors and positions of all composite objects of ObjectSelector.
	*
	* @author	Julian
	* @date	2018-01-30
	*
	* @param	elapsedTime	The elapsed run-time.
	*/

	void update(const float elapsedTime) override {
		if (selection != nullptr) {
			sf::FloatRect bounds = selection->getGlobalBounds();

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

	void draw(sf::RenderTarget& renderTarget, sf::RenderStates renderStates) const override {
		renderTarget.draw(boundingBox);
		renderTarget.draw(topResizeHandle);
		renderTarget.draw(bottomResizeHandle);
		renderTarget.draw(leftResizeHandle);
		renderTarget.draw(rightResizeHandle);
	}
};