//
// Termwrap
//
// engine/element.hpp
//
// Copyright Dr Robert H Crowston, 2017. All rights reserved.
// See LICENCE for licensing rights.
//

// engine::element is a virtual class describing some element on the UI.

#ifndef RHC_TERMBOX_ENGINE_ELEMENT_H
#define RHC_TERMBOX_ENGINE_ELEMENT_H

#include "../driver/cell_style.hpp"
#include "../driver/driver.hpp"
#include "../driver/key_event.hpp"
#include "../driver/types.hpp"

namespace termwrap::engine
{
	class element
	{
		element() = default;
		~element() = 0;

		ordinate_t min_width() const noexcept = 0;
		ordinate_t preferred_width() const noexcept = 0;
		ordinate_t max_width() const noexcept = 0;

		ordinate_t min_height() const noexcept = 0;
		ordinate_t preferred_height() const noexcept = 0;
		ordinate_t max_height() const noexcept = 0;

		utf8_string_view visible_content(const ordinate_t width, const ordinate_t height) = 0;
		
		void accept_input(const native_char_t ch) { }
		
		void set_active() { }
		void set_inactive() { }

		ordinate_t cursor_position(const ordinate_t width, const ordinate_t height) { return 0; }
		bool cursor_visible() { return false; }
		
	};

} // End of namespace termwrap::engine.
	
#endif // !RHC_TERMBOX_ENGINE_ELEMENT_H.
