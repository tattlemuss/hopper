; For rmac use
; rmac -DRMAC=1 -DMOT=0 -fl -o dsp_rmac.lod dsp.s
; For Motorola's assembler use:
; asm56000.exe -A -p 56001c -Bdsp_mot.cld -ldsp_mot.lst -D MOT 1 -D RMAC 0 dsp.s
; cldlod dsp_mot.cld > dsp_mot.lod

	.org p:0
    if MOT
    page 255,255,0,0,0
    endif
    
    if RMAC
    .56001
    endif

; TODO: add tests containing parallel moves + extension word

; TODO: add tests for forced operator, like below but more comprehensive
	add		x1,b	a,x:>$0	a,y0
	add		x1,b	a,x:>$0	a,y1
	add		x1,b	a,x:>$0	b,y0
	add		x1,b	a,x:>$0	b,y1
	add		x1,b	b,x:>$0	a,y0
	add		x1,b	b,x:>$0	a,y1
	add		x1,b	b,x:>$0	b,y0
	add		x1,b	b,x:>$0	b,y1


	add		x1,b	a,x0	a,y:>$0
	add		x1,b	a,x1	a,y:>$0
	add		x1,b	b,x0	a,y:>$0
	add		x1,b	b,x1	a,y:>$0
	add		x1,b	a,x0	b,y:>$0
	add		x1,b	a,x1	b,y:>$0
	add		x1,b	b,x0	b,y:>$0
	add		x1,b	b,x1	b,y:>$0

	add		x1,b	x:>$0,a	a,y0
	add		x1,b	x:>$0,a	a,y1
	add		x1,b	x:>$0,a	b,y0
	add		x1,b	x:>$0,a	b,y1

	add		x1,a	x:>$0,b	a,y0
	add		x1,a	x:>$0,b	a,y1
	add		x1,a	x:>$0,b	b,y0
	add		x1,a	x:>$0,b	b,y1
	
	add		x1,b	a,x0	y:>$0,a
	add		x1,b	a,x1	y:>$0,a
	add		x1,b	b,x0	y:>$0,a
	add		x1,b	b,x1	y:>$0,a

	add		x1,a	a,x0	y:>$0,b
	add		x1,a	a,x1	y:>$0,b
	add		x1,a	b,x0	y:>$0,b
	add		x1,a	b,x1	y:>$0,b




label:

    jmp label

;ok so far
    abs a                       ;200026
    abs b                       ;20002e
    abs a x:(r0)-n0,x0          ;44c026
    abs b x:(r0)-n0,x0 a,y0     ;10802e
    asl a                       ;200032
    asl b                       ;20003a
    asl a y:(r1)-n1,y0          ;4ec132
    asr a                       ;200022
    asr b                       ;20002a
    asr a y:(r1)-n1,y0          ;4ec122
    clr a                       ;200013
    clr b                       ;20001b
    clr a y:(r1)-n1,y0          ;4ec113
    not a                       ;200017
    not b                       ;20001f
    not a y:(r1)-n1,y0          ;4ec117
    lsl a                       ;200033
    lsl b                       ;20003b
    lsl a y:(r1)-n1,y0          ;4ec133
    lsr a                       ;200023
    lsr b                       ;20002b
    lsr a y:(r1)-n1,y0          ;4ec123

    addr a,b                    ;20000a
    addr b,a                    ;200002
    addr a,b (r5)+              ;205d0a
    addl a,b                    ;20001a
    addl b,a                    ;200012
    addl a,b (r5)+              ;205d1a

    add b,a                     ;200010
    add a,b                     ;200018
    add x,a                     ;200020
    add x,b                     ;200028
    add y,a                     ;200030
    add y,b                     ;200038
    add x0,a                    ;200040
    add x0,b                    ;200048
    add y0,a                    ;200050
    add y0,b                    ;200058
    add x1,a                    ;200060
    add x1,b                    ;200068
    add y1,a                    ;200070
    add y1,b                    ;200078
    add y0,a x:(r5)+n5,x0 y:(r2)+n2,b   ;d3cd50

    cmp b,a                     ;200015
    cmp a,b                     ;20001d
;   cmp x,a                     ;should produce an error (x and y are not allowed)
;   cmp x,b                     ;should produce an error (x and y are not allowed)
;   cmp y,a                     ;should produce an error (x and y are not allowed)
;   cmp y,b                     ;should produce an error (x and y are not allowed)
    cmp x0,a                    ;200045
    cmp x0,b                    ;20004d
    cmp y0,a                    ;200055
    cmp y0,b                    ;20005d
    cmp x1,a                    ;200065
    cmp x1,b                    ;20006d
    cmp y1,a                    ;200075
    cmp y1,b                    ;20007d
    cmp y0,a x:(r5)+n5,x0 y:(r2)+n2,b   ;d3cd55

    cmpm b,a                     ;200017
    cmpm a,b                     ;20001f
;   cmpm x,a                     ;should produce an error (x and y are not allowed)
;   cmpm x,b                     ;should produce an error (x and y are not allowed)
;   cmpm y,a                     ;should produce an error (x and y are not allowed)
;   cmpm y,b                     ;should produce an error (x and y are not allowed)
    cmpm x0,a                    ;200047
    cmpm x0,b                    ;20004f
    cmpm y0,a                    ;200057
    cmpm y0,b                    ;20005f
    cmpm x1,a                    ;200067
    cmpm x1,b                    ;20006f
    cmpm y1,a                    ;200077
    cmpm y1,b                    ;20007f
    cmpm y0,a x:(r5)+n5,x0 y:(r2)+n2,b   ;d3cd57

    sub b,a                     ;200014
    sub a,b                     ;20001c
    sub x,a                     ;200024
    sub x,b                     ;20002c
    sub y,a                     ;200034
    sub y,b                     ;20003c
    sub x0,a                    ;200044
    sub x0,b                    ;20004c
    sub y0,a                    ;200054
    sub y0,b                    ;20005c
    sub x1,a                    ;20006c
    sub x1,b                    ;200064
    sub y1,a                    ;20007c
    sub y1,b                    ;200074
    sub y0,a x:(r5)+n5,x0 y:(r2)+n2,b   ;d3cd54

    tfr b,a                     ;200001
    tfr a,b                     ;200009
;    tfr x,a                    ;should produce an error (x and y are not allowed)
;    tfr x,b                    ;should produce an error (x and y are not allowed)
;    tfr y,a                    ;should produce an error (x and y are not allowed)
;    tfr y,b                    ;should produce an error (x and y are not allowed)
    tfr x0,a                    ;200041
    tfr x0,b                    ;200049
    tfr y0,a                    ;200051
    tfr y0,b                    ;200059
    tfr x1,a                    ;200061
    tfr x1,b                    ;200069
    tfr y1,a                    ;200071
    tfr y1,b                    ;200079
    tfr y0,a x:(r5)+n5,x0 y:(r2)+n2,b   ;d3cd51

    subl a,b                    ;20001e
    subl b,a                    ;200016
    subl a,b (r5)+              ;205d1e
    subr a,b                    ;20000e
    subr b,a                    ;200006
    subr a,b (r5)+              ;205d0e

    tst a                       ;20000b
    tst b                       ;200003
    tst a x:(r0)-n0,x0          ;44c00b
    tst b x:(r0)-n0,x0 a,y0     ;108003

   
     
    illegal                     ;000085
    nop                         ;000000
    reset                       ;000084
    rti                         ;000004
    rts                         ;00000c
    stop                        ;000087
    swi                         ;000006
    wait                        ;000086

    and x0,a                    ;200046
    and x0,b                    ;20004e
    and x1,a                    ;200066
    and x1,b                    ;20006e
    and y0,a                    ;200056
    and y0,b                    ;20005e
    and y1,a                    ;200076
    and y1,b                    ;20007e
; expression result too large?    and x1,b x:123456,a a,y0    ;57406e

    or  x0,a                    ;200042
    or  x0,b                    ;20004a
    or  x1,a                    ;200062
    or  x1,b                    ;20006a
    or  y0,a                    ;200052
    or  y0,b                    ;20005a
    or  y1,a                    ;200072
    or  y1,b                    ;20007a
; expression result too large?    or  x1,b x:123456,a a,y0    ;57406a

    eor x0,a                    ;200043
    eor x0,b                    ;20004b
    eor x1,a                    ;200063
    eor x1,b                    ;20006b
    eor y0,a                    ;200053
    eor y0,b                    ;20005b
    eor y1,a                    ;200073
    eor y1,b                    ;20007b
