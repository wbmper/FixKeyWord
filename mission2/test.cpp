#include "gmock/gmock.h"
#include "fix_keyword.cpp"

#define TEST 1

#if TEST
class Fixture : public ::testing::Test
{
public:
	std::string filename = "keyword_weekday_500.txt";
	std::string wrongFilename = "ad.txt";
	std::string expectedDataFile = "expected_output.txt";
	InputOutputManager ioManager;
};

TEST_F(Fixture, InputOutManager에서파일오픈한다)
{
	EXPECT_EQ(ioManager.openData(filename), true);
}

int main()
{
	testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}

#else

int main()
{
	InputOutputManager ioManager;
	std::string filename = "keyword_weekday_500.txt";
	ioManager.openData(filename);
	ioManager.execute();

	return 0;
}

#endif
