#ifndef ULTREALITY_RENDERING_PRIMITIVES_H
#define ULTREALITY_RENDERING_PRIMITIVES_H

#include <stdint.h>

#include <VectorTypes.h>

namespace UltReality::Rendering
{
	struct Rectangle
	{
		uint16_t width;
		uint16_t height;
	};

	struct Vertex
	{
		Math::Float3 pos;
		Math::Float4 color;
	};

	struct ObjectConstants
	{
		Math::Float4x4 worldViewProj = Math::Float4x4::Identity;
	};
}

#endif // !ULTREALITY_RENDERING_PRIMITIVES_H
