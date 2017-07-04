//
// Termwrap
//
// termwrap/utf8_string_view.hpp
//
// Copyright Dr Robert H Crowston, 2017. All rights reserved.
// See LICENCE for licensing rights.
//

// utf8_string_view is designed to be a drop-in replacement for std::basic_string_view.

#ifndef RHC_TERMWRAP_UTF8_STRING_VIEW_H
#define RHC_TERMWRAP_UTF8_STRING_VIEW_H

#include <cstdint>
#include <cstring>
#include <iterator>
#include <string_view>
#include "/opt/utf8/source/utf8.h"

namespace termwrap
{
	class utf8_string_view
	{
	public:
		using storage_t = std::string_view;
		using value_type = std::uint32_t;
		using size_type = storage_t::size_type;
		using const_iterator = utf8::unchecked::iterator<storage_t::const_iterator>;
		using iterator = const_iterator;
		using reverse_iterator = std::reverse_iterator<const_iterator>;
		using const_reverse_iterator = reverse_iterator;
		using safe_iterator = utf8::checked::iterator<storage_t::const_iterator>;

	private:
		storage_t octet_view;

	public:
		//
		// Constructors.
		//
		constexpr utf8_string_view() noexcept 
			: octet_view{} 
		{ }
		constexpr utf8_string_view(const utf8_string_view& other) noexcept = default;
		constexpr utf8_string_view(const storage_t& sv) noexcept
			: octet_view{sv}
		{ }
		constexpr utf8_string_view(const char* const s, const size_type count) noexcept
			: octet_view{s, count}
		{ }
		constexpr utf8_string_view(const char* const s)
			: octet_view{s}
		{ }
		
		constexpr operator=(const utf8_string_view& sv) noexcept = default;

		//
		// Iterators.
		//
		constexpr const_iterator begin() const noexcept
		{
			return iterator(octet_view.begin());
		}
		constexpr const_iterator cbegin() const noexcept
		{
			return begin();
		}
		constexpr const_iterator end() const noexcept
		{
			return iterator(octet_view.end());
		}
		constexpr const_iterator cend() const noexcept
		{
			return end();
		}
		constexpr const_iterator rbegin() const noexcept
		{
			return std::make_reverse_iterator(end());
		}
		constexpr const_iterator crbegin() const noexcept
		{
			return rbegin();
		}
		constexpr const_iterator rend() const noexcept
		{
			return std::make_reverse_iterator(begin());
		}
		constexpr const_iterator crend() const noexcept
		{
			return rend();
		}

		//
		// Accessors.
		//
		constexpr value_type operator[](const size_type pos) const
		{
			auto it = cbegin();
			std::advance(it, pos);
			return *it;
		}
		constexpr value_type at(const size_type pos) const
		{
			auto it = safe_iterator(octet_view.cbegin(), octet_view.cbegin(), octet_view.cend());
			std::advance(it, pos);
			return *it;
		}
		constexpr value_type front() const
		{
			return operator[](0);
		}
		constexpr value_type back() const
		{
			auto it = cend();
			--it;
			return *it;
		}
		
		//
		// Capacity.
		//
		size_type size() const noexcept
		{
			return utf8::unchecked::distance(octet_view.cbegin(), octet_view.cend());
		}
		constexpr empty() const noexcept
		{
			return octet_view.empty();
		}



	}; // End of class utf8_string_view.

} // End of namespace termwrap.

#endif // !RHC_TERMWRAP_UTF8_STRING_VIEW_H


