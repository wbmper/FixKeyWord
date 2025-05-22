#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include "gmock/gmock.h"

using namespace std;

#define ARRAGNE_THRESHOLD (2100000000)
#define UZ_INITIAL (10)

struct Node {
	string w;
	string wk;
};

struct Node2 {
	string name;
	int point;

	bool operator<(const Node2& other) const {
		return point < other.point;
	}
};

enum DAY_OF_WEEK
{
	monDay = 0,
	tuesDay,
	wednesDay,
	thursDay,
	friDay,
	saturDay,
	sunDay,
	maxWeekDay,
	inValidDay = 0xFF
};

enum TYPE_OF_DAY
{
	weekDays = 0,
	weekEnd,
	maxDayType,
	inValidType = 0xFF
};

vector<Node2> weekDayBest[maxWeekDay]; //월 ~ 금
vector<Node2> weekTypeBest[maxDayType]; //평일, 주말

int UZ = UZ_INITIAL;

// 레벤슈타인 거리 계산 알고리즘 (문자열 유사도 검사)
int levenshtein(const std::string& a, const std::string& b) {
	const size_t len_a = a.size();
	const size_t len_b = b.size();

	std::vector<std::vector<int>> d(len_a + 1, std::vector<int>(len_b + 1));

	for (size_t i = 0; i <= len_a; ++i) d[i][0] = i;
	for (size_t j = 0; j <= len_b; ++j) d[0][j] = j;

	for (size_t i = 1; i <= len_a; ++i) {
		for (size_t j = 1; j <= len_b; ++j) {
			if (a[i - 1] == b[j - 1])
				d[i][j] = d[i - 1][j - 1];
			else
				d[i][j] = 1 + std::min({ d[i - 1][j], d[i][j - 1], d[i - 1][j - 1] });
		}
	}
	return d[len_a][len_b];
}

// 점수 환산
bool similar(const std::string& keyWord1, const std::string& keyWord2) {
	if (keyWord1.empty() || keyWord2.empty()) return true;

	int dist = levenshtein(keyWord1, keyWord2);
	int max_len = std::max(keyWord1.length(), keyWord2.length());
	// 유사도 비율 (1.0: 완전히 같음, 0.0: 전혀 다름)
	double similarity = 1.0 - (double)dist / max_len;
	int score = 1 + static_cast<int>(similarity * 99);

	if (score >= 80) return true;
	return false;
}

int getDayType(int day)
{
	if (day < monDay || day >= maxDayType)
		return inValidType;

	if (day >= monDay && day <= friDay)
		return weekDays;

	return weekEnd;
}

int getDayOfWeek(const std::string& dayString)
{
	const string dayOfWeek[maxWeekDay] = { "monday", "tuesday", "wednesday", "thursday", "friday", "saturday", "sunday" };

	for (int day = monDay; day < maxWeekDay; day++)
	{
		if (dayOfWeek[day] == dayString)
			return day;
	}

	return inValidDay;
}

void initializeDayBestPoint()
{
	for (int day = 0; day < maxWeekDay; day++) {
		int num = 1;
		for (Node2& node : weekDayBest[day]) {
			node.point = num;
			num++;
		}
	}
}

void initializeTypeBestPoint()
{
	for (int dayType = 0; dayType < maxDayType; dayType++) {
		int num = 1;
		for (Node2& node : weekTypeBest[dayType]) {
			node.point = num;
			num++;
		}
	}
}

string input2(string keyWord, string dayString) {
	int dayOfWeek = getDayOfWeek(dayString);
	int dayType = getDayType(dayOfWeek);
	int point = UZ;

	//관리 목록에 존재하는지 확인
	//관리되는 키워드이면 점수가 증가

	int max1 = 0;
	int max2 = 0;

	int perfectFlag = 0;
	for (Node2& node : weekDayBest[dayOfWeek]) {
		if (node.name == keyWord) {
			max1 = node.point + (node.point * 0.1);
			node.point += (node.point * 0.1);
			perfectFlag = 1;
			break;
		}
	}

	for (Node2& node : weekTypeBest[dayType]) {
		if (node.name == keyWord) {
			max2 = node.point + (node.point * 0.1);
			node.point += (node.point * 0.1);
			break;
		}
	}

	//재정렬 작업
	if (UZ >= ARRAGNE_THRESHOLD || max1 >= ARRAGNE_THRESHOLD || max2 >= ARRAGNE_THRESHOLD) {
		UZ = UZ_INITIAL;
		initializeDayBestPoint();
		initializeTypeBestPoint();
	}

	if (perfectFlag == 1) {
		return keyWord;
	}


	//찰떡 HIT
	for (Node2& node : weekDayBest[dayOfWeek]) {
		if (similar(node.name, keyWord)) {
			return node.name;
		}
	}

	for (Node2& node : weekTypeBest[dayOfWeek]) {
		if (similar(node.name, keyWord)) {
			return node.name;
		}
	}

	//완벽 HIT / 찰떡 HIT 둘다 아닌경우
	if (weekDayBest[dayOfWeek].size() < 10) {
		weekDayBest[dayOfWeek].push_back({ keyWord, point });
		std::sort(weekDayBest[dayOfWeek].begin(), weekDayBest[dayOfWeek].end());
	}

	if (weekTypeBest[dayOfWeek].size() < 10) {
		weekTypeBest[dayOfWeek].push_back({ keyWord, point });
		std::sort(weekTypeBest[dayOfWeek].begin(), weekTypeBest[dayOfWeek].end());
	}

	if (weekDayBest[dayOfWeek].size() == 10) {
		if (weekDayBest[dayOfWeek].back().point < point) {
			weekDayBest[dayOfWeek].pop_back();
			weekDayBest[dayOfWeek].push_back({ keyWord, point });
			std::sort(weekDayBest[dayOfWeek].begin(), weekDayBest[dayOfWeek].end());
		}
	}

	if (weekTypeBest[dayOfWeek].size() == 10) {
		if (weekTypeBest[dayOfWeek].back().point < point) {
			weekTypeBest[dayOfWeek].pop_back();
			weekTypeBest[dayOfWeek].push_back({ keyWord, point });
			std::sort(weekTypeBest[dayOfWeek].begin(), weekTypeBest[dayOfWeek].end());
		}
	}

	return keyWord;
}

void input() {
	ifstream fin{ "keyword_weekday_500.txt" }; //500개 데이터 입력
	for (int i = 0; i < 500; i++) {
		string keyWord, dayOfTheWeek;
		fin >> keyWord >> dayOfTheWeek;
		string ret = input2(keyWord, dayOfTheWeek);
		std::cout << ret << "\n";
	}
}

int main() {
	input();
	testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}