#include "../defines.hpp"
#include "CLI.hpp"
#include "../matrices/matrices.hpp"
#include "../multiplication/multiplication.hpp"
#include <fstream>

CLI::CLI()
{
	//konstruktor parent klase
	UI();
	//ne bih trebao kreirati bezvezne benchmark objekte.
	// ipak, ovde cu to uraditi.
	// (da bih izbegao proveru da li je benchmark kreiran na kraju programa)
	bench = new Benchmark(1u, 1u, 1u, Benchmark::get_algorithms_struct(), 1u);
	//ista prica kao i za benchmark
	triple = new matrix_triple();
	pair_present = false;
	pair_multiplied = false;
	benchmark_executed = false;
	last_time = 0;
	last_algorithm = std::string();
}

CLI::~CLI()
{
	delete bench;
	delete triple;
}

void CLI::get_input(int & var)
{
	//ucitaj int sa tastature

	std::string tmp_str;
	while (true) {
		try {
			std::getline(std::cin, tmp_str);
			var = atoi(tmp_str.c_str());
			break;
		}

		//ovo se nece nikad desiti zbog toga kako radi atoi -_-
		//ali nema veze
		catch (...) {
			std::cout << "Morate uneti celobrojnu vrednost. Pokusajte ponovo.\n>";
		}
	}
}

void CLI::display_menu()
{
	

	bool out = false;
	int option;
	std::string dummy;
	while (!out) {
		std::string menu_str = LINIJA;
		menu_str += \
			"- trenutni radni direktorijum    : " + working_dir + "\n"
			+ "- par matrica ucitan / generisan : " + std::to_string(pair_present) + "\n"
			+ "- par matrica pomnozen           : " + std::to_string(pair_multiplied) + "\n"
			+ "- benchmark izvrsen              : " + std::to_string(benchmark_executed) + "\n"
			+ LINIJA
			+ "[0] Izadji iz programa\n\
[1] Podesi radni direktorijum\n\
[2] Ucitaj par matrica iz fajla\n\
[3] Generisi par matrica\n\
[4] Prikazi ucitane / generisane matrice\n\
[5] Sacuvaj par matrica u fajl\n\
[6] Pomnozi ucitane / generisane matrice\n\
[7] Prikazi proizvod i vreme mnozenja\n\
[8] Izvrsi benchmark\n\
[9] Sacuvaj rezultate bencmark - a u.txt fajl\n\
[0] Izadji iz programa\n"
+ LINIJA
+ ">> ";
		std::cout << menu_str;
		get_input(option);
		switch (option) {
		case 1:
			set_working_dir_cli();
			break;
		case 2:
			load_matrices();
			break;
		case 3:
			generate_matrices();
			break;
		case 4:
			display_matrices();
			break;
		case 5:
			save_matrices();
			break;
		case 6:
			multiply_matrix_pair();
			break;
		case 7:
			display_product_details();
			break;
		case 8:
			run_benchmark();
			break;
		case 9:
			save_benchmark_results();
			break;
		case 0:
			out = true;
			break;
		default:
			std::cout << "-------------------------------------------------" << std::endl;
			std::cout << "Odaberite jednu od ponudjenih opcija !!!" << std::endl;
			std::getline(std::cin, dummy);
			break;
		}
	}

}

void CLI::set_working_dir_cli()
{
	std::cout << LINIJA;
	std::cout << "Unesi putanju radnog direktorijuma:\n>> ";
	std::getline(std::cin, working_dir);
}

void CLI::load_matrices()
{
	std::string filename;
	std::cout << LINIJA;
	std::cout << "Unesi ime fajla:\n>> ";
	std::getline(std::cin, filename);
	bool success = load_matrix_pair(triple, working_dir, filename);
	if (!success)
	{
		std::cout << LINIJA;
		std::cout << "Neuspesno ucitavanje matrica!!!";
	}
	else
	{
		pair_present = true;
	}
}

void CLI::generate_matrices()
{
	//TODO: unesi zeljene dimenzije
	int rows1 = 0;
	int cols1_rows2 = 0;
	int cols2 = 0;

	std::cout << LINIJA;
	std::cout << "Unesi broj vrsta prve matrice\n>> ";
	get_input(rows1);
	std::cout << "Unesi broj vrsta kolona prve/broj vrsta druge matrice\n>> ";
	get_input(cols1_rows2);
	std::cout << "Unesi broj kolona druge matrice\n>> ";
	get_input(cols2);

	// pozovi fixed dimensions generator
	random_matrix_pair_generator_fixed_all_dimensions(triple, rows1, cols1_rows2, cols2);

	if (!triple->OK)
	{
		std::cout << LINIJA;
		std::cout << "Neuspesno generisanje matrica!!!";
	}
	else
	{
		pair_present = true;
	}
}

