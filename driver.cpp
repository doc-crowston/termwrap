//
// Termwrap
//
// termwrap/driver.cpp
//
// Copyright Dr Robert H Crowston, 2017. All rights reserved.
// See LICENCE for licensing rights.
//

#include "driver.hpp"

#include <cassert>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <string>
#include "/opt/termbox/include/termbox.h"
#include "impl/optional.hpp"

namespace termwrap
{

	//
	// Driver implementation (wrapper around termbox).
	//

	driver::driver()
	{
		int tb_status = tb_init();
		if (tb_status < 0)
		{
			switch (tb_status)
			{
				case TB_EUNSUPPORTED_TERMINAL:
					throw unsupported_terminal_error();
				case TB_EFAILED_TO_OPEN_TTY:
					throw failed_to_open_terminal_error();
				case TB_EPIPE_TRAP_ERROR:
					throw pipe_trap_error();
				default:
					throw driver_unknown_error();
			}
		}
	}

	driver::~driver()
	{
		tb_shutdown();
	}

	//driver::driver(driver&& other)
	//{ }
	//driver& driver::operator=(driver&& other)
	//{ }

	// Terminal painting.
	void driver::clear()
	{
		tb_clear();
	}

	void driver::redraw()
	{
		tb_present();
	}

	// Cell-level display.
	void driver::set_cell_style(const ordinate_t x, const ordinate_t y, const cell_style& style)
	{
		set_block_style(x,y, x,y, style);
	}

	cell_style driver::get_cell_style(const ordinate_t x, const ordinate_t y) const
	{
		const struct tb_cell* const native_buffer = tb_cell_buffer();
		const struct tb_cell* const cell = native_buffer + y*console_width() + x;
		const cell_style style {cell->fg, cell->bg};
		return style;
	}

	char driver::get_cell_text(const ordinate_t x, const ordinate_t y) const
	{
		const struct tb_cell* const native_buffer = tb_cell_buffer();
		const struct tb_cell* const cell = native_buffer + y*console_width() + x;
		return to_char(cell->ch);
	}

	void driver::set_cell(const ordinate_t x, const ordinate_t y, const char ch, const cell_style& style)
	{
		tb_change_cell(x, y, to_native_char(ch), style.to_native_fg(), style.to_native_bg());
	}

	void driver::write_at(ordinate_t x, const ordinate_t y, const string& text, const cell_style& style)
	{
		if (text.length() > console_width() - x)
			throw text_overflow_error();
		for (const auto& ch : text)
			tb_change_cell(x++, y, to_native_char(ch), style.to_native_fg(), style.to_native_bg());
	}

	void driver::write_at(ordinate_t x, const ordinate_t y, const string& text)
	{
		if (text.length() > console_width() - x)
			throw text_overflow_error();
		for (const auto& ch : text)
		{
			const struct tb_cell* const native_buffer = tb_cell_buffer();
			const struct tb_cell* const cell = native_buffer + y*console_width() + x;
			tb_change_cell(x, y, to_native_char(ch), cell->fg, cell->bg);
			x++;
		}
	}

	void driver::write_block_at(const ordinate_t start_x, const ordinate_t start_y, const string& text)
	{
		ordinate_t x = start_x; ordinate_t y = start_y;
		const auto width = console_width();

		for (const auto& ch : text)
		{
			switch (ch)
			{
				case '\n':
					++y;
					// [[fall_through]
				case '\r':
					x = start_x;
					continue;
				case '\t':
					x += tab_stop_width;
					x = (x/tab_stop_width)*tab_stop_width;
					continue;
			}

			if (x >= console_width())
			{
				x = start_x;
				++y;
			}

			if (y >= console_height())
				throw text_overflow_error();

			const struct tb_cell* const native_buffer = tb_cell_buffer();
			const struct tb_cell* const cell = native_buffer + y*width + x;
			tb_change_cell(x, y, to_native_char(ch), cell->fg, cell->bg);

			++x;
		}

	}

	void driver::set_block_style(const ordinate_t min_x, const ordinate_t min_y, const ordinate_t max_x, const ordinate_t max_y, const cell_style& style)
	{
		const struct tb_cell* const native_buffer = tb_cell_buffer();
		const ordinate_t width = console_width();
		for (ordinate_t y = min_y; y <= max_y; ++y)
		{
			for (ordinate_t x = min_x; x <= max_x; ++x)
			{
				const struct tb_cell* const cell = native_buffer + y*width + x;
				tb_change_cell(x, y, cell->ch, style.to_native_fg(), style.to_native_bg());
			}
		}
	}

	ordinate_t driver::console_height() const
	{
		return tb_height();
	}

	ordinate_t driver::console_width() const
	{
		return tb_width();
	}

	native_char_t driver::to_native_char(const char ch)
	{
		native_char_t nch;
		//assert(
		tb_utf8_char_to_unicode(&nch, &ch);
			//== 0 );
		return nch;
	}

	char driver::to_char(const native_char_t nch)
	{
		char ch;
		//assert(
		tb_utf8_unicode_to_char(&ch, nch);
			//== 0 );
		return ch;
	}

	template <class Rep, class Period>
	optional<key_event> wait_for_key_event(const std::chrono::duration<Rep, Period>& wait_duration)
	{
		
	}
} // End of namespace termwrap.

