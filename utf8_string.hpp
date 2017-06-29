//
// Termwrap
//
// termwrap/utf8_string.hpp
//
// Copyright Dr Robert H Crowston, 2017. All rights reserved.
// See LICENCE for licensing rights.
//

#ifndef RHC_TERMWRAP_UTF8_STRING_H
#define RHC_TERMWRAP_UTF8_STRING_H

#include <algorithm>
#include <cstdint>
#include <iterator>
#include <initializer_list>
#include <string>
#include "/opt/utf8/source/utf8.h"

namespace termwrap
{
	template <typename StorageT = std::basic_string>
	class utf8_string
	{
		StorageT raw_storage{};
		using std::uint32_t;

		public: 
		using size_type = StorageT::size_type;
		using npos = StorageT::npos;

		// Trying to emulate the specification of std::basic_string.
		
		//
		// Constructors.
		//
		utf8_string() noexcept { }
		utf8_string(size_type count, const uint32_t cp)
		{
			for (; count > 0; --count)
				utf8::append(std::back_inserter(raw_storage), cp);
		}
		utf8_string(const utf8_string& other, size_type pos, size_type count = npos)
		{
			auto begin = other.cbegin();
			std::advance(begin, pos);
			
			count = std::min({count, other.size()-pos});
			auto end = begin;
			std::advance(end, count);

			utf32to8(begin, end, std::back_inserter(raw_storage));
		}
		utf8_string(const StorageT& other, size_type pos, size_type count = npos)
		{
			raw_storage(StorageT(other.raw_storage, pos, count));
		}
		utf8_string(const uint32_t* cps, size_type count)
		{
			for (; count > 0; --count)
				utf8::append(std::back_inserter(raw_storage), *cps++);
		}
		utf8_string(const uint32_t* const cps)
		{
			while (*cps)
				utf8::append(std::back_inserter(raw_storage), *cps++);
		}
		template <class InputIt> // enableif = not utf8
		utf8_string(InputIt first, InputIt last)
		{
			std::copy(first, last, std::back_inserter(raw_storage))
		}
		template <>
		utf8_string<utf8::iterator>(utf8::iterator& first, const utf8::iterator& last)
		{
			utf8::utf32to8(first, last, std::back_inserter(raw_storage));
		}
		utf8_string(const utf_string& other) = default;
		utf8_string(const StorageT& other) : raw_storage(other) { }
		utf8_string(utf_string&& other) = default;
		utf8_string(std::initializer_list<uint32_t> init)
		{
			utf8::utf32to8(init.begin(), init.end(), std::back_inserter(raw_storage));
		}
		//explicit utf8_string(utf8_string_view sv) : utf8_string(sv.data(), sv.size())
		//{ }
		
		~utf8_string() = default;
		utf8_string& operator=(const utf8_string& other) = default;
		utf8_string& operator=(const StorageT& other) : raw_storage(other) { }

		

		
	}; // End of class utf8_string.

} // End of namespace termwrap.

#endif // !RHC_TERMWRAP_UTF8_STRING_H

