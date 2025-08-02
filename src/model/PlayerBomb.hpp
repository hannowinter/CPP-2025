#ifndef PLAYERBOMB_HPP
#define PLAYERBOMB_HPP

#include "PlayerProjectile.hpp"

// Class modelling a bomb shot by the player
class PlayerBomb : public PlayerProjectile {
public:

    // Create bomb projectile
    PlayerBomb(sf::Vector2f position);

    // Make bomb explode
    void explode();

    // Get hitbox
    sf::FloatRect hitbox() const override;

private:

    // Remember if bomb has exploded
    bool m_has_exploded = false;
};

#endif //PLAYERBOMB_HPP