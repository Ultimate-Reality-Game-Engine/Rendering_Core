#ifndef ULTREALITY_RENDERING_RENDERER_INTERFACE_H
#define ULTREALITY_RENDERING_RENDERER_INTERFACE_H

#include <stdint.h>
#include <string>
#include <vector>
#include <filesystem>

#include <DisplayTarget.h>
#include <PlatformMessageHandler.h>

#include <IRenderer_ResourceCreation.h>
#include <IRenderer_HardwareQuery.h>
#include <IRenderer_Settings.h>
#include <IRenderer_Profiling.h>

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

namespace UltReality::Rendering
{
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
		/// Initialize the renderer and prepare it to receive rendering task requests
		/// </summary>
		/// <param name="viewport">A <see cref="DisplayTarget"/> structure instance that points to a platform specific window instance to render to</param>
		virtual void RENDERER_INTERFACE_CALL Initialize(DisplayTarget viewport) = 0;

		/// <summary>
		/// Create a GPU buffer resource
		/// </summary>
		/// <param name="data">Data to load into buffer at creation</param>
		/// <param name="size">Number of bytes in the data</param>
		/// <param name="usage">Specify if the buffer is static or dynamic</param>
		/// <param name="type">Specify the type of buffer to create. Vertex, Index, Constant, Structured, UnorderedAccess</param>
		/// <returns>A handle to the created GPU buffer</returns>
		virtual BufferHandle RENDERER_INTERFACE_CALL CreateBuffer(const void* data, size_t size, BufferUsage usage, BufferType type, BufferMemoryType memType) = 0;
		
		/// <summary>
		/// Update a GPU buffer resource
		/// </summary>
		/// <param name="handle">Handle to the buffer resource to update</param>
		/// <param name="data">Data to use to update the buffer</param>
		/// <param name="size">Number of bytes in the data</param>
		/// <param name="offset">Number of bytes to offset into the handle's data buffer</param>
		virtual void RENDERER_INTERFACE_CALL UpdateBuffer(BufferHandle handle, const void* data, size_t size, size_t offset = 0) = 0;

		/// <summary>
		/// Destroy a GPU buffer resource
		/// </summary>
		/// <param name="handle">Handle to the buffer resource to destroy</param>
		virtual void RENDERER_INTERFACE_CALL DestroyBuffer(BufferHandle handle) = 0;

		/// <summary>
		/// Map a buffer for CPU access
		/// </summary>
		/// <param name="handle">Handle to the buffer to map to CPU</param>
		/// <param name="size">Number of bytes to map from the buffer</param>
		/// <param name="offset">Byte that the mapping will start at in the buffer</param>
		/// <param name="mapType">Specifies if the map is a Read or Write map (Note: Validity of map will depend on the memory type of the buffer)</param>
		/// <returns>Pointer to the mapped data</returns>
		virtual void* RENDERER_INTERFACE_CALL MapBuffer(BufferHandle handle, size_t size, size_t offset, MapType mapType) = 0;

		/// <summary>
		/// Un-map a buffer from CPU access
		/// </summary>
		/// <param name="handle">Handle to the buffer to un-map</param>
		virtual void RENDERER_INTERFACE_CALL UnmapBuffer(BufferHandle handle) = 0;

		/// <summary>
		/// Copy data from the GPU into a CPU memory region
		/// </summary>
		/// <param name="handle">Handle to the buffer to copy data from</param>
		/// <param name="destination">Location in CPU memory to copy data to</param>
		/// <param name="size">Number of bytes to copy</param>
		/// <param name="offset">Byte that the copy will begin at in the buffer</param>
		virtual void RENDERER_INTERFACE_CALL ReadBuffer(BufferHandle handle, void* destination, size_t size, size_t offset = 0) = 0;

		/// <summary>
		/// Create a GPU texture resource
		/// </summary>
		/// <param name="desc">Description of the texture to be loaded/created</param>
		/// <param name="initialData">Initial data to load into the texture resource</param>
		/// <returns>A handle to the created buffer resource</returns>
		virtual TextureHandle RENDERER_INTERFACE_CALL CreateTexture(const TextureDesc& desc, const void* initialData) = 0;

		/// <summary>
		/// Method that will have the renderer update a GPU texture resource
		/// </summary>
		/// <param name="handle">Handle to the texture resource to update</param>
		/// <param name="data">Data to use to update the texture</param>
		/// <param name="size">Number of bytes in the data</param>
		/// <param name="regionDesc">Description of the region of the texture to update</param>
		virtual void RENDERER_INTERFACE_CALL UpdateTexture(TextureHandle handle, const void* data, size_t size, const TextureUpdateDesc& regionDesc) = 0;

		/// <summary>
		/// Method that will have the renderer destroy a GPU texture resource
		/// </summary>
		/// <param name="handle">Handle to the texture resource to destroy</param>
		virtual void RENDERER_INTERFACE_CALL DestroyTexture(TextureHandle handle) = 0;

