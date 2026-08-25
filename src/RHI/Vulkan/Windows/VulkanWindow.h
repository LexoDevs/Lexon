
#include <iostream>
#include "../../../Core/Input/InputSystem.h"

class Window {

	public:
	    virtual ~Window() = default;

		virtual void InitWindow() = 0;
		virtual void DestroyWindow() = 0;
	    virtual bool ShouldClose() const = 0;
		virtual void PoolEvents() const = 0;
		virtual void* GetNativeWindow() const = 0;
		virtual double GetTime() const = 0;
		virtual void SetWindowTitle(std::string title) const = 0;
		virtual void SetKeyCallback() const = 0;

		bool WindowWasResiced() const { return framebufferResized; }
		void ResetResizeFlag() { framebufferResized = false; }
		
		int GetWidth() const { return width; }
		int GetHeight() const { return height; }
		std::string GetTitle() const {return title;}

	protected:

		int width = 1280;
    	int height = 720;
		std::string title = "Ventana";

		bool framebufferResized = false;

};


class GLFWWindow : public Window {
	public:

	    GLFWWindow() = default;

		void InitWindow() override;
		void DestroyWindow() override;
    	bool ShouldClose() const override;
		void PoolEvents() const override;
		void* GetNativeWindow() const override { return glfwwindow;};
		double GetTime() const override;
		void SetWindowTitle(std::string title) const override;
		void SetKeyCallback() const override;
	private:

		void CreateGLFWindow();

		static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

		GLFWwindow* glfwwindow = nullptr;

};