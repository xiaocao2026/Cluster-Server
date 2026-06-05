aux_source_directory(. SRC_LIST)
aux_source_directory(./client CLIENT_LIST)


add_executable(ClusterClient ${SRC_LIST} ${CLIENT_LIST} )

# target_link_libraries(ClusterServer muduo_net muduo_base mysqlclient hiredis pthread)