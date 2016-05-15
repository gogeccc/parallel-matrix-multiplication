#pragma once
#include "../defines.hpp"

UINT_ calculate_depth(UINT_ n_cores);

matrix_triple* parallel_multiplication_tbb(matrix_triple* matrix_triple_);