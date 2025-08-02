#ifndef PLAYERBOMB_HPP
#define PLAYERBOMB_HPP

#include "PlayerProjectile.hpp"

// Class modelling a bomb shot by the player
class PlayerBomb : public PlayerProjectile {
public:

    // Create bomb projectile
    PlayerBomb(sf::Vector2f position);

    // Get hitbox
    sf::FloatRect hitbox() const override;
};

#endif //PLAYERBOMB_HPP