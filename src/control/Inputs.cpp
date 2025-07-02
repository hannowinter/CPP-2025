#include "Inputs.hpp"

void Inputs::update()
{
	pressed_keys.clear();
}

void Inputs::event(const sf::Event& event)
{
	if (const sf::Event::KeyPressed* press = event.getIf<sf::Event::KeyPressed>())
	{
		if (!held_keys.contains(press->code)) // prevent auto repeat
			pressed_keys.insert(press->code);
		held_keys.insert(press->code);
	}
	if (const sf::Event::KeyReleased* release = event.getIf<sf::Event::KeyReleased>())
	{
		held_keys.erase(release->code);
	}
}