; expression result too large?    eor x1,b x:123456,a a,y0    ;57406b

    div x0,a                    ;018040
    div x0,b                    ;018048
    div x1,a                    ;018060
    div x1,b                    ;018068
    div y0,a                    ;018050
    div y0,b                    ;018058
    div y1,a                    ;018070
    div y1,b                    ;018078
    ;div x1,b x:123456,a a,y0       ;should produce an error (no parallel moves allowed)

    andi #12,mr                 ;000cb8
    andi #56,ccr                ;0038b9
    andi #90,omr                ;005aba
    ori #12,mr                  ;000cf8
    ori #56,ccr                 ;0038f9
    ori #90,omr                 ;005afa

    movec #12,m0                ;050ca0
    movec #23,m1                ;0517a1
    movec #34,m2                ;0522a2
    movec #45,m3                ;052da3
    movec #56,m4                ;0538a4
    movec #67,m5                ;0543a5
    movec #78,m6                ;054ea6
    movec #89,m7                ;0559a7
    movec #90,sr                ;055ab9
    movec #01,omr               ;0501ba
    movec #12,sp                ;050cbb
    movec #23,ssh               ;0517bc
    movec #34,ssl               ;0522bd
    movec #45,la                ;052dbe
    movec #56,lc                ;0538bf

    rep #4000                   ;06a0af

    tcc x0,a                    ;020040
    tcc x0,b                    ;020048
    tcc x1,a                    ;020060
    tcc x1,b                    ;020068
    tcc y0,a                    ;020050
    tcc y0,b                    ;020058
    tcc y1,a                    ;020070
    tcc y1,b                    ;020078

    tcc b,a                     ;020000
    tcc x1,b r3,r7              ;03036f
    ths b,a                     ;020000
    ths x1,b r3,r7              ;03036f
    tge b,a                     ;021000
    tge x1,b r3,r7              ;03136f
    tne b,a                     ;022000
    tne x1,b r3,r7              ;03236f
    tpl b,a                     ;023000
    tpl x1,b r3,r7              ;03336f
    tnn b,a                     ;024000
    tnn x1,b r3,r7              ;03436f
    tec b,a                     ;025000
    tec x1,b r3,r7              ;03536f
    tlc b,a                     ;026000
    tlc x1,b r3,r7              ;03636f
    tgt b,a                     ;027000
    tgt x1,b r3,r7              ;03736f
    tcs b,a                     ;028000
    tcs x1,b r3,r7              ;03836f
    tlo b,a                     ;028000
    tlo x1,b r3,r7              ;03836f
    tlt b,a                     ;029000
    tlt x1,b r3,r7              ;03936f
    teq b,a                     ;02a000
    teq x1,b r3,r7              ;03a36f
    tmi b,a                     ;02b000
    tmi x1,b r3,r7              ;03b36f
    tnr b,a                     ;02c000
    tnr x1,b r3,r7              ;03c36f
    tes b,a                     ;02d000
    tes x1,b r3,r7              ;03d36f
    tls b,a                     ;02e000
    tls x1,b r3,r7              ;03e36f
    tle b,a                     ;02f000
    tle x1,b r3,r7              ;03f36f

    jcc  (r1)-n1                ;0ac1a0
    jcc  (r2)+n2                ;0acaa0
    jcc  (r3)-                  ;0ad3a0
    jcc  (r4)+                  ;0adca0
    jcc  (r5)                   ;0ae5a0
    jcc  (r6+n6)                ;0aeea0
    jcc  -(r7)                  ;0affa0
    ;jcc  label                  ;0af a0 Not implemented yet

    jcc  (r1)-n1                ;0ac1a0
    jhs  (r1)-n1                ;0ac1a0
    jge  (r1)-n1                ;0ac1a1
    jne  (r1)-n1                ;0ac1a2
    jpl  (r1)-n1                ;0ac1a3
    jnn  (r1)-n1                ;0ac1a4
    jec  (r1)-n1                ;0ac1a5
    jlc  (r1)-n1                ;0ac1a6
    jgt  (r1)-n1                ;0ac1a7
    jcs  (r1)-n1                ;0ac1a8
    jlo  (r1)-n1                ;0ac1a8
    jlt  (r1)-n1                ;0ac1a9
    jeq  (r1)-n1                ;0ac1aa
    jmi  (r1)-n1                ;0ac1ab
    jnr  (r1)-n1                ;0ac1ac
    jes  (r1)-n1                ;0ac1ad
    jls  (r1)-n1                ;0ac1ae
    jle  (r1)-n1                ;0ac1af

    jmp  (r1)-n1                ;0ac180
    jmp  (r2)+n2                ;0aca80
    jmp  (r3)-                  ;0ad380
    jmp  (r4)+                  ;0adc80
    jmp  (r5)                   ;0ae580
    jmp  (r6+n6)                ;0aee80
    jmp  -(r7)                  ;0aff80

    bchg #$9,x:(r1)-n1           ;0b4109
    bchg #$9,x:(r2)+n2           ;0b4a09
    bchg #$9,x:(r3)-             ;0b5309
    bchg #$9,x:(r4)+             ;0b5c09
    bchg #$9,x:(r5)              ;0b6509
    bchg #$9,x:(r6+n6)           ;0b6e09
    bchg #$9,x:-(r7)             ;0b7f09
    bchg #$9,y:(r1)-n1           ;0b4149
    bchg #$9,y:(r2)+n2           ;0b4a49
    bchg #$9,y:(r3)-             ;0b5349
    bchg #$9,y:(r4)+             ;0b5c49
    bchg #$9,y:(r5)              ;0b6549
    bchg #$9,y:(r6+n6)           ;0b6e49
    bchg #$9,y:-(r7)             ;0b7f49

    bclr #$9,x:(r1)-n1           ;0a4109
    bclr #$9,x:(r2)+n2           ;0a4a09
    bclr #$9,x:(r3)-             ;0a5309
    bclr #$9,x:(r4)+             ;0a5c09
    bclr #$9,x:(r5)              ;0a6509
    bclr #$9,x:(r6+n6)           ;0a6e09
    bclr #$9,x:-(r7)             ;0a7f09
    bclr #$9,y:(r1)-n1           ;0a4149
    bclr #$9,y:(r2)+n2           ;0a4a49
    bclr #$9,y:(r3)-             ;0a5349
    bclr #$9,y:(r4)+             ;0a5c49
    bclr #$9,y:(r5)              ;0a6549
    bclr #$9,y:(r6+n6)           ;0a6e49
    bclr #$9,y:-(r7)             ;0a7f49

    bset #$9,x:(r1)-n1           ;0a4129
    bset #$9,x:(r2)+n2           ;0a4a29
    bset #$9,x:(r3)-             ;0a5329
    bset #$9,x:(r4)+             ;0a5c29
    bset #$9,x:(r5)              ;0a6529
    bset #$9,x:(r6+n6)           ;0a6e29
    bset #$9,x:-(r7)             ;0a7f29
    bset #$9,y:(r1)-n1           ;0a4169
    bset #$9,y:(r2)+n2           ;0a4a69
    bset #$9,y:(r3)-             ;0a5369
    bset #$9,y:(r4)+             ;0a5c69
    bset #$9,y:(r5)              ;0a6569
    bset #$9,y:(r6+n6)           ;0a6e69
    bset #$9,y:-(r7)             ;0a7f69

    btst #$9,x:(r1)-n1           ;0b4129
    btst #$9,x:(r2)+n2           ;0b4a29
    btst #$9,x:(r3)-             ;0b5329
    btst #$9,x:(r4)+             ;0b5c29
    btst #$9,x:(r5)              ;0b6529
    btst #$9,x:(r6+n6)           ;0b6e29
    btst #$9,x:-(r7)             ;0b7f29
    btst #$9,y:(r1)-n1           ;0b4169
    btst #$9,y:(r2)+n2           ;0b4a69
    btst #$9,y:(r3)-             ;0b5369
    btst #$9,y:(r4)+             ;0b5c69
    btst #$9,y:(r5)              ;0b6569
    btst #$9,y:(r6+n6)           ;0b6e69
    btst #$9,y:-(r7)             ;0b7f69

    jscc (r1)-n1                ;0bc1a0
    jshs (r1)-n1                ;0bc1a0
    jsge (r1)-n1                ;0bc1a1
    jsne (r1)-n1                ;0bc1a2
    jspl (r1)-n1                ;0bc1a3
    jsnn (r1)-n1                ;0bc1a4
    jsec (r1)-n1                ;0bc1a5
    jslc (r1)-n1                ;0bc1a6
    jsgt (r1)-n1                ;0bc1a7
    jscs (r1)-n1                ;0bc1a8
    jslo (r1)-n1                ;0bc1a8
    jslt (r1)-n1                ;0bc1a9
    jseq (r1)-n1                ;0bc1aa
    jsmi (r1)-n1                ;0bc1ab
    jsnr (r1)-n1                ;0bc1ac
    jses (r1)-n1                ;0bc1ad
    jsls (r1)-n1                ;0bc1ae
    jsle (r1)-n1                ;0bc1af

    jsr  (r1)-n1                ;2bc180
    jsr  (r2)+n2                ;2bca80
    jsr  (r3)-                  ;2bd380
    jsr  (r4)+                  ;2bdc80
    jsr  (r5)                   ;2be580
    jsr  (r6+n6)                ;2bee80
    jsr  -(r7)                  ;2bff80

    neg a                       ;200036
    neg b                       ;20003e
    neg a x:(r0),x0 y:(r4),y0   ;c08036

backequ equ 10

    jclr #backequ,x:(r1)-n1,$1234           ;0a418a
    jclr #forwardequ,x:(r1)-n1,$1234        ;0a418a

forwardequ equ 20

    jmp  $123                   ;0c0123
    jscc $123                   ;0f0123
    jshs $123                   ;0f0123
    jsge $123                   ;0f1123
    jsne $123                   ;0f2123
    jspl $123                   ;0f3123
    jsnn $123                   ;0f4123
    jsec $123                   ;0f5123
    jslc $123                   ;0f6123
    jsgt $123                   ;0f7123
    jscs $123                   ;0f8123
    jslo $123                   ;0f8123
    jslt $123                   ;0f9123
    jseq $123                   ;0fa123
    jsmi $123                   ;0fb123
    jsnr $123                   ;0fc123
    jses $123                   ;0fd123
    jsls $123                   ;0fe123
    jsle $123                   ;0ff123

