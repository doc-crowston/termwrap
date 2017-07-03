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
#include "/opt/utf8/source/utf8.h"

namespace termwrap
{
	class utf8_string
	{
	public:
		using storage_t = std::string;
		using std::uint32_t;

	private:
		storage_t raw_storage{};

	public:
		using size_type = storage_t::size_type;
		using npos = storage_t::npos;

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
			raw_storage(storage_t(other.raw_storage, pos, count));
		}
		utf8_string(uint32_t* const cps, size_type count)
		{
			for (; count > 0; --count)
				utf8::append(std::back_inserter(raw_storage), *cps++);
		}
		utf8_string(uint32_t* const cps)
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
		utf8_string(const StorageT& other) : raw_storage.assign(other) { }
		utf8_string(utf_string&& other) = default;
		utf8_string(const std::initializer_list<uint32_t> init)
		{
			utf8::utf32to8(init.begin(), init.end(), std::back_inserter(raw_storage));
		}
		//explicit utf8_string(utf8_string_view sv) : utf8_string(sv.data(), sv.size())
		//{ }

		~utf8_string() = default;
		utf8_string& operator=(const utf8_string& other) = default;
		utf8_string& operator=(const StorageT& other) : raw_storage.assign(other) { }

		//
		// Assignment.
		//
		utf8_string& assign(size_type count, const uint32_t cp)
		{
			raw_storage.clear();
			for (; count > 0; --count)
				utf8::append(std::back_inserter(raw_storage), cp);
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
		utf8_string& assign(const uint32_t* const cps)
		{
			raw_storage.clear();
			while (*cps)
				utf8::append(std::back_inserter(raw_storage), *cps++);
		}
		utf8_string& assign(const_iterator& first, const const_iterator& last)
		{
			raw_storage.clear();
			utf8::utf32to8(first, last, std::back_inserter(raw_storage));
		}
		utf8_string& assign(const std::initializer_list<uint32_t> init)
		{
			raw_storage.clear();
			utf8::utf32to8(init.begin(), init.end(), std::back_inserter(raw_storage));
		}
		//utf8_string& assign(utf8_string_view sv) { utf8_string(sv.data(), sv.size())}
		//{ }

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
		uint32_t operator[](const size_type pos) const
		{
			return at(pos);
		}
		uint32_t front() const
		{
			return validate_next(cbegin());
		}
		uint32_t back() const
		{
			auto it = cend();
			--it;
			return validate_next(it);
		}
		char* data()
		{
			return storage.data();
		}
		const char* data() const
		{
			return storage.data();
		}
		const char* c_str() const
		{
			return storage.c_str();
		}
		//operator utf8_string_view() const noexcept
		//{
		//}
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
		const_interator cbegin() const
		{
			return const_iterator(raw_storage.cbegin());
		}
		iterator end()
		{
			return iterator(raw_storage.end());
		}
		const_iterator end() const
		{
			return iterator(raw_storage.end());
		}
		const_iterator cend() const
		{
			return iterator(raw_storage.cend());
		}
		reverse_iterator rbegin()
		{
			return reverse_iterator(raw_storage.end());
		}
		const_reverse_iterator rbegin() const
		{
			return const_reverse_iterator(raw_storage.end());
		}
		const_reverse_iterator crbegin() const
		{
			return const_reverse_iterator(raw_storage.cend());
		}
		reverse_iterator rend()
		{
			return reverse_iterator(raw_storage.begin());
		}
		const_reverse_iterator rend() const
		{
			return const_reverse_iterator(raw_storage.begin());
		}
		const_reverse_iterator crend() const
		{
			return const_reverse_iterator(raw_storage.cbegin());
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
			return utf8::unchecked::distance(cbegin(), cend());
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
		const_iterator get_iterator_at(const size_type index) const
		{
			auto it = cbegin();
			std::advance(it, index);
			return it;
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
			auto it = get_iterator_at(index);
			for (; count > 0; --count)
				utf8::append(cp, std::inserter(raw_storage, it.base()));
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
			raw_storage.insert(it.base(), str.raw_storage);
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
		iterator insert(const_iterator pos, uint32_t cp)
		{
			utf8::append(cp, std::inserter(raw_storage, pos.base()));
			return pos;
		}
		template <class InputIt>
		iterator insert(const_iterator pos, InputIt first, InputIt last)
		{
			return raw_storage.insert(pos, first, last);
		}
		template <>
		iterator insert(const_iterator pos, const_iterator first, const const_iterator last)
		{
			utf8::utf32to8(first, last, std::inserter(raw_storage, pos.base()));
			return pos;
		}
		iterator insert(const_iterator pos, const std::initializer_list<uint32_t> ilist)
		{
			utf8::utf32to8(ilist.begin(), ilist.end(), std::inserter(raw_storage, pos.base()));
			return pos;
		}
		//utf8_string& insert(const size_type pos, utf8_string_view& sv)
		//template <class T>
		//utf8_string& insert(const size_type index, const T& t, size_type index_str, size_type count = npos);
		basic_string& erase(const size_type index = 0, const size_type = npos)
		{
			auto begin = get_iterator_at(index);
			auto end = get_end_iterator(begin, index);
			raw_storage.erase(begin, end);
			return *this;
		}
		iterator erase(const_iterator pos) noexcept
		{
			auto begin = pos.base();
			utf8::unchecked::next(pos.base());
			return iterator(raw_storage.erase(begin, pos));
		}
		iterator erase(const_iterator first, const_iterator last) noexcept
		{
			return iterator(raw_storage.erase(first.base(), last.base());
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
			raw_storage.append(pos, first, last);
			return *this;
		}
		template <>
		utf8_string& append(const_iterator first, const const_iterator last)
		{
			utf8::utf32to8(first, last, std::back_inserter(raw_storage));
			return *this;
		}
		utf8_string append(const std::initializer_list<uint32_t>& ilist)
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

	}; // End of class utf8_string.

} // End of namespace termwrap.

#endif // !RHC_TERMWRAP_UTF8_STRING_H

