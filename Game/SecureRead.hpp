#pragma once

#include <istream>

#include "BaseStreamExceptions.hpp"

/**
 * @class	SecureRead
 *
 * @brief	Exception safe(r) wrapper class for reading primitive values.
 * 			Designed for usage for ints, floats, doubles, chars etc.
 * 			Anything with a operator>>(std::istream&, TYPE&) should work.
 *
 * @author	Julian
 * @date	2018-01-23
 *
 * @tparam	T	Generic type parameter.
 */

template<class T>
class SecureRead {
	T v;
public:
	SecureRead() : v(0) { };
	SecureRead(T v) : v(v) { }
	
	operator T() const {
		return v;
	}
	
	operator T&() {
		return v;
	}
};

/**
 * @fn	template<class T> std::istream& operator>> (std::istream& is, SecureRead<T>& v)
 *
 * @brief	Stream extraction operator
 * 			
 * @detailed Extracts value T from std::istream& is.
 * 			 Throws an exception when something unexpected happens.
 *
 * @author	Julian
 * @date	2018-01-23
 *
 * @exception	UnexpectedValueException	Thrown when an Unexpected Value error condition
 *  occurs.
 *
 * @tparam	T	Generic type parameter.
 * @param [in,out]	is	The is.
 * @param [in,out]	v 	A SecureRead<T> to process.
 *
 * @return	The result of the operation.
 */

template<class T>
std::istream& operator>> (std::istream& is, SecureRead<T>& v) {
	int64_t pos = is.tellg();
	
	try {
		is >> exceptions >> (T&) v;
	} catch (const std::istream::failure&) {
		std::string unexpectedSymbol;
		is.clear();
		is.seekg(pos);
		is >> unexpectedSymbol;
		throw UnexpectedValueException(pos, unexpectedSymbol);
	}
	
	return is;
}