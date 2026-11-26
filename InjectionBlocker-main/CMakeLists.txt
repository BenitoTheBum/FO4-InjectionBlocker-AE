cmake_minimum_required(VERSION 3.20)

# ---- Options ----

option(COPY_BUILD "Copy the build output to the Fallout 4 directory." OFF)

# ---- Cache build vars ----

macro(set_from_environment VARIABLE)
	if (NOT DEFINED ${VARIABLE} AND DEFINED ENV{${VARIABLE}})
		set(${VARIABLE} $ENV{${VARIABLE}})
	endif ()
endmacro()

set_from_environment(Fallout4Path)
set_from_environment(VCPKG_ROOT)
set_from_environment(VCPKG_INSTALLATION_ROOT)
set_from_environment(CommonLibF4Path)

if (NOT DEFINED VCPKG_ROOT AND DEFINED VCPKG_INSTALLATION_ROOT)
	set(VCPKG_ROOT "${VCPKG_INSTALLATION_ROOT}")
endif ()

if (NOT DEFINED VCPKG_ROOT AND EXISTS "C:/vcpkg")
	set(VCPKG_ROOT "C:/vcpkg")
endif ()

if (DEFINED VCPKG_ROOT)
	set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
	set(VCPKG_TARGET_TRIPLET "x64-windows-static-md" CACHE STRING "")
else ()
	message(
		WARNING
		"Neither VCPKG_ROOT nor VCPKG_INSTALLATION_ROOT is set. Continuing without vcpkg."
	)
endif ()

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL" CACHE STRING "")

# ---- Project ----

project(
	GLXRM_InjectionBlocker
	VERSION 1.0.0
	LANGUAGES CXX
)

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/cmake/Version.h.in
	${CMAKE_CURRENT_BINARY_DIR}/include/Version.h
	@ONLY
)

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.rc.in
	${CMAKE_CURRENT_BINARY_DIR}/version.rc
	@ONLY
)

# ---- Include guards ----

if (PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
	message(
		FATAL_ERROR
		"In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
	)
endif ()

# ---- Globals ----

if (MSVC)
	if (NOT ${CMAKE_GENERATOR} STREQUAL "Ninja")
		add_compile_options(
			/MP	# Build with Multiple Processes
		)
	endif ()
endif ()

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_DEBUG OFF)

# ---- Dependencies ----

if (NOT TARGET CommonLibF4)
	if (DEFINED CommonLibF4Path AND EXISTS "${CommonLibF4Path}/CMakeLists.txt")
		add_subdirectory("${CommonLibF4Path}" CommonLibF4)
	elseif (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../CommonLibF4/CMakeLists.txt")
		add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../CommonLibF4" CommonLibF4)
	elseif (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/CommonLibF4/CMakeLists.txt")
		add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/CommonLibF4" CommonLibF4)
	else ()
		message(FATAL_ERROR "CommonLibF4 not found.")
	endif ()
endif ()

find_package(spdlog REQUIRED CONFIG)

# ---- Add source files ----

include(cmake/sourcelist.cmake)

source_group(
	TREE ${CMAKE_CURRENT_SOURCE_DIR}
	FILES ${SOURCES}
)

source_group(
	TREE ${CMAKE_CURRENT_BINARY_DIR}
	FILES ${CMAKE_CURRENT_BINARY_DIR}/include/Version.h
)

# ---- Create DLL ----

add_library(
	${PROJECT_NAME}
	SHARED
	${SOURCES}
	${CMAKE_CURRENT_BINARY_DIR}/version.rc
	.clang-format
)

target_compile_definitions(
	${PROJECT_NAME}
	PRIVATE
		_UNICODE
)

target_compile_features(
	${PROJECT_NAME}
	PRIVATE
		cxx_std_23
)

target_include_directories(
	${PROJECT_NAME}
	PRIVATE
		${CMAKE_CURRENT_BINARY_DIR}/include
		${CMAKE_CURRENT_SOURCE_DIR}/include
		${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_directories(
	${PROJECT_NAME}
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/src/detourxs
)

target_link_libraries(
	${PROJECT_NAME}
	PRIVATE
		CommonLibF4::CommonLibF4
		spdlog::spdlog
)

target_precompile_headers(
	${PROJECT_NAME}
	PRIVATE
		src/PCH.h
)

if (MSVC)
	target_compile_options(
		${PROJECT_NAME}
		PRIVATE
			/utf-8	# Set Source and Executable character sets to UTF-8
			/Zi	# Debug Information Format

			/permissive-	# Standards conformance
			/Zc:preprocessor	# Enable preprocessor conformance mode

			/W4	# Warning level

			"$<$<CONFIG:DEBUG>:>"
			"$<$<CONFIG:RELEASE>:/Zc:inline;/JMC-;/Ob3>"
	)

	target_link_options(
		${PROJECT_NAME}
		PRIVATE
			"$<$<CONFIG:DEBUG>:/INCREMENTAL;/OPT:NOREF;/OPT:NOICF>"
			"$<$<CONFIG:RELEASE>:/INCREMENTAL:NO;/OPT:REF;/OPT:ICF;/DEBUG:FULL>"
	)
endif ()

# ---- File copying ----

if (COPY_BUILD)
	if (DEFINED Fallout4Path)
		add_custom_command(
			TARGET ${PROJECT_NAME}
			POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${Fallout4Path}/Data/F4SE/Plugins/
			COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_PDB_FILE:${PROJECT_NAME}> ${Fallout4Path}/Data/F4SE/Plugins/
		)
	else ()
		message(
			WARNING
			"Variable Fallout4Path is not defined. Skipping post-build copy command."
		)
	endif ()
endif ()
