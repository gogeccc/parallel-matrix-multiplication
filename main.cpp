#include "matrices/matrices.hpp"
#include "multiplication/multiplication.hpp"
#include "multiplication/parallel_multiplication.hpp"
#include "UI/UI.hpp"
#include "UI/CLI.hpp"
#include "benchmark/benchmark.hpp"
#include "defines.hpp"
#include <iostream>
#include <fstream>
#include <thread>
#include <string>
#include "tbb/task_scheduler_init.h"

int main_tests() 
{
	CLI().display_menu();

	return NULL;
}


int main()
{
	main_tests();
}
