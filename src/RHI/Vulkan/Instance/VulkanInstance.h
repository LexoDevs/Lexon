#pragma once

#include "vector"
#include "../Helpers/VulkanConstants.h"

#include <vulkan/vulkan.h>
#include "ValidationLayers.h"


class VulkanInstance {
public:
    VulkanInstance();
	~VulkanInstance();

	void CreateInstances();
	void DestroyInstance();
	VkInstance GetInstance() {return instance;};
private: 

	void GetInstanceVersions();
	std::vector<const char*> GetInstanceExtensionsRequireds();

	VkInstance instance;
	VulkanValidation m_Validation;

	bool m_Initialized = false;
};