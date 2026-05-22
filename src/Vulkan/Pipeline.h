#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#include <filesystem>

#include "Swapchain.h"


class GraphicsPipeline {
public:
	void createGraphicsPipeline(LogicalDevice device, Swapchain swapchain);
	static std::vector<char> readFile(const std::string& filename);
    VkShaderModule createShaderModule(const std::vector<char>& code, LogicalDevice device);
	VkPipelineLayout getGrapicsPipelineLayout() {return pipelineLayout;}
	VkPipeline getGrapicsPipeline() {return Pipeline;}
		VkShaderModule getShaderModule() {return shaderModule;}
private:
	VkShaderModule shaderModule;
	VkPipelineLayout pipelineLayout;
	VkPipeline Pipeline;



};
