#ifndef PLAYERCONTROL_H
#define PLAYERCONTROL_H

#include <SFML/Graphics.hpp>
#include <random>

#include "Control.hpp"
#include "../model/Constants.hpp"
#include "../model/Player.hpp"
#include "../view/Layer.hpp"
#include "../view/PlayerView.hpp"
#include "../model/GameState.hpp"

// Control class for Player
class PlayerControl : public Control
{
public:
	// Creates a new PlayerControl for Player at position.
	PlayerControl(sf::Vector2f position);

	// Abstract methods of parent class
	void init(const ControlList& controls) override;
	void update(const UpdateState& state) override;
	void draw(LayerManager& layers) override;

	// Checks for collisions with bullets, aliens and upgrades.
	void check_collision_bullet(Control& control, ControlList& controls, GameState& state);
	void check_collision_alien(Control& control, ControlList& controls, GameState& state);
	void check_collision_upgrade(Control& control, ControlList& controls, std::mt19937& random);

	// Makes player invisible.
	void hide();

	// Gets weapon for next shot.
	constants::upgrades::Weapon get_weapon() const;

	// Sets weapon for next shot.
	void set_weapon(constants::upgrades::Weapon weapon);

	// Gets reference to model object.
	const Player& get() const;
	Player& get();

	// Gets reference to view object.
	const PlayerView& get_view() const;
	
private:
	// Model
	Player m_player;

	// View
	PlayerView m_player_view;

	// Remaining cooldown until next shot
	float m_shoot_cooldown;

	// Indicating whether player is hidden
	bool m_hidden = false;

	// Current weapon
	constants::upgrades::Weapon m_weapon = constants::upgrades::Weapon::DEFAULT;
};

#endif