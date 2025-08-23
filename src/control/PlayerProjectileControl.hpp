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
        std::derived_from<PlayerProjectile> M, // model class
        std::derived_from<PlayerProjectileView> V // view class
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

    // Get reference to projectile
    const PlayerProjectile& get() const;
    PlayerProjectile& get();

protected:

    // Controlled PlayerProjectile
    std::unique_ptr<PlayerProjectile> m_projectile;

    // Controlled PlayerProjectileView
    std::unique_ptr<PlayerProjectileView> m_projectile_view;
};

// Controller for bullet shot by player
class PlayerBulletControl : public PlayerProjectileControl
{
public:

    // Create Bullet shot by player at position
    PlayerBulletControl(sf::Vector2f position);
};

// Controller for laser shot by player
class LaserControl : public PlayerProjectileControl {
public:

    // Create LaserControl
    LaserControl(sf::Vector2f position);
};

// Controller for bomb shot by player
class BombControl : public PlayerProjectileControl {
public:

    // Create BombControl
    BombControl(sf::Vector2f position);

    // Update BombControl
    void update(const UpdateState& state) override;

    // Make bomb explode
    void explode();

    // Check if bomb has exploded
    bool has_exploded() const;

private:

    // Remember if bomb has exploded
    bool m_has_exploded = false;

    // Remaining lifetime of explosion
    float m_lifetime;
};

#endif //PLAYERPROJECTILECONTROL_HPP