#include "AlienControl.hpp"
#include "GameControl.hpp"
#include "../model/Constants.hpp"
#include "../Util.hpp"

AlienControl::AlienControl(sf::Vector2f grid_origin, size_t column, size_t row) :
	m_alien{ grid_origin, column, row }
{

}

void AlienControl::update(const UpdateState& state)
{
	const GameControl& game_control = *state.controls.get<GameControl>();
	m_alien.align_grid(game_control.alien_grid_origin());
}

void AlienControl::draw(Layer& layer)
{
	m_alien_view.draw(layer, m_alien);
}

Alien& AlienControl::get()
{
	return m_alien;
}

const Alien& AlienControl::get() const
{
	return m_alien;
}