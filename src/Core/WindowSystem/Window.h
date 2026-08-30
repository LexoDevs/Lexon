#pragma once
#include <WindowProperties.h>

class Window {

	public:
	    virtual ~Window() = default;

		virtual void InitWindow() = 0;
		virtual void DestroyWindow() = 0;
	    virtual bool ShouldClose() const = 0;
		virtual void CloseWindow() const = 0;
		virtual void PollEvents() const = 0;
		virtual void* GetNativeWindow() const = 0;
		virtual double GetTime() const = 0;
		virtual void SetWindowTitle(std::string title) const = 0;
		virtual void SetKeyCallback() const = 0;
		virtual void SetInputSystem(InputSystem* inputSystem) = 0;
		virtual void OnKeyEvent(int key,int action) = 0;
		virtual void WaitEvents() const = 0;

		bool WindowWasResiced() const { return framebufferResized; }
		void ResetResizeFlag() { framebufferResized = false; }
		
		uint32_t GetWidth() const { return WindowSice.width; }
		uint32_t GetHeight() const { return WindowSice.height; }
		std::string GetTitle() const {return title;}

	protected:

	WindowExtent WindowSice;
	std::string title = "Ventana";
	bool framebufferResized = false;
	bool m_Initialized = false;
};