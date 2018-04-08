#pragma once
#ifdef __APPLE__
	#include <OpenCL/opencl.hpp>
#else
	#include <CL/cl.hpp>
#endif
#include <string>

class Reaper {
private:
	inline std::string load_kernel(const char*);
};
