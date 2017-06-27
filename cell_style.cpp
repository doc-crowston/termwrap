//
// Termwrap
//
// termwrap/cell_style.cpp
//
// Copyright Dr Robert H Crowston, 2017. All rights reserved.
// See LICENCE for licensing rights.
//

#include "cell_style.hpp"
#include "error.hpp"
#include "/opt/termbox/include/termbox.h"

namespace termwrap
{
	static constexpr native_style_t to_native_color(const color& c)
	{
		switch (c)
		{
			case color::unset:
				return TB_DEFAULT;
			case color::white:
				return TB_WHITE;
			case color::black:
				return TB_BLACK;
			case color::red:
				return TB_RED;
			case color::green:
				return TB_GREEN;
			case color::yellow:
				return TB_YELLOW;
			case color::blue:
				return TB_BLUE;
			case color::magenta:
				return TB_MAGENTA;
			case color::cyan:
				return TB_CYAN;
			// ...
			default:
				throw color_not_supported_error();
		}
	}

	static constexpr color to_color(const native_style_t native_style)
	{
		switch(native_style & 0xff)
		{
			case TB_DEFAULT:
				return color::unset;
			case TB_WHITE:
				return color::white;
			case TB_BLACK:
				return color::black;
			case TB_RED:
				return color::red;
			case TB_GREEN:
				return color::green;
			case TB_YELLOW:
				return color::yellow;
			case TB_BLUE:
				return color::blue;
			case TB_MAGENTA:
				return color::magenta;
			case TB_CYAN:
				return color::cyan;
			// ...
			default:
				throw color_not_supported_error();
		}
		//__builtin_unreachable();
	}

	static constexpr native_style_t to_native_weight(const font_weight& w)
	{
		switch (w)
		{
			case font_weight::normal:
				return 0x0;
			case font_weight::bold:
				return TB_BOLD;
		}
		//__builtin_unreachable();
	}

	static constexpr native_style_t to_native_decoration(const text_decoration& d)
	{
		switch (d)
		{
			case text_decoration::normal:
				return 0x0;
			case text_decoration::underline:
				return TB_UNDERLINE;
		}
		//__builtin_unreachable();
	}
	
	cell_style::cell_style(const native_style_t fg, const native_style_t bg)
	{
		foreground = to_color(fg);
		background = to_color(bg);
		weight = font_weight::normal;
		decoration = text_decoration::normal;
	}

	native_style_t cell_style::to_native_fg() const 
	{
		return to_native_color(foreground) | to_native_weight(weight) | to_native_decoration(decoration);
	}

	native_style_t cell_style::to_native_bg() const
	{
		return to_native_color(background);
	}
} // End of namespace termwrap.

