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
option(RENDERING_CORE_BUILD_TESTS "Build the test suit" ${PROJECT_IS_TOP_LEVEL})

include(GNUInstallDirs)

# End CMake configuration and settings ************************************************************
#**************************************************************************************************

# Create Global Properties ************************************************************************
#**************************************************************************************************

if(RENDERING_CORE_BUILD_TESTS)
	# Create a set for build all unit tests
	set_property(GLOBAL PROPERTY UNIT_TEST_TARGETS "")
	set_property(GLOBAL PROPERTY UNIT_TEST_SOURCES "")
endif()

# End Create Global Properties ********************************************************************
#**************************************************************************************************

# Add Sub-Directories *****************************************************************************
#**************************************************************************************************

add_subdirectory(DisplayTarget)
add_subdirectory(Primitives)
add_subdirectory(Renderer_Interface)

# End Add Sub-Directories *************************************************************************
#**************************************************************************************************

# A version that is often used to denote a specific build of the software, including revisions, builds, or other metadata
set(PACKAGE_VERSION_BUILD "${CMAKE_SYSTEM_PROCESSOR}-${CMAKE_CXX_COMPILER_ID}-${CMAKE_CXX_COMPILER_VERSION}")

set(PACKAGE_VERSION "${PROJECT_VERSION}-${PACKAGE_VERSION_BUILD}")

set(CPACK_PACKAGE_DIRECTORY "${CMAKE_SOURCE_DIR}/out/package")

set(CPACK_PACKAGE_NAME "Rendering_Core")
set(CPACK_PACKAGE_VERSION "${PACKAGE_VERSION}")

set(CPACK_PACKAGE_VENDOR "Andrew Todd")
set(CPACK_PACKAGE_CONTACT "andrewdanieltodd@gmail.com")
include(CPack)

if(UTILITIES_STATIC_VERBOSE)
	message(STATUS "PACKAGE_VERSION is: ${PACKAGE_VERSION}")
	message(STATUS "PACKAGE_FILE_NAME is: ${CPACK_PACKAGE_FILE_NAME}")
endif()

# Create Unit Test Groups *************************************************************************
#**************************************************************************************************
if(RENDERING_CORE_BUILD_TESTS)
	get_property(test_targets GLOBAL PROPERTY UNIT_TEST_TARGETS)
	if(test_targets)
		# Create a custom target for all tests
		add_custom_target(all_unit_tests DEPENDS ${test_targets})
	endif()

	get_property(test_source GLOBAL PROPERTY UNIT_TEST_SOURCES)
	if(test_source)
		# Create an executable for the custom target, such that the IDEs can see it as a runnable target
		add_executable(run_all_unit_tests EXCLUDE_FROM_ALL ${test_source})
		# Link the executable with GTest and the TokenValueParser library
		target_link_libraries(run_all_unit_tests PRIVATE GTest::gtest_main UtilitiesShared UtilitiesStatic)
		set_target_properties(run_all_unit_tests PROPERTIES INSTALLABLE OFF)
	endif()
endif()
# End Create Unit Test Groups *********************************************************************
#**************************************************************************************************
