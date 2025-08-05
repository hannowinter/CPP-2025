#include <gtest/gtest.h>
// #include <gmock/gmock.h>
#include "../src/control/Controls.hpp"

template <size_t I>
struct ProxyControl : public Control
{
	size_t data;

	ProxyControl(size_t data) :
		data{ data } { }

	void add_children(ControlList& controls) override
	{

	}

	void init(const ControlList& controls) override
	{

	}

	void update(const UpdateState& state) override
	{

	}

	void draw(LayerManager& layers) override
	{

	}
};

struct ControlListTest : public testing::Test
{
protected:
	ControlListTest()
	{

	}

	ControlList controls;
};


TEST_F(ControlListTest, controlList)
{
	// contains no controls initially
	EXPECT_EQ(std::ranges::size(controls), 0);

	ProxyControl<0>& control41 = controls.add<ProxyControl<0>>(41);
	ProxyControl<0>& control17 = controls.add<ProxyControl<0>>(17);
	ProxyControl<1>& control67 = controls.add<ProxyControl<1>>(67);

	// still no elements because we haven't called "execute_requests" yet
	EXPECT_EQ(std::ranges::size(controls), 0);
	EXPECT_EQ(controls.count<ProxyControl<0>>(), 0);
	EXPECT_EQ(controls.count<ProxyControl<1>>(), 0);

	controls.execute_requests();

	// check the expected counts
	EXPECT_EQ(std::ranges::size(controls), 3);
	EXPECT_EQ(controls.count<ProxyControl<0>>(), 2);
	EXPECT_EQ(controls.count<ProxyControl<1>>(), 1);
	EXPECT_EQ(controls.count<ProxyControl<2>>(), 0);

	// check if the controls are accessible via "get" and in the correct order
	EXPECT_EQ(controls.get<ProxyControl<0>>(0)->data, control41.data);
	EXPECT_EQ(controls.get<ProxyControl<0>>(1)->data, control17.data);
	EXPECT_EQ(controls.get<ProxyControl<1>>(0)->data, control67.data);

	// check if "get" returns nullptr for controls that are not contained
	EXPECT_EQ(controls.get<ProxyControl<0>>(3), nullptr);
	EXPECT_EQ(controls.get<ProxyControl<2>>(0), nullptr);

	controls.remove(&control41);
	controls.remove(&control67);

	// no change because we haven't calledd "execute_requests" yet
	EXPECT_EQ(std::ranges::size(controls), 3);

	controls.execute_requests();

	// check the expected new counts
	EXPECT_EQ(std::ranges::size(controls), 1);
	EXPECT_EQ(controls.count<ProxyControl<0>>(), 1);
	EXPECT_EQ(controls.count<ProxyControl<1>>(), 0);

	// control17 should now be at index 0 instead of index 1
	EXPECT_EQ(controls.get<ProxyControl<0>>(0)->data, control17.data);

	// check if trying to get the controls we removed now yields nullptr
	EXPECT_EQ(controls.get<ProxyControl<0>>(1), nullptr);
	EXPECT_EQ(controls.get<ProxyControl<1>>(0), nullptr);
}

int main2(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}