#include "pipeline.h"

namespace vd {

	VulkanPipeline::VulkanPipeline(const std::string& vertPath, const std::string& fragPath) {
		createPipeline(vertPath, fragPath);
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

	void VulkanPipeline::createPipeline(const std::string& vertPath, const std::string& fragPath) {

		auto vertShaderContents = readFile(vertPath);
		auto fragShaderContents = readFile(fragPath);

		std::cout << "Vertex code size: " << vertShaderContents.size() << "\n";
		std::cout << "Fragment code size: " << fragShaderContents.size() << "\n";

	}
}