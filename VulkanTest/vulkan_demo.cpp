#include "vulkan_demo.h"

namespace vd {
	void VulkanDemo::run() {
		while (!window.shouldClose()) {
			glfwPollEvents();
		}
	}
}