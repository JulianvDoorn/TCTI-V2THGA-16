#pragma once

#include <string>
#include <istream>

#include "BaseStreamExceptions.hpp"
#include "StreamManipulators.hpp"

/**
 * @class	ConsumeStringException
 *
 * @brief	Exception for signalling consume string errors.
 *
 * @author	Julian
 * @date	2018-01-25
 */

class ConsumeStringException : UnexpectedValueException {
public:
	ConsumeStringException(int64_t pos, const std::string& consumeString, const std::string& consumed) : UnexpectedValueException(pos, std::string("[") + consumeString + "] does not match [" + consumed + "]") { }
};

/**
 * @class	ConsumeString
 *
 * @brief	Used for matching an std::istream& reading with a certain string.
 * 
 * @detailed	When operator>>(std::istream&, const ConsumeString&) is used, the content of the readings from the stream aren't put into ConsumeString but instead are only read and matched.
 * 				When the readings doesn't match this string, a ConsumeString exception is thrown.
 *
 * @author	Julian
 * @date	2018-01-25
 */

class ConsumeString : public std::string {
public:
	ConsumeString(const std::string& str) : std::string(std::move(str)) { }
};

/**
 * @fn	std::istream& operator>> (std::istream& is, const ConsumeString& consumeString)
 *
 * @brief	Stream extraction operator
 * 			
 * @detailed Consumes consumeString from is. Once a mismatch or EOF is found, an exception is thrown.
 *
 * @author	Julian
 * @date	2018-01-25
 *
 * @exception	ConsumeStringException	Thrown when a Consume String error condition occurs.
 *
 * @param [in,out]	is			 	The is.
 * @param 		  	consumeString	The consume string.
 *
 * @return	The shifted result.
 */

std::istream& operator>> (std::istream& is, const ConsumeString& consumeString) {
	std::string consumed;

	int64_t pos = is.tellg();

	for (char rightChar : consumeString) {
		char leftChar;

		is >> std::noskipws >> exceptions >> leftChar;

		consumed.push_back(leftChar);

		if (leftChar != rightChar) {
			throw ConsumeStringException(pos, consumeString, consumed);
		}
	}

	return is;
}