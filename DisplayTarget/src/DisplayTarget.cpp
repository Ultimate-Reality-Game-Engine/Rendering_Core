#include <DisplayTarget.h>

using namespace UltReality::Utilities;

namespace UltReality::Rendering
{
#if defined(_WIN_TARGET)
#include <windows.h>

	void DisplayTarget::Destroy(const EWindowClose&)
	{
		DestroyWindow(static_cast<HWND>(nativeHandle));
	}
#endif
}