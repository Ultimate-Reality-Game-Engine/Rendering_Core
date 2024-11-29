#ifndef ULTREALITY_RENDERING_RENDERER_INTERFACE_H
#define ULTREALITY_RENDERING_RENDERER_INTERFACE_H

#include <stdint.h>

#include <DisplayTarget.h>
#include <PlatformMessageHandler.h>

#if defined(_WIN_TARGET)
	#if defined(RENDERER_INTERFACE_EXPORTS)
		#define RENDERER_INTERFACE_ABI __declspec(dllexport)
	#else
		#define RENDERER_INTERFACE_ABI __declspec(dllimport)
	#endif

	#define RENDERER_INTERFACE_CALL __cdecl
#elif defined(_LINUX_TARGET)
	#if defined(RENDER_INTERFACE_EXPORTS)
		#define RENDERER_INTERFACE_ABI __attribute__((visibility("default")))
	#else
		#define RENDERER_INTERFACE_ABI __attribute__((visibility("default")))
	#endif

	#define RENDERER_INTERFACE_CALL __cdecl
#elif(_MACOS_TARGET)
	#if defined(RENDER_INTERFACE_EXPORTS)
		#define RENDERER_INTERFACE_ABI __attribute__((visibility("default")))
	#else
		#define RENDERER_INTERFACE_ABI __attribute__((visibility("default")))
	#endif

	#define RENDERER_INTERFACE_CALL __cdecl
#endif

namespace UltReality::Utilities
{
	class GameTimer;
}

namespace UltReality::Rendering
{
	struct FrameStats
	{
		float fps;
		float mspf;
	};

	struct AdapterDesc
	{
		wchar_t Desc[128];
		uint32_t VendorID;
		uint32_t DeviceID;
		uint32_t SubSysID;
		uint32_t Revision;
		size_t VideoMem;
		size_t SysMem;
		size_t SrdSysMem;
		struct LocalID
		{
			uint32_t low;
			int32_t high;
		};

		LocalID AdapterID;
	};

	struct DisplaySettings
	{
		uint16_t width;
		uint16_t height;
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
		uint16_t targetFPS;
		bool dynamicResolution;
		uint16_t minResolution;
		uint16_t maxResolution;
	};

	/// <summary>
	/// Defines a common agnostic interface for specific renderers to implement so core engine modules can initiate rendering tasks
	/// </summary>
	class RENDERER_INTERFACE_ABI IRenderer
	{
	protected:
		DisplaySettings m_displaySettings;
		AntiAliasingSettings m_antiAliasingSettings;
		TextureSettings m_textureSettings;
		ShadowSettings m_shadowSettings;
		LightingSettings m_lightingSettings;
		PostProcessingSettings m_postProcessingSettings;
		PerformanceSettings m_performanceSettings;

	public:
		virtual ~IRenderer() = default;

		/// <summary>
		/// Method that will initialize the renderer and prepare it to receive rendering task requests
		/// </summary>
		/// <param name="targetWindow">A <seealso cref="UltReality.Rendering.DisplayTarget"/> structure instance that points to a platform specific window instance to render to</param>
		virtual void RENDERER_INTERFACE_CALL Initialize(DisplayTarget targetWindow, const UltReality::Utilities::GameTimer* gameTimer) = 0;

		/// <summary>
		/// Method that produces a buffer in the renderer
		/// </summary>
		virtual void RENDERER_INTERFACE_CALL CreateBuffer() = 0;

		/// <summary>
		/// Method that issues a render call. Purge the render queue
		/// </summary>
		virtual void RENDERER_INTERFACE_CALL Render() = 0;

		/// <summary>
		/// Method that draws the rendered buffer onto the target window
		/// </summary>
		virtual void RENDERER_INTERFACE_CALL Present() = 0;

		/// <summary>
		/// Method that processes the commands queued up the point this method is called
		/// </summary>
		virtual void RENDERER_INTERFACE_CALL FlushCommandQueue() = 0;

		/// <summary>
		/// Method that calculates frame stats
		/// </summary>
		virtual void RENDERER_INTERFACE_CALL CalculateFrameStats(FrameStats* fs) = 0;

