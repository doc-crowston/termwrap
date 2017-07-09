#include "utf8_string.hpp"
#include "utf8_string_view.hpp"
#include <algorithm>
#include <iostream>
#include <iterator>

using termwrap::utf8_string;
using termwrap::utf8_string_view;

int main()
{
	utf8_string bumble{"\u00a1Bumble bée¡. ¥ × 2 Alpha école polytechnique §± Señator. 最高 \u00df\u6c34\U0001f34c"};
	std::cout << bumble << std::endl;

	std::cout << bumble.front() << " " << bumble.back() << std::endl;

	auto bobble = utf8_string_view(bumble);
	std::cout << bobble << std::endl;

	bobble.remove_prefix(3);
	std::cout << bobble << std::endl;

	bobble.remove_suffix(5);
	std::cout << bobble << std::endl;
	
	return 0;
}

