#pragma once

#include <istream>
#include <string>
#include <sstream>
#include <iostream>
#include <cctype>

#include "SecureRead.hpp"
#include "SpecialCharacter.hpp"
#include "ConsumeString.hpp"

/**
 * @class	KeyValuePair
 *
 * @brief	Key-value pair for reading from input streams.
 * 			
 * @detailed Input stream should have the following input as example:
 * 			 SomeKeyValue = "Hello world"
 * 			 
 * 			 The following value types are allowed:
 * 			 Float formatted as {FLOAT}, example: 10.5
 * 			 Vector2f formatted as ({FLOAT}, {FLOAT}), example: (10.5, 5.0)
 * 			 String formatted as "{STRING CONTENT}", example "Hello world"
 * 			 Color formatted as #{HEX COLOR CODE}, example #1A2B3C
 * 			
 *
 * @author	Julian
 * @date	2018-01-23
 */

class KeyValuePair {
public:

	/**
	 * @enum	Type
	 *
	 * @brief	The possible values that a KeyValuePair can have.
	 */

	enum class Type {
		Float,
		Vector,
		Rect,
		String,
		Color,
		Bool
	};

	/**
	 * @union	Value
	 *
	 * @brief	A union of various datatypes.
	 * 			
	 * @detailed The reason it was decided to use a union was to achieve run-time polymorphism without requiring virtual methods or class wrappers.
	 *
	 * @author	Julian
	 * @date	2018-01-23
	 */

	union Value {
		float_t f;
		sf::Vector2f* v;
		sf::FloatRect* r;
		std::string* s;
		sf::Color* c;
		bool b;
	};

	Type type;

	/** @brief	Key as-is representation read from the file */
	std::string key;

	/** @brief	Value, this is a union of various pointers pointing one the few allowed datatypes */
	Value value;

	~KeyValuePair() {
		// TODO: find out if the destructor actually selects the right destructor for the union Value.

		if (type == Type::Float) {

		}
		else if (type == Type::Vector) {
			//delete value.v;
		}
		else if (type == Type::Rect) {
			//delete value.r;
		}
		else if (type == Type::String) {
			//delete value.s;
		}
		else if (type == Type::Color) {
			//delete value.c;
		}
		else if (type == Type::Bool) {
			
		}
	}
};

template<class T>
std::ostream& operator<< (std::ostream& os, sf::Rect<T>& v) {
	return os << "Rect(" << v.left << ", " <<  v.top << ", " << v.width << ", " << v.height << ')';
}

template<class T>
std::istream& operator>> (std::istream& is, sf::Rect<T>& v) {
	is >> ConsumeString("Rect");

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

	v = sf::Rect<T>(left, top, width, height);

	return is;
}

/**
 * @fn	std::istream& operator>> (std::istream& is, KeyValuePair& pair)
 *
 * @brief	Stream extraction operator
 * 			
 * @detailed Extracts the key-value pair accordingly.
 *
 * @author	Julian
 * @date	2018-01-23
 *
 * @param [in,out]	is  	The is.
 * @param [in,out]	pair	The pair.
 *
 * @return	The shifted result.
 */

std::istream& operator>> (std::istream& is, KeyValuePair& pair) {
	is >> pair.key;
	is >> SpecialCharacter::Assignment;

	std::string reading;

	int64_t pos = is.tellg();
	getline(is, reading);

	std::istringstream iss(reading);

	std::string rhs;
	iss >> rhs;
	iss.seekg(0);

	if (isdigit(rhs[0])) {
		// parse float
		float_t f;
		iss >> static_cast<SecureRead<float_t&>>(f);

		pair.type = KeyValuePair::Type::Float;
		pair.value.f = f;
	}
	else if (rhs.compare(0, 8, "Vector2(") == 0) {
		// parse vector
		sf::Vector2f* v = new sf::Vector2f();
		iss >> *v;

		pair.type = KeyValuePair::Type::Vector;
		pair.value.v = v;
	} else if (rhs.compare(0, 5, "Rect(") == 0) {
		// parse rect
		sf::FloatRect* r = new sf::FloatRect();
		iss >> *r;

		pair.type = KeyValuePair::Type::Rect;
		pair.value.r = r;
	} else if (rhs[0] == '"') {
		// parse string
		QuotedString* s = new QuotedString();
		iss >> *s;

		pair.type = KeyValuePair::Type::String;
		pair.value.s = s;
	} else if (rhs[0] == '#') {
		// parse color
		ColorFactory colorFactory(iss);
		sf::Color* c = new sf::Color(colorFactory.getColor());

		pair.type = KeyValuePair::Type::Color;
		pair.value.c = c;
	}
	else {
		// parse keywords
		std::string s;
		iss >> s;

		pair.type = KeyValuePair::Type::Bool;

		if (s == "true") {
			pair.value.b = true;
		}
		else if (s == "false") {
			pair.value.b = false;
		}
		else {
			std::string unexpectedSymbol;

			iss.seekg(0);
			iss >> unexpectedSymbol;
			throw UnexpectedValueException(pos, unexpectedSymbol);
		}
	}

	return is;
}