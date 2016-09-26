#pragma once
#include <vector>
#include <fstream>
#include <string>

namespace HiScore {
	inline std::vector<int> load() {
		std::ifstream ifs("dat/score.dat", std::ios::binary);
		if (!ifs)
			std::runtime_error("Failed open score.dat");
		std::vector<int> scores(10);
		for (auto& score : scores) {
			ifs.read((char*)&score, sizeof(int));
		}
		return scores;
	}
	inline void regist(int score) {
		std::vector<int> scores = load();
		for (int i = 0; i < scores.size(); i++) {
			if (scores[i] < score) {
				scores.insert(scores.begin() + i, score);
				scores.pop_back();
				break;
			}
		}

		std::ofstream ofs("dat/score.dat", std::ios::binary);
		if (!ofs)
			std::runtime_error("Failed open score.dat");
		for (auto& score : scores) {
			ofs.write((char*)&score, sizeof(int));
		}
	}
}
