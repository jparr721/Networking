#include <iostream>
#include <vector>
#include "../include/detect.hpp"

void Detect::query_platforms() {
	std::vector<cl::Platform> platforms;
	cl::Platform::get(&platforms);

	auto platform = platforms.front();
	std::vector<cl::Device> devices;
	platform.getDevices(CL_DEVICE_TYPE_GPU, &devices); // Force use of the GPU

	std::cout << "Found " << devices.size() << " devices" << std::endl;
	std::cout << "Listing found devices and versions..." << std::endl;
	for (int i = 0; i < devices.size(); ++i) {
			auto device = devices[i];
			auto vendor = device.getInfo<CL_DEVICE_VENDOR>();
			auto version = device.getInfo<CL_DEVICE_VERSION>();
			std::cout << "Vendor: " << vendor << " Version: " << version << std::endl;
	}

}

