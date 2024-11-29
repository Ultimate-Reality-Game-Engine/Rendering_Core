# CMakeList.txt : CMakeLists for UltReality::Rendering::

# CMake configuration and settings ****************************************************************
#**************************************************************************************************

cmake_minimum_required(VERSION 3.25)

project(RENDERING_CORE VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define custom configuration types for Debug, Release, and Release-Dev
set(CMAKE_CONFIGURATION_TYPES "Debug;Release;RelWithDebInfo" CACHE STRING "List of supported build modes." FORCE)

# Define compiler definitions to create OS specific code segments
if(WIN32) # Windows target
	add_compile_definitions(TARGET_OS=Windows _WIN_TARGET)
elseif(UNIX AND NOT APPLE) # Linux target
	add_compile_definitions(TARGET_OS=Linux _LINUX_TARGET)
elseif(APPLE) #MacOS target
	add_compile_definitions(TARGET_OS=MacOS _MACOS_TARGET)
endif()

# Make sure CMake exports its compile commands so things like IntelliSense can detect them in editor
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Create options that are dependent on this project being top level
option(RENDERING_CORE_INSTALL "Enable project install" ${PROJECT_IS_TOP_LEVEL})

include(GNUInstallDirs)

# End CMake configuration and settings ************************************************************
#**************************************************************************************************

# Sub Directories *********************************************************************************
#**************************************************************************************************

add_subdirectory(DisplayTarget)
add_subdirectory(Renderer_Interface)

# End Sub Directories *****************************************************************************
#**************************************************************************************************
