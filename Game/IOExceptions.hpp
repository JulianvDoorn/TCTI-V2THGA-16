#pragma once

#include <exception>
#include <string>
#include <fstream>

std::istream& exceptions(std::istream& is) {
	is.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	return is;
}

class BodyFactoryException : public std::exception {
	std::string err;
	
public:
	explicit BodyFactoryException(std::string s) : err(std::move(s)) { }
	
	const char* what() const noexcept final {
		return err.c_str();
	}
};

class UnknownTypeException : public BodyFactoryException {
public:
	explicit UnknownTypeException(const std::string& type) : BodyFactoryException(std::string("Unknown type [") + type + "]") { }
};

class EOFException : public BodyFactoryException {
public:
	EOFException() : BodyFactoryException("End of file reached") { }
	
	explicit EOFException(const std::string& s) : BodyFactoryException(s + " end of file reached") { }
};

class SyntaxException : public BodyFactoryException {
public:
	explicit SyntaxException(const std::string& s) : BodyFactoryException(s) { }
};

class UnexpectedValueException : public BodyFactoryException {
public:
	explicit UnexpectedValueException(int64_t position, const std::string& unexpectedSymbol) : BodyFactoryException(std::string("Unexpected value at [") + std::to_string(position) + "] got: " + unexpectedSymbol) { }
};

class OpeningBracketMissingException : public SyntaxException {
public:
	explicit OpeningBracketMissingException(int64_t position) : SyntaxException(std::string("Opening bracket missing at [") + std::to_string(position) + "]") { }
	explicit OpeningBracketMissingException(int64_t position, const std::string& unexpectedSymbol) : SyntaxException(std::string("Opening bracket missing at [") + std::to_string(position) + "] unexpected symbol: " + unexpectedSymbol) { }
};

class ClosingBracketMissingException : public SyntaxException {
public:
	explicit ClosingBracketMissingException(int64_t position) : SyntaxException(std::string("Closing bracket missing at [") + std::to_string(position) + "]") { }
	explicit ClosingBracketMissingException(int64_t position, const std::string& unexpectedSymbol) : SyntaxException(std::string("Closing bracket missing at [") + std::to_string(position) + "] unexpected symbol: " + unexpectedSymbol) { }
};

class CommaMissingException : public SyntaxException {
public:
	explicit CommaMissingException(int64_t position) : SyntaxException(std::string("Comma missing at [") + std::to_string(position) + "]") { }
	explicit CommaMissingException(int64_t position, const std::string& unexpectedSymbol) : SyntaxException(std::string("Comma missing at [") + std::to_string(position) + "] unexpected symbol: " + unexpectedSymbol) { }
};

class QuoteMissingException : public SyntaxException {
public:
	explicit QuoteMissingException(int64_t position) : SyntaxException(std::string("Quote missing at [") + std::to_string(position) + "]") { }
	explicit QuoteMissingException(int64_t position, const std::string& unexpectedSymbol) : SyntaxException(std::string("Quote missing at [") + std::to_string(position) + "] unexpected symbol: " + unexpectedSymbol) { }
};

class HashtagMissingException : public SyntaxException {
public:
	explicit HashtagMissingException(int64_t position) : SyntaxException(std::string("Quote missing at [") + std::to_string(position) + "]") { }
	explicit HashtagMissingException(int64_t position, const std::string& unexpectedSymbol) : SyntaxException(std::string("Quote missing at [") + std::to_string(position) + "] unexpected symbol: " + unexpectedSymbol) { }
};

class AssignmentMissingException : public SyntaxException {
public:
	explicit AssignmentMissingException(int64_t position) : SyntaxException(std::string("Assignment (=) missing at [") + std::to_string(position) + "]") { }
	explicit AssignmentMissingException(int64_t position, const std::string& unexpectedSymbol) : SyntaxException(std::string("Assignment (=) missing at [") + std::to_string(position) + "] unexpected symbol: " + unexpectedSymbol) { }
};