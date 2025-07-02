#ifndef INPUTS_H
#define INPUTS_H

#include <SFML/Graphics.hpp>
#include <unordered_set>

struct Inputs
{
	std::unordered_set<sf::Keyboard::Key> held_keys;
	std::unordered_set<sf::Keyboard::Key> pressed_keys;

public:
	void update();
	void event(const sf::Event& event);
};

#endif