void CLI::display_matrices()
{
	if (pair_present)
	{
		std::cout << LINIJA;
		std::cout << "Matrice bi mogle biti velike!\n\
Da li zelite da prikazete matrice ili samo njihove dimenzije? [1/0]\n>> ";
		int choice;
		get_input(choice);
		if (choice == 0)
		{
			std::cout << LINIJA;
			std::cout << "Matrica 1: "
				+ std::to_string(triple->multiplier.n_rows)
				+ "x"
				+ std::to_string(triple->multiplier.n_cols)
				<< std::endl;
			std::cout << "Matrica 2: "
				+ std::to_string(triple->multiplicand.n_rows)
				+ "x"
				+ std::to_string(triple->multiplicand.n_cols)
				<< std::endl;
			get_input(choice);
		}
		else if (choice == 1)
		{
			std::cout << LINIJA;
			std::cout << "Matrica 1: "
				+ std::to_string(triple->multiplier.n_rows)
				+ "x"
				+ std::to_string(triple->multiplier.n_cols)
				<< std::endl;
			triple->multiplier.print();
			get_input(choice);
			std::cout << "Matrica 2: "
				+ std::to_string(triple->multiplicand.n_rows)
				+ "x"
				+ std::to_string(triple->multiplicand.n_cols)
				<< std::endl;
			triple->multiplicand.print();
			get_input(choice);
		}
	}
}

void CLI::save_matrices()
{
	if (pair_present)
	{
		//sacuvaj matricu
		std::string filename;

		std::cout << LINIJA;
		std::cout << "Unesi ime fajla\n>> ";
		std::getline(std::cin, filename);

		bool success = save_matrix_pair(triple, working_dir, filename);

		if (!success)
		{
			std::cout << LINIJA;
			std::cout << "Neuspesno cuvanje fajla!!!" << std::endl;
			int dummy;
			get_input(dummy);
		}

	}
}

void CLI::multiply_matrix_pair()
{
	if (pair_present)
	{
		//pitaj kojim algoritmom
		int choice;

		std::cout << LINIJA;
		std::cout << "[0] Armadillo algoritam" << std::endl;
		std::cout << "[1] Paralelni algoritam" << std::endl;
		std::cout << "[2] Serijski algoritam" << std::endl;
		std::cout << "Unesi broj ispred zeljenog algoritma\n>> ";
		get_input(choice);

		if (choice == 0)
		{
			last_algorithm = "Armadillo";
			multiply_pair(triple, multiply::arma);
			last_time = triple->arma_time;
			last_product = &triple->arma_product;
		}
		else if (choice == 1)
		{
			last_algorithm = "Paralelni";
			multiply_pair(triple, multiply::parallel);
			last_time = triple->parallel_time;
			last_product = &triple->parallel_product;
		}
		else if (choice == 2)
		{
			last_algorithm = "Serijski";
			multiply_pair(triple, multiply::serial);
			last_time = triple->serial_time;
			last_product = &triple->serial_product;
		}

		//proveriti da li je mnozenje bilo ok
		if (triple->last_multiplication_ok)
		{
			pair_multiplied = true;
		}
		else //mnozenje nije izvrseno
		{
			std::cout << LINIJA;
			std::cout << "Neuspesno mnozenje!!!/nVerovatno ste menjali matrice u fajlu." << std::endl;
			int dummy;
			get_input(dummy);
		}

	}
}

void CLI::display_product_details()
{
	if (pair_multiplied)
	{
		//upozorenje, pa prikaz kao kod para
		std::cout << LINIJA;
		std::cout << "Matrica bi mogla biti velika!\n\
Da li zelite da prikazete matricu ili samo njene dimenzije? [1/0]\n>> ";
		int choice;
		get_input(choice);
		if (choice == 0)
		{
			std::cout << LINIJA;
			std::cout << last_algorithm << ", vreme: " << std::to_string(last_time) << " ns" << std::endl;
			std::cout << "Matrica proizvoda: "
				+ std::to_string(last_product->n_rows)
				+ "x"
				+ std::to_string(last_product->n_cols)
				<< std::endl;
			get_input(choice);
		}
		else if (choice == 1)
		{
			std::cout << LINIJA;
			std::cout << last_algorithm << ", vreme: " << std::to_string(last_time) << " ns" << std::endl;
			std::cout << "Matrica proizvoda: "
				+ std::to_string(last_product->n_rows)
				+ "x"
				+ std::to_string(last_product->n_cols)
				<< std::endl;
			last_product->print();
			get_input(choice);
		}
	}
}

