//
// Termwrap
//
// termwrap/textbox.hpp
//
// Copyright Dr Robert H Crowston, 2017. All rights reserved.
// See LICENCE for licensing rights.
//

#ifndef RHC_TERMBOX_TEXTBOX_H
#define RHC_TERMBOX_TEXTBOX_H

#include "cell_style.hpp"
#include "driver.hpp"
#include "key_event.hpp"
#include "types.hpp"

namespace termwrap
{
	class textbox
	{
		driver& parent;
		
		const size_t max_content_length;
		const ordinate_t display_width;
		
		const ordinate_t begin_x;
		const ordinate_t begin_y;
		constexpr ordinate_t end_x() { return begin_x + max_view_length - 1; }

		cell_style unfilled_style{};
		cell_style filled_style{};

		string content{};

	public:
		enum class insert_mode
		{
			overwrite,
			insert
		};
	
	private:
		bool has_focus = false;
		bool input_accepted = false;
		
		insert_mode mode = insert_mode::insert;

		size_t view_position = 0;
		size_t cursor_position = 0;

	public:
		textbox(driver& parent, const ordinate_t start_x, const ordinate_t start_y, const ordinate_t display_width, const size_t max_content_length, const cell_style& unfilled_style, const cell_style& filled_style);

		void set_content(const string_view& content);
		string_view get_content() const noexcept { return content; }

		constexpr bool check_input_accepted() noexcept { return input_accepted; } 

		void set_focus(const bool focus);		constexpr bool check_has_focus() noexcept { return has_focus; }

		void set_mode(const insert_mode mode_) noexcept { mode = mode_; }
		constexpr insert_mode get_mode() const noexcept { return mode; }
		
		void set_view_position(const size_t position) noexcept { view_position = position; }
		constexpr size_t get_view_position() noexcept const { return view_position; }

		void set_cursor_position(const size_t cursor_positon) noexcept { return cursor_position; }
		constexpr size_t get_cursor_position() noexcept const { return cursor_position; }

		void redraw();
		
		void cursor_left();
		void cursor_right();
		void cursor_home();
		void cursor_end();

		void accept_key_event(const key_event& event);
	
	}; // End of class textbox.
} // End of namespace termwrap.

#endif // !RHC_TERMBOX_TEXTBOX_H

