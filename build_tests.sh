#!/usr/bin/env sh

rm tests/test1.prg
vasmm68k_mot tests/test1.s -Ftos -devpac -o tests/test1.prg


