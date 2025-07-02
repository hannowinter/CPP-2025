#ifndef PLAYERCONTROL_H
#define PLAYERCONTROL_H

#include <SFML/Graphics.hpp>

#include "Actors.hpp"
#include "../model/Player.hpp"
#include "../view/Layer.hpp"
#include "../view/PlayerView.hpp"

class PlayerControl : public ActorControl
{
public:
	PlayerControl(sf::Vector2f position);

	void update(const UpdateState& state) override;
	void draw(Layer& layer) override;
	
private:
	Player m_player;
	PlayerView m_player_view;
};

#endif