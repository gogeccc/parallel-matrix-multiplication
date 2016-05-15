#include "matrices.hpp"
#include "random.hpp"
#include "../defines.hpp"
#include <cstdlib>
#include <thread>
#include <string>
#include <ctime>
#include <chrono>
#include <fstream>
#include <sstream>
#include <stdexcept>

//koristi se za kreiranje jedinstvenih imena trojkama matrica
UINT_ matrix_counter = 0;

std::string* generate_triple_name(std::string* string_)
{
	//broj nanosekundi od pocetka unix epohe
	long long ns_epoch = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	//radi sigurnosti dodaj i vrednost brojaca u ime
	*string_ = "pair_" + std::to_string(matrix_counter) + "_" + std::to_string(ns_epoch);

	++matrix_counter;

	return string_;
}

void random_matrix_filler(matrix* matrix_)
{
	//iteratori iteriraju po kolonama
	matrix::iterator mat_iter = matrix_->begin();
	matrix::iterator mat_end = matrix_->end();
	for (mat_iter; mat_iter != mat_end; ++mat_iter)
	{
		//RANGE i PIVOT se definisu u defines.h
		*mat_iter = random_in_range_around_pivot(RANGE, PIVOT);
	}

	return;
}

matrix_triple* random_matrix_pair_generator_random_dimensions(matrix_triple* matrix_triple_,
	UINT_ size_range, UINT_ size_pivot)
{
	//dati jedinstveno ime paru
	generate_triple_name(&(matrix_triple_->triple_name));

	//ako korisnik zeli matricu dimenzija 0x0 
	if (size_pivot == size_range && size_pivot == 0) //optimizovan raspored provera xD
	{
		//nema potrebe generisati bilo sta
		return matrix_triple_;
	}

	int first_rows = 0, first_columns = 0;
	int second_rows = 0, second_columns = 0;
	// velicina ne sme biti 0
	while (first_rows == 0)
	{
		// moram abs() radi sigurnosti - npr range = 2, pivot = 1
		first_rows = abs(random_in_range_around_pivot(size_range, size_pivot));
	}
	while (first_columns == 0)
	{
		first_columns = abs(random_in_range_around_pivot(size_range, size_pivot));
	}
	//moraju se poklapati "unutrasnje" dimenzije mnozilaca
	second_rows = first_columns;
	while (second_columns == 0)
	{
		second_columns = abs(random_in_range_around_pivot(size_range, size_pivot));
	}
	//konstruisi matrice sa odgovarajucim velicinama
	matrix_triple_->multiplier = matrix(first_rows, first_columns);
	matrix_triple_->multiplicand = matrix(second_rows, second_columns);
	//sad popuni matrice
	////////////////
	//problem sa threadovima - ponavljanje citavih kolona i vrsta
	// PROBLEM RESEN KORISCENJEM random_device GENERATORA IZ <random>
	////////////////
	//po jedan thread za obe matrice
	std::thread first_thread(random_matrix_filler, &(matrix_triple_->multiplier));
	std::thread second_thread(random_matrix_filler, &(matrix_triple_->multiplicand));
	first_thread.join();
	second_thread.join();


	//generisani par matrica je sigurno moguce pomnoziti,
	// par je OK
	matrix_triple_->OK = true;
	//generisani par matrica nije mnozen
	matrix_triple_->last_multiplication_ok = false;

	return matrix_triple_;
}

