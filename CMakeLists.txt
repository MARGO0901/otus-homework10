cmake_minimum_required(VERSION 3.10)
project(async_cli)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(lib)

add_executable(async_cli main.cpp)
target_link_libraries(async_cli libasync)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)