cmake_minimum_required(VERSION 3.30)
project(Yukon_Cardgame C)

set(CMAKE_C_STANDARD 11)
set(SRC src)

set(GAME_SOURCES
        ${SRC}/card.c
        ${SRC}/game_view.c
        ${SRC}/game_manager.c
        ${SRC}/game_controller.c
        ${SRC}/command_parser.c
        ${SRC}/card_deck.c
        ${SRC}/game_utils.c
)

# Create main executable
add_executable(Yukon_Cardgame ${SRC}/main.c ${GAME_SOURCES})
target_include_directories(Yukon_Cardgame PRIVATE include)

# Do you want tests?
option(BUILD_TESTS "Build with CUnit tests" OFF)

if(BUILD_TESTS)
    # Set paths to your CUnit static library and headers
    set(CUNIT_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/third_party/include)
    set(CUNIT_LIBRARY ${CMAKE_SOURCE_DIR}/third_party/lib/libcunit.a)

    # Make test executable
    add_executable(run_tests tests/test_main.c ${GAME_SOURCES}
            tests/test_main.c)
    target_include_directories(run_tests PRIVATE include ${CUNIT_INCLUDE_DIR})
    target_link_libraries(run_tests ${CUNIT_LIBRARY} m)

    # Register test
    enable_testing()
    add_test(NAME unit_tests COMMAND run_tests)
endif()