#ifndef __CHESS_RANDOM_H__
#define __CHESS_RANDOM_H__

#include <random> // random_device, mt19937, uniform_int_distribution
#include <chrono>

class Random {
public:
	Random();
	int getRandomInt(int min, int max);
private:
	std::mt19937 engine;
};

#endif
