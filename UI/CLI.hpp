#pragma once
#include "../defines.hpp"
#include "UI.hpp"

class CLI : public UI
{
private:
	Benchmark* bench;
	matrix_triple* triple;
	bool pair_present;
	bool pair_multiplied;
	bool benchmark_executed;
	unsigned long long last_time;
	std::string last_algorithm;
	matrix* last_product;

public:
	CLI();
	~CLI();

	void get_input(int& var);
	void display_menu();

	void set_working_dir_cli();
	void load_matrices();
	void generate_matrices();
	void display_matrices();
	void save_matrices();
	void multiply_matrix_pair();
	void display_product_details();
	void run_benchmark();
	void save_benchmark_results();
};
