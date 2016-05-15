#include "defines.hpp"

bool operator==(matrix& first, matrix& second)
{
	//"absdiff" - apsolutna razlika izmedju elemenata
	//0 - tolerancija (tol = 0 - elementi moraju biti jednaki - abs(first(i, j) - second(i, j)) <= tol
	return arma::approx_equal(first, second, "absdiff", 0);
}

