cmake_minimum_required(VERSION 3.16)
project(yukon C)

set(CMAKE_C_STANDARD 11)

# External dependencies paths
set(SHARED_INCLUDES
        vendored
        vendored/SDL_ttf
        vendored/nativefiledialog-extended
)

# Add external dependencies
add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)
add_subdirectory(vendored/SDL_ttf EXCLUDE_FROM_ALL)
add_subdirectory(vendored/nativefiledialog-extended EXCLUDE_FROM_ALL)

# Create game logic library sources
file(GLOB GAME_LOGIC_SOURCES
        src/core/yukon_core.c
        src/core/model/*.c
        src/core/utils/*.c
        src/core/service/*.c
)

# Create GUI sources
file(GLOB GUI_SOURCES
        src/gui/main.c
        src/gui/controller/*.c
        src/gui/scene/*.c
        src/gui/view/*.c
        src/gui/utils/*.c
        src/gui/service/*.c
)

# Build the game logic library
add_library(yukon_core SHARED ${GAME_LOGIC_SOURCES})
target_include_directories(yukon_core PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core/include
        ${SHARED_INCLUDES}  # Add this line to get access to NFD and other dependencies
)

# Build the GUI application
if(WIN32)
    # Use WIN32 for Windows GUI application
    add_executable(yukon_gui WIN32 ${GUI_SOURCES})
    target_link_libraries(yukon_gui PRIVATE yukon_core SDL2main SDL2 SDL2_ttf nfd)

    # Copy DLLs to output directory
    add_custom_command(
            TARGET yukon_gui POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy $<TARGET_FILE:SDL2::SDL2> $<TARGET_FILE_DIR:yukon_gui>
            COMMAND "${CMAKE_COMMAND}" -E copy $<TARGET_FILE:SDL2_ttf::SDL2_ttf> $<TARGET_FILE_DIR:yukon_gui>
            COMMAND "${CMAKE_COMMAND}" -E copy $<TARGET_FILE:nfd::nfd> $<TARGET_FILE_DIR:yukon_gui>
            COMMAND "${CMAKE_COMMAND}" -E copy $<TARGET_FILE:yukon_core> $<TARGET_FILE_DIR:yukon_gui>
            VERBATIM
    )
else()
    add_executable(yukon_gui ${GUI_SOURCES})
    target_link_libraries(yukon_gui PRIVATE yukon_core SDL2 SDL2_ttf nfd)
endif()

# Set include directories for GUI - notice we point to the core include directory
# but not the core internals
target_include_directories(yukon_gui PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src  # Base directory for simpler includes
        ${CMAKE_CURRENT_SOURCE_DIR}/src/gui
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core/include  # Only public core API
        ${SHARED_INCLUDES}
)

# Add backward compatibility symlink/alias
if(WIN32)
    add_custom_command(TARGET yukon_gui POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:yukon_gui> ${CMAKE_BINARY_DIR}/yukon.exe
            COMMENT "Creating yukon.exe alias for backward compatibility"
    )
else()
    add_custom_command(TARGET yukon_gui POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E create_symlink $<TARGET_FILE:yukon_gui> ${CMAKE_BINARY_DIR}/yukon
            COMMENT "Creating yukon symlink for backward compatibility"
    )
endif()

# Distribution packaging commands
add_custom_command(TARGET yukon_gui POST_BUILD
        # Create necessary directories
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/Release"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/temp_dist"

        # Copy executable, library and assets to temp directory
        COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:yukon_gui>" "${CMAKE_BINARY_DIR}/temp_dist/"
        COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:yukon_core>" "${CMAKE_BINARY_DIR}/temp_dist/"
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/assets" "${CMAKE_BINARY_DIR}/temp_dist/assets"
)

if(WIN32 OR CMAKE_CROSSCOMPILING)
    add_custom_command(TARGET yukon_gui POST_BUILD
            # Copy DLLs for Windows builds when cross-compiling
            COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_BINARY_DIR}/*.dll" "${CMAKE_BINARY_DIR}/temp_dist"
    )
endif()

add_custom_command(TARGET yukon_gui POST_BUILD
        # Create zip file from temp directory (using chdir to avoid -C flag issues)
        COMMAND ${CMAKE_COMMAND} -E chdir "${CMAKE_BINARY_DIR}/temp_dist"
        ${CMAKE_COMMAND} -E tar "cf" "${CMAKE_BINARY_DIR}/Release/yukon.zip" "--format=zip" "."

        # Clean up temp directory
        COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/temp_dist"
        COMMENT "Creating distribution package"
)