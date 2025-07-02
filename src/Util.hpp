#ifndef UTIL_H
#define UTIL_H

#include <SFML/Graphics.hpp>

template <typename T>
bool overlaps(const sf::Rect<T>& r1, const sf::Rect<T>& r2)
{
	return
		r1.position.x + r1.size.x > r2.position.x &&
		r1.position.y + r1.size.y > r2.position.y &&
		r2.position.x + r2.size.x > r1.position.x &&
		r2.position.y + r2.size.y > r1.position.y;
}

#endif