#pragma once
#include "Core/VulkanContext.h"
#include "Instance/VulkanInstance.h"

class VulkanRHI {

    public:
        VulkanRHI();

    void InitVulkan();
    void DestroyVulkan();
    
    VulkanContext& GetVulkanContext() {return context;};
    VulkanInstance& GetVulkanInstance() {return instance;};

    private:
        VulkanContext context;

        VulkanInstance instance;

};