#include "HUDView.hpp"
#include "../model/Constants.hpp"
#include "../model/GameState.hpp"
#include <format>

// Create HUD and load font
HUDView::HUDView() :
	m_font{ "assets/fonts/DejaVuSansMono.ttf" },
	m_score{ m_font },
	m_level{ m_font },
	m_lives{ m_font }
{

}

// Draw HUD
void HUDView::draw(Layer& layer, const GameState& state)
{
	// Show game information as text and add to layer

	// Score is shown on the left
	m_score.setString(std::format("Score: {}", state.score));
	m_score.setPosition({constants::PADDING, 0.0f});
	layer.add_to_layer(m_score);

	// Level is shown in the middle
	m_level.setString(std::format("Level: {}", state.level));
	m_level.setPosition({(constants::VIEW_WIDTH - m_level.getLocalBounds().size.x - constants::PADDING)/ 2.0f, 0.0f});
	layer.add_to_layer(m_level);

	// Number of lives is shown on the right
	m_lives.setString(std::format("Lives: {}", state.lives));
	m_lives.setPosition({constants::VIEW_WIDTH - m_lives.getLocalBounds().size.x - constants::PADDING, 0.0f});
	layer.add_to_layer(m_lives);
}