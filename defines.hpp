#pragma once

#include <string>
#include <thread>
#include <mutex>
#include "armadillo"


//domen za pseudoslucajne brojeve kojima se popunjavaju matrice
// brojevi su izmedju pivot-range i pivot+range
#define PIVOT 0
#define RANGE 100


//podrazumevani radni folder
#define DIR_NOT_SET ""

//koristice se na raznim mestima (meniji, ispisi itd)
#define LINIJA "----------------------------------------------\n"

//smanjuje broj karaktera u kodu
// (UINT je vec koriscen u jednom headeru windows biblioteke, otuda UINT_)
typedef unsigned int UINT_;


//odredjuje na koliko taskova ce se deliti algoritam pri izvrsavanju
////pih, ipak nisam toliko strucan za #define xD
//#define N_CORES ((UINT_ n_cores = std::thread::hardware_concurrency() != 0) ? n_cores : 16)


//sablon matrice redefinisan za punjenje int-ovima
typedef arma::Mat<int> matrix;


//ukoliko je apsolutna razlika izmedju nekog clana jedne matrice i
// clana na istoj poziciji u drugoj matrici veca od 0, 
//  matrice se nece smatrati jednakim.
//za poredjenje matrica, ovo je jednostavnije od armadillo operator== koji vraca matricu jednakosti;
// tehnicki, *verovatno* mogu postojati slucajevi
//  kada poredjenje nece dati ispravan rezultat, 
//   ali *tvrdim* da je verovatnoca za to veoma mala i 
//	  zanemarljiva za potrebe ovog projekta.
bool operator==(matrix& first, matrix& second);


//matrix triple radi lakseg rukovanja matricama
// (zapravo je pentet, ali nek prodje i ovako xD)
typedef struct mat_trip
{
	std::string triple_name;
	matrix multiplier, multiplicand;
	matrix serial_product, parallel_product, arma_product;
	unsigned long long serial_time, parallel_time, arma_time;
	bool OK, last_multiplication_ok;
	mat_trip() : triple_name("PAIR_NAME_NOT_SET"),
		serial_time(0), parallel_time(0), arma_time(0),
		OK(false), last_multiplication_ok(false)
	{};
} matrix_triple;


//parametar koji se prosledjuje funkciji multiply_pair
class multiply
{
public:
	enum {
		serial,
		parallel,
		arma
	};
};