#include "gmock/gmock.h"
#include "fix_keyword.cpp"

class Fixture : public ::testing::Test
{
public:
	std::string filename = "keyword_weekday_500.txt";
	std::string wrongFilename = "adfd.txt";
	InputOutputManager ioManager;
};

TEST_F(Fixture, InputOutManager�������Ͽ����Ѵ�)
{
	EXPECT_EQ(ioManager.openData(filename), true);
	EXPECT_EQ(ioManager.openData(wrongFilename), false);
	ioManager.closeData();
}

int main()
{
	testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}