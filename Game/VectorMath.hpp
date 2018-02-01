#pragma once

#include <numeric>
#include <SFML/Graphics.hpp>
#include "math.h"

/**
 * @fn	template <class T, class U> auto operator/ (const sf::Vector2<T>& v, U i) -> sf::Vector2<decltype(T + U)>
 *
 * @brief	Division operator
 *
 * @author	Julian
 * @date	2018-01-19
 *
 * @tparam	T	Generic type parameter.
 * @tparam	U	Generic type parameter.
 * @param	v	Vector to be divided by i.
 * @param	i	Integer to divide v.
 *
 * @return	Vector divided by integer.
 */

template <class T, class U>
auto operator/ (const sf::Vector2<T>& v, U i) -> sf::Vector2<decltype(T + U)> {
	return { v.x / i, v.y / i };
}

/**
 * @fn	template <class T, class U> auto operator* (const sf::Vector2<T>& v, U i) -> sf::Vector2<decltype(T + U)>
 *
 * @brief	Multiplication operator
 *
 * @author	Julian
 * @date	2018-01-19
 *
 * @tparam	T	Generic type parameter.
 * @tparam	U	Generic type parameter.
 * @param	v	Vector to multiply with i.
 * @param	i	Integer to multiply with v.
 *
 * @return	Vector multiplied by integer.
 */

template <class T, class U>
auto operator* (const sf::Vector2<T>& v, U i) -> sf::Vector2<decltype(T + U)> {
	return { v.x * i, v.y * i };
}

/**
 * @fn	template <class T> sf::Vector2<T> normalize(const sf::Vector2<T>& v)
 *
 * @brief	Normalizes the given vector. Normalization is reduzing a vector's magnitude to 1.
 * https://en.wikipedia.org/wiki/Unit_vector
 *
 * @author	Julian
 * @date	2018-01-19
 *
 * @tparam	T	Generic type parameter.
 * @param	v	vector to normalize.
 *
 * @return	Normalized vector.
 */

template <class T>
sf::Vector2<T> normalize(const sf::Vector2<T>& v) {
	float length = sqrtf((v.x * v.x) + (v.y * v.y));
	if (length != 0) {
		return { v.x / length, v.y / length };
	}
	else {
		return v;
	}
}

/**
 * @fn	template <class T> float magnitude(const sf::Vector2<T>& v)
 *
 * @brief	Magnitudes the given vector. The magnitude of a vector is the distance from the
 * origin to its point. https://en.wikipedia.org/wiki/Magnitude_(mathematics)
 *
 * @author	Julian
 * @date	2018-01-19
 *
 * @tparam	T	Generic type parameter.
 * @param	v	Vector to get the magnitude of.
 *
 * @return	Distance between { 0, 0 } and v.
 */

template <class T>
float magnitude(const sf::Vector2<T>& v) {
	return sqrtf((v.x * v.x) + (v.y * v.y));
}

/**
 * @fn	template <class T, class U> auto dot(const sf::Vector2<T>& v0, const sf::Vector2<U>& v1) -> decltype(T * U)
 *
 * @brief	Applies the dot product on the given two vectors.
 * https://en.wikipedia.org/wiki/Dot_product
 *
 * @author	Julian
 * @date	2018-01-19
 *
 * @tparam	T	Generic type parameter.
 * @tparam	U	Generic type parameter.
 * @param	v0	Vector0.
 * @param	v1	Vector1.
 *
 * @return	Dot product of v0 and v1.
 */

template <class T, class U>
auto dot(const sf::Vector2<T>& v0, const sf::Vector2<U>& v1) -> decltype(T * U) {
	return v0.x * v1.x + v0.y * v1.y;
}

/**
 * @fn	template <class T, class U> auto getDistance(const sf::Vector2<T>& v0, const sf::Vector2<U>& v1) -> decltype(T - U)
 *
 * @brief	Gets the distance between v0 and v1
 *
 * @author	Julian
 * @date	2018-01-19
 *
 * @tparam	T	Generic type parameter.
 * @tparam	U	Generic type parameter.
 * @param	v0	Vector0.
 * @param	v1	Vector1.
 *
 * @return	The distance between Vector0 and Vector1.
 */

template <class T, class U>
auto getDistance(const sf::Vector2<T>& v0, const sf::Vector2<U>& v1) -> decltype(T - U) {
	const sf::Vector2<decltype(T - U)> n = v1 - v0;
	return magnitude(n);
}