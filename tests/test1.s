	opt	d+
	illegal
	reset
	nop  
	rte  
	rts  
	trapv
	rtr  
	stop	#$2300
	swap	d7
	link.w	a6,#$100
	link.w	a6,#-$100
	unlk	a1
	move	usp,a7
	move	a1,usp
	trap	#$e
	move.w	(a7)+,sr
	move.w	$1010.l,sr
	move.w	$1010.w,sr
	move.w	sr,-(a7)
	move.w	sr,4(a7,d7.l)
	nbcd.b	(a1)+
	pea	4(a7)
	pea $1234.w
	pea $cafebad.l
	pea (a0)
	lea 1235(a0),a7
	lea -35(a6,d7.w),a7
	tas	3(a7,d7.w)
	jsr	(a3)
	jsr L2
	jsr $7ba0.w
	jmp	5(a3)
	jmp L1
	jsr	L2
	jmp	L0
	jmp -5(a2,d7.l)
	asl.w	(a2)
	asl.w	L2
	asr.w	L3
	asl.w	L2
	asr.w	L3
	or.l	L0(pc),d7
	or.l	L0(pc,d7.w),d7
	asr.w	-(a2)
	bchg.b	#$3,(a7)
L0:
	movem.l	d0-d6,-(a7)
	movem.l	d0/d2/d4/a0/a2/a5,-(a7)
	movem.l	d0/d2/d4/a0/a2/a5,$ffff8800.w
	movem.l	(a7)+,d0-d6
	movem.w	d0-d7/a0-a5,-(a7)
	movem.w	(a7)+,d1-d7/a0-a5
	ori.b	#$12,(a7)
	ori.w	#$1234,(a7)
	ori.l	#$12345678,(a7)
	andi.b	#$12,(a7)
	andi.w	#$1234,(a7)
	andi.b	#$12,3(a7)
	andi.l	#$12345678,(a7)
	subi.w	#$4678,(a7)
	subi.l	#$12345678,(a7)
	addi.l	#$12345678,(a7)
	eori.w	#$5678,(a7)
	eori.l	#$12345678,(a7)
	cmpi.l	#$12345678,(a7)
	negx.b	(a7)
	negx.w	d0
	negx.l	d0
	negx.w	(a7)
	negx.l	(a7)
	clr.b	-(a7)
	clr.w	-(a7)
	clr.l	-(a7)
	clr.w	d7
	neg.l	d7
	not.b	d7
	tst.b	(a7)
	bra.s	L0
	bra.w	L0
	bcc.w	L0
	bra.s	L1
	bhi.w	L1
	bcc.w	L1
	ext.w	d7
	ext.l	d1
	movep.w	d7,0(a7)
	movep.w	1(a7),d7
	movep.l	d7,-2(a7)
	movep.l	-3(a7),d7
	bchg	d7,(a4)+
	bclr	d1,(a5)+
	bset	d2,(a6)+
	btst	d3,(a7)+
	dbmi	d7,L0
	dbge	d7,L0
	dbf	d1,L0
	dbge	d2,L0
	st	4(a7)
	scc	4(a7)
	scs	4(a7)
	seq	4(a7)
	sge	4(a7)
	sgt	4(a7)
	shi	4(a7)
	subq.w	#$1,d7
	subq.l	#$2,(a7)
	subq.l	#$2,$ffff8240.w
	subq.l	#$2,$7fff.w
	addq.w	#$1,d7
	addq.l	#$8,a0
	addq.l	#$2,(a7)
	addq.l	#$2,-(a7)
L1:
	bra.s	L1
	bra.w	L1
	bsr.s	L1
	bsr.w	L1
	sbcd	d7,d1
	sbcd	-(a7),-(a1)
	abcd	d7,d1
	abcd	-(a7),-(a1)
	sub.w	d7,(a7)
	sub.w	(a7),d7
	sub.l	d7,(a7)
	sub.l	(a7),d7
	add.w	d7,(a7)
	add.w	(a7),d7
	add.l	d7,(a7)
	add.l	(a7),d7
	or.w	d7,(a7)
	or.w	(a7),d7
	or.l	d7,(a7)
	or.l	(a7),d7
	and.w	d7,(a7)
	and.w	(a7),d7
	and.l	d7,(a7)
	and.l	(a7),d7
	subx.w	d7,d1
	subx.l	d7,d1
	subx.w	-(a7),-(a1)
	subx.l	-(a7),-(a1)
	addx.w	d7,d1
	addx.l	d7,d1
	addx.w	-(a7),-(a1)
	addx.l	-(a7),-(a1)
	cmpm.b	(a7)+,(a7)+
	cmpm.w	(a7)+,(a7)+
	cmpm.l	(a7)+,(a7)+
	cmp.b	(a7),d7
	cmp.w	(a1),d7
	cmp.l	(a2),d7
	cmpa.w	(a3)+,a7
	cmpa.l	-(a4),a7
	eor.w	d2,d7
	eor.l	d2,(a7)
	eor.b	d2,L2
	mulu.w	(a1)+,d7
	muls.w	-(a1),d7
	asr.b	#$1,d7
	ror.w	d7,d1
	asl.w	#$3,d1
	asr.w	#$3,d1
	lsl.w	#$3,d1
	lsr.w	#$3,d1
	roxl.w	#$3,d1
	roxl.w	#$3,d1
	rol.w	#$3,d1
	ror.w	#$3,d1
	chk.w	(a7),d7
	divu.w	d2,d7
	divu.w	3000(a7),d7
	movea.w	(a7)+,a7
	movea.w	10(a7),a7
	movea.l	(a7)+,a7
	movea.l	10(a7),a7
	exg	d0,d7
	exg	a0,a7
	exg	d0,a7
	move.w	#$2700,sr
	ori	#$700,sr
	andi	#$1234,sr
	move	#$ff,ccr
	move	(a7)+,ccr
	ori	#$ff,ccr
	andi	#$ff,ccr
	cmpa.l	d2,a0
	cmpa.l	4(a0),a1
	suba.l	d2,a0
	suba.l	-(a4),a0
	adda.l	d2,a0
	addx.l	d2,d4
	subx.l	d2,d1
	ror.w	(a0)
	rol.w	4(a0)
	roxr.w	0(a0,d0.w)
	roxl.w	$1234.l
	
L2:	dc.w	0
L3:	dc.w	0