; expression result too large?    jmp  >$123456                   ;0af080123456
; expression result too large?    jscc >$123456                   ;0bf0a0123456
; expression result too large?    jshs >$123456                   ;0bf0a0123456
; expression result too large?    jsge >$123456                   ;0bf0a1123456
; expression result too large?    jsne >$123456                   ;0bf0a2123456
; expression result too large?    jspl >$123456                   ;0bf0a3123456
; expression result too large?    jsnn >$123456                   ;0bf0a4123456
; expression result too large?    jsec >$123456                   ;0bf0a5123456
; expression result too large?    jslc >$123456                   ;0bf0a6123456
; expression result too large?    jsgt >$123456                   ;0bf0a7123456
; expression result too large?    jscs >$123456                   ;0bf0a8123456
; expression result too large?    jslo >$123456                   ;0bf0a8123456
; expression result too large?    jslt >$123456                   ;0bf0a9123456
; expression result too large?    jseq >$123456                   ;0bf0aa123456
; expression result too large?    jsmi >$123456                   ;0bf0ab123456
; expression result too large?    jsnr >$123456                   ;0bf0ac123456
; expression result too large?    jses >$123456                   ;0bf0ad123456
; expression result too large?    jsls >$123456                   ;0bf0ae123456
; expression result too large?    jsle >$123456                   ;0bf0af123456

    bchg #5,X:$12               ;0b1205
    bchg #5,X:<<$ffe2           ;0ba205
    bchg #5,Y:$12               ;0b1245
    bchg #5,Y:<<$ffe2           ;0ba245

    bclr #5,X:$12               ;0a1205
    bclr #5,X:<<$ffe2           ;0aa205
    bclr #5,Y:$12               ;0a1245
    bclr #5,Y:<<$ffe2           ;0aa245

    bset #5,X:$12               ;0a1225
    bset #5,X:<<$ffe2           ;0aa225
    bset #5,Y:$12               ;0a1265
    bset #5,Y:<<$ffe2           ;0aa265

    btst #5,X:$12               ;0b1225
    btst #5,X:<<$ffe2           ;0ba225
    btst #5,Y:$12               ;0b1265
    btst #5,Y:<<$ffe2           ;0ba265

    rep X:$12               ;061120
    nop
    rep Y:$12               ;061160
    nop
    rep x:(r1)-n1           ;064120
    nop
    rep x:(r2)+n2           ;064a20
    nop
    rep x:(r3)-             ;065320
    nop
    rep x:(r4)+             ;065c20
    nop
    rep x:(r5)              ;066520
    nop
    rep x:(r6+n6)           ;066e20
    nop
    rep x:-(r7)             ;067f20
    nop
    rep y:(r1)-n1           ;064160
    nop
    rep y:(r2)+n2           ;064a60
    nop
    rep y:(r3)-             ;065360
    nop
    rep y:(r4)+             ;065c60
    nop
    rep y:(r5)              ;066560
    nop
    rep y:(r6+n6)           ;066e60
    nop
    rep y:-(r7)             ;067f60
    nop

    bchg #6,a0	                    ;0bc846
    bchg #6,b0                      ;0bc946
    bchg #6,a2	                    ;0bca46
    bchg #6,b2                      ;0bcb46
    bchg #6,a1	                    ;0bcc46
    bchg #6,b1                      ;0bcd46
    bchg #6,a                       ;0bce46
    bchg #6,b                       ;0bcf46
    bchg #6,r0	                    ;0bd046
    bchg #6,r1	                    ;0bd146
    bchg #6,r2	                    ;0bd246
    bchg #6,r3	                    ;0bd346
    bchg #6,r4	                    ;0bd446
    bchg #6,r5	                    ;0bd546
    bchg #6,r6	                    ;0bd646
    bchg #6,r7	                    ;0bd746
    bchg #6,n0                      ;0bd846
    bchg #6,n1                      ;0bd946
    bchg #6,n2                      ;0bda46
    bchg #6,n3                      ;0bdb46
    bchg #6,n4                      ;0bdc46
    bchg #6,n5                      ;0bdd46
    bchg #6,n6                      ;0bde46
    bchg #6,n7                      ;0bdf46
    bchg #6,m0                      ;0be046
    bchg #6,m1                      ;0be146
    bchg #6,m2                      ;0be246
    bchg #6,m3                      ;0be346
    bchg #6,m4                      ;0be446
    bchg #6,m5                      ;0be546
    bchg #6,m6                      ;0be646
    bchg #6,m7                      ;0be746
    bchg #6,sr	                    ;0be946
    bchg #6,omr                     ;0bea46
    bchg #6,sp	                    ;0beb46
    bchg #6,ssh                     ;0bec46
    bchg #6,ssl                     ;0bed46
    bchg #6,la                      ;0bee46
    bchg #6,lc                      ;0bef46

    bclr #6,a0	                    ;0ac846
    bclr #6,b0                      ;0ac946
    bclr #6,a2	                    ;0aca46
    bclr #6,b2                      ;0acb46
    bclr #6,a1	                    ;0acc46
    bclr #6,b1                      ;0acd46
    bclr #6,a                       ;0ace46
    bclr #6,b                       ;0acf46
    bclr #6,r0	                    ;0ad046
    bclr #6,r1	                    ;0ad146
    bclr #6,r2	                    ;0ad246
    bclr #6,r3	                    ;0ad346
    bclr #6,r4	                    ;0ad446
    bclr #6,r5	                    ;0ad546
    bclr #6,r6	                    ;0ad646
    bclr #6,r7	                    ;0ad746
    bclr #6,n0                      ;0ad846
    bclr #6,n1                      ;0ad946
    bclr #6,n2                      ;0ada46
    bclr #6,n3                      ;0adb46
    bclr #6,n4                      ;0adc46
    bclr #6,n5                      ;0add46
    bclr #6,n6                      ;0ade46
    bclr #6,n7                      ;0adf46
    bclr #6,m0                      ;0ae046
    bclr #6,m1                      ;0ae146
    bclr #6,m2                      ;0ae246
    bclr #6,m3                      ;0ae346
    bclr #6,m4                      ;0ae446
    bclr #6,m5                      ;0ae546
    bclr #6,m6                      ;0ae646
    bclr #6,m7                      ;0ae746
    bclr #6,sr	                    ;0ae946
    bclr #6,omr                     ;0aea46
    bclr #6,sp	                    ;0aeb46
    bclr #6,ssh                     ;0aec46
    bclr #6,ssl                     ;0aed46
    bclr #6,la                      ;0aee46
    bclr #6,lc                      ;0aef46

    bset #6,a0	                    ;0ac866
    bset #6,b0                      ;0ac966
    bset #6,a2	                    ;0aca66
    bset #6,b2                      ;0acb66
    bset #6,a1	                    ;0acc66
    bset #6,b1                      ;0acd66
    bset #6,a                       ;0ace66
    bset #6,b                       ;0acf66
    bset #6,r0	                    ;0ad066
    bset #6,r1	                    ;0ad166
    bset #6,r2	                    ;0ad266
    bset #6,r3	                    ;0ad366
    bset #6,r4	                    ;0ad466
    bset #6,r5	                    ;0ad566
    bset #6,r6	                    ;0ad666
    bset #6,r7	                    ;0ad766
    bset #6,n0                      ;0ad866
    bset #6,n1                      ;0ad966
    bset #6,n2                      ;0ada66
    bset #6,n3                      ;0adb66
    bset #6,n4                      ;0adc66
    bset #6,n5                      ;0add66
    bset #6,n6                      ;0ade66
    bset #6,n7                      ;0adf66
    bset #6,m0                      ;0ae066
    bset #6,m1                      ;0ae166
    bset #6,m2                      ;0ae266
    bset #6,m3                      ;0ae366
    bset #6,m4                      ;0ae466
    bset #6,m5                      ;0ae566
    bset #6,m6                      ;0ae666
    bset #6,m7                      ;0ae766
    bset #6,sr	                    ;0ae966
    bset #6,omr                     ;0aea66
    bset #6,sp	                    ;0aeb66
    bset #6,ssh                     ;0aec66
    bset #6,ssl                     ;0aed66
    bset #6,la                      ;0aee66
    bset #6,lc                      ;0aef66

    btst #6,a0	                    ;0bc866
    btst #6,b0                      ;0bc966
    btst #6,a2	                    ;0bca66
    btst #6,b2                      ;0bcb66
    btst #6,a1	                    ;0bcc66
    btst #6,b1                      ;0bcd66
    btst #6,a                       ;0bce66
    btst #6,b                       ;0bcf66
    btst #6,r0	                    ;0bd066
    btst #6,r1	                    ;0bd166
    btst #6,r2	                    ;0bd266
    btst #6,r3	                    ;0bd366
    btst #6,r4	                    ;0bd466
    btst #6,r5	                    ;0bd566
    btst #6,r6	                    ;0bd666
    btst #6,r7	                    ;0bd766
    btst #6,n0                      ;0bd866
    btst #6,n1                      ;0bd966
    btst #6,n2                      ;0bda66
    btst #6,n3                      ;0bdb66
    btst #6,n4                      ;0bdc66
    btst #6,n5                      ;0bdd66
    btst #6,n6                      ;0bde66
    btst #6,n7                      ;0bdf66
    btst #6,m0                      ;0be066
    btst #6,m1                      ;0be166
    btst #6,m2                      ;0be266
    btst #6,m3                      ;0be366
    btst #6,m4                      ;0be466
    btst #6,m5                      ;0be566
    btst #6,m6                      ;0be666
    btst #6,m7                      ;0be766
    btst #6,sr	                    ;0be966
    btst #6,omr                     ;0bea66
    btst #6,sp	                    ;0beb66
    btst #6,ssh                     ;0bec66
    btst #6,ssl                     ;0bed66
    btst #6,la                      ;0bee66
    btst #6,lc                      ;0bef66


    do x:(r1)-n1,z1             ;064100
    nop
    enddo                       ;00008c
z1: nop
    do x:(r2)+n2,z2             ;064a00
    nop
    enddo                       ;00008c
z2: nop
    do x:(r3)-,z3               ;065300
    nop
    enddo                       ;00008c
z3: nop
    do x:(r4)+,z4               ;065c00
    nop
    enddo                       ;00008c
z4: nop
    do x:(r5),z5                ;066500
    nop
    enddo                       ;00008c
z5: nop
    do x:(r6+n6),z6             ;066e00
    nop
    enddo                       ;00008c
z6: nop
    do x:-(r7),z7               ;067f00
    nop
    enddo                       ;00008c
z7: nop
    do y:(r1)-n1,z8             ;064140
    nop
    enddo                       ;00008c
z8: nop
    do y:(r2)+n2,z9             ;064a40
    nop
    enddo                       ;00008c
z9: nop
    do y:(r3)-,z10              ;065340
    nop
    enddo                       ;00008c
z10:nop
    do y:(r4)+,z11              ;065c40
    nop
    enddo                       ;00008c
z11:nop
    do y:(r5),z12               ;066540
    nop
    enddo                       ;00008c
z12:nop
    do y:(r6+n6),z13            ;066e40
    nop
    enddo                       ;00008c
z13:    nop
    do y:-(r7),z14              ;067f40
    nop
    enddo                       ;00008c
z14:nop
    do X:$12,z15                ;061200
    nop
    enddo                       ;00008c
z15:nop
    do Y:$12,z16                ;061240
    nop
    enddo                       ;00008c
z16:nop
    do #$777,z17                ;067787
    nop
    enddo                       ;00008c
z17:nop
    do x0,z18                   ;06c400
    nop
    enddo                       ;00008c
z18:nop
    do x1,z19                   ;06c500
    nop
    enddo                       ;00008c
z19:nop
    do y0,z20                   ;06c600
    nop
    enddo                       ;00008c
z20:nop
    do y1,z21                   ;06c700
    nop
    enddo                       ;00008c
z21:nop
    do a0,z22                   ;06c800
    nop
    enddo                       ;00008c
z22:nop
    do b0,z23                   ;06c900
    nop
    enddo                       ;00008c
z23:nop
    do a2,z24                   ;06ca00
    nop
    enddo                       ;00008c
