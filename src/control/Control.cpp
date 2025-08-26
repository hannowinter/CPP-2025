#include "Control.hpp"

void Control::add_children(ControlList& controls) {}

// Calls update on all controls in list.
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

// Calls draw on all controls in list.
void ControlList::draw(LayerManager& layers)
{
	for (const auto& control : m_controls)
		control->draw(layers);
}

// Carries out all requests.
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
	// This is a common pattern for removing elements satisfying a specific condition.
	// "std::remove_if" removes all elements from the range "[m_controls.begin(), m_controls.end())"
	// satisfying the lambda's condition, letting the remaining elements fill their gaps.
	// As "std::remove_if" can't change the range's length, it returns an iterator to the end of the
	// remaining elements, which we further pass to "erase" to cut off the leftover section.
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

void ControlList::request_remove(const Control* control)
{
	m_controls_to_remove.push_back(control);
}

void ControlList::clear_init_requests()
{
	m_controls_to_init.clear();
}

// Creates iterator starting at first control in list.
typename ControlList::control_list_t::iterator ControlList::begin()
{
	return m_controls.begin();
}

// Creates iterator starting at one past the last control in list.
typename ControlList::control_list_t::iterator ControlList::end()
{
	return m_controls.end();
}

// Creates read-only iterator starting at first control in list.
typename ControlList::control_list_t::const_iterator ControlList::begin() const
{
	return m_controls.begin();
}

// Creates read-only iterator starting at one past the last control in list.
typename ControlList::control_list_t::const_iterator ControlList::end() const
{
	return m_controls.end();
}
