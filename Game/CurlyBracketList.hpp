#pragma once

#include <string>
#include <sstream>
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

std::istream& ignoreLine(std::istream& is)
{
	// skip until end-of-line
	is.ignore(std::numeric_limits<int>::max(), is.widen('\n'));
	return is;
}

/**
 * @class	CurlyBracketList
 *
 * @brief	List of T's.
 *
 * @author	Julian
 * @date	2018-01-23
 *
 * @tparam	T	Generic type parameter.
 */

template<class T>
class CurlyBracketList : public std::vector<T> { };

/**
 * @fn	template<class T> std::istream& operator>> (std::istream& is, CurlyBracketList<T>& list)
 *
 * @brief	Stream extraction operator
 * 			
 * @detailed Reads a {, then continuously reads T's using operator>>(std::istream&, T&) until a } has been found. Each T has to be placed on a separate line.
 * 			 Example reading:
 * 			 {
 * 			 1
 * 			 2
 * 			 3
 * 			 }
 * 			 This reads a vector with values { 1, 2, 3 }
 *
 * @author	Julian
 * @date	2018-01-23
 *
 * @tparam	T	Generic type parameter.
 * @param [in,out]	is  	The is.
 * @param [in,out]	list	The list.
 *
 * @return	std::istream& is
 */

template<class T>
std::istream& operator>> (std::istream& is, CurlyBracketList<T>& list) {
	is >> exceptions >> SpecialCharacter::LeftCurlyBracket;
	is >> exceptions >> ignoreLine;

	while (true) {
		std::string line;
		getline(is, line);
		std::istringstream iss(line);

		char c;
		iss >> exceptions >> c;

		if (c != SpecialCharacter::RightCurlyBracket) {
			iss.seekg(0);
			list.push_back(T());
			iss >> exceptions >> list.back();
		}
		else {
			// } found
			break;
		}
	}

	return is;
}