; This is a modified version of the .mch file from rmac.
; Duplicates are removed and the notes are edited
abs     M_ACC56    M_AM_NONE    PARMOVE %mmmmmmmmmmmmmmmm0010d110  dsp_ab                   d=(a=0, b=1)
asl     M_ACC56    M_AM_NONE    PARMOVE %mmmmmmmmmmmmmmmm0011d010  dsp_ab                   d=(a=0, b=1)
asr     M_ACC56    M_AM_NONE    PARMOVE %mmmmmmmmmmmmmmmm0010d010  dsp_ab                   d=(a=0, b=1)
clr     M_ACC56    M_AM_NONE    PARMOVE %mmmmmmmmmmmmmmmm0001d011  dsp_ab                   d=(a=0, b=1)
lsl     M_ACC56    M_AM_NONE    PARMOVE %mmmmmmmmmmmmmmmm0011d011  dsp_ab                   d=(a=0, b=1)
lsr     M_ACC56    M_AM_NONE    PARMOVE %mmmmmmmmmmmmmmmm0010d011  dsp_ab                   d=(a=0, b=1)
not     M_ACC56    M_AM_NONE    PARMOVE %mmmmmmmmmmmmmmmm0001d111  dsp_ab                   d=(a=0, b=1)
addl    M_ACC56    M_ACC56      PARMOVE %mmmmmmmmmmmmmmmm0001d010  dsp_baab                 d=(b,a=0, a,b=1)
addr    M_ACC56    M_ACC56      PARMOVE %mmmmmmmmmmmmmmmm0000d010  dsp_baab                 d=(b,a=0, a,b=1)
add     M_ACC56    M_ACC56      PARMOVE %mmmmmmmmmmmmmmmm0001d000  dsp_baab           +     d=(a=0, b=1)
-       M_ALL48    M_ACC56      PARMOVE %mmmmmmmmmmmmmmmm0jjjd000  dsp_acc48                jjj=(x=2, y=3, x0=4, y0=5, x1=6, y1=7), d=(a=0, b=1)
cmp     M_ACC56    M_ACC56      PARTEST %mmmmmmmmmmmmmmmm0000d101  dsp_baab           +     d=(a=0, b=1)
-       M_ALU24    M_ACC56      PARTEST %mmmmmmmmmmmmmmmm0jjjd101  dsp_acc48                jjj=(x=2, y=3, x0=4, y0=5, x1=6, y1=7), d=(a=0, b=1)
cmpm    M_ACC56    M_ACC56      PARTEST %mmmmmmmmmmmmmmmm0000d111  dsp_baab           +     d=(a=0, b=1)
-       M_ALU24    M_ACC56      PARTEST %mmmmmmmmmmmmmmmm0jjjd111  dsp_acc48                jjj=(x=2, y=3, x0=4, y0=5, x1=6, y1=7), d=(a=0, b=1)
sub     M_ACC56    M_ACC56      PARMOVE %mmmmmmmmmmmmmmmm0001d100  dsp_baab           +     d=(a=0, b=1)
-       M_ALL48    M_ACC56      PARMOVE %mmmmmmmmmmmmmmmm0jjjd100  dsp_acc48                jjj=(x=2, y=3, x0=4, y0=5, x1=6, y1=7), d=(a=0, b=1)
tfr     M_ACC56    M_ACC56      PARMOVE %mmmmmmmmmmmmmmmm0000d001  dsp_baab           +     d=(a=0, b=1)
-       M_ALU24    M_ACC56      PARMOVE %mmmmmmmmmmmmmmmm0jjjd001  dsp_acc48                jjj=(x=2, y=3, x0=4, y0=5, x1=6, y1=7), d=(a=0, b=1)
rnd     M_ACC56    M_AM_NONE    PARMOVE %mmmmmmmmmmmmmmmm0001d001  dsp_ab                   d=(a=0, b=1)
rol     M_ACC56    M_AM_NONE    PARMOVE %mmmmmmmmmmmmmmmm0011d111  dsp_ab                   d=(a=0, b=1)
ror     M_ACC56    M_AM_NONE    PARMOVE %mmmmmmmmmmmmmmmm0010d111  dsp_ab                   d=(a=0, b=1)
subl    M_ACC56    M_ACC56      PARMOVE %mmmmmmmmmmmmmmmm0001d110  dsp_baab                 d=(b,a=0, a,b=1)
subr    M_ACC56    M_ACC56      PARMOVE %mmmmmmmmmmmmmmmm0000d110  dsp_baab                 d=(b,a=0, a,b=1)
tst     M_ACC56    M_AM_NONE    PARTEST %mmmmmmmmmmmmmmmm0000d011  dsp_ab                   d=(a=0, b=1)
enddo   M_AM_NONE  M_AM_NONE    NOPARMO %000000000000000010001100  dsp_self
illegal M_AM_NONE  M_AM_NONE    NOPARMO %000000000000000000000101  dsp_self
nop     M_AM_NONE  M_AM_NONE    NOPARMO %000000000000000000000000  dsp_self
reset   M_AM_NONE  M_AM_NONE    NOPARMO %000000000000000010000100  dsp_self
rti     M_AM_NONE  M_AM_NONE    NOPARMO %000000000000000000000100  dsp_self
rts     M_AM_NONE  M_AM_NONE    NOPARMO %000000000000000000001100  dsp_self
stop    M_AM_NONE  M_AM_NONE    NOPARMO %000000000000000010000111  dsp_self
swi     M_AM_NONE  M_AM_NONE    NOPARMO %000000000000000000000110  dsp_self
wait    M_AM_NONE  M_AM_NONE    NOPARMO %000000000000000010000110  dsp_self
adc     M_INP48    M_ACC56      PARMOVE %mmmmmmmmmmmmmmmm001jd001  dsp_xyab                 j=(x=0, y=1), d=(a=0, b=1)
sbc     M_INP48    M_ACC56      PARMOVE %mmmmmmmmmmmmmmmm001jd101  dsp_xyab                 s1 (j)=(x=0,y=1),s2 (d)=(a=0,b=1)
and     M_ALU24    M_ACC56      PARMOVE %mmmmmmmmmmmmmmmm01jjd110  dsp_x0y0ab               jj=(x0=0, x1=2, y0=1, y1=3), d=(a=0, b=1)
eor     M_ALU24    M_ACC56      PARMOVE %mmmmmmmmmmmmmmmm01jjd011  dsp_x0y0ab               jj=(x0=0, x1=2, y0=1, y1=3), d=(a=0, b=1)
div     M_ALU24    M_ACC56      NOPARMO %000000011000000001jjd000  dsp_x0y0ab               jj=(x0=0, x1=2, y0=1, y1=3), d=(a=0, b=1)
or      M_ALU24    M_ACC56      PARMOVE %mmmmmmmmmmmmmmmm01jjd010  dsp_x0y0ab               jj=(x0=0, x1=2, y0=1, y1=3), d=(a=0, b=1)
andi    M_DSPIM8   M_DSPPCU     NOPARMO %00000000iiiiiiii101110ee  dsp_immcr                ee=(mr=0, ccr=1, omr=2)
ori     M_DSPIM8   M_DSPPCU     NOPARMO %00000000iiiiiiii111110ee  dsp_immcr                ee=(mr=0, ccr=1, omr=2)
tcc     M_ACC56    M_ACC56      NOPARMO %0000001e00000ttt0jjjdTTT  dsp_baab           +     s1,d1 [s2,d2]
;-       M_ALL48    M_ACC56      NOPARMO %0000001e00000ttt0jjjdTTT  dsp_tcc2                 s1,d1 [s2,d2]
;ths     M_ACC56    M_ACC56      NOPARMO %0000001e00000ttt0jjjdTTT  dsp_baab           +     s1,d1 [s2,d2]
;-       M_ALL48    M_ACC56      NOPARMO %0000001e00000ttt0jjjdTTT  dsp_tcc2                 s1,d1 [s2,d2]
tcs     M_ACC56    M_ACC56      NOPARMO %0000001e10000ttt0jjjdTTT  dsp_baab           +     s1,d1 [s2,d2]
;-       M_ALL48    M_ACC56      NOPARMO %0000001e100000sssjjjd000  dsp_tcc2                 s1,d1 [s2,d2]
;tlo     M_ACC56    M_ACC56      NOPARMO %0000001e10000ttt0jjjdTTT  dsp_baab           +     s1,d1 [s2,d2]
;-       M_ALL48    M_ACC56      NOPARMO %0000001e10000ttt0jjjdTTT  dsp_tcc2                 s1,d1 [s2,d2]
tec     M_ACC56    M_ACC56      NOPARMO %0000001e01010ttt0jjjdTTT  dsp_baab           +     s1,d1 [s2,d2]
;-       M_ALL48    M_ACC56      NOPARMO %0000001e01010ttt0jjjdTTT  dsp_tcc2                 s1,d1 [s2,d2]
teq     M_ACC56    M_ACC56      NOPARMO %0000001e10100ttt0jjjdTTT  dsp_baab           +     s1,d1 [s2,d2]
;-       M_ALL48    M_ACC56      NOPARMO %0000001e10100ttt0jjjdTTT  dsp_tcc2                 s1,d1 [s2,d2]
tes     M_ACC56    M_ACC56      NOPARMO %0000001e11010ttt0jjjdTTT  dsp_baab           +     s1,d1 [s2,d2]
;-       M_ALL48    M_ACC56      NOPARMO %0000001e11010ttt0jjjdTTT  dsp_tcc2                 s1,d1 [s2,d2]
tge     M_ACC56    M_ACC56      NOPARMO %0000001e00010ttt0jjjdTTT  dsp_baab           +     s1,d1 [s2,d2]
;-       M_ALL48    M_ACC56      NOPARMO %0000001e00010ttt0jjjdTTT  dsp_tcc2                 s1,d1 [s2,d2]
tgt     M_ACC56    M_ACC56      NOPARMO %0000001e01110ttt0jjjdTTT  dsp_baab           +     s1,d1 [s2,d2]
;-       M_ALL48    M_ACC56      NOPARMO %0000001e01110ttt0jjjdTTT  dsp_tcc2                 s1,d1 [s2,d2]
tlc     M_ACC56    M_ACC56      NOPARMO %0000001e01100ttt0jjjdTTT  dsp_baab           +     s1,d1 [s2,d2]
;-       M_ALL48    M_ACC56      NOPARMO %0000001e01100ttt0jjjdTTT  dsp_tcc2                 s1,d1 [s2,d2]
tle     M_ACC56    M_ACC56      NOPARMO %0000001e11110ttt0jjjdTTT  dsp_baab           +     s1,d1 [s2,d2]
;-       M_ALL48    M_ACC56      NOPARMO %0000001e11110ttt0jjjdTTT  dsp_tcc2                 s1,d1 [s2,d2]
tls     M_ACC56    M_ACC56      NOPARMO %0000001e11100ttt0jjjdTTT  dsp_baab           +     s1,d1 [s2,d2]
;-       M_ALL48    M_ACC56      NOPARMO %0000001e11100ttt0jjjdTTT  dsp_tcc2                 s1,d1 [s2,d2]
tlt     M_ACC56    M_ACC56      NOPARMO %0000001e10010ttt0jjjdTTT  dsp_baab           +     s1,d1 [s2,d2]
;-       M_ALL48    M_ACC56      NOPARMO %0000001e10010ttt0jjjdTTT  dsp_tcc2                 s1,d1 [s2,d2]
tmi     M_ACC56    M_ACC56      NOPARMO %0000001e10110ttt0jjjdTTT  dsp_baab           +     s1,d1 [s2,d2]
;-       M_ALL48    M_ACC56      NOPARMO %0000001e10110ttt0jjjdTTT  dsp_tcc2                 s1,d1 [s2,d2]
tne     M_ACC56    M_ACC56      NOPARMO %0000001e00100ttt0jjjdTTT  dsp_baab           +     s1,d1 [s2,d2]
;-       M_ALL48    M_ACC56      NOPARMO %0000001e00100ttt0jjjdTTT  dsp_tcc2                 s1,d1 [s2,d2]
tnr     M_ACC56    M_ACC56      NOPARMO %0000001e11000ttt0jjjdTTT  dsp_baab           +     s1,d1 [s2,d2]
;-       M_ALL48    M_ACC56      NOPARMO %0000001e11000ttt0jjjdTTT  dsp_tcc2                 s1,d1 [s2,d2]
tpl     M_ACC56    M_ACC56      NOPARMO %0000001e00110ttt0jjjdTTT  dsp_baab           +     s1,d1 [s2,d2]
;-       M_ALL48    M_ACC56      NOPARMO %0000001e00110ttt0jjjdTTT  dsp_tcc2                 s1,d1 [s2,d2]
tnn     M_ACC56    M_ACC56      NOPARMO %0000001e01000ttt0jjjdTTT  dsp_baab           +     s1,d1 [s2,d2]
;-       M_ALL48    M_ACC56      NOPARMO %0000001e01000ttt0jjjdTTT  dsp_tcc2                 s1,d1 [s2,d2]
jcc     M_DSPABS12 M_AM_NONE    NOPARMO %000011100000aaaaaaaaaaaa  dsp_abs12          +     Jcc xxx aaaaaaaaaaaa=12bit address
-       C_DSPABSEA M_AM_NONE    NOPARMO %0000101011mmmrrr10100000  dsp_ea                   Jcc ea mmmrrr=ea
;jhs     M_DSPABS12 M_AM_NONE    NOPARMO %000011100000aaaaaaaaaaaa  dsp_abs12          +     Jcc xxx aaaaaaaaaaaa=12bit address
;-       C_DSPABSEA M_AM_NONE    NOPARMO %0000101011mmmrrr10100000  dsp_ea                   Jcc ea mmmrrr=ea
jcs     M_DSPABS12 M_AM_NONE    NOPARMO %000011101000aaaaaaaaaaaa  dsp_abs12          +     Jcc xxx aaaaaaaaaaaa=12bit address
-       C_DSPABSEA M_AM_NONE    NOPARMO %0000101011mmmrrr10101000  dsp_ea                   Jcc ea mmmrrr=ea
;jlo     M_DSPABS12 M_AM_NONE    NOPARMO %000011101000aaaaaaaaaaaa  dsp_abs12          +     Jcc xxx aaaaaaaaaaaa=12bit address
;-       C_DSPABSEA M_AM_NONE    NOPARMO %0000101011mmmrrr10101000  dsp_ea                   Jcc ea mmmrrr=ea
jec     M_DSPABS12 M_AM_NONE    NOPARMO %000011100101aaaaaaaaaaaa  dsp_abs12          +     Jcc xxx aaaaaaaaaaaa=12bit address
-       C_DSPABSEA M_AM_NONE    NOPARMO %0000101011mmmrrr10100101  dsp_ea                   Jcc ea mmmrrr=ea
jeq     M_DSPABS12 M_AM_NONE    NOPARMO %000011101010aaaaaaaaaaaa  dsp_abs12          +     Jcc xxx aaaaaaaaaaaa=12bit address
-       C_DSPABSEA M_AM_NONE    NOPARMO %0000101011mmmrrr10101010  dsp_ea                   Jcc ea mmmrrr=ea
jes     M_DSPABS12 M_AM_NONE    NOPARMO %000011101101aaaaaaaaaaaa  dsp_abs12          +     Jcc xxx aaaaaaaaaaaa=12bit address
-       C_DSPABSEA M_AM_NONE    NOPARMO %0000101011mmmrrr10101101  dsp_ea                   Jcc ea mmmrrr=ea
jge     M_DSPABS12 M_AM_NONE    NOPARMO %000011100001aaaaaaaaaaaa  dsp_abs12          +     Jcc xxx aaaaaaaaaaaa=12bit address
-       C_DSPABSEA M_AM_NONE    NOPARMO %0000101011mmmrrr10100001  dsp_ea                   Jcc ea mmmrrr=ea
jgt     M_DSPABS12 M_AM_NONE    NOPARMO %000011100111aaaaaaaaaaaa  dsp_abs12          +     Jcc xxx aaaaaaaaaaaa=12bit address
-       C_DSPABSEA M_AM_NONE    NOPARMO %0000101011mmmrrr10100111  dsp_ea                   Jcc ea mmmrrr=ea
;jge     M_DSPABS12 M_AM_NONE    NOPARMO %000011100001aaaaaaaaaaaa  dsp_abs12          +     Jcc xxx aaaaaaaaaaaa=12bit address
;-       C_DSPABSEA M_AM_NONE    NOPARMO %0000101011mmmrrr10100001  dsp_ea                   Jcc ea mmmrrr=ea
jlc     M_DSPABS12 M_AM_NONE    NOPARMO %000011100110aaaaaaaaaaaa  dsp_abs12          +     Jcc xxx aaaaaaaaaaaa=12bit address
-       C_DSPABSEA M_AM_NONE    NOPARMO %0000101011mmmrrr10100110  dsp_ea                   Jcc ea mmmrrr=ea
jle     M_DSPABS12 M_AM_NONE    NOPARMO %000011101111aaaaaaaaaaaa  dsp_abs12          +     Jcc xxx aaaaaaaaaaaa=12bit address
-       C_DSPABSEA M_AM_NONE    NOPARMO %0000101011mmmrrr10101111  dsp_ea                   Jcc ea mmmrrr=ea
jls     M_DSPABS12 M_AM_NONE    NOPARMO %000011101110aaaaaaaaaaaa  dsp_abs12          +     Jcc xxx aaaaaaaaaaaa=12bit address
-       C_DSPABSEA M_AM_NONE    NOPARMO %0000101011mmmrrr10101110  dsp_ea                   Jcc ea mmmrrr=ea
jlt     M_DSPABS12 M_AM_NONE    NOPARMO %000011101001aaaaaaaaaaaa  dsp_abs12          +     Jcc xxx aaaaaaaaaaaa=12bit address
-       C_DSPABSEA M_AM_NONE    NOPARMO %0000101011mmmrrr10101001  dsp_ea                   Jcc ea mmmrrr=ea
jmi     M_DSPABS12 M_AM_NONE    NOPARMO %000011101011aaaaaaaaaaaa  dsp_abs12          +     Jcc xxx aaaaaaaaaaaa=12bit address
-       C_DSPABSEA M_AM_NONE    NOPARMO %0000101011mmmrrr10101011  dsp_ea                   Jcc ea mmmrrr=ea
jne     M_DSPABS12 M_AM_NONE    NOPARMO %000011100010aaaaaaaaaaaa  dsp_abs12          +     Jcc xxx aaaaaaaaaaaa=12bit address
-       C_DSPABSEA M_AM_NONE    NOPARMO %0000101011mmmrrr10100010  dsp_ea                   Jcc ea mmmrrr=ea
jnr     M_DSPABS12 M_AM_NONE    NOPARMO %000011101100aaaaaaaaaaaa  dsp_abs12          +     Jcc xxx aaaaaaaaaaaa=12bit address
-       C_DSPABSEA M_AM_NONE    NOPARMO %0000101011mmmrrr10101100  dsp_ea                   Jcc ea mmmrrr=ea
jpl     M_DSPABS12 M_AM_NONE    NOPARMO %000011100011aaaaaaaaaaaa  dsp_abs12          +     Jcc xxx aaaaaaaaaaaa=12bit address
-       C_DSPABSEA M_AM_NONE    NOPARMO %0000101011mmmrrr10100011  dsp_ea                   Jcc ea mmmrrr=ea
jnn     M_DSPABS12 M_AM_NONE    NOPARMO %000011100100aaaaaaaaaaaa  dsp_abs12          +     Jcc xxx aaaaaaaaaaaa=12bit address
-       C_DSPABSEA M_AM_NONE    NOPARMO %0000101011mmmrrr10100100  dsp_ea                   Jcc ea mmmrrr=ea
jmp     M_DSPABS12 M_AM_NONE    NOPARMO %000011000000aaaaaaaaaaaa  dsp_abs12          +     JMP  xxx aaaaaaaaaaaa=12bit address
-       C_DSPABSEA M_AM_NONE    NOPARMO %0000101011mmmrrr10000000  dsp_ea                   JMP ea (+optional 24bit address)
jscc    M_DSPABS12 M_AM_NONE    NOPARMO %000011110000aaaaaaaaaaaa  dsp_abs12          +     JScc xxx aaaaaaaaaaaa=12bit address
-       C_DSPABSEA M_AM_NONE    NOPARMO %0000101111mmmrrr10100000  dsp_ea                   JScc ea mmmrrr=ea (+optional 24bit address)
;jshs    M_DSPABS12 M_AM_NONE    NOPARMO %000011110000aaaaaaaaaaaa  dsp_abs12          +     JScc xxx aaaaaaaaaaaa=12bit address
;-       C_DSPABSEA M_AM_NONE    NOPARMO %0000101111mmmrrr10100000  dsp_ea                   JScc ea mmmrrr=ea (+optional 24bit address)
jscs    M_DSPABS12 M_AM_NONE    NOPARMO %000011111000aaaaaaaaaaaa  dsp_abs12          +     JScc xxx aaaaaaaaaaaa=12bit address
-       C_DSPABSEA M_AM_NONE    NOPARMO %0000101111mmmrrr10101000  dsp_ea                   JScc ea mmmrrr=ea (+optional 24bit address)
;jslo    M_DSPABS12 M_AM_NONE    NOPARMO %000011111000aaaaaaaaaaaa  dsp_abs12          +     JScc xxx aaaaaaaaaaaa=12bit address
;-       C_DSPABSEA M_AM_NONE    NOPARMO %0000101111mmmrrr10101000  dsp_ea                   JScc ea mmmrrr=ea (+optional 24bit address)
jsec    M_DSPABS12 M_AM_NONE    NOPARMO %000011110101aaaaaaaaaaaa  dsp_abs12          +     JScc xxx aaaaaaaaaaaa=12bit address
-       C_DSPABSEA M_AM_NONE    NOPARMO %0000101111mmmrrr10100101  dsp_ea                   JScc ea mmmrrr=ea (+optional 24bit address)
jseq    M_DSPABS12 M_AM_NONE    NOPARMO %000011111010aaaaaaaaaaaa  dsp_abs12          +     JScc xxx aaaaaaaaaaaa=12bit address
-       C_DSPABSEA M_AM_NONE    NOPARMO %0000101111mmmrrr10101010  dsp_ea                   JScc ea mmmrrr=ea (+optional 24bit address)
jses    M_DSPABS12 M_AM_NONE    NOPARMO %000011111101aaaaaaaaaaaa  dsp_abs12          +     JScc xxx aaaaaaaaaaaa=12bit address
-       C_DSPABSEA M_AM_NONE    NOPARMO %0000101111mmmrrr10101101  dsp_ea                   JScc ea mmmrrr=ea (+optional 24bit address)
jsge    M_DSPABS12 M_AM_NONE    NOPARMO %000011110001aaaaaaaaaaaa  dsp_abs12          +     JScc xxx aaaaaaaaaaaa=12bit address
-       C_DSPABSEA M_AM_NONE    NOPARMO %0000101111mmmrrr10100001  dsp_ea                   JScc ea mmmrrr=ea (+optional 24bit address)
jsgt    M_DSPABS12 M_AM_NONE    NOPARMO %000011110111aaaaaaaaaaaa  dsp_abs12          +     JScc xxx aaaaaaaaaaaa=12bit address
-       C_DSPABSEA M_AM_NONE    NOPARMO %0000101111mmmrrr10100111  dsp_ea                   JScc ea mmmrrr=ea (+optional 24bit address)
jslc    M_DSPABS12 M_AM_NONE    NOPARMO %000011110110aaaaaaaaaaaa  dsp_abs12          +     JScc xxx aaaaaaaaaaaa=12bit address
-       C_DSPABSEA M_AM_NONE    NOPARMO %0000101111mmmrrr10100110  dsp_ea                   JScc ea mmmrrr=ea (+optional 24bit address)
jsle    M_DSPABS12 M_AM_NONE    NOPARMO %000011111111aaaaaaaaaaaa  dsp_abs12          +     JScc xxx aaaaaaaaaaaa=12bit address
-       C_DSPABSEA M_AM_NONE    NOPARMO %0000101111mmmrrr10101111  dsp_ea                   JScc ea mmmrrr=ea (+optional 24bit address)
jsls    M_DSPABS12 M_AM_NONE    NOPARMO %000011111110aaaaaaaaaaaa  dsp_abs12          +     JScc xxx aaaaaaaaaaaa=12bit address
-       C_DSPABSEA M_AM_NONE    NOPARMO %0000101111mmmrrr10101110  dsp_ea                   JScc ea mmmrrr=ea (+optional 24bit address)
jslt    M_DSPABS12 M_AM_NONE    NOPARMO %000011111001aaaaaaaaaaaa  dsp_abs12          +     JScc xxx aaaaaaaaaaaa=12bit address
-       C_DSPABSEA M_AM_NONE    NOPARMO %0000101111mmmrrr10101001  dsp_ea                   JScc ea mmmrrr=ea (+optional 24bit address)
jsmi    M_DSPABS12 M_AM_NONE    NOPARMO %000011111011aaaaaaaaaaaa  dsp_abs12          +     JScc xxx aaaaaaaaaaaa=12bit address
-       C_DSPABSEA M_AM_NONE    NOPARMO %0000101111mmmrrr10101011  dsp_ea                   JScc ea mmmrrr=ea (+optional 24bit address)
jsne    M_DSPABS12 M_AM_NONE    NOPARMO %000011110010aaaaaaaaaaaa  dsp_abs12          +     JScc xxx aaaaaaaaaaaa=12bit address
-       C_DSPABSEA M_AM_NONE    NOPARMO %0000101111mmmrrr10100010  dsp_ea                   JScc ea mmmrrr=ea (+optional 24bit address)
jsnr    M_DSPABS12 M_AM_NONE    NOPARMO %000011111100aaaaaaaaaaaa  dsp_abs12          +     JScc xxx aaaaaaaaaaaa=12bit address
-       C_DSPABSEA M_AM_NONE    NOPARMO %0000101111mmmrrr10101100  dsp_ea                   JScc ea mmmrrr=ea (+optional 24bit address)
jspl    M_DSPABS12 M_AM_NONE    NOPARMO %000011110011aaaaaaaaaaaa  dsp_abs12          +     JScc xxx aaaaaaaaaaaa=12bit address
-       C_DSPABSEA M_AM_NONE    NOPARMO %0000101111mmmrrr10100011  dsp_ea                   JScc ea mmmrrr=ea (+optional 24bit address)
jsnn    M_DSPABS12 M_AM_NONE    NOPARMO %000011110100aaaaaaaaaaaa  dsp_abs12          +     JScc xxx aaaaaaaaaaaa=12bit address
-       C_DSPABSEA M_AM_NONE    NOPARMO %0000101111mmmrrr10100100  dsp_ea                   JScc ea mmmrrr=ea (+optional 24bit address)
jsr     M_DSPABS12 M_AM_NONE    NOPARMO %000011010000aaaaaaaaaaaa  dsp_abs12          +     JSR  xxx aaaaaaaaaaaa=12bit address
-       C_DSPABSEA M_AM_NONE    NOPARMO %0000101111mmmrrr10000000  dsp_ea                   JSR ea mmmrrr=ea  (+optional 24bit address)
neg     M_ACC56    M_AM_NONE    PARMOVE %mmmmmmmmmmmmmmmm0011d110  dsp_ab                   d=(a=0, b=1)
bchg    C_DSPIM    M_DSPEA      NOPARMO %0000101101mmmrrr0s0bbbbb  dsp_ea_imm5        +     bchg #n,X:ea / #n,Y:ea mmmrrr=ea, s=(X=0, Y=1), bbbbb=0-31
-       C_DSPIM    M_DSPAA      NOPARMO %0000101100aaaaaa0s0bbbbb  dsp_ea_imm5        +     bchg #n,X:aa / bchg #n,Y:aa
-       C_DSPIM    M_DSPPP      NOPARMO %0000101110pppppp0s0bbbbb  dsp_ea_imm5        +     bchg #n,X:pp / bchg #n,Y:pp
-       C_DSPIM    C_DD         NOPARMO %00001011110001dd010bbbbb  dsp_reg_imm5       +     bchg #n,D DDD See A.9 Instruction Encoding and Table A-18 for specific register encodings.
-       C_DSPIM    C_DDD        NOPARMO %0000101111001ddd010bbbbb  dsp_reg_imm5       +     bchg #n,D DDD See A.9 Instruction Encoding and Table A-18 for specific register encodings.
-       C_DSPIM    C_TTT        NOPARMO %0000101111010ddd010bbbbb  dsp_reg_imm5       +     bchg #n,D TTT See A.9 Instruction Encoding and Table A-18 for specific register encodings.
-       C_DSPIM    C_NNN        NOPARMO %0000101111011ddd010bbbbb  dsp_reg_imm5       +     bchg #n,D NNN See A.9 Instruction Encoding and Table A-18 for specific register encodings.
-       C_DSPIM    C_FFF        NOPARMO %0000101111100ddd010bbbbb  dsp_reg_imm5       +     bchg #n,D FFF See A.9 Instruction Encoding and Table A-18 for specific register encodings.
-       C_DSPIM    C_GGG        NOPARMO %0000101111111ddd010bbbbb  dsp_reg_imm5             bchg #n,D GGG See A.9 Instruction Encoding and Table A-18 for specific register encodings.
bclr    C_DSPIM    M_DSPEA      NOPARMO %0000101001mmmrrr0s0bbbbb  dsp_ea_imm5        +     bclr #n,X:ea / #n,Y:ea mmmrrr=ea, s=(X=0, Y=1), bbbbb=0-31
-       C_DSPIM    M_DSPAA      NOPARMO %0000101000aaaaaa0s0bbbbb  dsp_ea_imm5        +     bclr #n,X:aa / bclr #n,Y:aa
-       C_DSPIM    M_DSPPP      NOPARMO %0000101010pppppp0s0bbbbb  dsp_ea_imm5        +     bclr #n,X:pp / bclr #n,Y:pp
-       C_DSPIM    C_DDD        NOPARMO %0000101011001ddd010bbbbb  dsp_reg_imm5       +     bclr #n,D DDD See A.9 Instruction Encoding and Table A-18 for specific register encodings.
-       C_DSPIM    C_DD         NOPARMO %00001010110001dd010bbbbb  dsp_reg_imm5       +     bclr #n,D DDD See A.9 Instruction Encoding and Table A-18 for specific register encodings.
-       C_DSPIM    C_TTT        NOPARMO %0000101011010ddd010bbbbb  dsp_reg_imm5       +     bclr #n,D TTT See A.9 Instruction Encoding and Table A-18 for specific register encodings.
-       C_DSPIM    C_NNN        NOPARMO %0000101011011ddd010bbbbb  dsp_reg_imm5       +     bclr #n,D NNN See A.9 Instruction Encoding and Table A-18 for specific register encodings.
-       C_DSPIM    C_FFF        NOPARMO %0000101011100ddd010bbbbb  dsp_reg_imm5       +     bclr #n,D FFF See A.9 Instruction Encoding and Table A-18 for specific register encodings.
-       C_DSPIM    C_GGG        NOPARMO %0000101011111ddd010bbbbb  dsp_reg_imm5             bclr #n,D GGG See A.9 Instruction Encoding and Table A-18 for specific register encodings.
bset    C_DSPIM    M_DSPEA      NOPARMO %0000101001mmmrrr0s1bbbbb  dsp_ea_imm5        +     bset #n,X:ea / #n,Y:ea mmmrrr=ea, s=(X=0, Y=1), bbbbb=0-31
-       C_DSPIM    M_DSPAA      NOPARMO %0000101000aaaaaa0s1bbbbb  dsp_ea_imm5        +     bset #n,X:aa / bset #n,Y:aa
-       C_DSPIM    M_DSPPP      NOPARMO %0000101010pppppp0s1bbbbb  dsp_ea_imm5        +     bset #n,X:pp / bset #n,Y:pp
-       C_DSPIM    C_DD         NOPARMO %00001010110001dd011bbbbb  dsp_reg_imm5       +     bset #n,D DDD See A.9 Instruction Encoding and Table A-18 for specific register encodings.
-       C_DSPIM    C_DDD        NOPARMO %0000101011001ddd011bbbbb  dsp_reg_imm5       +     bset #n,D DDD See A.9 Instruction Encoding and Table A-18 for specific register encodings.
-       C_DSPIM    C_TTT        NOPARMO %0000101011010ddd011bbbbb  dsp_reg_imm5       +     bset #n,D TTT See A.9 Instruction Encoding and Table A-18 for specific register encodings.
-       C_DSPIM    C_NNN        NOPARMO %0000101011011ddd011bbbbb  dsp_reg_imm5       +     bset #n,D NNN See A.9 Instruction Encoding and Table A-18 for specific register encodings.
-       C_DSPIM    C_FFF        NOPARMO %0000101011100ddd011bbbbb  dsp_reg_imm5       +     bset #n,D FFF See A.9 Instruction Encoding and Table A-18 for specific register encodings.
-       C_DSPIM    C_GGG        NOPARMO %0000101011111ddd011bbbbb  dsp_reg_imm5             bset #n,D GGG See A.9 Instruction Encoding and Table A-18 for specific register encodings.
btst    C_DSPIM    M_DSPEA      NOPARMO %0000101101mmmrrr0s1bbbbb  dsp_ea_imm5        +     btst#n,X:ea / #n,Y:ea mmmrrr=ea, s=(X=0, Y=1), bbbbb=0-31
-       C_DSPIM    M_DSPAA      NOPARMO %0000101100aaaaaa0s1bbbbb  dsp_ea_imm5        +     btst #n,X:aa / btst #n,Y:aa
-       C_DSPIM    M_DSPPP      NOPARMO %0000101110pppppp0s1bbbbb  dsp_ea_imm5        +     btst #n,X:pp / btst #n,Y:pp
-       C_DSPIM    C_DDD        NOPARMO %0000101111001ddd011bbbbb  dsp_reg_imm5       +     btst #n,D DDD See A.9 Instruction Encoding and Table A-18 for specific register encodings.
-       C_DSPIM    C_DD         NOPARMO %00001011110001dd011bbbbb  dsp_reg_imm5       +     btst #n,D DDD See A.9 Instruction Encoding and Table A-18 for specific register encodings.
-       C_DSPIM    C_TTT        NOPARMO %0000101111010ddd011bbbbb  dsp_reg_imm5       +     btst #n,D TTT See A.9 Instruction Encoding and Table A-18 for specific register encodings.
-       C_DSPIM    C_NNN        NOPARMO %0000101111011ddd011bbbbb  dsp_reg_imm5       +     btst #n,D NNN See A.9 Instruction Encoding and Table A-18 for specific register encodings.
-       C_DSPIM    C_FFF        NOPARMO %0000101111100ddd011bbbbb  dsp_reg_imm5       +     btst #n,D FFF See A.9 Instruction Encoding and Table A-18 for specific register encodings.
-       C_DSPIM    C_GGG        NOPARMO %0000101111111ddd011bbbbb  dsp_reg_imm5             btst #n,D GGG See A.9 Instruction Encoding and Table A-18 for specific register encodings.
do      M_DSPEA    C_DSPABS24   NOPARMO %0000011001mmmrrr0s000000  dsp_ea_abs16       +     DO X:ea,expr / DO Y:ea,expr mmmrrr=ea, s=(X=0, Y=1), expr=16bit in extension word
-       M_DSPAA    C_DSPABS24   NOPARMO %0000011000aaaaaa0s000000  dsp_ea_abs16       +     DO X:aa,expr / DO Y:aa,expr aaaaaa=aa, s=(X=0, Y=1), expr=16bit in extension word
-       C_DSPIM    C_DSPABS24   NOPARMO %00000110iiiiiiii1000hhhh  dsp_imm12_abs16    +     DO #xxx,expr hhhhiiiiiiii=12bit immediate, expr=16bit in extension word
-       M_ALU24    C_DSPABS24   NOPARMO %0000011011000ddd00000000  dsp_alu24_abs16    +     DO S,expr x0, x1, y0, y1
-       C_DDD      C_DSPABS24   NOPARMO %0000011011001ddd00000000  dsp_reg_abs16      +     DO S,expr DDD See A.9 Instruction Encoding and Table A-18 for specific register encodings.
-       C_TTT      C_DSPABS24   NOPARMO %0000011011010ddd00000000  dsp_reg_abs16      +     DO S,expr TTT See A.9 Instruction Encoding and Table A-18 for specific register encodings.
-       C_NNN      C_DSPABS24   NOPARMO %0000011011011ddd00000000  dsp_reg_abs16      +     DO S,expr NNN See A.9 Instruction Encoding and Table A-18 for specific register encodings.
-       C_FFF      C_DSPABS24   NOPARMO %0000011011100ddd00000000  dsp_reg_abs16      +     DO S,expr FFF See A.9 Instruction Encoding and Table A-18 for specific register encodings.
-       C_GGG      C_DSPABS24   NOPARMO %0000011011111ddd00000000  dsp_reg_abs16            DO S,expr GGG See A.9 Instruction Encoding and Table A-18 for specific register encodings.
rep     C_DSPIM    M_AM_NONE    NOPARMO %00000110iiiiiiii1010hhhh  dsp_imm12          +     rep #xx
-       M_DSPEA    M_AM_NONE    NOPARMO %0000011001mmmrrr0s100000  dsp_ea             +     rep x:ea / y:ea
-       M_DSPAA    M_AM_NONE    NOPARMO %0000011000aaaaaa0s100000  dsp_ea             +     rep x:aa / y:aa
-       M_ALU24    M_AM_NONE    NOPARMO %0000011011000ddd00100000  dsp_alu24          +     rep S,expr x0, x1, y0, y1
-       C_DDD      M_AM_NONE    NOPARMO %0000011011001ddd00100000  dsp_reg            +     rep S DDD See A.9 Instruction Encoding and Table A-18 for specific register encodings.
-       C_TTT      M_AM_NONE    NOPARMO %0000011011010ddd00100000  dsp_reg            +     rep S TTT See A.9 Instruction Encoding and Table A-18 for specific register encodings.
-       C_NNN      M_AM_NONE    NOPARMO %0000011011011ddd00100000  dsp_reg            +     rep S NNN See A.9 Instruction Encoding and Table A-18 for specific register encodings.
-       C_FFF      M_AM_NONE    NOPARMO %0000011011100ddd00100000  dsp_reg            +     rep S FFF See A.9 Instruction Encoding and Table A-18 for specific register encodings.
-       C_GGG      M_AM_NONE    NOPARMO %0000011011111ddd00100000  dsp_reg                  rep S GGG See A.9 Instruction Encoding and Table A-18 for specific register encodings.
jsclr   C_DSPIM    M_DSPEA      NOPARMO %0000101101mmmrrr1s0bbbbb  dsp_ea_imm5_abs16  +     JSCLR #n,X:ea,xxxx / #n,Y:ea,xxxx n=bbbbb=0-31, ea=mmmrrr, xxxx=16bit extension, s=(X=0, Y=1)
-       C_DSPIM    M_DSPAA      NOPARMO %0000101100aaaaaa1s0bbbbb  dsp_ea_imm5_abs16  +     JSCLR #n,X:aa,xxxx / #n,Y:aa,xxxx n=bbbbb=0-31, aa=aaaaaa=short address, s=(X=0, Y=1)
-       C_DSPIM    M_DSPPP      NOPARMO %0000101110pppppp1s0bbbbb  dsp_ea_imm5_abs16  +     JSCLR #n,X:pp,xxxx / #n,Y:pp,xxxx n=bbbbb=0-31, pp=pppppp=short i/o address, s=(X=0, Y=1)
-       C_DSPIM    C_DD         NOPARMO %00001011110001dd000bbbbb  dsp_reg_imm5_abs16 +     JSCLR  #n,S,xxxx DDD See A.9 Instruction Encoding and Table A-18 for specific register encodings.
-       C_DSPIM    C_DDD        NOPARMO %0000101111001ddd000bbbbb  dsp_reg_imm5_abs16 +     JSCLR  #n,S,xxxx DDD See A.9 Instruction Encoding and Table A-18 for specific register encodings.
-       C_DSPIM    C_TTT        NOPARMO %0000101111010ddd000bbbbb  dsp_reg_imm5_abs16 +     JSCLR  #n,S,xxxx TTT See A.9 Instruction Encoding and Table A-18 for specific register encodings.
-       C_DSPIM    C_NNN        NOPARMO %0000101111011ddd000bbbbb  dsp_reg_imm5_abs16 +     JSCLR  #n,S,xxxx NNN See A.9 Instruction Encoding and Table A-18 for specific register encodings.
-       C_DSPIM    C_FFF        NOPARMO %0000101111100ddd000bbbbb  dsp_reg_imm5_abs16 +     JSCLR  #n,S,xxxx FFF See A.9 Instruction Encoding and Table A-18 for specific register encodings.
-       C_DSPIM    C_GGG        NOPARMO %0000101111101ddd000bbbbb  dsp_reg_imm5_abs16       JSCLR  #n,S,xxxx GGG See A.9 Instruction Encoding and Table A-18 for specific register encodings.
jset    C_DSPIM    M_DSPEA      NOPARMO %0000101001mmmrrr1s1bbbbb  dsp_ea_imm5_abs16  +     JSET  #n,X:ea,xxxx / #n,Y:ea,xxxx n=bbbbb=0-31, ea=mmmrrr, xxxx=16bit extension, s=(X=0, Y=1)
-       C_DSPIM    M_DSPAA      NOPARMO %0000101000aaaaaa1s1bbbbb  dsp_ea_imm5_abs16  +     JSET  #n,X:aa,xxxx / #n,Y:aa,xxxx n=bbbbb=0-31, aa=aaaaaa=short address, s=(X=0, Y=1)
-       C_DSPIM    M_DSPPP      NOPARMO %0000101010pppppp1s1bbbbb  dsp_ea_imm5_abs16  +     JSET  #n,X:pp,xxxx / #n,Y:pp,xxxx n=bbbbb=0-31, pp=pppppp=short i/o address, s=(X=0, Y=1)
-       C_DSPIM    C_DD         NOPARMO %00001010110001dd001bbbbb  dsp_reg_imm5_abs16 +     JSET  #n,S,xxxx DDD See A.9 Instruction Encoding and Table A-18 for specific register encodings.
-       C_DSPIM    C_DDD        NOPARMO %0000101011001ddd001bbbbb  dsp_reg_imm5_abs16 +     JSET  #n,S,xxxx DDD See A.9 Instruction Encoding and Table A-18 for specific register encodings.
-       C_DSPIM    C_TTT        NOPARMO %0000101011010ddd001bbbbb  dsp_reg_imm5_abs16 +     JSET  #n,S,xxxx TTT See A.9 Instruction Encoding and Table A-18 for specific register encodings.
-       C_DSPIM    C_NNN        NOPARMO %0000101011011ddd001bbbbb  dsp_reg_imm5_abs16 +     JSET  #n,S,xxxx NNN See A.9 Instruction Encoding and Table A-18 for specific register encodings.
-       C_DSPIM    C_FFF        NOPARMO %0000101011100ddd001bbbbb  dsp_reg_imm5_abs16 +     JSET  #n,S,xxxx FFF See A.9 Instruction Encoding and Table A-18 for specific register encodings.
-       C_DSPIM    C_GGG        NOPARMO %0000101011101ddd001bbbbb  dsp_reg_imm5_abs16       JSET  #n,S,xxxx GGG See A.9 Instruction Encoding and Table A-18 for specific register encodings.
jsset   C_DSPIM    M_DSPEA      NOPARMO %0000101101mmmrrr1s1bbbbb  dsp_ea_imm5_abs16  +     JSSET #n,X:ea,xxxx / JSSET #n,Y:ea,xxxx n=bbbbb=0-31, ea=mmmrrr, xxxx=16bit extension, s=(X=0, Y=1)
-       C_DSPIM    M_DSPAA      NOPARMO %0000101100aaaaaa1s1bbbbb  dsp_ea_imm5_abs16  +     JSSET #n,X:aa,xxxx / #n,Y:aa,xxxx n=bbbbb=0-31, aa=aaaaaa=short address, s=(X=0, Y=1)
-       C_DSPIM    M_DSPPP      NOPARMO %0000101110pppppp1s1bbbbb  dsp_ea_imm5_abs16  +     JSSET #n,X:pp,xxxx / #n,Y:pp,xxxx n=bbbbb=0-31, pp=pppppp=short i/o address, s=(X=0, Y=1)
-       C_DSPIM    C_DD         NOPARMO %00001011110001dd001bbbbb  dsp_reg_imm5_abs16 +     JSSET #n,S,xxxx DDD See A.9 Instruction Encoding and Table A-18 for specific register encodings.
-       C_DSPIM    C_DDD        NOPARMO %0000101111001ddd001bbbbb  dsp_reg_imm5_abs16 +     JSSET #n,S,xxxx DDD See A.9 Instruction Encoding and Table A-18 for specific register encodings.
-       C_DSPIM    C_TTT        NOPARMO %0000101111010ddd001bbbbb  dsp_reg_imm5_abs16 +     JSSET #n,S,xxxx TTT See A.9 Instruction Encoding and Table A-18 for specific register encodings.
-       C_DSPIM    C_NNN        NOPARMO %0000101111011ddd001bbbbb  dsp_reg_imm5_abs16 +     JSSET #n,S,xxxx NNN See A.9 Instruction Encoding and Table A-18 for specific register encodings.
-       C_DSPIM    C_FFF        NOPARMO %0000101111100ddd001bbbbb  dsp_reg_imm5_abs16 +     JSSET #n,S,xxxx FFF See A.9 Instruction Encoding and Table A-18 for specific register encodings.
-       C_DSPIM    C_GGG        NOPARMO %0000101111101ddd001bbbbb  dsp_reg_imm5_abs16       JSSET #n,S,xxxx GGG See A.9 Instruction Encoding and Table A-18 for specific register encodings.
jclr    C_DSPIM    M_DSPEA      NOPARMO %0000101001mmmrrr1s0bbbbb  dsp_ea_imm5_abs16  +     JCLR  #n,X:ea,xxxx / #n,Y:ea,xxxx n=bbbbb=0-31, ea=mmmrrr, xxxx=16bit extension, s=(X=0, Y=1)
-       C_DSPIM    M_DSPAA      NOPARMO %0000101000aaaaaa1s0bbbbb  dsp_ea_imm5_abs16  +     JCLR  #n,X:aa,xxxx / #n,Y:aa,xxxx n=bbbbb=0-31, aa=aaaaaa=short address, s=(X=0, Y=1)
-       C_DSPIM    M_DSPPP      NOPARMO %0000101010pppppp1s0bbbbb  dsp_ea_imm5_abs16  +     JCLR  #n,X:pp,xxxx / #n,Y:pp,xxxx n=bbbbb=0-31, pp=pppppp=short i/o address, s=(X=0, Y=1)
-       C_DSPIM    C_DD         NOPARMO %00001010110001dd000bbbbb  dsp_reg_imm5_abs16 +     JCLR  #n,S,xxxx DDD See A.9 Instruction Encoding and Table A-18 for specific register encodings.
-       C_DSPIM    C_DDD        NOPARMO %0000101011001ddd000bbbbb  dsp_reg_imm5_abs16 +     JCLR  #n,S,xxxx DDD See A.9 Instruction Encoding and Table A-18 for specific register encodings.
-       C_DSPIM    C_TTT        NOPARMO %0000101011010ddd000bbbbb  dsp_reg_imm5_abs16 +     JCLR  #n,S,xxxx TTT See A.9 Instruction Encoding and Table A-18 for specific register encodings.
-       C_DSPIM    C_NNN        NOPARMO %0000101011011ddd000bbbbb  dsp_reg_imm5_abs16 +     JCLR  #n,S,xxxx NNN See A.9 Instruction Encoding and Table A-18 for specific register encodings.
-       C_DSPIM    C_FFF        NOPARMO %0000101011100ddd000bbbbb  dsp_reg_imm5_abs16 +     JCLR  #n,S,xxxx FFF See A.9 Instruction Encoding and Table A-18 for specific register encodings.
-       C_DSPIM    C_GGG        NOPARMO %0000101011101ddd000bbbbb  dsp_reg_imm5_abs16       JCLR  #n,S,xxxx GGG See A.9 Instruction Encoding and Table A-18 for specific register encodings.
lua     M_DSPEA    C_LUADST     NOPARMO %00000100010mmrrr0001dddd  dsp_ea_lua               mmrrr=ea (subset), dddd=(bit 3=(0=Rn, 1=Nn), bits 2-0=0-7)
norm    M_DSPR     M_ACC56      NOPARMO %0000000111011rrr0001d101  dsp_ab_rn                norm Rn,D D=(a=0, b=1)
move    M_AM_NONE  M_AM_NONE    PARMOVE %mmmmmmmmmmmmmmmm00000000  dsp_self
movec   M_DSPIM8   C_MOVEC      NOPARMO %00000101iiiiiiii101ddddd  dsp_immmovec       +     move(c) #xx,d1
-       M_DSPEA    C_MOVEC      NOPARMO %0000010111mmmrrr0s1ddddd  dsp_movec_ea       +     move(c) x:ea,d1 / y:ea,d1
-       C_MOVEC    M_DSPEA      NOPARMO %0000010101mmmrrr0s1ddddd  dsp_movec_ea       +     move(c) s1,x:ea / s1,y:ea
-       C_DSPIM    C_MOVEC      NOPARMO %0000010111110100001ddddd  dsp_movec_ea       +     move(c) #xxxx,d1
-       M_DSPAA    C_MOVEC      NOPARMO %0000010110aaaaaa0s1ddddd  dsp_movec_aa       +     move(c) x:aa,d1 / y:aa,d1
-       C_MOVEC    M_DSPAA      NOPARMO %0000010100aaaaaa0s1ddddd  dsp_movec_aa       +     move(c) s1,x:aa / s1,y:aa
-       C_MOVEC    M_ALU24      NOPARMO %0000010001000eee101ddddd  dsp_movec_reg      +     move(c) s1,d2
-       C_MOVEC    C_DDD        NOPARMO %0000010001001eee101ddddd  dsp_movec_reg      +     move(c) s1,d2
-       C_MOVEC    C_TTT        NOPARMO %0000010001010eee101ddddd  dsp_movec_reg      +     move(c) s1,d2
-       C_MOVEC    C_NNN        NOPARMO %0000010001011eee101ddddd  dsp_movec_reg      +     move(c) s1,d2
-       C_MOVEC    C_FFF        NOPARMO %0000010001100eee101ddddd  dsp_movec_reg      +     move(c) s1,d2
-       C_MOVEC    C_GGG        NOPARMO %0000010001111eee101ddddd  dsp_movec_reg      +     move(c) s1,d2
-       M_ALU24    C_MOVEC      NOPARMO %0000010011000eee101ddddd  dsp_movec_reg      +     move(c) s2,d1
-       C_DDD      C_MOVEC      NOPARMO %0000010011001eee101ddddd  dsp_movec_reg      +     move(c) s2,d1
-       C_TTT      C_MOVEC      NOPARMO %0000010011010eee101ddddd  dsp_movec_reg      +     move(c) s2,d1
-       C_NNN      C_MOVEC      NOPARMO %0000010011011eee101ddddd  dsp_movec_reg      +     move(c) s2,d1
-       C_FFF      C_MOVEC      NOPARMO %0000010011100eee101ddddd  dsp_movec_reg      +     move(c) s2,d1
-       C_GGG      C_MOVEC      NOPARMO %0000010011111eee101ddddd  dsp_movec_reg            move(c) s2,d1
movem   M_ALU24    M_DSPEA      NOPARMO %0000011101mmmrrr10000ddd  dsp_movem_ea       +     move(m) s,p:ea / p:ea,d
-       C_DDD      M_DSPEA      NOPARMO %0000011101mmmrrr10001ddd  dsp_movem_ea       +     move(m) s,p:ea / p:ea,d
-       C_TTT      M_DSPEA      NOPARMO %0000011101mmmrrr10010ddd  dsp_movem_ea       +     move(m) s,p:ea / p:ea,d
-       C_NNN      M_DSPEA      NOPARMO %0000011101mmmrrr10011ddd  dsp_movem_ea       +     move(m) s,p:ea / p:ea,d
-       C_FFF      M_DSPEA      NOPARMO %0000011101mmmrrr10100ddd  dsp_movem_ea       +     move(m) s,p:ea / p:ea,d
-       C_GGG      M_DSPEA      NOPARMO %0000011101mmmrrr10111ddd  dsp_movem_ea       +     move(m) s,p:ea / p:ea,d
-       M_DSPEA    M_ALU24      NOPARMO %0000011111mmmrrr10000ddd  dsp_movem_ea       +     move(m) s,p:ea / p:ea,d
-       M_DSPEA    C_DDD        NOPARMO %0000011111mmmrrr10001ddd  dsp_movem_ea       +     move(m) s,p:ea / p:ea,d
-       M_DSPEA    C_TTT        NOPARMO %0000011111mmmrrr10010ddd  dsp_movem_ea       +     move(m) s,p:ea / p:ea,d
-       M_DSPEA    C_NNN        NOPARMO %0000011111mmmrrr10011ddd  dsp_movem_ea       +     move(m) s,p:ea / p:ea,d
-       M_DSPEA    C_FFF        NOPARMO %0000011111mmmrrr10100ddd  dsp_movem_ea       +     move(m) s,p:ea / p:ea,d
-       M_DSPEA    C_GGG        NOPARMO %0000011111mmmrrr10111ddd  dsp_movem_ea       +     move(m) s,p:ea / p:ea,d
-       M_ALU24    M_DSPAA      NOPARMO %0000011100aaaaaa00000ddd  dsp_movem_aa       +     move(m) s,p:aa / p:aa,d
-       C_DDD      M_DSPAA      NOPARMO %0000011100aaaaaa00001ddd  dsp_movem_aa       +     move(m) s,p:aa / p:aa,d
-       C_TTT      M_DSPAA      NOPARMO %0000011100aaaaaa00010ddd  dsp_movem_aa       +     move(m) s,p:aa / p:aa,d
-       C_NNN      M_DSPAA      NOPARMO %0000011100aaaaaa00011ddd  dsp_movem_aa       +     move(m) s,p:aa / p:aa,d
-       C_FFF      M_DSPAA      NOPARMO %0000011100aaaaaa00100ddd  dsp_movem_aa       +     move(m) s,p:aa / p:aa,d
-       C_GGG      M_DSPAA      NOPARMO %0000011100aaaaaa00111ddd  dsp_movem_aa       +     move(m) s,p:aa / p:aa,d
-       M_DSPAA    M_ALU24      NOPARMO %0000011110aaaaaa00000ddd  dsp_movem_aa       +     move(m) s,p:aa / p:aa,d
-       M_DSPAA    C_DDD        NOPARMO %0000011110aaaaaa00001ddd  dsp_movem_aa       +     move(m) s,p:aa / p:aa,d
-       M_DSPAA    C_TTT        NOPARMO %0000011110aaaaaa00010ddd  dsp_movem_aa       +     move(m) s,p:aa / p:aa,d
-       M_DSPAA    C_NNN        NOPARMO %0000011110aaaaaa00011ddd  dsp_movem_aa       +     move(m) s,p:aa / p:aa,d
-       M_DSPAA    C_FFF        NOPARMO %0000011110aaaaaa00100ddd  dsp_movem_aa       +     move(m) s,p:aa / p:aa,d
-       M_DSPAA    C_GGG        NOPARMO %0000011110aaaaaa00111ddd  dsp_movem_aa             move(m) s,p:aa / p:aa,d
mac     M_ALU24    M_ALU24      PARMOVE %mmmmmmmmmmmmmmmm1qqqdk10  dsp_mult                 mac -+s1,s2,d / mac -+s2,s1,d
macr    M_ALU24    M_ALU24      PARMOVE %mmmmmmmmmmmmmmmm1qqqdk11  dsp_mult                 macr -+s1,s2,d / macr -+s2,s1,d
mpy     M_ALU24    M_ALU24      PARMOVE %mmmmmmmmmmmmmmmm1qqqdk00  dsp_mult                 mpy -+s1,d2,d / -+s2,s1,d
mpyr    M_ALU24    M_ALU24      PARMOVE %mmmmmmmmmmmmmmmm1qqqdk01  dsp_mult                 mpyr -+s1,d2,d / -+s2,s1,d

