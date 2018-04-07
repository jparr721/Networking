#define CL_USE_DEPRECATED_OPENCL_2_0_APIS
#include <fstream>
#include <iostream>
#include "../include/reaper.hpp"
#include "../include/detect.hpp"

inline bool Reaper::load_kernel(std::string kernel_file_name) {
	std::ifstream kernel_file(kernel_file_name);
	
}

int main() {
	Detect d;
	d.query_platforms();
}
