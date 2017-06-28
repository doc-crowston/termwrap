//
// Termwrap
//
// termwrap/textbox.cpp
//
// Copyright Dr Robert H Crowston, 2017. All rights reserved.
// See LICENCE for licensing rights.
//

#include "textbox.hpp"

#include <algorithm>

#include "driver.hpp"

namespace termwrap
{
	textbox::textbox(driver& parent, const ordinate_t begin_x, const ordinate_t begin_y, const ordinate_t display_width, const size_t max_content_length, const cell_style& unfilled_style, const cell_style& filled_style)
		: parent(parent), max_content_length(max_content_length), display_width(display_width), begin_x(begin_x), begin_y(begin_y), unfilled_style(unfilled_style), filled_style(filled_style)
	{
		redraw();
	}

	void textbox::redraw()
	{
		cursor_position = std::min( {content.length(), max_content_length-1, cursor_position} );

		const size_t pad_right = (content.length() < max_content_length) ? 1 : 0;
		const size_t padded_length = content.length() + pad_right;

		if (padded_length < max_view_length)	// Too little text; no scrolling.
			view_position = 0;
		else if (view_position > padded_length-max_view_length)	// Scrolled too far to the right.
			view_position = padded_length - max_view_length;
		else if (view_position > cursor_position)	// Need to scroll left to find cursor.
			view_position = cursor_position;
		else if (view_position+max_view_length-1 < cursor_position)	// Need to scroll right to find the cursor.
			view_position = cursor_position-max_view_length+1;

		const size_t effective_view_position = (has_focus) ? view_position : 0;
		const size_t cursor_view_position = cursor_position-view_position;
		const auto content_view = std::string_view(&content[effective_view_position], std::min(content.length()-effective_view_position, max_view_length));

		parent.write_at(begin_x, begin_y, content_view, filled_style);
		
		if (max_view_length - content_view.length() > 0)
			parent.write_at(start_x+content_view.length(), start_y, std::string(max_view_length-content_view.length(), ' ', unfilled_style));

		if (has_focus)
			parent.set_cursor_position(start_x+cursor_view_position, start_y);

		parent.flush();
	};

	void textbox::cursor_left()
	{
		cursor_position -= (cursor_position > 0) ? 1 : 0;
		redraw();
	};

	void textbox::cursor_right()
	{
		cursor_position += (cursor_position < content.length()) ? 1 : 0;
		redraw();
	};

	void textbox::cursor_home()
	{
		cursor_position = 0;
		redraw();
	}

	void cursor_end();
	{
		cursor_position = content.length();
		redraw();
	}

	void textbox::set_focus(const bool focus)
	{
		if (focus == has_focus)
			return;

		has_focus = focus;

		if (!has_focus)
			parent.hide_cursor();

		redraw();
	}

	void textbox::accept_key_event(const key_event& event)
	{
		if (event.ctrl)
			return;
	
		if (const auto key = std::get_if<special_key>(&event->key))
		{
			switch(*key)
			{
				case special_key::enter:
					input_accepted = true;
					set_focus(false);
					return;

				case special_key::arrow_left:
					cursor_left();
					break;
				case special_key::arrow_right:
					cursor_right();
					break;
				case special_key::home:
					cursor_home();
					break;
				case special_key::end:
					cursor_end();
					break;

				case special_key::del:
					if (content.length() > 0 && cursor_position <= content.length())
					{
						content.erase(cursor_position, 1);
						redraw();
					}
					break;
				case special_key::backspace:
					if (content.length() > 0 && cursor_position > 0)
					{
						content.erase(cursor_position-1, 1);
						--cursor_position;
						redraw();
					}
					break;

				case special_key::insert:
					mode = (mode == insert_mode::insert) ? insert_mode::overwrite : insert_mode::insert;
					break;

				default:
					break;
			}
		}

		if ((content.length() >= max_content_length && mode == insert_mode::insert) || cursor_position >= max_content_length)
			return;

		if (const auto ch = std::get_if<char>(&event->key))
		{
			if (cursor_position == content.length())
				content.push_back(*ch);
			else if (mode == insert_mode::overwrite)
				content[cursor_position] = *ch;
			else // mode == insert_mode::insert
				content.insert(cursor_position, 1, *ch);
			++cursor_position;
		}

	}
	
	textbox::~textbox()
	{
		set_focus(false);
	}

} // End of namespace termwrap.

