#include "utf8_string.hpp"
#include "utf8_string_view.hpp"
#include <algorithm>
#include <iostream>
#include <iterator>

using termwrap::utf8_string;
using termwrap::utf8_string_view;

int main()
{
	utf8_string bumble{"高"};

	utf8_string_view bob{bumble};

	for (const auto& ch : bob)
		std::cout << ch << std::endl;
	
	return 0;
}

