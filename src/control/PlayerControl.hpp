#ifndef PLAYERCONTROL_H
#define PLAYERCONTROL_H

#include <SFML/Graphics.hpp>

#include "Controls.hpp"
#include "../model/Player.hpp"
#include "../view/Layer.hpp"
#include "../view/PlayerView.hpp"

// Controller for Player
class PlayerControl : public Control
{
public:
	// Create PlayerControl for Player at position
	PlayerControl(sf::Vector2f position);

	// Abstract methods of parent class
	void init(const ControlList& controls) override;
	void update(const UpdateState& state) override;
	void draw(LayerManager& layers) override;

	// Get controlled Player
	const Player& get() const;
	Player& get();
	
private:
	// Controlled Player model
	Player m_player;

	// Controlled Player view
	PlayerView m_player_view;

	// Remaining cooldown until next shot
	float m_shoot_cooldown;
};

#endif