void CLI::run_benchmark()
{
	//todo: ucitaj parametre za benchmark, konstruisi objekat
	//moram podmeni kako bih mogao unositi razne algoritme
	int size_min = 0;
	int size_max = 0;
	int size_step = 0;
	Benchmark::algorithms_t algs = Benchmark::get_algorithms_struct();
	int n_cycles = 0;
	std::string filename;

	//ovde ide podmeni
	bool out = false;
	bool executed = false;
	int option;
	std::string dummy;
	while (!out) {
		std::string menu_str = LINIJA;
		menu_str += \
			  "[0] ponisti benchmark " + std::to_string(0) + "\n"
			+ "[1] unesi min dimenzije matrice      : " + std::to_string((UINT_)size_min) + "\n"
			+ "[2] unesi max dimenzije matrice      : " + std::to_string((UINT_)size_max) + "\n"
			+ "[3] unesi korak za dimenzije matrice : " + std::to_string((UINT_)size_step) + "\n"
			+ "[4] unesi broj ciklusa testiranja    : " + std::to_string((UINT_)n_cycles) + "\n"
			+ "[5] (de)selektuj armadillo algoritam : " + std::to_string(algs.arma) + "\n"
			+ "[6] (de)selektuj paralelni algoritam : " + std::to_string(algs.parallel) + "\n"
			+ "[7] (de)selektuj serijski algoritam  : " + std::to_string(algs.serial) + "\n"
			+ "[8] podesi fajl za cuvanje rezultata : " + filename + "\n"
			+ "[9] pokreni benchmark            \n"
			+ "[0] ponisti benchmark " + std::to_string(0) + "\n"
			+ LINIJA
			+ ">> ";
		std::cout << menu_str;
		get_input(option);
		switch (option) {
		case 1:
			std::cout << LINIJA;
			std::cout << "Unesi minimalne dimenzije matrice\n>> ";
			get_input(size_min);
			break;
		case 2:
			std::cout << LINIJA;
			std::cout << "Unesi maksimalne dimenzije matrice\n>> ";
			get_input(size_max);
			break;
		case 3:
			std::cout << LINIJA;
			std::cout << "Unesi korak za dim. matrice\n>> ";
			get_input(size_step);
			break;
		case 4:
			std::cout << LINIJA;
			std::cout << "Unesi broj ciklusa testiranja\n>> ";
			get_input(n_cycles);
			break;
		case 5:
			//arma
			if (algs.arma == benchmark::test::yes)
			{
				algs.arma = benchmark::test::no;
			}
			else if (algs.arma == benchmark::test::no)
			{
				algs.arma = benchmark::test::yes;
			}
			break;
		case 6:
			//parallel
			if (algs.parallel == benchmark::test::yes)
			{
				algs.parallel = benchmark::test::no;
			}
			else if (algs.parallel == benchmark::test::no)
			{
				algs.parallel = benchmark::test::yes;
			}
			break;
		case 7:
			//serial
			if (algs.serial == benchmark::test::yes)
			{
				algs.serial = benchmark::test::no;
			}
			else if (algs.serial == benchmark::test::no)
			{
				algs.serial = benchmark::test::yes;
			}
			break;
		case 8:
			std::cout << LINIJA;
			std::cout << "Unesi naziv fajla za cuvanje rezultata\n>> ";
			std::getline(std::cin, filename);
			break;
		case 9:
			//kreiraj benchmark
			delete bench;
			bench = new Benchmark(size_min, size_max, size_step, algs, n_cycles);
			//provera da li je bad
			if (bench->is_good())
			{
				//	cout << izvrsava se...
				std::cout << "Benchmark se izvrsava, moze potrajati.\nMolimo sacekajte..." << std::endl;
				//	izvrsi
				bench->run();
				//	sacuvaj rezultate
				if (filename != "")
				{
					bool success = true;
					////////////////
					//success = bench->save_results(working_dir, filename);
					////////////////
					if (!success)
					{
						std::cout << LINIJA;
						std::cout << "Neuspesno cuvanje rezultata u fajl!!!\nMozete ih sacuvati iz glavnog menija." << std::endl;
						int dummy;
						get_input(dummy);
					}
				}
				benchmark_executed = bench->is_executed();
				out = bench->is_executed();
			}
			//else
			else //nisu ok parametri
			{
				//	cout << err_msg
				std::cout << LINIJA;
				std::cout << bench->get_err_msg() << std::endl;
				std::getline(std::cin, dummy);
			}
			break;
		case 0:
			out = true;
			break;
		default:
			std::cout << "-------------------------------------------------" << std::endl;
			std::cout << "Odaberite jednu od ponudjenih opcija !!!" << std::endl;
			std::getline(std::cin, dummy);
			break;
		}
	}
}

void CLI::save_benchmark_results()
{
	if (benchmark_executed)
	{
		// uradi fju u Benchmark klasi sa hendlovanjem error-a (is_open() etc.)
	}
}
