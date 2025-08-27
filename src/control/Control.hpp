#ifndef CONTROLS_H
#define CONTROLS_H

#include "Inputs.hpp"
#include "../view/LayerManager.hpp"
#include <vector>
#include <memory>

class ControlList; // forward declaration

// Information to be passed on every update (time passed, current inputs, list of all controls)
struct UpdateState
{
	float delta;
	const Inputs& inputs;
	ControlList& controls;
};

// Base class for all control classes
class Control
{
public:
	virtual ~Control() = default;

	// Methods for all controllers

	// Adds all this control's child controls (if it even has any).
	virtual void add_children(ControlList& controls);
	// Initializes this control.
	virtual void init(const ControlList& controls) = 0;
	// Updates this control.
	virtual void update(const UpdateState& state) = 0;
	// Draws this control.
	virtual void draw(LayerManager& layers) = 0;

	// Checks if Control is of specified type "C" and if true, returns it as a "C" pointer.
	// Useful for "if" statements.
	template <std::derived_from<Control> C>
	const C* is() const
	{
		return dynamic_cast<const C*>(this);
	}

	template <std::derived_from<Control> C>
	C* is()
	{
		return dynamic_cast<C*>(this);
	}
};

// Stores all controls.
// 
// All changes are request-based; when adding or removing a control, it adds it as a request to a temporary list.
// Only when calling "execute_requests", these requests actually take effect.
// The reason for this design is the following:
// The "update" method iterates through all controls in order to update each control.
// During this process, we need to prevent any insertion or erasure of elements to or from the list,
// otherwise its iterators may be invalidated, leading to undefined behavior.
// The "request_add" and "request_remove" methods may be called during this iteration process.
// Instead of immediately inserting or erasing the specified control, we first put them into
// separate lists "m_controls_to_add" and "m_controls_to_remove" to remember them,
// and only after the iteration process is done, we insert or erase them to or from "m_controls"
// using the "execute_requests" function.
class ControlList
{
	using control_list_t = std::vector<std::unique_ptr<Control>>;

public:
	// Calls every control's "update" method.
	void update(float delta, const Inputs& inputs);
	// Calls every control's "draw" method.
	void draw(LayerManager& layers);

	// Carries out all changes requested via "request_add" and "request_remove".
	void execute_requests();

	// Constructs a new control of type "C", calls "add_children" on it and 
	// adds a request to add it to the list and to initialize it.
	// The control is only added and initialized when calling "execute_requests",
	// "add_children" however gets called immediately.
	template <std::derived_from<Control> C, typename... ArgTs> requires
		std::is_constructible_v<C, ArgTs...>
	C& request_add(ArgTs&&... args)
	{
		C& result = dynamic_cast<C&>(
			*m_controls_to_add.emplace_back(std::make_unique<C>(std::forward<ArgTs>(args)...))
		);
		result.add_children(*this);
		m_controls_to_init.push_back(&result);
		return result;
	}

	// Adds a request to remove the specified control from the list.
	// The control is only removed when calling "execute_requests".
	void request_remove(const Control* control);

	// Gets the count of all controls of type "C".
	template <std::derived_from<Control> C>
	size_t count() const
	{
		return std::count_if(
			m_controls.begin(),
			m_controls.end(),
			[](const auto& control) { return control->template is<C>(); }
		);
	}

	// Gets the n-th controls of type "C".
	// Returns "nullptr" if it doesn't exist.
	template <std::derived_from<Control> C>
	C* get(size_t nth = 0) const
	{
		for (const auto& control : m_controls)
		{
			if (C* c = control->is<C>())
			{
				if (nth == 0)
					return c;
				else
					nth--;
			}
		}
		return nullptr;
	}

	// Clears all initialize requests.
	void clear_init_requests();

	// Provide iterators in order to allow for using range-based for loops.
	typename control_list_t::iterator begin();
	typename control_list_t::iterator end();
	typename control_list_t::const_iterator begin() const;
	typename control_list_t::const_iterator end() const;

private:
	control_list_t m_controls; // currently active controls

	// All requests
	std::vector<std::unique_ptr<Control>> m_controls_to_add; // controls to be added
	std::vector<const Control*> m_controls_to_remove; // controls to be removed
	std::vector<Control*> m_controls_to_init; // controls to be initialized
};

#endif