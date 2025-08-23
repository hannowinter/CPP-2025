#include "TestCommon.hpp"
#include "../src/control/Control.hpp"

// mock control class used for testing
// if "AddChildren" is true, then it will add a child of index I + 10, 
// as long as this new index is less than or equal to 50
template <size_t I, bool AddChildren = false>
struct MockControl : public Control
{
	size_t data;
	bool initialized = false;
	size_t updates = 0;
	float delta_sum = 0.0f;
	size_t draws = 0;

	MockControl(size_t data) :
		data{ data } {
	}

	void add_children(ControlList& controls) override
	{
		if constexpr (AddChildren && I + 10 <= 50)
		{
			controls.request_add<MockControl<I + 10, true>>(data);
		}
	}

	void init(const ControlList& controls) override
	{
		initialized = true;
	}

	void update(const UpdateState& state) override
	{
		updates++;
		delta_sum += state.delta;
	}

	void draw(LayerManager& layers) override
	{
		draws++;
	}
};

// fixture for testing the ControlList class, containing all the
// necessary mock objects for calling the update and draw cycles
struct ControlListTest : public testing::Test
{
protected:
	ControlListTest() :
		mock_window{ sf::VideoMode{ { constants::VIEW_WIDTH, constants::VIEW_HEIGHT } }, "test" },
		mock_layers{ mock_window }
	{

	}

	ControlList controls;
	const Inputs mock_inputs;
	sf::RenderWindow mock_window;
	LayerManager mock_layers;
};

// testing whether querying list information using "count" and "get" gives the expected results
// in conjunction with list modifications by "add" and "request_remove"
TEST_F(ControlListTest, controlListAddRemoveCountGet)
{
	// contains no controls initially
	EXPECT_EQ(std::ranges::size(controls), 0);

	MockControl<0>& control41 = controls.request_add<MockControl<0>>(41);
	MockControl<0>& control17 = controls.request_add<MockControl<0>>(17);
	MockControl<1>& control67 = controls.request_add<MockControl<1>>(67);

	// still no elements because we haven't called "execute_requests" yet
	EXPECT_EQ(std::ranges::size(controls), 0);
	EXPECT_EQ(controls.count<MockControl<0>>(), 0);
	EXPECT_EQ(controls.count<MockControl<1>>(), 0);

	controls.execute_requests();

	// check the expected counts
	EXPECT_EQ(std::ranges::size(controls), 3);
	EXPECT_EQ(controls.count<MockControl<0>>(), 2);
	EXPECT_EQ(controls.count<MockControl<1>>(), 1);
	EXPECT_EQ(controls.count<MockControl<2>>(), 0);

	// check if the controls are accessible via "get" and in the correct order
	EXPECT_EQ(controls.get<MockControl<0>>(0)->data, control41.data);
	EXPECT_EQ(controls.get<MockControl<0>>(1)->data, control17.data);
	EXPECT_EQ(controls.get<MockControl<1>>(0)->data, control67.data);

	// check if "get" returns nullptr for controls that are not contained
	EXPECT_EQ(controls.get<MockControl<0>>(3), nullptr);
	EXPECT_EQ(controls.get<MockControl<2>>(0), nullptr);

	controls.request_remove(&control41);
	controls.request_remove(&control67);

	// no change because we haven't called "execute_requests" yet
	EXPECT_EQ(std::ranges::size(controls), 3);

	controls.execute_requests();

	// check the expected new counts
	EXPECT_EQ(std::ranges::size(controls), 1);
	EXPECT_EQ(controls.count<MockControl<0>>(), 1);
	EXPECT_EQ(controls.count<MockControl<1>>(), 0);

	// control17 should now be at index 0 instead of index 1
	EXPECT_EQ(controls.get<MockControl<0>>(0)->data, control17.data);

	// check if trying to get the controls we removed now yields nullptr
	EXPECT_EQ(controls.get<MockControl<0>>(1), nullptr);
	EXPECT_EQ(controls.get<MockControl<1>>(0), nullptr);

	// remove the last one and check if it's now empty
	controls.request_remove(controls.get<MockControl<0>>(0));
	controls.execute_requests();
	EXPECT_EQ(std::ranges::size(controls), 0);
}

// testing whether the "ControlList::init", "ControlList::update" and "ControlList::draw" functions 
// properly call the respective "Control::init", "Control::update" and "Control::draw" function on all
// of its elements
TEST_F(ControlListTest, controlListInitUpdateDraw)
{
	MockControl<0>& control0 = controls.request_add<MockControl<0>>(0);
	MockControl<1>& control1 = controls.request_add<MockControl<1>>(0);

	// should not be initialized yet
	EXPECT_FALSE(control0.initialized);
	EXPECT_FALSE(control1.initialized);

	controls.execute_requests();

	// only gets initialized when the "add" request gets executed
	EXPECT_TRUE(control0.initialized);
	EXPECT_TRUE(control1.initialized);

	// no updates or draws yet
	EXPECT_EQ(control0.updates, 0);
	EXPECT_EQ(control1.updates, 0);
	EXPECT_EQ(control0.draws, 0);
	EXPECT_EQ(control1.draws, 0);

	controls.update(1.0f, mock_inputs);
	controls.update(2.0f, mock_inputs);
	controls.update(3.0f, mock_inputs);

	controls.draw(mock_layers);
	controls.draw(mock_layers);
	controls.draw(mock_layers);
	controls.draw(mock_layers);

	// check the expected count of updates and draws
	EXPECT_EQ(control0.updates, 3);
	EXPECT_EQ(control0.delta_sum, 1.0f + 2.0f + 3.0f); // (floating point imprecisions won't occur with small whole numbers)
	EXPECT_EQ(control0.draws, 4);
}

// testing whether adding controls that each also add child controls behaves properly
TEST_F(ControlListTest, controlListAddChildren)
{
	// By passing "true" to the template, each MockControl will now recursively spawn a child of index I+10,
	// as long as I+10 <= 50.
	MockControl<0, true>& control0 = controls.request_add<MockControl<0, true>>(0);
	MockControl<15, true>& control15 = controls.request_add<MockControl<15, true>>(0);
	MockControl<40, true>& control40 = controls.request_add<MockControl<40, true>>(0);

	controls.execute_requests();

	// check the expected counts
	// We started with indices 0, 15 and 40, so we should expect the following children:
	// 0 -> 10  ->  20  ->  30  ->  40  ->  50
	//          15  ->  25  ->  35  ->  45
	//                              40  ->  50  
	EXPECT_EQ((controls.count<MockControl<0, true>>()), 1);
	EXPECT_EQ((controls.count<MockControl<10, true>>()), 1);
	EXPECT_EQ((controls.count<MockControl<20, true>>()), 1);
	EXPECT_EQ((controls.count<MockControl<30, true>>()), 1);
	EXPECT_EQ((controls.count<MockControl<40, true>>()), 2);
	EXPECT_EQ((controls.count<MockControl<50, true>>()), 2);
	EXPECT_EQ((controls.count<MockControl<15, true>>()), 1);
	EXPECT_EQ((controls.count<MockControl<25, true>>()), 1);
	EXPECT_EQ((controls.count<MockControl<35, true>>()), 1);
	EXPECT_EQ((controls.count<MockControl<45, true>>()), 1);

	// should not go higher than 50
	EXPECT_EQ((controls.count<MockControl<60, true>>()), 0);
	EXPECT_EQ((controls.count<MockControl<55, true>>()), 0);
}