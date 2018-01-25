#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

#include "ConsumeString.hpp"

/**
* @fn	template<class T> std::ostream& operator<< (std::ostream& os, const sf::Vector2<T>& v)
*
* @brief	Stream insertion operator for sf::Vector2<T>
* 
* @detailed Writes the sf::Vector2<T> to os in the following format: Vector2({X}, {Y})
* 
* @author	Julian
* @date	2018-01-25
*
* @tparam	T	Generic type parameter.
* @param [in,out]	os	Output stream
* @param [in,out]	v 	A sf::Vector2<T> to write to os.
*
* @return	std::ostream& os
*/

template<class T>
std::ostream& operator<< (std::ostream& os, const sf::Vector2<T>& v) {
	return os << "Vector2(" << v.x << ", " << v.y << ')';
}

/**
 * @fn	template<class T> std::istream& operator>> (std::istream& is, sf::Vector2<T>& v)
 *
 * @brief	Stream extraction operator for sf::Vector2<T>
 * 
 * @detailed Reads the sf::Vector2<T> from is in the following format: Vector2({X}, {Y})
 * 			 Throws an exception when the format is incorrect.
 *
 * @author	Julian
 * @date	2018-01-25
 *
 * @tparam	T	Generic type parameter.
 * @param [in,out]	is	Input stream
 * @param [in,out]	v 	A sf::Vector2<T> to read from is.
 *
 * @return	std::istream& is
 */

template<class T>
std::istream& operator>> (std::istream& is, sf::Vector2<T>& v) {
	is >> std::ws >> ConsumeString("Vector2");

	SecureRead<T> x;
	SecureRead<T> y;

	is >> std::ws >> SpecialCharacter::LeftBracket;
	is >> std::ws >> x;
	is >> std::ws >> SpecialCharacter::Comma;
	is >> std::ws >> y;
	is >> std::ws >> SpecialCharacter::RightBracket;

	v = sf::Vector2<T>(x, y);

	return is;
}