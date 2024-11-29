#include <DisplayTargetFactory.h>

#include <stdexcept>
#include <algorithm>

#if defined(_WIN_TARGET)
#include <windows.h>
#elif defined(_LINUX_TARGET)
#include <X11/Xlib.h>
#elif defined(_MAC_TARGET)
#include <Cocoa/Cocoa.h>
#endif

namespace UltReality::Rendering
{
	DisplayTarget DisplayTargetFactory::CreateTarget(const WindowConfig& config, MessageHandler messageHandler)
	{
#if defined(_WIN_TARGET)
		std::string windowClassName = config.title;
		windowClassName.erase(std::remove_if(windowClassName.begin(), windowClassName.end(), ::isspace), windowClassName.end());
		windowClassName += "_WindowClass";

		HINSTANCE hInstance = GetModuleHandle(nullptr);
		if (!hInstance)
			throw std::runtime_error("Failed to get module handle (HINSTANE) for the application");

		// Register the window class if not already done
		if (!m_isWindowClassRegistered)
		{
			WNDCLASS wc = {};
			wc.style = CS_HREDRAW | CS_VREDRAW;
			wc.lpfnWndProc = messageHandler ? messageHandler : DefWindowProc; // Use custom message handler if it was provided, default otherwise
			wc.cbClsExtra = 0;
			wc.cbWndExtra = 0;
			wc.hInstance = hInstance;
			wc.hIcon = LoadIcon(0, IDI_APPLICATION);
			wc.hCursor = LoadCursor(0, IDC_ARROW);
			wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
			wc.lpszMenuName = 0;
			wc.lpszClassName = windowClassName.c_str();

			if (!RegisterClass(&wc))
				throw std::runtime_error("Failed to register window class");

			m_isWindowClassRegistered = true;
		}

		// Compute window rectangle dimensions based on requested client area dimensions
		RECT R = { 0,0, config.width, config.height };
		AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
		uint16_t width = R.right - R.left;
		uint16_t height = R.bottom - R.top;

		DWORD style = static_cast<DWORD>(config.style) | static_cast<DWORD>(config.sizeMode);

		// Create the window
		HWND hwnd = CreateWindow(
			windowClassName.c_str(),		// Window class name 
			config.title.c_str(),			// Window title 
			style,							// Window style 
			CW_USEDEFAULT, CW_USEDEFAULT,	// Position 
			config.width, config.height,	// Size 
			nullptr,						// Parent window 
			nullptr,						// Menu 
			hInstance,						// Instance handle 
			nullptr							// Additional application data
		);

		if (!hwnd)
		{
			DWORD error = GetLastError(); // Get Windows Error
			throw std::runtime_error("Failed to create window. Error code: " + std::to_string(error)); // Throw std exception formatted with Windows Error
		}

		// Show the window now that its been created
		ShowWindow(hwnd, SW_SHOW);
		UpdateWindow(hwnd);

		// Return a DisplayTarget initialized with the window's handle
		return DisplayTarget(hwnd);
#elif defined(_LINUX_TARGET)

#elif defined(_MAC_TARGET)

#endif
	}
}