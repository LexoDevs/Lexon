#include "VulkanLogDevice.h"

#include <set>
#include <iostream>
#include "../Resources/VulkanConstants.h"


void VulkanLogicalDevice::CreateLogicalDevice() {

	QueueFamilyIndices indices = findQueueFamilies();

	m_Context.QueueFamilie = indices;

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

	float queuePriority = 0.5f;
	for (uint32_t queueFamily : uniqueQueueFamilies) {
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkPhysicalDeviceFeatures deviceFeatures{};
	deviceFeatures.geometryShader = VK_TRUE;
	deviceFeatures.tessellationShader = VK_TRUE;
	deviceFeatures.samplerAnisotropy = VK_TRUE;

	VkPhysicalDeviceDescriptorIndexingFeatures DescriptorSetIndexingFeatures{};
	DescriptorSetIndexingFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES;


	VkPhysicalDeviceDynamicRenderingFeaturesKHR DynamicRenderingFeature = {};
	DynamicRenderingFeature.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES_KHR;
	DynamicRenderingFeature.pNext = NULL;
	DynamicRenderingFeature.dynamicRendering = VK_TRUE;

	VkPhysicalDeviceSynchronization2FeaturesKHR Synchronization2Feature = {};
	Synchronization2Feature.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SYNCHRONIZATION_2_FEATURES_KHR;
	Synchronization2Feature.synchronization2 = VK_TRUE;

	DynamicRenderingFeature.pNext = &Synchronization2Feature;
	Synchronization2Feature.pNext = nullptr;

	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pNext = &DynamicRenderingFeature;
	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.pEnabledFeatures = &deviceFeatures;

	createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size()); //requiredExtensions
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();

	if (enableValidationLayers) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else {
		createInfo.enabledLayerCount = 0;
	}

	bool DeviceSupportsDynamicRendering = m_Context.physicalDevice;

	if (vkCreateDevice(m_Context.physicalDevice, &createInfo, nullptr, &m_Context.device) != VK_SUCCESS) {
		throw std::runtime_error("failed to create logical device!");
	}
	std::cout << "\t\033[1;32mLogical device \033[0mguardado en \033[1;32m" << m_Context.device << "\033[0m\n\n";

	vkGetDeviceQueue(m_Context.device, indices.graphicsFamily.value(), 0, &m_Context.graphicsQueue);

	vkGetDeviceQueue(m_Context.device , indices.presentFamily.value(), 0, &m_Context.transferQueue);


};

    void VulkanLogicalDevice::DestroyLogicalDevice(){
		
	};


QueueFamilyIndices VulkanLogicalDevice::findQueueFamilies() {
	std::cout << "\033[1;36m[!] Buscando familia de colas...\033[0m\n";

	QueueFamilyIndices indices;

	uint32_t count;
	vkGetPhysicalDeviceQueueFamilyProperties(m_Context.physicalDevice, &count, nullptr);
	std::cout << "\t\033[1;33mNumero de familias de colas: \033[0m\033[1;32m"<<count<<"\033[0m\n";

	std::vector<VkQueueFamilyProperties> props(count);
	vkGetPhysicalDeviceQueueFamilyProperties(m_Context.physicalDevice, &count, props.data());


	for (int i = 0; i <= props.size()-1; i++) {
		VkQueueFlags flags = props[i].queueFlags;

		if (props[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			m_Context.QueueFamilie.graphicsFamily = i;
			m_Context.QueueFamilie.presentFamily = i;
			std::cout << "\t\033[1;33mFamilia de Colas seleccionada: "<<"\033[0m";
			std::cout << "\033[1;32mCola [" << i << "] -> Flags = " << flags << " -> ";
		}

		if (flags & VK_QUEUE_GRAPHICS_BIT)      std::cout << "GRAPHICS ";
		if (flags & VK_QUEUE_COMPUTE_BIT)       std::cout << "COMPUTE ";
		if (flags & VK_QUEUE_TRANSFER_BIT)      std::cout << "TRANSFER ";
		if (flags & VK_QUEUE_SPARSE_BINDING_BIT)std::cout << "SPARSE ";
		if (flags & VK_QUEUE_PROTECTED_BIT)     std::cout << "PROTECTED ";

		std::cout<<"\033[0m\n\n";

		if (m_Context.QueueFamilie.isComplete()) break;

	}


	return m_Context.QueueFamilie;
}