#include "parallel_multiplication.hpp"
#include "defines.hpp"
#include "tbb/tick_count.h"


//vrsi serijsko mnozenje matrica
matrix_triple* serial_multiplication(matrix_triple* matrix_triple_)
{

	//nove reference radi jednostavnijeg kodiranja
	matrix& product = matrix_triple_->serial_product;
	matrix& multiplier = matrix_triple_->multiplier;
	matrix& multiplicand = matrix_triple_->multiplicand;

	//napravi matricu proizvoda odgovarajucih dimenzija
	product = matrix(multiplier.n_rows, multiplicand.n_cols);

	//broj sabiraka koji odredjuje broj iteracija u unutrasnjoj petlji
	UINT_ n_addends = matrix_triple_->multiplier.n_cols; //matrix_triple_->multiplicand.n_rows
	//iteriramo po vrstama matrice proizvoda
	for (UINT_ i = 0; i < product.n_rows; ++i)
	{
		//iteriramo po pojedinacnim elementima trenutne vrste
		for (UINT_ j = 0; j < product.n_cols; ++j)
		{
			//inicijalizacija trenutnog elementa
			product(i, j) = 0;
			//iteriramo po sabircima
			for (UINT_ k = 0; k < n_addends; ++k)
			{
				//trenutni sabirak se dobija mnozenjem...
				// (vidi se u kodu) - prvi indeks je vrsta, drugi kolona
				product(i, j) += multiplier(i, k) * multiplicand(k, j);
			}
		}
	}

	return matrix_triple_;
}

//pozovi fukciju koja vrsi paralelno mnozenje matrica
matrix_triple* parallel_multiplication(matrix_triple* matrix_triple_)
{
	parallel_multiplication_tbb(matrix_triple_);
	return matrix_triple_;
}

//funkcija wrapper za sve tipove mnozenja
bool multiply_pair(matrix_triple* matrix_triple_, const UINT_ algorithm = multiply::serial)
{
	//prvo posao koji se uvek obavlja:
	// proveri da li je par matrica ispravno ucitan/generisan
	// i da li su dimenzije matrica odgovarajuce
	bool pair_ok = matrix_triple_->OK
				&& matrix_triple_->multiplier.n_cols == matrix_triple_->multiplicand.n_rows;
	//ako matrice nisu odgovarajucih dimenzija,
	// triplet se ne moze mnoziti - nije OK
	matrix_triple_->OK = pair_ok;

	if (pair_ok)
	{
		//za merenje vremena izvrsenja
		tbb::tick_count tick_count_;
		tbb::tick_count::interval_t tick_interval_;

		//sad odaberi algoritam
		if (algorithm == multiply::parallel)
		{
			//meri vreme
			tick_count_ = tbb::tick_count::now();

			//pomnozi paralelnim algoritmom
			parallel_multiplication(matrix_triple_);

			//izracunaj protekli interval vremena
			tick_interval_ = tbb::tick_count::now() - tick_count_;
			//zabelezi vreme u nanosekundama
			matrix_triple_->parallel_time = tick_interval_.seconds() * 1000000;
		}
		else if (algorithm == multiply::serial)
		{
			//meri vreme
			tick_count_ = tbb::tick_count::now();

			//mnozi serijskim algoritmom
			serial_multiplication(matrix_triple_);

			//interval vremena
			tick_interval_ = tbb::tick_count::now() - tick_count_;
			//nanosekunde
			matrix_triple_->serial_time = tick_interval_.seconds() * 1000000;
		}
		else if (algorithm == multiply::arma)
		{
			//meri vreme
			tick_count_ = tbb::tick_count::now();

			//pomnozi operatorom * iz Armadillo biblioteke
			matrix_triple_->arma_product = matrix_triple_->multiplier
										 * matrix_triple_->multiplicand;

			//izracunaj protekli interval vremena
			tick_interval_ = tbb::tick_count::now() - tick_count_;
			//zabelezi vreme u nanosekundama
			matrix_triple_->arma_time = tick_interval_.seconds() * 1000000;
		}
		//mnozenje nije imalo razloga da ne uspe
		// (izuzeci ce biti propagirani)
		matrix_triple_->last_multiplication_ok = true;
	}
	else //ako mnozenje ne moze biti izvrseno
	{
		// neka oni koji pristupaju tripletu matrica znaju da mnozenje nije uspelo.
		matrix_triple_->last_multiplication_ok = false;
	}
	
	return pair_ok;
}