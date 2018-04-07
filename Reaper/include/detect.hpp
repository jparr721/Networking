#define CL_USE_DEPRECATED_OPENCL_2_0_APIS
#pragma once
#ifdef __APPLE__
	#include <OpenCL/opencl.h>
#else
	#include <CL/cl.hpp>
#endif

class Detect {
private:
	cl_context create_context();
public:
	void query_platforms();

};
