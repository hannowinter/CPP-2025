#ifndef UPGRADECONTROL_HPP
#define UPGRADECONTROL_HPP

#include <random>

#include "Control.hpp"
#include "../view/UpgradeView.hpp"
#include "../model/Constants.hpp"
#include "../control/PlayerControl.hpp"

class UpgradeControl : public Control 
{
public:

    // Creates UpgradeControl.
    UpgradeControl();

    // Abstract methods of parent class
    void init(const ControlList& controls) override;
    void update(const UpdateState& state) override;
    void draw(LayerManager& layers) override;

    // Determine a random position for the upgrade to be placed at.
    sf::Vector2f determine_random_position(PlayerControl& player, std::mt19937& random) const;

    // Spawns the upgrade.
    void spawn_at(sf::Vector2f position, constants::upgrades::Weapon weapon);

    // Resets control to spawn next upgrade.
    void reset(std::mt19937& random);

    // Indicates that the upgrade has been picked up.
    void pick_up();

    // Checks if upgrade has been picked up.
    bool is_picked_up();

    // Gets type of upgrade.
    constants::upgrades::Weapon type();

    // Gets hitbox of upgrade.
    sf::FloatRect hitbox() const;

private:

    // Controlled view
    UpgradeView m_view;

    // Boolean indicating whether an upgrade has already been spawned
    bool m_spawned = false;

    // Boolean indicating whether the upgrade has been picked up
    bool m_picked_up = false;

    // Upgrade to be spawned
    constants::upgrades::Weapon m_upgrade;

    // Location for upgrade to be spawned
    sf::Vector2f m_position;

    // Time until next upgrade spawn
    float m_timer;
};

#endif //UPGRADECONTROL_HPP