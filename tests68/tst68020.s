	; This is my original input test source file
	opt d+
	opt o-
L0:
	move.b ([L0,pc],d2.w*2,$400),d1
	move.b ([L0,pc],a3.w*8,$401),d1
	move.b ([L0,pc,d2.w*2],$400),d1
	move.b ([L0,pc,a3.w*8],$401),d1
	move.b ([L0,pc],$400),d1
	move.b ([L0,pc],$401),d1
	move.b ([a1],a3.w*8,$401),d1
	move.b ([a0],$400),d1
	move.b ([$10,a0],d2.w*2,$400),d1
	move.b ([$10,a1],a3.w*8,$401),d1
	move.b 16(a1,a3.w*8),d1
	move.b ([$0],d2.w*2,$400),d1
	move.b ($400,d2.w*2),d1
	move.b d1,(d2.w*8)
	move.b ([$10,a0,d2.w*2],$400),d1
	move.b ([$10,a1,a3.w*8],$401),d1
	move.b ([$10,d2.l*2],$400),d1
	move.b ([$10,a3.l],$401),d1
L1:
	move.w sr,4(a7,d7.l*2)
	move.w sr,4(a7,d6.l*4)
	move.w sr,4(a7,d5.l*8)
	move.w L1(pc,d7.l),d0
	move.w L1(pc,d7.l*4),sr
	move.b 0(a5,a6.l*4),(a7)
	move.b 0(a5,d0.l*4),(a0)
	move.b L1(pc,a1.l*4),d0
	move.w ($1234,a0,d2.w),d0
	move.w ($12345678,a0,d2.w),d0
	move.w ([$2000,a7]),d0
	move.w ([$64]),d0
	jmp ([d7.l])
	bkpt #$4
	move.w ccr,(a0)
	rtd #-$100
	movec	sfc,d0
	movec	dfc,d2
	movec	usp,d4
	movec	vbr,d6
	movec	cacr,a1
	movec	caar,a3
	movec	msp,a4
	movec	isp,a5
	movec	d0,sfc
	movec	d2,dfc
	movec	d3,usp
	movec	d6,vbr
	movec	a0,cacr
	movec	a2,caar
	movec	a4,msp
	movec	a6,isp
	moves.w a0,(a1)
	moves.w -(a5),d3
	moves.w ([$100,a0],d1.w),d0
	moves.w a5,([$100,a0,a3.w*4])
	bfchg	(a0){15:3}
	bfchg	2043(a0){30:d3}
	bfclr	L2{d0:d7}
	bfclr	(a0){d0:32}
	bfexts	L2(pc){d0:32},d1
	bfextu	L2(pc){d1:32},d6
	bfffo	L2(pc){1:10},d7
	bfins	d7,2(a0){1:10}
	bfset	2(a0){1:10}
	bftst	L2(pc){1:10}
L2:
	bsr.l	L1
	callm	#$ff,0(a6,d0.w*8)
	callm	#$34,L2(pc)
	cas.b	d1,d3,(a7)
	cas.w	d1,d3,(a7)
	cas.l	d0,d7,-(a7)
	cas2.w	d6:d1,d2:d3,(a5):(d4)
	chk.l	127(a0,d1.l*8),d7
	chk2.b	$12345678.l,d7
	chk2.l	$5678.w,a3
	cmp2.b	$5678.w,a3
	divs.l	3(a0),d0
	divs.l	3(a0),d0:d1
	divs.l	L2(pc),d1:d0
	divs.l	L2(pc),d0
	divsl.l	L2(pc),d7:d6
	divu.l	3(a0),d0
	divu.l	3(a0),d0:d1
	divu.l	L2(pc),d1:d0
	divu.l	L2(pc),d0
	divul.l	L2(pc),d7:d6
	extb.l	d5
	link.l	a3,#-$12345678
	muls.l	3(a0),d0
	muls.l	3(a0),d0:d1
	muls.l	L2(pc),d1:d0
	muls.l	L2(pc),d0
	mulu.l	3(a0),d0
	mulu.l	3(a0),d0:d1
	mulu.l	L2(pc),d1:d0
	mulu.l	L2(pc),d0
	pack	-(a1),-(a7),#$1234
	pack	d7,d1,#$8765
	rtm	a6
	rtm	d1
	trapf
	traphi
	trapls
	trapcc
	trapcs
	trapne
	trapeq
	trapvc
	trapvs
	trappl
	trapmi
	trapge
	traplt
	trapgt
	traple
	trapf.w  #$1234
	traphi.w #$1234
	trapls.w #$1234
	trapcc.w #$1234
	trapcs.w #$1234
	trapne.w #$1234
	trapeq.w #$1234
	trapvc.w #$1234
	trapvs.w #$1234
	trappl.w #$1234
	trapmi.w #$1234
	trapge.w #$1234
	traplt.w #$1234
	trapgt.w #$1234
	traple.w #$1234
	trapcc.l #$12345678
	trapcs.l #$12345678
	trapne.l #$12345678
	trapeq.l #$12345678
	trapvc.l #$12345678
	trapvs.l #$12345678
	unpk	-(a1),-(a7),#$1234
	unpk d5,d7,#$8889
