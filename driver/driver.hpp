//
// Termwrap
//
// termwrap/driver.hpp
//
// Copyright Dr Robert H Crowston, 2017. All rights reserved.
// See LICENCE for licensing rights.
//

#ifndef RHC_TERMWRAP_DRIVER_H
#define RHC_TERMWRAP_DRIVER_H

#include <chrono>
#include <cstdint>
#include <optional>
#include <string_view>

#include "cell_style.hpp"
#include "key_event.hpp"
#include "error.hpp"
#include "types.hpp"

namespace termwrap
{
	//
	// Driver interface.
	//

	class driver
	{
	public:
		driver();
		~driver();

		driver(const driver& ) = delete;
		driver(driver&& other);
		driver& operator=(const driver& ) = delete;
		driver& operator=(driver&& other);

		// Screen painting.
		void clear();
		void redraw();

		// Line display.		
		void write_at(ordinate_t x, const ordinate_t y, const string_view& text);
		void write_at(ordinate_t x, const ordinate_t y, const string_view& text, const cell_style& style);

		// Block display.
		void write_block_at(ordinate_t x, ordinate_t y, const string_view& text);
		void write_block_at(ordinate_t x, ordinate_t y, ordinate_t max_y, const string_view& text);
		void write_block_at(ordinate_t x, ordinate_t y, ordinate_t max_x, ordinate_t max_y, const string_view& text);
		void set_block_style(const ordinate_t min_x, const ordinate_t min_y, const ordinate_t max_x, const ordinate_t max_y, const cell_style& style);

		
		// Cell-level display.
		void set_cell_style(const ordinate_t x, const ordinate_t y, const cell_style& style);
		void set_cell(const ordinate_t x, const ordinate_t y, const u8char_t ch, const cell_style& style);
		cell_style get_cell_style(const ordinate_t x, const ordinate_t y) const;
		u8char_t get_cell_text(const ordinate_t x, const ordinate_t y) const;

		// Terminal properties.
		ordinate_t console_height() const;
		ordinate_t console_width() const;
		/*bool supports_16_bit_color() const;
		bool supports_256_bit_color() const;
		bool supports_bold() const;*/

		//static native_char_t to_native_char(const char ch);
		//static char to_char(const native_char_t nch);

		void hide_cursor();
		void set_cursor_position(const ordinate_t x, const ordinate_t y);
		
	private:
		std::optional<key_event> wait_for_key_event_impl(const unsigned wait_ms);

	public:
		template <class Rep, class Period>
		std::optional<key_event> wait_for_key_event(const std::chrono::duration<Rep, Period>& wait_duration)
		{
			const auto wait_ms = std::chrono::duration_cast<std::chrono::milliseconds>(wait_duration).count();
			return wait_for_key_event_impl(wait_ms);
		}


		// Callbacks.
		/*template <class Callable>
		void on_width_change(Callable&& callback);
		template <class Callable>
		void on_height_change(Callable&& callback);
		template <class Callable>
		void on_capabilities_change(Callable&& callback);
		*/
		
		ordinate_t tab_stop_width = 4;

	};
} // End of namespace termwrap.

#endif // !RHC_TERMWRAP_DRIVER_H.

