/**
 * @file clz_types.hpp
 * @brief Core result type and error-propagation utilities for Curloz Engine.
 *
 * Defines clz::Result, factory helpers (Success / Failure), and the
 * CHECK / INVALIDATE macros that form the engine's lightweight error
 * handling convention — no exceptions, no std::expected, just a bool
 * and a message.
 */

#pragma once

#include <string>
#include <fmt/base.h>

namespace clz
{
	/**
	 * @struct Result
	 * @brief Represents the outcome of a fallible engine operation.
	 *
	 * Carries a boolean success flag and an optional human-readable
	 * message. The default constructor produces a successful result.
	 * Use the Success() and Failure() factory functions rather than
	 * constructing Result directly.
	 */
	struct Result
	{
		bool success;         ///< true if the operation succeeded, false otherwise.
		std::string message;  ///< Human-readable status or error description.

		/**
		 * @brief Default-constructs a successful Result with a placeholder message.
		 */
		Result()
		{
			success = true;
			message = ":p";
		}

		/**
		 * @brief Prints the result message to stdout.
		 */
		void what()
		{
			fmt::print("{}\n", message);
		}
	};

	/**
	 * @brief Creates a silent success Result with the default placeholder message.
	 *
	 * Use when no diagnostic output is needed on the happy path.
	 *
	 * @return A Result with success = true.
	 */
	inline Result Success()
	{
		Result result;
		result.success = true;
		result.message = ":p";
		return result;
	}

	/**
	 * @brief Creates a success Result and prints the message to stdout.
	 *
	 * Use when you want to confirm a successful step with a log line.
	 *
	 * @param msg Descriptive message to store and print.
	 * @return A Result with success = true and message set to @p msg.
	 */
	inline Result Success(const std::string &msg)
	{
		Result result;
		result.success = true;
		result.message = msg;
		fmt::println("Success: {}", msg);
		return result;
	}

	/**
	 * @brief Creates a failure Result with an optional error message.
	 *
	 * Does not print anything — the CHECK / INVALIDATE macros handle
	 * printing at the call site.
	 *
	 * @param msg Error description. Defaults to "Unknown error".
	 * @return A Result with success = false and message set to @p msg.
	 */
	inline Result Failure(const std::string &msg = "Unknown error")
	{
		Result result;
		result.success = false;
		result.message = msg;
		return result;
	}

	/**
	 * @def CHECK(clz_result)
	 * @brief Early-returns false if @p clz_result indicates failure.
	 *
	 * Intended for use in functions that return bool. Prints "ERROR: "
	 * followed by the result message, then returns false. Has no effect
	 * if clz_result.success is true.
	 *
	 * @param clz_result A clz::Result value to inspect.
	 */
	#define CHECK(clz_result)		\
	if (!clz_result.success)		\
	{					\
		fmt::print("ERROR: ");		\
		clz_result.what();		\
		return false;			\
	}

	/**
	 * @def INVALIDATE(clz_result)
	 * @brief Early-returns the failing Result itself if it indicates failure.
	 *
	 * Intended for use in functions that return clz::Result. Prints
	 * "ERROR: " followed by the result message, then propagates the
	 * Result up the call stack. Has no effect if clz_result.success is true.
	 *
	 * @param clz_result A clz::Result value to inspect.
	 */
	#define INVALIDATE(clz_result)		\
	if (!clz_result.success)		\
	{					\
		fmt::print("ERROR: ");		\
		clz_result.what();		\
		return clz_result;		\
	}
}
