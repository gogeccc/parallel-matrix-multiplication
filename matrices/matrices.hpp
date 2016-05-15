#pragma once

#include "../defines.hpp"

//generise par matrica popunjenih slucajnim vrednostima.
//dimenzije matrice su u domenu od pivot-range do pivot+range.
matrix_triple* random_matrix_pair_generator_random_dimensions(matrix_triple* matrix_triple_,
											UINT_ size_range, UINT_ size_pivot);
//generisi matrice kako bi se dobio proizvod odredjenih dimenzija, 
//	vrati praznu matricu ako prosledjeni parametri nisu u redu
matrix_triple* random_matrix_pair_generator_fixed_product_dimensions(matrix_triple* matrix_triple_,
	UINT_ rows, UINT_ columns);
//generisi matrice sa tacno odredjenim dimenzijama, 
//	vrati praznu matricu ako prosledjeni parametri nisu u redu
matrix_triple* random_matrix_pair_generator_fixed_all_dimensions(matrix_triple* matrix_triple_,
	UINT_ first_rows_, UINT_ first_cols__sec_rows, UINT_ second_columns_);
//sacuvaj par mnozilaca u fajl na prosledjenoj putanji.
//FOLDER PATH SE MORA ZAVSITI SA '\' ('\\')
bool save_matrix_pair(matrix_triple* matrix_triple_, std::string folder_path, std::string file_name);
//ucitaj par mnozilaca iz fajla na prosledjenoj putanji.
//FOLDER PATH SE MORA ZAVSITI SA '\' ('\\')
bool load_matrix_pair(matrix_triple* matrix_triple_, std::string folder_path, std::string file_name);