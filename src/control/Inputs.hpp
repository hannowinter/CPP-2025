#ifndef INPUTS_H
#define INPUTS_H

#include <SFML/Graphics.hpp>
#include <unordered_set>

// Struct to manage pressed and held keys
struct Inputs
{
	std::unordered_set<sf::Keyboard::Key> held_keys;
	std::unordered_set<sf::Keyboard::Key> pressed_keys;

public:
	// Clears list of active inputs.
	void update();

	// Records when a key is pressed.
	void event(const sf::Event& event);
};

#endif