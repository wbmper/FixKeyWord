#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include "gmock/gmock.h"

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

struct InputData {
	std::string keyWord;
	std::string weekDay;
};
struct keyWordManageData {
	std::string name;
	int point;

	bool operator<(const keyWordManageData& other) const {
		return point < other.point;
	}
};

class UZManager
{
public:
	static UZManager& GetInstance() {
		static UZManager instance;
		return instance;
	}
	void Initialize()
	{
		UZ = initializeVal - 1;
	}
	void update()
	{
		UZ++;
	}
	int getUZ()
	{
		return UZ;
	}
private:
	const int initializeVal = 10;
	int UZ;
};

class SimilarityCalculator
{
public:
	bool isSimilar(const std::string& keyWord1, const std::string& keyWord2)
	{
		return false;
	}

private:
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
};

class InputOutputManager
{
public:
	bool openData(const std::string& fileName) {
		fin.open(fileName);

		if (fin.fail()) {
			std::cerr << "파일을 열 수 없습니다." << std::endl;
			return false;
		}

		return true;
	}
	void execute() {
		updateInputData();
		printOutput();
		closeData();
	}
private:
	std::vector<InputData> input = {};
	std::ifstream fin = {};
	void updateInputData() {
		std::string line;
		while (true) {
			std::string keyWord, dayOfTheWeek;
			fin >> keyWord >> dayOfTheWeek;
			input.push_back({ keyWord , dayOfTheWeek });
			if (!std::getline(fin, line))
			{
				break;
			}
		}
	}
	void printOutput() {
	}
	void closeData()
	{
		fin.close();
	}
};

class IKeywordManager
{
public:
	virtual void initializePoint() = 0;
	virtual void registerData(int index, const std::string& keyWord) = 0;
	virtual bool getPerfectKeyword(int index, const std::string& keyWord, keyWordManageData& target) = 0;
	virtual std::string getSimilarKeyword(int index, std::string& keyWord) = 0;
private:
	std::vector<keyWordManageData> data;
};

class DayBestManager : public IKeywordManager
{
	void initializePoint() override
	{
		for (int day = 0; day < maxWeekDay; day++) {
			int num = 1;
			for (keyWordManageData& node : weekDayBest[day]) {
				node.point = num;
				num++;
			}
		}
	};
	void registerData(int index, const std::string& keyWord) override
	{
		if (weekDayBest[index].size() < 10) {
			weekDayBest[index].push_back({ keyWord, UZManager::GetInstance().getUZ() });
			std::sort(weekDayBest[index].begin(), weekDayBest[index].end());
		}

		if (weekDayBest[index].back().point < UZManager::GetInstance().getUZ()) { // 가장 작은 keyword의 점수가 UZ 값보다 작으면 pop하고 push
			weekDayBest[index].pop_back();
			weekDayBest[index].push_back({ keyWord, UZManager::GetInstance().getUZ() });
			std::sort(weekDayBest[index].begin(), weekDayBest[index].end());
		}
	}
	bool getPerfectKeyword(int index, const std::string& keyWord, keyWordManageData& target) override
	{
		for (keyWordManageData& node : weekDayBest[index]) {
			if (node.name == keyWord) {
				node.point += (node.point * 0.1);
				target = node;
				return true;
			}
		}

		return false;
	}
	std::string getSimilarKeyword(int index, std::string& keyWord) override
	{
		std::string name;
		name.clear();
		for (keyWordManageData& node : weekDayBest[index]) {
			if (cal.isSimilar(node.name, keyWord)) {
				name = node.name;
				break;
			}
		}

		return name;
	}
private:
	std::vector<keyWordManageData> weekDayBest[maxWeekDay];
	SimilarityCalculator cal;
};

class TypeBestManager : public IKeywordManager
{
	void initializePoint() override
	{
		for (int day = 0; day < maxDayType; day++) {
			int num = 1;
			for (keyWordManageData& node : weekTypeBest[day]) {
				node.point = num;
				num++;
			}
		}
	};
	void registerData(int index, const std::string& keyWord) override
	{
		if (weekTypeBest[index].size() < 10) {
			weekTypeBest[index].push_back({ keyWord, UZManager::GetInstance().getUZ()});
			std::sort(weekTypeBest[index].begin(), weekTypeBest[index].end());
		}

		if (weekTypeBest[index].back().point < UZManager::GetInstance().getUZ()) { // 가장 작은 keyword의 점수가 UZ 값보다 작으면 pop하고 push
			weekTypeBest[index].pop_back();
			weekTypeBest[index].push_back({ keyWord, UZManager::GetInstance().getUZ() });
			std::sort(weekTypeBest[index].begin(), weekTypeBest[index].end());
		}
	}
	bool getPerfectKeyword(int index, const std::string& keyWord, keyWordManageData& target) override
	{
		for (keyWordManageData& node : weekTypeBest[index]) {
			if (node.name == keyWord) {
				node.point += (node.point * 0.1);
				target = node;
				return true;
			}
		}

		return false;
	}
	std::string getSimilarKeyword(int index, std::string& keyWord) override
	{
		std::string name;
		name.clear();
		for (keyWordManageData& node : weekTypeBest[index]) {
			if (cal.isSimilar(node.name, keyWord)) {
				name = node.name;
				break;
			}
		}

		return name;
	}
private:
	std::vector<keyWordManageData> weekTypeBest[maxDayType];
	SimilarityCalculator cal;
};
