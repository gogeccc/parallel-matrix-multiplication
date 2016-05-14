#pragma once
#include "defines.hpp"

//pomnozi multiplier i multiplicand iz prosledjenog tripleta.
//sacuvaj rezultat u product.
//sacuvaj i vreme mnozenja u serial_time ili parallel time polje tripleta
// (u zavisnosti od izabranog nacina mnozenja).
//ako mnozenje uspe, vrati true
// (mozda se promene i vrednosti OK i last_multiplication_ok polja tripleta).
bool multiply_pair(matrix_triple* matrix_triple_, const UINT_ parallel_ = multiply::serial);