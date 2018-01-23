#pragma once

#include <exception>
#include <string>
#include <fstream>

/**
* @fn	std::istream& exceptions(std::istream& is)
*
* @brief	Turns on exceptions on the given input stream
*
* @author	Julian
* @date	2018-01-23
*
* @param [in,out]	is	The is.
*
* @return	std::istream& is
*/

std::istream& exceptions(std::istream& is) {
	is.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	return is;
}

/**
 * @class	BodyFactoryException
 *
 * @brief	Exception for signalling body factory errors.
 *
 * @author	Julian
 * @date	2018-01-23
 */

class BodyFactoryException : public std::exception {
	std::string err;
	
public:
	explicit BodyFactoryException(std::string s) : err(std::move(s)) { }
	
	const char* what() const noexcept final {
		return err.c_str();
	}
};

/**
 * @class	UnknownTypeException
 *
 * @brief	Exception for signalling unknown type errors.
 *
 * @author	Julian
 * @date	2018-01-23
 */

class UnknownTypeException : public BodyFactoryException {
public:
	explicit UnknownTypeException(const std::string& type) : BodyFactoryException(std::string("Unknown type [") + type + "]") { }
};

/**
 * @class	EOFException
 *
 * @brief	Exception for signalling EOF errors.
 *
 * @author	Julian
 * @date	2018-01-23
 */

class EOFException : public BodyFactoryException {
public:
	EOFException() : BodyFactoryException("End of file reached") { }
	
	explicit EOFException(const std::string& s) : BodyFactoryException(s + " end of file reached") { }
};

/**
 * @class	SyntaxException
 *
 * @brief	Exception for signalling syntax errors.
 *
 * @author	Julian
 * @date	2018-01-23
 */

class SyntaxException : public BodyFactoryException {
public:
	explicit SyntaxException(const std::string& s) : BodyFactoryException(s) { }
};

/**
 * @class	UnexpectedValueException
 *
 * @brief	Exception for signalling unexpected value errors.
 *
 * @author	Julian
 * @date	2018-01-23
 */

class UnexpectedValueException : public BodyFactoryException {
public:
	explicit UnexpectedValueException(int64_t position, const std::string& unexpectedSymbol) : BodyFactoryException(std::string("Unexpected value at [") + std::to_string(position) + "] got: " + unexpectedSymbol) { }
};

/**
 * @class	OpeningBracketMissingException
 *
 * @brief	Exception for signalling when an opening bracket ( is expected but missed.
 *
 * @author	Julian
 * @date	2018-01-23
 */

class OpeningBracketMissingException : public SyntaxException {
public:
	explicit OpeningBracketMissingException(int64_t position) : SyntaxException(std::string("Opening bracket missing at [") + std::to_string(position) + "]") { }
	explicit OpeningBracketMissingException(int64_t position, const std::string& unexpectedSymbol) : SyntaxException(std::string("Opening bracket missing at [") + std::to_string(position) + "] unexpected symbol: " + unexpectedSymbol) { }
};

/**
 * @class	ClosingBracketMissingException
 *
 * @brief	Exception for signalling when a closing bracket ) is expected but missed.
 *
 * @author	Julian
 * @date	2018-01-23
 */

class ClosingBracketMissingException : public SyntaxException {
public:
	explicit ClosingBracketMissingException(int64_t position) : SyntaxException(std::string("Closing bracket missing at [") + std::to_string(position) + "]") { }
	explicit ClosingBracketMissingException(int64_t position, const std::string& unexpectedSymbol) : SyntaxException(std::string("Closing bracket missing at [") + std::to_string(position) + "] unexpected symbol: " + unexpectedSymbol) { }
};

/**
 * @class	CommaMissingException
 *
 * @brief	Exception for signalling when a comma , is expected but missed.
 *
 * @author	Julian
 * @date	2018-01-23
 */

class CommaMissingException : public SyntaxException {
public:
	explicit CommaMissingException(int64_t position) : SyntaxException(std::string("Comma missing at [") + std::to_string(position) + "]") { }
	explicit CommaMissingException(int64_t position, const std::string& unexpectedSymbol) : SyntaxException(std::string("Comma missing at [") + std::to_string(position) + "] unexpected symbol: " + unexpectedSymbol) { }
};

/**
 * @class	QuoteMissingException
 *
 * @brief	Exception for signalling when a quote " is expected but missed.
 *
 * @author	Julian
 * @date	2018-01-23
 */

class QuoteMissingException : public SyntaxException {
public:
	explicit QuoteMissingException(int64_t position) : SyntaxException(std::string("Quote missing at [") + std::to_string(position) + "]") { }
	explicit QuoteMissingException(int64_t position, const std::string& unexpectedSymbol) : SyntaxException(std::string("Quote missing at [") + std::to_string(position) + "] unexpected symbol: " + unexpectedSymbol) { }
};

/**
 * @class	HashtagMissingException
 *
 * @brief	Exception for signalling when a hashtag # is expected but missed.
 *
 * @author	Julian
 * @date	2018-01-23
 */

class HashtagMissingException : public SyntaxException {
public:
	explicit HashtagMissingException(int64_t position) : SyntaxException(std::string("Quote missing at [") + std::to_string(position) + "]") { }
	explicit HashtagMissingException(int64_t position, const std::string& unexpectedSymbol) : SyntaxException(std::string("Quote missing at [") + std::to_string(position) + "] unexpected symbol: " + unexpectedSymbol) { }
};

/**
 * @class	AssignmentMissingException
 *
 * @brief	Exception for signalling when a assignment = is expected but missed.
 *
 * @author	Julian
 * @date	2018-01-23
 */

class AssignmentMissingException : public SyntaxException {
public:
	explicit AssignmentMissingException(int64_t position) : SyntaxException(std::string("Assignment (=) missing at [") + std::to_string(position) + "]") { }
	explicit AssignmentMissingException(int64_t position, const std::string& unexpectedSymbol) : SyntaxException(std::string("Assignment (=) missing at [") + std::to_string(position) + "] unexpected symbol: " + unexpectedSymbol) { }
};