#include "utf8_string.hpp"
#include <algorithm>
#include <iostream>
#include <iterator>

using termwrap::utf8_string;

int main()
{
	utf8_string bumble{"\u00a1Bumble bée¡. ¥ × 2"};
	std::cout << bumble << std::endl;

	std::cout << bumble.front() << " " << bumble.back() << std::endl;
	
	return 0;
}

