#ifndef BOMBCONTROL_HPP
#define BOMBCONTROL_HPP

#include "PlayerProjectileControl.hpp"

class BombControl : public PlayerProjectileControl {
public:

    // Create BombControl
    BombControl(sf::Vector2f position);

    // Update BombControl
    void update(const UpdateState& state) override;

    // Make bomb explode
    void explode();

    // Check if bomb has exploded
    bool has_exploded();

private:

    // Remember if bomb has exploded
    bool m_has_exploded = false;

    // Remaining lifetime of explosion
    float m_lifetime;
};

#endif //BOMBCONTROL_HPP