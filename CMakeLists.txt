cmake_minimum_required(VERSION 3.16)

project(ProgettoC C)

set(CMAKE_C_STANDARD 11)

# Sotto-progetti e target principale
add_subdirectory(src/ware)
add_subdirectory(src/cli)
add_subdirectory(src/file_manager)
add_subdirectory(src/sort)
add_subdirectory(src/search)
add_subdirectory(src/filter)

add_executable(GestioneRichieste
    src/main.c
)

target_link_libraries(GestioneRichieste PRIVATE 
    ware
    cli
    file_manager
    sort
    filter
    search
)