add_executable(adq_node
    src/main.c
    src/node_app.c
)

target_include_directories(adq_node
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(adq_node PRIVATE adq_common)
