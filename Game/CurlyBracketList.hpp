#pragma once

#include <string>
#include <sstream>
#include <istream>

#include "BaseStreamExceptions.hpp"

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

/**
 * @fn	std::istream& operator>> (std::istream& is, CurlyBracketList<T>& list)
 *
 * @brief	Stream extraction operator
 *
 * @author	Jeffrey
 * @date	1/31/2018
 *
 * @exception	ClosingBracketMissingException	Thrown when a Closing Bracket Missing error
 * 												condition occurs.
 *
 * @param [in,out]	is  	The is.
 * @param [in,out]	list	The list.
 *
 * @return	The shifted result.
 */

std::istream& operator>> (std::istream& is, CurlyBracketList<T>& list) {
	try {
		is >> exceptions >> SpecialCharacter::LeftCurlyBracket;
	}
	catch (const OpeningBracketMissingException&) {
		is >> exceptions >> SpecialCharacter::LeftCurlyBracket;
	}

	is >> exceptions >> ignoreLine;

	while (true) {
		int64_t pos = is.tellg();

		std::string line;
		getline(is, line);
		std::istringstream iss(line);

		char c;
		
		try {
			iss >> exceptions >> c;
		}
		catch (const std::ios::failure&) {
			is.seekg(pos);

			std::string unexpectedSymbol;
			is >> unexpectedSymbol;

			throw ClosingBracketMissingException(pos, unexpectedSymbol);
		}


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