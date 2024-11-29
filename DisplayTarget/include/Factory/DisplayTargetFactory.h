#ifndef ULTREALITY_RENDERING_DISPLAY_TARGET_FACTORY_H
#define ULTREALITY_RENDERING_DISPLAY_TARGET_FACTORY_H

#include <stdint.h>
#include <string>

#include <DisplayTarget.h>

namespace UltReality::Rendering
{
#if defined(_WIN_TARGET)
#include <windows.h>
	using MessageHandler = LRESULT(*)(HWND, UINT, WPARAM, LPARAM);
#elif defined(_LINUX_TARGET)
	using MessageHandler = /*Need to define message handler on Linux*/;
#elif defined(_MAC_TARGET)
	using MessageHandler = /*Need to define message handler on MacOS*/;
#endif

	enum class WindowStyle : uint32_t
	{
#if defined(_WIN_TARGET)
		Windowed = WS_OVERLAPPEDWINDOW,
		Fullscreen = WS_POPUP | WS_VISIBLE,
		Borderless = WS_POPUP
#elif defined(_LINUX_TARGET)

#elif defined(_MAC_TARGET)

#endif
	};

	enum class WindowSizeMode : uint32_t
	{
#if defined(_WIN_TARGET)
		Resizable = WS_THICKFRAME,
		FixedSize = 0
#elif defined(_LINUX_TARGET)

#elif defined(_MAC_TARGET)

#endif
	};

	struct WindowConfig
	{
		std::string title;										// Title of the window
		uint16_t width;											// The width of the window in pixels
		uint16_t height;										// The height of the window in pixels
		WindowStyle style = WindowStyle::Windowed;				// The style of the window. Windowed, Fullscreen, or Borderless
		WindowSizeMode sizeMode = WindowSizeMode::Resizable;	// The resizing mode of the window. Resizable, or FixedSize
	};

	/// <summary>
	/// A Factory to create windows specific to the target platform and return them as the platform agnostic <seealso cref="UltReality.Rendering.DisplayTarget"/> structures
	/// </summary>
	class DisplayTargetFactory
	{
	protected:
		// Static member tracks if the window class is registered
		static inline bool m_isWindowClassRegistered = false;

	public:
		/// <summary>
		/// Create a platform specific window and return it as a <seealso cref="UltReality.Rendering.DisplayTarget"/>
		/// </summary>
		/// <param name="config">A <seealso cref="UltReality.Rendering.WindowConfig"/> structure that defines the properties of the window to create</param>
		/// <param name="messageHandler">A pointer to a function to handle the platform specific messages from the created window</param>
		/// <returns><seealso cref="UltReality.Rendering.DisplayTarget"/> instance pointing to the platform specific window</returns>
		/// <exception cref="std.runtime_error">Thrown if the window creation fails</exception>
		static DisplayTarget CreateTarget(const WindowConfig& config, MessageHandler messagehandler = nullptr);


		static DisplayTarget CreateSubTarget();
	};
}

#endif // !ULTREALITY_RENDERING_DISPLAY_TARGET_FACTORY_H
