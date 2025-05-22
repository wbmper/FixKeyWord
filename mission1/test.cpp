#include "gmock/gmock.h"
#include "fix_keyword.cpp"

TEST(Group, String타입의데이를받으며Day를의미하는숫자로반환)
{
	EXPECT_EQ(getDayOfWeek("monday"), weekDays);
	EXPECT_EQ(getDayOfWeek("tuesday"), weekDays);
	EXPECT_EQ(getDayOfWeek("wednesday"), weekDays);
	EXPECT_EQ(getDayOfWeek("thursday"), weekDays);
	EXPECT_EQ(getDayOfWeek("friday"), weekDays);
	EXPECT_EQ(getDayOfWeek("saturday"), weekEnd);
	EXPECT_EQ(getDayOfWeek("sunday"), weekEnd);
	EXPECT_EQ(getDayOfWeek("adfadf"), inValid); // Invalid
}

TEST(Group, Day를의미하는숫자를받으면평일또는주말을분류할수있어야한다)
{
	EXPECT_EQ(getDayType(monDay), weekDays);
	EXPECT_EQ(getDayType(tuesDay), weekDays);
	EXPECT_EQ(getDayType(wednesDay), weekDays);
	EXPECT_EQ(getDayType(thursDay), weekDays);
	EXPECT_EQ(getDayType(friDay), weekDays);
	EXPECT_EQ(getDayType(saturDay), weekEnd);
	EXPECT_EQ(getDayType(sunDay), weekEnd);
	EXPECT_EQ(getDayType(1000), inValid); // Invalid
}
