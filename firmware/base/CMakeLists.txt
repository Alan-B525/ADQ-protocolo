add_executable(adq_base
    src/main.c
    src/base_app.c
)

target_include_directories(adq_base
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(adq_base PRIVATE adq_common)
