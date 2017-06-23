//
// Termwrap
//
// termwrap/key_event.hpp
//
// Copyright Dr Robert H Crowston, 2017. All rights reserved.
// See LICENCE for licensing rights.
//

#ifndef RHC_TERMWRAP_KEY_EVENT_H
#define RHC_TERMWRAP_KEY_EVENT_H

namespace termwrap
{
	enum class special_key
	{
		F1,
		F2,
		F3,
		F4,
		F5,
		F6,
		F7,
		F8,
		F9,
		F10,
		F11,
		F12,
		insert,
		del,
		home,
		end,
		page_up,
		page_down,
		arrow_up,
		arrow_down,
		arrow_left,
		arrow_right,
		backspace,
		escape,
		enter,
		tab
	};

	struct key_event
	{
		key_event(const native_char_t key, const bool ctrl = false, const bool alt = false)
			: key(key), ctrl(ctrl), alt(alt)
		{ }
		key_event(const special_key key, const bool ctrl = false, const bool alt = false)
			: key(key), ctrl(ctrl), alt(alt)
		{ }
		key_event(const key_event other&) = default;
		key_event operator= (const key_event other&) = default;

		special_key skey;
		native_char_t key;

		bool ctrl;
		bool alt;
	};
}

#endif // !RHC_TERMWRAP_KEY_EVENT_H.

