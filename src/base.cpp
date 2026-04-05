#include "base.hpp"
#include "renderer/renderer.hpp"
#include "window/window.hpp"
#include "clz_types.hpp"
#include <fmt/base.h>

namespace clz
{
	bool init()
	{
		CHECK(window::init());

		CHECK(render::init());
		fmt::print("Initialized renderer\n");

		fmt::print("Initialized all internal engines\nWelcome to curloz engine\n");
		return true;
	}

	void update()
	{
	    while (!window::windowShouldClose())
	    {
	        window::update(0);
	        render::update(0);
	    }
	}

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
