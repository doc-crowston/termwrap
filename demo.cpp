#include <algorithm>
#include <cassert>
#include <chrono>
#include <string>
#include <string_view>
#include <thread>

#include "driver.hpp"

using namespace termwrap;

class tui
{
	driver console{};

	public:
	void title_bar()
	{
		cell_style style {};
		style.weight = font_weight::bold;
		console.write_at(0,0,  "Fare Analytics Tool", style);
		console.write_at(22,0, "- Fare summary");
	}

	void status_bar()
	{
		console.write_at(1,console.console_height()-1,
			"Online    localhost    143,347,741,925 fares    98,231 candidates    17 excluded    Last parse Jun 17 17:08    Next update Jun 17 18:08    No changes"
		);
	}
	void menu_bar()
	{
		cell_style style{}; style.foreground = color::cyan;
		console.write_at(0,1, "F1 Rank    F2 Changes     F3 Inspector    F4 Excludes list", style);
		for (unsigned x = 0; x < (90-32); x++)
			if ( (x < 2) || (x > 10 && x < 13) || (x > 25 && x < 28) || (x > 41 && x < 44) )
				console.set_cell_style(x, 1, text_decoration::underline);
		for (unsigned x = 29; x < 38; x++)
			console.set_cell_style(x, 1, font_weight::bold);
	}
	void inspector()
	{
		const std::string fare_rules = "  V FARE BASIS     BK    FARE   TRAVEL-TICKET AP  MINMAX  RTG\n  1   IENJSB         I   320.00     ----     21/1 SU/12M EH01\nPASSENGER TYPE-ADT                 AUTO PRICE-YES\nFROM-MAD TO-LON    CXR-IB    TVL-15OCT17  RULE-M602 IPREURP/21\nFARE BASIS-IENJSB            SPECIAL FARE  DIS-E   VENDOR-ATP\nFARE TYPE-BXR      RT-2ND LEVEL BUSINESS CLASS EXCURSION\nEUR   285.00  5000  E18OCT16 D-INFINITY   FC-IENJSB  FN-\nSYSTEM DATES - CREATED 17OCT16/1315  EXPIRES INFINITY\n.";
		console.write_block_at(1,3, fare_rules);

		cell_style highlight_style {};
		highlight_style.weight = font_weight::bold;
		highlight_style.foreground = color::green;

		console.set_block_style(1,4, 81,4, highlight_style);
		console.set_block_style(46,5, 49,5, highlight_style);
		console.set_block_style(6,6, 8,6, highlight_style);
		console.set_block_style(13,6, 15,6, highlight_style);
		console.set_block_style(24,6, 25,6, highlight_style);
	}
	void flush()
	{
		console.redraw();
	}

	void text_box()
	{
		using std::size_t;

		const std::string query_label = "Fare query";

		const size_t max_input_length = 25;
		const size_t max_view_length = 22;

		const unsigned start_x = 0+query_label.length()+2;
		const unsigned max_x   = start_x + max_view_length - 1;
		const unsigned start_y = 14, max_y = start_y;

		console.write_at(0, start_y, query_label);

		cell_style input_style;
		input_style.decoration = text_decoration::underline;
		input_style.background = color::cyan;
		input_style.foreground = color::black;

		console.set_block_style(start_x, start_y, max_x, max_y, input_style);

		flush();

		{
			enum class insert_mode { overwrite, insert };
			insert_mode mode = insert_mode::insert;

			std::string input{};
			std::size_t view_position = 0;
			std::size_t cursor_position = 0;

			auto redraw_input = [&]
			{
				cursor_position = std::min( {input.length(), max_input_length-1, cursor_position} );

				const size_t pad_right = (input.length() < max_input_length) ? 1 : 0;
				const size_t padded_length = input.length() + pad_right;

				if (padded_length < max_view_length)	// Too little text; no scrolling.
					view_position = 0;
				else if (view_position > padded_length-max_view_length)	// Scrolled too far to the right.
					view_position = padded_length - max_view_length;
				else if (view_position > cursor_position)	// Need to scroll left to find cursor.
					view_position = cursor_position;
				else if (view_position+max_view_length-1 < cursor_position)	// Need to scroll right to find the cursor.
					view_position = cursor_position-max_view_length+1;

				//if (cursor_position-view_position

				const size_t cursor_view_position = cursor_position-view_position;

				const auto input_view = std::string_view(&input[view_position], std::min(input.length()-view_position, max_view_length));

				console.write_at(start_x, start_y, input_view);
				if (max_view_length - input_view.length() > 0)
					console.write_at(start_x+input_view.length(), start_y, std::string(max_view_length-input_view.length(), ' '));

				console.set_cursor_position(start_x+cursor_view_position, start_y);
				flush();
			};

			auto cursor_left = [&]
			{
				cursor_position -= (cursor_position > 0) ? 1 : 0;
				redraw_input();
			};

			auto cursor_right = [&]
			{
				cursor_position += (cursor_position < input.length()) ? 1 : 0;
				redraw_input();
			};

			while(true)
			{
				redraw_input();

				const auto event = [&] () -> std::optional<key_event>
				{
					try
					{
						using namespace std::chrono_literals;
						return console.wait_for_key_event(5s);
					}
					catch (const utf8_codepoint_too_wide_error& )
					{
						return {};
					}
				}();

				if (!event)
					continue;

				if (event->ctrl)
				{
					if (const auto ch = std::get_if<char>(&event->key))
					{
						if (*ch == 'C')
							return;
					}
					continue;
				}

				if (const auto key = std::get_if<special_key>(&event->key))
				{
					switch(*key)
					{
						case special_key::enter:
							return;

						case special_key::arrow_left:
							cursor_left();
							break;
						case special_key::arrow_right:
							cursor_right();
							break;
						case special_key::home:
							cursor_position = 0;
							redraw_input();
							break;
						case special_key::end:
							cursor_position = input.length();
							redraw_input();
							break;

						case special_key::del:
							if (input.length() > 0 && cursor_position <= input.length())
							{
								input.erase(cursor_position, 1);
								redraw_input();
							}
							break;
						case special_key::backspace:
							if (input.length() > 0 && cursor_position > 0)
							{
								input.erase(cursor_position-1, 1);
								--cursor_position;
								redraw_input();
							}
							break;

						case special_key::insert:
							mode = (mode == insert_mode::insert) ? insert_mode::overwrite : insert_mode::insert;
							break;

						default:
							break;
					}
				}

				if ((input.length() >= max_input_length && mode == insert_mode::insert) || cursor_position >= max_input_length)
					continue;

				if (const auto ch = std::get_if<char>(&event->key))
				{
					if (cursor_position == input.length())
						input.push_back(*ch);
					else if (mode == insert_mode::overwrite)
						input[cursor_position] = *ch;
					else // mode == insert_mode::insert
						input.insert(cursor_position, 1, *ch);
					++cursor_position;
				}

			}
			console.hide_cursor();
			flush();
		}
	}
};

int main()
{

	tui ui;
	ui.title_bar();
	ui.status_bar();
	ui.menu_bar();
	ui.inspector();
	ui.flush();

	ui.text_box();
	ui.flush();

	//using namespace std::chrono_literals;
	//std::this_thread::sleep_for(15s);
}

