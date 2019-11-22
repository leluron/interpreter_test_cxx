file(GLOB src "*.cpp" "*.h")

add_executable (interpreter ${src})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")

target_compile_features(interpreter PRIVATE
    cxx_auto_type 
    cxx_nullptr 
    cxx_range_for
    cxx_lambdas
    cxx_override)