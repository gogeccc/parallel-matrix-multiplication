#include "random.hpp"
#include "defines.hpp"
#include <random>
#include <vector>

int random_in_range_around_pivot(UINT_ range_, int pivot) 
{
	if (range_ == 0) {
		return pivot;
	}

	//std::default_random_engine generator;
	std::random_device generator;
	std::uniform_int_distribution<int> distribution(pivot - (int)range_ + 1, pivot + (int)range_ - 1);

	return distribution(generator);
}
