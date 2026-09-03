#pragma onces

#include "../Core/VulkanContext.h"
#include <vulkan/vulkan.h>
#include <string>

#include <fstream>
#include "../../../Assets/Loaders/LoaderAssets.h"

class VulkanPipeline {
public:
    VulkanPipeline();
~VulkanPipeline();

	void createGraphicsPipeline(VkDevice device,VkExtent2D swapChainExtent,VkSurfaceFormatKHR  swapChainSurfaceFormat, VkDescriptorSetLayout descriptorsetlayout);
    void DestroyPipelineGraphics();
    
	static std::vector<char> readFile(const std::string& filename);
    VkShaderModule createShaderModule(const std::vector<char>& code);

    void recreateGraphicsPipeline();
	VkPipeline GetPipeline() {return Pipeline;};
	VkPipelineLayout GetPipelineLeyout() {return pipelineLayout;};


private:
	VkShaderModule shaderModule;
	VkPipelineLayout pipelineLayout;
	VkPipeline Pipeline;

VkDevice cp_device;
VkExtent2D cp_swapChainExtent;
VkSurfaceFormatKHR  cp_swapChainSurfaceFormat;
VkDescriptorSetLayout cp_descriptorsetlayout;
};
