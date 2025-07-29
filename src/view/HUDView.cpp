#include "HUDView.hpp"
#include "../model/GameState.hpp"
#include <format>

HUDView::HUDView() :
	m_font{ "assets/fonts/DejaVuSansMono.ttf" },
	m_text{ m_font }
{

}

void HUDView::draw(Layer& layer, const GameState& state)
{
	m_text.setString(std::format("Score: {}", state.score));
	layer.add_to_layer(m_text);
}