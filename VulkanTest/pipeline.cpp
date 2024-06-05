#include "pipeline.h"

namespace vd {

	/*VulkanPipeline::VulkanPipeline(const VkDevice& pLogicalDevice, const std::string& vertPath, const std::string& fragPath) {
		logicalDevice = pLogicalDevice;
		createPipeline(vertPath, fragPath);
	}*/

	//VulkanPipeline::~VulkanPipeline() {
		//vkDestroyPipelineLayout(logicalDevice, pipelineLayout, nullptr);
	//}

	void VulkanPipeline::createGraphicsPipeline(const VkDevice& pLogicalDevice, const VkFormat& pSwapChainImageFormat, const std::string& vertPath, const std::string& fragPath) {
		logicalDevice = pLogicalDevice;
		swapChainImageFormat = pSwapChainImageFormat;
		createRenderPass();
		createGraphicsPipeline(vertPath, fragPath);
	}

	void VulkanPipeline::cleanUp() {
		if (logicalDevice) {
			vkDestroyPipelineLayout(logicalDevice, pipelineLayout, nullptr);
			vkDestroyRenderPass(logicalDevice, renderPass, nullptr);
		}
	}

	std::vector<char> VulkanPipeline::readFile(const std::string& filePath) {

		// Open file from given path using binary format, jump to end
		std::ifstream file{ filePath, std::ios::ate | std::ios::binary };

		if (!file.good()) {
			std::cout << "Trying to read from file: " << filePath << std::endl;
			throw std::runtime_error("Error while opening file " + filePath);
		}
		if (!file.is_open()) {
			throw std::runtime_error("Error while opening file " + filePath);
		}

		// Calculate total buffer size required
		size_t fileSize = static_cast<size_t>(file.tellg());

		// Create buffer
		std::vector<char> fileBuffer(fileSize);

		// Jump back to start and read in entirety
		file.seekg(0);
		file.read(fileBuffer.data(), fileSize);

		// Close file and return contents
		file.close();
		return fileBuffer;
	}

	void VulkanPipeline::createRenderPass() {
		VkAttachmentDescription colourAttachment{};
		colourAttachment.format = swapChainImageFormat;
		colourAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colourAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colourAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colourAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colourAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colourAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colourAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference colorAttachmentRef{};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;

		VkRenderPassCreateInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = 1;
		renderPassInfo.pAttachments = &colourAttachment;
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;

		if (vkCreateRenderPass(logicalDevice, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create render pass!");
		}
	}

	void VulkanPipeline::createGraphicsPipeline(const std::string& vertPath, const std::string& fragPath) {
		auto vertShaderContents = readFile(vertPath);
		auto fragShaderContents = readFile(fragPath);

		std::cout << "Vertex code size: " << vertShaderContents.size() << "\n";
		std::cout << "Fragment code size: " << fragShaderContents.size() << "\n";

		VkShaderModule vertShaderModule = createShaderModule(vertShaderContents);
		VkShaderModule fragShaderModule = createShaderModule(fragShaderContents);

		VkPipelineShaderStageCreateInfo vertShaderStageCreateInfo{};
		vertShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		vertShaderStageCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
		vertShaderStageCreateInfo.module = vertShaderModule;
		vertShaderStageCreateInfo.pName = "main";

		VkPipelineShaderStageCreateInfo fragShaderStageCreateInfo{};
		fragShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		fragShaderStageCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
		fragShaderStageCreateInfo.module = fragShaderModule;
		fragShaderStageCreateInfo.pName = "main";

		VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageCreateInfo, fragShaderStageCreateInfo };

		VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo{};
		vertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputStateCreateInfo.vertexBindingDescriptionCount = 0;
		vertexInputStateCreateInfo.vertexAttributeDescriptionCount = 0;

		VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo{};
        inputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssemblyStateCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssemblyStateCreateInfo.primitiveRestartEnable = VK_FALSE;

		VkPipelineViewportStateCreateInfo viewportStateCreateInfo{};
		viewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportStateCreateInfo.viewportCount = 1;
		viewportStateCreateInfo.scissorCount = 1;

		VkPipelineRasterizationStateCreateInfo rasterizerStateCreateInfo{};
		rasterizerStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizerStateCreateInfo.depthClampEnable = VK_FALSE;
		rasterizerStateCreateInfo.rasterizerDiscardEnable = VK_FALSE;
		rasterizerStateCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizerStateCreateInfo.lineWidth = 1.0f;
		rasterizerStateCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterizerStateCreateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
		rasterizerStateCreateInfo.depthBiasEnable = VK_FALSE;

		VkPipelineMultisampleStateCreateInfo multisamplingStateCreateInfo{};
		multisamplingStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisamplingStateCreateInfo.sampleShadingEnable = VK_FALSE;
		multisamplingStateCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

		VkPipelineColorBlendAttachmentState colorBlendAttachment{};
		colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachment.blendEnable = VK_FALSE;

		VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo{};
		colorBlendStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlendStateCreateInfo.logicOpEnable = VK_FALSE;
		colorBlendStateCreateInfo.logicOp = VK_LOGIC_OP_COPY;
		colorBlendStateCreateInfo.attachmentCount = 1;
		colorBlendStateCreateInfo.pAttachments = &colorBlendAttachment;
		colorBlendStateCreateInfo.blendConstants[0] = 0.0f;
		colorBlendStateCreateInfo.blendConstants[1] = 0.0f;
		colorBlendStateCreateInfo.blendConstants[2] = 0.0f;
		colorBlendStateCreateInfo.blendConstants[3] = 0.0f;

		std::vector<VkDynamicState> dynamicStates = {
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_SCISSOR
		};
		VkPipelineDynamicStateCreateInfo dynamicState{};
		dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
		dynamicState.pDynamicStates = dynamicStates.data();

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pushConstantRangeCount = 0;

		if (vkCreatePipelineLayout(logicalDevice, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create pipeline layout!");
		}

		vkDestroyShaderModule(logicalDevice, vertShaderModule, nullptr);
		vkDestroyShaderModule(logicalDevice, fragShaderModule, nullptr);
	}


	VkShaderModule VulkanPipeline::createShaderModule(const std::vector<char>& code) {
		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

		VkShaderModule shaderModule;
		if (vkCreateShaderModule(logicalDevice, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create shader module!");
		}

		return shaderModule;
	}
}