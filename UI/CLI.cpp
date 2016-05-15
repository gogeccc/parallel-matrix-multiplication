#include "../defines.hpp"
#include "CLI.hpp"

CLI::CLI()
{
	//konstruktor parent klase
	UI();
	//ne bih trebao kreirati bezvezne benchmark objekte.
	// ipak, ovde cu to uraditi.
	// (da bih izbegao proveru da li je benchmark kreiran na kraju programa)
	bench = new Benchmark(1u, 1u, 1u, Benchmark::get_algorithms_struct(), 1u);
	pair_present = false;
	pair_multiplied = false;
	benchmark_executed = false;
}

CLI::~CLI()
{
	delete bench;
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

	bool out = false;
	int option;
	std::string dummy;
	while (!out) {
		std::cout << menu_str;
		get_input(option);
		switch (option) {
		case 1:
			set_working_dir_cli();
			break;
		case 2:
			load_matrix_pair();
			break;
		case 3:
			generate_matrix_pair();
			break;
		case 4:
			display_matrix_pair();
			break;
		case 5:
			save_matrix_pair();
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
}

void CLI::load_matrix_pair()
{
}

void CLI::generate_matrix_pair()
{
}

void CLI::display_matrix_pair()
{
}

void CLI::save_matrix_pair()
{
}

void CLI::multiply_matrix_pair()
{
}

void CLI::display_product_details()
{
}

void CLI::run_benchmark()
{
}

void CLI::save_benchmark_results()
{
}
