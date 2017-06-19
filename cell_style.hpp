//
// Termwrap 
//
// termwrap/cell_style.hpp
//
// Copyright Dr Robert H Crowston, 2017. All rights reserved.
// See LICENCE for licensing rights.
//

#ifndef RHC_TERMWRAP_CELL_STYLE_H
#define RHC_TERMWRAP_CELL_STYLE_H

#include <cstdint>

namespace termwrap
{
	//
	// Cell style attributes.
	//

	enum class color
	{
		unset,
		white,
		black,
		red,
		green,
		yellow,
		blue,
		magenta,
		cyan
	};

	enum class font_weight
	{
		normal,
		bold
	};

	enum class text_decoration
	{
		normal,
		underline
	};

	using native_style_t = std::uint16_t;

	struct cell_style
	{
		cell_style() { }
		cell_style(const font_weight w) : weight(w) { }
		cell_style(const text_decoration d) : decoration(d) { }

		cell_style(const native_style_t fg, const native_style_t bg);
		
		color foreground {};
		color background {};
		font_weight weight {};
		text_decoration decoration {};

		native_style_t to_native_fg() const ;
		native_style_t to_native_bg() const ;
	};
} // End of namespace termwrap.

#endif // !RHC_TERMWRAP_CELL_STYLE_H

