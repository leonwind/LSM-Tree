file(GLOB_RECURSE LIB_SOURCES *.cpp)
file(GLOB_RECURSE LIB_HEADERS *.hpp)
add_library(lsm-tree ${LIB_SOURCES} ${LIB_HEADERS})