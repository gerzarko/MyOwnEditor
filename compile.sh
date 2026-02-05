#/bin/bash

mkdir -p out
gcc -Wall -Wextra -pedantic -std=c99 -o out/editor code_editor.c
