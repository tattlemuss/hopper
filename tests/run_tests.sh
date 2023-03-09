#!/usr/bin/env sh

rm *.prg
rm *.bin
rm *.diff
rm *.txt

vasmm68k_mot test_68000.s -Ftos -devpac -o test_68000.prg
vasmm68k_mot test_68020.s -m68020 -Ftos -o test_68020.bin

# Disassemble and compare

echo "test 1"
../src/bin/hopper test_68000.prg > test_68000.txt
echo "test 2"
../src/bin/hopper --m68020  test_68020.bin > test_68020.txt
echo "diffs"
diff --ignore-all-space test_68000.s test_68000.txt > test_68000.diff
diff --ignore-all-space test_68020.s test_68020.txt > test_68020.diff


