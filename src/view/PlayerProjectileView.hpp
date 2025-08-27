#ifndef PLAYERPROJECTILEVIEW_HPP
#define PLAYERPROJECTILEVIEW_HPP

#include "Layer.hpp"
#include "../model/PlayerProjectile.hpp"
#include <SFML/Graphics/Drawable.hpp>

class PlayerProjectileView 
{
public:

    // Creates PlayerProjectileView.
    PlayerProjectileView(const sf::Texture& sprite);

    // Draws projectile.
    virtual void draw(Layer& layer, const PlayerProjectile& bullet);

protected:

    // Projectile texture
    sf::Sprite m_sprite;
};

// View for bullets of player
class PlayerBulletView : public PlayerProjectileView
{
public:

    // Creates bullet view.
    PlayerBulletView();
};

// View for lasers of player
class LaserView : public PlayerProjectileView {
public:

    // Creates LaserView.
    LaserView();
};

// View for bombs of player
class BombView : public PlayerProjectileView {
public:

    // Creates BombView.
    BombView();

    // Makes bomb explode.
    void explode();
};

#endif //PLAYERPROJECTILEVIEW_HPP