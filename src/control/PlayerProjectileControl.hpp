#ifndef PLAYERPROJECTILECONTROL_HPP
#define PLAYERPROJECTILECONTROL_HPP

#include "Controls.hpp"
#include "../model/PlayerProjectile.hpp"
#include "../view/PlayerProjectileView.hpp"

// Class modelling a projectile shot by the player
class PlayerProjectileControl : public Control {
public:

    // Create projectile shot by player at position
    PlayerProjectileControl(std::unique_ptr<PlayerProjectile> projectile,
                            std::unique_ptr<PlayerProjectileView> view);

    // Abstract methods of parent class
    void init(const ControlList& controls) override;
    void update(const UpdateState& state) override;
    void draw(LayerManager& layers) override;

    // Get reference to projectile
    const PlayerProjectile& get() const;
    PlayerProjectile& get();

protected:

    // Controlled PlayerProjectile
    std::unique_ptr<PlayerProjectile> m_projectile;

    // Controlled PlayerProjectileView
    std::unique_ptr<PlayerProjectileView> m_projectile_view;
};

#endif //PLAYERPROJECTILECONTROL_HPP