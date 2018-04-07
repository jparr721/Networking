#pragma once
#ifdef __APPLE__
	#include <OpenCL/opencl.hpp>
#else
	#include <CL/cl.hpp>
#endif
#include <string>

class Reaper {
private:
	inline bool load_kernel(std::string);

};
