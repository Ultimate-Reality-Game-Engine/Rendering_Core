# CMakeList.txt : CMakeLists for UltReality::Rendering::Renderer_Interface

project(RENDERING_INTERFACE VERSION 1.0.0)

# Create options that are dependent onthis project being top level
option(RENDERER_INTERFACE_VERBOSE "Enable verbose messages for RendererInterface" ${PROJECT_IS_TOP_LEVEL})

message(STATUS "RENDERER_INTERFACE_VERBOSE: ${RENDERER_INTERFACE_VERBOSE}")

# Target Creation *********************************************************************************
#**************************************************************************************************
include(FetchContent)

if(NOT TARGET Utilities)
	# Fetch the Utilities library
	FetchContent_Declare(
		Utilities 
		GIT_REPOSITORY https://github.com/Ultimate-Reality-Game-Engine/Utilities.git 
		GIT_TAG master
	)
	FetchContent_MakeAvailable(Utilities)
endif()

# Create the Renderer Interface library
add_library(RendererInterface INTERFACE)

target_include_directories(RendererInterface INTERFACE 
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> 
 	$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}> # This is used when the library is installed
)

target_link_libraries(RendererInterface INTERFACE DisplayTarget UtilitiesStatic)

set_target_properties(RendererInterface PROPERTIES 
	VERSION ${PROJECT_VERSION} 
	SOVERSION ${PROJECT_VERSION_MAJOR}
)

# End Target Creation *****************************************************************************
#**************************************************************************************************

# Installation and Packing Configuration **********************************************************
#**************************************************************************************************

# Install the library, header files, and config version file

# Install the targets
install(
	TARGETS RendererInterface 
	EXPORT RendererInterface_Targets 
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} # Static libraries/import libraries (.lib files for .dll linking) 
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} # Shared libraries (.so) 
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} # .exe or .dll 
	PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} # Headers/include directories marked as PUBLIC 
	PRIVATE_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} # Headers/include directories marked as PRIVATE
)

# Create the targets CMake file which contains the above definitions
install(
	EXPORT RendererInterface_Targets 
	FILE RendererInterface_Targets.cmake 
	NAMESPACE UltReality::Rendering:: 
	DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/cmake/RendererInterface
)

# Install the actual includes
install(
	DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ 
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/IRenderer
)

# Generate and install the package version config file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
	"RendererInterface_ConfigVersion.cmake" 
	VERSION ${PROJECT_VERSION} 
	COMPATIBILITY SameMajorVersion
)
configure_package_config_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/cmake_config/RendererInterface_Config.cmake.in" 
	"${CMAKE_CURRENT_BINARY_DIR}/RendererInterface_Config.cmake" 
	INSTALL_DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/cmake/RendererInterface
)

# Install the CMake config files
install(
	FILES "${CMAKE_CURRENT_BINARY_DIR}/RendererInterface_ConfigVersion.cmake" 
	"${CMAKE_CURRENT_BINARY_DIR}/RendererInterface_Config.cmake" 
	DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/cmake/RendererInterface
)

# Define Package install paths
set(INCLUDEDIR_FOR_PKG_CONFIG "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}")
set(LIBDIR_PKG_CONFIG "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")

# Create and install the package config file
configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/cmake_config/RendererInterface.pc.in"
	"${CMAKE_CURRENT_BINARY_DIR}/RendererInterface.pc" @ONLY
)

# Install the package config file
install(
	FILES "${CMAKE_CURRENT_BINARY_DIR}/RendererInterface.pc" 
 	DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
)

# A version that is often used to denote a specific build of the software, including revisions, builds, or other metadata
set(PACKAGE_VERSION_BUILD "${CMAKE_SYSTEM_PROCESSOR}-${CMAKE_CXX_COMPILER_ID}-${CMAKE_CXX_COMPILER_VERSION}")

set(PACKAGE_VERSION "${PROJECT_VERSION}-${PACKAGE_VERSION_BUILD}")

set(PACKAGE_DIRECTORY "${CMAKE_SOURCE_DIR}/out/package")
file(MAKE_DIRECTORY "${PACKAGE_DIRECTORY}")

set(CPACK_PACKAGE_NAME "RendererInterface")
set(CPACK_PACKAGE_VERSION "${PACKAGE_VERSION}")

set(CPACK_PACKAGE_VENDOR "Andrew Todd")
set(CPACK_PACKAGE_CONTACT "andrewdanieltodd@gmail.com")
include(CPack)

if(RENDERER_INTERFACE_VERBOSE)
	message(STATUS "PACKAGE_VERSION is: ${PACKAGE_VERSION}")
	message(STATUS "PACKAGE_FILE_NAME is: ${CPACK_PACKAGE_FILE_NAME}")
endif()

# End Installation and Packing Configuration ******************************************************
#**************************************************************************************************
