#pragma once

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>
#include <array>
#include <chrono>
#include <iostream>
#include <fstream>
#include <optional>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

#include "window_wrapper.h"

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

struct QueueFamilyIndices {
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete() {
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

struct Vertex {
	glm::vec2 pos;
	glm::vec3 colour;

	static VkVertexInputBindingDescription getBindingDescription() {
		VkVertexInputBindingDescription bindingDescription{};
		bindingDescription.binding = 0;
		bindingDescription.stride = (sizeof(Vertex));
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}

	static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
		std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};
		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, pos);

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, colour);
		
		return attributeDescriptions;
	}
};

struct UniformBufferObject {
	alignas(16) glm::mat4 model;
	alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 proj;
};

namespace vd {
	class VulkanPipeline {
	public:
		const int MAX_FRAMES_IN_FLIGHT = 2;

		void createGraphicsPipeline(WindowWrapper& window, const std::string& vertPath, const std::string& fragPath);
		void cleanUp();
		void drawFrame();
		void setFramebufferResized();

	private:
		const std::vector<Vertex> vertices = {
			{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
			{{0.5f, -0.5f}, {1.0f, 1.0f, 0.0f}},
			{{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
			{{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
		};

		const std::vector<uint16_t> indices = {
			0, 1, 2, 2, 3, 0
		};

		WindowWrapper* window = nullptr;
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
		VkDevice logicalDevice = VK_NULL_HANDLE;
		VkSurfaceKHR surface;
		VkQueue graphicsQueue;
		VkQueue presentQueue;
		VkSwapchainKHR swapChain;
		std::vector<VkImage> swapChainImages;
		VkFormat swapChainImageFormat;
		VkExtent2D swapChainImageExtent;
		std::vector<VkImageView> swapChainImageViews;
		VkRenderPass renderPass;
		VkDescriptorSetLayout descriptorSetLayout;
		VkPipelineLayout pipelineLayout;
		VkPipeline pipeline;
		std::vector<VkFramebuffer> swapChainFrameBuffers;
		VkCommandPool commandPool;
		VkImage textureImage;
		VkDeviceMemory textureImageMemory;
		VkImageView textureImageView;
		VkSampler textureSampler;
		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;
		VkBuffer indexBuffer;
		VkDeviceMemory indexBufferMemory;
		std::vector<VkBuffer> uniformBuffers;
		std::vector<VkDeviceMemory> uniformBuffersMemory;
		std::vector<void*> uniformBuffersMapped;
		VkDescriptorPool descriptorPool;
		std::vector<VkDescriptorSet> descriptorSets;
		std::vector<VkCommandBuffer> commandBuffers;
		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VkFence> inFlightFences;
		uint32_t currentFrame = 0;
		bool framebufferResized = false;

		void selectPhysicalDevice();
		bool isDeviceSuitable(VkPhysicalDevice device);
		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
		bool checkDeviceExtensionSupport(VkPhysicalDevice device);
		SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
		VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR chooseSwapSurfacePresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
		VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
		void createLogicalDevice();
		void createSwapChain();
		void recreateSwapChain();
		void cleanUpSwapChain();
		void createImageViews();
		VkImageView createImageView(VkImage image, VkFormat format);
		void createRenderPass();
		void createDescriptorSetLayout();
		void createGraphicsPipeline(const std::string& vertPath, const std::string& fragPath);
		void createFramebuffers();
		void createCommandPool();
		void createTextureImage();
		void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
		void createTextureImageView();
		void createTextureSampler();
		void createVertexBuffer();
		void createIndexBuffer();
		void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
		void copyBuffer(VkBuffer src, VkBuffer dst, VkDeviceSize size);
		void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
		VkCommandBuffer beginSingleTimeCommands();
		void endSingleTimeCommands(VkCommandBuffer commandBuffer);
		void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
		void createSyncObjects();
		void createUniformBuffers();
		void createDescriptorPool();
		void createDescriptorSets();
		void updateUniformBuffer(uint32_t frameIndex);
		uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
		void createCommandBuffers();
		void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
		VkShaderModule createShaderModule(const std::vector<char>& code);
		static std::vector<char> readFile(const std::string& filePath);
	};
}