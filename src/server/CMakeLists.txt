include_directories(
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_SOURCE_DIR}/thirdparty
)

aux_source_directory(. SRC_DIR)
aux_source_directory(./sql SQL_DIR)
aux_source_directory(./model MODEL_DIR)
aux_source_directory(./service SERVICE_DIR)
aux_source_directory(./net NET_DIR)

add_executable(ChatServer
  ${SRC_DIR}
  ${SQL_DIR}
  ${MODEL_DIR}
  ${SERVICE_DIR}
  ${NET_DIR}
)

target_link_libraries(ChatServer
  muduo_net
  muduo_base
  mysqlclient
  pthread
)