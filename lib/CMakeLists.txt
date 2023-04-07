file(GLOB FILES *.cpp *.h)
add_library(lib ${FILES})

target_compile_definitions(lib PUBLIC)

target_include_directories(lib INTERFACE "${CMAKE_CURRENT_LIST_DIR}/lib")
