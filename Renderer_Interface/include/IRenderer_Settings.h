#ifndef ULTREALITY_RENDERING_IRENDERER_SETTINGS_H
#define ULTREALITY_RENDERING_IRENDERER_SETTINGS_H

#include <Primitives.h>

namespace UltReality::Rendering
{
	struct DisplaySettings
	{
		Rectangle resolution;
		enum class ScreenMode
		{
			Windowed,
			Borderless,
			Fullscreen
		};
		ScreenMode mode;
		uint16_t refreshRate;
		bool vSync;
	};

	struct AntiAliasingSettings
	{
		enum class AntiAliasingType
		{
			None,
			MSAA,
			FXAA,
			TAA
		};
		AntiAliasingType type;
		uint8_t sampleCount;
		uint8_t qualityLevel;
	};

	struct TextureSettings
	{
		uint8_t filteringLevel;
		enum class TextureQuality
		{
			low,
			medium,
			high,
			ultra
		};
		TextureQuality quality;
		bool mipmapping;
	};

	struct ShadowSettings
	{
		enum class ShadowQuality
		{
			low,
			medium,
			high,
			ultra
		};
		ShadowQuality quality;
		uint16_t mapResolution;
		bool softShadows;
	};

	struct LightingSettings
	{
		bool globalIllumination;
		bool bloom;
		uint8_t bloomIntensity;
		bool hdr;
		bool ambientOcclusion;
		enum class AOQuality
		{
			low,
			medium,
			high,
			ultra
		};
		AOQuality quality;
	};

	struct PostProcessingSettings
	{
		bool bloom;
		uint8_t bloomIntensity;
		bool DOF;
		struct DOFParams
		{

		};
		DOFParams dofParams;
		bool motionBlur;
		uint8_t motionBlurIntensity;
	};

	struct PerformanceSettings
	{
		bool dynamicResolution;
		Rectangle minResolution;
		Rectangle maxResolution;
	};
}

#endif // !ULTREALITY_RENDERING_IRENDERER_SETTINGS_H
