#pragma once
#include "../defines.hpp"
#include "UI.hpp"

class CLI : public UI
{
private:
	Benchmark* bench;
	bool pair_present;
	bool pair_multiplied;
	bool benchmark_executed;


public:
	CLI();
	~CLI();

	void get_input(int& var);
	void display_menu();

	void set_working_dir_cli();
	void load_matrix_pair();
	void generate_matrix_pair();
	void display_matrix_pair();
	void save_matrix_pair();
	void multiply_matrix_pair();
	void display_product_details();
	void run_benchmark();
	void save_benchmark_results();
};
