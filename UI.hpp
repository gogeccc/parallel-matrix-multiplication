#pragma once
#include "defines.hpp"
#include <string>
#include <vector>
#include <map>

class UI
{
protected:
	std::string working_dir;
public:
	UI() : working_dir(DIR_NOT_SET) {};
	std::string set_working_dir(std::string dir);
	std::string get_working_dir();
};
