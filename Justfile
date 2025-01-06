alias b := build
alias r := run
# build project
build :
    make -C debug

run: build
    ./debug/infman