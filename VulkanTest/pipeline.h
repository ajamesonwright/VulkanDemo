#pragma once

#include <string>
#include <vector>

namespace vd {
	class VulkanPipeline {
	public:
		VulkanPipeline(const std::string& vertPath, const std::string& fragPath);

	private:
		void createPipeline(const std::string& vertPath, const std::string& fragPath);
		static std::vector<char> readFile(const std::string& filePath);
	};
}