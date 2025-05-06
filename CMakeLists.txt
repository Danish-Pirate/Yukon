cmake_minimum_required(VERSION 3.16)
project(Yukon_Cardgame C)

set(CMAKE_C_STANDARD 11)
set(SRC src)

set(SRC_DIRS
        ${SRC}
        ${SRC}/controller
        ${SRC}/model
        ${SRC}/scene
        ${SRC}/service
        ${SRC}/utils
        ${SRC}/view
)

file(GLOB_RECURSE GAME_SOURCES "${SRC}/*.c")
list(FILTER GAME_SOURCES EXCLUDE REGEX ".*main\\.c$")

# External dependencies
set(SHARED_INCLUDES
        vendored/SDL/include
        vendored/SDL_ttf/include
        vendored/nativefiledialog-extended/include
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)
add_subdirectory(vendored/SDL_ttf EXCLUDE_FROM_ALL)
add_subdirectory(vendored/nativefiledialog-extended EXCLUDE_FROM_ALL)

file(COPY assets DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

if(WIN32)
    add_executable(Yukon_Cardgame WIN32 ${SRC}/main.c ${GAME_SOURCES})
else()
    add_executable(Yukon_Cardgame ${SRC}/main.c ${GAME_SOURCES})
endif()

target_include_directories(Yukon_Cardgame PRIVATE
        ${SHARED_INCLUDES}
        ${SRC}
)

target_link_libraries(Yukon_Cardgame PRIVATE SDL2::SDL2 SDL2_ttf::SDL2_ttf nfd)

option(BUILD_TESTS "Build with CUnit tests" ON)

if(BUILD_TESTS)
    file(GLOB TEST_SOURCES "tests/*.c")
    set(CUNIT_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/vendored/cunit/include)

    find_library(CUNIT_LIBRARY
            NAMES cunit libcunit
            PATHS ${CMAKE_SOURCE_DIR}/vendored/cunit/lib
    )

    if(NOT CUNIT_LIBRARY)
        message(STATUS "CUnit library not found - will use system CUnit")
        find_library(CUNIT_LIBRARY NAMES cunit libcunit)
    endif()

    add_executable(run_tests ${TEST_SOURCES} ${GAME_SOURCES})
    target_include_directories(run_tests PRIVATE
            ${SHARED_INCLUDES}
            ${CUNIT_INCLUDE_DIR}
            ${SRC}
    )
    target_link_libraries(run_tests PRIVATE ${CUNIT_LIBRARY} SDL2::SDL2 SDL2_ttf::SDL2_ttf nfd)

    enable_testing()
    add_test(NAME unit_tests COMMAND run_tests)
endif()