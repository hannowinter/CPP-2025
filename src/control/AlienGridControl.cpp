#include "AlienGridControl.hpp"

#include "PlayerControl.hpp"
#include "AlienControl.hpp"
#include "GameControl.hpp"
#include "../model/Constants.hpp"

AlienGridControl::AlienGridControl(ControlList& controls) :
    m_mode{ SHIFT_RIGHT },
    m_origin{ constants::PADDING, constants::PADDING }
{
    controls.add<PlayerControl>(sf::Vector2f{
       (constants::VIEW_WIDTH - constants::player::SIZE.x) / 2.0f,
       constants::VIEW_HEIGHT - constants::player::SIZE.y - constants::PADDING
        });

    for (size_t x = 0; x < constants::alien::COLUMNS; x++)
    {
        size_t y = 0;
        for (size_t i = 0; i < constants::alien::GREEN_ROWS; i++, y++)
        {
            controls.add<AlienControl>(Alien::GREEN, m_origin, x, y);
        }
        for (size_t i = 0; i < constants::alien::YELLOW_ROWS; i++, y++)
        {
            controls.add<AlienControl>(Alien::YELLOW, m_origin, x, y);
        }
        for (size_t i = 0; i < constants::alien::RED_ROWS; i++, y++)
        {
            controls.add<AlienControl>(Alien::RED, m_origin, x, y);
        }
    }
}

void AlienGridControl::init(const ControlList& controls)
{
    GameControl& game_control = *controls.get<GameControl>();
    reset_swerve_timer(game_control.random());
}

void AlienGridControl::update(const UpdateState& state)
{
    GameControl& game_control = *state.controls.get<GameControl>();
    
    constexpr size_t initial_alien_count = constants::alien::COLUMNS * constants::alien::TOTAL_ROWS;
    size_t alien_count = std::count_if(
        state.controls.begin(),
        state.controls.end(),
        [](const auto& c) { return c->template is<AlienControl>(); }
    );
    // ratio = 0.0 initially, ratio = 1.0 if only one alien is left
    float ratio = 1.0f - static_cast<float>(alien_count - 1) / (initial_alien_count - 1);
    ratio = std::pow(ratio, 3.0f);
    m_speed_multiplier = std::lerp(
        constants::alien::MIN_SPEED_MULTIPLIER,
        constants::alien::MAX_SPEED_MULTIPLIER,
        ratio
    );

    size_t alien_min_col = SIZE_MAX;
    size_t alien_max_col = 0;
    for (const auto& control : state.controls)
    {
        if (AlienControl* alien = control->is<AlienControl>())
        {
            alien_min_col = std::min(alien->get().column(), alien_min_col);
            alien_max_col = std::max(alien->get().column(), alien_max_col);
        }
    }

    float alien_grid_left = alien_min_col * (constants::alien::SIZE.x + constants::alien::SPACING.x);
    float alien_grid_right = alien_max_col * (constants::alien::SIZE.x + constants::alien::SPACING.x) + constants::alien::SIZE.x;

    sf::Vector2f velocity{};

    switch (m_mode)
    {
    case SHIFT_RIGHT:
        velocity = { constants::alien::MOVE_SPEED_X, 0.0f };
        m_origin += velocity * state.delta * m_speed_multiplier;
        if (m_origin.x + alien_grid_right > constants::VIEW_WIDTH - constants::PADDING)
        {
            m_origin.x = constants::VIEW_WIDTH - constants::PADDING - alien_grid_right;
            m_prev_mode = m_mode;
            m_mode = DESCEND;
        }
        break;
    case SHIFT_LEFT:
        velocity = { -constants::alien::MOVE_SPEED_X, 0.0f };
        m_origin += velocity * state.delta * m_speed_multiplier;
        if (m_origin.x + alien_grid_left < constants::PADDING)
        {
            m_origin.x = constants::PADDING - alien_grid_left;
            m_prev_mode = m_mode;
            m_mode = DESCEND;
        }
        break;
    case DESCEND:
        velocity = { 0.0f, constants::alien::MOVE_SPEED_Y };
        m_origin += velocity * state.delta * m_speed_multiplier;
        m_descend_timer += state.delta;
        if (m_descend_timer >= constants::alien::DESCEND_DURATION / m_speed_multiplier)
        {
            m_descend_timer = 0.0f;
            Mode new_prev_mode = m_mode;
            if (m_prev_mode == SHIFT_LEFT)
                m_mode = SHIFT_RIGHT;
            if (m_prev_mode == SHIFT_RIGHT)
                m_mode = SHIFT_LEFT;
            m_prev_mode = new_prev_mode;
        }
        break;
    }

    m_swerve_timer -= state.delta;
    if (m_swerve_timer <= 0.0f)
    {
        reset_swerve_timer(game_control.random());
        size_t count = state.controls.count<AlienControl>();
        size_t alien1_to_swerve = 
            std::uniform_int_distribution<size_t>{ 0, count - 1 }(game_control.random());
        size_t alien2_to_swerve =
            std::uniform_int_distribution<size_t>{ 0, count - 1 }(game_control.random());

        AlienControl& alien1_control = *state.controls.get<AlienControl>(alien1_to_swerve);
        AlienControl& alien2_control = *state.controls.get<AlienControl>(alien2_to_swerve);

        alien1_control.start_swerve(velocity, alien2_control.get().column(), alien2_control.get().row());
        alien2_control.start_swerve(velocity, alien1_control.get().column(), alien1_control.get().row());
    }
}

sf::Vector2f AlienGridControl::origin() const
{
    return m_origin;
}

float AlienGridControl::speed_multiplier() const
{
    return m_speed_multiplier;
}

void AlienGridControl::reset_swerve_timer(std::mt19937& random)
{
    m_swerve_timer =
        std::uniform_real_distribution<float>{ 2.0f, 8.0f }(random);
}

void AlienGridControl::draw(Layer& layer)
{

}