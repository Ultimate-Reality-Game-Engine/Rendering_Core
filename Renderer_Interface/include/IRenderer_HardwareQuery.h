#ifndef ULTREALITY_RENDERING_IRENDERER_HARDWARE_QUERY_H
#define ULTREALITY_RENDERING_IRENDERER_HARDWARE_QUERY_H

namespace UltReality::Rendering
{
	struct AdapterDesc
	{
		std::string name;
		uint32_t vendorID;
		uint32_t deviceID;
		uint32_t subSysID;
		uint32_t revision;
		size_t videoMemory; // Dedicated video memory in bytes
		size_t sharedMemory; // Shared system memory in bytes
		uint32_t localID; // The index of the adapter used to query it by the implementing Renderer
	};

	struct OutputDesc
	{
		std::string name;
		uint32_t width;
		uint32_t height;
		uint32_t localID;
	};

	struct DisplayMode
	{
		uint32_t width;
		uint32_t height;
		struct Rational
		{
			uint32_t numerator;
			uint32_t denominator;
		};
		Rational refreshRate;
	};
}

#endif // !ULTREALITY_RENDERING_IRENDERER_HARDWARE_QUERY_H
