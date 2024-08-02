#!/usr/bin/env sh
set -e
sed s/\;.*//g dsp.s > dsp2.s
rm -f *.p56
if ! rmac -Fp -m56001 -DRMAC=1 -DMOT=0 dsp2.s -o dsp.p56; then
	exit 1
fi
tail -c +10 dsp.p56 > dsp2.p56

../hopper56 dsp2.p56 > test.txt

echo "****** diffs: ******"
diff --ignore-all-space --ignore-blank-lines dsp2.s test.txt

