#pragma once

#include <istream>

/**
* @fn	std::istream& ignoreLine(std::istream& is)
*
* @brief	Read until end-of-line on the given std::istream& is
*
* @author	Julian
* @date	2018-01-23
*
* @param [in,out]	is	Input stream
*
* @return	std::istream& is
*/

std::istream& ignoreLine(std::istream& is) {
	// skip until end-of-line
	is.ignore(std::numeric_limits<int>::max(), is.widen('\n'));
	return is;
}

/**
* @fn	std::istream& exceptions(std::istream& is)
*
* @brief	Turns on exceptions on the given input stream
*
* @author	Julian
* @date	2018-01-23
*
* @param [in,out]	is	Input stream
*
* @return	std::istream& is
*/

std::istream& exceptions(std::istream& is) {
	is.exceptions(std::istream::failbit | std::istream::badbit);

	return is;
}