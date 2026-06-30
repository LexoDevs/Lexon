#include "VulkanRHI.h"
#include <iostream>

VulkanRHI::VulkanRHI()
    : instance(context),
      validacionlayers(context)
{
}
    void VulkanRHI::InitVulkan(){ 
        instance.CreateInstances();
        validacionlayers.CreateDebugUtilsMessengerEXT(context,nullptr);
    };

    void VulkanRHI::DestroyVulkan(){ 

        validacionlayers.DestroyDebugUtilsMessengerEXT(context,nullptr);
        instance.DestroyInstance();
        
    };
