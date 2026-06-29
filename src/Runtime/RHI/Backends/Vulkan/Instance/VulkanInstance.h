#pragma once

#include "../Core/VulkanContext.h"
#include "vector"
#include "../Resources/VulkanConstants.h"



class VulkanInstance {
public:

	void CreateInstances();
	void DestroyInstance();
	VkInstance GetInstance() {return m_Context.instance;};

private:
    VulkanContext& m_Context;

	void GetInstanceVersions();

	std::vector<const char*> GetInstanceExtensionsRequireds();

};