#include "HUDView.hpp"
#include "../model/Constants.hpp"
#include "../model/GameState.hpp"
#include <format>

// Create HUD and load font
HUDView::HUDView() :
	m_font{ "assets/fonts/DejaVuSansMono.ttf" },
	m_score{ m_font },
	m_level{ m_font },
	m_lives{ m_font },
	m_gameover{ m_font },
	m_restart{ m_font }
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

	// Show GameOver if game has ended
	if (state.over)
	{
		m_gameover.setString("Game Over!");
		m_gameover.setCharacterSize(100);
		m_gameover.setStyle(sf::Text::Bold);
		m_gameover.setPosition({(constants::VIEW_WIDTH - m_gameover.getLocalBounds().size.x) / 2.0f,
								(constants::VIEW_HEIGHT - m_gameover.getLocalBounds().size.y) / 2.0f - 20 * constants::PADDING});
		layer.add_to_layer(m_gameover);

		m_restart.setString("Press SPACE to restart.");
		m_restart.setCharacterSize(70);
		m_restart.setPosition({(constants::VIEW_WIDTH - m_restart.getLocalBounds().size.x) / 2.0f,
								(constants::VIEW_HEIGHT - m_restart.getLocalBounds().size.y) / 2.0f});
		layer.add_to_layer(m_restart);
	}
	else if (state.level_won)
	{
		m_level_won.setString("Level won!");
		m_level_won.setCharacterSize(100);
		m_level_won.setStyle(sf::Text::Bold);
		m_level_won.setPosition({(constants::VIEW_WIDTH - m_level_won.getLocalBounds().size.x) / 2.0f,
								(constants::VIEW_HEIGHT - m_level_won.getLocalBounds().size.y) / 2.0f - 20 * constants::PADDING});
		layer.add_to_layer(m_level_won);

		m_continue.setString("Press SPACE to continue.");
		m_continue.setCharacterSize(70);
		m_continue.setPosition({(constants::VIEW_WIDTH - m_continue.getLocalBounds().size.x) / 2.0f,
								(constants::VIEW_HEIGHT - m_continue.getLocalBounds().size.y) / 2.0f});
		layer.add_to_layer(m_continue);
	}
}
