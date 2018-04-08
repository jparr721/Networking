#define CL_USE_DEPRECATED_OPENCL_2_0_APIS
#include <fstream>
#include <iostream>
#include "../include/reaper.hpp"
#include "../include/detect.hpp"

inline std::string Reaper::load_kernel(const char* name) {
	std::ifstream kernel_file(name);
	std::string result((std::istreambuf_iterator<char> (kernel_file)), std::istreambuf_iterator<char>());	
	std::cout << "Kenel loaded successfully!" << std::endl;
	return result;
}

int main() {
	Detect d;
	d.query_platforms();
}
