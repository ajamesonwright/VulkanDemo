#pragma once

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <vector>

#include "window_wrapper.h"
#include "pipeline.h"

namespace vd {

	class VulkanDemo {

	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;
		static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

		VulkanDemo();
		~VulkanDemo();
		void run();
		VulkanPipeline* getPipeline() { return &pipeline; };

	private:
		const std::string VERT = "shaders/shader.vert.spv";
		const std::string FRAG = "shaders/shader.frag.spv";
		const std::string MODEL_PATH = "models/viking_room.obj";
		const std::string TEXTURE_PATH = "textures/viking_room.png";

		WindowWrapper window{ WIDTH, HEIGHT, "Vulkan Demo" };
		VulkanPipeline pipeline;
		VkDebugUtilsMessengerEXT debugMessenger;

		bool checkValidationLayerSupport();
		std::vector<const char*> getRequiredExtensions();
		static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* callbackData, void* userData);
		VkResult createDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* createInfo, const VkAllocationCallbacks* allocator, VkDebugUtilsMessengerEXT* debugMessenger);
		void destroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* allocator);
		void initWindow();
		void initVulkan();
		void createSurface();
		void createPipeline();
		void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		void configureDebugMessenger();
	};

}

