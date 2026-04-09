/**
 * @file base.hpp
 * @brief Core engine lifecycle interface for Curloz Engine.
 *
 * Exposes the three main lifecycle functions — init, update, and cleanup —
 * that drive the engine's startup, main loop, and shutdown sequence.
 */

#pragma once

namespace clz
{
	/**
	 * @brief Initializes all internal engine subsystems.
	 *
	 * Brings up the window and renderer in order. If any subsystem fails
	 * to initialize, the function returns false immediately.
	 *
	 * @return true if all subsystems initialized successfully, false otherwise.
	 */
	bool init();

	/**
	 * @brief Runs the main engine loop until the window signals a close.
	 *
	 * Ticks the window and renderer each frame in sequence. Blocks until
	 * window::windowShouldClose() returns true.
	 */
	void update();

	/**
	 * @brief Shuts down all internal engine subsystems in reverse init order.
	 *
	 * Cleans up the renderer first, then the window. Safe to call once after
	 * update() returns.
	 */
	void cleanup();
}
