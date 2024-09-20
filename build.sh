#!/bin/bash

build_linux() {
    mkdir -p "exports/linux/$1"
    clang -o "exports/linux/$1/$1" "$1.c" -target "x86_64-linux-gnu" -Llib -Iinclude -lm -l:libraylib.a
}

build_windows() {
    mkdir -p "exports/windows/$1"
    clang -o "exports/windows/$1/$1" "$1.c" -target "x86_64-windows-gnu" -Llib -Iinclude -l:libraylibdll.a
    cp "lib/raylib.dll" "exports/windows/$1/"
}

if [[ $1 == "build" ]]; then
    build_linux "$2"
fi

if [[ $1 == "build-windows" ]]; then
    build_windows "$2"
fi

if [[ $1 == "build-all" ]]; then
    build_linux "$2"
    build_windows "$2"
fi

if [[ $1 == "run" ]]; then
    build_linux "$2"
    "exports/linux/$2/$2"
fi

if [[ $1 == "run-windows" ]]; then
    build_windows "$2"
    "exports/windows/$2/$2.exe"
fi
