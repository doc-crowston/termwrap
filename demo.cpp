#include <chrono>
#include <string>
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
		const std::string query_label = "Fare query";
		
		const unsigned start_x = 0+query_label.length()+2;
		const unsigned max_x   = start_x + 12;
		const unsigned start_y = 14, max_y = start_y;

		console.write_at(0, start_y, query_label);
		
		cell_style input_style;
		input_style.decoration = text_decoration::underline;
		input_style.background = color::cyan;
		input_style.foreground = color::black;

		console.set_block_style(start_x, start_y, max_x, max_y, input_style);
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

	using namespace std::chrono_literals;
	std::this_thread::sleep_for(15s);
}

