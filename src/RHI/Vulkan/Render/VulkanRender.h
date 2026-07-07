#pragma once
#include "../VulkanRHI.h"
#include <iostream>

class VulkanRender {

    public:
        VulkanRender(VulkanContext& context)
        : m_Context(context)              
    {
    }

    void drawFrame();

    private:
    VulkanContext& m_Context;




};


