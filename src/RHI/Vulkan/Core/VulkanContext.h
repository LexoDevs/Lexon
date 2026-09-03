#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include "../Helpers/VulkanConstants.h"
#include <vulkan/vulkan.h> 
#include <vector>
#include "../Helpers/VulkanFormats.h"
#include <array>
#include "../../../Assets/Loaders/LoaderAssets.h"
#include <WindowProperties.h>

struct VulkanContext
{


    //Instancia de vulkan
    VkInstance instance;

    //Log de mensajes de las capas de validacion de vulkan

    //Superficie virtual de ventana
    VkSurfaceKHR surface;

    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    VkPresentModeKHR presentationsurface;
    std::vector<VkSurfaceFormatKHR> availableFormats;
    VkSurfaceFormatKHR surfaceformats;


    
    VkExtent2D swapExtent;


    //Dispositivo fisico seleccionado (la gráfica)
    VkPhysicalDevice physicalDevice;

    //Dispositivo virtual que se usara en la aplicacion a partir del dis. fisico
    VkDevice device;
	QueueFamilyIndices QueueFamilie;
        //Familia de colas del dispositivo fisico
        VkQueue graphicsQueue;
        VkQueue transferQueue;
        VkQueue computeQueue;

    // Cadena de intercambio
    VkSwapchainKHR swapchain;
	VkExtent2D swapChainExtent;
	VkSurfaceFormatKHR swapChainSurfaceFormat;
	VkFormat swapChainImageFormat;

	std::vector<VkImage> swapchainImages;
	std::vector<VkImageView> swapChainImageViews;

    // Pipeline grafico
	VkShaderModule shaderModule;
	VkPipelineLayout pipelineLayout;
	VkPipeline Pipeline;
    VkDescriptorSetLayout descriptorSetLayout;

    VkCommandPool commandPool;

    VkDescriptorPool descriptorPool;



    std::vector<uint32_t> mipLevels;
    std::vector<VkImage> textureImages;
    std::vector<VkDeviceMemory> textureImageMemories;

    std::vector<int> texWidth, texHeight, texChannels;

    VkPhysicalDeviceMemoryProperties memProperties;
    uint32_t VulkanMemoryTypeIndex;
    
    VkImageView textureImageView;
    VkSampler textureSampler;
    VkDescriptorSet descriptorSets[MAX_FRAMES_IN_FLIGHT];


    // Buffer de comandos
    VkCommandBuffer commandBuffers[MAX_FRAMES_IN_FLIGHT];
    VkCommandBuffer commandBuffer;

    // Buffer de Vertices
            VkBuffer vertexBuffer;
            VkDeviceMemory vertexBufferMemory;

    //Buffer de Indices

            VkBuffer indexBuffer;
            VkDeviceMemory indexBufferMemory;

    //Buffer uniforme        
    VkBuffer uniformBuffers[MAX_FRAMES_IN_FLIGHT];
    std::vector<VkDeviceMemory> uniformBuffersMemory;
    std::vector<void*> uniformBuffersMapped;
    UniformBufferObject UBO{}; 


    //IMGUI




    //Depth buffer
    VkExtent2D depthsize;
    VkImage depthImage;
    VkDeviceMemory depthImageMemory;
    VkImageView depthImageView;
    VkClearValue clearDepth;
    VkFormat depthformat;

    //Semaforos para sincronizacion
    VkSemaphore imageAvailableSemaphore[MAX_FRAMES_IN_FLIGHT];
    VkSemaphore renderFinishedSemaphore[MAX_FRAMES_IN_FLIGHT]; // Uno por imagen del swapchain
    VkFence inFlightFence[MAX_FRAMES_IN_FLIGHT];
    uint32_t frameIndex = 0;
};
