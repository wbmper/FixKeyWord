#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include "gmock/gmock.h"

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

	void closeData()
	{
		fin.close();
	}
	void updateInputData() {
	}
	void printOutput() {
	}
private:
	std::vector<InputData> input;
	std::ifstream fin;
};

class IKeywordManager
{
public:
	virtual void initializePoint() = 0;
	virtual void registerData() = 0;
	virtual bool getPerfectKeyword() = 0;
	virtual bool getSimilarKeyword() = 0;
private:
	std::vector<keyWordManageData> data;
};

class DayBestManager : public IKeywordManager
{
};

class TypeBestManager : public IKeywordManager
{
};
