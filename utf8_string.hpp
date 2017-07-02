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
		utf8_string& assign(utf8::iterator& first, const utf8::iterator& last)
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
		



	}; // End of class utf8_string.

} // End of namespace termwrap.

#endif // !RHC_TERMWRAP_UTF8_STRING_H

