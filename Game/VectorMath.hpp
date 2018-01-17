#pragma once

#include <numeric>
#include <SFML/Graphics.hpp>

std::ostream& operator<< (std::ostream& os, sf::Vector2f v) {
	return os << "Vector2f(" << v.x << ", " << v.y << ')';
}

sf::Vector2f operator/ (const sf::Vector2f& v, int i) {
	return { v.x / i, v.y / i };
}

sf::Vector2f operator* (const sf::Vector2f& v, int i) {
	return { v.x * i, v.y * i };
}

sf::Vector2f normalize(const sf::Vector2f& v) {
	float length = sqrtf((v.x * v.x) + (v.y * v.y));
	if (length != 0) {
		return { v.x / length, v.y / length };
	}
	else {
		return v;
	}
}

float magnitude(const sf::Vector2f& v) {
	return sqrtf((v.x * v.x) + (v.y * v.y));
}

float dot(const sf::Vector2f& v0, const sf::Vector2f& v1) {
	return v0.x * v1.x + v0.y * v1.y;
	//	return std::inner_product(&v0.x, &v0.y, &v1.x, 0.0f);
}

float getDistance(const sf::Vector2f& v0, const sf::Vector2f& v1) {
	const sf::Vector2f n = v1 - v0;
	return magnitude(n);
}