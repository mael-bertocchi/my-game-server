cmake_minimum_required(VERSION 3.10.0)
cmake_policy(SET CMP0167 NEW)

project(r-type_server)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

file(GLOB_RECURSE SRC CONFIGURE_DEPENDS src/*.cpp)

find_package(libconfig CONFIG REQUIRED)
find_package(libpqxx CONFIG REQUIRED)
find_package(Lua 5.4 REQUIRED)

find_path(SOL2_INCLUDE_DIR "sol/abort.hpp")

add_executable(${PROJECT_NAME} ${SRC})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include ${SOL2_INCLUDE_DIR} ${LUA_INCLUDE_DIR})

if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /permissive- /WX)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

if (TARGET libconfig::libconfig++)
    set(LIBCONFIG_LIBRARIES libconfig::libconfig++)
elseif (TARGET libconfig::config++)
    set(LIBCONFIG_LIBRARIES libconfig::config++)
else()
    message(FATAL_ERROR "Could not find a valid libconfig target")
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBCONFIG_LIBRARIES} ${LUA_LIBRARIES} libpqxx::pqxx)
