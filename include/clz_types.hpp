#pragma once

#include <string>
#include <fmt/base.h>

namespace clz
{
	struct Result
	{
		bool success;
		std::string message;

		Result()
		{
			success = true;
			message = ":p";
		}
		void what()
		{
			fmt::print("{}\n", message);
		}
	};

	inline Result Success()
	{
		Result result;
		result.success = true;
		result.message = ":p";
		return result;
	}

	inline Result Success(const std::string &msg)
	{
		Result result;
		result.success = true;
		result.message = msg;
		fmt::println("Success: {}", msg);
		return result;
	}
    inline Result Failure(const std::string &msg = "Unknown error")
    {
        Result result;
        result.success = false;
        result.message = msg;
        return result;
    }


	#define CHECK(clz_result) 	\
	if (!clz_result.success) 	\
	{			        \
	fmt::print("ERROR: "); \
	clz_result.what();	\
		return false;	        \
	}
	#define INVALIDATE(clz_result)	\
	if (!clz_result.success)	\
	{				\
	    fmt::print("ERROR: "); \
		clz_result.what();	\
		return clz_result;	\
	}
}
