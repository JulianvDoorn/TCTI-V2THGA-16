#pragma once

#include <istream>
#include <string>
#include <sstream>
#include <iostream>
#include <cctype>
#include <math.h>

#include "SecureRead.hpp"
#include "SpecialCharacter.hpp"
#include "ConsumeString.hpp"
#include "QuotedString.hpp"
#include "ColorFactory.hpp"
#include "VectorStreamOperators.hpp"
#include "RectStreamOperators.hpp"

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
		float_t floatValue;
		sf::Vector2f* vectorValue;
		sf::FloatRect* rectValue;
		std::string* stringValue;
		sf::Color* colorValue;
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
			//delete value.vectorValue;
		}
		else if (type == Type::Rect) {
			//delete value.rectValue;
		}
		else if (type == Type::String) {
			//delete value.stringValue;
		}
		else if (type == Type::Color) {
			//delete value.colorValue;
		}
		else if (type == Type::Bool) {
			
		}
	}
};

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
		float_t floatValue;
		iss >> static_cast<SecureRead<float_t&>>(floatValue);

		pair.type = KeyValuePair::Type::Float;
		pair.value.floatValue = floatValue;
	}
	else if (rhs.compare(0, 8, "Vector2(") == 0) {
		// parse vector
		sf::Vector2f* vectorValue = new sf::Vector2f();
		iss >> *vectorValue;

		pair.type = KeyValuePair::Type::Vector;
		pair.value.vectorValue = vectorValue;
	} else if (rhs.compare(0, 5, "Rect(") == 0) {
		// parse rect
		sf::FloatRect* rectValue = new sf::FloatRect();
		iss >> *rectValue;

		pair.type = KeyValuePair::Type::Rect;
		pair.value.rectValue = rectValue;
	} else if (rhs[0] == '"') {
		// parse string
		QuotedString* stringValue = new QuotedString();
		iss >> *stringValue;

		pair.type = KeyValuePair::Type::String;
		pair.value.stringValue = stringValue;
	} else if (rhs[0] == '#') {
		// parse color
		ColorFactory colorFactory(iss);
		sf::Color* colorValue = new sf::Color(colorFactory.getColor());

		pair.type = KeyValuePair::Type::Color;
		pair.value.colorValue = colorValue;
	}
	else {
		// parse keywords
		std::string stringValue;
		iss >> stringValue;

		pair.type = KeyValuePair::Type::Bool;

		if (stringValue == "true") {
			pair.value.b = true;
		}
		else if (stringValue == "false") {
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