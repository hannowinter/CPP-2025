#ifndef GAMECONTROL_H
#define GAMECONTROL_H

#include <random>

#include "Controls.hpp"
#include "Inputs.hpp"
#include "../model/Alien.hpp"
#include "../view/LayerManager.hpp"
#include "../model/GameState.hpp"
#include "../view/HUDView.hpp"

class GameControl : public Control
{
public:
	GameControl(ControlList& controls);

	void init(const ControlList& controls) override;
	void update(const UpdateState& state) override;
	void draw(LayerManager& layers) override;

	std::mt19937& random();
	GameState& state();
	const GameState& state() const;

private:
	GameState m_state;
	HUDView m_hud;
	std::mt19937 m_random;
};

#endif