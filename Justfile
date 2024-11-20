alias b := build
alias r := run
# build project
build:
    ninja -C build

run: build
    ./build/infman