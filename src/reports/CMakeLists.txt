add_library(reports reports.c)

target_include_directories(reports PUBLIC .)

target_link_libraries(reports PUBLIC ware)

target_link_libraries(reports PUBLIC sort)