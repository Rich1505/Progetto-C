add_library(stats stats.c)

target_include_directories(stats PUBLIC .)

target_link_libraries(stats PUBLIC ware)