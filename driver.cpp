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
#include <optional>

#include "/opt/termbox/include/termbox.h"
#include "types.hpp"

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

	u8char_t driver::get_cell_text(const ordinate_t x, const ordinate_t y) const
	{
		const struct tb_cell* const native_buffer = tb_cell_buffer();
		const struct tb_cell* const cell = native_buffer + y*console_width() + x;
		return cell->ch;
	}

	void driver::set_cell(const ordinate_t x, const ordinate_t y, const u8char_t ch, const cell_style& style)
	{
		tb_change_cell(x, y, ch, style.to_native_fg(), style.to_native_bg());
	}

	void driver::write_at(ordinate_t x, const ordinate_t y, const string_view& text, const cell_style& style)
	{
		if (text.length() > console_width() - x)
			throw text_overflow_error();
		for (const auto& ch : text)
			tb_change_cell(x++, y, ch, style.to_native_fg(), style.to_native_bg());
	}

	void driver::write_at(ordinate_t x, const ordinate_t y, const string_view& text)
	{
		if (text.length() > console_width() - x)
			throw text_overflow_error();
		for (const auto& ch : text)
		{
			const struct tb_cell* const native_buffer = tb_cell_buffer();
			const struct tb_cell* const cell = native_buffer + y*console_width() + x;
			tb_change_cell(x, y, ch, cell->fg, cell->bg);
			x++;
		}
	}

	void driver::write_block_at(const ordinate_t start_x, const ordinate_t start_y, const string_view& text)
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
			tb_change_cell(x, y, ch, cell->fg, cell->bg);

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

	/*native_char_t driver::to_native_char(const char ch)
	{
		native_char_t nch;
		tb_utf8_char_to_unicode(&nch, &ch);
		return nch;
	}

	char driver::to_char(const native_char_t nch)
	{
		char ch;

		const auto byte_length = tb_utf8_unicode_to_char(&ch, nch);
		if (byte_length != 1)
			throw utf8_codepoint_too_wide_error();

		return ch;
	}*/

	void driver::hide_cursor()
	{
		tb_set_cursor(TB_HIDE_CURSOR, TB_HIDE_CURSOR);
	}

	void driver::set_cursor_position(const ordinate_t x, const ordinate_t y)
	{
		tb_set_cursor(x, y);
	}

	std::optional<key_event> driver::wait_for_key_event_impl(const unsigned wait_ms)
	{
		struct tb_event native_event{};

		const int state = tb_peek_event(&native_event, wait_ms);

		if (state < 0)
			throw failed_peek_poll_event();
		if (state == 0)
			return {};
		if (state != TB_EVENT_KEY)
			return {};

		const bool alt = false; //native_event.mod & TB_MOD_ALT;

		if (native_event.ch)
			return key_event(native_event.ch, false, alt);

		switch (native_event.key)
		{
			case TB_KEY_ESC:
				return key_event(special_key::escape, false, alt);
			case TB_KEY_F1:
				return key_event(special_key::F1, false, alt);
			case TB_KEY_F2:
				return key_event(special_key::F2, false, alt);
			case TB_KEY_F3:
				return key_event(special_key::F3, false, alt);
			case TB_KEY_F4:
				return key_event(special_key::F4, false, alt);
			case TB_KEY_F5:
				return key_event(special_key::F5, false, alt);
			case TB_KEY_F6:
				return key_event(special_key::F6, false, alt);
			case TB_KEY_F7:
				return key_event(special_key::F7, false, alt);
			case TB_KEY_F8:
				return key_event(special_key::F8, false, alt);
			case TB_KEY_F9:
				return key_event(special_key::F9, false, alt);
			case TB_KEY_F10:
				return key_event(special_key::F10, false, alt);
			case TB_KEY_F11:
				return key_event(special_key::F11, false, alt);
			case TB_KEY_F12:
				return key_event(special_key::F12, false, alt);

			case TB_KEY_INSERT:
				return key_event(special_key::insert, false, alt);
			case TB_KEY_DELETE:
				return key_event(special_key::del, false, alt);
			case TB_KEY_HOME:
				return key_event(special_key::home, false, alt);
			case TB_KEY_END:
				return key_event(special_key::end, false, alt);
			case TB_KEY_PGUP:
				return key_event(special_key::page_up, false, alt);
			case TB_KEY_PGDN:
				return key_event(special_key::page_down, false, alt);

			case TB_KEY_ARROW_UP:
				return key_event(special_key::arrow_up, false, alt);
			case TB_KEY_ARROW_DOWN:
				return key_event(special_key::arrow_down, false, alt);
			case TB_KEY_ARROW_LEFT:
				return key_event(special_key::arrow_left, false, alt);
			case TB_KEY_ARROW_RIGHT:
				return key_event(special_key::arrow_right, false, alt);

			case TB_KEY_ENTER:
				return key_event(special_key::enter, false, alt);
			case TB_KEY_SPACE:
				return key_event(' ', false, alt);
			case TB_KEY_TAB:
				return key_event(special_key::tab, false, alt);
			case TB_KEY_BACKSPACE2:
			case TB_KEY_BACKSPACE:
				return key_event(special_key::backspace, false, alt);

			case TB_KEY_CTRL_A:
				return key_event('A', true, alt);
			case TB_KEY_CTRL_B:
				return key_event('B', true, alt);
			case TB_KEY_CTRL_C:
				return key_event('C', true, alt);
			case TB_KEY_CTRL_D:
				return key_event('D', true, alt);
			case TB_KEY_CTRL_E:
				return key_event('E', true, alt);
			case TB_KEY_CTRL_F:
				return key_event('F', true, alt);
			case TB_KEY_CTRL_G:
				return key_event('G', true, alt);
			//case TB_KEY_CTRL_H:
			//	return key_event('H', true, alt);
			//case TB_KEY_CTRL_I:
			//	return key_event('I', true, alt);
			case TB_KEY_CTRL_J:
				return key_event('J', true, alt);
			case TB_KEY_CTRL_K:
				return key_event('K', true, alt);
			case TB_KEY_CTRL_L:
				return key_event('L', true, alt);
			//case TB_KEY_CTRL_M:
			//	return key_event('M', true, alt);
			case TB_KEY_CTRL_N:
				return key_event('N', true, alt);
			case TB_KEY_CTRL_O:
				return key_event('O', true, alt);
			case TB_KEY_CTRL_P:
				return key_event('P', true, alt);
			case TB_KEY_CTRL_Q:
				return key_event('Q', true, alt);
			case TB_KEY_CTRL_R:
				return key_event('R', true, alt);
			case TB_KEY_CTRL_S:
				return key_event('S', true, alt);
			case TB_KEY_CTRL_T:
				return key_event('T', true, alt);
			case TB_KEY_CTRL_U:
				return key_event('U', true, alt);
			case TB_KEY_CTRL_V:
				return key_event('V', true, alt);
			case TB_KEY_CTRL_W:
				return key_event('W', true, alt);
			case TB_KEY_CTRL_X:
				return key_event('X', true, alt);
			case TB_KEY_CTRL_Y:
				return key_event('Y', true, alt);
			case TB_KEY_CTRL_Z:
				return key_event('Z', true, alt);

			//case TB_KEY_CTRL_TILDE:
			//	return key_event('~', true, alt);
			case TB_KEY_CTRL_2:
				return key_event('2', true, alt);
			//case TB_KEY_CTRL_3:
			//	return key_event('3', true, alt);
			case TB_KEY_CTRL_4:
				return key_event('4', true, alt);
			case TB_KEY_CTRL_5:
				return key_event('5', true, alt);
			case TB_KEY_CTRL_6:
				return key_event('6', true, alt);
			case TB_KEY_CTRL_7:
				return key_event('7', true, alt);
			//case TB_KEY_CTRL_8:
			//	return key_event('8', true, alt);

			//case TB_KEY_CTRL_BACKSLASH:
			//	return key_event('\\', true, alt);
			//case TB_KEY_CTRL_SLASH:
			//	return key_event('/', true, alt);
			//case TB_KEY_CTRL_UNDERSCORE:
			//	return key_event('_', true, alt);

			default:
				assert(false);
		}
		return {};
	}

} // End of namespace termwrap.

