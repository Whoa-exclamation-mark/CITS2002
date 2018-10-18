cmake_minimum_required(VERSION 3.10)
project(bake! C)

set(CMAKE_C_STANDARD 99)

set(DCMAKE_BUILD_TYPE Debug)

add_executable(bake src/main.c src/datatypes.h src/lexicon/lexicon.c src/lexicon/lexicon.h src/runner/runner.c src/runner/runner.h src/phaser/phaser.c src/phaser/phaser.h src/helpers/helpers.c src/helpers/helpers.h src/history/history.h src/history/md5_history.c src/history/file_history.c src/command/command.c src/command/command.h src/logger/logger.h src/logger/print_log.c src/logger/file_log.c src/history/url_history.c)