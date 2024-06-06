#include "window_wrapper.h"

namespace vd {
	WindowWrapper::WindowWrapper(int p_width, int p_height, std::string p_name) : width{ p_width }, height{ p_height }, name{ p_name } {
		//initWindow();
	}

	WindowWrapper::~WindowWrapper() {
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	void WindowWrapper::initWindow() {
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
		glfwSetWindowUserPointer(window, this);
	}
}
