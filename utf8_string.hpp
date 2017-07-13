//
// Termwrap
//
// termwrap/utf8_string.hpp
//
// Copyright Dr Robert H Crowston, 2017. All rights reserved.
// See LICENCE for licensing rights.
//

// utf8_string is designed to be a drop-in replacement for std::basic_string. .capacity() and .max_size() are not included.

#ifndef RHC_TERMWRAP_UTF8_STRING_H
#define RHC_TERMWRAP_UTF8_STRING_H

#include <algorithm>
#include <cstdint>
#include <iterator>
#include <initializer_list>
#include <string>
#include "utf8_char.hpp"
#include "utf8_string_view.hpp"
#include "/opt/utf8/source/utf8.h"

namespace termwrap
{
	using std::uint32_t;

	class utf8_string
	{
	public:
		using storage_t = std::string;

	private:
		storage_t raw_storage{};

	public:
		using size_type = storage_t::size_type;
		using value_type = std::uint32_t;
		static constexpr auto npos = storage_t::npos;

		using iterator = utf8::unchecked::iterator<storage_t::iterator>;
		using const_iterator = utf8::unchecked::iterator<storage_t::const_iterator>;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

		// Trying to emulate the specification of std::basic_string.

		//
		// Constructors.
		//
		utf8_string() noexcept { }
		utf8_string(size_type count, const uint32_t cp)
		{
			assign(count, cp);
		}
		utf8_string(const utf8_string& other, size_type pos, size_type count = npos)
		{
			assign(other, pos, count);
		}
		utf8_string(const storage_t& other, size_type pos, size_type count = npos)
		{
			raw_storage.assign(storage_t(other, pos, count));
		}
		utf8_string(const uint32_t* cps, size_type count)
		{
			for (; count > 0; --count)
				utf8::append(*cps++, std::back_inserter(raw_storage));
		}
		utf8_string(const uint32_t* cps)
		{
			while (*cps)
				utf8::append(*cps++, std::back_inserter(raw_storage));
		}
		template <class InputIt>
		utf8_string(InputIt first, InputIt last)
		{
			std::copy(first, last, std::back_inserter(raw_storage));
		}
		/*template <>
		utf8_string(utf8::iterator& first, const utf8::iterator& last)
		{
			utf8::utf32to8(first, last, std::back_inserter(raw_storage));
		}*/
		utf8_string(const utf8_string& other) = default;
		utf8_string(const storage_t& other) : raw_storage(other) { }
		utf8_string(utf8_string&& other) = default;
		utf8_string(const char* const other) : raw_storage(other) { }
		utf8_string(const std::initializer_list<uint32_t> init)
		{
			utf8::utf32to8(init.begin(), init.end(), std::back_inserter(raw_storage));
		}
		explicit utf8_string(utf8_string_view sv) : raw_storage(sv.data(), sv.size())
		{ }

		~utf8_string() = default;
		utf8_string& operator=(const utf8_string& other) = default;
		utf8_string& operator=(const storage_t& other) { raw_storage.assign(other); return *this; }
		utf8_string& operator=(const char* str) { raw_storage = str; return *this; }

		//
		// Assignment.
		//
		utf8_string& assign(size_type count, const uint32_t cp)
		{
			raw_storage.clear();
			for (; count > 0; --count)
				utf8::append(cp, std::back_inserter(raw_storage));
			return *this;
		}
		utf8_string& assign(const utf8_string& str)
		{
			raw_storage.assign(str.raw_storage);
			return *this;
		}
		utf8_string& assign(const utf8_string& str, size_type pos, size_type count = npos)
		{
			raw_storage.clear();
			auto begin = str.cbegin();
			std::advance(begin, pos);

			count = std::min({count, str.size()-pos});
			auto end = begin;
			std::advance(end, count);

			utf32to8(begin, end, std::back_inserter(raw_storage));
			return *this;
		}
		utf8_string& assign(utf8_string&& str)
		{
			*this = std::move(str);
			return *this;
		}
		utf8_string& assign(const uint32_t* cps)
		{
			raw_storage.clear();
			while (*cps)
				utf8::append(*cps++, std::back_inserter(raw_storage));
			return *this;
		}
		utf8_string& assign(const_iterator& first, const const_iterator& last)
		{
			raw_storage.clear();
			utf8::utf32to8(first, last, std::back_inserter(raw_storage));
			return *this;
		}
		utf8_string& assign(const std::initializer_list<uint32_t> init)
		{
			raw_storage.clear();
			utf8::utf32to8(init.begin(), init.end(), std::back_inserter(raw_storage));
			return *this;
		}
		utf8_string& assign(const utf8_string_view& sv)
		{
			raw_storage.assign(sv.data(), sv.size());
			return *this;
		}

