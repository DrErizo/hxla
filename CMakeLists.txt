cmake_minimum_required(VERSION 3.14.15926)
project(hxla)
set(CMAKE_CXX_STANDARD 20)
set(SFML_BUILD_AUDIO FALSE)
set(SFML_BUILD_NETWORK FALSE)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT hxla)

add_compile_options(-g)
#add_compile_options(-Os)

file(GLOB_RECURSE SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
)
add_executable(${PROJECT_NAME} ${SOURCES})
if(WIN32 AND CMAKE_SIZEOF_VOID_P EQUAL 4)
	message(STATUS "Building for 32 bit Windows")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:IA32")
elseif(UNIX)
    target_link_libraries(${PROJECT_NAME} PRIVATE pthread)
endif()

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics sfml-window sfml-system)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/sfml)
