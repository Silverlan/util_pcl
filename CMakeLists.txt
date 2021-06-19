cmake_minimum_required(VERSION 3.12)

set(INCLUDE_DIRS)
function(add_include_dir IDENTIFIER)
	set(PRETTYNAME ${IDENTIFIER})
	set(ARGV ${ARGN})
	list(LENGTH ARGV ARGC)
	if(${ARGC} GREATER 0)
		list(GET ARGV 0 PRETTYNAME)
	endif()
	string(TOUPPER ${IDENTIFIER} UIDENTIFIER)

	set(${DEFAULT_DIR} "")
	set(DEPENDENCY_${UIDENTIFIER}_INCLUDE ${DEFAULT_DIR} CACHE PATH "Path to ${PRETTYNAME} include directory.")
	set(INCLUDE_DIRS ${INCLUDE_DIRS} DEPENDENCY_${UIDENTIFIER}_INCLUDE PARENT_SCOPE)
endfunction(add_include_dir)

set(LIBRARIES)
function(add_external_library IDENTIFIER)
	set(PRETTYNAME ${IDENTIFIER})
	set(ARGV ${ARGN})
	list(LENGTH ARGV ARGC)
	if(${ARGC} GREATER 0)
		list(GET ARGV 0 PRETTYNAME)
	endif()
	string(TOUPPER ${IDENTIFIER} UIDENTIFIER)

	set(DEPENDENCY_${UIDENTIFIER}_LIBRARY "" CACHE FILEPATH "Path to ${PRETTYNAME} library.")
	set(LIBRARIES ${LIBRARIES} DEPENDENCY_${UIDENTIFIER}_LIBRARY PARENT_SCOPE)
endfunction(add_external_library)

function(link_external_library IDENTIFIER)
	set(PRETTYNAME ${IDENTIFIER})
	set(ARGV ${ARGN})
	list(LENGTH ARGV ARGC)
	if(${ARGC} GREATER 0)
		list(GET ARGV 0 PRETTYNAME)
	endif()
	string(TOUPPER ${IDENTIFIER} UIDENTIFIER)

	set(${DEFAULT_DIR} "")
	set(DEPENDENCY_${UIDENTIFIER}_INCLUDE ${DEFAULT_DIR} CACHE PATH "Path to ${PRETTYNAME} include directory.")
	set(INCLUDE_DIRS ${INCLUDE_DIRS} DEPENDENCY_${UIDENTIFIER}_INCLUDE PARENT_SCOPE)

	set(DEPENDENCY_${UIDENTIFIER}_LIBRARY "" CACHE FILEPATH "Path to ${PRETTYNAME} library.")
	set(LIBRARIES ${LIBRARIES} DEPENDENCY_${UIDENTIFIER}_LIBRARY PARENT_SCOPE)
endfunction(link_external_library)

set(BOOST_LIBRARIES)
function(link_boost_library PROJ_NAME LABEL LIB_NAME)
	if(WIN32)
		set(BOOST_LIB_NAME boost_${LIB_NAME}-vc141-mt-x64-1_69.lib)
	else()
		set(BOOST_LIB_NAME libboost_${LIB_NAME}.so.1.69.0)
	endif()
	find_library(${LABEL} ${BOOST_LIB_NAME} HINTS ${DEPENDENCY_BOOST_LIBRARY_LOCATION} NO_DEFAULT_PATH)
	target_link_libraries(${PROJ_NAME} ${${LABEL}})
endfunction(link_boost_library)

##### CONFIGURATION #####

set(PROJ_NAME util_pcl)

project(${PROJ_NAME} CXX)

set(CMAKE_CXX_STANDARD 20)
set(ENABLE_STATIC_LIBRARY_FLAG 0)
# set(PRECOMPILED_HEADER "stdafx")

link_external_library(pcl)
link_external_library(flann)
link_external_library(sharedutils)
link_external_library(mathutil)
link_external_library(vfilesystem)
link_external_library(pcl_search)
link_external_library(pcl_io)
link_external_library(pcl_kdtree)
link_external_library(pcl_features)
link_external_library(pcl_octree)
link_external_library(pcl_surface)
link_external_library(pcl_filters)
link_external_library(pcl_sample_consensus)
link_external_library(pcl_segmentation)

add_include_dir(boost)
add_include_dir(glm)
add_include_dir(eigen)

set(DEFINITIONS
	BOOST_ALL_DYN_LINK
)

##### CONFIGURATION #####

set(LIB_TYPE SHARED)
if(ENABLE_STATIC_LIBRARY_FLAG)
	option(CONFIG_STATIC_LIBRARY "Build as static library?" OFF)
endif()
if(DEFINED ${CONFIG_STATIC_LIBRARY} AND ${CONFIG_STATIC_LIBRARY})
	set(LIB_TYPE STATIC)
endif()

foreach(def IN LISTS DEFINITIONS)
	add_definitions(-D${def})
endforeach(def)

function(def_vs_filters FILE_LIST)
	foreach(source IN LISTS FILE_LIST)
	    get_filename_component(source_path "${source}" PATH)
	    string(REPLACE "${CMAKE_CURRENT_LIST_DIR}" "" source_path_relative "${source_path}")
	    string(REPLACE "/" "\\" source_path_msvc "${source_path_relative}")
	    source_group("${source_path_msvc}" FILES "${source}")
	endforeach()
endfunction(def_vs_filters)

file(GLOB_RECURSE SRC_FILES
    "${CMAKE_CURRENT_LIST_DIR}/src/*.h"
    "${CMAKE_CURRENT_LIST_DIR}/src/*.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/include/*.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/*.hpp"
)
add_library(${PROJ_NAME} ${LIB_TYPE} ${SRC_FILES})
if(WIN32)
	target_compile_options(${PROJ_NAME} PRIVATE /wd4251)
	target_compile_options(${PROJ_NAME} PRIVATE /wd4996)
endif()
def_vs_filters("${SRC_FILES}")

foreach(LIB IN LISTS LIBRARIES)
	target_link_libraries(${PROJ_NAME} ${${LIB}})
endforeach(LIB)

target_include_directories(${PROJ_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)
target_include_directories(${PROJ_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src)

foreach(INCLUDE_PATH IN LISTS INCLUDE_DIRS)
	target_include_directories(${PROJ_NAME} PRIVATE ${${INCLUDE_PATH}})
endforeach(INCLUDE_PATH)

set(TARGET_PROPERTIES LINKER_LANGUAGE CXX)
if(DEFINED PRECOMPILED_HEADER)
	include(PrecompiledHeader.cmake)
	add_precompiled_header(${PROJ_NAME} "src/${PRECOMPILED_HEADER}.h" c++17 FORCEINCLUDE)
endif()
set_target_properties(${PROJ_NAME} PROPERTIES ${TARGET_PROPERTIES})

link_boost_library(${PROJ_NAME} DEPENDENCY_BOOST_THREAD_LIBRARY thread)
link_boost_library(${PROJ_NAME} DEPENDENCY_BOOST_SYSTEM_LIBRARY system)
link_boost_library(${PROJ_NAME} DEPENDENCY_BOOST_DATE_TIME_LIBRARY date_time)
link_boost_library(${PROJ_NAME} DEPENDENCY_BOOST_CHRONO_LIBRARY chrono)
link_boost_library(${PROJ_NAME} DEPENDENCY_BOOST_FILESYSTEM_LIBRARY filesystem)
link_boost_library(${PROJ_NAME} DEPENDENCY_BOOST_IOSTREAMS_LIBRARY iostreams)