		//
		// Accessors.
		//
		// Note: unlike std::basic_string, these do not return references.
		uint32_t at(const size_type pos) const
		{
			auto it = cbegin();
			std::advance(it, pos);
			return *it;
		}
		utf8_char at(const size_type pos)
		{
			auto it = begin();
			std::advance(it, pos);
			return utf8_char(raw_storage, &*it.base());
		}
		uint32_t operator[](const size_type pos) const
		{
			return at(pos);
		}
		utf8_char operator[](const size_type pos)
		{
			return at(pos);
		}
		uint32_t front() const
		{
			auto it = cbegin();
			return *it;
		}
		uint32_t back() const
		{
			auto it = cend();
			--it;
			return *it;
		}
		char* data()
		{
			return raw_storage.data();
		}
		const char* data() const
		{
			return raw_storage.data();
		}
		const char* data_at(const size_type pos) const
		{
			auto it = raw_storage.cbegin();
			utf8::unchecked::advance(it, pos);
			return &*it;
		}
		const char* c_str() const
		{
			return raw_storage.c_str();
		}
		operator utf8_string_view() const noexcept
		{
			return utf8_string_view(raw_storage.data(), raw_storage.length());
		}
		//
		// Iterators.
		//
		iterator begin()
		{
			return iterator(raw_storage.begin());
		}
		const_iterator begin() const
		{
			return const_iterator(raw_storage.begin());
		}
		const_iterator cbegin() const
		{
			return const_iterator(raw_storage.cbegin());
		}
		iterator end()
		{
			return iterator(raw_storage.end());
		}
		const_iterator end() const
		{
			return const_iterator(raw_storage.cend());
		}
		const_iterator cend() const
		{
			return const_iterator(raw_storage.cend());
		}
		reverse_iterator rbegin()
		{
			return std::make_reverse_iterator(end());
		}
		const_reverse_iterator rbegin() const
		{
			return std::make_reverse_iterator(cend());
		}
		const_reverse_iterator crbegin() const
		{
			return std::make_reverse_iterator(cend());
		}
		reverse_iterator rend()
		{
			return std::make_reverse_iterator(begin());
		}
		const_reverse_iterator rend() const
		{
			return std::make_reverse_iterator(begin());
		}
		const_reverse_iterator crend() const
		{
			return std::make_reverse_iterator(cbegin());
		}
		//
		// Capacity.
		//
		bool empty() const noexcept
		{
			return raw_storage.empty();
		}
		size_type size() const noexcept
		{
			return utf8::unchecked::distance(raw_storage.cbegin(), raw_storage.cend());
		}
		size_type length() const noexcept
		{
			return size();
		}
		void reserve(size_type new_cap = 0)
		{
			raw_storage.reserve(new_cap);
		}
		void shrink_to_fit()
		{
			raw_storage.shrink_to_fit();
		}
		//
		// Operations.
		//
		void clear()
		{
			raw_storage.clear();
		}
	private:
		iterator get_iterator_at(const size_type index)
		{
			auto it = begin();
			std::advance(it, index);
			return it;
		}
		const_iterator get_iterator_at(const size_type index) const
		{
			auto it = cbegin();
			std::advance(it, index);
			return it;
		}
		iterator get_end_iterator(const iterator begin, const size_type count = npos)
		{
			if (count == npos)
				return end();
			else
			{
				auto end = begin;
				std::advance(end, count);
				return end;
			}
		}
		const_iterator get_end_iterator(const const_iterator begin, const size_type count = npos) const
		{
			if (count == npos)
				return cend();
			else
			{
				auto end = begin;
				std::advance(end, count);
				return end;
			}
		}
	public:
		utf8_string& insert(const size_type index, size_type count, uint32_t cp)
		{
			auto it = get_iterator_at(index).base();
			for (; count > 0; --count)
				utf8::append(cp, std::inserter(raw_storage, it));
			return *this;
		}
		utf8_string& insert(const size_type index, const uint32_t* cps)
		{
			auto it = get_iterator_at(index);
			while (*cps)
				utf8::append(*cps++, std::inserter(raw_storage, it.base()));
			return *this;
		}
		utf8_string& insert(const size_type index, const uint32_t* cps, size_type count)
		{
			auto it = get_iterator_at(index);
			for(; count > 0; --count)
				utf8::append(*cps++, std::inserter(raw_storage, it.base()));
			return *this;
		}
		utf8_string& insert(const size_type index, const utf8_string& str)
		{
			auto it = get_iterator_at(index);
			raw_storage.insert(it.base(), str.raw_storage.begin(), str.raw_storage.end());
			return *this;
		}
		utf8_string& insert(const size_type index, const utf8_string& str, const size_type index_str, size_type count = npos)
		{
			auto it = get_iterator_at(index);
			auto begin = str.get_iterator_at(index_str);
			auto end = str.get_end_iterator(begin, count);
			utf32to8(begin, end, std::inserter(raw_storage, it.base()));
			return *this;
		}
		iterator insert(iterator pos, uint32_t cp)
		{
			utf8::append(cp, std::inserter(raw_storage, pos.base()));
			return pos;
		}
		template <class InputIt>
		iterator insert(const_iterator pos, InputIt first, InputIt last)
		{
			return raw_storage.insert(pos, first, last);
		}
		/*template <>
		iterator insert(const_iterator pos, const_iterator first, const const_iterator last)
		{
			utf8::utf32to8(first, last, std::inserter(raw_storage, pos.base()));
			return pos;
		}*/
		iterator insert(iterator pos, const std::initializer_list<uint32_t> ilist)
		{
			utf8::utf32to8(ilist.begin(), ilist.end(), std::inserter(raw_storage, pos.base()));
			return pos;
		}
		//utf8_string& insert(const size_type pos, utf8_string_view& sv)
		//template <class T>
		//utf8_string& insert(const size_type index, const T& t, size_type index_str, size_type count = npos);
		utf8_string& erase(const size_type index = 0, const size_type count = npos)
		{
			auto begin = get_iterator_at(index);
			auto end = get_end_iterator(begin, count);
			raw_storage.erase(begin.base(), end.base());
			return *this;
		}
		iterator erase(const_iterator pos) noexcept
		{
			auto begin = pos.base();
			auto base = pos.base();
			utf8::unchecked::next(base);
			return iterator(raw_storage.erase(begin, pos.base()));
		}
		iterator erase(const_iterator first, const_iterator last) noexcept
		{
			return iterator(raw_storage.erase(first.base(), last.base()));
		}
		void push_back(uint32_t cp)
		{
			utf8::append(cp, std::back_inserter(raw_storage));
		}
		void pop_back()
		{
			auto it = raw_storage.cend();
			utf8::unchecked::prior(it);
			raw_storage.erase(it, raw_storage.cend());
		}
		utf8_string& append(size_type count, uint32_t cp)
		{
			for (; count > 0; --count)
				utf8::append(cp, std::back_inserter(raw_storage));
			return *this;
		}
		utf8_string& append(const uint32_t* cps)
		{
			while (*cps)
				utf8::append(*cps++, std::back_inserter(raw_storage));
			return *this;
		}
		utf8_string& append(const uint32_t* cps, size_type count)
		{
			for(; count > 0; --count)
				utf8::append(*cps++, std::back_inserter(raw_storage));
			return *this;
		}
		utf8_string& append(const utf8_string& str)
		{
			raw_storage.append(str.raw_storage);
			return *this;
		}
		utf8_string& append(const utf8_string& str, const size_type index_str, size_type count = npos)
		{
			auto begin = str.get_iterator_at(index_str);
			auto end = str.get_end_iterator(begin, count);
			utf32to8(begin, end, std::back_inserter(raw_storage));
			return *this;
		}
		template <class InputIt>
		utf8_string& append(InputIt first, InputIt last)
		{
			raw_storage.append(first, last);
			return *this;
		}
		/*template <>
		utf8_string& append(const_iterator first, const const_iterator last)
		{
			utf8::utf32to8(first, last, std::back_inserter(raw_storage));
			return *this;
		}*/
		utf8_string& append(const std::initializer_list<uint32_t>& ilist)
		{
			utf8::utf32to8(ilist.begin(), ilist.end(), std::back_inserter(raw_storage));
			return *this;
		}
		//utf8_string& append(utf8_string_view& sv)
		//template <class T>
		//utf8_string& append(const T& t, size_type index_str, size_type count = npos);
		utf8_string& operator+=(const utf8_string& str)
		{
			return append(str);
		}
		utf8_string& operator+=(const uint32_t cp)
		{
			push_back(cp);
			return *this;
		}
		utf8_string& operator+=(uint32_t* const cps)
		{
			return append(cps);
		}
		utf8_string& operator+=(const std::initializer_list<uint32_t>& ilist)
		{
			return append(ilist);
		}
		//utf8_string& operator+=(const utf8_string_view& sv);
		int compare(const utf8_string& str) const noexcept
		{
			return raw_storage.compare(str.raw_storage);
		}
		int compare(const size_type pos1, const size_type count1, const utf8_string& str, const size_type pos2 = 0, const size_type count2 = npos) const
		{
			auto begin = get_iterator_at(pos1);
			auto end = get_end_iterator(begin, count1);
			auto str_begin = str.get_iterator_at(pos2);
			auto str_end = get_end_iterator(str_begin, count2);

			utf8_string temporary(begin, end);
			utf8_string temporary_str(str_begin, str_end);

			return temporary.compare(temporary_str);
		}
		int compare(const uint32_t* const cps) const
		{
			utf8_string temporary(cps);
			return compare(temporary);
		}
		int compare(const size_type pos1, const size_type count1, const uint32_t* const cps) const
		{
			utf8_string temporary(cps);
			return compare(pos1, count1, temporary);
		}
		int compare(const size_type pos1, const size_type count1, const uint32_t* const cps, const size_type count2) const
		{
			utf8_string temporary(cps, count2);
			return compare(pos1, count1, temporary);
		}
		// int compare ... string_view
		// replace
		//
		template <class CharT, class Traits>
		friend std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const utf8_string& str);

	}; // End of class utf8_string.

	template <class CharT, class Traits>
	std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const utf8_string& str)
	{
		std::ostream::sentry s(os);
		if (s)
		{
			os.write(str.raw_storage.data(), str.raw_storage.length());
		}
		return os;
	}

} // End of namespace termwrap.

#endif // !RHC_TERMWRAP_UTF8_STRING_H

