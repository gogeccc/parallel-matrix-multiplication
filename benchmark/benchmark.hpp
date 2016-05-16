#pragma once
#include "../defines.hpp"
#include <string>
#include <vector>
#include <map>

//koristi se za konstruisanje algorithms objekta
namespace benchmark
{
	namespace algorithms {
		enum algs_
		{
			arma,
			serial,
			parallel
		};
	}
	namespace test
	{
		enum test_
		{
			no,
			yes
		};
	}
}


////!!!!! ima malo neuredjenosti sa public i private sekcijama 
////!!!!! (sto generalno nije dobra praksa)
class Benchmark
{
private:
	typedef struct mat_sizes
	{
		//od...
		UINT_ min;
		//do...
		UINT_ max;
		//korak
		UINT_ step;
		mat_sizes() {};
		mat_sizes(UINT_ min_, UINT_ max_, UINT_ step_) :
			min(min_), max(max_), step(step_)
		{};
	} matrix_sizes_t;
	matrix_sizes_t matrix_sizes;
public:
	//koje algoritme testirati
	typedef struct alg
	{
		bool arma;
		bool serial;
		bool parallel;
		alg() : arma(false), serial(false), parallel(false) {};
		alg(bool arma_, bool serial_, bool parallel_) :
			arma(arma_), serial(serial_), parallel(parallel_)
		{};
	} algorithms_t;
private:
	algorithms_t algorithms;
	//koliko ciklusa testiranja
	UINT_ n_cycles;
public:
	//rezultati po algoritmu
	typedef struct res
	{
		unsigned long long arma, serial, parallel;
		res() {};
		res(unsigned long long arma_, unsigned long long serial_, unsigned long long parallel_) :
			arma(arma_), serial(serial_), parallel(parallel_)
		{};
	} results_t;
private:
	//rezultati spakovani po testiranim velicinama matrice
	// (redom - ordered map)
	std::map<UINT_, results_t> results_per_size;
	//indikator da parametri za konstrukciju nisi bili ispravni
	bool bad;
	//ako neko pozove run() kada je bad true
	bool last_run_ok;
	//sadrzace informaciju o gresci zbog koje je objekat bad
	std::string err_msg;
	//spreci kreiranje praznih objekata
	Benchmark() {};
public:

	//dobavi nepodesene algoritme
	static algorithms_t get_algorithms_struct();

	//podesi algoritme koje zelis da koristis u benchmarku
	// @params:
	//! algs - algorithms structure
	//! which - possible values: 
	//		benchmark::algorithms::arma, 
	//		benchmark::algorithms::serial, 
	//		benchmark::algorithms::parallel
	//! to_what - test algorithm or not, possible values: 
	//		benchmark::test::yes, 
	//		benchmark::test::no
	static algorithms_t& set_algorithms(algorithms_t& algs,
		benchmark::algorithms::algs_ which, benchmark::test::test_ to_what);

	// vrsim proveru da li su parametri ispravni, 
	// ako nisu samo podesavam polje bad na true
	// !!! to get algorithms: ::get_algorithms_struct
	// !!! to set algorithms: ::set_algorithms_struct
	Benchmark(UINT_ min_mat_size, UINT_ max_mat_size, UINT_ size_step,
		algorithms_t& algorithms__,
		UINT_ n_cycles_);

	//da li je objekat uspesno konstruisan
	bool is_good();

	//pokreni benchmark, vrati true ako si se uspesno pokrenuo
	// (zavisi od polja bad)
	bool run();

	//da li je benchmark uspesno izvrsen
	bool is_executed();

	//vrati string koji sadrzi poruku o gresci
	std::string get_err_msg();

	std::map<UINT_, Benchmark::results_t> get_results_map();
	//vrati true ako je popunjavanje stringa proslo bez problema
	std::string get_results_string();

};

//kako uporediti dva algorithms objekta
// (pri proveri da li je izabran koji algoritam)
static inline bool operator==(const Benchmark::algorithms_t& first,
	const Benchmark::algorithms_t& second);