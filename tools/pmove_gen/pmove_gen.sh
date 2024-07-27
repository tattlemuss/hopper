#! /usr/bin/env sh

rm *.p56
# Build code of all pmove-using instructions
rmac -Fp -m56001 pmove.asm -o pmove.p56
# Parse the .p56 and source code to generate the table.
python3 pmove_gen.py pmove.asm pmove.p56 ../../src/hopper56/pmove_table.i



