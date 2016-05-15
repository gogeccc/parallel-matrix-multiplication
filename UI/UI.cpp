#include "UI.hpp"
#include "../matrices/matrices.hpp"
#include "../multiplication/multiplication.hpp"
#include "../defines.hpp"
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

