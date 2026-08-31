#pragma once
#include "../Extensions/VulkanExtensions.h"
#include "../Helpers/VulkanFormats.h"
#include <optional>

#include "../Core/VulkanContext.h"


class VulkanLogicalDevice {
public:
    VulkanLogicalDevice(){};
    ~VulkanLogicalDevice(){ DestroyLogicalDevice();};

	void CreateLogicalDevice(VkPhysicalDevice physicalDevice);
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice physicalDevice);
    void DestroyLogicalDevice();
    VkDevice GetHandle() { return device;};
    QueueFamilyIndices GetQueueFamily() { return QueueFamilie;};
    VkQueue GetGraphicsQueue() { return graphicsQueue;};


private:
    //Dispositivo virtual que se usara en la aplicacion a partir del dis. fisico
    VkDevice device;
	QueueFamilyIndices QueueFamilie;
        //Familia de colas del dispositivo fisico
        VkQueue graphicsQueue;
        VkQueue transferQueue;
        VkQueue computeQueue;

    bool initialized = false;

};