#pragma once

#include "../Core/VulkanContext.h"
#include "vector"
#include "../Resources/VulkanConstants.h"
#include <vulkan/vulkan.h>

class VulkanInstance {
public:
    VulkanInstance(VulkanContext& context)
        : m_Context(context)
    {
    }

	//Destroy y Create
	void CreateInstances();
	void DestroyInstance();

	//Get

private:
	//Contexto de vulkan (Datos)
    VulkanContext& m_Context;
	

	//Funciones auxiliares/internas de la instancia
	void GetInstanceVersions();
	std::vector<const char*> GetInstanceExtensionsRequireds();

};