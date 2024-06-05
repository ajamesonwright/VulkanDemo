#pragma once

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace vd {
	class VulkanPipeline {
	public:
		//VulkanPipeline();
		//~VulkanPipeline();
		//const VkDevice& logicalDevice, const std::string& vertPath, const std::string& fragPath);
		void createGraphicsPipeline(const VkDevice& logicalDevice, const VkFormat& swapChainImageFormat, const std::string& vertPath, const std::string& fragPath);
		void cleanUp();

	private:
		VkDevice logicalDevice = nullptr;
		VkFormat swapChainImageFormat;
		VkRenderPass renderPass;
		VkPipelineLayout pipelineLayout;

		void createRenderPass();
		void createGraphicsPipeline(const std::string& vertPath, const std::string& fragPath);
		static std::vector<char> readFile(const std::string& filePath);
		VkShaderModule createShaderModule(const std::vector<char>& code);
	};
}