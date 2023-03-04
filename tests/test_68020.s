L0:
	move.w	sr,4(a7,d7.l*2)
	move.w	sr,4(a7,d6.l*4)
	move.w	sr,4(a7,d5.l*8)
	move.w	L0(pc,d7.l*4),sr
	move.b 0(a5,a6.l*4),(a7)
	move.b 0(a5,d0.l*4),(a0)
