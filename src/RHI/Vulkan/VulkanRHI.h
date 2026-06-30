#pragma once
#include "Core/VulkanContext.h"
#include "Instance/VulkanInstance.h"
#include "Instance/ValidationLayers.h"

class VulkanRHI {

    public:
    VulkanRHI();

    void InitVulkan();
    void DestroyVulkan();
    
    VulkanContext& GetVulkanContext() {return context;};
    VulkanInstance& GetVulkanInstance() {return instance;};
    VulkanValidation& GetVulkanValidation() {return validacionlayers;};

    private:
        VulkanContext context;
        VulkanInstance instance;
        VulkanValidation validacionlayers;
};