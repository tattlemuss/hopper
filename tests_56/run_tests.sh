#!/usr/bin/env sh

rm *.p56
rmac -Fp -m56001 test.asm -o test.p56
tail -c +10 test.p56 > test2.p56

../src/hopper56/test test2.p56 > test.txt

cat test.txt

diff --ignore-all-space --ignore-blank-lines test.asm test.txt

