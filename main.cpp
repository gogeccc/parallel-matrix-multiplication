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

	//matrix_triple* foo = new matrix_triple;
	//random_matrix_pair_generator_fixed_all_dimensions(foo, 50, 50, 50);
	//std::cout << "mnozenje...\n";
	//multiply_pair(foo, multiply::serial);
	//std::cout << "serijsko> ";
	//std::cout << foo->serial_time << std::endl;

	return NULL;
}


int main()
{
	main_tests();
}
