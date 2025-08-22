#ifndef UTIL_H
#define UTIL_H

#include <SFML/Graphics.hpp>

// Determine whether two rectangles overlap
template <typename T>
bool overlaps(const sf::Rect<T>& r1, const sf::Rect<T>& r2)
{
	// Detect axis-aligned bounding box collisions
	return
		r1.position.x + r1.size.x > r2.position.x &&
		r1.position.y + r1.size.y > r2.position.y &&
		r2.position.x + r2.size.x > r1.position.x &&
		r2.position.y + r2.size.y > r1.position.y;
}

template <typename T, typename R>
T lerp(const T& v1, const T& v2, const R& r)
{ // std::lerp does not work with sf::Vector2f unfortunately
	return v1 * (R{ 1 } - r) + v2 * r;
};

#endif