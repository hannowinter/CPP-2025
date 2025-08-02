#ifndef BOMBVIEW_HPP
#define BOMBVIEW_HPP
#include "PlayerProjectileView.hpp"

class BombView : public PlayerProjectileView {
public:

    // Create BombView
    BombView();

    // Make bomb explode
    void explode();
};

#endif //BOMBVIEW_HPP