#pragma once

#include <SFML\Graphics.hpp>

#include "Events.hpp"

/**
 * @class	FreeCamera
 *
 * @brief	Freely moving camera using WASD
 * 
 * Freely moving camera. Reads inputs from game.keyboard and places the view anywhere on the game.
 * Usage:
 * @code
 * RenderWindow myRenderWindow = getSomeRenderWindow();
 * FreeCamera myCamera(myRenderWindow, 100); // constructs a free camera for myRenderWindow with speed = 100
 * myCamera.connect(); // tell FreeCamera to start listening for key events
 * 
 * while (running) { // game loop
 *	myCamera.update(); // update camera every frame
 *	sleep_ms(1/30);
 * }
 * 
 * myCamera.disconnect(); // tell FreeCamera to stop listening for key events
 * myCamera.reset(); // reset the view of myRenderWindow to the original state
 * @endcode
 *
 * @author	Julian
 * @date	2018-01-30
 */

class FreeCamera {
	sf::View view;
	sf::RenderWindow& window;

	float speed;
	sf::Vector2i movement = { 0, 0 };
	sf::Vector2f position = { 0, 0 };

	EventConnection keyPressedConnection;
	EventConnection keyReleasedConnection;

public:

	/**
	 * @fn	FreeCamera::FreeCamera(sf::RenderWindow& window) : view(sf::Vector2f(), static_cast<sf::Vector2f>(window.getSize())), window(window)
	 *
	 * @brief	Constructor
	 * 
	 * Constructs a FreeCamera with an undefined speed. Speed must be set before using the FreeCamera
	 *
	 * @author	Julian
	 * @date	2018-01-30
	 *
	 * @param [in,out]	window	The window.
	 */

	FreeCamera(sf::RenderWindow& window) : view(sf::Vector2f(), static_cast<sf::Vector2f>(window.getSize())), window(window) { }

	/**
	 * @fn	FreeCamera::FreeCamera(sf::RenderWindow& window, float cameraSpeed) : view(sf::Vector2f(), static_cast<sf::Vector2f>(window.getSize())), window(window), speed(cameraSpeed)
	 *
	 * @brief	Constructor
	 * 
	 * Constructs a FreeCamera with a determined speed.
	 * The FreeCamera is ready to listen for events and update cycles.
	 *
	 * @author	Julian
	 * @date	2018-01-30
	 *
	 * @param [in,out]	window	   	The window.
	 * @param 		  	cameraSpeed	The camera speed.
	 */

	FreeCamera(sf::RenderWindow& window, float cameraSpeed) : view(sf::Vector2f(), static_cast<sf::Vector2f>(window.getSize())), window(window), speed(cameraSpeed) { }

	/**
	 * @fn	void FreeCamera::connect()
	 *
	 * @brief	Connects the FreeCamera to the game.keyboard EventSources.
	 * 
	 * The keys W, A, S and D adds/subtracts to/from the movement vector2.
	 *
	 * @author	Julian
	 * @date	2018-01-30
	 */

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

	/**
	 * @fn	void FreeCamera::disconnect()
	 *
	 * @brief	Disconnects the FreeCamera from the game.keyboard EventSources.
	 *
	 * @author	Julian
	 * @date	2018-01-30
	 */

	void disconnect() {
		keyPressedConnection.disconnect();
		keyReleasedConnection.disconnect();
	}

	/**
	 * @fn	void FreeCamera::setCameraSpeed(float cameraSpeed)
	 *
	 * @brief	Sets the camera movement speed
	 *
	 * @author	Julian
	 * @date	2018-01-30
	 *
	 * @param	cameraSpeed	The camera speed.
	 */

	void setCameraSpeed(float cameraSpeed) {
		speed = cameraSpeed;
	}

	/**
	 * @fn	void FreeCamera::setPosition(sf::Vector2f pos)
	 *
	 * @brief	Sets the camera position to the given pos.
	 *
	 * @author	Julian
	 * @date	2018-01-30
	 *
	 * @param	pos	The position.
	 */

	void setPosition(sf::Vector2f pos) {
		position = pos;
	}

	/**
	 * @fn	void FreeCamera::update(const float elapsedTime)
	 *
	 * @brief	Updates the camera with a given elapsedTime.
	 * 
	 * Every cycle (movement * speed * elapsedTime) is added to position.
	 *
	 * @author	Julian
	 * @date	2018-01-30
	 *
	 * @param	elapsedTime	The elapsed run-time.
	 */

	void update(const float elapsedTime) {
		position += static_cast<sf::Vector2f>(movement) * speed * elapsedTime;

		view.setCenter(position);
		window.setView(view);
	}

	/**
	 * @fn	void FreeCamera::reset()
	 *
	 * @brief	Resets the camera view to the original state.
	 *
	 * @author	Julian
	 * @date	2018-01-30
	 */

	void reset() {
		view.setCenter(window.getDefaultView().getCenter());
		window.setView(view);
	}
};