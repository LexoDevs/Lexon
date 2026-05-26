
#include "Window.h"



const char* FormatToString(VkFormat format) {
    switch (format) {
    case 44:              return "VK_FORMAT_B8G8R8A8_UNORM";
    case 50:              return "VK_FORMAT_B8G8R8A8_SRGB ";
    case 37:                 return "VK_FORMAT_R8G8B8A8_UNORM ";
    case 43:               return "VK_FORMAT_R8G8B8A8_SRGB ";
    case 64:               return "VK_FORMAT_A2B10G10R10_UNORM_PACK32 ";

    default: return "tochar(format)";
    }
}

const char* PresentModeToString(VkPresentModeKHR presentmode) {
    switch (presentmode) {
    case 0:                  return "VK_PRESENT_MODE_IMMEDIATE_KHR";
    case 1:                  return "VK_PRESENT_MODE_MAILBOX_KHR";
    case 2:                  return "VK_PRESENT_MODE_FIFO_KHR";
    case 3:                  return "VK_PRESENT_MODE_FIFO_RELAXED_KHR";
    case 1000111000:         return "VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR";
    case 1000111001:         return "VK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR";
    case 1000361000:         return "VK_PRESENT_MODE_FIFO_LATEST_READY_KHR";

    default: return "Formato desconocido.";
    }
}





//############## Clase Window ################//
 
void Window::CargarGLFW() {
    
    glfwInit();

}

void Window::DesargarGLFW() {

    glfwTerminate();

}

void Window::CrearVentana(const char* name) {

    //Definicion de las condiciones de la ventana
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);



    //Creacion de ventana
    std::cout << "[!] Creando ventana..."<<"\n";
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, name, nullptr, nullptr);
    std::cout <<"\t" << name << " guardada en " << window << "\n";

    //Cambiar icono de ventana
    GLFWimage iconwindow;


    //Se incluye en la lista de ventanas creadas
    GestorVentanas.push_back(window);
}

/*void Window::ActualizarVentanas(LogicalDevice logicaldevices, Pool ComandPool, Swapchain swapchain, GraphicsPipeline pipeline, Render render) {

    while (GestorVentanas.size() > 0) {
        size_t i = 0;

        while (i < GestorVentanas.size() ) { 

            if (!glfwWindowShouldClose(GestorVentanas[i])) {

                glfwSetKeyCallback(GestorVentanas[i], GLFW_KeyCallback);
                glfwPollEvents();
                std::cout<<"Intentando cargar el logical device:" <<logicaldevices.GetLogicalDevice() <<"\n";
                render.drawFrame(logicaldevices, ComandPool, swapchain, pipeline);
                //masterengine.StartDrawFrame(logicaldevices);
            }
            else {
                std::cout<<"Cerrando ventana"<<std::endl;
                glfwDestroyWindow(GestorVentanas[i]);
                GestorVentanas.erase(GestorVentanas.begin() + i);
                i--;

            }
            i++;
        }
    }
}*/

void Window::LimpiarVentanas() {

    //Cierra todas las ventanas al limpiar el array 
    GestorVentanas.clear();
}

void Window::InitWindowsSistem() {
    //Cargado de la libreria GLFW para gestionar ventanas
    CargarGLFW();

    //Creacion de una ventana
    CrearVentana("Ventana principal");

};

//############## Clase Surface Window ################//

void WindowSurface::CreateWindowSurface(VulkanInstance instance, Window window) {

    std::cout << "[!] Creando superficie de ventana virtual...\n";

    if (glfwCreateWindowSurface(instance.GetInstance(), window.GetWindows(0), nullptr, &surface) != 0) {
        throw std::runtime_error("failed to create window surface!");
    }

};




VkSurfaceCapabilitiesKHR WindowSurface::GetSurfaceCapabilities(PhysicalDevice physicaldevice) {
    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicaldevice.GetPhysicalDevice(), surface, &surfaceCapabilities);

    std::cout << "La resolucion de la ventana es: " << surfaceCapabilities.currentExtent.height << 'x' << surfaceCapabilities.currentExtent.width << std::endl;

    return surfaceCapabilities;
}

VkExtent2D WindowSurface::chooseSwapExtent(VkSurfaceCapabilitiesKHR surfaceCapabilities, Window window) {

    if (surfaceCapabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
    {
        return surfaceCapabilities.currentExtent;
    }
    int width, height;
    glfwGetFramebufferSize(window.GetWindows(0), &width, &height);

    return {
    std::clamp<uint32_t>(width, surfaceCapabilities.minImageExtent.width, surfaceCapabilities.maxImageExtent.width),
    std::clamp<uint32_t>(height, surfaceCapabilities.minImageExtent.height, surfaceCapabilities.maxImageExtent.height)
    };
}

std::vector<VkSurfaceFormatKHR> WindowSurface::getSurfaceFormats(PhysicalDevice physicaldevice, uint32_t &pSurfaceFormatCount) {
    pSurfaceFormatCount = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicaldevice.GetPhysicalDevice(), surface, &pSurfaceFormatCount, NULL);
    std::vector<VkSurfaceFormatKHR> availableFormats(pSurfaceFormatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicaldevice.GetPhysicalDevice(), surface, &pSurfaceFormatCount, availableFormats.data());

    return availableFormats;
}

VkSurfaceFormatKHR WindowSurface::chooseSwapSurfaceFormat(std::vector<VkSurfaceFormatKHR> availableFormats, uint32_t pSurfaceFormatCount) {
    std::cout << "Se detectaron un total de " << pSurfaceFormatCount << " formatos\n";
    for (int i = 0; i < availableFormats.size()-1; i++) {
        //std::cout << "\tLos formatos son: " << FormatToString(availableFormats[i].format) << "\n";

        if (FormatToString(availableFormats[i].format) == "VK_FORMAT_B8G8R8A8_UNORM") {
            std::cout << "\tEl formato seleccionado es: " << FormatToString(availableFormats[i].format) << std::endl;
            return availableFormats[i];
        }
    }
    return {};
}

VkPresentModeKHR WindowSurface::GetSurfacePresentationsMode(PhysicalDevice physicaldevice) {
    uint32_t pPresentModeCount = 0;
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicaldevice.GetPhysicalDevice(), surface, &pPresentModeCount, NULL);
    std::vector<VkPresentModeKHR> availablePresentModes(pPresentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicaldevice.GetPhysicalDevice(), surface, &pPresentModeCount, availablePresentModes.data());

    std::cout << "Se detectaron un total de " << pPresentModeCount << " modos de presentacion\n";

    for (int i = 0; i < availablePresentModes.size(); i++) {
        //std::cout << "\tLos formatos son: " << PresentModeToString(availablePresentModes[i]) << "\n";

        if (PresentModeToString(availablePresentModes[i]) == "VK_PRESENT_MODE_FIFO_KHR") {

            std::cout << "\tEl formato seleccionado es: " << PresentModeToString(availablePresentModes[i]) << std::endl;
            return availablePresentModes[i];

        }
    }
    return {};
}


