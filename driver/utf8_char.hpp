//
// Termwrap
//
// termwrap/utf8_char.hpp
//
// Copyright Dr Robert H Crowston, 2017. All rights reserved.
// See LICENCE for licensing rights.
//

#ifndef RHC_TERMWRAP_UTF8_CHAR_H
#define RHC_TERMWRAP_UTF8_CHAR_H

#include <cstddef>
#include <cstdint>
#include <iterator>
#include <string>
#include "/opt/utf8/source/utf8.h"

namespace termwrap
{
	using std::uint32_t;

	struct utf8_char
	{
		utf8_char(std::string& octet_storage, const std::string::pointer octet_start)
			: octet_storage(octet_storage), octet_start(octet_start)
		{ }

		operator uint32_t() const noexcept
		{
			auto temp = utf8::unchecked::iterator(octet_start);
			return *temp;
		}

		utf8_char& operator=(const uint32_t cp)
		{
			auto begin = utf8::unchecked::iterator(octet_start);
			auto end = utf8::unchecked::iterator(octet_start);
			std::next(end);
			const auto it = octet_storage.erase(std::string::iterator(begin.base()), std::string::iterator(end.base()));
			utf8::append(cp, std::inserter(octet_storage, it));
			return *this;
		}

		std::string::const_pointer operator&() const noexcept
		{
			return octet_start;
		}

		private:
			std::string& octet_storage;			
			const std::string::pointer octet_start;
	}; // End of struct utf8_char.
} // End of namespace termwrap.

#endif // !RHC_TERMWRAP_UTF8_CHAR_H.

