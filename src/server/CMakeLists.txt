set(INSITEX_SERVER_TARGET_NAME ${PROJECT_NAME}Server)

file(GLOB_RECURSE INSITEX_SERVER_SOURCES *.cpp)

add_executable(${INSITEX_SERVER_TARGET_NAME} ${INSITEX_SERVER_SOURCES})

target_link_libraries(${INSITEX_SERVER_TARGET_NAME} PUBLIC ${PROJECT_NAME}Ipc)