#ifndef HUDVIEW_H
#define HUDVIEW_H

#include "Layer.hpp"
#include "../model/GameState.hpp"

class HUDView
{
public:
	HUDView();

	void draw(Layer& layer, const GameState& state);

private:
	sf::Font m_font;
	sf::Text m_text;
};

#endif