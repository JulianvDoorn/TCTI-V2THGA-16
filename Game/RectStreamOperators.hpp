#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

#include "ConsumeString.hpp"

/**
* @fn	template<class T> std::ostream& operator<< (std::ostream& os, const sf::Rect<T>& v)
*
* @brief	Stream insertion operator for sf::Rect<T>
*
* @detailed Writes the sf::Rect<T> to os in the following format: Rect({L}, {T}, {W}, {H})
*
* @author	Julian
* @date	2018-01-25
*
* @tparam	T	Generic type parameter.
* @param [in,out]	os	Output stream
* @param [in,out]	v 	A sf::Rect<T> to write to os.
*
* @return	std::ostream& os
*/

template<class T>
std::ostream& operator<< (std::ostream& os, sf::Rect<T>& vectorValue) {
	return os << "Rect(" << vectorValue.left << ", " << vectorValue.top << ", " << vectorValue.width << ", " << vectorValue.height << ')';
}

/**
* @fn	template<class T> std::istream& operator>> (std::istream& is, sf::Rect<T>& v)
*
* @brief	Stream extraction operator for sf::Rect<T>
*
* @detailed Reads the sf::Rect<T> from is in the following format: Rect({L}, {T}, {W}, {H})
* 			 Throws an exception when the format is incorrect.
*
* @author	Julian
* @date	2018-01-25
*
* @tparam	T	Generic type parameter.
* @param [in,out]	is	Input stream
* @param [in,out]	v 	A sf::Rect<T> to read from is.
*
* @return	std::istream& is
*/

template<class T>
std::istream& operator>> (std::istream& is, sf::Rect<T>& vectorValue) {
	is >> std::ws >> ConsumeString("Rect");

	SecureRead<T> left;
	SecureRead<T> top;
	SecureRead<T> width;
	SecureRead<T> height;

	is >> std::ws >> SpecialCharacter::LeftBracket;
	is >> std::ws >> left;
	is >> std::ws >> SpecialCharacter::Comma;
	is >> std::ws >> top;
	is >> std::ws >> SpecialCharacter::Comma;
	is >> std::ws >> width;
	is >> std::ws >> SpecialCharacter::Comma;
	is >> std::ws >> height;
	is >> std::ws >> SpecialCharacter::RightBracket;

	vectorValue = sf::Rect<T>(left, top, width, height);

	return is;
}