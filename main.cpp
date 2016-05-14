#include "matrices.hpp"
#include "multiplication.hpp"
#include "parallel_multiplication.hpp"
#include "UI.hpp"
#include "benchmark.hpp"
#include "defines.hpp"
#include <iostream>
#include <fstream>
#include <thread>
#include <string>
#include "tbb/task_scheduler_init.h"

int main_tests() 
{
	//pripremamo parametre
	Benchmark::algorithms_t algs = Benchmark::get_algorithms_struct();
	Benchmark::set_algorithms(algs, benchmark::algorithms::arma, benchmark::test::no);
	Benchmark::set_algorithms(algs, benchmark::algorithms::serial, benchmark::test::yes);
	Benchmark::set_algorithms(algs, benchmark::algorithms::parallel, benchmark::test::no);
	//konstruisemo klasu
	Benchmark bench = Benchmark(500u, 500u, 1u, algs, 1u);
	//demonstracija koriscenja metoda klase
	if (bench.is_good())
	{
		bench.run();
	}
	else
	{
		std::cout << bench.get_err_msg() << std::endl;
	}

	if (bench.is_executed())
	{
		std::cout << bench.get_results_string() << std::endl;
	}
	else
	{
		std::cout << bench.get_err_msg() << std::endl;
	}

	return NULL;
}


int main()
{
	main_tests();
}