		/// <summary>
		/// Method that gets info on available adapters and reports details
		/// </summary>
		/// <returns></returns>
		virtual void RENDERER_INTERFACE_CALL LogAdapters() = 0;

		/// <summary>
		/// Method to set the display settings for the renderer
		/// </summary>
		/// <param name="settings">Instance of <seealso cref="UltReality.Rendering.DisplaySettings"/> struct to get settings from</param>
		virtual void RENDERER_INTERFACE_CALL SetDisplaySettings(const DisplaySettings& settings) = 0;

		/// <summary>
		/// Method to set the anti aliasing settings for the renderer
		/// </summary>
		/// <param name="settings">Instance of <seealso cref="UltReality.Rendering.AntiAliasingSettings"/> struct to get settings from</param>
		virtual void RENDERER_INTERFACE_CALL SetAntiAliasingSettings(const AntiAliasingSettings& settings) = 0;

		/// <summary>
		/// Method to set the texture settings for the renderer
		/// </summary>
		/// <param name="settings">Instance of <seealso cref="UltReality.Rendering.TextureSettings"/> struct to get settings from</param>
		virtual void RENDERER_INTERFACE_CALL SetTextureSettings(const TextureSettings& settings) = 0;

		/// <summary>
		/// Method to set the shadow settings for the renderer
		/// </summary>
		/// <param name="settings">Instance of <seealso cref="UltReality.Rendering.ShadowSettings"/> struct to get settings from</param>
		virtual void RENDERER_INTERFACE_CALL SetShadowSettings(const ShadowSettings& settings) = 0;

		/// <summary>
		/// Method to set the lighting settings for the renderer
		/// </summary>
		/// <param name="settings">Instance of <seealso cref="UltReality.Rendering.LightingSettings"/> struct to get settings from</param>
		virtual void RENDERER_INTERFACE_CALL SetLightingSettings(const LightingSettings& settings) = 0;

		/// <summary>
		/// Method to set the post-processing settings for the renderer
		/// </summary>
		/// <param name="settings">Instance of <seealso cref="UltReality.Rendering.PostProcessingSettings"/> struct to get settings from</param>
		virtual void RENDERER_INTERFACE_CALL SetPostProcessingSettings(const PostProcessingSettings& settings) = 0;

		/// <summary>
		/// Method to set the performance related settings for the renderer
		/// </summary>
		/// <param name="settings">Instance of <seealso cref="UltReality.Rendering.PerformanceSettings"/> struct to get settings from</param>
		virtual void RENDERER_INTERFACE_CALL SetPerformanceSettings(const PerformanceSettings& settings) = 0;

		/// <summary>
		/// Method to get the renderers internal display settings
		/// </summary>
		/// <returns>Immutable reference to renderers display settings <seealso cref="UltReality::Rendering::DisplaySettings"/> instance</returns>
		const DisplaySettings& RENDERER_INTERFACE_CALL GetDisplaySettings() const noexcept
		{
			return m_displaySettings;
		}

		/// <summary>
		/// Method to get the renderers internal anti-aliasing settings
		/// </summary>
		/// <returns>Immutable reference to renderers anti-aliasing settings <seealso cref="UltReality::Rendering::AntiAliasingSettings"/> instance</returns>
		const AntiAliasingSettings& RENDERER_INTERFACE_CALL GetAntiAliasingSettings() const noexcept
		{
			return m_antiAliasingSettings;
		}

		/// <summary>
		/// Method to get the renderers internal texture settings
		/// </summary>
		/// <returns>Immutable reference to renderers texture settings <seealso cref="UltReality::Rendering::TextureSettings"/> instance</returns>
		const TextureSettings& RENDERER_INTERFACE_CALL GetTextureSettings() const noexcept
		{
			return m_textureSettings;
		}

		/// <summary>
		/// Method to get the renderers internal shadow settings
		/// </summary>
		/// <returns>Immutable reference to renderers shadow settings <seealso cref="UltReality::Rendering::ShadowSettings"/> instance</returns>
		const ShadowSettings& RENDERER_INTERFACE_CALL GetShadowSettings() const noexcept
		{
			return m_shadowSettings;
		}