z24:nop
    do b2,z25                   ;06cb00
    nop
    enddo                       ;00008c
z25:nop
    do a1,z26                   ;06cc00
    nop
    enddo                       ;00008c
z26:nop
    do b1,z27                   ;06cd00
    nop
    enddo                       ;00008c
z27:nop
    do a,z28                    ;06ce00
    nop
    enddo                       ;00008c
z28:nop
    do b,z29                    ;06cf00
    nop
    enddo                       ;00008c
z29:nop
    do r0,z30                   ;06d000
    nop
    enddo                       ;00008c
z30:nop
    do r1,z31                   ;06d100
    nop
    enddo                       ;00008c
z31:nop
    do r2,z32                   ;06d200
    nop
    enddo                       ;00008c
z32:nop
    do r3,z33                   ;06d300
    nop
    enddo                       ;00008c
z33:nop
    do r4,z34                   ;06d400
    nop
    enddo                       ;00008c
z34:nop
    do r5,z35                   ;06d500
    nop
    enddo                       ;00008c
z35:nop
    do r6,z36                   ;06d600
    nop
    enddo                       ;00008c
z36:nop
    do r7,z37                   ;06d700
    nop
    enddo                       ;00008c
z37:nop
    do n0,z38                   ;06d800
    nop
    enddo                       ;00008c
z38:nop
    do n1,z39                   ;06d900
    nop
    enddo                       ;00008c
z39:nop
    do n2,z40                   ;06da00
    nop
    enddo                       ;00008c
z40:nop
    do n3,z41                   ;06db00
    nop
    enddo                       ;00008c
z41:nop
    do n4,z42                   ;06dc00
    nop
    enddo                       ;00008c
z42:nop
    do n5,z43                   ;06dd00
    nop
    enddo                       ;00008c
z43:nop
    do n6,z44                   ;06de00
    nop
    enddo                       ;00008c
z44:nop
    do n7,z45                   ;06df00
    nop
    enddo                       ;00008c
z45:nop
    do m0,z46                   ;06e000
    nop
    enddo                       ;00008c
z46:nop
    do m1,z47                   ;06e100
    nop
    enddo                       ;00008c
z47:nop
    do m2,z48                   ;06e200
    nop
    enddo                       ;00008c
z48:nop
    do m3,z49                   ;06e300
    nop
    enddo                       ;00008c
z49:nop
    do m4,z50                   ;06e400
    nop
    enddo                       ;00008c
z50:nop
    do m5,z51                   ;06e500
    nop
    enddo                       ;00008c
z51:nop
    do m6,z52                   ;06e600
    nop
    enddo                       ;00008c
z52:nop
    do m7,z53                   ;06e700
    nop
    enddo                       ;00008c

z53:

    rep x0                            ;06c420
    nop
    rep x1                            ;06c520
    nop
    rep y0                            ;06c620
    nop
    rep y1                            ;06c720
    nop
    rep a0                            ;06c820
    nop
    rep b0                            ;06c920
    nop
    rep a2                            ;06ca20
    nop
    rep b2                            ;06cb20
    nop
    rep a1                            ;06cc20
    nop
    rep b1                            ;06cd20
    nop
    rep a                             ;06ce20
    nop
    rep b                             ;06cf20
    nop
    rep r0                            ;06d020
    nop
    rep r1                            ;06d120
    nop
    rep r2                            ;06d220
    nop
    rep r3                            ;06d320
    nop
    rep r4                            ;06d420
    nop
    rep r5                            ;06d520
    nop
    rep r6                            ;06d620
    nop
    rep r7                            ;06d720
    nop
    rep n0                            ;06d820
    nop
    rep n1                            ;06d920
    nop
    rep n2                            ;06da20
    nop
    rep n3                            ;06db20
    nop
    rep n4                            ;06dc20
    nop
    rep n5                            ;06dd20
    nop
    rep n6                            ;06de20
    nop
    rep n7                            ;06df20
    nop
    rep m0                            ;06e020
    nop
    rep m1                            ;06e120
    nop
    rep m2                            ;06e220
    nop
    rep m3                            ;06e320
    nop
    rep m4                            ;06e420
    nop
    rep m5                            ;06e520
    nop
    rep m6                            ;06e620
    nop
    rep m7                            ;06e720
    nop

    jsclr #$9,a0,$1234                 ;0bc809001234
    jsclr #$9,b0,$1234                 ;0bc909001234
    jsclr #$9,a2,$1234                 ;0bca09001234
    jsclr #$9,b2,$1234                 ;0bcb09001234
    jsclr #$9,a1,$1234                 ;0bcc09001234
    jsclr #$9,b1,$1234                 ;0bcd09001234
    jsclr #$9,a,$1234                 ;0bce09001234
    jsclr #$9,b,$1234                 ;0bcf09001234
    jsclr #$9,r0,$1234                 ;0bd009001234
    jsclr #$9,r1,$1234                 ;0bd109001234
    jsclr #$9,r2,$1234                 ;0bd209001234
    jsclr #$9,r3,$1234                 ;0bd309001234
    jsclr #$9,r4,$1234                 ;0bd409001234
    jsclr #$9,r5,$1234                 ;0bd509001234
    jsclr #$9,r6,$1234                 ;0bd609001234
    jsclr #$9,r7,$1234                 ;0bd709001234
    jsclr #$9,n0,$1234                 ;0bd809001234
    jsclr #$9,n1,$1234                 ;0bd909001234
    jsclr #$9,n2,$1234                 ;0bda09001234
    jsclr #$9,n3,$1234                 ;0bdb09001234
    jsclr #$9,n4,$1234                 ;0bdc09001234
    jsclr #$9,n5,$1234                 ;0bdd09001234
    jsclr #$9,n6,$1234                 ;0bde09001234
    jsclr #$9,n7,$1234                 ;0bdf09001234
    jsclr #$9,m0,$1234                 ;0be009001234
    jsclr #$9,m1,$1234                 ;0be109001234
    jsclr #$9,m2,$1234                 ;0be209001234
    jsclr #$9,m3,$1234                 ;0be309001234
    jsclr #$9,m4,$1234                 ;0be409001234
    jsclr #$9,m5,$1234                 ;0be509001234
    jsclr #$9,m6,$1234                 ;0be609001234
    jsclr #$9,m7,$1234                 ;0be709001234

    jsclr #$9,x:(r1)-n1,$1234          ;0b4189001234
    jsclr #$9,x:(r2)+n2,$1234          ;0b4a89001234
    jsclr #$9,x:(r3)-,$1234          ;0b5389001234
    jsclr #$9,x:(r4)+,$1234          ;0b5c89001234
    jsclr #$9,x:(r5),$1234          ;0b6589001234
    jsclr #$9,x:(r6+n6),$1234          ;0b6e89001234
    jsclr #$9,x:-(r7),$1234          ;0b7f89001234
    jsclr #$9,y:(r1)-n1,$1234          ;0b41c9001234
    jsclr #$9,y:(r2)+n2,$1234          ;0b4ac9001234
    jsclr #$9,y:(r3)-,$1234          ;0b53c9001234
    jsclr #$9,y:(r4)+,$1234          ;0b5cc9001234
    jsclr #$9,y:(r5),$1234          ;0b65c9001234
    jsclr #$9,y:(r6+n6),$1234          ;0b6ec9001234
    jsclr #$9,y:-(r7),$1234          ;0b7fc9001234
    jsclr #$9,X:$12,$1234          ;0b1289001234
;    jsclr #$9,X:<<$12,$1234          ;should produce an error - x:aaaaaa must be between $ffc0 and $ffff
    jsclr #$9,X:<<$ffe2,$1234          ;0ba289001234
    jsclr #$9,Y:$12,$1234          ;0b12c9001234
;    jsclr #$9,Y:<<$12,$1234          ;0b92c9001234
    jsclr #$9,Y:<<$ffe2,$1234          ;should produce an error - y:aaaaaa must be between $ffc0 and $ffff

    jset #$9,a0,$1234                 ;0ac829001234
    jset #$9,b0,$1234                 ;0ac929001234
    jset #$9,a2,$1234                 ;0aca29001234
    jset #$9,b2,$1234                 ;0acb29001234
    jset #$9,a1,$1234                 ;0acc29001234
    jset #$9,b1,$1234                 ;0acd29001234
    jset #$9,a,$1234                 ;0ace29001234
    jset #$9,b,$1234                 ;0acf29001234
    jset #$9,r0,$1234                 ;0ad029001234
    jset #$9,r1,$1234                 ;0ad129001234
    jset #$9,r2,$1234                 ;0ad229001234
    jset #$9,r3,$1234                 ;0ad329001234
    jset #$9,r4,$1234                 ;0ad429001234
    jset #$9,r5,$1234                 ;0ad529001234
    jset #$9,r6,$1234                 ;0ad629001234
    jset #$9,r7,$1234                 ;0ad729001234
    jset #$9,n0,$1234                 ;0ad829001234
    jset #$9,n1,$1234                 ;0ad929001234
    jset #$9,n2,$1234                 ;0ada29001234
    jset #$9,n3,$1234                 ;0adb29001234
    jset #$9,n4,$1234                 ;0adc29001234
    jset #$9,n5,$1234                 ;0add29001234
    jset #$9,n6,$1234                 ;0ade29001234
    jset #$9,n7,$1234                 ;0adf29001234
    jset #$9,m0,$1234                 ;0ae029001234
    jset #$9,m1,$1234                 ;0ae129001234
    jset #$9,m2,$1234                 ;0ae229001234
    jset #$9,m3,$1234                 ;0ae329001234
    jset #$9,m4,$1234                 ;0ae429001234
    jset #$9,m5,$1234                 ;0ae529001234
    jset #$9,m6,$1234                 ;0ae629001234
    jset #$9,m7,$1234                 ;0ae729001234

    jset #$9,x:(r1)-n1,$1234          ;0a41a9001234
    jset #$9,x:(r2)+n2,$1234          ;0a4aa9001234
    jset #$9,x:(r3)-,$1234          ;0a53a9001234
    jset #$9,x:(r4)+,$1234          ;0a5ca9001234
    jset #$9,x:(r5),$1234          ;0a65a9001234
    jset #$9,x:(r6+n6),$1234          ;0a6ea9001234
    jset #$9,x:-(r7),$1234          ;0a7fa9001234
    jset #$9,y:(r1)-n1,$1234          ;0a41e9001234
    jset #$9,y:(r2)+n2,$1234          ;0a4ae9001234
    jset #$9,y:(r3)-,$1234          ;0a53e9001234
    jset #$9,y:(r4)+,$1234          ;0a5ce9001234
    jset #$9,y:(r5),$1234          ;0a65e9001234
    jset #$9,y:(r6+n6),$1234          ;0a6ee9001234
    jset #$9,y:-(r7),$1234          ;0a7fe9001234
    jset #$9,X:$12,$1234           ;000000001234
