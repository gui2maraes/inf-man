cmake_minimum_required(VERSION 3.16)
project(inf-man VERSION 1.0 LANGUAGES C)
set(CMAKE_C_STANDARD 11)

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # don't build the supplied example games

FetchContent_Declare(
    raylib
    GIT_REPOSITORY "https://github.com/raysan5/raylib.git"
    GIT_TAG "master"
    GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(raylib)

add_executable(infman src/main.c src/level.c src/player.c src/worldfile.c
    src/game.c src/collision.c src/leaderboard.c src/gui.c src/bullet.c src/enemy.c src/texmanager.c src/animator.c src/utils.c)
target_include_directories(infman PUBLIC include)
target_link_libraries(infman PRIVATE raylib)