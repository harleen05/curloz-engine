/**
 * @file base.cpp
 * @brief Implementation of the core engine lifecycle functions.
 *
 * Drives initialization, the main loop, and shutdown for all internal
 * subsystems (window and renderer). Entry point for engine bring-up is
 * clz::init(), followed by clz::update(), then clz::cleanup().
 */

#include "base.hpp"
#include "renderer/renderer.hpp"
#include "window/window.hpp"
#include "clz_types.hpp"
#include <fmt/base.h>

namespace clz
{
	/**
	 * @brief Initializes the window and renderer subsystems.
	 *
	 * Calls window::init() first, then render::init(). Uses the CHECK macro
	 * to bail out early if either step fails. Prints confirmation to stdout
	 * on success.
	 *
	 * @return true if all subsystems came up cleanly, false on any failure.
	 */
	bool init()
	{
		CHECK(window::init());

		CHECK(render::init());
		fmt::print("Initialized renderer\n");

		fmt::print("Initialized all internal engines\nWelcome to curloz engine\n");
		return true;
	}

	/**
	 * @brief Runs the engine's main loop.
	 *
	 * Pumps window events and renderer updates every frame. Runs until
	 * window::windowShouldClose() signals that the window has been closed
	 * or a quit event was received.
	 *
	 * @note The delta-time argument passed to window::update() and
	 *       render::update() is currently hardcoded to 0 (placeholder).
	 */
	void update()
	{
	    while (!window::windowShouldClose())
	    {
	        window::update(0);
	        render::update(0);
	    }
	}

	/**
	 * @brief Tears down all engine subsystems in reverse initialization order.
	 *
	 * Shuts down the renderer before the window to avoid use-after-free of
	 * any window-backed rendering resources. Prints shutdown messages to
	 * stdout for each step.
	 */
	void cleanup()
	{
		render::cleanup();
		fmt::println("Closed renderer");
		window::cleanup();
		fmt::println("Closed window");

		fmt::println("Closed all internal engines");
		fmt::println("Goodbye, See you soon :)");
	}
}
