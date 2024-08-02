	.org	p:0

	adc	y,a	b,x:(r1)+	x0,b

	abs 	a
	abs	b

	adc	x,a
	adc	x,b
	adc	y,a
	adc	y,b

	add	b,a
	add	a,b
	add	x,a
	add	x,b
	add	y,a
	add	y,b
	add	x0,a
	add	x0,b
	add	y0,a
	add	y0,b
	add	x1,a
	add	x1,b
	add	y1,a
	add	y1,b
	addl	a,b
	addl	b,a
	addr	a,b
	addr	b,a
	and	x0,a
	and	x1,a
	and	y0,a
	and	y1,a
	and	x0,b
	and	x1,b
	and	y0,b
	and	y1,b
	asl	a
	asl	b
	asr	a
	asr	b
	clr	a
	clr	b
	cmp	b,a
	cmp	a,b
	cmp	x0,a
	cmp	x0,b
	cmp	y0,a
	cmp	y0,b
	cmp	x1,a
	cmp	x1,b
	cmp	y1,a
	cmp	y1,b

	cmpm	b,a
	cmpm	a,b
 	cmpm	x0,a
	cmpm	x0,b
	cmpm	y0,a
	cmpm	y0,b
	cmpm	x1,a
	cmpm	x1,b
	cmpm	y1,a
	cmpm	y1,b

	eor	x0,a
	eor	x1,a
	eor	y0,a
	eor	y1,a
	eor	x0,b
	eor	x1,b
	eor	y0,b
	eor	y1,b

	lsl	a
	lsl	b
	lsr	a
	lsr	b

	mac	x0,x0,a
	mac	y0,y0,a
	mac	x1,x0,a
	mac	y1,y0,a
	mac	x0,y1,a
	mac	y0,x0,a
	mac	x1,y0,a
	mac	y1,x1,a
	mac	-x0,x0,a
	mac	-y0,y0,a
	mac	-x1,x0,a
	mac	-y1,y0,a
	mac	-x0,y1,a
	mac	-y0,x0,a
	mac	-x1,y0,a
	mac	-y1,x1,a
	mac	x0,x0,b
	mac	y0,y0,b
	mac	x1,x0,b
	mac	y1,y0,b
	mac	x0,y1,b
	mac	y0,x0,b
	mac	x1,y0,b
	mac	y1,x1,b
	mac	-x0,x0,b
	mac	-y0,y0,b
	mac	-x1,x0,b
	mac	-y1,y0,b
	mac	-x0,y1,b
	mac	-y0,x0,b
	mac	-x1,y0,b
	mac	-y1,x1,b

	macr	x0,x0,a
	macr	y0,y0,a
	macr	x1,x0,a
	macr	y1,y0,a
	macr	x0,y1,a
	macr	y0,x0,a
	macr	x1,y0,a
	macr	y1,x1,a
	macr	-x0,x0,a
	macr	-y0,y0,a
	macr	-x1,x0,a
	macr	-y1,y0,a
	macr	-x0,y1,a
	macr	-y0,x0,a
	macr	-x1,y0,a
	macr	-y1,x1,a
	macr	x0,x0,b
	macr	y0,y0,b
	macr	x1,x0,b
	macr	y1,y0,b
	macr	x0,y1,b
	macr	y0,x0,b
	macr	x1,y0,b
	macr	y1,x1,b
	macr	-x0,x0,b
	macr	-y0,y0,b
	macr	-x1,x0,b
	macr	-y1,y0,b
	macr	-x0,y1,b
	macr	-y0,x0,b
	macr	-x1,y0,b
	macr	-y1,x1,b

	mpy	x0,x0,a
	mpy	y0,y0,a
	mpy	x1,x0,a
	mpy	y1,y0,a
	mpy	x0,y1,a
	mpy	y0,x0,a
	mpy	x1,y0,a
	mpy	y1,x1,a
	mpy	-x0,x0,a
	mpy	-y0,y0,a
	mpy	-x1,x0,a
	mpy	-y1,y0,a
	mpy	-x0,y1,a
	mpy	-y0,x0,a
	mpy	-x1,y0,a
	mpy	-y1,x1,a
	mpy	x0,x0,b
	mpy	y0,y0,b
	mpy	x1,x0,b
	mpy	y1,y0,b
	mpy	x0,y1,b
	mpy	y0,x0,b
	mpy	x1,y0,b
	mpy	y1,x1,b
	mpy	-x0,x0,b
	mpy	-y0,y0,b
	mpy	-x1,x0,b
	mpy	-y1,y0,b
	mpy	-x0,y1,b
	mpy	-y0,x0,b
	mpy	-x1,y0,b
	mpy	-y1,x1,b

	mpyr	x0,x0,a
	mpyr	y0,y0,a
	mpyr	x1,x0,a
	mpyr	y1,y0,a
	mpyr	x0,y1,a
	mpyr	y0,x0,a
	mpyr	x1,y0,a
	mpyr	y1,x1,a
	mpyr	-x0,x0,a
	mpyr	-y0,y0,a
	mpyr	-x1,x0,a
	mpyr	-y1,y0,a
	mpyr	-x0,y1,a
	mpyr	-y0,x0,a
	mpyr	-x1,y0,a
	mpyr	-y1,x1,a
	mpyr	x0,x0,b
	mpyr	y0,y0,b
	mpyr	x1,x0,b
	mpyr	y1,y0,b
	mpyr	x0,y1,b
	mpyr	y0,x0,b
	mpyr	x1,y0,b
	mpyr	y1,x1,b
	mpyr	-x0,x0,b
	mpyr	-y0,y0,b
	mpyr	-x1,x0,b
	mpyr	-y1,y0,b
	mpyr	-x0,y1,b
	mpyr	-y0,x0,b
	mpyr	-x1,y0,b
	mpyr	-y1,x1,b

	neg	a
	neg	b
	not	a
	not	b

	or	x0,a
	or	x1,a
	or	y0,a
	or	y1,a
	or	x0,b
	or	x1,b
	or	y0,b
	or	y1,b

	rnd	a
	rnd	b
	rol	a
	rol	b
	ror	a
	ror	b

	sbc	x,a
	sbc	x,b
	sbc	y,a
	sbc	y,b

	sub	b,a
	sub	a,b
	sub	x,a
	sub	x,b
	sub	y,a
	sub	y,b
	sub	x0,a
	sub	x0,b
	sub	y0,a
	sub	y0,b
	sub	x1,a
	sub	x1,b
	sub	y1,a
	sub	y1,b

	subl	b,a
	subl	a,b
	subr	b,a
	subr	a,b


	tfr	b,a
	tfr	a,b
	tfr	x0,a
	tfr	x0,b
	tfr	x1,a
	tfr	x1,b
	tfr	y0,a
	tfr	y0,b
	tfr	y1,a
	tfr	y1,b

	tst	a
	tst	b

	abs 	a
	abs	b

	adc	x,a
	adc	x,b
	adc	y,a
	adc	y,b

	adc	x,a	#100,x0
	adc	x,a	#100,x1
	adc	y,a	b,x:(r1)+	x0,b

	adc	x,a	#-100,x0
	adc	x,a	#-100,x1

	adc	x,a	#-100,y0
	adc	x,a	#-100,y1
	adc	x,b	#100,a
	adc	x,a	#100,b
	adc	x,b	#100,a0
	adc	x,a	#100,b0
	adc	x,a	#100,x0
	adc	x,a	#100,x0

	adc	x,a	x0,y0
	adc	x,a	y1,n5
	adc	x,a	(r0)+n0
	adc	x,a	(r1)-n1
	adc	x,a	(r6)-
	adc	x,a	(r7)+

	adc	x,b	x:(r5)-n5,a
	adc	x,b	x:(r0)+n0,a
	adc	x,b	x:(r5)-,a
	adc	x,b	x:(r2)+,a
	adc	x,b	x:(r6+n6),a
	adc	x,b	x:(r7),a
	adc	x,b	x:-(r1),r3
	adc	x,b	y:(r1)-n1,a
	adc	x,a	y:(r0)+n0,b
	adc	x,b	y:(r5)-,a
	adc	x,b	y:(r2)+,x0
	adc	x,b	y:(r6+n6),x1
	adc	x,b	y:(r7),n2
	adc	x,b	y:-(r1),a

	adc	x,b	#6,x0
	adc	x,b	#$6000,x0
	adc	x,b	#$ffa000,x0
	adc	x,b	x:$15,x0
	adc	x,b	x:$1500,x0
	adc	x,b	x0,x:$15

	adc	x,b	a,x:(r5)-n5
	adc	x,b			a,y:(r5)-n5
	adc	x,b	a,x:(r0)+n0
	adc	x,b			a,y:(r0)+n0
	adc	x,b	a,x:(r5)-
	adc	x,b			a,y:(r5)-
	adc	x,b	a,x:(r0)+
	adc	x,b			a,y:(r0)+

	adc	y,b	a,x:$1234	a,y0
	adc	y,a	b,x:(r1)+	x0,b

	adc	y,a	#$abcdef,r0

	adc	y,a	y:$1234,b1

	adc	x,a	b,x1		y:(r6)-n6,b
	adc	x,a	y0,b		b,y:(r1)+
	adc	y,b	y0,a		a,y:(r5+n5)
	adc	y,b	a10,l:$1234
	adc	y,b	b10,l:$1234
	adc	y,b	a,l:$1234
	adc	y,b	a,l:$1234
	adc	y,b	x,l:$1234
	adc	y,b	y,l:$1234
	adc	y,b	ab,l:$1234
	adc	y,b	ba,l:$1234

	adc	x,a	x1,x:(r0)+	y0,y:(r4)+n4

	add 	x0,a 	a,x1		a,y:(r1)+
	addl	a,b	#0,r0
	addr 	b,a	x0,x:(r1)+n1	y0,y:(r4)-
	and	x0,a	(r5)-n5

	andi	#$fe,ccr
	andi	#$0,omr
	andi	#$fe,mr

	asl	a	(r3)-
	asr 	b	x:-(r3),r3

	debug
	debugcs
	enddo
	illegal
	nop
	reset
	rti
	rts
	stop
	swi
	wait

	bchg	#$7,x:<<$ffe2
	bchg	#$7,y:<<$ffe2


	bchg	#$7,y:(r4)+
	bchg	#$7,y:(r4)+n4

	div	x0,a
	div	x0,b
	div	y0,a
	div	y0,b
	div	x1,a
	div	x1,b
	div	y1,a
	div	y1,b

	tle	b,a
	tle	a,b
	tle	x0,a
	tle	x0,b
	tle	x1,a
	tle	x1,b
	tge	y0,a
	tle	y0,b
	tle	y1,a
	tle	y1,b

	tcc	y0,b
	tge	y0,b
	tne	y0,b
	tpl	y0,b
	tnn	y0,b
	tec	y0,b
	tlc	y0,b
	tgt	y0,b
	tcs	y0,b
	tlt	y0,b
	teq	y0,b
	tmi	y0,b
	tnr	y0,b
	tes	y0,b
	tls	y0,b
	tle	y0,b

	jmp	$fff
	jcc	$137
	jge	$137
	jne	$137
	jpl	$137
	jnn	$137
	jec	$137
	jlc	$137
	jgt	$137
	jcs	$137
	jlt	$137
	jeq	$137
	jmi	$137
	jnr	$137
	jes	$137
	jls	$137
	jle	$137

	jsr	$fff
	jscc	$137
	jsge	$137
	jsne	$137
	jspl	$137
	jsnn	$137
	jsec	$137
	jslc	$137
	jsgt	$137
	jscs	$137
	jslt	$137
	jseq	$137
	jsmi	$137
	jsnr	$137
	jses	$137
	jsls	$137
	jsle	$137

	jmp	(r1+n1)
	jmp	(r7)-n7
	jmp	(r7)+n7
	jmp	(r7)-
	jmp	(r7)+
	jmp	(r7)
	jmp	(r7+n7)
	jmp	-(r7)
	jne	$1000

	jsr	(r1+n1)
	jclr	#$0,x:<<$ffe9,$101
	jclr	#$9,x:$12,$101
	jclr	#$7,y:(r0)+,$101

	jscc	(r7)-n7
	jscc	(r7)+n7
	jscc	(r7)-
	jscc	(r7)+
	jscc	(r7)
	jscc	(r7+n7)
	jscc	-(r0)
	jscc	$1000
	jscc	(r1+n1)
	jscc	(r7)-n7
	jscc	(r7)+n7
	jscc	(r7)-
	jscc	(r7)+
	jscc	(r7)
	jscc	(r7+n7)
	jscc	-(r0)
	jscc	$1000

	bchg	#$7,y:$12

	bchg	#$5,a
	bchg	#$5,b
	bchg	#$5,x0
	bchg	#$5,x1
	bchg	#$5,y0
	bchg	#$5,y1
	bchg	#$5,r5
	bchg	#$5,m0
	bchg	#$5,m7
	bchg	#$5,r0
	bchg	#$5,r7
	bchg	#$5,n0
	bchg	#$5,n7
	btst	#$5,omr
	btst	#$5,sr
	btst	#$5,sp
	btst	#$5,ssh
	btst	#$5,ssl
	btst	#$5,la
	btst	#$5,lc

	do	y:$10,$923
	do	y:$10,$923
	do	x:(r0),$923
	do	y:(r0),$923
	do	#$777,$923
	do	#$37,$923
	do	a,$923

	rep	y:$10
	rep	y:$10
	rep	x:(r0)
	rep	y:(r0)
	rep	#$777
	rep	#$37
	rep	a

	do	#$777,$124
	do	#$37,$125

	lua	(r0)+n0,r1
	lua	(r0)-n0,r1
	lua	(r0)+,r1
	lua	(r0)-,r1
	lua	(r7)+n7,n1
	lua	(r4)-n4,n1
	lua	(r4)+,n1
	lua	(r4)-,n1

	norm	r0,a
	norm	r7,b

	movec	#$9f,sr
	movec	x:(r0+n0),sr
	movec	sr,x:(r0+n0)

	movec	#$123456,la

	movec	x:$15,lc
	movec	y:$15,lc
	movec	lc,x:$15
	movec	lc,y:$15

	movec	la,la
	movec	la,x0
	movec	x0,lc

	movem	x0,p:(r0)-n0
	movem	p:(r5+n5),lc
	movem	p:$19,omr

	movep	x:(r0),x:<<$ffe9

	movep	#$123456,x:<<$ffe9
	movep	x:<<$ffe9,y:$12345
	movep	x:<<$ffe9,omr

	; Suspicious ggn tests
	neg a	x:(r0),x0		y:(r4)+,y0
	neg a	x:(r0)+n0,x0		y:(r5)-,y0
	neg a	x:(r0)-,x0		y:(r6)+n6,y0
	neg a	x:(r0)+,x0		y:(r7),y0