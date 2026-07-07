#include "VulkanPhyDevice.h"

std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

void VulkanPhysicalDevice::SelectPhysicalDevices() {

	std::cout << "\033[1;36m[!] Buscando dispositivos fisicos...\033[0m\n";


	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(m_Context.instance, &deviceCount, nullptr);
	std::cout << "\t\033[1;33mNumero de dispositivos fisicos detectados: \033[0m"<< deviceCount<<"\n";


	if (deviceCount == 0) {
		throw std::runtime_error("failed to find GPUs with Vulkan support!");
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(m_Context.instance, &deviceCount, devices.data());


	for (const auto& device : devices) {
		if (IsDeviceSelectable(device)) {

			m_Context.physicalDevice = device;

			break;
		}
	}
	std::cout << "\t\033[1;33mNumero de dispositivos fisicos detectados: \033[0m"<< deviceCount<<"\n";

	if (m_Context.physicalDevice == VK_NULL_HANDLE) {
		throw std::runtime_error("failed to find a suitable GPU!");
	}

	//Mostrar información del dispositivo seleccionado
	VkPhysicalDeviceProperties2 prop2{};
	prop2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
	VkPhysicalDeviceDriverProperties driverProps{};
	driverProps.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DRIVER_PROPERTIES;
	prop2.pNext = &driverProps;

	vkGetPhysicalDeviceProperties2(m_Context.physicalDevice, &prop2);
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
		vkDestroyDevice(m_Context.device, nullptr);
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
