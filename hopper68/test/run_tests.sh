#!/usr/bin/env sh

rm *.prg
rm *.bin
rm *.diff
rm *.txt

vasmm68k_mot tst68000.s -Ftos -devpac -o tst68000.prg
vasmm68k_mot tst68020.s -m68020 -Ftos -o tst68020.prg
vasmm68k_mot tst68030.s -m68030 -Ftos -o tst68030.prg

# Disassemble and compare

echo "test 68000"
../hopper68 tst68000.prg > tst68000.txt
echo "test 68020"
../hopper68 --m68020  tst68020.prg > tst68020.txt
echo "test 68030"
../hopper68 --m68030  tst68030.prg > tst68030.txt
echo "diffs"
DIFF_OPTS="--ignore-blank-lines --ignore-all-space"

diff $DIFF_OPTS tst68000.s tst68000.txt > tst68000.diff
diff $DIFF_OPTS tst68030.s tst68030.txt > tst68030.diff
diff $DIFF_OPTS tst68020.s tst68020.txt > tst68020.diff


