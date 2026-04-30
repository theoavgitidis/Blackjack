cmake_minimum_required(VERSION 3.16)
project(BlackjackConsole LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(blackjack_core
    src/Card.cpp
    src/Deck.cpp
    src/Hand.cpp
    src/Participant.cpp
    src/Player.cpp
    src/Dealer.cpp
    src/ConsoleUI.cpp
    src/Game.cpp
)

target_include_directories(blackjack_core PUBLIC include)

add_executable(blackjack src/main.cpp)
target_link_libraries(blackjack PRIVATE blackjack_core)

# Ensure UTF-8 source/execution charset on MSVC (Windows console suit symbols)
if (MSVC)
    target_compile_options(blackjack_core PRIVATE /utf-8)
    target_compile_options(blackjack PRIVATE /utf-8)
endif()

include(CTest)

if (BUILD_TESTING)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    add_executable(blackjack_tests
        tests/CardTest.cpp
        tests/DeckTest.cpp
        tests/HandTest.cpp
        tests/ParticipantTest.cpp
        tests/ConsoleUITest.cpp
        tests/GameTest.cpp
    )

    # MSVC encodes access specifiers into symbol names. Our headers expose some
    # internals as public under BLACKJACK_UNIT_TEST, so the core library must be
    # compiled with the same definition to avoid unresolved externals.
    target_compile_definitions(blackjack_core PRIVATE BLACKJACK_UNIT_TEST)
    target_link_libraries(blackjack_tests PRIVATE blackjack_core GTest::gtest_main)
    target_compile_definitions(blackjack_tests PRIVATE BLACKJACK_UNIT_TEST)

    if (MSVC)
        target_compile_options(blackjack_tests PRIVATE /utf-8)
    endif()

    include(GoogleTest)
    gtest_discover_tests(blackjack_tests)
endif()
