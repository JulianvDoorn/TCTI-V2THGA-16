#pragma once

#include <numeric>
#include <SFML/Graphics.hpp>
#include "ConsumeString.hpp"
#include "SecureRead.hpp"
#include "math.h"


template<class T>
std::ostream& operator<< (std::ostream& os, sf::Vector2<T> v) {
	return os << "Vector2(" << v.x << ", " << v.y << ')';
}

template<class T>
std::istream& operator>> (std::istream& is, sf::Vector2<T>& v) {
	is >> ConsumeString("Vector2");

	SecureRead<T> x;
	SecureRead<T> y;

	is >> std::ws >> SpecialCharacter::LeftBracket;
	is >> std::ws >> x;
	is >> std::ws >> SpecialCharacter::Comma;
	is >> std::ws >> y;
	is >> std::ws >> SpecialCharacter::RightBracket;

	v = sf::Vector2<T>(x, y);

	return is;
}

/**
 * @fn	sf::Vector2f operator/ (const sf::Vector2f& v, int i)
 *
 * @brief	Division operator
 *
 * @author	Julian
 * @date	2018-01-19
 *
 * @param	v	Vector to be divided by i
 * @param	i	Integer to divide v
 *
 * @return	Vector divided by integer
 */

sf::Vector2f operator/ (const sf::Vector2f& v, int i) {
	return { v.x / i, v.y / i };
}

/**
 * @fn	sf::Vector2f operator* (const sf::Vector2f& v, int i)
 *
 * @brief	Multiplication operator
 *
 * @author	Julian
 * @date	2018-01-19
 *
 * @param	v	Vector to multiply with i
 * @param	i	Integer to multiply with v
 *
 * @return	Vector multiplied by integer
 */

sf::Vector2f operator* (const sf::Vector2f& v, int i) {
	return { v.x * i, v.y * i };
}

/**
 * @fn	sf::Vector2f normalize(const sf::Vector2f& v)
 *
 * @brief	Normalizes the given vector.
 * 			Normalization is reduzing a vector's magnitude to 1.
 * 			https://en.wikipedia.org/wiki/Unit_vector
 *
 * @author	Julian
 * @date	2018-01-19
 *
 * @param	v	vector to normalize
 *
 * @return	Normalized vector
 */

sf::Vector2f normalize(const sf::Vector2f& v) {
	float length = sqrtf((v.x * v.x) + (v.y * v.y));
	if (length != 0) {
		return { v.x / length, v.y / length };
	}
	else {
		return v;
	}
}

/**
 * @fn	float magnitude(const sf::Vector2f& v)
 *
 * @brief	Magnitudes the given vector.
 * 			The magnitude of a vector is the distance from the origin to its point.
 * 			https://en.wikipedia.org/wiki/Magnitude_(mathematics)
 *
 * @author	Julian
 * @date	2018-01-19
 *
 * @param	v	Vector to get the magnitude of
 *
 * @return	Distance between { 0, 0 } and v
 */

float magnitude(const sf::Vector2f& v) {
	return sqrtf((v.x * v.x) + (v.y * v.y));
}

/**
 * @fn	float dot(const sf::Vector2f& v0, const sf::Vector2f& v1)
 *
 * @brief	Applies the dot product on the given two vectors.
 * 			https://en.wikipedia.org/wiki/Dot_product
 *
 * @author	Julian
 * @date	2018-01-19
 *
 * @param	v0	Vector0
 * @param	v1	Vector1
 *
 * @return	Dot product of v0 and v1
 */

float dot(const sf::Vector2f& v0, const sf::Vector2f& v1) {
	return v0.x * v1.x + v0.y * v1.y;
}

/**
 * @fn	float getDistance(const sf::Vector2f& v0, const sf::Vector2f& v1)
 *
 * @brief	Gets the distance between v0 and v1
 *
 * @author	Julian
 * @date	2018-01-19
 *
 * @param	v0	Vector0
 * @param	v1	Vector1
 *
 * @return	The distance between Vector0 and Vector1
 */

float getDistance(const sf::Vector2f& v0, const sf::Vector2f& v1) {
	const sf::Vector2f n = v1 - v0;
	return magnitude(n);
}