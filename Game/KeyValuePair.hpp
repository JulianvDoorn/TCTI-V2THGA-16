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
		Unknown,
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
		bool b;
		sf::Vector2f* vectorValue;
		sf::FloatRect* rectValue;
		std::string* stringValue;
		sf::Color* colorValue;

		Value(nullptr_t ptr = nullptr) : vectorValue(ptr) { }

		~Value() { }

		Value& operator=(const float_t& val) {
			floatValue = val;
			return *this;
		}

		Value& operator=(const bool& val) {
			b = val;
			return *this;
		}

		Value& operator=(sf::Vector2f* ptr) {
			vectorValue = ptr;
			return *this;
		}

		Value& operator=(sf::FloatRect* ptr) {
			rectValue = ptr;
			return *this;
		}

		Value& operator=(std::string* ptr) {
			stringValue = ptr;
			return *this;
		}

		Value& operator=(sf::Color* ptr) {
			colorValue = ptr;
			return *this;
		}

		Value& operator=(nullptr_t ptr) {
			vectorValue = ptr;
			return *this;
		}

		bool operator==(const Value& rhs) const {
			return vectorValue == rhs.vectorValue;
		}

		bool operator==(const nullptr_t& rhs) const {
			return vectorValue == rhs;
		}

		template <class T>
		bool operator!=(const T& rhs) const {
			return !(*this == rhs);
		}

		operator Value&() {
			return *this;
		}

		operator float() {
			return floatValue;
		}

		operator int() {
			return static_cast<int>(floatValue);
		}

		operator bool() {
			return b;
		}

		template <class T>
		operator sf::Vector2<T>() {
			return static_cast<sf::Vector2<T>>(*vectorValue);
		}

		template <class T>
		operator sf::Rect<T>() {
			return static_cast<sf::Rect<T>>(*rectValue);
		}

		operator std::string() {
			return std::string(*stringValue);
		}

		operator sf::Color() {
			return *colorValue;
		}
	};

	Type type;

	/** @brief	Key as-is representation read from the file */
	std::string key;

	/** @brief	Value, this is a union of various pointers pointing one the few allowed datatypes */
	Value value;

	KeyValuePair() : type(Type::Unknown), key(), value(nullptr) { }

	KeyValuePair(KeyValuePair&& movedFrom) noexcept : type(movedFrom.type), key(movedFrom.key), value(movedFrom.value) {
		movedFrom.value = nullptr;
	}

	~KeyValuePair() {
		if (value != nullptr) {
			if (type == Type::Float) {
				// doesn't need destructing
			}
			else if (type == Type::Vector) {
				delete value.vectorValue;
			}
			else if (type == Type::Rect) {
				delete value.rectValue;
			}
			else if (type == Type::String) {
				delete value.stringValue;
			}
			else if (type == Type::Color) {
				delete value.colorValue;
			}
			else if (type == Type::Bool) {
				// doesn't need destructing
			}
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
		pair.value = vectorValue;
	} else if (rhs.compare(0, 5, "Rect(") == 0) {
		// parse rect
		sf::FloatRect* rectValue = new sf::FloatRect();
		iss >> *rectValue;

		pair.type = KeyValuePair::Type::Rect;
		pair.value = rectValue;
	} else if (rhs[0] == '"') {
		// parse string
		QuotedString* stringValue = new QuotedString();
		iss >> *stringValue;

		pair.type = KeyValuePair::Type::String;
		pair.value = stringValue;
	} else if (rhs[0] == '#') {
		// parse color
		ColorFactory colorFactory(iss);
		sf::Color* colorValue = new sf::Color(colorFactory.getColor());

		pair.type = KeyValuePair::Type::Color;
		pair.value = colorValue;
	}
	else {
		// parse keywords
		std::string stringValue;
		iss >> stringValue;

		pair.type = KeyValuePair::Type::Bool;

		if (stringValue == "true") {
			pair.value = true;
		}
		else if (stringValue == "false") {
			pair.value = false;
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