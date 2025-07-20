#ifndef ALIENGRIDCONTROL_H
#define ALIENGRIDCONTROL_H

#include <random>

#include "Controls.hpp"
#include "Inputs.hpp"
#include "../model/Alien.hpp"

class AlienGridControl : public Control
{
public:
	enum Mode
	{
		SHIFT_RIGHT, SHIFT_LEFT, DESCEND
	};

	AlienGridControl(ControlList& controls);

	void init(const ControlList& controls) override;
	void update(const UpdateState& state) override;
	void draw(Layer& layer) override;

	sf::Vector2f origin() const;
	float speed_multiplier() const;

	void reset_swerve_timer(std::mt19937& random);

private:
	Mode m_prev_mode;
	Mode m_mode = SHIFT_RIGHT;
	sf::Vector2f m_origin;
	float m_descend_timer = 0.0f;
	float m_speed_multiplier = 1.0f;

	float m_swerve_timer = 0.0f;
};

#endif