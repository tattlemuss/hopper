#!/usr/bin/env sh
set -e
rm -f *.p56
if ! rmac -Fp -m56001 test.asm -o test.p56; then
	exit 1
fi
tail -c +10 test.p56 > test2.p56

../hopper56 test2.p56 |more > test.txt

echo "****** diffs: ******"
diff --ignore-all-space --ignore-blank-lines test.asm test.txt
