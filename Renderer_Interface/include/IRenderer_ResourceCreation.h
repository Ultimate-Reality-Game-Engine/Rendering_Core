#ifndef ULTREALITY_RENDERING_IRENDERER_RESOURCE_CREATION_H
#define ULTREALITY_RENDERING_IRENDERER_RESOURCE_CREATION_H

namespace UltReality::Rendering
{
	using BufferHandle = size_t;
	using TextureHandle = size_t;
	using ShaderHandle = size_t;

	enum class BufferType
	{
		Vertex,
		Index,
		Constant,
		Structured,
		UnorderedAccess
	};

	enum class BufferUsage
	{
		Static,
		Dynamic
	};
	enum class BufferMemoryType
	{
		Default, // GPUOnly Default heap, not CPU accessible
		Upload, // CPUToGPU Upload heap: CPU writes, GPU reads
		Readback, // GPUToCPU Readback heap: GPU writes, CPU reads
	};

	enum class MapType
	{
		Read,
		Write
	};

	struct TextureDesc
	{
		uint16_t width;
		uint16_t height;
		uint16_t depth = 0; // For 3D textures
		uint16_t mipLevels;
	};

	struct TextureUpdateDesc
	{
		uint16_t x;			// starting x coordinate
		uint16_t y;			// starting y coordinate
		uint16_t x = 0;		// starting z coordinate
		uint16_t width;		// width of the region to update
		uint16_t height;	// height of the region to update
		uint16_t depth;		// depth of the region to update
	};

	enum class ShaderType
	{
		Vertex,
		Pixel,
		Geometry,
		Hull,
		Domain,
		Compute,
		Mesh
	};
}

#endif // !ULTREALITY_RENDERING_IRENDERER_RESOURCE_CREATION_H
