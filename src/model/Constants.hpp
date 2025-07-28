#include <SFML/Graphics.hpp>

namespace constants
{
    constexpr int VIEW_WIDTH = 1200;
    constexpr int VIEW_HEIGHT = 900;
    constexpr sf::FloatRect VIEW_RECT = { { 0.0f, 0.0f }, { VIEW_WIDTH, VIEW_HEIGHT } };
    constexpr unsigned int FRAME_RATE = 60;

    constexpr float PADDING = 10.0f; // minimum distance of actors to the view's border

    namespace player
    {
        constexpr float MOVE_SPEED = 400.0f;
        constexpr sf::Vector2f SIZE = { 60.0f, 30.0f };
        constexpr float SHOOT_COOLDOWN = 0.4f;
    }

    namespace player_bullet
    {
        constexpr float MOVE_SPEED = 1000.0f;
        constexpr sf::Vector2f SIZE = { 6.0f, 12.0f };
    }

    namespace alien_bullet
    {
        constexpr float MOVE_SPEED = 300.0f;
        constexpr sf::Vector2f SIZE = { 6.0f, 32.0f };
    }

    namespace alien_grid
    {
        constexpr float MIN_INTENSITY = 1.0f;
        constexpr float MAX_INTENSITY = 10.0f;

        // constants controlling the positioning and movement of the alien grid
        constexpr sf::Vector2f SPACING = { 20.0f, 20.0f }; // distance of neighboring aliens from each other
        constexpr size_t COLUMNS = 14;
        constexpr size_t GREEN_ROWS = 2;
        constexpr size_t YELLOW_ROWS = 3;
        constexpr size_t RED_ROWS = 3;
        constexpr size_t TOTAL_ROWS = GREEN_ROWS + YELLOW_ROWS + RED_ROWS;
        constexpr float SHIFT_SPEED = 40.0f; // move speed when in mode `SHIFT_LEFT` or `SHIFT_RIGHT`
        constexpr float DESCEND_SPEED = 20.0f; // move speed when in mode `DESCEND`
        constexpr float DESCEND_DURATION = 1.0f; // duration of mode `DESCEND`

        // constants controlling the wait time between consecutive swerves
        constexpr float SWERVE_MIN_WAIT = 3.0f;
        constexpr float SWERVE_MAX_WAIT = 8.0f;
    }

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
    }
}