		/// <summary>
		/// Method to get the renderers internal lighting settings
		/// </summary>
		/// <returns>Immutable reference to renderers lighting settings <seealso cref="UltReality::Rendering::LightingSettings"/> instance</returns>
		const LightingSettings& RENDERER_INTERFACE_CALL GetLightingSettings() const noexcept
		{
			return m_lightingSettings;
		}

		/// <summary>
		/// Method to get the renderers internal post-processing settings
		/// </summary>
		/// <returns>Immutable reference to renderers post-processing settings <seealso cref="UltReality::Rendering::PostProcessingSettings"/> instance</returns>
		const PostProcessingSettings& RENDERER_INTERFACE_CALL GetPostProcessingSettings() const noexcept
		{
			return m_postProcessingSettings;
		}

		/// <summary>
		/// Method to get the renderers internal performance settings
		/// </summary>
		/// <returns>Immutable reference to renderers performance settings <seealso cref="UltReality::Rendering::PerformanceSettings"/> instance</returns>
		const PerformanceSettings& RENDERER_INTERFACE_CALL GetPerformanceSettings() const noexcept
		{
			return m_performanceSettings;
		}

		/// <summary>
		/// Method to set the initial display settings for the renderer before initialization
		/// </summary>
		/// <param name="settings">Instance of <seealso cref="UltReality.Rendering.DisplaySettings"/> struct to get settings from</param>
		void RENDERER_INTERFACE_CALL InitDisplaySettings(const DisplaySettings& settings)
		{
			m_displaySettings = settings;
		}

		/// <summary>
		/// Method to set the initial anti aliasing settings for the renderer before initialization
		/// </summary>
		/// <param name="settings">Instance of <seealso cref="UltReality.Rendering.AntiAliasingSettings"/> struct to get settings from</param>
		void RENDERER_INTERFACE_CALL InitAntiAliasingSettings(const AntiAliasingSettings& settings)
		{
			m_antiAliasingSettings = settings;
		}

		/// <summary>
		/// Method to set the initial texture settings for the renderer before initialization
		/// </summary>
		/// <param name="settings">Instance of <seealso cref="UltReality.Rendering.TextureSettings"/> struct to get settings from</param>
		void RENDERER_INTERFACE_CALL InitTextureSettings(const TextureSettings& settings)
		{
			m_textureSettings = settings;
		}

		/// <summary>
		/// Method to set the initial shadow settings for the renderer before initialization
		/// </summary>
		/// <param name="settings">Instance of <seealso cref="UltReality.Rendering.ShadowSettings"/> struct to get settings from</param>
		void RENDERER_INTERFACE_CALL InitShadowSettings(const ShadowSettings& settings)
		{
			m_shadowSettings = settings;
		}

		/// <summary>
		/// Method to set the initial lighting settings for the renderer before initialization
		/// </summary>
		/// <param name="settings">Instance of <seealso cref="UltReality.Rendering.LightingSettings"/> struct to get settings from</param>
		void RENDERER_INTERFACE_CALL InitLightingSettings(const LightingSettings& settings)
		{
			m_lightingSettings = settings;
		}

		/// <summary>
		/// Method to set the initial post-processing settings for the renderer before initialization
		/// </summary>
		/// <param name="settings">Instance of <seealso cref="UltReality.Rendering.PostProcessingSettings"/> struct to get settings from</param>
		void RENDERER_INTERFACE_CALL InitPostProcessingSettings(const PostProcessingSettings& settings)
		{
			m_postProcessingSettings = settings;
		}

		/// <summary>
		/// Method to set the initial performance related settings for the renderer before initialization
		/// </summary>
		/// <param name="settings">Instance of <seealso cref="UltReality.Rendering.PerformanceSettings"/> struct to get settings from</param>
		void RENDERER_INTERFACE_CALL InitPerformanceSettings(const PerformanceSettings& settings)
		{
			m_performanceSettings = settings;
		}
	};
}

#endif //!ULTREALITY_RENDERING_RENDERER_INTERFACE_H