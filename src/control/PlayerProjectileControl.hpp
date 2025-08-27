#ifndef PLAYERPROJECTILECONTROL_HPP
#define PLAYERPROJECTILECONTROL_HPP

#include "Control.hpp"
#include "../model/PlayerProjectile.hpp"
#include "../view/PlayerProjectileView.hpp"

// Class modelling a projectile shot by the player
class PlayerProjectileControl : public Control 
{
public:
    // Create projectile shot by player at position
    template <
        std::derived_from<PlayerProjectile> M, // model type
        std::derived_from<PlayerProjectileView> V // view type
    >
    PlayerProjectileControl(
        M&& projectile,
        V&& view
    ) :
        m_projectile{ std::make_unique<M>(std::move(projectile)) },
        m_projectile_view{ std::make_unique<V>(std::move(view)) }
    {

    }

    // Abstract methods of parent class
    void init(const ControlList& controls) override;
    void update(const UpdateState& state) override;
    void draw(LayerManager& layers) override;

    // Gets reference to model object.
    const PlayerProjectile& get() const;
    PlayerProjectile& get();

protected:
    // Model
    std::unique_ptr<PlayerProjectile> m_projectile;

    // View
    std::unique_ptr<PlayerProjectileView> m_projectile_view;
};

// Control class for bullets shot by player
class PlayerBulletControl : public PlayerProjectileControl
{
public:

    // Creates control at position.
    PlayerBulletControl(sf::Vector2f position);
};

// Control class for lasers shot by player
class LaserControl : public PlayerProjectileControl {
public:

    // Creates control at position.
    LaserControl(sf::Vector2f position);
};

// Control class for bombs shot by player
class BombControl : public PlayerProjectileControl {
public:

    // Creates control at position.
    BombControl(sf::Vector2f position);

    // Updates BombControl.
    void update(const UpdateState& state) override;

    // Makes bomb explode.
    void explode();

    // Checks if bomb has exploded.
    bool has_exploded() const;

private:

    // Remember if bomb has exploded
    bool m_has_exploded = false;

    // Remaining lifetime of explosion
    float m_lifetime;
};

#endif //PLAYERPROJECTILECONTROL_HPP