		/// <summary>
		/// Method that will have the renderer create a GPU shader resource from source code directly
		/// </summary>
		/// <param name="source">Shader source</param>
		/// <param name="type">Type of shader to compile and load</param>
		/// <returns>A handle to the created shader resource</returns>
		virtual ShaderHandle RENDERER_INTERFACE_CALL CreateShaderFromSource(const std::string& source, ShaderType type) = 0;
		
		/// <summary>
		/// Method that will have the renderer create a GPU shader resource from a source file
		/// </summary>
		/// <param name="filePath">Path to the shader code file</param>
		/// <param name="type">Type of shader to compile and load</param>
		/// <returns>A handle to the created shader resource</returns>
		virtual ShaderHandle RENDERER_INTERFACE_CALL CreateShaderFromSource(const std::filesystem::path& filePath, ShaderType type) = 0;

		/// <summary>
		/// Method that will have the renderer create a GPU shader resource from a pre-compiled shader
		/// </summary>
		/// <param name="filePath">Path to the pre-compiled shader binary</param>
		/// <param name="type">Type of shader to load</param>
		/// <returns>A handle to the created shader resource</returns>
		virtual ShaderHandle RENDERER_INTERFACE_CALL CreateShaderFromBinary(const std::string& filePath, ShaderType type) = 0;

		/// <summary>
		/// Method that will have the renderer destroy a GPU shader resource
		/// </summary>
		/// <param name="handle">Handle to the shader resource to destroy</param>
		/// <returns></returns>
		virtual void RENDERER_INTERFACE_CALL DestroyShader(ShaderHandle handle) = 0;

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
		/// Method that gets descriptions of the available adapters (GPUs)
		/// </summary>
		/// <returns>A collection of descriptions for the systems adapters (GPUs)</returns>
		virtual std::vector<AdapterDesc> RENDERER_INTERFACE_CALL GetDisplayAdapters() const = 0;

		/// <summary>
		/// Method that gets descriptions of the available outputs for a specified adapter (GPU)
		/// </summary>
		/// <param name="adapter">The adapter (GPU) to get output descriptions for</param>
		/// <returns>A collection of descriptions for the adapter's (GPU's) outputs</returns>
		virtual std::vector<OutputDesc> RENDERER_INTERFACE_CALL GetOutputsForAdapter(const AdapterDesc& adapter) const = 0;

		/// <summary>
		/// Method that gets display mode details for a specified output device
		/// </summary>
		/// <param name="output">The output device to get display mode info for</param>
		/// <returns>A collection of available display mode descriptions for the output</returns>
		virtual std::vector<DisplayMode> RENDERER_INTERFACE_CALL GetDisplayModesForOutput(const OutputDesc& output) const = 0;

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

		/// <summary>
		/// Method that will configure the renderer to compute GPU profiling details
		/// </summary>
		/// <param name="enable">Enables or disables GPU profiling</param>
		virtual void RENDERER_INTERFACE_CALL EnableGPUProfiling(bool enable) noexcept = 0;

		/// <summary>
		/// Method that queries the renderers active GPU profiling configuration
		/// </summary>
		/// <returns>True if the renderer is configured to compute GPU profiling details, false otherwise</returns>
		virtual bool RENDERER_INTERFACE_CALL IsGpuProfilingEnabled() const noexcept = 0;

		/// <summary>
		/// Starts a GPU timer section (useful for measuring specific passes)
		/// used in conjunction with <see cref="EndGpuTimer"/>
		/// </summary>
		/// <param name="pass">The rendering pass to begin timing on</param>
		virtual void RENDERER_INTERFACE_CALL BeginGpuTimer(GpuTimingPass pass) noexcept = 0;

		/// <summary>
		/// Ends a GPU timer section (useful for measuring specific passes)
		/// </summary>
		/// <param name="pass"></param>
		/// <returns></returns>
		virtual void RENDERER_INTERFACE_CALL EndGpuTimer(GpuTimingPass pass) noexcept = 0;

		/// <summary>
		/// Retrieves the total GPU frame time in milliseconds
		/// </summary>
		virtual float RENDERER_INTERFACE_CALL GetGpuFrameTime() const noexcept = 0;

		/// <summary>
		/// Retrieves the GPU time for specified pass in milliseconds
		/// </summary>
		/// <param name="pass">The pass to get timing info for</param>
		virtual float RENDERER_INTERFACE_CALL GetGpuTimeForPass(GpuTimingPass pass) const noexcept = 0;

		/// <summary>
		/// Reset the GPU profiling data
		/// </summary>
		virtual void RENDERER_INTERFACE_CALL ResetGpuTimers() noexcept = 0;
	};
}

#endif //!ULTREALITY_RENDERING_RENDERER_INTERFACE_H