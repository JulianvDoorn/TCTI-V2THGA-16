#pragma once

#include <exception>
#include <string>
#include <fstream>

/**
 * @class	StreamReadException
 *
 * @brief	Exception for signalling body factory errors.
 *
 * @author	Julian
 * @date	2018-01-23
 */

class StreamReadException : public std::exception {
	std::string err;
	
public:
	explicit StreamReadException(std::string s) : err(std::move(s)) { }
	
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

class UnknownTypeException : public StreamReadException {
public:
	explicit UnknownTypeException(const std::string& type) : StreamReadException(std::string("Unknown type [") + type + "]") { }
};

/**
 * @class	EOFException
 *
 * @brief	Exception for signalling EOF errors.
 *
 * @author	Julian
 * @date	2018-01-23
 */

class EOFException : public StreamReadException {
public:
	EOFException() : StreamReadException("End of file reached") { }
	
	explicit EOFException(const std::string& s) : StreamReadException(s + " end of file reached") { }
};

/**
 * @class	SyntaxException
 *
 * @brief	Exception for signalling syntax errors.
 *
 * @author	Julian
 * @date	2018-01-23
 */

class SyntaxException : public StreamReadException {
public:
	explicit SyntaxException(const std::string& s) : StreamReadException(s) { }
};

/**
 * @class	UnexpectedValueException
 *
 * @brief	Exception for signalling unexpected value errors.
 *
 * @author	Julian
 * @date	2018-01-23
 */

class UnexpectedValueException : public StreamReadException {
public:
	explicit UnexpectedValueException(int64_t position, const std::string& unexpectedSymbol) : StreamReadException(std::string("Unexpected value at [") + std::to_string(position) + "] got: " + unexpectedSymbol) { }
};