#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

#define ARRAGNE_THRESHOLD (2100000000)
#define UZ_INITIAL (10)

struct Node2 {
	std::string name;
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

std::vector<Node2> weekDayBest[maxWeekDay]; //월 ~ 금
std::vector<Node2> weekTypeBest[maxDayType]; //평일, 주말

int UZ = UZ_INITIAL - 1;

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
	if (day < monDay || day >= maxWeekDay)
		return inValidType;

	if (day >= monDay && day <= friDay)
		return weekDays;

	return weekEnd;
}

int getDayOfWeek(const std::string& dayString)
{
	const std::string dayOfWeek[maxWeekDay] = { "monday", "tuesday", "wednesday", "thursday", "friday", "saturday", "sunday" };

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

void registerWeekDayBest(int day, const std::string& keyWord)
{
	if (weekDayBest[day].size() < 10) {
		weekDayBest[day].push_back({ keyWord, UZ });
		std::sort(weekDayBest[day].begin(), weekDayBest[day].end());
	}

	if (weekDayBest[day].back().point < UZ) { // 가장 작은 keyword의 점수가 UZ 값보다 작으면 pop하고 push
		weekDayBest[day].pop_back();
		weekDayBest[day].push_back({ keyWord, UZ });
		std::sort(weekDayBest[day].begin(), weekDayBest[day].end());
	}
}

void registerWeekTypeBest(int dayType, std::string& keyWord)
{
	if (weekTypeBest[dayType].size() < 10) {
		weekTypeBest[dayType].push_back({ keyWord, UZ });
		std::sort(weekTypeBest[dayType].begin(), weekTypeBest[dayType].end());
	}

	if (weekTypeBest[dayType].back().point < UZ) {
		weekTypeBest[dayType].pop_back();
		weekTypeBest[dayType].push_back({ keyWord, UZ });
		std::sort(weekTypeBest[dayType].begin(), weekTypeBest[dayType].end());
	}
}

bool getPerfectKeywordFromDayBest(int dayOfWeek, const std::string& keyWord, Node2& target)
{
	for (Node2& node : weekDayBest[dayOfWeek]) {
		if (node.name == keyWord) {
			node.point += (node.point * 0.1);
			target = node;
			return true;
		}
	}

	return false;
}

bool getPerfectKeywordFromDayType(int dayType, const std::string& keyWord, Node2& target)
{
	for (Node2& node : weekTypeBest[dayType]) {
		if (node.name == keyWord) {
			node.point += (node.point * 0.1);
			target = node;
			return true;
		}
	}

	return false;
}

std::string input2(std::string keyWord, std::string dayString) {

	UZ++;
	int dayOfWeek = getDayOfWeek(dayString);
	int dayType = getDayType(dayOfWeek);
	int point = UZ;

	//관리 목록에 존재하는지 확인
	//관리되는 키워드이면 점수가 증가

	int max1 = 0;
	int max2 = 0;

	int perfectFlag = 0;
	Node2 target;
	if (getPerfectKeywordFromDayBest(dayOfWeek, keyWord, target))
	{
		max1 = target.point;
		perfectFlag = 1;
	}

	if (getPerfectKeywordFromDayType(dayType, keyWord, target))
	{
		max2 = target.point;
	}

	//재정렬 작업
	if (UZ >= ARRAGNE_THRESHOLD || max1 >= ARRAGNE_THRESHOLD || max2 >= ARRAGNE_THRESHOLD) {
		UZ = UZ_INITIAL - 1;
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

	for (Node2& node : weekTypeBest[dayType]) {
		if (similar(node.name, keyWord)) {
			return node.name;
		}
	}

	//완벽 HIT / 찰떡 HIT 둘다 아닌경우
	registerWeekDayBest(dayOfWeek, keyWord);
	registerWeekTypeBest(dayType, keyWord);

	return keyWord;
}

void input() {
	std::ifstream fin{ "keyword_weekday_500.txt" }; //500개 데이터 입력
	for (int i = 0; i < 500; i++) {
		std::string keyWord, dayOfTheWeek;
		fin >> keyWord >> dayOfTheWeek;
		std::string ret = input2(keyWord, dayOfTheWeek);
		std::cout << ret << "\n";
	}
}

int main() {
	input();
}