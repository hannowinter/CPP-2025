#ifndef PLAYERCONTROL_H
#define PLAYERCONTROL_H

#include <SFML/Graphics.hpp>

#include "Controls.hpp"
#include "../model/Player.hpp"
#include "../view/Layer.hpp"
#include "../view/PlayerView.hpp"

class PlayerControl : public Control
{
public:
	PlayerControl(sf::Vector2f position);

	void init(const ControlList& controls) override;
	void update(const UpdateState& state) override;
	void draw(Layer& layer) override;

	const Player& get() const;
	Player& get();
	
private:
	Player m_player;
	PlayerView m_player_view;
	float m_shoot_cooldown = 0.0f;
};

#endif