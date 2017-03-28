cmake_minimum_required( VERSION 2.8.8 )
project( ProgramOptions )

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic-errors -Wno-deprecated -std=c++11 -fno-rtti" ) # -fno-exceptions -DPROGRAMOPTIONS_NO_EXCEPTIONS # needed for Catch
set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -ggdb -fno-omit-frame-pointer" )
set( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -march=native -DNDEBUG -s -flto" )

include_directories( "./" )
include_directories( "./ext/" )

file( GLOB_RECURSE test_src test/*.cxx )
add_executable( Test ${test_src} )

file( GLOB_RECURSE examples_src examples/*.cxx )
foreach( src_name ${examples_src} )
    # CMake's regex engine doesn't support lazy evaluation (?), hence the hack below
    string( REGEX REPLACE "^.*[^a-zA-Z_]([a-zA-Z_]+)\\.cxx$" "\\1" exe_name ${src_name} )
    add_executable( ${exe_name} EXCLUDE_FROM_ALL ${src_name} )
endforeach( src_name ${examples_src} )
