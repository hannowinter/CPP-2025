#include "LaserControl.hpp"

#include "../model/PlayerLaser.hpp"
#include "../view/LaserView.hpp"

// Create LaserControl at position
LaserControl::LaserControl(sf::Vector2f position) :
    PlayerProjectileControl(std::make_unique<PlayerLaser>(position), std::make_unique<LaserView>())
{

}
