cmake_minimum_required(VERSION 3.5)

if(NOT DEFINED PROJECT_NAME)
	set(PROGRAMOPTIONS_NOT_SUBPROJECT ON)
endif()

project(ProgramOptions LANGUAGES CXX VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(ProgramOptionsHxx INTERFACE)
target_include_directories(ProgramOptionsHxx INTERFACE "${CMAKE_CURRENT_LIST_DIR}/include")

if(PROGRAMOPTIONS_NOT_SUBPROJECT)
	option(PROGRAMOPTIONS_NO_COLORS "disable colored output" OFF)
	option(PROGRAMOPTIONS_BUILD_TEST "build test" ON)
	option(PROGRAMOPTIONS_BUILD_EXAMPLES "build examples" ON)

	if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR
		CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR
		CMAKE_CXX_COMPILER_ID MATCHES "Intel")
		set(PROGRAMOPTIONS_GNU_OPTIONS TRUE)
	elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
		set(PROGRAMOPTIONS_MS_OPTIONS TRUE)
	endif()

	if(PROGRAMOPTIONS_GNU_OPTIONS)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -fno-rtti")
		set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -ggdb -fno-omit-frame-pointer")
		set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -march=native -DNDEBUG -s -flto")
	endif()

	if(PROGRAMOPTIONS_NO_COLORS)
		add_definitions(-DPROGRAMOPTIONS_NO_COLORS)
	endif()

	if(PROGRAMOPTIONS_BUILD_TEST)
		add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/ext/Catch2")

		file(GLOB_RECURSE test_src test/*.cxx)
		add_executable(Test ${test_src})
		target_link_libraries(Test
			Catch2::Catch2
			ProgramOptionsHxx
		)
	endif()

	if(PROGRAMOPTIONS_BUILD_EXAMPLES)
		file(GLOB_RECURSE examples_src examples/*.cxx)
		foreach(src_name ${examples_src})
		    # CMake's regex engine doesn't support lazy evaluation (?), hence the hack below
		    string(REGEX REPLACE "^.*[^a-zA-Z_]([a-zA-Z_]+)\\.cxx$" "\\1" exe_name ${src_name})
		    add_executable(${exe_name} ${src_name})

			target_link_libraries(${exe_name} ProgramOptionsHxx)
		endforeach(src_name ${examples_src})
	endif()
endif()
