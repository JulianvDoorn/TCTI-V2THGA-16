#pragma once

#include <string>
#include <istream>

#include "SpecialCharacter.hpp"
#include "BaseStreamExceptions.hpp"
#include "StreamManipulators.hpp"

/**
 * @class	QuotedString
 *
 * @brief	String subclass that overloads operator>>(std::istream&, QuotedString).
 *
 * @author	Julian
 * @date	2018-01-23
 */

class QuotedString : public std::string {
public:
	QuotedString() : std::string() { }
	QuotedString(std::string str) : std::string(std::move(str)) { }
};

/**
 * @fn	std::istream& operator>> (std::istream& is, QuotedString& qs)
 *
 * @brief	Stream extraction operator
 * 			
 * @detailed Used for extracting a string of characters encapsulated by "...".
 * 			 Useful for reading exact values that are space sensitive and/or unknown where they exactly end.
 * 			 
 * 			 Usage: @code{.cpp}
 * 			 QuotedString quotedString;
 * 			 std::stringstream ss("\"some Text Encapsulated By Quote Chars\"");
 * 			 ss >> quotedString;
 * 			 std::cout << quotedString << std::endl; // should print someTextEncapsulatedByQuoteChars WITHOUT the quotes
 * 			 @endcode
 * 
 * @author	Julian
 * @date	2018-01-23
 *
 * @exception	EOFException		 	Thrown when an EOF error condition occurs.
 * @exception	QuoteMissingException	Thrown when a Quote Missing error condition occurs.
 *
 * @param [in,out]	is	The input stream.
 * @param [in,out]	qs	The quoted string.
 *
 * @return	std::istream reference to is
 */

std::istream& operator>> (std::istream& is, QuotedString& qs) {
	is >> SpecialCharacter::Quote;
	
	int nextc = is.peek();
	
	while (nextc != SpecialCharacter::Quote && nextc != '\n') {
		char c;

		try {
			is >> exceptions >> std::noskipws >> c;
		} catch (const std::istream::failure&) {
			throw EOFException("Unexpected");
		}
		
		qs.push_back(c);
		nextc = is.peek();
	}

	if (nextc == '\n') {
		throw QuoteMissingException(is.tellg());
	}
	
	is >> exceptions >> std::noskipws >> SpecialCharacter::Quote;
	
	return is;
}