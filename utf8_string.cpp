#include "utf8_string.hpp"
#include <iostream>

using termwrap::utf8_string;

int main()
{
	utf8_string bumble{"Bumble bée¡."};
	std::cout << bumble << std::endl;
	return 0;
}

