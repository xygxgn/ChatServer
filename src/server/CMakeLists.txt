include_directories(
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_SOURCE_DIR}/thirdparty
)

aux_source_directory(./sql SQL_DIR)
aux_source_directory(./model MODEL_DIR)
aux_source_directory(./service SERVICE_DIR)
aux_source_directory(./net NET_DIR)
aux_source_directory(./redis REDIS_DIR)

add_library(chatserver
  ${SQL_DIR}
  ${MODEL_DIR}
  ${SERVICE_DIR}
  ${NET_DIR}
  ${REDIS_DIR}
)

target_link_libraries(chatserver
  muduo_net
  muduo_base
  mysqlclient
  hiredis
  pthread
)

add_executable(ChatServer
  main.cc
)

target_link_libraries(ChatServer
  chatserver
)