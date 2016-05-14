#pragma once
#include "defines.hpp"
#include "benchmark.hpp"
#include <string>
#include <vector>
#include <map>

class UI
{
protected:
	std::string working_dir;
	//Benchmark* latest_benchmark;
	//ovo polje ce se proveravati pre kreiranja novog benchmarka
	// (da li korisnik zeli da sacuva rezultate ili ne)
	//bool benchmark_run;

public:
	UI() : working_dir(DIR_NOT_SET) {};
	std::string set_working_dir(std::string dir);
	std::string get_working_dir();
};

