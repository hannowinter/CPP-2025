#ifndef HUDVIEW_H
#define HUDVIEW_H

#include "Layer.hpp"
#include "../model/GameState.hpp"

// View for the HUD
class HUDView
{
public:
	// Create HUD
	HUDView();

	// Draw HUD
	void draw(Layer& layer, const GameState& state);

private:
	// Font and text to be shown
	sf::Font m_font;
	sf::Text m_score;
	sf::Text m_level;
	sf::Text m_lives;

	// GameOver texts
	sf::Text m_gameover;
	sf::Text m_restart;
};

#endif