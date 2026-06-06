add_library(cli cli.c)

target_include_directories(cli PUBLIC .)

target_link_libraries(cli PUBLIC file_manager)

target_link_libraries(cli PUBLIC sort)

target_link_libraries(cli PUBLIC filter)

target_link_libraries(cli PUBLIC search)

target_link_libraries(cli PUBLIC ware)