; MY NOTES
movep   M_DSPEA    M_DSPPP     NOPARMO %0000100sw1mmmrrr1spppppp  dsp_movep_ea    	   MOVEP XY:ea,XY:pp
movep   M_DSPEA    M_DSPPP     NOPARMO %0000100sw1mmmrrr01pppppp  dsp_movep_ea         MOVEP P:ea,XY:pp
movep   M_DSPEA    M_DSPPP     NOPARMO %0000100sw1dddddd00pppppp  dsp_movep_ea    	   MOVEP REG,XY:pp

;movep   M_DSPEA    M_DSPPP      NOPARMO %0000100s11mmmrrr0spppppp  dsp_movep_ea       +     movep p:ea,x:pp / p:ea,y:pp 
;-       M_DSPAA    M_DSPPP      NOPARMO %0000100s11mmmrrr1spppppp  dsp_movep_ea       +     movep p:aa,x:pp / p:aa,y:pp 
;-       M_DSPPP    M_DSPEA      NOPARMO %0000100s01mmmrrr1spppppp  dsp_movep_ea       +     movep x:pp,p:ea / y:pp,p:ea
;;-       M_DSPPP    M_DSPPP      NOPARMO %0000100s01mmmrrr1spppppp  dsp_movep_ea       +     x:pp,p:ea / y:pp,p:ea
;;-       M_DSPPP    M_DSPAA      NOPARMO %0000100s01mmmrrr1spppppp  dsp_movep_ea       +     x:pp,p:aa / y:pp,p:aa
;
;-       C_DSPIM    M_DSPPP      NOPARMO %0000100s111101001spppppp  dsp_movep_ea       +     #xxxxxx,x:pp / #xxxxxx,y:pp
;-       C_DSPIM    M_DSPEA      NOPARMO %0000100s111101001spppppp  dsp_movep_ea       +     #xxxxxx,x:pp / #xxxxxx,y:pp
;
;-       M_ALU24    M_DSPPP      NOPARMO %0000100s11000ddd0spppppp  dsp_movep_reg      +     movep s,x:pp / s,y:pp
;-       C_DDD      M_DSPPP      NOPARMO %0000100s11001ddd0spppppp  dsp_movep_reg      +     movep s,x:pp / s,y:pp
;-       C_TTT      M_DSPPP      NOPARMO %0000100s11010ddd0spppppp  dsp_movep_reg      +     movep s,x:pp / s,y:pp
;-       C_NNN      M_DSPPP      NOPARMO %0000100s11011ddd0spppppp  dsp_movep_reg      +     movep s,x:pp / s,y:pp
;-       C_FFF      M_DSPPP      NOPARMO %0000100s11100ddd0spppppp  dsp_movep_reg      +     movep s,x:pp / s,y:pp
;-       C_GGG      M_DSPPP      NOPARMO %0000100s11111ddd0spppppp  dsp_movep_reg      +     movep s,x:pp / s,y:pp
;-       M_DSPPP    M_ALU24      NOPARMO %0000100s01000ddd0spppppp  dsp_movep_reg      +     movep x:pp,d / y:pp,d
;-       M_DSPPP    C_DDD        NOPARMO %0000100s01001ddd0spppppp  dsp_movep_reg      +     movep x:pp,d / y:pp,d
;-       M_DSPPP    C_TTT        NOPARMO %0000100s01010ddd0spppppp  dsp_movep_reg      +     movep x:pp,d / y:pp,d
;-       M_DSPPP    C_NNN        NOPARMO %0000100s01011ddd0spppppp  dsp_movep_reg      +     movep x:pp,d / y:pp,d
;-       M_DSPPP    C_FFF        NOPARMO %0000100s01100ddd0spppppp  dsp_movep_reg      +     movep x:pp,d / y:pp,d
;-       M_DSPPP    C_GGG        NOPARMO %0000100s01111ddd0spppppp  dsp_movep_reg            movep x:pp,d / y:pp,d

