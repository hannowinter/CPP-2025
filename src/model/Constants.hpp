#include <SFML/Graphics.hpp>

namespace constants 
{
    constexpr int VIEW_WIDTH = 960;
    constexpr int VIEW_HEIGHT = 720;
    constexpr sf::FloatRect VIEW_RECT = { { 0.0f, 0.0f }, { VIEW_WIDTH, VIEW_HEIGHT } };
    constexpr unsigned int FRAME_RATE = 60;

    constexpr float PADDING = 10.0f; // minimum distance of the player to the view's border

    namespace player
    {
        constexpr float MOVE_SPEED = 300.0f;
        constexpr sf::Vector2f SIZE = { 60.0f, 30.0f };
        constexpr float SHOOT_COOLDOWN = 0.3f;
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

    namespace alien
    {
        constexpr float MOVE_SPEED_X = 30.0f;
        constexpr float MOVE_SPEED_Y = 15.0f;
        constexpr sf::Vector2f SIZE = { 40.0f, 32.0f };
        constexpr sf::Vector2f SPACING = { 20.0f, 20.0f };
        constexpr float DESCEND_DURATION = 1.0f;
        constexpr size_t COLUMNS = 11;
        constexpr size_t GREEN_ROWS = 1;
        constexpr size_t YELLOW_ROWS = 2;
        constexpr size_t RED_ROWS = 2;
        constexpr size_t TOTAL_ROWS =
            GREEN_ROWS + YELLOW_ROWS + RED_ROWS;
        constexpr float MIN_SPEED_MULTIPLIER = 1.0f;
        constexpr float MAX_SPEED_MULTIPLIER = 10.0f;
        constexpr float MAX_SHOOT_TIME = 50.0f;
        constexpr float SHAKE_INTENSITY_EXPONENT = 0.4f;
        constexpr float SHAKE_MIN_TARGET = 2.0f;
        constexpr float SHAKE_MAX_TARGET = 5.0f;
        constexpr float SHAKE_MIN_DURATION = 0.2f;
        constexpr float SHAKE_MAX_DURATION = 0.6f;
        constexpr float SWERVE_ACCELERATION = 2000.0f;
        constexpr float SWERVE_MAX_SPEED = 400.0f;
        constexpr float RETREAT_THRESHOLD = 80.0f;
        constexpr float RETREAT_ACCELERATION = 3000.0f;
        constexpr float RETREAT_MIN_SPEED = 250.0f;
        constexpr float RETREAT_FINISH_THRESHOLD = 2.0f;
    }
}