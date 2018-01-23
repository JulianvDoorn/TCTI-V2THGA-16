#pragma once

#include <istream>
#include <string>
#include <sstream>
#include <iostream>
#include <cctype>

#include "SecureRead.hpp"
#include "SpecialCharacter.hpp"

class KeyValuePair {
public:
	enum class Type {
		Float,
		Vector,
		String,
		Color
	};

	union Value {
		float_t* f;
		sf::Vector2f* v;
		std::string* s;
		sf::Color* c;
	};

	Type type;

	std::string key;
	Value value;

	~KeyValuePair() {
		if (type == Type::Float) {
			//delete value.f;
		}
		else if (type == Type::Vector) {
			//delete value.v;
		}
		else if (type == Type::String) {
			//delete value.s;
		}
		else if (type == Type::Color) {
			//delete value.c;
		}
	}
};

std::istream& operator>> (std::istream& is, KeyValuePair& pair) {
	is >> pair.key;
	is >> SpecialCharacter::Assignment;

	std::string reading;

	getline(is, reading);

	std::istringstream iss(reading);

	char c;
	iss >> c;
	iss.seekg(0);

	if (std::isdigit(c)) {
		// parse float
		float_t* f = new float_t();
		iss >> static_cast<SecureRead<float_t&>>(*f);

		std::cout << "float: " << *f << std::endl;

		pair.type = KeyValuePair::Type::Float;
		pair.value.f = f;
	} else if (c == '(') {
		// parse vector
		sf::Vector2f* v = new sf::Vector2f();
		iss >> *v;

		std::cout << "vector: " << *v << std::endl;

		pair.type = KeyValuePair::Type::Vector;
		pair.value.v = v;
	} else if (c == '"') {
		// parse string
		QuotedString* s = new QuotedString();
		iss >> *s;

		std::cout << "string: " << *s << std::endl;

		pair.type = KeyValuePair::Type::String;
		pair.value.s = s;
	} else if (c == '#') {
		// parse color
		ColorFactory colorFactory(iss);
		sf::Color* c = new sf::Color(colorFactory.getColor());

		std::cout << "color: " << ColorFactory::getStringFromColor(*c) << std::endl;

		pair.type = KeyValuePair::Type::Color;
		pair.value.c = c;
	}

	return is;
}