#include "HUDView.hpp"
#include "../model/GameState.hpp"
#include <format>

// Create HUD and load font
HUDView::HUDView() :
	m_font{ "assets/fonts/DejaVuSansMono.ttf" },
	m_text{ m_font }
{

}

// Draw HUD
void HUDView::draw(Layer& layer, const GameState& state)
{
	// Show game information as text and add to layer
	m_text.setString(std::format("Score: {}               Level: {}               Lives: {}", state.score, state.level, state.lives));
	layer.add_to_layer(m_text);
}