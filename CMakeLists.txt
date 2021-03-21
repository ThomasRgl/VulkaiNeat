

cmake_minimum_required(VERSION 3.0)
project(VulkaiNeat)

set(CMAKE_CXX_STANDARD 17)

include_directories("include")

add_subdirectory(src)


add_executable(main "src/main.cpp")
target_link_libraries(main libutil.a libneuralnet.a libsnake.a)