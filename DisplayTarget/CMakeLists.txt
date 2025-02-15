# CMakeList.txt : CMakeLists for UltReality::Rendering::DisplayTarget

project(DISPLAY_TARGET VERSION 1.0.0)

# Create options that are dependent onthis project being top level
option(DISPLAY_TARGET_VERBOSE "Enable verbose messages for DisplayTarget" ${PROJECT_IS_TOP_LEVEL})

message(STATUS "DISPLAY_TARGET_VERBOSE: ${DISPLAY_TARGET_VERBOSE}")

# Target Creation *********************************************************************************
#**************************************************************************************************

# Define an INTERFACE target for the DisplayTarget header only
add_library(DisplayTarget INTERFACE)

target_include_directories(DisplayTarget INTERFACE 
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/DisplayTarget> 
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}> # This is used when the library is installed
)

set_target_properties(DisplayTarget PROPERTIES 
	VERSION ${PROJECT_VERSION} 
	SOVERSION ${PROJECT_VERSION_MAJOR}
)

if(NOT DEFINED DISPLAY_TARGET_FACTORY_BUILD)
	set(DISPLAY_TARGET_FACTORY_BUILD OFF)
endif()

if(DISPLAY_TARGET_FACTORY_BUILD)

	# Create list of all source files to be used in this target
	file(GLOB_RECURSE DisplayTargetFactory_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

	if(DisplayTargetFactory_SOURCE)
		# Create the DisplayTargetFactory Interface library
		add_library(DisplayTargetFactory STATIC ${DisplayTargetFactory_SOURCE})

		target_include_directories(DisplayTargetFactory PUBLIC 
			$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/Factory> 
 			$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}> # This is used when the library is installed
		)

		target_link_libraries(DisplayTargetFactory PUBLIC DisplayTarget)

		set_target_properties(DisplayTargetFactory PROPERTIES 
			VERSION ${PROJECT_VERSION} 
			SOVERSION ${PROJECT_VERSION_MAJOR}
		)
	endif()
endif()

# End Target Creation *****************************************************************************
#**************************************************************************************************

# Installation and Packing Configuration **********************************************************
#**************************************************************************************************

# Install the library, header files, and config version file
if(DISPLAY_TARGET_FACTORY_BUILD) 
	set(DisplayTarget_TARGETS DisplayTarget DisplayTargetFactory)
else()
	set(DisplayTarget_TARGETS DisplayTarget)
endif()

# Install the targets
install(
	TARGETS ${DisplayTarget_TARGETS} 
	EXPORT DisplayTarget_Targets 
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} # Static libraries/import libraries (.lib files for .dll linking) 
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} # Shared libraries (.so) 
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} # .exe or .dll 
	PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} # Headers/include directories marked as PUBLIC 
	PRIVATE_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} # Headers/include directories marked as PRIVATE
)

# Create the targets CMake file which contains the above definitions
install(
	EXPORT DisplayTarget_Targets 
	FILE DisplayTarget_Targets.cmake 
	NAMESPACE UltReality::Rendering:: 
	DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/cmake/DisplayTarget
)

# Install the actual includes
install(
	DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ 
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Generate and install the package version config files
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
	"DisplayTarget_ConfigVersion.cmake"
	VERSION ${PROJECT_VERSION} 
	COMPATIBILITY SameMajorVersion
)
configure_package_config_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/cmake_config/DisplayTarget_Config.cmake.in" 
	"${CMAKE_CURRENT_BINARY_DIR}/DisplayTarget_Config.cmake" 
	INSTALL_DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/cmake/DisplayTarget
)

# Install the CMake config files
install(
	FILES "${CMAKE_CURRENT_BINARY_DIR}/DisplayTarget_ConfigVersion.cmake" 
	"${CMAKE_CURRENT_BINARY_DIR}/DisplayTarget_Config.cmake" 
	DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/cmake/DisplayTarget
)

# Define Package install paths
set(INCLUDEDIR_FOR_PKG_CONFIG "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}")
set(LIBDIR_PKG_CONFIG "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")

# Create and install the package config file
configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/cmake_config/DisplayTarget.pc.in" 
	"${CMAKE_CURRENT_BINARY_DIR}/DisplayTarget.pc" @ONLY
)

# Install the package config file
install(
	FILES "${CMAKE_CURRENT_BINARY_DIR}/DisplayTarget.pc" 
 	DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
)

# A version that is often used to denote a specific build of the software, including revisions, builds, or other metadata
set(PACKAGE_VERSION_BUILD "${CMAKE_SYSTEM_PROCESSOR}-${CMAKE_CXX_COMPILER_ID}-${CMAKE_CXX_COMPILER_VERSION}")

set(PACKAGE_VERSION "${PROJECT_VERSION}-${PACKAGE_VERSION_BUILD}")

set(PACKAGE_DIRECTORY "${CMAKE_SOURCE_DIR}/out/package")
file(MAKE_DIRECTORY "${PACKAGE_DIRECTORY}")

set(CPACK_PACKAGE_NAME "DisplayTarget")
set(CPACK_PACKAGE_VERSION "${PACKAGE_VERSION}")

set(CPACK_PACKAGE_VENDOR "Andrew Todd")
set(CPACK_PACKAGE_CONTACT "andrewdanieltodd@gmail.com")
include(CPack)

if(DISPLAY_TARGET_VERBOSE)
	message(STATUS "PACKAGE_VERSION is: ${PACKAGE_VERSION}")
	message(STATUS "PACKAGE_FILE_NAME is: ${CPACK_PACKAGE_FILE_NAME}")
endif()
# End Installation and Packing Configuration ******************************************************
#**************************************************************************************************
