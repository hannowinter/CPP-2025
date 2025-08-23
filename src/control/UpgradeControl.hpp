#ifndef UPGRADECONTROL_HPP
#define UPGRADECONTROL_HPP

#include "Control.hpp"
#include "../view/UpgradeView.hpp"
#include "../model/Constants.hpp"

class UpgradeControl : public Control {
public:

    // Create UpgradeControl
    UpgradeControl();

    // Abstract methods of parent class
    void init(const ControlList& controls) override;
    void update(const UpdateState& state) override;
    void draw(LayerManager& layers) override;

    // Reset controller to spawn next upgrade
    void reset(const UpdateState& state);

    // Indicate that the upgrade has been picked up
    void pick_up();

    // Check if upgrade has been picked up
    bool is_picked_up();

    // Get type of upgrade
    constants::upgrades::Weapon type();

    // Get hitbox of upgrade
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