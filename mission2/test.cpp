#include "gmock/gmock.h"
#include "fix_keyword.cpp"

class Fixture : public ::testing::Test
{
public:
	std::string filename = "keyword_weekday_500.txt";
	std::string wrongFilename = "adfd.txt";
	std::string expectedDataFile = "expected_output.txt";
	InputOutputManager ioManager;
};

TEST_F(Fixture, InputOutManager�������Ͽ����Ѵ�)
{
	EXPECT_EQ(ioManager.openData(filename), true);
	EXPECT_EQ(ioManager.openData(wrongFilename), false);
}

TEST_F(Fixture, InputOutManager����Input�����͸�������Ʈ�Ѵ�)
{
	EXPECT_EQ(ioManager.openData(filename), true);
	ioManager.execute();
}

int main()
{
	testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}