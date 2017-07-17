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
		using safe_iterator = utf8::iterator<storage_t::const_iterator>;
		using const_pointer = storage_t::const_pointer;

		const static auto npos = storage_t::npos;

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

		constexpr utf8_string_view& operator=(const utf8_string_view& sv) noexcept = default;

		//
		// Iterators.
		//
		/*constexpr*/ const_iterator begin() const noexcept
		{
			return iterator(octet_view.begin());
		}
		/*constexpr*/ const_iterator cbegin() const noexcept
		{
			return begin();
		}
		/*constexpr*/ const_iterator end() const noexcept
		{
			return iterator(octet_view.end());
		}
		/*constexpr*/ const_iterator cend() const noexcept
		{
			return end();
		}
		//constexpr const_reverse_iterator rbegin() const noexcept
		//{
		//	return std::make_reverse_iterator(end());
		//}
		//constexpr const_reverse_iterator crbegin() const noexcept
		//{
		//	return rbegin();
		//}
		//constexpr const_reverse_iterator rend() const noexcept
		//{
		//	return std::make_reverse_iterator(begin());
		//}
		//constexpr const_reverse_iterator crend() const noexcept
		//{
		//	return rend();
		//}

		//
		// Accessors.
		//
		/*constexpr*/ value_type operator[](const size_type pos) const
		{
			auto it = cbegin();
			std::advance(it, pos);
			return *it;
		}
		/*constexpr*/ value_type at(const size_type pos) const
		{
			auto it = safe_iterator(octet_view.cbegin(), octet_view.cbegin(), octet_view.cend());
			std::advance(it, pos);
			return *it;
		}
		/*constexpr*/ value_type front() const
		{
			return operator[](0);
		}
		/*constexpr*/ value_type back() const
		{
			auto it = cend();
			--it;
			return *it;
		}
		/*constexpr*/ const_pointer data() const noexcept
		{
			return octet_view.data();
		}
		const_pointer data_at(const size_type pos) const
		{
			auto it = octet_view.cbegin();
			utf8::unchecked::advance(it, pos);
			return &*it;
		}

		//
		// Capacity.
		//
		size_type size() const noexcept
		{
			return utf8::unchecked::distance(octet_view.cbegin(), octet_view.cend());
		}
		size_type length() const noexcept
		{
			return size();
		}
		size_type octet_size() const noexcept
		{
			return octet_view.size();
		}
		constexpr bool empty() const noexcept
		{
			return octet_view.empty();
		}
		//
		// Modifiers.
		//
		/*constexpr*/ void remove_prefix(size_type n)
		{
			octet_view.remove_prefix(octet_count(n));
		}
		/*constexpr*/ void remove_suffix(size_type n)
		{
			octet_view.remove_suffix(octet_count(n, octet_view.crbegin()));
		}
		void swap(utf8_string_view& other) noexcept
		{
			octet_view.swap(other.octet_view);
		}
		//
		// Operations.
		//
		size_type copy(char* dest, const size_type count, const size_type pos = 0) const
		{
			const auto octet_pos = octet_count(pos);
			auto octet_it = &octet_view[octet_pos];
			const auto octet_c   = octet_count(count, octet_it);
			octet_view.copy(dest, octet_c, octet_pos);
			return octet_c;
		}
		/*constexpr*/ utf8_string_view substr(const size_type pos = 0, const size_type count = npos) const
		{
			const auto octet_pos = octet_count(pos);
			const auto rcount = std::min((octet_view.length() - octet_pos), octet_count(count));
			return utf8_string_view(&octet_view[octet_pos], rcount);
		}
		// find, rfind, find_first_of, find_last_of, et c..
		//
		// Comparisons.
		//
		/*constexpr*/ bool operator==(const utf8_string_view& other) const noexcept
		{
			return (octet_view == other.octet_view);
		}
		/*constexpr*/ bool operator!=(const utf8_string_view& other) const noexcept
		{
			return (octet_view != other.octet_view);
		}
		/*constexpr*/ bool operator<(const utf8_string_view& other) const noexcept
		{
			return (octet_view < other.octet_view);
		}
		/*constexpr*/ bool operator<=(const utf8_string_view& other) const noexcept
		{
			return (octet_view <= other.octet_view);
		}
		/*constexpr*/ bool operator>(const utf8_string_view& other) const noexcept
		{
			return (octet_view > other.octet_view);
		}
		/*constexpr*/ bool operator>=(const utf8_string_view& other) const noexcept
		{
			return (octet_view >= other.octet_view);
		}

	private:
		//
		// Helpers.
		//
		/*constexpr*/ size_type octet_count(const size_type cp_count) const
		{
			return octet_count(cp_count, octet_view.cbegin());
		}

		template <class Iterator>
		size_type octet_count(size_type cp_count, Iterator begin) const
		{
			if (cp_count == npos) return npos;
			size_type octets = 0;
			for (; cp_count > 0; --cp_count)
			{
				const auto octets_per_cp = utf8::internal::sequence_length(begin);
				octets += octets_per_cp;
				std::advance(begin, octets_per_cp);
			}
			return octets;
		}

	}; // End of class utf8_string_view.

	template <class CharT, class Traits>
	std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const utf8_string_view& sv)
	{
		std::ostream::sentry s(os);
		if (s)
		{
			os.write(sv.data(), sv.octet_size());
		}
		return os;
	}

} // End of namespace termwrap.

#endif // !RHC_TERMWRAP_UTF8_STRING_VIEW_H


