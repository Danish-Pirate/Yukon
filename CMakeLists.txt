cmake_minimum_required(VERSION 3.16)
project(yukon C)

set(CMAKE_C_STANDARD 11)

file(GLOB_RECURSE GAME_SOURCES src/*.c)

# External dependencies
set(SHARED_INCLUDES
        vendored
        vendored/SDL_ttf
        vendored/nativefiledialog-extended
)

# Add external dependencies
add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)
add_subdirectory(vendored/SDL_ttf EXCLUDE_FROM_ALL)
add_subdirectory(vendored/nativefiledialog-extended EXCLUDE_FROM_ALL)

if(WIN32)
    # Use WIN32 for Windows GUI application
    add_executable(yukon WIN32 ${GAME_SOURCES})
    target_link_libraries(yukon PRIVATE SDL2main SDL2 SDL2_ttf nfd)
    add_custom_command(
            TARGET yukon POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy $<TARGET_FILE:SDL2::SDL2> $<TARGET_FILE_DIR:yukon>
            COMMAND "${CMAKE_COMMAND}" -E copy $<TARGET_FILE:SDL2_ttf::SDL2_ttf> $<TARGET_FILE_DIR:yukon>
            COMMAND "${CMAKE_COMMAND}" -E copy $<TARGET_FILE:nfd::nfd> $<TARGET_FILE_DIR:yukon>
            VERBATIM
    )
else()
    add_executable(yukon ${GAME_SOURCES})
    target_link_libraries(yukon PRIVATE SDL2 SDL2_ttf nfd)
endif()

add_custom_command(TARGET yukon POST_BUILD
        # Create necessary directories
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/Release"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/temp_dist"
        # Copy executable and assets to temp directory
        COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:yukon>" "${CMAKE_BINARY_DIR}/temp_dist/"
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/assets" "${CMAKE_BINARY_DIR}/temp_dist/assets"
)
if(WIN32 OR CMAKE_CROSSCOMPILING)
    add_custom_command(TARGET yukon POST_BUILD
            # Copy DLLs for Windows builds when cross-compiling
            COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_BINARY_DIR}/*.dll" "${CMAKE_BINARY_DIR}/temp_dist"
    )
endif ()

target_include_directories(yukon PRIVATE ${SHARED_INCLUDES})

add_custom_command(TARGET yukon POST_BUILD
        # Create zip file from temp directory (using chdir to avoid -C flag issues)
        COMMAND ${CMAKE_COMMAND} -E chdir "${CMAKE_BINARY_DIR}/temp_dist"
        ${CMAKE_COMMAND} -E tar "cf" "${CMAKE_BINARY_DIR}/Release/yukon.zip" "--format=zip" "."

        # Clean up temp directory
        COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/temp_dist"
        COMMENT "Creating distribution package"
)