cmake_minimum_required(VERSION 3.16)
project(BlackjackConsole LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(blackjack
    src/main.cpp
    src/Card.cpp
    src/Deck.cpp
    src/Hand.cpp
    src/Participant.cpp
    src/Player.cpp
    src/Dealer.cpp
    src/ConsoleUI.cpp
    src/Game.cpp
)

target_include_directories(blackjack PRIVATE include)

# Ensure UTF-8 source/execution charset on MSVC (Windows console suit symbols)
if (MSVC)
    target_compile_options(blackjack PRIVATE /utf-8)
endif()

