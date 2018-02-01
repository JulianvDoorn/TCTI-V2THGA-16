#pragma once

#include <SFML/Graphics.hpp>

/**
 * @class	KeySchemeNotFoundException
 *
 * @brief	Exception for signalling key scheme not found errors.
 *
 * @author	Wiebe
 * @date	25-1-2018
 */

class KeySchemeNotFoundException : public std::exception {
public:

	/**
	 * @fn	const char* KeySchemeNotFoundException::what() const noexcept
	 *
	 * @brief	Gets the exception message.
	 *
	 * @author	Wiebe
	 * @date	25-1-2018
	 *
	 * @return	Null if it fails, else a pointer to a const char.
	 */

	const char* what() const noexcept {
		return "KeyScheme with given difficulty cannot been found!";
	}
};

/**
 * @class	KeyScheme
 *
 * @brief	A key scheme which can be used the change the player keyboard controls.
 *
 * @author	Wiebe
 * @date	25-1-2018
 */

class KeyScheme {
	EventConnection keyPressedConn;
	EventConnection keyReleasedConn;

public:
	/** @brief	The move left key */
	sf::Keyboard::Key moveLeft;

	/** @brief	The move right key */
	sf::Keyboard::Key moveRight;

	/** @brief	The jump key */
	sf::Keyboard::Key jump;

	/** @brief	The roll key */
	sf::Keyboard::Key roll;

	/** @brief	The run key */
	sf::Keyboard::Key run;

	/**
	* @enum	Difficulty
	*
	* @brief	Values that represent difficulties for various key schemes.
	*/

	enum class Difficulty {
		EASY,
		MODERATE,
		HARD,
		UNDEFINED
	};

	/** @brief	The difficulty */
	Difficulty difficulty;

	bool moveLeftPressed = false;
	bool moveRightPressed = false;
	bool jumpPressed = false;
	bool rollPressed = false;
	bool runPressed = false;

	/**
	 * @fn	KeyScheme::KeyScheme(sf::Keyboard::Key moveLeft, sf::Keyboard::Key moveRight, sf::Keyboard::Key jump, sf::Keyboard::Key roll, sf::Keyboard::Key run, Difficulty difficulty = Difficulty::UNDEFINED)
	 *
	 * @brief	Constructor
	 *
	 * @author	Wiebe
	 * @date	25-1-2018
	 *
	 * @param	moveLeft  	The move left key.
	 * @param	moveRight 	The move right key.
	 * @param	jump	  	The jump key.
	 * @param	roll	  	The roll key.
	 * @param	run		  	The run key.
	 * @param	difficulty	(Optional) The difficulty for the KeySchemes. Default is Difficulty::UNDEFINED
	 */

	KeyScheme(const sf::Keyboard::Key moveLeft, const sf::Keyboard::Key moveRight, const sf::Keyboard::Key jump, const sf::Keyboard::Key roll, const sf::Keyboard::Key run, Difficulty difficulty = Difficulty::UNDEFINED) :
		moveLeft(moveLeft),
		moveRight(moveRight),
		jump(jump),
		roll(roll),
		run(run),
		difficulty(difficulty)
	{ };

	void connect() {
		keyPressedConn = game.keyboard.keyPressed.connect([=](const sf::Keyboard::Key key) {
			if (key == jump) {
				jumpPressed = true;
			}
			else if (key == roll) {
				rollPressed = true;
			}
			else if (key == run) {
				runPressed = true;
			}
			else if (key == moveLeft) {
				moveLeftPressed = true;
			}
			else if (key == moveRight) {
				moveRightPressed = true;
			}
		});

		keyReleasedConn = game.keyboard.keyReleased.connect([=](const sf::Keyboard::Key key) {
			if (key == jump) {
				jumpPressed = false;
			}
			else if (key == roll) {
				rollPressed = false;
			}
			else if (key == run) {
				runPressed = false;
			}
			else if (key == moveLeft) {
				moveLeftPressed = false;
			}
			else if (key == moveRight) {
				moveRightPressed = false;
			}
		});
	}

	void disconnect() {
		keyPressedConn.disconnect();
		keyReleasedConn.disconnect();
	}

   /**
	* @fn	friend operator<<(std::ostream &os, sf::Keyboard::Key key)
	*
	* @brief	Used the print sf::Keyboard::key instances as an character.
	* 			
	* @param	os  	The output stream.
	* @param	key 	An key.
	*
	* @author	Wiebe
	* @date	25-1-2018
	*/
	friend std::ostream& operator<<(std::ostream &os, sf::Keyboard::Key key);
};


/**
* @fn	operator<<(std::ostream &os, sf::Keyboard::Key key)
*
* @brief	Used the print sf::Keyboard::key instances as an character.
*
* @param	os  	The output stream.
* @param	key 	An key.
*
* @author	Wiebe
* @date	25-1-2018
*/
std::ostream& operator<<(std::ostream &os, sf::Keyboard::Key key) {
	os << static_cast<char>('A' + key);
	return os;
}
