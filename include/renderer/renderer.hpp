#pragma once

#include "clz_types.hpp"

namespace clz
{
	namespace render
	{
		clz::Result init();
		clz::Result update(float deltaTime);
		void cleanup();
	}
}
