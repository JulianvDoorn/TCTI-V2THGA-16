#pragma once

#include <SFML/Graphics.hpp>

/**
 * @class	KeyScheme
 *
 * @brief	A key scheme which can be used the change the player keyboard controls.
 *
 * @author	Wiebe
 * @date	25-1-2018
 */

class KeyScheme {
public:

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

	KeyScheme(sf::Keyboard::Key moveLeft, sf::Keyboard::Key moveRight, sf::Keyboard::Key jump, sf::Keyboard::Key roll, sf::Keyboard::Key run, Difficulty difficulty = Difficulty::UNDEFINED) :
		moveLeft(moveLeft),
		moveRight(moveRight),
		jump(jump),
		roll(roll),
		run(run),
		difficulty(difficulty)
	{};

	/**
	 * @fn	KeyScheme::KeyScheme()
	 *
	 * @brief	Default constructor
	 *
	 * @author	Wiebe
	 * @date	25-1-2018
	 */

	KeyScheme() { difficulty = Difficulty::UNDEFINED; };


	/** @brief	The difficulty */
	Difficulty difficulty;

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
