#include "base.hpp"

int main()
{
	if (!clz::init())
		return 1;
	
	clz::update();
	clz::cleanup();
	
	return 0;
}
