#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include "vulkan_demo.h"

int main() {
	vd::VulkanDemo app{};

	try {
		app.run();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << "\n";
		return EXIT_FAILURE;
	}

	return 0;
}