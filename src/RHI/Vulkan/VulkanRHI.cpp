#include "VulkanRHI.h"
#include <iostream>

VulkanRHI::VulkanRHI(): instance(instance)
{

}

    void VulkanRHI::InitVulkan(){ 
        std::cout<<"Iniciando instancia:"<<std::endl;
        instance.CreateInstances();
    };

    void VulkanRHI::DestroyVulkan(){ 
        instance.DestroyInstance();
        
    };
