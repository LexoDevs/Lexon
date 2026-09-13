#include "InputSystem.h"
#include <iostream>

void InputSystem::SetKeyState(KeyCode key,bool pressed)
    {
        const auto index = static_cast<size_t>(key);

        m_Keys[index] = pressed;
    }

bool InputSystem::IsKeyPressed(KeyCode key) const
    {
        const auto index = static_cast<size_t>(key);

        return m_Keys[index];
    }

void InputSystem::SetMouseButtonState( MouseButton button, bool pressed)
    {
        const size_t index = static_cast<size_t>(button);
        m_MouseButtons[index] = pressed;
    }

bool InputSystem::IsMouseButtonPressed( MouseButton button) const
    {
        const size_t index = static_cast<size_t>(button);
        return m_MouseButtons[index];
    }

void InputSystem::SetMousePosition( double xpos, double ypos)
    {
        if (m_FirstMouseEvent)
        {
            m_MouseX = xpos;
            m_MouseY = ypos;
            m_FirstMouseEvent = false;
            return;
        }

        m_MouseDeltaX += xpos - m_MouseX;
        m_MouseDeltaY += ypos - m_MouseY;

        m_MouseX = xpos;
        m_MouseY = ypos;

        //std::cout<<"Posicion x raton: "<<m_MouseX<<std::endl;
        //std::cout<<"Posicion y raton: "<<m_MouseY<<std::endl;
    }

KeyCode InputSystem::Selector(){
    
    if (IsKeyPressed(KeyCode::Escape))
    {
        return KeyCode::Escape;
    }
    
    if (IsKeyPressed(KeyCode::W))
    {
        return  KeyCode::W;
    }
    
    if (IsKeyPressed(KeyCode::H))
    {
        return  KeyCode::H;
    }
    if (IsKeyPressed(KeyCode::D))
    {
        return  KeyCode::D;
    }
    if (IsKeyPressed(KeyCode::S))
    {
        return  KeyCode::S;
    }
    if (IsKeyPressed(KeyCode::A))
    {
        return  KeyCode::A;
    }

    if (IsKeyPressed(KeyCode::Q))
    {
        return  KeyCode::Q;
    }
    if (IsKeyPressed(KeyCode::E))
    {
        return  KeyCode::E;
    }


    else
    {
        return  KeyCode::Unknown;
    }



}


void InputSystem::BeginFrame()
{
    m_MouseDeltaX = 0.0;
    m_MouseDeltaY = 0.0;
}

void InputSystem::ResetMouseDelta()
{
    m_MouseDeltaX = 0.0;
    m_MouseDeltaY = 0.0;
    m_FirstMouseEvent = true;
}


/*static glm::vec3 VectorFrontalCamara(glm::vec3 v) {
    float modulo = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);

    return {v.x/(modulo), v.y/(modulo) , v.z/(modulo)};
}

static glm::vec3 multiplicarMatrizVector(glm::vec3 vec) {
        std::vector<std::vector<double>> matriz = {
        {0.0, -1.0, 0.0},
        {1.0, 0.0, 0.0},
        {0.0, 0.0, 1.0}
    };

    int filas = matriz.size();
    int columnas = matriz[0].size();
    
    glm::vec3 resultado;

    for (int i = 0; i < filas; ++i) {
        for (int j = 0; j < columnas; ++j) {
            resultado[i] += matriz[i][j] * vec[j];
        }
    }

    return resultado;
}*/

