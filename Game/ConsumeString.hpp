#pragma once

#include <string>
#include <istream>

class ConsumeStringException : std::exception {
	const std::string err;

public:
	ConsumeStringException(const std::string& consumeString, const std::string& consumed) : err(std::string("[") + consumeString + "] does not match [" + consumed + "]") { }

	const char* what() const noexcept override {
		return err.c_str();
	}
};

class ConsumeString : public std::string {
public:
	ConsumeString(const std::string& str) : std::string(std::move(str)) { }
};

std::istream& operator>> (std::istream& is, const ConsumeString& consumeString) {
	std::string consumed;

	is >> std::ws;

	for (char rightChar : consumeString) {
		char leftChar;

		is >> std::noskipws >> leftChar;

		consumed.push_back(leftChar);

		if (leftChar != rightChar) {
			throw ConsumeStringException(consumeString, consumed);
		}
	}

	return is;
}