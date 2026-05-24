add_library(sort sort.c)

target_include_directories(sort PUBLIC .)

target_link_libraries(sort PUBLIC ware)