#include "parallel_multiplication.hpp"
#include "defines.hpp"
#include "tbb/task_scheduler_init.h"
#include "tbb/task.h"
#include "tbb/scalable_allocator.h"
#include "tbb/tbbmalloc_proxy.h"
#include <thread>
#include <cmath>

//paralelna implementacija, 
// magija sa rekurzivnim divide and conquer TBB taskovima

class Mat_multiplication_task : public tbb::task
{
private:
	matrix_triple* matrix_triple_;
	UINT_ start_row, end_row;
	UINT_ start_col, end_col;
	Mat_multiplication_task *child_1, *child_2, *child_3, *child_4;
public:
	//konstruktor za instanciranje root taska
	Mat_multiplication_task(matrix_triple* mat_trip_) : 
		matrix_triple_(mat_trip_),
		start_row(0), end_row(mat_trip_->parallel_product.n_rows),
		start_col(0), end_col(mat_trip_->parallel_product.n_cols),
		child_1(NULL),
		child_2(NULL),
		child_3(NULL),
		child_4(NULL)
	{};
	//konstruktor za instanciranje child taskova
	Mat_multiplication_task(matrix_triple* mat_trip_, 
		UINT_ s_row, UINT_ e_row,
		UINT_ s_col, UINT_ e_col) :
		matrix_triple_(mat_trip_),
		start_row(s_row), end_row(e_row),
		start_col(s_col), end_col(e_col),
		child_1(NULL),
		child_2(NULL),
		child_3(NULL),
		child_4(NULL)
	{};

	task* execute()
	{
		//idemo dublje u rekurzivno stablo ako
		// mozemo da delimo matricu dalje
		//   (npr: ukoliko imamo 1x1 matricu,
		//     ne bi bilo logicki ispravno uopste se spustati niz stablo).
		if ((end_col - start_col) > 1 || (end_row - start_row) > 1)
		{
			//spawn child taskove

			//----------- delimo trenutni "kvadrat" ---------------------

			//indeksi pocetnih i krajnjih vrsta koji ce biti proslednjeni child taskovima
			UINT_ s_row_1, s_row_2, s_row_3, s_row_4, e_row_1, e_row_2, e_row_3, e_row_4;
			//indeksi pocetnih i krajnjih kolona koji ce biti proslednjeni child taskovima
			UINT_ s_col_1, s_col_2, s_col_3, s_col_4, e_col_1, e_col_2, e_col_3, e_col_4;
		
			//procenjena sredina trenutnog "kvadrata"
			UINT_ center_row = (end_row - start_row) / 2 + start_row;
			UINT_ center_col = (end_col - start_col) / 2 + start_col;

			//gornja leva cetvrtina
			s_row_1 = start_row;
			e_row_1 = center_row;
			s_col_1 = start_col;
			e_col_1 = center_col;
			//gornja desna cetvrtina
			s_row_2 = start_row;
			e_row_2 = center_row;
			s_col_2 = center_col;
			e_col_2 = end_col;
			//donja leva cetvrtina
			s_row_3 = center_row;
			e_row_3 = end_row;
			s_col_3 = start_col;
			e_col_3 = center_col;
			//donja desna cetvrtina
			s_row_4 = center_row;
			e_row_4 = end_row;
			s_col_4 = center_col;
			e_col_4 = end_col;


			//----------------konstruisemo child taskove---------------------


			child_1 = new(allocate_child()) 
				Mat_multiplication_task(matrix_triple_,
				s_row_1, e_row_1, s_col_1, e_col_1);
			child_2 = new(allocate_child()) 
				Mat_multiplication_task(matrix_triple_,
				s_row_2, e_row_2, s_col_2, e_col_2);
			child_3 = new(allocate_child()) 
				Mat_multiplication_task(matrix_triple_,
				s_row_3, e_row_3, s_col_3, e_col_3);
			child_4 = new(allocate_child()) 
				Mat_multiplication_task(matrix_triple_,
				s_row_4, e_row_4, s_col_4, e_col_4);

			//------------------ pokrecemo child taskove ----------------------

			//cekaj 4 child taska + 
			// ne dozvoli instant sponovanje na threadu podslednjeg izvrsenog childa
			set_ref_count(5);
			//pokreni child taskove
			spawn(*child_1);
			spawn(*child_2);
			spawn(*child_3);
			spawn_and_wait_for_all(*child_4);

			//sad da oslobodimo memoriju
			//// AUTOMATSKI SE POZIVA tbb::task::destroy(task) kad se izvrsi execute() metoda
			//delete child_1, child_2, child_3, child_4;
			//tbb::task::destroy(*child_1);
			//tbb::task::destroy(*child_2);
			//tbb::task::destroy(*child_3);
			//tbb::task::destroy(*child_4);

			//POSAO JE GOTOV

		}
		else //matrica se ne moze deliti dalje, 
			//racunajmo elemente nama prosledjene podmatrice
		{
			//lokalne promenljive
			// (pojednostavljuje kod)
			matrix& multiplier = matrix_triple_->multiplier;
			matrix& multiplicand = matrix_triple_->multiplicand;
			matrix& product = matrix_triple_->parallel_product;

			UINT_ n_addends = multiplier.n_cols; //multiplicand.n_rows;

			//tri fora kao kod serijskog mnozenja,
			// jedina razlika su pocetni i krajni indeksi
			//  zavisni od pozicije u rekurzivnom stablu
			//   te su ograniceni na odredjene manje delove rezultatne matrice
			//
			//iteriraj po vrstama
			for (UINT_ i = start_row; i < end_row; ++i)
			{
				//iteriraj po pojedinacnim elementima trenutne vrste
				for (UINT_ j = start_col; j < end_col; ++j)
				{
					//inicijalizuj trenutni element
					product(i, j) = 0;
					//racunaj trenutni element racunanjem i dodavanjem sabiraka
					//////////////
					// ukoliko bi bila neophodna veca paralelizacija, 
					// ovde bi mogao parallel for
					//////////////
					for (UINT_ k = 0; k < n_addends; ++k)
					{
						product(i, j) += multiplier(i, k) * multiplicand(k, j);
					}
				}
			}
		}

		return NULL;
	}
};

matrix_triple* parallel_multiplication_tbb(matrix_triple* matrix_triple_)
{
	//konstruisanje matrice proizvoda
	// (ne moze se odraditi paralelno).
	//	inicijalizacija elemenata na vrednost 0
	//	ce biti uradjena paralelno.)
	matrix_triple_->parallel_product = matrix(matrix_triple_->multiplier.n_rows, matrix_triple_->multiplicand.n_cols);

	//spawnovati rekurzivni task

	//konstruisanje root taska
	Mat_multiplication_task* root_task_ = new(tbb::task::allocate_root())
		Mat_multiplication_task(matrix_triple_);
	//spawnovanje root taska
	tbb::task::spawn_root_and_wait(*root_task_);

	//oslobadjanje memorije
	//// AUTOMATSKI SE POZIVA tbb::task::destroy(task) kad se izvrsi execute() metoda
	//delete root_task_;
	//tbb::task::destroy(*root_task_);

	return matrix_triple_;
}