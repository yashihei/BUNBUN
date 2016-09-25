#pragma once
#include <vector>
#include <fstream>
#include <string>

namespace HiScore {
	inline std::vector<int> load() {
		std::ifstream ifs("dat/score.txt");
		if (!ifs)
			std::runtime_error("Failed open score.txt");

		std::vector<int> scores;
		std::string buf;
		while (std::getline(ifs, buf)) {
			scores.push_back(std::atoi(buf.c_str()));
		}
		return scores;
	}

	inline void regist(int score) {
		std::vector<int> scores = load();
		for (int i = 0; i < scores.size(); i++) {
			if (scores[i] < score) {
				scores.insert(scores.begin() + i, score);
				if (scores.size() > 10)
					scores.pop_back();
				break;
			}
		}

		std::ofstream ofs("dat/score.txt", std::ios::trunc);
		if (!ofs)
			std::runtime_error("Failed open score.txt");
		for (int i = 0; i < scores.size(); i++) {
			ofs << scores[i] << "\n";
		}
	}
}
