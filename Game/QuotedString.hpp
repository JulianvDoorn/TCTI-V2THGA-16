#pragma once

#include <string>
#include "SpecialCharacter.hpp"
#include "IOExceptions.hpp"

class QuotedString : public std::string {
public:
	QuotedString() : std::string() { }
	QuotedString(std::string str) : std::string(std::move(str)) { }
};

std::istream& operator>> (std::istream& is, QuotedString& qs) {
	is >> SpecialCharacter::Quote;
	
	std::ios::fmtflags skipwsFlag = is.flags() & std::ios::skipws;
	is.unsetf(std::ios::skipws);
	
	int nextc = is.peek();
	
	while (nextc != SpecialCharacter::Quote && nextc != '\n') {
		char c;

		try {
			is >> exceptions >> c;
		} catch (const std::istream::failure&) {
			throw EOFException("Unexpected");
		}
		
		qs.push_back(c);
		nextc = is.peek();
	}

	if (nextc == '\n') {
		throw QuoteMissingException(is.tellg());
	}

	if (skipwsFlag) {
		is.setf(std::ios::skipws);
	}
	
	is >> SpecialCharacter::Quote;
	
	return is;
}