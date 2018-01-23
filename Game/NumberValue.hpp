#pragma once

#include <istream>
#include <iostream>
#include "IOExceptions.hpp"

template<class T>
class NumberValue {
	T v;
public:
	NumberValue() : v(0) { };
	NumberValue(T v) : v(v) { }
	
	operator T() const {
		return v;
	}
	
	operator T&() {
		return v;
	}
};

template<class T>
std::istream& operator>> (std::istream& is, NumberValue<T>& v) {
	int64_t pos = is.tellg();
	
	try {
		is >> (T&) v;
	} catch (const std::istream::failure&) {
		std::string unexpectedSymbol;
		is.clear();
		is.seekg(pos);
		is >> unexpectedSymbol;
		throw UnexpectedValueException(pos, unexpectedSymbol);
	}
	
	return is;
}