#include "Devices.h"

//############## Funcion Callback de Capas de Validacion ################//


std::vector<const char*> requiredDeviceExtension = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME,
	
};

static const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME,
	VK_KHR_SHADER_DRAW_PARAMETERS_EXTENSION_NAME,
	VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME,
	
};

std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());


//############## Dispositivos fisicos de vulkan ################//

void PhysicalDevice::SelectPhysicalDevices(VulkanInstance instance) {

	std::cout << "\033[1;36m[!] Buscando dispositivos fisicos...\033[0m\n";


	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance.GetInstance(), &deviceCount, nullptr);
	std::cout << "\t\033[1;33mNumero de dispositivos fisicos detectados: \033[0m"<< deviceCount<<"\n";


	if (deviceCount == 0) {
		throw std::runtime_error("failed to find GPUs with Vulkan support!");
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance.GetInstance(), &deviceCount, devices.data());


	for (const auto& device : devices) {
		if (IsDeviceSelectable(device)) {

			physicalDevice = device;

			break;
		}
	}

	if (GetPhysicalDevice() == VK_NULL_HANDLE) {
		throw std::runtime_error("failed to find a suitable GPU!");
	}

	//Mostrar información del dispositivo seleccionado
	VkPhysicalDeviceProperties2 prop2{};
	prop2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
	VkPhysicalDeviceDriverProperties driverProps{};
	driverProps.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DRIVER_PROPERTIES;
	prop2.pNext = &driverProps;

	vkGetPhysicalDeviceProperties2(physicalDevice, &prop2);
	std::cout << "\t\033[1;33mDatos del dispositivo fisico seleccionado: \033[0m\n";
	std::cout << "\t\t\033[1;33mGPU seleccionado: \033[0m\033[1;32m" << prop2.properties.deviceName << "\033[0m\n";
	std::cout << "\t\t\033[1;33mDriver Name: \033[0m\033[1;32m" << driverProps.driverName << "\033[0m\n";
	std::cout << "\t\t\033[1;33mDriver Info: \033[0m\033[1;32m" << driverProps.driverInfo << "\033[0m\n";
	std::cout << "\t\t\033[1;33mAPI Version: \033[0m\033[1;32m"
		<< VK_VERSION_MAJOR(prop2.properties.apiVersion) << "."
		<< VK_VERSION_MINOR(prop2.properties.apiVersion) << "."
		<< VK_VERSION_PATCH(prop2.properties.apiVersion) <<"\033[0m\n";
	std::cout << "\t\t\033[1;33mVendor ID:\033[0m\033[1;32m 0x" << std::hex << prop2.properties.vendorID << std::dec << "\033[0m\n\n";

};

bool PhysicalDevice::IsDeviceSelectable(VkPhysicalDevice physicalDevice) {
	bool extensionsSupported = checkDeviceExtensionSupport(physicalDevice);

	VkPhysicalDeviceFeatures supportedFeatures;
	vkGetPhysicalDeviceFeatures(physicalDevice, &supportedFeatures);

	return extensionsSupported && supportedFeatures.samplerAnisotropy;
}

bool PhysicalDevice::checkDeviceExtensionSupport(VkPhysicalDevice physicalDevice) {
	uint32_t extensionCount;


	vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);

	std::cout << "\t\033[1;33mNumero de extensiones del dispositivo fisico detectadas: \033[0m"<< extensionCount<<"\n";


	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, availableExtensions.data());

	int cont = 0, contmax = 20;
	std::cout<<"\t\033[1;33mPrimeras \033[0m\033[1;32m"<< contmax <<" extensiones \033[0m\033[1;33mdel dispositivo fisico:\033[0m\n";


	for (const VkExtensionProperties& e : availableExtensions) {
		if (cont <= contmax) {
			printf("\t\t\033[1;32m%s\033[0m\n", e.extensionName);
			cont++;
		}
		else {
			break;
		}
	}
	std::cout << "\n";


	for (const auto& extension : availableExtensions) {
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}

//############## Dispositivos logicos de vulkan ################//

void LogicalDevice::CreateLogicalDevice(PhysicalDevice physicaldevice) {
	QueueFamilyIndices indices = findQueueFamilies(physicaldevice.GetPhysicalDevice());
	SetQueueFamilies(indices);

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

	bool DeviceSupportsDynamicRendering = physicaldevice.GetPhysicalDevice();

	if (vkCreateDevice(physicaldevice.GetPhysicalDevice(), &createInfo, nullptr, &logicaldevice) != VK_SUCCESS) {
		throw std::runtime_error("failed to create logical device!");
	}
	std::cout << "\t\033[1;32mLogical device \033[0mguardado en \033[1;32m" << logicaldevice << "\033[0m\n\n";

	vkGetDeviceQueue(logicaldevice, indices.graphicsFamily.value(), 0, &graphicsQueue);

	vkGetDeviceQueue(logicaldevice, indices.presentFamily.value(), 0, &presentQueue);

};

QueueFamilyIndices LogicalDevice::findQueueFamilies(VkPhysicalDevice device) {
	std::cout << "\033[1;36m[!] Buscando familia de colas...\033[0m\n";

	QueueFamilyIndices indices;

	uint32_t count;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &count, nullptr);
	std::cout << "\t\033[1;33mNumero de familias de colas: \033[0m\033[1;32m"<<count<<"\033[0m\n";

	std::vector<VkQueueFamilyProperties> props(count);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &count, props.data());


	for (int i = 0; i <= props.size()-1; i++) {
		VkQueueFlags flags = props[i].queueFlags;

		if (props[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			QueueFamilie.graphicsFamily = i;
			QueueFamilie.presentFamily = i;
			std::cout << "\t\033[1;33mFamilia de Colas seleccionada: "<<"\033[0m";
			std::cout << "\033[1;32mCola [" << i << "] -> Flags = " << flags << " -> ";

		}

		if (flags & VK_QUEUE_GRAPHICS_BIT)      std::cout << "GRAPHICS ";
		if (flags & VK_QUEUE_COMPUTE_BIT)       std::cout << "COMPUTE ";
		if (flags & VK_QUEUE_TRANSFER_BIT)      std::cout << "TRANSFER ";
		if (flags & VK_QUEUE_SPARSE_BINDING_BIT)std::cout << "SPARSE ";
		if (flags & VK_QUEUE_PROTECTED_BIT)     std::cout << "PROTECTED ";

		std::cout<<"\033[0m\n\n";

		if (QueueFamilie.isComplete()) break;

	}


	return QueueFamilie;
}

