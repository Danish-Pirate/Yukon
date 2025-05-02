cmake_minimum_required(VERSION 3.16)
project(Yukon_Cardgame C)

set(CMAKE_C_STANDARD 11)
set(SRC src)

set(GAME_SOURCES
        ${SRC}/card.c
        ${SRC}/game_manager.c
        ${SRC}/game_controller.c
        ${SRC}/card_deck.c
        ${SRC}/game_utils.c
        ${SRC}/scene_manager.c
        ${SRC}/play_scene.c
        ${SRC}/startup_scene.c
        ${SRC}/texture_manager.c
        ${SRC}/ui_card.c
        ${SRC}/ui_button.c
)

# Set runtime and library output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# Add SDL2 as a subdirectory
add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

# Add SDL_ttf as a subdirectory
add_subdirectory(vendored/SDL_ttf EXCLUDE_FROM_ALL)

# Create main executable
if(WIN32)
    add_executable(Yukon_Cardgame WIN32 ${SRC}/main.c ${GAME_SOURCES})
else()
    add_executable(Yukon_Cardgame ${SRC}/main.c ${GAME_SOURCES})
endif()

target_include_directories(Yukon_Cardgame PRIVATE include)
# Link against both SDL2 and SDL2_ttf
target_link_libraries(Yukon_Cardgame PRIVATE SDL2::SDL2 SDL2_ttf::SDL2_ttf)

# Do you want tests?
option(BUILD_TESTS "Build with CUnit tests" OFF)

if(BUILD_TESTS)
    # Set paths to your CUnit static library and headers
    set(CUNIT_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/vendored/cunit/include)
    set(CUNIT_LIBRARY ${CMAKE_SOURCE_DIR}/vendored/cunit/lib/libcunit.a)

    # Make test executable
    add_executable(run_tests tests/test_main.c ${GAME_SOURCES}
            tests/test_main.c)
    target_include_directories(run_tests PRIVATE include ${CUNIT_INCLUDE_DIR})
    target_link_libraries(run_tests ${CUNIT_LIBRARY} m)

    # Register test
    enable_testing()
    add_test(NAME unit_tests COMMAND run_tests)
endif()