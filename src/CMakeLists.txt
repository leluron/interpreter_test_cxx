file(GLOB src "*.cpp")

add_executable (interpreter ${src})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")

target_compile_features(interpreter PRIVATE
    cxx_std_17)