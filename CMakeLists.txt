cmake_minimum_required(VERSION 3.30)
project(Yukon_Cardgame C)

set(CMAKE_C_STANDARD 11)

include_directories(include)

add_executable(Yukon_Cardgame
        src/main.c
        src/card.c
        src/game_view.c
        include/game_view.h
        src/game_manager.c
        include/game_manager.h
        src/game_controller.c
        include/game_controller.h
        src/command_parser.c
        include/command_parser.h
        include/card.h
        include/card_deck.h
        src/card_deck.c
        src/game_utils.c
        include/game_utils.h
)
