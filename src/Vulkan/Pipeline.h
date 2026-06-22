#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#include <filesystem>

#include "Swapchain.h"

#include "../LoaderAssets.h"

class GraphicsPipeline {
public:
	void createGraphicsPipeline(LogicalDevice device, Swapchain swapchain, VkFormat formatDepth);
    void CreateDescriptorSetLayout(LogicalDevice logicaldevice);
    void DestroyDescriptorSetLayout(LogicalDevice logicaldevice);

	static std::vector<char> readFile(const std::string& filename);
    VkShaderModule createShaderModule(const std::vector<char>& code, LogicalDevice device);
	VkPipelineLayout getGrapicsPipelineLayout() {return pipelineLayout;}
	VkPipeline& getGrapicsPipeline() {return Pipeline;}
		VkShaderModule getShaderModule() {return shaderModule;}
VkDescriptorSetLayout getdescriptorSetLayout(){ return descriptorSetLayout;}
private:
	VkShaderModule shaderModule;
	VkPipelineLayout pipelineLayout;
	VkPipeline Pipeline;
    VkDescriptorSetLayout descriptorSetLayout;



};
