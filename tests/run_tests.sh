#!/usr/bin/env sh

rm test1.prg
rm test_68020.bin

vasmm68k_mot test1.s -Ftos -devpac -o test1.prg
vasmm68k_mot test_68020.s -m68020 -Ftos -o test_68020.bin

# Disassemble and compare

echo "test 1"
../src/bin/hopper test1.prg > test1.txt
echo "test 2"
../src/bin/hopper --m68020  test_68020.bin > test_68020.txt
echo "diffs"
diff --ignore-all-space test1.s test1.txt > test1.diff
diff --ignore-all-space test_68020.s test_68020.txt > test_68020.diff


