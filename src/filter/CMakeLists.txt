add_library(filter filter.c)

target_include_directories(filter PUBLIC .)

target_link_libraries(filter PUBLIC ware)