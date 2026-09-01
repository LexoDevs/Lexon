#pragma once
#include <iostream>
#include <InputSystem.h>

#include "Window.h"

class GLFWWindow : public Window {
	public:

	    GLFWWindow();
		~GLFWWindow() override;

		void InitWindow() override;
		void DestroyWindow() override;
    	bool ShouldClose() const override;
		void CloseWindow() const override;
		void PollEvents() const override;
		void* GetNativeWindow() const override { return glfwwindow;};
		double GetTime() const override;
		void SetWindowTitle(std::string title) const override;
		void SetKeyCallback() const override;
		void SetInputSystem(InputSystem* inputSystem) { m_InputSystem = inputSystem;};
		void OnKeyEvent(int key,int action);
		void WaitEvents() const;
		bool& GetHUDVisibility() {return HUDVisibility;};
		void SetHUDVisibility(bool value) { HUDVisibility = value;};

	private:
		bool HUDVisibility = true;
		void CreateGLFWindow();
		static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
		
		GLFWwindow* glfwwindow = nullptr;
    	InputSystem* m_InputSystem = nullptr;
		
};

static void GLFW_KeyBoardCallback( GLFWwindow* window, int key, int scancode, int action, int mods){
    
	auto glfwWindow = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(window));

    if (glfwWindow){
        glfwWindow->OnKeyEvent(key, action);
    }
}