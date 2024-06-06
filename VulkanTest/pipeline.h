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
		void createGraphicsPipeline(const VkDevice device, const VkFormat format, const std::string& vertPath, const std::string& fragPath);
		void cleanUp();

	private:
		VkDevice logicalDevice = nullptr;
		VkFormat swapChainImageFormat;
		VkRenderPass renderPass;
		VkPipelineLayout pipelineLayout;
		VkPipeline pipeline;
		std::vector<VkFramebuffer> swapChainFrameBuffers;

		static std::vector<char> readFile(const std::string& filePath);
		void createRenderPass();
		void createGraphicsPipeline(const std::string& vertPath, const std::string& fragPath);
		void createFramebuffers();
		VkShaderModule createShaderModule(const std::vector<char>& code);
	};
}