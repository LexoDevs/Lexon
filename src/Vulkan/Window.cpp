#include "Window.h"
#include "VulkanFormats.h"

#include "dwmapi.h"
//############## Clase Window ################//
 
void Window::CargarGLFW() {
    
    glfwInit();

}

void Window::DesargarGLFW() {

    glfwTerminate();

}

void Window::framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
    auto app = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    app->getframebufferResized() = true;

}

void Window::PersonalizarVentana(GLFWwindow* window){

    #ifdef _WIN32
    HWND hwnd = glfwGetWin32Window(window);
    if (hwnd)
    {
        // ==================== COLOR DE TÍTULO Y BORDE ====================
        // Azul personalizado (puedes cambiar los valores RGB)
        COLORREF captionColor = RGB(0, 100, 255);     // Azul bonito
        COLORREF borderColor  = RGB(0, 80, 220);      // Azul un poco más oscuro para el borde

        // Color de la barra de título
        DwmSetWindowAttribute(hwnd, DWMWA_CAPTION_COLOR, 
                             &captionColor, sizeof(captionColor));

        // Color del borde de la ventana
        DwmSetWindowAttribute(hwnd, DWMWA_BORDER_COLOR, 
                             &borderColor, sizeof(borderColor));

        // Opcional: Color del texto del título (blanco para que se vea bien)
        COLORREF titleTextColor = RGB(255, 255, 255);
        DwmSetWindowAttribute(hwnd, DWMWA_TEXT_COLOR, 
                             &titleTextColor, sizeof(titleTextColor));

        // Esquinas redondeadas (Windows 11)
        DWM_WINDOW_CORNER_PREFERENCE preference = DWMWCP_ROUND;
        DwmSetWindowAttribute(hwnd, DWMWA_WINDOW_CORNER_PREFERENCE, 
                             &preference, sizeof(preference));
    }
    #endif

};

void Window::CrearVentana(const char* name)
{
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    std::cout << "\033[1;36m[!] Creando ventana...\033[0m\n";
    
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, name, nullptr, nullptr);
    if (!window)
    {
        throw std::runtime_error("Failed to create GLFW window");
    }


    PersonalizarVentana(window);

    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);

    std::cout << "\t\033[1;32m" << name << "\033[0m guardada en \033[1;32m" 
              << window << "\033[0m\n\n";

    GestorVentanas.push_back(window);
    glfwSetWindowUserPointer(window, this);
}

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

void Window::DestroyWindowsSistem(){

    DesargarGLFW();

};



//############## Clase Surface Window ################//

void WindowSurface::CreateWindowSurface(VulkanInstance& instance, Window window) {

	std::cout << "\033[1;36m[!] Creando superficie de ventana virtual...\033[0m\n";

    if (glfwCreateWindowSurface(instance.GetInstance(), window.GetWindows(0), nullptr, &surface) != 0) {
        throw std::runtime_error("failed to create window surface!");
    }

    std::cout << "\t\033[1;32mWindow Surface\033[0m:" << " alojada en \033[1;32m" << &surface << "\033[0m\n\n";


};




VkSurfaceCapabilitiesKHR WindowSurface::GetSurfaceCapabilities(PhysicalDevice physicaldevice) {
    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicaldevice.GetPhysicalDevice(), surface, &surfaceCapabilities);

    std::cout << "\t\033[1;33mLa resolucion de la ventana es: \033[0m\033[1;32m" << surfaceCapabilities.currentExtent.height << 'x' << surfaceCapabilities.currentExtent.width << "\033[0m\n\n";

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
    std::cout << "\t\033[1;33mSe detectaron un total de \033[0m\033[1;32m" << pSurfaceFormatCount << "\033[0m\033[1;33m formatos\033[0m\n";
    for (int i = 0; i < availableFormats.size()-1; i++) {

        if (FormatToString(availableFormats[i].format) == "VK_FORMAT_B8G8R8A8_UNORM") {

            std::cout << "\t\t\033[1;33mEl formato seleccionado es: \033[0m\033[1;32m" << FormatToString(availableFormats[i].format) << "\033[0m\n\n";
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
    std::cout << "\t\033[1;33mSe detectaron un total de \033[0m\033[1;32m" << pPresentModeCount << "\033[0m\033[1;33m modos de presentacion\033[0m\n";


    for (int i = 0; i < availablePresentModes.size(); i++) {

        if (PresentModeToString(availablePresentModes[i]) == "VK_PRESENT_MODE_MAILBOX_KHR") {

            std::cout << "\t\t\033[1;33mEl formato seleccionado es: \033[0m\033[1;32m" << PresentModeToString(availablePresentModes[i]) << "\033[0m\n\n";
            return availablePresentModes[i];

        }
    }
    return {};
}


