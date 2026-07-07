#pragma onces

#include "../Core/VulkanContext.h"
#include <vulkan/vulkan.h>
#include <string>

#include <fstream>
#include "../../../Assets/Loaders/LoaderAssets.h"

class VulkanPipeline {
public:
    VulkanPipeline(VulkanContext& context)
        : m_Context(context)
    {
    }

	void createGraphicsPipeline(VkFormat formatDepth);
    void CreateDescriptorSetLayout();
    void DestroyDescriptorSetLayout();

	static std::vector<char> readFile(const std::string& filename);
    VkShaderModule createShaderModule(const std::vector<char>& code);

private:


    VulkanContext& m_Context;


};
