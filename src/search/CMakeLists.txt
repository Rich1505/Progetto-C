add_library(search search.c)

target_include_directories(search PUBLIC .)

target_link_libraries(search PUBLIC ware)