#pragma once
#include <random>

class Random {
public:
	Random() {
		std::random_device rd;
		setSeed(rd());
	}
	void setSeed(int seed) {
		mt.seed(seed);
	}
	int next(int n) {
		return next(0, n);
	}
	int next(int a, int b) {
		std::uniform_int_distribution<int> dice(a, b);
		return dice(mt);
	}
	float next(float n) {
		return next(0.0f, n);
	}
	float next(float a, float b) {
		std::uniform_real_distribution<float> score(a, b);
		return score(mt);
	}
	int nextPlusMinus(int n) {
		return next(-n, n);
	}
	float nextPlusMinus(float n) {
		return next(-n, n);
	}
	bool trueOrFalse() { return next(1) == 0 ? true : false; }
private:
	std::mt19937 mt;
};
