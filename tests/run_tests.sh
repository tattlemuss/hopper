#!/usr/bin/env sh

rm *.prg
rm *.bin
rm *.diff
rm *.txt

vasmm68k_mot test_68000.s -Ftos -devpac -o test_68000.prg
vasmm68k_mot test_68020.s -m68020 -Ftos -o test_68020.prg
vasmm68k_mot test_68030.s -m68030 -Ftos -o test_68030.prg

# Disassemble and compare

echo "test 68000"
../src/bin/hopper test_68000.prg > test_68000.txt
echo "test 68020"
../src/bin/hopper --m68020  test_68020.prg > test_68020.txt
echo "test 68030"
../src/bin/hopper --m68030  test_68030.prg > test_68030.txt
echo "diffs"
DIFF_OPTS="--ignore-blank-lines --ignore-all-space"

diff $DIFF_OPTS test_68000.s test_68000.txt > test_68000.diff
diff $DIFF_OPTS test_68030.s test_68030.txt > test_68030.diff
diff $DIFF_OPTS test_68020.s test_68020.txt > test_68020.diff


