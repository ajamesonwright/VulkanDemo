#pragma once

#include "window_wrapper.h"
#include "pipeline.h"

namespace vd {
	class VulkanDemo {

	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		void run();

	private:
		WindowWrapper window{WIDTH, HEIGHT, "Vulkan Demo"};
		VulkanPipeline pipeline{ "shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv" };
	};

}

