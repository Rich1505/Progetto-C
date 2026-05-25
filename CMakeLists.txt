cmake_minimum_required(VERSION 3.16)

set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

project(ProgettoC C)

set(CMAKE_C_STANDARD 11)

include(FetchContent)

FetchContent_Declare(
    civetweb
    GIT_REPOSITORY https://github.com/civetweb/civetweb.git
    GIT_TAG v1.16
)


# Scarica e rende disponibili le proprietà del contenuto in un colpo solo
FetchContent_MakeAvailable(civetweb)

# Ora definisci la tua libreria custom usando i sorgenti scaricati
add_library(civetweb
    ${civetweb_SOURCE_DIR}/src/civetweb.c
)

target_include_directories(civetweb PUBLIC
    ${civetweb_SOURCE_DIR}/include
)

target_compile_definitions(civetweb PRIVATE
    NO_SSL
)

if(WIN32)
    target_link_libraries(civetweb PUBLIC ws2_32)
endif()

# Sotto-progetti e target principale
add_subdirectory(src/ware)
add_subdirectory(src/cli)
add_subdirectory(src/web)
add_subdirectory(src/file_manager)
add_subdirectory(src/sort)

add_executable(server
    src/main.c
)

target_link_libraries(server PRIVATE 
    ware
    cli
    web
    file_manager
    sort
    civetweb
)