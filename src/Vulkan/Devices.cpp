#include "Devices.h"

//############## Funcion Callback de Capas de Validacion ################//

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData)
{
    if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
        std::cerr << "\033[1;31m[VALIDATION ERROR] \033[0m" << pCallbackData->pMessage << std::endl;
    
    else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
        std::cout << "\033[1;33m[VALIDATION WARNING] \033[0m" << pCallbackData->pMessage << std::endl;
    
    else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
        std::cout << "\033[1;36m[VALIDATION INFO] \033[0m" << pCallbackData->pMessage << std::endl;
    
    else
        std::cout << "\033[1;90m[VALIDATION VERBOSE] \033[0m" << pCallbackData->pMessage << std::endl;

    return VK_FALSE;
}

const std::vector<char const*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

std::vector<const char*> requiredDeviceExtension = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME,
	
};

static const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME,
	VK_KHR_SHADER_DRAW_PARAMETERS_EXTENSION_NAME,
	VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME,
	
};

std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}

#ifdef NDEBUG
constexpr bool enableValidationLayers = false;
#else
constexpr bool enableValidationLayers = true;
#endif

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}


void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
	createInfo = {};
	createInfo.sType 				= VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity 		= VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
									  VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
									  VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

	createInfo.messageType 			= VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
									  VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
									  VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

	createInfo.pfnUserCallback 		= debugCallback;
}

void setupDebugMessenger() {
	if (!enableValidationLayers) return;

	VkDebugUtilsMessengerCreateInfoEXT createInfo;
	populateDebugMessengerCreateInfo(createInfo);

	//if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
	//	throw std::runtime_error("failed to set up debug messenger!");
	//}
}

bool checkValidationLayerSupport() {
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const char* layerName : validationLayers) {
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers) {
			if (strcmp(layerName, layerProperties.layerName) == 0) {
				layerFound = true;
				break;
			}
		}

		if (!layerFound) {
			return false;
		}
	}

	return true;
}

//############## Estructura para deparar los datos de version en 3 ################//

struct {
	int Major = 0;
	int Minor = 0;
	int Patch = 0;
} m_Version;

//############## Estructura para agrupar las colas de dispositivos fisicos ################//



//############## Instancia de Vulkan ################//

void VulkanInstance::CreateInstance() {

	std::cout << "[!] Creando instancia de Vulkan..." << "\n";
	std::cout << "Comprobando version... \n";

	GetInstanceVersion();

	if (enableValidationLayers && !checkValidationLayerSupport()) {
		throw std::runtime_error("validation layers requested, but not available!");
	}

	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Hello Triangle";
	appInfo.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(0, 1, 0);
	appInfo.apiVersion = VK_MAKE_API_VERSION(0, m_Version.Major, m_Version.Minor, m_Version.Patch);

	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	std::cout << "Cargando extensiones... \n";
	auto extensionsList = GetInstanceExtensionsRequired();


	std::cout << "Extensiones cargadas: \n";
	for (int i = 0; i <= extensionsList.size()-1; i++) {
		std::cout << "\tExtension: " << extensionsList[i] << " guardada en " << &extensionsList[i] << '\n';

	}

	std::cout << "\n";

	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensionsList.size());
	createInfo.ppEnabledExtensionNames = extensionsList.data();

	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
	if (enableValidationLayers) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();

		populateDebugMessengerCreateInfo(debugCreateInfo);
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
	}
	else {
		createInfo.enabledLayerCount = 0;
		createInfo.pNext = nullptr;
	}

	if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
		throw std::runtime_error("failed to create instance!");
	}
	std::cout << '\t' <<"Instancia guardada en " << instance << "\n";


};

void VulkanInstance::DestroyInstance() {
	if (enableValidationLayers) {
           // DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
        }

	vkDestroyInstance(instance, nullptr);

};

//Obtencion de la version de la instancia de vulkan creada
void VulkanInstance::GetInstanceVersion() {

	uint32_t InstanceVersion = 0;

	VkResult res = vkEnumerateInstanceVersion(&InstanceVersion);
	if (!res) {

		m_Version.Major = VK_API_VERSION_MAJOR(InstanceVersion);
		m_Version.Minor = VK_API_VERSION_MINOR(InstanceVersion);
		m_Version.Patch = VK_API_VERSION_PATCH(InstanceVersion);

		std::cout<<"\t" << "La version cargada de Vulkan es: " <<
			m_Version.Major << '.' << m_Version.Minor <<'.' << m_Version.Patch<<"\n";
	}
}

