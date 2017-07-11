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
#include "utf8_string.hpp"
#include "utf8_string_view.hpp"

namespace termwrap
{
	using std::size_t;
	using u8char_t = std::uint32_t;

	using ordinate_t = std::uint_least16_t;
	using native_char_t = std::uint32_t;
	using string = utf8_string;
	using string_view = utf8_string_view;
} // End of namspace termwrap.

#endif // !RHC_TERMWRAP_TYPES_H.

