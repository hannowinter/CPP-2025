#include "Inputs.hpp"

// Clear list of active inputs
void Inputs::update()
{
	pressed_keys.clear();
}

// Record when a key is pressed
void Inputs::event(const sf::Event& event)
{
	// If a key is pressed, add it to list of held keys
	if (const sf::Event::KeyPressed* press = event.getIf<sf::Event::KeyPressed>())
	{
		// If key is not currently held, add it to list of pressed keys
		if (!held_keys.contains(press->code)) // prevent auto repeat
			pressed_keys.insert(press->code);

		// Add key to list of held keys
		held_keys.insert(press->code);
	}

	// If a key is released, remove it from the list of held keys
	if (const sf::Event::KeyReleased* release = event.getIf<sf::Event::KeyReleased>())
	{
		held_keys.erase(release->code);
	}
}