;    jset #$9,X:<<$12,$1234           ;000000001234
    jset #$9,X:<<$ffe2,$1234           ;should produce an error - x:aaaaaa must be between $ffc0 and $ffff
    jset #$9,Y:$12,$1234           ;000000001234
;    jset #$9,Y:<<$12,$1234           ;000000001234
    jset #$9,Y:<<$ffe2,$1234           ;should produce an error - y:aaaaaa must be between $ffc0 and $ffff

    jsset #$9,x:(r1)-n1,$1234          ;0b41a9001234
    jsset #$9,x:(r2)+n2,$1234          ;0b4aa9001234
    jsset #$9,x:(r3)-,$1234          ;0b53a9001234
    jsset #$9,x:(r4)+,$1234          ;0b5ca9001234
    jsset #$9,x:(r5),$1234          ;0b65a9001234
    jsset #$9,x:(r6+n6),$1234          ;0b6ea9001234
    jsset #$9,x:-(r7),$1234          ;0b7fa9001234
    jsset #$9,y:(r1)-n1,$1234          ;0b41e9001234
    jsset #$9,y:(r2)+n2,$1234          ;0b4ae9001234
    jsset #$9,y:(r3)-,$1234          ;0b53e9001234
    jsset #$9,y:(r4)+,$1234          ;0b5ce9001234
    jsset #$9,y:(r5),$1234          ;0b65e9001234
    jsset #$9,y:(r6+n6),$1234          ;0b6ee9001234
    jsset #$9,y:-(r7),$1234          ;0b7fe9001234
    jsset #$9,X:$12,$1234          ;000000001234
;    jsset #$9,X:<<$12,$1234         ;should produce an error - x:aaaaaa must be between $ffc0 and $ffff
    jsset #$9,X:<<$ffe2,$1234          ;000000001234
    jsset #$9,Y:$12,$1234          ;000000001234
;    jsset #$9,Y:<<$12,$1234         ;should produce an error - y:aaaaaa must be between $ffc0 and $ffff
    jsset #$9,Y:<<$ffe2,$1234          ;000000001234

    jsset #$9,a0,$1234                 ;0bc829001234
    jsset #$9,b0,$1234                 ;0bc929001234
    jsset #$9,a2,$1234                 ;0bca29001234
    jsset #$9,b2,$1234                 ;0bcb29001234
    jsset #$9,a1,$1234                 ;0bcc29001234
    jsset #$9,b1,$1234                 ;0bcd29001234
    jsset #$9,a,$1234                 ;0bce29001234
    jsset #$9,b,$1234                 ;0bcf29001234
    jsset #$9,r0,$1234                 ;0bd029001234
    jsset #$9,r1,$1234                 ;0bd129001234
    jsset #$9,r2,$1234                 ;0bd229001234
    jsset #$9,r3,$1234                 ;0bd329001234
    jsset #$9,r4,$1234                 ;0bd429001234
    jsset #$9,r5,$1234                 ;0bd529001234
    jsset #$9,r6,$1234                 ;0bd629001234
    jsset #$9,r7,$1234                 ;0bd729001234
    jsset #$9,n0,$1234                 ;0bd829001234
    jsset #$9,n1,$1234                 ;0bd929001234
    jsset #$9,n2,$1234                 ;0bda29001234
    jsset #$9,n3,$1234                 ;0bdb29001234
    jsset #$9,n4,$1234                 ;0bdc29001234
    jsset #$9,n5,$1234                 ;0bdd29001234
    jsset #$9,n6,$1234                 ;0bde29001234
    jsset #$9,n7,$1234                 ;0bdf29001234
    jsset #$9,m0,$1234                 ;0be029001234
    jsset #$9,m1,$1234                 ;0be129001234
    jsset #$9,m2,$1234                 ;0be229001234
    jsset #$9,m3,$1234                 ;0be329001234
    jsset #$9,m4,$1234                 ;0be429001234
    jsset #$9,m5,$1234                 ;0be529001234
    jsset #$9,m6,$1234                 ;0be629001234
    jsset #$9,m7,$1234                 ;0be729001234

    jclr #$9,x:(r1)-n1,$1234          ;0a4189001234
    jclr #$9,x:(r2)+n2,$1234          ;0a4a89001234
    jclr #$9,x:(r3)-,$1234          ;0a5389001234
    jclr #$9,x:(r4)+,$1234          ;0a5c89001234
    jclr #$9,x:(r5),$1234          ;0a6589001234
    jclr #$9,x:(r6+n6),$1234          ;0a6e89001234
    jclr #$9,x:-(r7),$1234          ;0a7f89001234
    jclr #$9,y:(r1)-n1,$1234          ;0a41c9001234
    jclr #$9,y:(r2)+n2,$1234          ;0a4ac9001234
    jclr #$9,y:(r3)-,$1234          ;0a53c9001234
    jclr #$9,y:(r4)+,$1234          ;0a5cc9001234
    jclr #$9,y:(r5),$1234          ;0a65c9001234
    jclr #$9,y:(r6+n6),$1234          ;0a6ec9001234
    jclr #$9,y:-(r7),$1234          ;0a7fc9001234
    jclr #$9,X:$12,$1234           ;000000001234
;    jclr #$9,X:<<$12,$1234           ;000000001234
    jclr #$9,X:<<$ffe2,$1234           ;000000001234
