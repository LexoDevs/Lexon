
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <GLFWWindow.h>
#include <InputMapGLFW.h>


//CONSTRUCCION Y DESTRUCCION
GLFWWindow::GLFWWindow(){};

GLFWWindow::~GLFWWindow(){ DestroyWindow(); };

void GLFWWindow::InitWindow(){
    if (m_Initialized) { return; }

    if (!glfwInit()) { throw std::runtime_error("Failed to initialize GLFW"); }
    
    CreateGLFWindow();

    m_Initialized = true;
}

void GLFWWindow::DestroyWindow(){
    if (!m_Initialized) { return; }

    if (glfwwindow) {
        glfwDestroyWindow(glfwwindow);
        glfwwindow = nullptr;
    }

    glfwTerminate();
    m_Initialized = false;

    std::cout << "Ventana destruida" << std::endl;
};

//FUNCIONES BASICAS DE LA VENTANA
bool GLFWWindow::ShouldClose() const { return glfwWindowShouldClose(glfwwindow);}

void GLFWWindow::CloseWindow() const { glfwSetWindowShouldClose(glfwwindow, GLFW_TRUE); }

void GLFWWindow::PollEvents() const { glfwPollEvents(); }

double GLFWWindow::GetTime() const { return glfwGetTime(); };

void GLFWWindow::SetWindowTitle(std::string title) const { glfwSetWindowTitle(glfwwindow, title.c_str()); };

void GLFWWindow::SetKeyCallback() const { glfwSetKeyCallback(glfwwindow, GLFW_KeyBoardCallback); };


//FUNCIONES RELACIONADAS CON EL INPUT
void GLFWWindow::WaitEvents() const { glfwWaitEvents();};

void GLFWWindow::OnKeyEvent(int key, int action){

    if (!m_InputSystem){
        return;
    }

    KeyCode engineKey = GLFWKeyToKeyCode(key);

    if (engineKey == KeyCode::Unknown){
        return;
    }

    if (action == GLFW_PRESS){
        std::cout<<"Input presionado"<<std::endl;
        m_InputSystem->SetKeyState( engineKey, true );
    }
    else if (action == GLFW_RELEASE){
                std::cout<<"Input soltado"<<std::endl;
        m_InputSystem->SetKeyState( engineKey, false);
    }
    
}

void GLFWWindow::CreateGLFWindow(){

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    std::cout << "\033[1;36m[!] Creando ventana...\033[0m\n";
    
    glfwwindow = glfwCreateWindow(WindowSice.width, WindowSice.height, "Hola", nullptr, nullptr);

    if (!glfwwindow)
    {
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwSetWindowUserPointer(glfwwindow, this);

    glfwSetFramebufferSizeCallback(glfwwindow, framebufferResizeCallback);

    std::cout << "\t\033[1;32m" << title << "\033[0m guardada en \033[1;32m" 
              << glfwwindow << "\033[0m\n\n";
    
};



void GLFWWindow::framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
    auto glfwWindow  = reinterpret_cast<GLFWWindow*>(
        glfwGetWindowUserPointer(window)
    );
    glfwWindow->WindowSice.width = width;
    glfwWindow->WindowSice.height = height;

    glfwWindow->framebufferResized = true;

}

