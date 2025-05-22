#include "gmock/gmock.h"
#include "fix_keyword.cpp"

TEST(Group, StringŸ���ǵ��̸�������Day���ǹ��ϴ¼��ڷι�ȯ)
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

TEST(Group, Day���ǹ��ϴ¼��ڸ����������϶Ǵ��ָ����з��Ҽ��־���Ѵ�)
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
