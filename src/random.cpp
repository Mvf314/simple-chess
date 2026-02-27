#include "random.h"

Random::Random() {
	// Seed with time.
	unsigned long seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	engine = std::mt19937(seed);
}

int Random::getRandomInt(int min, int max) {
	std::uniform_int_distribution<> dist(min, max);
	return dist(engine);
}
