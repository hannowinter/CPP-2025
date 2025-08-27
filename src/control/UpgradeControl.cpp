#include "UpgradeControl.hpp"

#include "GameControl.hpp"
#include "PlayerControl.hpp"
#include "../model/Constants.hpp"

// Creates UpgradeControl.
UpgradeControl::UpgradeControl() :
    m_view{}
{

}

// Initializes control.
void UpgradeControl::init(const ControlList& controls)
{
    GameControl* game_control = controls.get<GameControl>();

    reset(game_control->random());
}

// Executes all relevant updates.
void UpgradeControl::update(const UpdateState& state)
{
    // Decrement timer
    m_timer -= state.delta;

    // Select position to spawn item where player is not located
    if (m_timer <= 0.0f && !m_spawned)
    {
        PlayerControl& player_control = *state.controls.get<PlayerControl>();
        GameControl& game_control = *state.controls.get<GameControl>();

        sf::Vector2f position = determine_random_position(player_control, game_control.random());
        
        // Determine next upgrade
        auto weapon = (game_control.random()() % 2) == 0 ? constants::upgrades::Weapon::LASER : constants::upgrades::Weapon::BOMB;

        spawn_at(position, weapon);
    }
}

// Draws Upgrade to screen.
void UpgradeControl::draw(LayerManager& layers)
{
    if (!m_picked_up && m_spawned)
        m_view.draw(layers.get(LayerID::ACTORS), m_upgrade, m_position);
}

sf::Vector2f UpgradeControl::determine_random_position(PlayerControl& player, std::mt19937& random) const
{
    // Determine item size for coordinates
    float item_x_size;
    float item_y;
    if (m_upgrade == constants::upgrades::Weapon::LASER)
    {
        item_x_size = constants::upgrades::LASER_ITEM_SIZE.x;
        item_y = constants::VIEW_HEIGHT - constants::upgrades::LASER_ITEM_SIZE.y - constants::PADDING;
    }
    else // m_upgrade == constants::upgrades::Weapon::BOMB
    {
        item_x_size = constants::upgrades::BOMB_ITEM_SIZE.x;
        item_y = constants::VIEW_HEIGHT - constants::upgrades::BOMB_ITEM_SIZE.y - constants::PADDING;
    }

    sf::Vector2f position;

    position.y = item_y;

    // Get PlayerController and player hitbox info
    const auto& player_hitbox = player.get().hitbox();

    float player_x = player_hitbox.position.x;
    float player_width = player_hitbox.size.x;

    // Screen dimensions
    float screen_width = constants::VIEW_WIDTH;

    // Decide side: distance to left edge or right edge
    float dist_left = player_x; // distance from player left edge to screen left edge
    float dist_right = screen_width - (player_x + player_width); // distance from player right edge to screen right edge

    if (dist_left > dist_right)
    {
        // Spawn somewhere randomly between left edge and player's left edge - item width
        // Spawn range: [0, player_x - item_x_size]
        if (player_x - item_x_size > 0)
        {
            std::uniform_real_distribution<float> dist(0.0f, player_x - item_x_size);
            position.x = dist(random);
        }
        else
        {
            // No space on left, fallback to right
            std::uniform_real_distribution<float> dist(player_x + player_width, screen_width - item_x_size);
            position.x = dist(random);
        }
    }
    else
    {
        // Spawn somewhere randomly between player's right edge and right screen edge - item width
        // Spawn range: [player_x + player_width, screen_width - item_x_size]
        if (screen_width - (player_x + player_width) > item_x_size)
        {
            std::uniform_real_distribution<float> dist(player_x + player_width, screen_width - item_x_size);
            position.x = dist(random);
        }
        else
        {
            // No space on right, fallback to left
            std::uniform_real_distribution<float> dist(0.0f, player_x - item_x_size);
            position.x = dist(random);
        }
    }

    // Make sure that the item is on screen
    position.x = std::clamp(
        position.x,
        constants::PADDING,
        constants::VIEW_WIDTH - item_x_size - constants::PADDING);

    return position;
}

void UpgradeControl::spawn_at(sf::Vector2f position, constants::upgrades::Weapon weapon)
{
    m_spawned = true;
    m_picked_up = false;
    m_position = position;
    m_upgrade = weapon;
}

// Gets hitbox of upgrade if it has been spawned.
sf::FloatRect UpgradeControl::hitbox() const
{
    // If no upgrade has been spawned, return unreachable hitbox
    if (!m_spawned)
        return {{0.0f, 0.0f}, {0.0f, 0.0f}};

    if (m_upgrade == constants::upgrades::Weapon::LASER)
            return {m_position, constants::upgrades::LASER_ITEM_SIZE};
    else // m_upgrade == constants::upgrades::Weapon::BOMB
            return {m_position, constants::upgrades::BOMB_ITEM_SIZE};
}

// Resets controller to spawn next upgrade.
void UpgradeControl::reset(std::mt19937& random)
{
    // Reset state
    m_spawned = false;
    m_picked_up = false;

    // Calculate new timer
    m_timer =
        std::uniform_real_distribution<float>{constants::upgrades::MIN_SPAWN_TIME,constants::upgrades::MAX_SPAWN_TIME}
                                            (random);
}

// Indicates that the upgrade has been picked up.
void UpgradeControl::pick_up()
{
    m_picked_up = true;
}

// Checks if upgrade has been picked up.
bool UpgradeControl::is_picked_up()
{
    return m_picked_up;
}

// Gets type of upgrade.
constants::upgrades::Weapon UpgradeControl::type()
{
    return m_upgrade;
}
