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