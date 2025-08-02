#include "BombControl.hpp"

#include "../model/PlayerBomb.hpp"
#include "../view/BombView.hpp"

// Create BombControl at position
BombControl::BombControl(sf::Vector2f position) :
    PlayerProjectileControl(std::make_unique<PlayerBomb>(position), std::make_unique<BombView>())
{

}