add_library(file_manager file_manager.c)

target_include_directories(file_manager PUBLIC .)

target_link_libraries(file_manager PUBLIC ware)