#ifndef GAMECONTROL_H
#define GAMECONTROL_H

#include "Controls.hpp"
#include "Inputs.hpp"
#include "../model/Alien.hpp"

class GameControl : public Control
{
public:
	GameControl(ControlList& controls);

	void update(const UpdateState& state) override;
	void draw(Layer& layer) override;

	sf::Vector2f alien_grid_origin() const;

private:
	Alien::Mode m_alien_prev_mode;
	Alien::Mode m_alien_mode = Alien::SHIFT_RIGHT;
	sf::Vector2f m_alien_grid_origin;
	float m_alien_descend_timer = 0.0f;
};

#endif