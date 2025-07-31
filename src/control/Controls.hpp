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

// base class for all control classes
class Control
{
public:
	// Methods for all controllers
	virtual ~Control() = default;

	virtual void spawn_children(ControlList& controls);
	virtual void init(const ControlList& controls) = 0;
	virtual void update(const UpdateState& state) = 0;
	virtual void draw(LayerManager& layers) = 0;

	// Check if constant controller is of specified type and cast
	template <std::derived_from<Control> C>
	const C* is() const
	{
		return dynamic_cast<const C*>(this);
	}

	// Check if non-constant controller is of specified type and cast
	template <std::derived_from<Control> C>
	C* is()
	{
		return dynamic_cast<C*>(this);
	}
};

// Stores all controls.
class ControlList
{
	using control_list_t = std::vector<std::unique_ptr<Control>>;

public:
	// Used to call method on all controllers in list
	void update(float delta, const Inputs& inputs);
	void draw(LayerManager& layers);

	// Carries out all changes requested via "add" and "remove".
	void execute_requests();

	// Constructs a new control of type "C" and adds a request to add it to the list and to initialize it.
	// This will also call "spawn_children" afterwards.
	// The control is only added when calling "execute_requests".
	template <std::derived_from<Control> C, typename... ArgTs> requires
		std::is_constructible_v<C, ArgTs...>
	C& add(ArgTs&&... args)
	{
		C& result = dynamic_cast<C&>(
			*m_controls_to_add.emplace_back(std::make_unique<C>(std::forward<ArgTs>(args)...))
		);
		result.spawn_children(*this);
		m_controls_to_init.push_back(&result);
		return result;
	}

	// Adds a request to remove the specified control from the list.
	// The control is only removed when calling "execute_requests".
	void remove(const Control* control);

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

	// The "update" method iterates through "m_controls" in order to update each control.
	// During this process, we need to prevent any insertion or erasure of elements to or from "m_controls",
	// otherwise its iterators may be invalidated, leading to undefined behavior.
	// The "add" and "remove" methods may be called during this iteration process.
	// Instead of immediately inserting or erasing the specified control, we first put them into
	// separate lists "m_controls_to_add" and "m_controls_to_remove" to remember them,
	// and only after the iteration process is done, we insert or erase them to or from "m_controls"
	// using the "push_changes" function.
};

#endif