debug	M_AM_NONE  M_AM_NONE    NOPARMO %000000000000001000000000  dsp_self
debugcc M_AM_NONE  M_AM_NONE    NOPARMO %000000000000001100000000  dsp_self
;debughs	M_AM_NONE  M_AM_NONE    NOPARMO %000000000000001100000000  dsp_self
debugcs	M_AM_NONE  M_AM_NONE    NOPARMO %000000000000001100001000  dsp_self
;debuglo	M_AM_NONE  M_AM_NONE    NOPARMO %000000000000001100001000  dsp_self
debugec	M_AM_NONE  M_AM_NONE    NOPARMO %000000000000001100000101  dsp_self
debugeq	M_AM_NONE  M_AM_NONE    NOPARMO %000000000000001100001010  dsp_self
debuges	M_AM_NONE  M_AM_NONE    NOPARMO %000000000000001100001101  dsp_self
debugge	M_AM_NONE  M_AM_NONE    NOPARMO %000000000000001100000001  dsp_self
debuggt	M_AM_NONE  M_AM_NONE    NOPARMO %000000000000001100000111  dsp_self
debuglc	M_AM_NONE  M_AM_NONE    NOPARMO %000000000000001100000110  dsp_self
debugle	M_AM_NONE  M_AM_NONE    NOPARMO %000000000000001100001111  dsp_self
debugls	M_AM_NONE  M_AM_NONE    NOPARMO %000000000000001100001110  dsp_self
debuglt	M_AM_NONE  M_AM_NONE    NOPARMO %000000000000001100001001  dsp_self
debugmi	M_AM_NONE  M_AM_NONE    NOPARMO %000000000000001100001011  dsp_self
debugne	M_AM_NONE  M_AM_NONE    NOPARMO %000000000000001100000010  dsp_self
debugnr	M_AM_NONE  M_AM_NONE    NOPARMO %000000000000001100001100  dsp_self
debugpl	M_AM_NONE  M_AM_NONE    NOPARMO %000000000000001100000011  dsp_self
debugnn	M_AM_NONE  M_AM_NONE    NOPARMO %000000000000001100000100  dsp_self
; This are extras to support the special parallel move type which has 0000 in the top bits
move	M_AM_NONE  M_AM_NONE    NOPARMO %0000100P10PPPPPPPPPPPPPP  dsp_pm        Parallel move Class II R:Y Y0,A    A,Y:ea
move	M_AM_NONE  M_AM_NONE    NOPARMO %0000100P00PPPPPPPPPPPPPP  dsp_pm        Parallel move Class II X:R A,X:ea  X0,A


