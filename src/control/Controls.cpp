#include "Controls.hpp"

void Control::spawn_children(ControlList& controls) {}

// Call update on all controllers in list
void ControlList::update(float delta, const Inputs& inputs)
{
	// Store update information
	UpdateState update_state{
		.delta = delta,
		.inputs = inputs,
		.controls = *this
	};

	// Update all controls
	for (const auto& control : m_controls)
		control->update(update_state);
}

// Call draw on every stored controller
void ControlList::draw(LayerManager& layers)
{
	for (const auto& control : m_controls)
		control->draw(layers);
}

void ControlList::execute_requests()
{
	// Remove all controls to be removed from list
	m_controls.erase(std::remove_if(m_controls.begin(), m_controls.end(), [&](const auto& elem)
		{
			return std::find(
				m_controls_to_remove.begin(),
				m_controls_to_remove.end(),
				elem.get()
			) != m_controls_to_remove.end();
		}), m_controls.end());
	m_controls_to_remove.clear();

	// Move all new controls to list
	m_controls.insert(
		m_controls.end(),
		std::make_move_iterator(m_controls_to_add.begin()),
		std::make_move_iterator(m_controls_to_add.end())
	);
	m_controls_to_add.clear();

	// Initialize all controls that are left to initialize
	for (auto& control : m_controls_to_init)
		control->init(*this);
	m_controls_to_init.clear();
}

void ControlList::remove(const Control* control)
{
	m_controls_to_remove.push_back(control);
}

// Create iterator starting at first controller in list
typename ControlList::control_list_t::iterator ControlList::begin()
{
	return m_controls.begin();
}

// Create iterator starting at one past the last controller in list
typename ControlList::control_list_t::iterator ControlList::end()
{
	return m_controls.end();
}

// Create read-only iterator starting at first controller in list
typename ControlList::control_list_t::const_iterator ControlList::begin() const
{
	return m_controls.begin();
}

// Create read-only iterator starting at one past the last controller in list
typename ControlList::control_list_t::const_iterator ControlList::end() const
{
	return m_controls.end();
}