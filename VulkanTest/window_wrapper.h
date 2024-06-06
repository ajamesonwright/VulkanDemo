#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace vd {

	class WindowWrapper {

	public:
		WindowWrapper(int p_width, int p_height, std::string p_name);
		WindowWrapper(const WindowWrapper&) = delete;
		WindowWrapper& operator=(const WindowWrapper&) = delete;
		~WindowWrapper();

		void initWindow();
		bool shouldClose() { return glfwWindowShouldClose(window); }
		GLFWwindow& getWindow() { return *window; };
		VkInstance& getVkInstance() { return instance; }
		VkSurfaceKHR& getVkSurface() { return surface; }

	private:
		const int width;
		const int height;
		std::string name;
		GLFWwindow* window = nullptr;
		VkInstance instance = VK_NULL_HANDLE;
		VkSurfaceKHR surface = VK_NULL_HANDLE;
	};
}
