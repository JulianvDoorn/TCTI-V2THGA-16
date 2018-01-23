#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include "IOExceptions.hpp"
#include "QuotedString.hpp"
#include "SpecialCharacter.hpp"

class MalformedColorStringException : public BodyFactoryException {
public:
	explicit MalformedColorStringException(const std::string& s) : BodyFactoryException("Malformed/unknown color: \"" + s + "\"") { }
};

class UnexpectedHexChar {
public:
	char erroneousChar;
	explicit UnexpectedHexChar(char c) : erroneousChar(c) { }
	
	operator char() const {
		return erroneousChar;
	}
};

class UnexpectedHexCharException : public BodyFactoryException {
public:
	explicit UnexpectedHexCharException(UnexpectedHexChar e, const std::string& s) : BodyFactoryException(std::string("Malformed hex char \"") + (char) e + "\" in \"" + s + "\"") { }
};

class ConversionTable : public std::map<const std::string, const sf::Color> {
public:
	ConversionTable(std::initializer_list<std::pair<const std::string, const sf::Color>> list) : map(list) { }
	
	iterator convert(const std::string& name) {
		return find(name);
	}
	
	iterator convert(const sf::Color& color) {
		iterator it;
		
		for (it = begin(); it != end(); it++) {
			if (it->second == color) {
				return it;
			}
		}
		
		return it;
	}
};

class ColorFactory {
	std::istream& input;
	
	static ConversionTable colorNameConversionTable;
	
	static uint32_t readHex(const char* begin, const char* end) {
		uint32_t buffer = 0;
		
		while (begin <= end) {
			if (*begin >= '0' && *begin <= '9') {
				buffer <<= 4;
				buffer += *begin - '0';
			} else if (*begin >= 'A' && *begin <= 'F') {
				buffer <<= 4;
				buffer += *begin - 'A' + 10;
			} else {
				throw UnexpectedHexChar(*begin);
			}
			
			begin++;
		}
		
		return buffer;
	}
	
public:
	ColorFactory(std::istream& input) : input(input) { }
	
	sf::Color getColor() {
		char c;
		input >> c;
		input.seekg(-1, std::ios::cur);
		
		if (c == '#') {
			std::string color;
			input >> color;
			return getColorFromString(color);
		} else if (c == '\"') {
			QuotedString color;
			input >> color;
			return getColorFromString(color);
		} else {
			std::string unexpectedSymbol;
			input >> unexpectedSymbol;
			throw MalformedColorStringException(unexpectedSymbol);
		}
	}
	
	static sf::Color getColorFromString(const std::string& name) {
		ConversionTable::iterator conversion = colorNameConversionTable.convert(name);
		
		if(conversion != colorNameConversionTable.end()) {
		   return conversion->second;
		} else {
			if (name[0] != '#') {
				throw MalformedColorStringException(name);
			}
			
			uint8_t r;
			uint8_t g;
			uint8_t b;
			uint8_t a;
			
			try {
				if (name.size() == 7 || name.size() == 9) {
					r = (uint8_t) readHex(&name[1], &name[2]);
					g = (uint8_t) readHex(&name[3], &name[4]);
					b = (uint8_t) readHex(&name[5], &name[6]);
					
					if (name.size() == 9) {
						a = (uint8_t) readHex(&name[7], &name[8]);
					} else {
						a = 255;
					}
				} else {
					throw MalformedColorStringException(name);
				}
			} catch (const UnexpectedHexChar& e) {
				throw UnexpectedHexCharException(e, name);
			}
			
			return { r, g, b, a };
		}
	}
	
	static std::string getStringFromColor(const sf::Color& color) {
		ConversionTable::iterator conversion = colorNameConversionTable.convert(color);

		if (conversion != colorNameConversionTable.end()) {
			std::string str;
			str += SpecialCharacter::Quote;
			str += conversion->first;
			str += SpecialCharacter::Quote;
			return str;
		} else {
			std::ostringstream ss;
			ss << std::setw(2) << std::setfill('0') << std::uppercase << std::hex << (uint32_t) color.r;
			ss << std::setw(2) << std::setfill('0') << std::uppercase << std::hex << (uint32_t) color.g;
			ss << std::setw(2) << std::setfill('0') << std::uppercase << std::hex << (uint32_t) color.b;
			
			if (color.a < 255) {
				ss << std::setw(2) << std::setfill('0') << std::uppercase << std::hex << (uint32_t) color.a;
			}
			
			return "#" + ss.str();
		}
	}
};

ConversionTable ColorFactory::colorNameConversionTable = {
		{ "red", sf::Color::Red },
		{ "black", sf::Color::Black },
		{ "blue", sf::Color::Blue },
		{ "cyan", sf::Color::Cyan },
		{ "green", sf::Color::Green },
		{ "magenta", sf::Color::Magenta },
		{ "transparent", sf::Color::Transparent },
		{ "white", sf::Color::White },
		{ "yellow", sf::Color::Yellow }
};