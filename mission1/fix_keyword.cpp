#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include "gmock/gmock.h"

using namespace std;

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

vector<Node2> weekBest[7]; //�� ~ ��
vector<Node2> twoBest[2]; //����, �ָ�
int UZ = 9;

// ������Ÿ�� �Ÿ� ��� �˰��� (���ڿ� ���絵 �˻�)
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

// ���� ȯ��
bool similer(const std::string& a, const std::string& b) {
	if (a.empty() && b.empty()) return 100;
	if (a.empty() || b.empty()) return 1;

	int dist = levenshtein(a, b);
	int max_len = std::max(a.length(), b.length());
	// ���絵 ���� (1.0: ������ ����, 0.0: ���� �ٸ�)
	double similarity = 1.0 - (double)dist / max_len;

	int score = 1 + static_cast<int>(similarity * 99);

	if (score >= 80) return true;
	return false;
}

string input2(string w, string wk) {
	UZ++;

	int index = 0;
	if (wk == "monday") index = 0;
	if (wk == "tuesday") index = 1;
	if (wk == "wednesday") index = 2;
	if (wk == "thursday") index = 3;
	if (wk == "friday") index = 4;
	if (wk == "saturday") index = 5;
	if (wk == "sunday") index = 6;

	//���� / �ָ�
	int index2 = 0;
	if (index >= 0 && index <= 4) index2 = 0;
	else index2 = 1;

	int point = UZ;

	//���� ��Ͽ� �����ϴ��� Ȯ��
	//�����Ǵ� Ű�����̸� ������ ����

	long long int max1 = 0;
	long long int max2 = 0;

	int flag = 0;
	for (Node2& node : weekBest[index]) {
		if (node.name == w) {
			max1 = node.point + (node.point * 0.1);
			node.point += (node.point * 0.1);
			flag = 1;
			break;
		}
	}

	for (Node2& node : twoBest[index2]) {
		if (node.name == w) {
			max2 = node.point + (node.point * 0.1);
			node.point += (node.point * 0.1);
			break;
		}
	}

	//������ �۾�
	if (UZ >= 2100000000 || max1 >= 2100000000 || max2 >= 2100000000) {
		UZ = 9;
		for (int i = 0; i < 5; i++) {
			int num = 1;
			for (Node2& node : weekBest[i]) {
				node.point = num;
				num++;
			}
		}
		for (int i = 0; i < 2; i++) {
			int num = 1;
			for (Node2& node : twoBest[i]) {
				node.point = num;
				num++;
			}
		}
	}

	if (flag == 1) {
		return w;
	}


	//���� HIT
	for (Node2& node : weekBest[index]) {
		if (similer(node.name, w)) {
			return node.name;
		}
	}

	for (Node2& node : twoBest[index]) {
		if (similer(node.name, w)) {
			return node.name;
		}
	}

	//�Ϻ� HIT / ���� HIT �Ѵ� �ƴѰ��
	if (weekBest[index].size() < 10) {
		weekBest[index].push_back({ w, point });
		std::sort(weekBest[index].begin(), weekBest[index].end());
	}

	if (twoBest[index].size() < 10) {
		twoBest[index].push_back({ w, point });
		std::sort(twoBest[index].begin(), twoBest[index].end());
	}

	if (weekBest[index].size() == 10) {
		if (weekBest[index].back().point < point) {
			weekBest[index].pop_back();
			weekBest[index].push_back({ w, point });
			std::sort(weekBest[index].begin(), weekBest[index].end());
		}
	}

	if (twoBest[index].size() == 10) {
		if (twoBest[index].back().point < point) {
			twoBest[index].pop_back();
			twoBest[index].push_back({ w, point });
			std::sort(twoBest[index].begin(), twoBest[index].end());
		}
	}

	return w;
}

void input() {
	ifstream fin{ "keyword_weekday_500.txt" }; //500�� ������ �Է�
	for (int i = 0; i < 500; i++) {
		string t1, t2;
		fin >> t1 >> t2;
		string ret = input2(t1, t2);
		std::cout << ret << "\n";
	}
}

int main() {
	input();
	testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}