;    jclr #$9,Y:$12,$1234           ;000000001234
;    jclr #$9,Y:<<$12,$1234           ;000000001234
    jclr #$9,Y:<<$ffe2,$1234           ;000000001234

    jclr #$9,a0,$1234                 ;0ac889001234
    jclr #$9,b0,$1234                 ;0ac989001234
    jclr #$9,a2,$1234                 ;0aca89001234
    jclr #$9,b2,$1234                 ;0acb89001234
    jclr #$9,a1,$1234                 ;0acc89001234
    jclr #$9,b1,$1234                 ;0acd89001234
    jclr #$9,a,$1234                 ;0ace89001234
    jclr #$9,b,$1234                 ;0acf89001234
    jclr #$9,r0,$1234                 ;0ad089001234
    jclr #$9,r1,$1234                 ;0ad189001234
    jclr #$9,r2,$1234                 ;0ad289001234
    jclr #$9,r3,$1234                 ;0ad389001234
    jclr #$9,r4,$1234                 ;0ad489001234
    jclr #$9,r5,$1234                 ;0ad589001234
    jclr #$9,r6,$1234                 ;0ad689001234
    jclr #$9,r7,$1234                 ;0ad789001234
    jclr #$9,n0,$1234                 ;0ad889001234
    jclr #$9,n1,$1234                 ;0ad989001234
    jclr #$9,n2,$1234                 ;0ada89001234
    jclr #$9,n3,$1234                 ;0adb89001234
    jclr #$9,n4,$1234                 ;0adc89001234
    jclr #$9,n5,$1234                 ;0add89001234
    jclr #$9,n6,$1234                 ;0ade89001234
    jclr #$9,n7,$1234                 ;0adf89001234
    jclr #$9,m0,$1234                 ;0ae089001234
    jclr #$9,m1,$1234                 ;0ae189001234
    jclr #$9,m2,$1234                 ;0ae289001234
    jclr #$9,m3,$1234                 ;0ae389001234
    jclr #$9,m4,$1234                 ;0ae489001234
    jclr #$9,m5,$1234                 ;0ae589001234
    jclr #$9,m6,$1234                 ;0ae689001234
    jclr #$9,m7,$1234                 ;0ae789001234

    lua (r5)-n5,r5              ;044515
    lua (r6)+n6,r2              ;044e12
    lua (r4)-,r3                ;045413
    nop
    lua (r3)+,r2                ;045b12
    lua (r5)-n5,n5              ;04451d
    lua (r6)+n6,n2              ;044e1a
    lua (r4)-,n3                ;04541b
    lua (r3)+,n2                ;045b1a

    norm r4,a                   ;01dc15
    norm r5,b                   ;01dd1d

    mac x0,x0,a                  ;200082
    mac -y0,y0,a                 ;200096
    mac x1,x0,a                  ;2000a2
    mac -y1,y0,a                 ;2000b6
    mac x0,y1,a                  ;2000c2
    mac -y0,x0,a                 ;2000d6
    mac x1,y0,a                  ;2000e2
    mac -y1,x1,a                 ;2000f6
    mac x0,x0,b                  ;20008a
    mac -y0,y0,b                 ;20009e
    mac x1,x0,b                  ;2000aa
    mac -y1,y0,b                 ;2000be
    mac x0,y1,b                  ;2000ca
    mac -y0,x0,b                 ;2000de
    mac x1,y0,b                  ;2000ea
    mac -y1,x1,b                 ;2000fe
    mac -y1,x1,b  (r5)+                     ;205dfe
    mac -y1,x1,b  x:(r2),r0                 ;60e2fe
    mac -y1,x1,b  x:(r5)+n5,a b,y0          ;1a8dfe
    mac -y1,x1,b  y:(r2),r0                 ;68e2fe
    mac -y1,x1,b  l:-(r4),a10               ;40fcfe
    mac -y1,x1,b  x1,x:(r0)+ y0,y:(r4)+n4   ;9418FE

    macr x0,x0,a                 ;200083
    macr -y0,y0,a                ;200097
    macr x1,x0,a                 ;2000a3
    macr -y1,y0,a                ;2000b7
    macr x0,y1,a                 ;2000c3
    macr -y0,x0,a                ;2000d7
    macr x1,y0,a                 ;2000e3
    macr -y1,x1,a                ;2000f7
    macr x0,x0,b                 ;20008b
    macr -y0,y0,b                ;20009f
    macr x1,x0,b                 ;2000ab
    macr -y1,y0,b                ;2000bf
    macr x0,y1,b                 ;2000cb
    macr -y0,x0,b                ;2000df
    macr x1,y0,b                 ;2000eb
    macr -y1,x1,b                ;2000ff

    mpy x0,x0,a                  ;200080
    mpy -y0,y0,a                 ;200094
    mpy x1,x0,a                  ;2000a0
    mpy -y1,y0,a                 ;2000b4
    mpy x0,y1,a                  ;2000c0
    mpy -y0,x0,a                 ;2000d4
    mpy x1,y0,a                  ;2000e0
    mpy -y1,x1,a                 ;2000f4
    mpy x0,x0,b                  ;200088
    mpy -y0,y0,b                 ;20009c
    mpy x1,x0,b                  ;2000a8
    mpy -y1,y0,b                 ;2000bc
    mpy x0,y1,b                  ;2000c8
    mpy -y0,x0,b                 ;2000dc
    mpy x1,y0,b                  ;2000e8
    mpy -y1,x1,b                 ;2000fc

    mpyr x0,x0,a                 ;200081
    mpyr -y0,y0,a                ;200095
    mpyr x1,x0,a                 ;2000a1
    mpyr -y1,y0,a                ;2000b5
    mpyr x0,y1,a                 ;2000c1
    mpyr -y0,x0,a                ;2000d5
    mpyr x1,y0,a                 ;2000e1
    mpyr -y1,x1,a                ;2000f5
    mpyr x0,x0,b                 ;200089
    mpyr -y0,y0,b                ;20009d
    mpyr x1,x0,b                 ;2000a9
    mpyr -y1,y0,b                ;2000bd
    mpyr x0,y1,b                 ;2000c9
    mpyr -y0,x0,b                ;2000dd
    mpyr x1,y0,b                 ;2000e9
    mpyr -y1,x1,b                ;2000fd
    
    movec y:-(r7),sr           ;05ff79
    movec y:-(r7),omr          ;05ff7a
    movec y:-(r7),sp           ;05ff7b
    movec y:-(r7),ssh          ;05ff7c
    movec y:-(r7),ssl          ;05ff7d
    movec y:-(r7),la           ;05ff7e
    movec y:-(r7),lc           ;05ff7f
    movec x:(r1)-n1,m0           ;05c120
    movec x:(r2)+n2,m0           ;05ca20
    movec x:(r3)-,m0           ;05d320
    movec x:(r4)+,m0           ;05dc20
    movec x:(r5),m0           ;05e520
    movec x:(r6+n6),m0           ;05ee20
    movec x:-(r7),m0           ;05ff20
    movec y:(r1)-n1,m0           ;05c160
    movec y:(r2)+n2,m0           ;05ca60
    movec y:(r3)-,m0           ;05d360
    movec y:(r4)+,m0           ;05dc60
    movec y:(r5),m0           ;05e560
    movec y:(r6+n6),m0           ;05ee60
    movec y:-(r7),m0           ;05ff60
    movec y:-(r7),m1           ;05ff61
    movec y:-(r7),m2           ;05ff62
    movec y:-(r7),m3           ;05ff63
    movec y:-(r7),m4           ;05ff64
    movec y:-(r7),m5           ;05ff65
    movec y:-(r7),m6           ;05ff66
    movec y:-(r7),m7           ;05ff67
    movec X:$1234,m0           ;05f020 001234
    movec Y:$1234,m0           ;05f060 001234
    movec #$1234,m0              ;05f460 001234
    movec X:$12,m0             ;059220
    movec Y:$12,m0             ;059260
    movec m0,X:$12               ;051220
    movec m0,Y:$12               ;051260
    movec x0,m0                  ;04c4a0
    movec x1,m0                  ;04c5a0
    movec y0,m0                  ;04c6a0
    movec y1,m0                  ;04c7a0
    movec a0,m0                  ;04c8a0
    movec b0,m0                  ;04c9a0
    movec a2,m0                  ;04caa0
    movec b2,m0                  ;04cba0
    movec a1,m0                  ;04cca0
    movec b1,m0                  ;04cda0
    movec a,m0                  ;04cea0
    movec b,m0                  ;04cfa0
    movec r0,m0                  ;04d0a0
    movec r1,m0                  ;04d1a0
    movec r2,m0                  ;04d2a0
    movec r3,m0                  ;04d3a0
    movec r4,m0                  ;04d4a0
    movec r5,m0                  ;04d5a0
    movec r6,m0                  ;04d6a0
    movec r7,m0                  ;04d7a0
    movec n0,m0                  ;04d8a0
    movec n1,m0                  ;04d9a0
    movec n2,m0                  ;04daa0
    movec n3,m0                  ;04dba0
    movec n4,m0                  ;04dca0
    movec n5,m0                  ;04dda0
    movec n6,m0                  ;04dea0
    movec n7,m0                  ;04dfa0
    movec m0,m0                  ;0460a0
    movec m1,m0                  ;0460a1
    movec m2,m0                  ;0460a2
    movec m3,m0                  ;0460a3
    movec m4,m0                  ;0460a4
    movec m5,m0                  ;0460a5
    movec m6,m0                  ;0460a6
    movec m7,m0                  ;0460a7
    movec sr,m0                  ;0460b9
    movec omr,m0                 ;???? it was originally 'om' here 0460ba
    movec sp,m0                  ;0460bb
    movec ssh,m0                  ;??? it was originally 'ss' here 0460bc
    movec ssl,m0                  ;??? it was originally 'ss' here 0460bd
    movec la,m0                  ;0460be
    movec lc,m0                  ;0460bf
    movec lc,m1                  ;0461bf
    movec lc,m2                  ;0462bf
    movec lc,m3                  ;0463bf
    movec lc,m4                  ;0464bf
    movec lc,m5                  ;0465bf
    movec lc,m6                  ;0466bf
    movec lc,m7                  ;0467bf
    movec lc,sr                  ;0479bf
    movec lc,omr                 ;047abf
    movec lc,sp                  ;047bbf
    movec lc,ssh                 ;047cbf
    movec lc,ssl                 ;047dbf
    movec lc,la                  ;047ebf
    movec lc,lc                  ;047fbf
    movec m0,x0                  ;0444a0
    movec m0,x1                  ;0445a0
    movec m0,y0                  ;0446a0
    movec m0,y1                  ;0447a0
    movec m0,a0                  ;0448a0
    movec m0,b0                  ;0449a0
    movec m0,a2                  ;044aa0
    movec m0,b2                  ;044ba0
    movec m0,a1                  ;044ca0
    movec m0,b1                  ;044da0
    movec m0,a                   ;044ea0
    movec m0,b                   ;044fa0
    movec m0,r0                  ;0450a0
    movec m0,r1                  ;0451a0
    movec m0,r2                  ;0452a0
    movec m0,r3                  ;0453a0
    movec m0,r4                  ;0454a0
    movec m0,r5                  ;0455a0
    movec m0,r6                  ;0456a0
    movec m0,r7                  ;0457a0
    movec m0,n0                  ;0458a0
    movec m0,n1                  ;0459a0
    movec m0,n2                  ;045aa0
    movec m0,n3                  ;045ba0
    movec m0,n4                  ;045ca0
    movec m0,n5                  ;045da0
    movec m0,n6                  ;045ea0
    movec m0,n7                  ;045fa0
    movec m0,m0                  ;0460a0
    movec m0,m1                  ;0461a0
    movec m0,m2                  ;0462a0
    movec m0,m3                  ;0463a0
    movec m0,m4                  ;0464a0
    movec m0,m5                  ;0465a0
    movec m0,m6                  ;0466a0
    movec m0,m7                  ;0467a0
    movec m0,sr                  ;0479a0
    movec m0,omr                 ;047aa0
    movec m0,sp                  ;047ba0
    movec m0,ssh                 ;047ca0
    movec m0,ssl                 ;047da0
    movec m0,la                  ;047ea0
    movec m0,lc                  ;047fa0
    movec m1,lc                  ;047fa1
    movec m2,lc                  ;047fa2
    movec m3,lc                  ;047fa3
    movec m4,lc                  ;047fa4
    movec m5,lc                  ;047fa5
    movec m6,lc                  ;047fa6
    movec m7,lc                  ;047fa7
    movec sr,lc                  ;047fb9
    movec omr,lc                  ;047fba
    movec sp,lc                  ;047fbb
    movec ssh,lc                  ;047fbc
    movec ssl,lc                  ;047fbd
    movec la,lc                  ;047fbe
    movec lc,lc                  ;047fbf
    movec #240,m0                ;05f0a0
    movec #240,m1                ;05f0a1
    movec #240,m2                ;05f0a2
    movec #240,m3                ;05f0a3
    movec #240,m4                ;05f0a4
    movec #240,m5                ;05f0a5
    movec #240,m6                ;05f0a6
    movec #240,m7                ;05f0a7
    movec #240,sr                ;05f0b9
    movec #240,omr               ;05f0ba
    movec #240,sp                ;05f0bb
    movec #240,ssh               ;05f0bc
    movec #240,ssl               ;05f0bd
    movec #240,la                ;05f0be
    movec #240,lc                ;05f0bf

    movem p:(r1)-n1,r0           ;07c190
    movem p:(r2)+n2,r0           ;07ca90
    movem p:(r3)-,r0           ;07d390
    movem p:(r4)+,r0           ;07dc90
    movem p:(r5),r0           ;07e590
    movem p:(r6+n6),r0           ;07ee90
    movem p:-(r7),r0           ;07ff90
    movem p:-(r7),r1           ;07ff91
    movem p:-(r7),r2           ;07ff92
    movem p:-(r7),r3           ;07ff93
    movem p:-(r7),r4           ;07ff94
    movem p:-(r7),r5           ;07ff95
    movem p:-(r7),r6           ;07ff96
    movem p:-(r7),r7           ;07ff97
    nop
    movem p:-(r7),n0           ;07ff98
    movem p:-(r7),n1           ;07ff99
    movem p:-(r7),n2           ;07ff9a
    movem p:-(r7),n3           ;07ff9b
    movem p:-(r7),n4           ;07ff9c
    movem p:-(r7),n5           ;07ff9d
    movem p:-(r7),n6           ;07ff9e
    movem p:-(r7),n7           ;07ff9f
    movem p:-(r7),m0           ;07ffa0
    movem p:-(r7),m1           ;07ffa1
    movem p:-(r7),m2           ;07ffa2
    movem p:-(r7),m3           ;07ffa3
    movem p:-(r7),m4           ;07ffa4
    movem p:-(r7),m5           ;07ffa5
    movem p:-(r7),m6           ;07ffa6
    movem p:-(r7),m7           ;07ffa7
    nop
    movem p:-(r7),sr           ;07ffb9
    nop
    movem p:-(r7),omr          ;07ffba
    movem p:-(r7),sp           ;07ffbb
    movem p:-(r7),ssh          ;07ffbc
    movem p:-(r7),ssl          ;07ffbd
    movem p:-(r7),la           ;07ffbe
    movem p:-(r7),lc           ;07ffbf
    movem p:-(r7),x0           ;07ff84
    movem p:-(r7),x1           ;07ff85
    movem p:-(r7),y0           ;07ff86
    movem p:-(r7),y1           ;07ff87
    movem p:-(r7),a0           ;07ff88
    movem p:-(r7),b0           ;07ff89
    movem p:-(r7),a2           ;07ff8a
    movem p:-(r7),b2           ;07ff8b
    movem p:-(r7),a1           ;07ff8c
    movem p:-(r7),b1           ;07ff8d
    movem p:-(r7),a            ;07ff8e
    movem p:-(r7),b            ;07ff8f
    
    movem p:$1234,r0           ;07f090
    movem m1,p:-(r7)      ;077fa1
    movem p:$12,r0           ;079210
    movem p:$12,r1           ;079211
    movem p:$12,r2           ;079212
    movem p:$12,r3           ;079213
    movem p:$12,r4           ;079214
    movem p:$12,r5           ;079215
    movem p:$12,r6           ;079216
    movem p:$12,r7           ;079217
    movem p:$12,n0           ;079218
    movem p:$12,n1           ;079219
    movem p:$12,n2           ;07921a
    movem p:$12,n3           ;07921b
    movem p:$12,n4           ;07921c
    movem p:$12,n5           ;07921d
    movem p:$12,n6           ;07921e
    movem p:$12,n7           ;07921f
    movem p:$12,m0           ;079220
    movem p:$12,m1           ;079221
    movem p:$12,m2           ;079222
    movem p:$12,m3           ;079223
    movem p:$12,m4           ;079224
    movem p:$12,m5           ;079225
    movem p:$12,m6           ;079226
    movem p:$12,m7           ;079227
    movem p:$12,sr           ;079239
    movem p:$12,omr          ;07923a
    movem p:$12,sp           ;07923b
    movem p:$12,ssh          ;07923c
    movem p:$12,ssl          ;07923d
    movem p:$12,la           ;07923e
    movem p:$12,lc           ;07923f
    movem lc,p:$12           ;07123f
    
    movep x:(r1)-n1,x:<<$fffe   ; 08c1be
    movep x:(r2)+n2,x:<<$fffe   ; 08cabe
    movep x:(r3)-,x:<<$fffe   ; 08d3be
    movep x:(r4)+,x:<<$fffe   ; 08dcbe
    movep x:(r5),x:<<$fffe   ; 08e5be
    movep x:(r6+n6),x:<<$fffe   ; 08eebe
    movep x:-(r7),x:<<$fffe   ; 08ffbe
    movep y:(r1)-n1,x:<<$fffe   ; 08c1fe
    movep y:(r2)+n2,x:<<$fffe   ; 08cafe
    movep y:(r3)-,x:<<$fffe   ; 08d3fe
    movep y:(r4)+,x:<<$fffe   ; 08dcfe
    movep y:(r5),x:<<$fffe   ; 08e5fe
    movep y:(r6+n6),x:<<$fffe   ; 08eefe
    movep y:-(r7),x:<<$fffe   ; 08fffe
    movep #$1234,x:<<$fffe   ; 08f4fe 001234
    movep x:<<$fffe,x:(r1)-n1 ; 0841be
    movep x:<<$fffe,x:(r2)+n2 ; 084abe
    movep x:<<$fffe,x:(r3)-   ; 0853be
    movep x:<<$fffe,x:(r4)+   ; 085cbe
    movep x:<<$fffe,x:(r5)    ; 0865be
    movep x:<<$fffe,x:(r6+n6) ; 086ebe
    movep x:<<$fffe,x:-(r7)   ; 087fbe
    movep x:<<$fffe,y:(r1)-n1 ; 0841fe
    movep x:<<$fffe,y:(r2)+n2 ; 084afe
    movep x:<<$fffe,y:(r3)-   ; 0853fe
    movep x:<<$fffe,y:(r4)+   ; 085cfe
    movep x:<<$fffe,y:(r5)    ; 0865fe
    movep x:<<$fffe,y:(r6+n6) ; 086efe
    movep x:<<$fffe,y:-(r7)   ; 087ffe
    movep x:(r1)-n1,y:<<$fffe   ; 09c1be
    movep x:(r2)+n2,y:<<$fffe   ; 09cabe
    movep x:(r3)-,y:<<$fffe   ; 09d3be
    movep x:(r4)+,y:<<$fffe   ; 09dcbe
    movep x:(r5),y:<<$fffe   ; 09e5be
    movep x:(r6+n6),y:<<$fffe   ; 09eebe
    movep x:-(r7),y:<<$fffe   ; 09ffbe
    movep y:(r1)-n1,y:<<$fffe   ; 09c1fe
    movep y:(r2)+n2,y:<<$fffe   ; 09cafe
    movep y:(r3)-,y:<<$fffe   ; 09d3fe
    movep y:(r4)+,y:<<$fffe   ; 09dcfe
    movep y:(r5),y:<<$fffe   ; 09e5fe
    movep y:(r6+n6),y:<<$fffe   ; 09eefe
    movep y:-(r7),y:<<$fffe   ; 09fffe
    movep #$1234,y:<<$fffe   ; 09f4be
    movep y:<<$fffe,x:(r1)-n1 ; 0941be
    movep y:<<$fffe,x:(r2)+n2 ; 094abe
    movep y:<<$fffe,x:(r3)-   ; 0953be
    movep y:<<$fffe,x:(r4)+   ; 095cbe
    movep y:<<$fffe,x:(r5)    ; 0965be
    movep y:<<$fffe,x:(r6+n6) ; 096ebe
    movep y:<<$fffe,x:-(r7)   ; 097fbe
    movep y:<<$fffe,y:(r1)-n1 ; 0941fe
    movep y:<<$fffe,y:(r2)+n2 ; 094afe
    movep y:<<$fffe,y:(r3)-   ; 0953fe
    movep y:<<$fffe,y:(r4)+   ; 095cfe
    movep y:<<$fffe,y:(r5)    ; 0965fe
    movep y:<<$fffe,y:(r6+n6) ; 096efe
    movep y:<<$fffe,y:-(r7)   ; 097ffe

    movep p:(r1)-n1,x:<<$fffe   ; 08c17e
    movep p:(r2)+n2,x:<<$fffe   ; 08ca7e
    movep p:(r3)-,x:<<$fffe   ; 08d37e
    movep p:(r4)+,x:<<$fffe   ; 08dc7e
    movep p:(r5),x:<<$fffe   ; 08e57e
    movep p:(r6+n6),x:<<$fffe   ; 08ee7e
    movep p:-(r7),x:<<$fffe   ; 08ff7e
    movep x:<<$fffe,p:(r1)-n1 ; 08417e
    movep x:<<$fffe,p:(r2)+n2 ; 084a7e
    movep x:<<$fffe,p:(r3)-   ; 08537e
    movep x:<<$fffe,p:(r4)+   ; 085c7e
    movep x:<<$fffe,p:(r5)    ; 08657e
    movep x:<<$fffe,p:(r6+n6) ; 086e7e
    movep x:<<$fffe,p:-(r7)   ; 087f7e
    movep p:(r1)-n1,y:<<$fffe   ; 09c17e
    movep p:(r2)+n2,y:<<$fffe   ; 09ca7e
    movep p:(r3)-,y:<<$fffe   ; 09d37e
    movep p:(r4)+,y:<<$fffe   ; 09dc7e
    movep p:(r5),y:<<$fffe   ; 09e57e
    movep p:(r6+n6),y:<<$fffe   ; 09ee7e
    movep p:-(r7),y:<<$fffe   ; 09ff7e
    movep y:<<$fffe,p:(r1)-n1 ; 09417e
    movep y:<<$fffe,p:(r2)+n2 ; 094a7e
    movep y:<<$fffe,p:(r3)-   ; 09537e
    movep y:<<$fffe,p:(r4)+   ; 095c7e
    movep y:<<$fffe,p:(r5)    ; 09657e
    movep y:<<$fffe,p:(r6+n6) ; 096e7e
    movep y:<<$fffe,p:-(r7)   ; 097f7e

    movep x0,x:<<$fffe  ; 08c43e
    movep x1,x:<<$fffe  ; 08c53e
    movep y0,x:<<$fffe  ; 08c63e
    movep y1,x:<<$fffe  ; 08c73e
    movep a0,x:<<$fffe  ; 08c83e
    movep b0,x:<<$fffe  ; 08c93e
    movep a2,x:<<$fffe  ; 08ca3e
    movep b2,x:<<$fffe  ; 08cb3e
    movep a1,x:<<$fffe  ; 08cc3e
    movep b1,x:<<$fffe  ; 08cd3e
    movep a,x:<<$fffe   ; 08ce3e
    movep b,x:<<$fffe   ; 08cf3e
    movep r0,x:<<$fffe  ; 08d03e
    movep r1,x:<<$fffe  ; 08d13e
    movep r2,x:<<$fffe  ; 08d23e
    movep r3,x:<<$fffe  ; 08d33e
    movep r4,x:<<$fffe  ; 08d43e
    movep r5,x:<<$fffe  ; 08d53e
    movep r6,x:<<$fffe  ; 08d63e
    movep r7,x:<<$fffe  ; 08d73e
    movep n0,x:<<$fffe  ; 08d83e
    movep n1,x:<<$fffe  ; 08d93e
    movep n2,x:<<$fffe  ; 08da3e
    movep n3,x:<<$fffe  ; 08db3e
    movep n4,x:<<$fffe  ; 08dc3e
    movep n5,x:<<$fffe  ; 08dd3e
    movep n6,x:<<$fffe  ; 08de3e
    movep n7,x:<<$fffe  ; 08df3e
    movep m0,x:<<$fffe  ; 08e03e
    movep m1,x:<<$fffe  ; 08e13e
    movep m2,x:<<$fffe  ; 08e23e
    movep m3,x:<<$fffe  ; 08e33e
    movep m4,x:<<$fffe  ; 08e43e
    movep m5,x:<<$fffe  ; 08e53e
    movep m6,x:<<$fffe  ; 08e63e
    movep m7,x:<<$fffe  ; 08e73e
    movep sr,x:<<$fffe  ; 08f93e
    movep omr,x:<<$fffe ; 08fa3e
    movep sp,x:<<$fffe  ; 08fb3e
    movep ssh,x:<<$fffe ; 08fc3e
    movep ssl,x:<<$fffe ; 08fd3e
    movep la,x:<<$fffe  ; 08fe3e
    movep lc,x:<<$fffe  ; 08ff3e
    movep x0,y:<<$fffe  ; 09c43e
    movep x1,y:<<$fffe  ; 09c53e
    movep y0,y:<<$fffe  ; 09c63e
    movep y1,y:<<$fffe  ; 09c73e
    movep a0,y:<<$fffe  ; 09c83e
    movep b0,y:<<$fffe  ; 09c93e
    movep a2,y:<<$fffe  ; 09ca3e
    movep b2,y:<<$fffe  ; 09cb3e
    movep a1,y:<<$fffe  ; 09cc3e
    movep b1,y:<<$fffe  ; 09cd3e
    movep a,y:<<$fffe   ; 09ce3e
    movep b,y:<<$fffe   ; 09cf3e
    movep r0,y:<<$fffe  ; 09d03e
    movep r1,y:<<$fffe  ; 09d13e
    movep r2,y:<<$fffe  ; 09d23e
    movep r3,y:<<$fffe  ; 09d33e
    movep r4,y:<<$fffe  ; 09d43e
    movep r5,y:<<$fffe  ; 09d53e
    movep r6,y:<<$fffe  ; 09d63e
    movep r7,y:<<$fffe  ; 09d73e
    movep n0,y:<<$fffe  ; 09d83e
    movep n1,y:<<$fffe  ; 09d93e
    movep n2,y:<<$fffe  ; 09da3e
    movep n3,y:<<$fffe  ; 09db3e
    movep n4,y:<<$fffe  ; 09dc3e
    movep n5,y:<<$fffe  ; 09dd3e
    movep n6,y:<<$fffe  ; 09de3e
    movep n7,y:<<$fffe  ; 09df3e
    movep m0,y:<<$fffe  ; 09e03e
    movep m1,y:<<$fffe  ; 09e13e
    movep m2,y:<<$fffe  ; 09e23e
    movep m3,y:<<$fffe  ; 09e33e
    movep m4,y:<<$fffe  ; 09e43e
    movep m5,y:<<$fffe  ; 09e53e
    movep m6,y:<<$fffe  ; 09e63e
    movep m7,y:<<$fffe  ; 09e73e
    movep sr,y:<<$fffe  ; 09f93e
    movep omr,y:<<$fffe ; 09fa3e
    movep sp,y:<<$fffe  ; 09fb3e
    movep ssh,y:<<$fffe ; 09fc3e
    movep ssl,y:<<$fffe ; 09fd3e
    movep la,y:<<$fffe  ; 09fe3e
    movep lc,y:<<$fffe  ; 09ff3e
    movep x:<<$fffe,x0  ; 08443e
    movep x:<<$fffe,x1  ; 08453e
    movep x:<<$fffe,y0  ; 08463e
    movep x:<<$fffe,y1  ; 08473e
    movep x:<<$fffe,a0  ; 08483e
    movep x:<<$fffe,b0  ; 08493e
    movep x:<<$fffe,a2  ; 084a3e
    movep x:<<$fffe,b2  ; 084b3e
    movep x:<<$fffe,a1  ; 084c3e
    movep x:<<$fffe,b1  ; 084d3e
    movep x:<<$fffe,a   ; 084e3e
    movep x:<<$fffe,b   ; 084f3e
    movep x:<<$fffe,r0  ; 08503e
    movep x:<<$fffe,r1  ; 08513e
    movep x:<<$fffe,r2  ; 08523e
    movep x:<<$fffe,r3  ; 08533e
    movep x:<<$fffe,r4  ; 08543e
    movep x:<<$fffe,r5  ; 08553e
    movep x:<<$fffe,r6  ; 08563e
    movep x:<<$fffe,r7  ; 08573e
    movep x:<<$fffe,n0  ; 08583e
    movep x:<<$fffe,n1  ; 08593e
    movep x:<<$fffe,n2  ; 085a3e
    movep x:<<$fffe,n3  ; 085b3e
    movep x:<<$fffe,n4  ; 085c3e
    movep x:<<$fffe,n5  ; 085d3e
    movep x:<<$fffe,n6  ; 085e3e
    movep x:<<$fffe,n7  ; 085f3e
    movep x:<<$fffe,m0  ; 08603e
    movep x:<<$fffe,m1  ; 08613e
    movep x:<<$fffe,m2  ; 08623e
    movep x:<<$fffe,m3  ; 08633e
    movep x:<<$fffe,m4  ; 08643e
    movep x:<<$fffe,m5  ; 08653e
    movep x:<<$fffe,m6  ; 08663e
    movep x:<<$fffe,m7  ; 08673e
    movep x:<<$fffe,sr  ; 08793e
    movep x:<<$fffe,omr ; 087a3e
    movep x:<<$fffe,sp  ; 087b3e
    movep x:<<$fffe,ssh ; 087c3e
    movep x:<<$fffe,ssl ; 087d3e
    movep x:<<$fffe,la  ; 087e3e
    movep x:<<$fffe,lc  ; 087f3e
    movep y:<<$fffe,x0  ; 09443e
    movep y:<<$fffe,x1  ; 09453e
    movep y:<<$fffe,y0  ; 09463e
    movep y:<<$fffe,y1  ; 09473e
    movep y:<<$fffe,a0  ; 09483e
    movep y:<<$fffe,b0  ; 09493e
    movep y:<<$fffe,a2  ; 094a3e
    movep y:<<$fffe,b2  ; 094b3e
    movep y:<<$fffe,a1  ; 094c3e
    movep y:<<$fffe,b1  ; 094d3e
    movep y:<<$fffe,a   ; 094e3e
    movep y:<<$fffe,b   ; 094f3e
    movep y:<<$fffe,r0  ; 09503e
    movep y:<<$fffe,r1  ; 09513e
    movep y:<<$fffe,r2  ; 09523e
    movep y:<<$fffe,r3  ; 09533e
    movep y:<<$fffe,r4  ; 09543e
    movep y:<<$fffe,r5  ; 09553e
    movep y:<<$fffe,r6  ; 09563e
    movep y:<<$fffe,r7  ; 09573e
    movep y:<<$fffe,n0  ; 09583e
    movep y:<<$fffe,n1  ; 09593e
    movep y:<<$fffe,n2  ; 095a3e
    movep y:<<$fffe,n3  ; 095b3e
    movep y:<<$fffe,n4  ; 095c3e
    movep y:<<$fffe,n5  ; 095d3e
    movep y:<<$fffe,n6  ; 095e3e
    movep y:<<$fffe,n7  ; 095f3e
    movep y:<<$fffe,m0  ; 09603e
    movep y:<<$fffe,m1  ; 09613e
    movep y:<<$fffe,m2  ; 09623e
    movep y:<<$fffe,m3  ; 09633e
    movep y:<<$fffe,m4  ; 09643e
    movep y:<<$fffe,m5  ; 09653e
    movep y:<<$fffe,m6  ; 09663e
    movep y:<<$fffe,m7  ; 09673e
    movep y:<<$fffe,sr  ; 09793e
    movep y:<<$fffe,omr ; 097a3e
    movep y:<<$fffe,sp  ; 097b3e
    movep y:<<$fffe,ssh ; 097c3e
    movep y:<<$fffe,ssl ; 097d3e
    movep y:<<$fffe,la  ; 097e3e
    movep y:<<$fffe,lc  ; 097f3e


    dc 1.0/3 
    dc 2.5+1.3  ;float+float
    nop
