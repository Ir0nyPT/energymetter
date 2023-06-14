file(GLOB FILES *.cpp *.h)
add_library(lib ${FILES})

target_compile_definitions(lib PUBLIC)

target_link_libraries(lib hardware_adc )

target_include_directories(lib INTERFACE "${CMAKE_CURRENT_LIST_DIR}/lib")
