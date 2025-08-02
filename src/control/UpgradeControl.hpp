#ifndef UPGRADECONTROL_HPP
#define UPGRADECONTROL_HPP

#include "Controls.hpp"
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

    // Get position of upgrade
    sf::Vector2f get_position() const;

private:

    // Controlled view
    UpgradeView m_view;

    // Boolean indicating whether an upgrade has already been spawned
    bool m_spawned = false;

    // Upgrade to be spawned
    constants::player::Weapon m_upgrade;

    // Location for upgrade to be spawned
    sf::Vector2f m_position;

    // Time until next upgrade spawn
    float m_timer;
};

#endif //UPGRADECONTROL_HPP