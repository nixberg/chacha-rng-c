#!/bin/sh

flags="-g3 -fPIC -Wall -Werror -Wextra -std=c99 -fsanitize=address,undefined"

mkdir -p .build

clang $flags tests.c chacha.c -o .build/tests

.build/tests

rm -r .build
