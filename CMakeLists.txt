cmake_minimum_required(VERSION 3.12)

project(PhotoBatch)

add_executable(PhotoBatch 
	"PhotoBatch.cpp" 
	"ArgumentParser.h"
	"ArgumentParser.cpp"
	"Utils.h" 
	"Utils.cpp"
	"Mode.h"
	"Mode.cpp" 
	"RenameMode.cpp" 
	"RenameMode.h"
	"ConvertMode.h"
	"ConvertMode.cpp"
	"ResizeMode.cpp"
	"ResizeMode.h" 
	"ScaleMode.h" 
	"ScaleMode.cpp")

set_target_properties(PhotoBatch PROPERTIES CXX_STANDARD 17)

set(FETCHCONTENT_QUIET OFF CACHE BOOL "disabilita o quiet mode to FechContent" FORCE)
include(FetchContent)

add_subdirectory(Unittests)
add_subdirectory(External)

target_link_libraries(PhotoBatch PRIVATE stb)