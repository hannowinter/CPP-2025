#ifndef PLAYERPROJECTILEVIEW_HPP
#define PLAYERPROJECTILEVIEW_HPP

#include "Layer.hpp"
#include "../model/PlayerProjectile.hpp"
#include "SFML/Graphics/Drawable.hpp"

class PlayerProjectileView {
public:

    // Create PlayerProjectileView
    PlayerProjectileView(const sf::Texture& sprite);

    // Draw projectile
    virtual void draw(Layer& layer, const PlayerProjectile& bullet);

private:

    // Projectile texture
    sf::Sprite m_sprite;
};

#endif //PLAYERPROJECTILEVIEW_HPP