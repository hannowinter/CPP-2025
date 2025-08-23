#ifndef PLAYERPROJECTILEVIEW_HPP
#define PLAYERPROJECTILEVIEW_HPP

#include "Layer.hpp"
#include "../model/PlayerProjectile.hpp"
#include <SFML/Graphics/Drawable.hpp>

class PlayerProjectileView 
{
public:

    // Create PlayerProjectileView
    PlayerProjectileView(const sf::Texture& sprite);

    // Draw projectile
    virtual void draw(Layer& layer, const PlayerProjectile& bullet);

protected:

    // Projectile texture
    sf::Sprite m_sprite;
};

// View for bullets of player
class PlayerBulletView : public PlayerProjectileView
{
public:

    // Create bullet view
    PlayerBulletView();
};

// View for lasers of player
class LaserView : public PlayerProjectileView {
public:

    // Create LaserView
    LaserView();
};

// View for bombs of player
class BombView : public PlayerProjectileView {
public:

    // Create BombView
    BombView();

    // Make bomb explode
    void explode();
};

#endif //PLAYERPROJECTILEVIEW_HPP