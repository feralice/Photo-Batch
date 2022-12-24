FetchContent_Declare(
	stbContents
	GIT_REPOSITORY https://github.com/nothings/stb.git
)

FETCHCONTENT_MAKEAVAILABLE(stbContents)

FETCHCONTENT_GETPROPERTIES(stbContents SOURCE_DIR _stbSourceDir)

add_library(stb INTERFACE)
target_include_directories(stb INTERFACE ${_stbSourceDir})