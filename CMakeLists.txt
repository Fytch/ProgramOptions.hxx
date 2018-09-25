cmake_minimum_required( VERSION 3.1 )
project( ProgramOptions )
set( CMAKE_CXX_STANDARD 11 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_CXX_EXTENSIONS OFF )

option( PROGRAMOPTIONS_NO_EXCEPTIONS "disable exceptions" OFF )
option( PROGRAMOPTIONS_NO_COLORS "disable colored output" OFF )
option( PROGRAMOPTIONS_BUILD_TEST "build test" ON )
option( PROGRAMOPTIONS_BUILD_EXAMPLES "build examples" ON )

if( CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR
	CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR
	CMAKE_CXX_COMPILER_ID MATCHES "Intel" )
	set( PROGRAMOPTIONS_GNU_OPTIONS TRUE )
elseif( CMAKE_CXX_COMPILER_ID MATCHES "MSVC" )
	set( PROGRAMOPTIONS_MS_OPTIONS TRUE )
endif()

include_directories( "./" )
include_directories( "./ext/Catch2/single_include/catch2/" )

if( PROGRAMOPTIONS_GNU_OPTIONS )
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wno-deprecated -fno-rtti" )
	set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -ggdb -fno-omit-frame-pointer" )
	set( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -march=native -DNDEBUG -s -flto" )
endif()

if( PROGRAMOPTIONS_NO_EXCEPTIONS )
	add_definitions( -DPROGRAMOPTIONS_NO_EXCEPTIONS )
	if( PROGRAMOPTIONS_GNU_OPTIONS )
		set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions" )
	elseif( PROGRAMOPTIONS_MS_OPTIONS )
		set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc" )
	endif()
endif()

if( PROGRAMOPTIONS_NO_COLORS )
	add_definitions( -DPROGRAMOPTIONS_NO_COLORS )
endif()

if( PROGRAMOPTIONS_BUILD_TEST )
	file( GLOB_RECURSE test_src test/*.cxx )
	add_executable( Test ${test_src} )
endif()

if( PROGRAMOPTIONS_BUILD_EXAMPLES )
	file( GLOB_RECURSE examples_src examples/*.cxx )
	foreach( src_name ${examples_src} )
	    # CMake's regex engine doesn't support lazy evaluation (?), hence the hack below
	    string( REGEX REPLACE "^.*[^a-zA-Z_]([a-zA-Z_]+)\\.cxx$" "\\1" exe_name ${src_name} )
	    add_executable( ${exe_name} ${src_name} )
	endforeach( src_name ${examples_src} )
endif()
