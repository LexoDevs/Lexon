#include "VulkanInstance.h"
#include <iostream>
#include <cstring>
#include <GLFW/glfw3.h>

struct {
	int Major = 0;
	int Minor = 0;
	int Patch = 0;
} m_Version;



bool checkValidationLayerSupports() {
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

void populateDebugMessengerCreateInfos(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
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

void VulkanInstance::GetInstanceVersions() {

	uint32_t InstanceVersion = 0;

	VkResult res = vkEnumerateInstanceVersion(&InstanceVersion);
	if (!res) {

		m_Version.Major = VK_API_VERSION_MAJOR(InstanceVersion);
		m_Version.Minor = VK_API_VERSION_MINOR(InstanceVersion);
		m_Version.Patch = VK_API_VERSION_PATCH(InstanceVersion);

		std::cout<<"\t" << "La \033[1;32mversion\033[0m cargada de \033[1;32mVulkan\033[0m es: \033[1;32m" <<
			m_Version.Major << '.' << m_Version.Minor <<'.' << m_Version.Patch<<"\033[0m\n\n";
	}
}

std::vector<const char*> VulkanInstance::GetInstanceExtensionsRequireds() {

	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

        if (enableValidationLayers) {
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

	return extensions;
}

void VulkanInstance::CreateInstances() {



	std::cout << "\033[1;36m[!] Creando instancia de Vulkan...\033[0m\n";
	std::cout << "\033[1;33m\tComprobando version...\033[0m\n";

	GetInstanceVersions();

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

	std::cout << "\033[1;33m\tCargando extensiones...\033[0m\n";

	auto extensionsList = GetInstanceExtensionsRequireds();

	std::cout << "\033[1;33m\tExtensiones cargadas:\033[0m\n";
	for (int i = 0; i <= extensionsList.size()-1; i++) {
		std::cout << "\t\t\033[1;32m"<<extensionsList[i]<<"\033[0m:" << " alojada en \033[1;32m" << &extensionsList[i] << "\033[0m\n";

	}

	std::cout << "\n";

	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensionsList.size());
	createInfo.ppEnabledExtensionNames = extensionsList.data();

    //...............................
	if (enableValidationLayers && !checkValidationLayerSupports()) {
		throw std::runtime_error("validation layers requested, but not available!");
	}

	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
	if (enableValidationLayers) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();

		populateDebugMessengerCreateInfos(debugCreateInfo);
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
	}
	else {
		createInfo.enabledLayerCount = 0;
		createInfo.pNext = nullptr;
	}
    //...............................
	std::cout << '\t' <<"\033[1;32mInstancia\033[0m guardada en \033[1;32m" << &m_Context.instance << "\033[0m\n\n";

	if (vkCreateInstance(&createInfo, nullptr, &m_Context.instance) != VK_SUCCESS) {
		throw std::runtime_error("failed to create instance!");
	}

	//m_Context.instance = GetInstance();

	std::cout << '\t' <<"\033[1;32mInstancia\033[0m guardada en \033[1;32m" << &m_Context.instance << "\033[0m\n\n";


}


void VulkanInstance::DestroyInstance() {

	vkDestroyInstance(m_Context.instance, nullptr);

};


