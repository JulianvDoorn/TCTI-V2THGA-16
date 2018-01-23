#pragma once

#include "NumberValue.hpp"

template<class T>
class BracketEncapsulated {
	T& v;

public:
	BracketEncapsulated() : v() { }
	BracketEncapsulated(T& v) : v(v) { }

	operator T() const {
		return v;
	}

	operator T&() {
		return v;
	}
};

template<class T>
std::istream& operator>>(std::istream& is, BracketEncapsulated<T&> v) {
	is >> SpecialCharacter::LeftBracket;
	is >> (T&) v;
	is >> SpecialCharacter::RightBracket;

	return is;
}