#ifndef ULTREALITY_RENDERING_IRENDERER_PROFILING_H
#define ULTREALITY_RENDERING_IRENDERER_PROFILING_H

namespace UltReality::Rendering
{
	enum class GpuTimingPass
	{
		Frame,			// Total frame GPU time
		ShadowPass,		// Shadow map rendering
		GeometryPass,	// Main geometry rendering
		LightingPass,	// Deferred or forward lighting calculations
		PostProcessing, // Bloom, SSAO, motion blur, etc.
		ComputePass,	// Compute shader-based operations
		UI,				// 2D UI rendering
		Custom0,		// Reserved for user-defined passes
		Custom1,		// Reserved for user-defined passes
		Custom2,		// Reserved for user-defined passes
	};
}

#endif // !ULTREALITY_RENDERING_IRENDERER_PROFILING_H
