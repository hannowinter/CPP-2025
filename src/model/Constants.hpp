#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <SFML/Graphics.hpp>

// Important globally used constants
namespace constants
{
    // Video constants (window size, framerate, ...)
    constexpr int VIEW_WIDTH = 1200;
    constexpr int VIEW_HEIGHT = 900;
    constexpr sf::FloatRect VIEW_RECT = { { 0.0f, 0.0f }, { VIEW_WIDTH, VIEW_HEIGHT } };
    constexpr unsigned int FRAME_RATE = 60;

    constexpr float PADDING = 10.0f; // minimum distance of actors to the view's border

    // Constants for player character
    namespace player
    {
        constexpr float MOVE_SPEED = 400.0f;
        constexpr sf::Vector2f SIZE = { 60.0f, 30.0f };
        constexpr float SHOOT_COOLDOWN = 0.4f;
    }

    // Constants for player bullet
    namespace player_bullet
    {
        constexpr float MOVE_SPEED = 1000.0f;
        constexpr sf::Vector2f BULLET_SIZE = { 6.0f, 12.0f };
        constexpr sf::Vector2f BOMB_SIZE = {36.0f, 52.0f };
        constexpr sf::Vector2f LASER_SIZE = { 15.0f, 36.0f };
    }

    // Constants for upgrade spawn times
    namespace upgrades
    {
        constexpr float MIN_SPAWN_TIME = 5.0f;
        constexpr float MAX_SPAWN_TIME = 10.0f;
        constexpr sf::Vector2f BOMB_ITEM_SIZE = {36.0f, 52.0f };
        constexpr sf::Vector2f LASER_ITEM_SIZE = { 62.0f, 62.0f };

        // Possible weapons
        enum class Weapon
        {
            DEFAULT,
            BOMB,
            LASER
        };
    }

    // Constants for alien bullet
    namespace alien_bullet
    {
        constexpr float MOVE_SPEED = 300.0f;
        constexpr sf::Vector2f SIZE = { 6.0f, 32.0f };
    }

    // Constants for grid of aliens
    namespace alien_grid
    {
        // Intensity is used as a measure of game difficulty
        constexpr float MIN_INTENSITY = 1.0f;
        constexpr float MAX_INTENSITY = 10.0f;

        // constants controlling the positioning and movement of the alien grid
        constexpr sf::Vector2f SPACING = { 20.0f, 20.0f }; // distance of neighboring aliens from each other
        constexpr size_t COLUMNS = 14;
        constexpr size_t GREEN_ROWS = 2;
        constexpr size_t YELLOW_ROWS = 3;
        constexpr size_t RED_ROWS = 3;
        constexpr size_t TOTAL_ROWS = GREEN_ROWS + YELLOW_ROWS + RED_ROWS;
        constexpr float SHIFT_SPEED = 40.0f; // move speed when in mode "SHIFT_LEFT" or "SHIFT_RIGHT"
        constexpr float DESCEND_SPEED = 20.0f; // move speed when in mode "DESCEND"
        constexpr float DESCEND_DURATION = 1.0f; // duration of mode "DESCEND"

        // constants controlling the wait time between consecutive swerves
        constexpr float SWERVE_MIN_WAIT = 3.0f;
        constexpr float SWERVE_MAX_WAIT = 8.0f;
    }

    // Constants for aliens
    namespace alien
    {
        constexpr sf::Vector2f SIZE = { 40.0f, 32.0f };

        constexpr float MAX_SHOOT_TIME = 60.0f; // maximum time (in seconds) between consecutive shoots

        constexpr float SHAKE_INTENSITY_EXPONENT = 0.4f; // softens the shake on higher intensities
        constexpr float SHAKE_MIN_TARGET_OFFSET = 2.0f; // minimum target offset from the alien's base position
        constexpr float SHAKE_MAX_TARGET_OFFSET = 5.0f; // maximum target offset from the alien's base position
        constexpr float SHAKE_MIN_DURATION = 0.4f;
        constexpr float SHAKE_MAX_DURATION = 0.6f;

        constexpr float SWERVE_ACCELERATION = 2000.0f;
        constexpr float SWERVE_MAX_SPEED = 400.0f;
        constexpr float RETREAT_THRESHOLD = 110.0f; // distance to the screen's bottom until the alien retreats

        constexpr float RETREAT_ACCELERATION = 3000.0f;
        constexpr float RETREAT_MIN_SPEED = 250.0f;

        constexpr float EFFECT_DURATION = 1.5f;
    }
}

#endif