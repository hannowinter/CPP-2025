#ifndef PLAYERLASER_HPP
#define PLAYERLASER_HPP

#include "PlayerProjectile.hpp"

class PlayerLaser : public PlayerProjectile {
public:

    // Create laser projectile
    PlayerLaser(sf::Vector2f position);

    // Get hotbox of projectile
    sf::FloatRect hitbox() const override;
};

#endif //PLAYERLASER_HPP