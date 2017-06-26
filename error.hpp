//
// Termwrap
//
// termwrap/driver.hpp
//
// Copyright Dr Robert H Crowston, 2017. All rights reserved.
// See LICENCE for licensing rights.
//

#ifndef RHC_TERMWRAP_ERROR_H
#define RHC_TERMWRAP_ERROR_H

#include <stdexcept>
#include <string>

namespace termwrap
{
	//
	// System exceptions.
	// These are problems with the underlying terminal interface.
	//

	struct driver_system_error : public std::runtime_error
	{
		driver_system_error(const std::string& error) : std::runtime_error(error) { }
		virtual ~driver_system_error() { };
	};

	struct driver_unknown_error : public driver_system_error
	{
		driver_unknown_error() : driver_system_error("Unknown termbox error.") { }
	};

	struct unsupported_terminal_error : public driver_system_error
	{
		unsupported_terminal_error() : driver_system_error("Unsupported terminal.") { }
	};

	struct failed_to_open_terminal_error : public driver_system_error
	{
		failed_to_open_terminal_error() : driver_system_error("Could not open terminal.") { }
	};

	struct pipe_trap_error : public driver_system_error
	{
		pipe_trap_error() : driver_system_error("Pipe error communicating with signal handler.") { }
	};

	//
	// Failed termbox function calls.
	//
	
	struct failed_peek_poll_event : public driver_system_error
	{
		failed_peek_poll_event() : driver_system_error("Failed to peek or to poll an event.") { }
	};

	//
	// Drawing exceptions.
	// Used to signify draws that were not attempted, because the terminal is in the wrong mode or there is insufficient space.
	//

	struct driver_draw_error : public std::runtime_error
	{
		driver_draw_error(const std::string& error) : std::runtime_error(error) { }
		virtual ~driver_draw_error() { };
	};

	struct terminal_coord_invalid_error : public driver_draw_error
	{
		terminal_coord_invalid_error() : driver_draw_error("Specified terminal coordinates are not present on the terminal.") { }
	};

	struct text_overflow_error : public driver_draw_error
	{
		text_overflow_error() : driver_draw_error("Text would overflow terminal.") { }
	};

	struct mode_unsupported_error : public driver_draw_error
	{
		mode_unsupported_error() : driver_draw_error("Cannot set requested text mode.") { }
	};

	struct color_not_supported_error : public driver_draw_error
	{
		color_not_supported_error() : driver_draw_error("Color not supported in present mode.") { }
	};

} // End of namespace termwrap.

#endif // !RHC_TERMWRAP_ERROR_H.

