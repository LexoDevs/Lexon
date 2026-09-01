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

void InputSystem::Selector(KeyCode &keysactives){

    if (IsKeyPressed(KeyCode::Escape))
    {
        keysactives = KeyCode::Escape;
        std::cout<<"Se ha pulsado escape"<<std::endl;
    }
        if (IsKeyPressed(KeyCode::W))
    {
        keysactives = KeyCode::W;
        std::cout<<"Se ha pulsado w"<<std::endl;
    }



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

