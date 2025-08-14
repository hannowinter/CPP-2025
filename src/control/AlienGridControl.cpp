#include "AlienGridControl.hpp"

#include "PlayerControl.hpp"
#include "AlienControl.hpp"
#include "GameControl.hpp"
#include "../model/Constants.hpp"

// Create AlienGridControl
AlienGridControl::AlienGridControl() :
    m_intensity{ constants::alien_grid::MIN_INTENSITY },
    m_mode{ SHIFT_RIGHT },
    m_prev_mode{ m_mode },
    m_origin{ constants::PADDING, 5 * constants::PADDING },
    m_descend_timer{},
    m_swerve_timer{},
    m_bottom{ 0.0f }
{
    
}

void AlienGridControl::add_children(ControlList& controls)
{
    // Fill grid with aliens
    for (size_t x = 0; x < constants::alien_grid::COLUMNS; x++)
    {
        size_t y = 0;
        for (size_t i = 0; i < constants::alien_grid::GREEN_ROWS; i++, y++)
        {
            controls.add<AlienControl>(Alien::GREEN, m_origin, x, y);
        }
        for (size_t i = 0; i < constants::alien_grid::YELLOW_ROWS; i++, y++)
        {
            controls.add<AlienControl>(Alien::YELLOW, m_origin, x, y);
        }
        for (size_t i = 0; i < constants::alien_grid::RED_ROWS; i++, y++)
        {
            controls.add<AlienControl>(Alien::RED, m_origin, x, y);
        }
    }
}

// Initialize this controller
void AlienGridControl::init(const ControlList& controls)
{
    // Get game controller
    GameControl& game_control = *controls.get<GameControl>();

    // Set random time until first swerve
    reset_swerve_timer(game_control.random());
}

// Execute all updates
void AlienGridControl::update(const UpdateState& state)
{
    // Get game controller
    GameControl& game_control = *state.controls.get<GameControl>();
    
    // Calculate intensity
    // "ratio == 0.1" initially, "ratio == 1.0" in level 10
    float ratio = (float) game_control.state().level / 10.0f;
    ratio = std::pow(ratio, 3.0f); // make the intensity increase more gentle initially and steeper towards the end
    m_intensity = std::lerp(
        constants::alien_grid::MIN_INTENSITY,
        constants::alien_grid::MAX_INTENSITY,
        ratio
    );

    // Determine bottomost point in grid
    size_t alien_max_row = 0;
    for (const auto& control : state.controls)
    {
        if (AlienControl* alien = control->is<AlienControl>())
            alien_max_row = std::max(alien->get().row(), alien_max_row);
    }

    m_bottom = m_origin.y + alien_max_row * (constants::alien::SIZE.y + constants::alien_grid::SPACING.y) + constants::alien::SIZE.x;

    // calculate the grid's leftmost and rightmost points, 
    // used to determine when the grid reaches the screen's left or right side
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

    float alien_grid_leftmost = 
        alien_min_col * (constants::alien::SIZE.x + constants::alien_grid::SPACING.x);
    float alien_grid_rightmost = 
        alien_max_col * (constants::alien::SIZE.x + constants::alien_grid::SPACING.x) + constants::alien::SIZE.x;

    // move the grid's origin depending on the current mode
    sf::Vector2f velocity{};
    switch (m_mode)
    {
    case SHIFT_RIGHT:
        // Determine velocity and move origin
        velocity = { constants::alien_grid::SHIFT_SPEED, 0.0f };
        m_origin += velocity * state.delta * intensity();

        // Check if border has been reached
        if (m_origin.x + alien_grid_rightmost > constants::VIEW_WIDTH - constants::PADDING)
        {
            m_origin.x = constants::VIEW_WIDTH - constants::PADDING - alien_grid_rightmost;
            set_mode(DESCEND);
        }

        break;
    case SHIFT_LEFT:
        // Determine velocity and move origin
        velocity = { -constants::alien_grid::SHIFT_SPEED, 0.0f };
        m_origin += velocity * state.delta * intensity();

        // Check if border has been reached
        if (m_origin.x + alien_grid_leftmost < constants::PADDING)
        {
            m_origin.x = constants::PADDING - alien_grid_leftmost;
            set_mode(DESCEND);
        }

        break;
    case DESCEND:
        // Determine velocity and move origin
        velocity = { 0.0f, constants::alien_grid::DESCEND_SPEED };
        m_origin += velocity * state.delta * intensity();

        // Check if descend phase is over
        m_descend_timer += state.delta;
        if (m_descend_timer >= constants::alien_grid::DESCEND_DURATION / intensity())
        {
            m_descend_timer = 0.0f;
            if (m_prev_mode == SHIFT_LEFT)
                set_mode(SHIFT_RIGHT);
            if (m_prev_mode == SHIFT_RIGHT)
                set_mode(SHIFT_LEFT);
        }

        break;
    }

    // check if a new pair of aliens needs to initiate a swerve
    m_swerve_timer -= state.delta;
    if (m_swerve_timer <= 0.0f)
    {
        reset_swerve_timer(game_control.random());
        size_t count = state.controls.count<AlienControl>();

        // Choose aliens to swerve
        size_t alien1_to_swerve = SIZE_MAX;
        size_t alien2_to_swerve = SIZE_MAX;

        do
        {
            alien1_to_swerve =
                std::uniform_int_distribution<size_t>{ 0, count - 1 }(game_control.random());
            alien2_to_swerve =
                std::uniform_int_distribution<size_t>{ 0, count - 1 }(game_control.random());
        } 
        while (alien1_to_swerve == alien2_to_swerve && count >= 2); // prevent "alien1_to_swerve == alien2_to_swerve" if possible

        // Make chosen aliens swerve
        AlienControl& alien1_control = *state.controls.get<AlienControl>(alien1_to_swerve);
        AlienControl& alien2_control = *state.controls.get<AlienControl>(alien2_to_swerve);

        alien1_control.start_swerve(velocity, alien2_control.get().column(), alien2_control.get().row());
        alien2_control.start_swerve(velocity, alien1_control.get().column(), alien1_control.get().row());
    }
}

// Get origin of grid
sf::Vector2f AlienGridControl::origin() const
{
    return m_origin;
}

// Get current intensity
float AlienGridControl::intensity() const
{
    return m_intensity;
}

// Set mode of grid
void AlienGridControl::set_mode(Mode new_mode)
{
    m_prev_mode = m_mode;
    m_mode = new_mode;
}

// Randomly set time until next swerve
void AlienGridControl::reset_swerve_timer(std::mt19937& random)
{
    m_swerve_timer = std::uniform_real_distribution<float>{
        constants::alien_grid::SWERVE_MIN_WAIT, 
        constants::alien_grid::SWERVE_MAX_WAIT 
    }(random);
}

// Draw grid
void AlienGridControl::draw(LayerManager& layers)
{
    // nothing to do here
}

float AlienGridControl::get_bottom()
{
    return m_bottom;
}
