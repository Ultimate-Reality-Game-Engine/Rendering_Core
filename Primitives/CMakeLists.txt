# CMakeList.txt : UltReality::Rendering::Primitives

project(RENDERING_PRIMITIVES VERSION 1.0.0)

# Create options that are dependent onthis project being top level
option(RENDERING_PRIMITIVES_VERBOSE "Enable verbose messages for Rendering Primitives" ${PROJECT_IS_TOP_LEVEL})

message(STATUS "RENDERING_PRIMITIVES_VERBOSE: ${RENDERING_PRIMITIVES_VERBOSE}")

# Target Creation *********************************************************************************
#**************************************************************************************************

include(FetchContent)

if (NOT TARGET UtilitiesShared)
	if(NOT TARGET UtilitiesStatic)
		# Fetch the Utilities library
		FetchContent_Declare(
			Utilities 
			GIT_REPOSITORY https://github.com/Ultimate-Reality-Game-Engine/Utilities.git 
			GIT_TAG master
		)
		FetchContent_MakeAvailable(Utilities)
	endif()
endif()

option(RENDERING_PRIMITIVES_DEBUG "Enable CMake related Debug messages" ON)

file(GLOB_RECURSE RenderingPrimitives_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/*/src/*.cpp")
file(GLOB Primitives_DIRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*)

if(RENDERING_PRIMITIVES_DEBUG)
	message(STATUS "Primitives_DIRS: ${Primitives_DIRS}")
	message(STATUS ${RenderingPrimitives_SOURCE})
endif()

if(NOT DEFINED RENDERING_PRIMITIVES_BUILD)
	set(RENDERING_PRIMITIVES_BUILD ON)
endif()

if(RENDERING_PRIMITIVES_BUILD)
	# Create the RenderingPrimities target
	if(RenderingPrimitives_SOURCE)
		add_library(RenderingPrimitives STATIC ${RenderingPrimitives_SOURCE})

		# Set up include directories for the static library target
		foreach(dir ${Primitives_DIRS})
			if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${dir}/include")
				if(RENDERING_PRIMITIVES_DEBUG)
					message(STATUS "Adding include directory: ${CMAKE_CURRENT_SOURCE_DIR}/${dir}/include")
				endif()
				target_include_directories(RenderingPrimitives PUBLIC 
					$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${dir}/include> 
 					$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/RenderingPrimitives/${dir}> # This is used when the library is installed
				)
			endif()

			if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${dir}/inl")
				if(RENDERING_PRIMITIVES_DEBUG)
					message(STATUS "Adding inl directory: ${CMAKE_CURRENT_SOURCE_DIR}/${dir}/inl")
				endif()
				target_include_directories(RenderingPrimitives PUBLIC 
					$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${dir}/inl> 
 					$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/RenderingPrimitives/${dir}> # This is used when the library is installed
				)
			endif()
		endforeach()

		# Link libraries to the target
		target_link_libraries(RenderingPrimitives UtilitiesStatic)

		set_target_properties(RenderingPrimitives PROPERTIES 
			VERSION ${PROJECT_VERSION} 
			SOVERSION ${PROJECT_VERSION_MAJOR}
		)

# End Target Creation *****************************************************************************
#**************************************************************************************************

# Installation and Packing Configuration **********************************************************
#**************************************************************************************************

		# Install the targets
		install(
			TARGETS RenderingPrimitives
			EXPORT RenderingPrimitives_Targets 
			ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} # Static libraries/import libraries (.lib files for .dll linking) 
			LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} # Shared libraries (.so) 
			RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} # .exe or .dll 
			PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} # Headers/include directories marked as PUBLIC 
			PRIVATE_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} # Headers/include directories marked as PRIVATE
		)

		# Create the targets CMake file which contains the above definitions
		install(
			EXPORT RenderingPrimitives_Targets 
			FILE RenderingPrimitives_Targets.cmake 
			NAMESPACE UltReality::Rendering:: 
			DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/cmake/RenderingPrimitives
		)

		# Install the actual includes
		foreach(dir ${RenderingPrimitives_DIRS})
			if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${dir}/include")
				install(
					DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${dir}/include/"
					DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/RenderingPrimitives/${dir}"
				)
			endif()

			if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${dir}/inl")
				install(
					DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${dir}/inl/"
					DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/RenderingPrimitives/${dir}"
				)
			endif()
		endforeach()

		# Generate and install the package version config files
		include(CMakePackageConfigHelpers)
		write_basic_package_version_file(
			"RenderingPrimitives_ConfigVersion.cmake" 
			VERSION ${PROJECT_VERSION} 
			COMPATIBILITY SameMajorVersion
		)
		configure_package_config_file(
			"${CMAKE_CURRENT_SOURCE_DIR}/cmake_config/RenderingPrimitives_Config.cmake.in" 
			"${CMAKE_CURRENT_BINARY_DIR}/RenderingPrimitives_Config.cmake" 
			INSTALL_DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/cmake/RenderingPrimitives
		)

		# Install the CMake config files
		install(
			FILES "${CMAKE_CURRENT_BINARY_DIR}/RenderingPrimitives_ConfigVersion.cmake" 
			"${CMAKE_CURRENT_BINARY_DIR}/RenderingPrimitives_Config.cmake" 
			DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/cmake/RenderingPrimitives
		)

		# Define Package install paths
		set(INCLUDEDIR_FOR_PKG_CONFIG "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}")
		set(LIBDIR_PKG_CONFIG "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")

		# Create and install the package config file
		configure_file(
			"${CMAKE_CURRENT_SOURCE_DIR}/cmake_config/RenderingPrimitives.pc.in" 
			"${CMAKE_CURRENT_BINARY_DIR}/RenderingPrimitives.pc" @ONLY
		)

		# Install the package config file
		install(
			FILES "${CMAKE_CURRENT_BINARY_DIR}/RenderingPrimitives.pc" 
			DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
		)
	endif()
endif()

# A version that is often used to denote a specific build of the software, including revisions, builds, or other metadata
set(PACKAGE_VERSION_BUILD "${CMAKE_SYSTEM_PROCESSOR}-${CMAKE_CXX_COMPILER_ID}-${CMAKE_CXX_COMPILER_VERSION}")

set(PACKAGE_VERSION "${PROJECT_VERSION}-${PACKAGE_VERSION_BUILD}")

set(CPACK_PACKAGE_DIRECTORY "${CMAKE_SOURCE_DIR}/out/package")

set(CPACK_PACKAGE_NAME "RenderingPrimitives")
set(CPACK_PACKAGE_VERSION "${PACKAGE_VERSION}")

set(CPACK_PACKAGE_VENDOR "Andrew Todd")
set(CPACK_PACKAGE_CONTACT "andrewdanieltodd@gmail.com")
include(CPack)

if(RENDERING_PRIMITIVES_VERBOSE)
	message(STATUS "PACKAGE_VERSION is: ${PACKAGE_VERSION}")
	message(STATUS "PACKAGE_FILE_NAME is: ${CPACK_PACKAGE_FILE_NAME}")
endif()

# End Installation and Packing Configuration ******************************************************
#**************************************************************************************************

# Create Unit Test Groups *************************************************************************
#**************************************************************************************************
if (RENDERING_CORE_BUILD_TESTS)
	if(RENDERING_PRIMITIVES_DEBUG)
		message(STATUS "Building test suit for RenderingPrimitives")
	endif()

	set(RenderingPrimitives_TEST_DIRS "")

	foreach(dir ${RenderingPrimitives_DIRS})
		if(IS_DIRECTORY "${dir}/tests")
			list(APPEND RenderingPrimitives_TEST_DIRS "${dir}/tests")
		endif()
		if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${dir}/tests")
				if(RENDERING_PRIMITIVES_DEBUG)	
					message(STATUS "Adding test directory: ${CMAKE_CURRENT_SOURCE_DIR}/${dir}/tests")
				endif()
				list(APPEND RenderingPrimitives_TEST_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/${dir}/tests")
			endif()
	endforeach()

	# Do not install GTest when packaging targets
	set(INSTALL_GTEST OFF)
	
	# Add all the tests directories
	foreach(tests_dir ${STATIC_TESTS_DIRS})
		if(RENDERING_PRIMITIVES_DEBUG)
			message(STATUS "Adding Sub-Directory: ${tests_dir}")
		endif()
		add_subdirectory("${tests_dir}")
	endforeach()
endif()
# End Create Unit Test Groups *********************************************************************
#**************************************************************************************************