;    dc 2.5+1    ;float+int
;    dc 2+1.3    ;int+float
;    dc 2+1      ;int+int
;    dc 2.5-1.3  ;float-float
;    ;dc 2.5-1    ;float-int
;    dc 2-1.3    ;int-float
;    dc 2-1      ;int-int
;    dc -2.5     ;-float
;    dc -2       ;-int
;    ;dc !2.5     ;!float (error)
;    dc !1       ;!int
;    ;dc ~2.5     ;~float (error)
;    dc ~1       ;~int
;    dc 2.5<=1.3 ;float<=float
;    dc 2.5<=1   ;float<=int
;    dc 2<=1.3   ;int<=float
;    dc 2<=1     ;int<=int
;    dc 2.5>=1.3 ;float>=float
;    dc 2.5>=1   ;float>=int
;    dc 2>=1.3   ;int>=float
;    dc 2>=1     ;int>=int
;    dc 2.5<1.3  ;float<float
;    dc 2.5<1    ;float<int
;    dc 2<1.3    ;int<float
;    dc 2<1      ;int<int
;    dc 2.5>1.3  ;float>float
;    dc 2.5>1    ;float>int
;    dc 2>1.3    ;int>float
;    dc 2>1      ;int>int
;    dc 2.5*1.3  ;float*float
;    dc 2.5*1    ;float*int
;    dc 2*1.3    ;int*float
;    dc 2*1      ;int*int
;    dc 2.5/1.3  ;float/float
;    dc 2.5/1    ;float/int
;    dc 2/1.3    ;int/float
;    dc 2/1      ;int/int
;    ;dc 2.5%7.3  ;float%float (error)
;    dc 2%7    ;int  %int
;    ;dc 2.5<<1.3 ;float<<float (error)
;    dc 2<<1   ;int  <<int
;    ;dc 2.5>>1.3 ;float>>float (error)
;    dc 2>>1   ;int  >>int
;    ;dc 2.5&1.3  ;float&float (error)
;    dc 2&1    ;int  &int
;    ;dc 2.5^1.3  ;float^float (error)
;    dc 2^1    ;int  ^int
;    ;dc 2.5|1.3  ;float|float (error)
;    dc 2|1    ;int  |int
;
;    dc 3+0.5,0.299+2,6+3                   ;2645a2
;    dc $123456,$98765,500 
    nop

