aux_source_directory(. SRC_DIR)

add_executable(ChatClient ${SRC_DIR})
target_link_libraries(ChatClient pthread)