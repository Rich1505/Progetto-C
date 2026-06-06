add_library(filters filters.c)

target_include_directories(filters PUBLIC .)

target_link_libraries(filters PUBLIC ware)