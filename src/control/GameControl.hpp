#ifndef GAMECONTROL_H
#define GAMECONTROL_H

#include <random>

#include "Controls.hpp"
#include "Inputs.hpp"
#include "../model/Alien.hpp"

class GameControl : public Control
{
public:
	GameControl(ControlList& controls);

	void init(const ControlList& controls) override;
	void update(const UpdateState& state) override;
	void draw(Layer& layer) override;

	std::mt19937& random();

private:
	std::mt19937 m_random;
};

#endif