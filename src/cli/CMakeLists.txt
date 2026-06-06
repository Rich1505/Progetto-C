add_library(cli cli.c)

target_include_directories(cli PUBLIC .)

target_link_libraries(cli PUBLIC file_manager)

target_link_libraries(cli PUBLIC sort)

target_link_libraries(cli PUBLIC filters)

target_link_libraries(cli PUBLIC search)

target_link_libraries(cli PUBLIC ware)

target_link_libraries(cli PUBLIC reports)

target_link_libraries(cli PUBLIC stats)