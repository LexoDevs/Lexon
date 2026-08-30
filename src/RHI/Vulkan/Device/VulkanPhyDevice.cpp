#include "VulkanPhyDevice.h"



std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

VulkanPhysicalDevice::VulkanPhysicalDevice(){

};




void VulkanPhysicalDevice::SelectPhysicalDevices(VkInstance instance) {

	std::cout << "\033[1;36m[!] Buscando dispositivos fisicos...\033[0m\n";

	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
	std::cout << "\t\033[1;33mNumero de dispositivos fisicos detectados: \033[0m"<< deviceCount<<"\n";


	if (deviceCount == 0) {
		throw std::runtime_error("failed to find GPUs with Vulkan support!");
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());


	for (const auto& device : devices) {
		if (IsDeviceSelectable(device)) {

			physicalDevice = device;

			break;
		}
	}
	std::cout << "\t\033[1;33mNumero de dispositivos fisicos detectados: \033[0m"<< deviceCount<<"\n";

	if (physicalDevice == VK_NULL_HANDLE) {
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

}

void VulkanPhysicalDevice::DestroyPhysicalDevices(){
		//vkDestroyDevice(device, nullptr);
	};


bool VulkanPhysicalDevice::IsDeviceSelectable(VkPhysicalDevice physicaldevice) {

	bool extensionsSupported = checkDeviceExtensionSupport(physicaldevice);

	VkPhysicalDeviceFeatures supportedFeatures;
	vkGetPhysicalDeviceFeatures(physicaldevice, &supportedFeatures);

	return extensionsSupported && supportedFeatures.samplerAnisotropy;
}

bool VulkanPhysicalDevice::checkDeviceExtensionSupport(VkPhysicalDevice physicaldevice) {
	uint32_t extensionCount;


	vkEnumerateDeviceExtensionProperties(physicaldevice, nullptr, &extensionCount, nullptr);

	std::cout << "\t\033[1;33mNumero de extensiones del dispositivo fisico detectadas: \033[0m"<< extensionCount<<"\n";


	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(physicaldevice, nullptr, &extensionCount, availableExtensions.data());

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


void VulkanPhysicalDevice::CreateSwapchainSupportDetails(VkSurfaceKHR Surface){
    swapDetails.capabilities =  GetSurfaceCapabilities(Surface);
    swapDetails.presentMode = GetSurfacePresentationsMode(Surface);
    swapDetails.format = chooseSwapSurfaceFormat(Surface);
}



VkSurfaceCapabilitiesKHR VulkanPhysicalDevice::GetSurfaceCapabilities(VkSurfaceKHR Surface) {
    VkSurfaceCapabilitiesKHR capabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, Surface, &capabilities);

    std::cout << "\t\033[1;33mLa resolucion de la ventana es: \033[0m\033[1;32m" << capabilities.currentExtent.height << 'x'
     << capabilities.currentExtent.width << "\033[0m\n\n";

    return capabilities;
}

VkPresentModeKHR VulkanPhysicalDevice::GetSurfacePresentationsMode(VkSurfaceKHR Surface) {
    uint32_t pPresentModeCount = 0;

    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, Surface, &pPresentModeCount, NULL);
	std::vector<VkPresentModeKHR> availablePresentModes(pPresentModeCount);

    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, Surface, &pPresentModeCount, availablePresentModes.data());

    std::cout << "\t\033[1;33mSe detectaron un total de \033[0m\033[1;32m" << pPresentModeCount << "\033[0m\033[1;33m modos de presentacion\033[0m\n";


    for (int i = 0; i < availablePresentModes.size(); i++) {

        if (PresentModeToString(availablePresentModes[i]) == "VK_PRESENT_MODE_MAILBOX_KHR") {

            std::cout << "\t\t\033[1;33mEl formato seleccionado es: \033[0m\033[1;32m" << PresentModeToString(availablePresentModes[i]) << "\033[0m\n\n";
            return availablePresentModes[i];

        }
    }
    return {};
}

VkSurfaceFormatKHR VulkanPhysicalDevice::chooseSwapSurfaceFormat(VkSurfaceKHR Surface) {
     uint32_t pSurfaceFormatCount = 0;

    std::vector<VkSurfaceFormatKHR>availableFormats = getSurfaceFormats(Surface, pSurfaceFormatCount);

    std::cout << "\t\033[1;33mSe detectaron un total de \033[0m\033[1;32m" << pSurfaceFormatCount << "\033[0m\033[1;33m formatos\033[0m\n";
	
    for (int i = 0; i < availableFormats.size()-1; i++) {

        if (FormatToString(availableFormats[i].format) == "VK_FORMAT_B8G8R8A8_UNORM") {

            std::cout << "\t\t\033[1;33mEl formato seleccionado es: \033[0m\033[1;32m" << FormatToString(availableFormats[i].format) << "\033[0m\n\n";

            
            return availableFormats[i];
        }
    }
    return {};
}

std::vector<VkSurfaceFormatKHR> VulkanPhysicalDevice::getSurfaceFormats(VkSurfaceKHR Surface, uint32_t &pSurfaceFormatCount) {
    pSurfaceFormatCount = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, Surface, &pSurfaceFormatCount, NULL);
    std::vector<VkSurfaceFormatKHR> availableFormats(pSurfaceFormatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, Surface, &pSurfaceFormatCount, availableFormats.data());

    return availableFormats;
}

VkExtent2D VulkanPhysicalDevice::chooseSwapExtent() {

    if (swapDetails.capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
    {
        return swapDetails.capabilities.currentExtent;
    }

    int width, height;
    //glfwGetFramebufferSize(GLFWwindow, &width, &height);

    width = std::clamp<uint32_t>(width, swapDetails.capabilities.minImageExtent.width, swapDetails.capabilities.maxImageExtent.width);
    height = std::clamp<uint32_t>(height, swapDetails.capabilities.minImageExtent.height, swapDetails.capabilities.maxImageExtent.height);
    
    swapDetails.capabilities.currentExtent.width = width;
    swapDetails.capabilities.currentExtent.height = height;

    return { width , height};
};