//Otencion de todas las extensiones requeridas por la instanciad de vulkan
std::vector<const char*> VulkanInstance::GetInstanceExtensionsRequired() {

	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

        if (enableValidationLayers) {
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

	return extensions;
}

//############## Dispositivos fisicos de vulkan ################//

void PhysicalDevice::SelectPhysicalDevices(VulkanInstance instance) {
	std::cout << "[!] Buscando dispositivos fisicos..." << "\n";


	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance.GetInstance(), &deviceCount, nullptr);
	std::cout<<"\tNumero de dispositivos fisicos detectados: "<<deviceCount<<"\n";

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
	std::cout << "\tDatos del dispositivo fisico seleccionado: \n";
	std::cout << "\t\tGPU seleccionado: " << prop2.properties.deviceName << std::endl;
	std::cout << "\t\tDriver Name: " << driverProps.driverName << std::endl;
	std::cout << "\t\tDriver Info: " << driverProps.driverInfo << std::endl;
	std::cout << "\t\tAPI Version: "
		<< VK_VERSION_MAJOR(prop2.properties.apiVersion) << "."
		<< VK_VERSION_MINOR(prop2.properties.apiVersion) << "."
		<< VK_VERSION_PATCH(prop2.properties.apiVersion) << std::endl;
	std::cout << "\t\tVendor ID: 0x" << std::hex << prop2.properties.vendorID << std::dec << std::endl;

};

bool PhysicalDevice::IsDeviceSelectable(VkPhysicalDevice device) {
	bool extensionsSupported = checkDeviceExtensionSupport(device);

	VkPhysicalDeviceFeatures supportedFeatures;
	vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

	return extensionsSupported && supportedFeatures.samplerAnisotropy;
}


bool PhysicalDevice::checkDeviceExtensionSupport(VkPhysicalDevice device) {
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
	std::cout << "\tNumero de extensiones del dispositivo fisico detectadas: " << extensionCount << "\n";


	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	int cont = 0, contmax = 9;
	std::cout<<"\tPrimeras "<< contmax <<" extensiones del dispositivo fisico:\n";


	for (const VkExtensionProperties& e : availableExtensions) {
		if (cont <= contmax) {
			printf("\t\t%s\n", e.extensionName);
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
	std::cout<<"Logical device guardado en: "<<logicaldevice<<"\n";
	vkGetDeviceQueue(logicaldevice, indices.graphicsFamily.value(), 0, &graphicsQueue);

	vkGetDeviceQueue(logicaldevice, indices.presentFamily.value(), 0, &presentQueue);

};

QueueFamilyIndices LogicalDevice::findQueueFamilies(VkPhysicalDevice device) {
	std::cout << "[!] Buscando familia de colas...\n";
	QueueFamilyIndices indices;

	uint32_t count;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &count, nullptr);
	std::cout << "\tNumero de familias de colas: "<<count<<"\n";

	std::vector<VkQueueFamilyProperties> props(count);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &count, props.data());

	int i = 0;
	for (int i = 0; i <= props.size()-1; i++) {
		VkQueueFlags flags = props[i].queueFlags;

		std::cout << "\tCola [" << i << "] -> Flags = " << flags << " -> ";

		// Mostrar nombres de las colas
		if (flags & VK_QUEUE_GRAPHICS_BIT)      std::cout << "GRAPHICS ";
		if (flags & VK_QUEUE_COMPUTE_BIT)       std::cout << "COMPUTE ";
		if (flags & VK_QUEUE_TRANSFER_BIT)      std::cout << "TRANSFER ";
		if (flags & VK_QUEUE_SPARSE_BINDING_BIT)std::cout << "SPARSE ";
		if (flags & VK_QUEUE_PROTECTED_BIT)     std::cout << "PROTECTED ";

		std::cout << "\n";

		if (props[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			indices.graphicsFamily = i;
			indices.presentFamily = i;
			std::cout << "\tCola seleccionada:" << i << "\n";
		}

		//if (indices.isComplete()) break;

	}


	return indices;
}

