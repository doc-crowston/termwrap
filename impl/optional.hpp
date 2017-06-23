//
// Termwrap
//
// termwrap/impl/optional.hpp
//
// Copyright Dr Robert H Crowston, 2017. All rights reserved.
// See LICENCE for licensing rights.
//

#ifndef RHC_TERMWRAP_IMPL_OPTIONAL_H
#define RHC_TERMWRAP_IMPL_OPTIONAL_H

#include <stdexcept>
#include <utility>

namespace termwrap{ namespace impl
{
	template <class T>
	struct optional : public std::pair<bool, T>
	{
		constexpr bool has_value() noexcept
		{
			return first;
		}
		constexpr T& value()
		{
			if (!has_value) throw std::logic_error("Bad optional access.");
			return second;
		}
		template <class U = T>
		optional& operator=(U&& value)
		{
			second = value;
			has_value_ = true;
		}
	};

} } // End of namespace termwrap::impl.

#endif // !RHC_TERMWRAP_IMPL_OPTIONAL_H.

