//
// Termwrap
//
// termwrap/types.hpp
//
// Copyright Dr Robert H Crowston, 2017. All rights reserved.
// See LICENCE for licensing rights.
//

#ifndef RHC_TERMWRAP_TYPES_H
#define RHC_TERMWRAP_TYPES_H

#include <cstdint>
#include <string>
#include <string_view>

namespace termwrap
{
	using std::size_t;

	using ordinate_t = std::uint_least16_t;
	using native_char_t = std::uint32_t;
	using string = std::string;
	using string_view = std::string_view;
} // End of namspace termwrap.

#endif // !RHC_TERMWRAP_TYPES_H.

