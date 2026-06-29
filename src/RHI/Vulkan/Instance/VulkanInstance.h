#pragma once

#include "../Core/VulkanContext.h"
#include "vector"
#include "../Resources/VulkanConstants.h"



class VulkanInstance {
public:
	//Destroy y Create
	void CreateInstances();
	void DestroyInstance();

	//Get
	VkInstance GetInstance() {return m_Context.instance;};

private:
	//Contexto de vulkan (Datos)
    VulkanContext& m_Context;

	//Funciones auxiliares/internas de la instancia
	void GetInstanceVersions();
	std::vector<const char*> GetInstanceExtensionsRequireds();

};