#include "UI.hpp"
#include "matrices.hpp"
#include "multiplication.hpp"
#include "defines.hpp"
#include <string>

std::string UI::set_working_dir(std::string dir)
{
	working_dir = dir;
	return working_dir;
}

std::string UI::get_working_dir()
{
	return working_dir;
}

// ------------------- Benchmark klasa -----------------

Benchmark::algorithms_t Benchmark::get_algorithms_struct()
{
	return algorithms_t();
}

Benchmark::algorithms_t& Benchmark::set_algorithms(algorithms_t& algs, benchmark::algorithms::algs_ which, benchmark::test::test_ to_what)
{
	if (which == benchmark::algorithms::arma)
	{
		algs.arma = (bool)to_what;
	}
	else if (which == benchmark::algorithms::serial)
	{
		algs.serial = (bool)to_what;
	}
	else if (which == benchmark::algorithms::parallel)
	{
		algs.parallel = (bool)to_what;
	}

	return algs;
}

bool operator==(const Benchmark::algorithms_t& first, const Benchmark::algorithms_t& second)
{
	return !(first.arma != second.arma 
		|| first.serial != second.serial
		|| first.parallel != second.parallel);
}

Benchmark::Benchmark(UINT_ min_mat_size, UINT_ max_mat_size, UINT_ size_step, algorithms_t& algorithms__, UINT_ n_cycles_)
{
	err_msg = "";
	//proveriti velicine matrica
	if (min_mat_size == 0)
	{
		err_msg += "BENCHMARK ERROR:\nmin_mat_size = 0\n";
	}
	if (max_mat_size == 0)
	{
		err_msg += "BENCHMARK ERROR:\nmax_mat_size = 0\n";
	}
	if (size_step == 0)
	{
		err_msg += "BENCHMARK ERROR:\nsize_step = 0\n";
	}
	if (min_mat_size > max_mat_size)
	{
		err_msg += "BENCHMARK ERROR:\nmin_mat_size > max_mat_size\n";
	}
	// proveriti da li je broj ciklusa ok
	if (n_cycles_ == 0)
	{
		err_msg += "BENCHMARK ERROR:\nn_cycles_ = 0\n";
	}
	//  proveriti da li je izabran koji algoritam
	if (algorithms__ == algorithms_t())
	{
		err_msg += "BENCHMARK ERROR:\nnije izabran nijedan algoritam\n";
	}

	if (err_msg == "") //nema gresaka, konstruisi objekat
	{
		err_msg = "NO BENCHMARK ERRORS";
		matrix_sizes = matrix_sizes_t(min_mat_size, max_mat_size, size_step);
		n_cycles = n_cycles_;
		algorithms = algorithms__;
		bad = false;
	}
	else //imamo neke greske
	{
		bad = true;
	}

	last_run_ok = false;
}

bool Benchmark::is_good()
{
	return !bad;
}

