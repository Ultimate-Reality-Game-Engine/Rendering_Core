#ifndef ULTREALITY_RENDERING_PRIMITIVES_H
#define ULTREALITY_RENDERING_PRIMITIVES_H

#include <VectorTypes.h>

namespace UltReality::Rendering
{
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