matrix_triple* random_matrix_pair_generator_fixed_product_dimensions(matrix_triple* matrix_triple_,
	UINT_ rows, UINT_ columns)
{

	//dati jedinstveno ime paru
	generate_triple_name(&(matrix_triple_->triple_name));

	//ako korisnik zeli matricu nemogucih dimenzija
	if (rows <= 0 || columns <= 0) //optimizovan raspored provera xD
	{
		//necemo generisati nista
		return matrix_triple_;
	}

	int first_rows = rows, first_columns = 0;
	int second_rows = 0, second_columns = columns;
	// velicina ne sme biti 0
	while (first_columns == 0)
	{
		first_columns = abs(random_in_range_around_pivot(RANGE, PIVOT));
	}
	//moraju se poklapati "unutrasnje" dimenzije mnozilaca
	second_rows = first_columns;

	//konstruisi matrice sa odgovarajucim velicinama
	matrix_triple_->multiplier = matrix(first_rows, first_columns);
	matrix_triple_->multiplicand = matrix(second_rows, second_columns);
	//sad popuni matrice
	////////////////
	//problem sa threadovima - ponavljanje citavih kolona i vrsta
	// PROBLEM RESEN KORISCENJEM random_device GENERATORA IZ <random>
	////////////////
	//po jedan thread za obe matrice
	std::thread first_thread(random_matrix_filler, &(matrix_triple_->multiplier));
	std::thread second_thread(random_matrix_filler, &(matrix_triple_->multiplicand));
	first_thread.join();
	second_thread.join();


	//generisani par matrica je sigurno moguce pomnoziti,
	// par je OK
	matrix_triple_->OK = true;
	//generisani par matrica nije mnozen
	matrix_triple_->last_multiplication_ok = false;

	return matrix_triple_;
}

matrix_triple* random_matrix_pair_generator_fixed_all_dimensions(matrix_triple* matrix_triple_,
	UINT_ first_rows_, UINT_ first_cols__sec_rows, UINT_ second_columns_)
{

	//dati jedinstveno ime paru
	generate_triple_name(&(matrix_triple_->triple_name));

	//ako korisnik zeli matricu nemogucih dimenzija
	if (first_rows_ <= 0 || first_cols__sec_rows <= 0 || second_columns_ <= 0) //optimizovan raspored provera xD
	{
		//necemo generisati nista
		return matrix_triple_;
	}

	int first_rows = first_rows_, first_columns = first_cols__sec_rows;
	int second_rows = first_cols__sec_rows, second_columns = second_columns_;

	//konstruisi matrice sa odgovarajucim velicinama
	matrix_triple_->multiplier = matrix(first_rows, first_columns);
	matrix_triple_->multiplicand = matrix(second_rows, second_columns);
	//sad popuni matrice
	////////////////
	//problem sa threadovima - ponavljanje citavih kolona i vrsta
	// PROBLEM RESEN KORISCENJEM random_device GENERATORA IZ <random>
	////////////////
	//po jedan thread za obe matrice
	std::thread first_thread(random_matrix_filler, &(matrix_triple_->multiplier));
	std::thread second_thread(random_matrix_filler, &(matrix_triple_->multiplicand));
	first_thread.join();
	second_thread.join();


	//generisani par matrica je sigurno moguce pomnoziti,
	// par je OK
	matrix_triple_->OK = true;
	//generisani par matrica nije mnozen
	matrix_triple_->last_multiplication_ok = false;

	return matrix_triple_;
}

bool save_matrix_pair(matrix_triple* matrix_triple_, std::string folder_path, std::string file_name)
{
	std::string file_path = folder_path + file_name;
	std::ofstream output_file(file_path, std::ios_base::binary | std::ios_base::out);
	bool success = output_file.is_open();
	if (success)
	{
		success = matrix_triple_->multiplier.save(output_file, arma::arma_ascii)
			   && matrix_triple_->multiplicand.save(output_file, arma::arma_ascii);
		output_file.close();
	}

	return success;
}

bool load_matrix_pair(matrix_triple* matrix_triple_, std::string folder_path, std::string file_name)
{
	std::string file_path = folder_path + file_name;
	std::ifstream input_file(file_path, std::ios_base::binary | std::ios_base::in);
	bool success = input_file.is_open();
	if (success)
	{
		matrix_triple_->triple_name = file_name;
		//sad ucitaj matrice iz streama
		success = matrix_triple_->multiplier.load(input_file, arma::arma_ascii)
			   && matrix_triple_->multiplicand.load(input_file, arma::arma_ascii);
		matrix_triple_->OK = success;
		input_file.close();
	}

	return success;
}