bool Benchmark::run()
{
	if (!bad)
	{
		//TODO: run benchmark
		////////testiram lambda funkcije
		//auto proba = [](Benchmark& bench) {
		//	//jedna iteracija benchmarka
		//};
		//broj izvrsenih iteracija ciklusa
		UINT_ n_iter;
		//trenutna velicina matrice
		UINT_ current_matrix_size = matrix_sizes.min;
		//struktura koja sadrzi matrice, vremena itd
		matrix_triple mat_triple;
		//siguran sam da sva mnozenja zajedno nece trajati vise od 45 godina
		// (vreme od unix epohe u nanosekundama staje u unsigned long long)
		unsigned long long arma_time, serial_time, parallel_time;
		//struktura sa rezultatima
		results_t reslts;
		// iterirati kroz velicine matrica
		while (current_matrix_size <= matrix_sizes.max) 
		{
			//popuni matrice slucajnim brojevima
			random_matrix_pair_generator_fixed_all_dimensions(&mat_triple,
				current_matrix_size, current_matrix_size, current_matrix_size);
			//resetuj ukupna vremena,
			// spremi ih za testiranje sa novom velicinom matrice
			arma_time = 0U,
			serial_time = 0U, 
			parallel_time = 0U;
			//resetuj broj dosada izvrsenih ciklusa
			n_iter = 0U;
			// for broj ciklusa
			for (UINT_ i = 0; i < n_cycles; ++i)
			{
				// poterati odgovarajuca mnozenja
				// i dodati vreme mnozenja na ukupno vreme
				if (algorithms.arma)
				{
					multiply_pair(&mat_triple, multiply::arma);
					arma_time += mat_triple.arma_time;
				}
				if (algorithms.parallel)
				{
					multiply_pair(&mat_triple, multiply::parallel);
					parallel_time += mat_triple.parallel_time;
				}
				if (algorithms.serial)
				{
					multiply_pair(&mat_triple, multiply::serial);
					serial_time += mat_triple.serial_time;
				}
				// povecati broj proteklih iteracija
				++n_iter;
			}
			//izracunaj dosadasnje vreme i dodaj novi par u mapu rezultata
			if (algorithms.arma)
			{
				arma_time = arma_time / n_iter;
			}
			if (algorithms.parallel)
			{
				parallel_time = parallel_time / n_iter;
			}
			if (algorithms.serial)
			{
				serial_time = serial_time / n_iter;
			}
			reslts = results_t(arma_time, serial_time, parallel_time);
			// napravi i dodaj par vrednosti u mapu
			results_per_size.insert(std::pair<UINT_, results_t>(current_matrix_size, reslts));
			//povecati curr mat size za step
			//malo @sentinelisanja@ nije na odmet xD
			// (ako bi dodavanjem koraka prekoracio maksimalnu vrednost,
			//  podesi velicinu na maksimalnu vrednost)
			if (current_matrix_size != matrix_sizes.max &&
				current_matrix_size + matrix_sizes.step > matrix_sizes.max)
			{
				current_matrix_size = matrix_sizes.max;
			}
			else
			{
				current_matrix_size += matrix_sizes.step;
			}

			last_run_ok = true;
		}

	}
	else //ne moze se izvrsiti benchmark
	{
		last_run_ok = false;
	}

	return last_run_ok;
}

bool Benchmark::is_executed()
{
	return last_run_ok;
}

std::string Benchmark::get_err_msg()
{
	return err_msg;
}

std::map<UINT_, Benchmark::results_t> Benchmark::get_results_map()
{
	return results_per_size;
}

std::string Benchmark::get_results_string()
{
	std::string output;

	if (last_run_ok)
	{
		//TODO: popuni string rezultatima
		output = "";
		//#define na brzaka, sto bi se reklo xD
#define LINIJA "----------------------------------------------\n"
		output += LINIJA;
		std::map<UINT_, Benchmark::results_t>::iterator pair_ = results_per_size.begin();
		//prolazimo kroz rezultat za svaku velicinu i dodajemo podatke u string
		for (pair_; pair_ != results_per_size.end(); ++pair_)
		{
			output += LINIJA;
			output += "Velicina matrice: " + std::to_string(pair_->first)
				+ " x " + std::to_string(pair_->first) + "; " + std::to_string(n_cycles)
				+ " ciklusa" + "\n"
				+ LINIJA;
			if (algorithms.arma)
			{
				output += "Armadillo: " + std::to_string(pair_->second.arma / 1000)
					+ " ms\n";
			}
			if (algorithms.serial)
			{
				output += "Serijski: " + std::to_string(pair_->second.serial / 1000)
					+ " ms\n";
			}
			if (algorithms.parallel)
			{
				output += "Paralelni: " + std::to_string(pair_->second.parallel / 1000)
					+ " ms\n";
			}
			output += "\n";
		}
	}
	else //nemamo rezultate
	{
		//neka korisnik zna u cemu je problem
		output = err_msg;
	}

	return output;
}


