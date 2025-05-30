set(INSITEX_IPC_TARGET_NAME ${PROJECT_NAME}Ipc)

file(GLOB_RECURSE INSITEX_IPC_HEADERS
    include/insitex/ipc/*.h
)
file(GLOB_RECURSE INSITEX_IPC_SOURCES
    src/*.cpp
)

add_library(${INSITEX_IPC_TARGET_NAME} SHARED ${INSITEX_IPC_SOURCES})
target_sources(
    ${INSITEX_IPC_TARGET_NAME}
        PUBLIC FILE_SET HEADERS
        BASE_DIRS include
        FILES ${INSITEX_IPC_HEADERS}
)

target_include_directories(${INSITEX_IPC_TARGET_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/insitex/ipc>
    $<INSTALL_INTERFACE:include/insitex/ipc>
)