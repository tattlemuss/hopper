// DO NOT EDIT
// Generated by nonp_gen.py

// Typedef for a single non-pmove decoder function
typedef int (*dsp_decoder)(nonp_context& ctx);

// Decode with top bits = %0
static int decode_idx_00(nonp_context& ctx)
{
  if ((ctx.header & 0xfffff) == 0x0008c)
     return decode_nonp_none(ctx, Opcode::O_ENDDO);
  if ((ctx.header & 0xfffff) == 0x00005)
     return decode_nonp_none(ctx, Opcode::O_ILLEGAL);
  if ((ctx.header & 0xfffff) == 0x00000)
     return decode_nonp_none(ctx, Opcode::O_NOP);
  if ((ctx.header & 0xfffff) == 0x00084)
     return decode_nonp_none(ctx, Opcode::O_RESET);
  if ((ctx.header & 0xfffff) == 0x00004)
     return decode_nonp_none(ctx, Opcode::O_RTI);
  if ((ctx.header & 0xfffff) == 0x0000c)
     return decode_nonp_none(ctx, Opcode::O_RTS);
  if ((ctx.header & 0xfffff) == 0x00087)
     return decode_nonp_none(ctx, Opcode::O_STOP);
  if ((ctx.header & 0xfffff) == 0x00006)
     return decode_nonp_none(ctx, Opcode::O_SWI);
  if ((ctx.header & 0xfffff) == 0x00086)
     return decode_nonp_none(ctx, Opcode::O_WAIT);
  if ((ctx.header & 0xfffff) == 0x00200)
     return decode_nonp_none(ctx, Opcode::O_DEBUG);
  if ((ctx.header & 0xfffff) == 0x00300)
     return decode_nonp_none(ctx, Opcode::O_DEBUGCC);
  if ((ctx.header & 0xfffff) == 0x00308)
     return decode_nonp_none(ctx, Opcode::O_DEBUGCS);
  if ((ctx.header & 0xfffff) == 0x00305)
     return decode_nonp_none(ctx, Opcode::O_DEBUGEC);
  if ((ctx.header & 0xfffff) == 0x0030a)
     return decode_nonp_none(ctx, Opcode::O_DEBUGEQ);
  if ((ctx.header & 0xfffff) == 0x0030d)
     return decode_nonp_none(ctx, Opcode::O_DEBUGES);
  if ((ctx.header & 0xfffff) == 0x00301)
     return decode_nonp_none(ctx, Opcode::O_DEBUGGE);
  if ((ctx.header & 0xfffff) == 0x00307)
     return decode_nonp_none(ctx, Opcode::O_DEBUGGT);
  if ((ctx.header & 0xfffff) == 0x00306)
     return decode_nonp_none(ctx, Opcode::O_DEBUGLC);
  if ((ctx.header & 0xfffff) == 0x0030f)
     return decode_nonp_none(ctx, Opcode::O_DEBUGLE);
  if ((ctx.header & 0xfffff) == 0x0030e)
     return decode_nonp_none(ctx, Opcode::O_DEBUGLS);
  if ((ctx.header & 0xfffff) == 0x00309)
     return decode_nonp_none(ctx, Opcode::O_DEBUGLT);
  if ((ctx.header & 0xfffff) == 0x0030b)
     return decode_nonp_none(ctx, Opcode::O_DEBUGMI);
  if ((ctx.header & 0xfffff) == 0x00302)
     return decode_nonp_none(ctx, Opcode::O_DEBUGNE);
  if ((ctx.header & 0xfffff) == 0x0030c)
     return decode_nonp_none(ctx, Opcode::O_DEBUGNR);
  if ((ctx.header & 0xfffff) == 0x00303)
     return decode_nonp_none(ctx, Opcode::O_DEBUGPL);
  if ((ctx.header & 0xfffff) == 0x00304)
     return decode_nonp_none(ctx, Opcode::O_DEBUGNN);
  if ((ctx.header & 0xf00fc) == 0x000b8)
     return decode_nonp_iiiiiiiiee(ctx, Opcode::O_ANDI);
  if ((ctx.header & 0xf00fc) == 0x000f8)
     return decode_nonp_iiiiiiiiee(ctx, Opcode::O_ORI);
  return 1;
}

// Decode with top bits = %1
static int decode_idx_01(nonp_context& ctx)
{
  if ((ctx.header & 0xf00fc) == 0x000b8)
     return decode_nonp_iiiiiiiiee(ctx, Opcode::O_ANDI);
  if ((ctx.header & 0xf00fc) == 0x000f8)
     return decode_nonp_iiiiiiiiee(ctx, Opcode::O_ORI);
  return 1;
}

// Decode with top bits = %10
static int decode_idx_02(nonp_context& ctx)
{
  if ((ctx.header & 0xf00fc) == 0x000b8)
     return decode_nonp_iiiiiiiiee(ctx, Opcode::O_ANDI);
  if ((ctx.header & 0xf00fc) == 0x000f8)
     return decode_nonp_iiiiiiiiee(ctx, Opcode::O_ORI);
  return 1;
}

// Decode with top bits = %11
static int decode_idx_03(nonp_context& ctx)
{
  if ((ctx.header & 0xf00fc) == 0x000b8)
     return decode_nonp_iiiiiiiiee(ctx, Opcode::O_ANDI);
  if ((ctx.header & 0xf00fc) == 0x000f8)
     return decode_nonp_iiiiiiiiee(ctx, Opcode::O_ORI);
  return 1;
}

// Decode with top bits = %100
static int decode_idx_04(nonp_context& ctx)
{
  return 1;
}

// Decode with top bits = %101
static int decode_idx_05(nonp_context& ctx)
{
  return 1;
}

// Decode with top bits = %110
static int decode_idx_06(nonp_context& ctx)
{
  if ((ctx.header & 0xfffc7) == 0x18040)
     return decode_nonp_jjd(ctx, Opcode::O_DIV);
  return 1;
}

// Decode with top bits = %111
static int decode_idx_07(nonp_context& ctx)
{
  if ((ctx.header & 0xff8f7) == 0x1d815)
     return decode_nonp_rrrd(ctx, Opcode::O_NORM);
  return 1;
}

// Decode with top bits = %1000
static int decode_idx_08(nonp_context& ctx)
{
  if ((ctx.header & 0xef880) == 0x20000)
     return decode_nonp_etttjjjdTTT(ctx, Opcode::O_TCC);
  if ((ctx.header & 0xef880) == 0x21000)
     return decode_nonp_etttjjjdTTT(ctx, Opcode::O_TGE);
  if ((ctx.header & 0xef880) == 0x22000)
     return decode_nonp_etttjjjdTTT(ctx, Opcode::O_TNE);
  if ((ctx.header & 0xef880) == 0x23000)
     return decode_nonp_etttjjjdTTT(ctx, Opcode::O_TPL);
  return 1;
}

// Decode with top bits = %1001
static int decode_idx_09(nonp_context& ctx)
{
  if ((ctx.header & 0xef880) == 0x25000)
     return decode_nonp_etttjjjdTTT(ctx, Opcode::O_TEC);
  if ((ctx.header & 0xef880) == 0x27000)
     return decode_nonp_etttjjjdTTT(ctx, Opcode::O_TGT);
  if ((ctx.header & 0xef880) == 0x26000)
     return decode_nonp_etttjjjdTTT(ctx, Opcode::O_TLC);
  if ((ctx.header & 0xef880) == 0x24000)
     return decode_nonp_etttjjjdTTT(ctx, Opcode::O_TNN);
  return 1;
}

// Decode with top bits = %1010
static int decode_idx_0a(nonp_context& ctx)
{
  if ((ctx.header & 0xef880) == 0x28000)
     return decode_nonp_etttjjjdTTT(ctx, Opcode::O_TCS);
  if ((ctx.header & 0xef880) == 0x2a000)
     return decode_nonp_etttjjjdTTT(ctx, Opcode::O_TEQ);
  if ((ctx.header & 0xef880) == 0x29000)
     return decode_nonp_etttjjjdTTT(ctx, Opcode::O_TLT);
  if ((ctx.header & 0xef880) == 0x2b000)
     return decode_nonp_etttjjjdTTT(ctx, Opcode::O_TMI);
  return 1;
}

// Decode with top bits = %1011
static int decode_idx_0b(nonp_context& ctx)
{
  if ((ctx.header & 0xef880) == 0x2d000)
     return decode_nonp_etttjjjdTTT(ctx, Opcode::O_TES);
  if ((ctx.header & 0xef880) == 0x2f000)
     return decode_nonp_etttjjjdTTT(ctx, Opcode::O_TLE);
  if ((ctx.header & 0xef880) == 0x2e000)
     return decode_nonp_etttjjjdTTT(ctx, Opcode::O_TLS);
  if ((ctx.header & 0xef880) == 0x2c000)
     return decode_nonp_etttjjjdTTT(ctx, Opcode::O_TNR);
  return 1;
}

// Decode with top bits = %1100
static int decode_idx_0c(nonp_context& ctx)
{
  if ((ctx.header & 0xef880) == 0x20000)
     return decode_nonp_etttjjjdTTT(ctx, Opcode::O_TCC);
  if ((ctx.header & 0xef880) == 0x21000)
     return decode_nonp_etttjjjdTTT(ctx, Opcode::O_TGE);
  if ((ctx.header & 0xef880) == 0x22000)
     return decode_nonp_etttjjjdTTT(ctx, Opcode::O_TNE);
  if ((ctx.header & 0xef880) == 0x23000)
     return decode_nonp_etttjjjdTTT(ctx, Opcode::O_TPL);
  return 1;
}

// Decode with top bits = %1101
static int decode_idx_0d(nonp_context& ctx)
{
  if ((ctx.header & 0xef880) == 0x25000)
     return decode_nonp_etttjjjdTTT(ctx, Opcode::O_TEC);
  if ((ctx.header & 0xef880) == 0x27000)
     return decode_nonp_etttjjjdTTT(ctx, Opcode::O_TGT);
  if ((ctx.header & 0xef880) == 0x26000)
     return decode_nonp_etttjjjdTTT(ctx, Opcode::O_TLC);
  if ((ctx.header & 0xef880) == 0x24000)
     return decode_nonp_etttjjjdTTT(ctx, Opcode::O_TNN);
  return 1;
}

// Decode with top bits = %1110
static int decode_idx_0e(nonp_context& ctx)
{
  if ((ctx.header & 0xef880) == 0x28000)
     return decode_nonp_etttjjjdTTT(ctx, Opcode::O_TCS);
  if ((ctx.header & 0xef880) == 0x2a000)
     return decode_nonp_etttjjjdTTT(ctx, Opcode::O_TEQ);
  if ((ctx.header & 0xef880) == 0x29000)
     return decode_nonp_etttjjjdTTT(ctx, Opcode::O_TLT);
  if ((ctx.header & 0xef880) == 0x2b000)
     return decode_nonp_etttjjjdTTT(ctx, Opcode::O_TMI);
  return 1;
}

// Decode with top bits = %1111
static int decode_idx_0f(nonp_context& ctx)
{
  if ((ctx.header & 0xef880) == 0x2d000)
     return decode_nonp_etttjjjdTTT(ctx, Opcode::O_TES);
  if ((ctx.header & 0xef880) == 0x2f000)
     return decode_nonp_etttjjjdTTT(ctx, Opcode::O_TLE);
  if ((ctx.header & 0xef880) == 0x2e000)
     return decode_nonp_etttjjjdTTT(ctx, Opcode::O_TLS);
  if ((ctx.header & 0xef880) == 0x2c000)
     return decode_nonp_etttjjjdTTT(ctx, Opcode::O_TNR);
  return 1;
}

// Decode with top bits = %10000
static int decode_idx_10(nonp_context& ctx)
{
  return 1;
}

// Decode with top bits = %10001
static int decode_idx_11(nonp_context& ctx)
{
  if ((ctx.header & 0xff8e0) == 0x440a0)
     return decode_nonp_eeeddddd(ctx, Opcode::O_MOVEC);
  if ((ctx.header & 0xff8e0) == 0x448a0)
     return decode_nonp_eeeddddd(ctx, Opcode::O_MOVEC);
  if ((ctx.header & 0xff8e0) == 0x450a0)
     return decode_nonp_eeeddddd(ctx, Opcode::O_MOVEC);
  if ((ctx.header & 0xff8e0) == 0x458a0)
     return decode_nonp_eeeddddd(ctx, Opcode::O_MOVEC);
  if ((ctx.header & 0xff8e0) == 0x460a0)
     return decode_nonp_eeeddddd(ctx, Opcode::O_MOVEC);
  if ((ctx.header & 0xff8e0) == 0x478a0)
     return decode_nonp_eeeddddd(ctx, Opcode::O_MOVEC);
  if ((ctx.header & 0xfe0f0) == 0x44010)
     return decode_nonp_mmrrrdddd(ctx, Opcode::O_LUA);
  return 1;
}

// Decode with top bits = %10010
static int decode_idx_12(nonp_context& ctx)
{
  return 1;
}

// Decode with top bits = %10011
static int decode_idx_13(nonp_context& ctx)
{
  if ((ctx.header & 0xff8e0) == 0x4c0a0)
     return decode_nonp_eeeddddd(ctx, Opcode::O_MOVEC);
  if ((ctx.header & 0xff8e0) == 0x4c8a0)
     return decode_nonp_eeeddddd(ctx, Opcode::O_MOVEC);
  if ((ctx.header & 0xff8e0) == 0x4d0a0)
     return decode_nonp_eeeddddd(ctx, Opcode::O_MOVEC);
  if ((ctx.header & 0xff8e0) == 0x4d8a0)
     return decode_nonp_eeeddddd(ctx, Opcode::O_MOVEC);
  if ((ctx.header & 0xff8e0) == 0x4e0a0)
     return decode_nonp_eeeddddd(ctx, Opcode::O_MOVEC);
  if ((ctx.header & 0xff8e0) == 0x4f8a0)
     return decode_nonp_eeeddddd(ctx, Opcode::O_MOVEC);
  return 1;
}

// Decode with top bits = %10100
static int decode_idx_14(nonp_context& ctx)
{
  if ((ctx.header & 0xfc0a0) == 0x50020)
     return decode_nonp_aaaaaasddddd(ctx, Opcode::O_MOVEC);
  if ((ctx.header & 0xf00e0) == 0x500a0)
     return decode_nonp_iiiiiiiiddddd(ctx, Opcode::O_MOVEC);
  return 1;
}

// Decode with top bits = %10101
static int decode_idx_15(nonp_context& ctx)
{
  if ((ctx.header & 0xfc0a0) == 0x54020)
     return decode_nonp_mmmrrrsddddd(ctx, Opcode::O_MOVEC);
  if ((ctx.header & 0xf00e0) == 0x500a0)
     return decode_nonp_iiiiiiiiddddd(ctx, Opcode::O_MOVEC);
  return 1;
}

// Decode with top bits = %10110
static int decode_idx_16(nonp_context& ctx)
{
  if ((ctx.header & 0xfc0a0) == 0x58020)
     return decode_nonp_aaaaaasddddd(ctx, Opcode::O_MOVEC);
  if ((ctx.header & 0xf00e0) == 0x500a0)
     return decode_nonp_iiiiiiiiddddd(ctx, Opcode::O_MOVEC);
  return 1;
}

// Decode with top bits = %10111
static int decode_idx_17(nonp_context& ctx)
{
  if ((ctx.header & 0xfffe0) == 0x5f420)
     return decode_nonp_ddddd(ctx, Opcode::O_MOVEC);
  if ((ctx.header & 0xfc0a0) == 0x5c020)
     return decode_nonp_mmmrrrsddddd(ctx, Opcode::O_MOVEC);
  if ((ctx.header & 0xf00e0) == 0x500a0)
     return decode_nonp_iiiiiiiiddddd(ctx, Opcode::O_MOVEC);
  return 1;
}

// Decode with top bits = %11000
static int decode_idx_18(nonp_context& ctx)
{
  if ((ctx.header & 0xfc0bf) == 0x60000)
     return decode_nonp_aaaaaas(ctx, Opcode::O_DO);
  if ((ctx.header & 0xfc0bf) == 0x60020)
     return decode_nonp_aaaaaas(ctx, Opcode::O_REP);
  if ((ctx.header & 0xf00f0) == 0x60080)
     return decode_nonp_iiiiiiiihhhh(ctx, Opcode::O_DO);
  if ((ctx.header & 0xf00f0) == 0x600a0)
     return decode_nonp_iiiiiiiihhhh(ctx, Opcode::O_REP);
  return 1;
}

// Decode with top bits = %11001
static int decode_idx_19(nonp_context& ctx)
{
  if ((ctx.header & 0xfc0bf) == 0x64000)
     return decode_nonp_mmmrrrs(ctx, Opcode::O_DO);
  if ((ctx.header & 0xfc0bf) == 0x64020)
     return decode_nonp_mmmrrrs(ctx, Opcode::O_REP);
  if ((ctx.header & 0xf00f0) == 0x60080)
     return decode_nonp_iiiiiiiihhhh(ctx, Opcode::O_DO);
  if ((ctx.header & 0xf00f0) == 0x600a0)
     return decode_nonp_iiiiiiiihhhh(ctx, Opcode::O_REP);
  return 1;
}

// Decode with top bits = %11010
static int decode_idx_1a(nonp_context& ctx)
{
  if ((ctx.header & 0xf00f0) == 0x60080)
     return decode_nonp_iiiiiiiihhhh(ctx, Opcode::O_DO);
  if ((ctx.header & 0xf00f0) == 0x600a0)
     return decode_nonp_iiiiiiiihhhh(ctx, Opcode::O_REP);
  return 1;
}

// Decode with top bits = %11011
static int decode_idx_1b(nonp_context& ctx)
{
  if ((ctx.header & 0xff8ff) == 0x6c000)
     return decode_nonp_ddd(ctx, Opcode::O_DO);
  if ((ctx.header & 0xff8ff) == 0x6c800)
     return decode_nonp_ddd(ctx, Opcode::O_DO);
  if ((ctx.header & 0xff8ff) == 0x6d000)
     return decode_nonp_ddd(ctx, Opcode::O_DO);
  if ((ctx.header & 0xff8ff) == 0x6d800)
     return decode_nonp_ddd(ctx, Opcode::O_DO);
  if ((ctx.header & 0xff8ff) == 0x6e000)
     return decode_nonp_ddd(ctx, Opcode::O_DO);
  if ((ctx.header & 0xff8ff) == 0x6f800)
     return decode_nonp_ddd(ctx, Opcode::O_DO);
  if ((ctx.header & 0xff8ff) == 0x6c020)
     return decode_nonp_ddd(ctx, Opcode::O_REP);
  if ((ctx.header & 0xff8ff) == 0x6c820)
     return decode_nonp_ddd(ctx, Opcode::O_REP);
  if ((ctx.header & 0xff8ff) == 0x6d020)
     return decode_nonp_ddd(ctx, Opcode::O_REP);
  if ((ctx.header & 0xff8ff) == 0x6d820)
     return decode_nonp_ddd(ctx, Opcode::O_REP);
  if ((ctx.header & 0xff8ff) == 0x6e020)
     return decode_nonp_ddd(ctx, Opcode::O_REP);
  if ((ctx.header & 0xff8ff) == 0x6f820)
     return decode_nonp_ddd(ctx, Opcode::O_REP);
  if ((ctx.header & 0xf00f0) == 0x60080)
     return decode_nonp_iiiiiiiihhhh(ctx, Opcode::O_DO);
  if ((ctx.header & 0xf00f0) == 0x600a0)
     return decode_nonp_iiiiiiiihhhh(ctx, Opcode::O_REP);
  return 1;
}

// Decode with top bits = %11100
static int decode_idx_1c(nonp_context& ctx)
{
  if ((ctx.header & 0xfc0f8) == 0x70000)
     return decode_nonp_aaaaaaddd(ctx, Opcode::O_MOVEM);
  if ((ctx.header & 0xfc0f8) == 0x70008)
     return decode_nonp_aaaaaaddd(ctx, Opcode::O_MOVEM);
  if ((ctx.header & 0xfc0f8) == 0x70010)
     return decode_nonp_aaaaaaddd(ctx, Opcode::O_MOVEM);
  if ((ctx.header & 0xfc0f8) == 0x70018)
     return decode_nonp_aaaaaaddd(ctx, Opcode::O_MOVEM);
  if ((ctx.header & 0xfc0f8) == 0x70020)
     return decode_nonp_aaaaaaddd(ctx, Opcode::O_MOVEM);
  if ((ctx.header & 0xfc0f8) == 0x70038)
     return decode_nonp_aaaaaaddd(ctx, Opcode::O_MOVEM);
  return 1;
}

// Decode with top bits = %11101
static int decode_idx_1d(nonp_context& ctx)
{
  if ((ctx.header & 0xfc0f8) == 0x74080)
     return decode_nonp_mmmrrrddd(ctx, Opcode::O_MOVEM);
  if ((ctx.header & 0xfc0f8) == 0x74088)
     return decode_nonp_mmmrrrddd(ctx, Opcode::O_MOVEM);
  if ((ctx.header & 0xfc0f8) == 0x74090)
     return decode_nonp_mmmrrrddd(ctx, Opcode::O_MOVEM);
  if ((ctx.header & 0xfc0f8) == 0x74098)
     return decode_nonp_mmmrrrddd(ctx, Opcode::O_MOVEM);
  if ((ctx.header & 0xfc0f8) == 0x740a0)
     return decode_nonp_mmmrrrddd(ctx, Opcode::O_MOVEM);
  if ((ctx.header & 0xfc0f8) == 0x740b8)
     return decode_nonp_mmmrrrddd(ctx, Opcode::O_MOVEM);
  return 1;
}

// Decode with top bits = %11110
static int decode_idx_1e(nonp_context& ctx)
{
  if ((ctx.header & 0xfc0f8) == 0x78000)
     return decode_nonp_aaaaaaddd(ctx, Opcode::O_MOVEM);
  if ((ctx.header & 0xfc0f8) == 0x78008)
     return decode_nonp_aaaaaaddd(ctx, Opcode::O_MOVEM);
  if ((ctx.header & 0xfc0f8) == 0x78010)
     return decode_nonp_aaaaaaddd(ctx, Opcode::O_MOVEM);
  if ((ctx.header & 0xfc0f8) == 0x78018)
     return decode_nonp_aaaaaaddd(ctx, Opcode::O_MOVEM);
  if ((ctx.header & 0xfc0f8) == 0x78020)
     return decode_nonp_aaaaaaddd(ctx, Opcode::O_MOVEM);
  if ((ctx.header & 0xfc0f8) == 0x78038)
     return decode_nonp_aaaaaaddd(ctx, Opcode::O_MOVEM);
  return 1;
}

// Decode with top bits = %11111
static int decode_idx_1f(nonp_context& ctx)
{
  if ((ctx.header & 0xfc0f8) == 0x7c080)
     return decode_nonp_mmmrrrddd(ctx, Opcode::O_MOVEM);
  if ((ctx.header & 0xfc0f8) == 0x7c088)
     return decode_nonp_mmmrrrddd(ctx, Opcode::O_MOVEM);
  if ((ctx.header & 0xfc0f8) == 0x7c090)
     return decode_nonp_mmmrrrddd(ctx, Opcode::O_MOVEM);
  if ((ctx.header & 0xfc0f8) == 0x7c098)
     return decode_nonp_mmmrrrddd(ctx, Opcode::O_MOVEM);
  if ((ctx.header & 0xfc0f8) == 0x7c0a0)
     return decode_nonp_mmmrrrddd(ctx, Opcode::O_MOVEM);
  if ((ctx.header & 0xfc0f8) == 0x7c0b8)
     return decode_nonp_mmmrrrddd(ctx, Opcode::O_MOVEM);
  return 1;
}

// Decode with top bits = %100000
static int decode_idx_20(nonp_context& ctx)
{
  if ((ctx.header & 0xec000) == 0x80000)
     return decode_nonp_PPPPPPPPPPPPPPP(ctx, Opcode::O_MOVE);
  return 1;
}

// Decode with top bits = %100001
static int decode_idx_21(nonp_context& ctx)
{
  if ((ctx.header & 0xe40c0) == 0x84040)
     return decode_nonp_swmmmrrrpppppp(ctx, Opcode::O_MOVEP);
  if ((ctx.header & 0xe40c0) == 0x84000)
     return decode_nonp_swddddddpppppp(ctx, Opcode::O_MOVEP);
  if ((ctx.header & 0xe4080) == 0x84080)
     return decode_nonp_swmmmrrrspppppp(ctx, Opcode::O_MOVEP);
  return 1;
}

// Decode with top bits = %100010
static int decode_idx_22(nonp_context& ctx)
{
  if ((ctx.header & 0xec000) == 0x88000)
     return decode_nonp_PPPPPPPPPPPPPPP(ctx, Opcode::O_MOVE);
  return 1;
}

// Decode with top bits = %100011
static int decode_idx_23(nonp_context& ctx)
{
  if ((ctx.header & 0xe40c0) == 0x84040)
     return decode_nonp_swmmmrrrpppppp(ctx, Opcode::O_MOVEP);
  if ((ctx.header & 0xe40c0) == 0x84000)
     return decode_nonp_swddddddpppppp(ctx, Opcode::O_MOVEP);
  if ((ctx.header & 0xe4080) == 0x84080)
     return decode_nonp_swmmmrrrspppppp(ctx, Opcode::O_MOVEP);
  return 1;
}

// Decode with top bits = %100100
static int decode_idx_24(nonp_context& ctx)
{
  if ((ctx.header & 0xec000) == 0x80000)
     return decode_nonp_PPPPPPPPPPPPPPP(ctx, Opcode::O_MOVE);
  return 1;
}

// Decode with top bits = %100101
static int decode_idx_25(nonp_context& ctx)
{
  if ((ctx.header & 0xe40c0) == 0x84040)
     return decode_nonp_swmmmrrrpppppp(ctx, Opcode::O_MOVEP);
  if ((ctx.header & 0xe40c0) == 0x84000)
     return decode_nonp_swddddddpppppp(ctx, Opcode::O_MOVEP);
  if ((ctx.header & 0xe4080) == 0x84080)
     return decode_nonp_swmmmrrrspppppp(ctx, Opcode::O_MOVEP);
  return 1;
}

// Decode with top bits = %100110
static int decode_idx_26(nonp_context& ctx)
{
  if ((ctx.header & 0xec000) == 0x88000)
     return decode_nonp_PPPPPPPPPPPPPPP(ctx, Opcode::O_MOVE);
  return 1;
}

// Decode with top bits = %100111
static int decode_idx_27(nonp_context& ctx)
{
  if ((ctx.header & 0xe40c0) == 0x84040)
     return decode_nonp_swmmmrrrpppppp(ctx, Opcode::O_MOVEP);
  if ((ctx.header & 0xe40c0) == 0x84000)
     return decode_nonp_swddddddpppppp(ctx, Opcode::O_MOVEP);
  if ((ctx.header & 0xe4080) == 0x84080)
     return decode_nonp_swmmmrrrspppppp(ctx, Opcode::O_MOVEP);
  return 1;
}

// Decode with top bits = %101000
static int decode_idx_28(nonp_context& ctx)
{
  if ((ctx.header & 0xfc0a0) == 0xa0000)
     return decode_nonp_aaaaaasbbbbb(ctx, Opcode::O_BCLR);
  if ((ctx.header & 0xfc0a0) == 0xa0020)
     return decode_nonp_aaaaaasbbbbb(ctx, Opcode::O_BSET);
  if ((ctx.header & 0xfc0a0) == 0xa00a0)
     return decode_nonp_aaaaaasbbbbb(ctx, Opcode::O_JSET);
  if ((ctx.header & 0xfc0a0) == 0xa0080)
     return decode_nonp_aaaaaasbbbbb(ctx, Opcode::O_JCLR);
  return 1;
}

// Decode with top bits = %101001
static int decode_idx_29(nonp_context& ctx)
{
  if ((ctx.header & 0xfc0a0) == 0xa4000)
     return decode_nonp_mmmrrrsbbbbb(ctx, Opcode::O_BCLR);
  if ((ctx.header & 0xfc0a0) == 0xa4020)
     return decode_nonp_mmmrrrsbbbbb(ctx, Opcode::O_BSET);
  if ((ctx.header & 0xfc0a0) == 0xa40a0)
     return decode_nonp_mmmrrrsbbbbb(ctx, Opcode::O_JSET);
  if ((ctx.header & 0xfc0a0) == 0xa4080)
     return decode_nonp_mmmrrrsbbbbb(ctx, Opcode::O_JCLR);
  return 1;
}

// Decode with top bits = %101010
static int decode_idx_2a(nonp_context& ctx)
{
  if ((ctx.header & 0xfc0a0) == 0xa8000)
     return decode_nonp_ppppppsbbbbb(ctx, Opcode::O_BCLR);
  if ((ctx.header & 0xfc0a0) == 0xa8020)
     return decode_nonp_ppppppsbbbbb(ctx, Opcode::O_BSET);
  if ((ctx.header & 0xfc0a0) == 0xa80a0)
     return decode_nonp_ppppppsbbbbb(ctx, Opcode::O_JSET);
  if ((ctx.header & 0xfc0a0) == 0xa8080)
     return decode_nonp_ppppppsbbbbb(ctx, Opcode::O_JCLR);
  return 1;
}

// Decode with top bits = %101011
static int decode_idx_2b(nonp_context& ctx)
{
  if ((ctx.header & 0xfc0ff) == 0xac0a0)
     return decode_nonp_mmmrrr(ctx, Opcode::O_JCC);
  if ((ctx.header & 0xfc0ff) == 0xac0a8)
     return decode_nonp_mmmrrr(ctx, Opcode::O_JCS);
  if ((ctx.header & 0xfc0ff) == 0xac0a5)
     return decode_nonp_mmmrrr(ctx, Opcode::O_JEC);
  if ((ctx.header & 0xfc0ff) == 0xac0aa)
     return decode_nonp_mmmrrr(ctx, Opcode::O_JEQ);
  if ((ctx.header & 0xfc0ff) == 0xac0ad)
     return decode_nonp_mmmrrr(ctx, Opcode::O_JES);
  if ((ctx.header & 0xfc0ff) == 0xac0a1)
     return decode_nonp_mmmrrr(ctx, Opcode::O_JGE);
  if ((ctx.header & 0xfc0ff) == 0xac0a7)
     return decode_nonp_mmmrrr(ctx, Opcode::O_JGT);
  if ((ctx.header & 0xfc0ff) == 0xac0a6)
     return decode_nonp_mmmrrr(ctx, Opcode::O_JLC);
  if ((ctx.header & 0xfc0ff) == 0xac0af)
     return decode_nonp_mmmrrr(ctx, Opcode::O_JLE);
  if ((ctx.header & 0xfc0ff) == 0xac0ae)
     return decode_nonp_mmmrrr(ctx, Opcode::O_JLS);
  if ((ctx.header & 0xfc0ff) == 0xac0a9)
     return decode_nonp_mmmrrr(ctx, Opcode::O_JLT);
  if ((ctx.header & 0xfc0ff) == 0xac0ab)
     return decode_nonp_mmmrrr(ctx, Opcode::O_JMI);
  if ((ctx.header & 0xfc0ff) == 0xac0a2)
     return decode_nonp_mmmrrr(ctx, Opcode::O_JNE);
  if ((ctx.header & 0xfc0ff) == 0xac0ac)
     return decode_nonp_mmmrrr(ctx, Opcode::O_JNR);
  if ((ctx.header & 0xfc0ff) == 0xac0a3)
     return decode_nonp_mmmrrr(ctx, Opcode::O_JPL);
  if ((ctx.header & 0xfc0ff) == 0xac0a4)
     return decode_nonp_mmmrrr(ctx, Opcode::O_JNN);
  if ((ctx.header & 0xfc0ff) == 0xac080)
     return decode_nonp_mmmrrr(ctx, Opcode::O_JMP);
  if ((ctx.header & 0xffce0) == 0xac440)
     return decode_nonp_ddbbbbb(ctx, Opcode::O_BCLR);
  if ((ctx.header & 0xffce0) == 0xac460)
     return decode_nonp_ddbbbbb(ctx, Opcode::O_BSET);
  if ((ctx.header & 0xffce0) == 0xac420)
     return decode_nonp_ddbbbbb(ctx, Opcode::O_JSET);
  if ((ctx.header & 0xffce0) == 0xac400)
     return decode_nonp_ddbbbbb(ctx, Opcode::O_JCLR);
  if ((ctx.header & 0xff8e0) == 0xac840)
     return decode_nonp_dddbbbbb(ctx, Opcode::O_BCLR);
  if ((ctx.header & 0xff8e0) == 0xad040)
     return decode_nonp_dddbbbbb(ctx, Opcode::O_BCLR);
  if ((ctx.header & 0xff8e0) == 0xad840)
     return decode_nonp_dddbbbbb(ctx, Opcode::O_BCLR);
  if ((ctx.header & 0xff8e0) == 0xae040)
     return decode_nonp_dddbbbbb(ctx, Opcode::O_BCLR);
  if ((ctx.header & 0xff8e0) == 0xaf840)
     return decode_nonp_dddbbbbb(ctx, Opcode::O_BCLR);
  if ((ctx.header & 0xff8e0) == 0xac860)
     return decode_nonp_dddbbbbb(ctx, Opcode::O_BSET);
  if ((ctx.header & 0xff8e0) == 0xad060)
     return decode_nonp_dddbbbbb(ctx, Opcode::O_BSET);
  if ((ctx.header & 0xff8e0) == 0xad860)
     return decode_nonp_dddbbbbb(ctx, Opcode::O_BSET);
  if ((ctx.header & 0xff8e0) == 0xae060)
     return decode_nonp_dddbbbbb(ctx, Opcode::O_BSET);
  if ((ctx.header & 0xff8e0) == 0xaf860)
     return decode_nonp_dddbbbbb(ctx, Opcode::O_BSET);
  if ((ctx.header & 0xff8e0) == 0xac820)
     return decode_nonp_dddbbbbb(ctx, Opcode::O_JSET);
  if ((ctx.header & 0xff8e0) == 0xad020)
     return decode_nonp_dddbbbbb(ctx, Opcode::O_JSET);
  if ((ctx.header & 0xff8e0) == 0xad820)
     return decode_nonp_dddbbbbb(ctx, Opcode::O_JSET);
  if ((ctx.header & 0xff8e0) == 0xae020)
     return decode_nonp_dddbbbbb(ctx, Opcode::O_JSET);
  if ((ctx.header & 0xff8e0) == 0xae820)
     return decode_nonp_dddbbbbb(ctx, Opcode::O_JSET);
  if ((ctx.header & 0xff8e0) == 0xac800)
     return decode_nonp_dddbbbbb(ctx, Opcode::O_JCLR);
  if ((ctx.header & 0xff8e0) == 0xad000)
     return decode_nonp_dddbbbbb(ctx, Opcode::O_JCLR);
  if ((ctx.header & 0xff8e0) == 0xad800)
     return decode_nonp_dddbbbbb(ctx, Opcode::O_JCLR);
  if ((ctx.header & 0xff8e0) == 0xae000)
     return decode_nonp_dddbbbbb(ctx, Opcode::O_JCLR);
  if ((ctx.header & 0xff8e0) == 0xae800)
     return decode_nonp_dddbbbbb(ctx, Opcode::O_JCLR);
  return 1;
}

// Decode with top bits = %101100
static int decode_idx_2c(nonp_context& ctx)
{
  if ((ctx.header & 0xfc0a0) == 0xb0000)
     return decode_nonp_aaaaaasbbbbb(ctx, Opcode::O_BCHG);
  if ((ctx.header & 0xfc0a0) == 0xb0020)
     return decode_nonp_aaaaaasbbbbb(ctx, Opcode::O_BTST);
  if ((ctx.header & 0xfc0a0) == 0xb0080)
     return decode_nonp_aaaaaasbbbbb(ctx, Opcode::O_JSCLR);
  if ((ctx.header & 0xfc0a0) == 0xb00a0)
     return decode_nonp_aaaaaasbbbbb(ctx, Opcode::O_JSSET);
  return 1;
}

// Decode with top bits = %101101
static int decode_idx_2d(nonp_context& ctx)
{
  if ((ctx.header & 0xfc0a0) == 0xb4000)
     return decode_nonp_mmmrrrsbbbbb(ctx, Opcode::O_BCHG);
  if ((ctx.header & 0xfc0a0) == 0xb4020)
     return decode_nonp_mmmrrrsbbbbb(ctx, Opcode::O_BTST);
  if ((ctx.header & 0xfc0a0) == 0xb4080)
     return decode_nonp_mmmrrrsbbbbb(ctx, Opcode::O_JSCLR);
  if ((ctx.header & 0xfc0a0) == 0xb40a0)
     return decode_nonp_mmmrrrsbbbbb(ctx, Opcode::O_JSSET);
  return 1;
}

// Decode with top bits = %101110
static int decode_idx_2e(nonp_context& ctx)
{
  if ((ctx.header & 0xfc0a0) == 0xb8000)
     return decode_nonp_ppppppsbbbbb(ctx, Opcode::O_BCHG);
  if ((ctx.header & 0xfc0a0) == 0xb8020)
     return decode_nonp_ppppppsbbbbb(ctx, Opcode::O_BTST);
  if ((ctx.header & 0xfc0a0) == 0xb8080)
     return decode_nonp_ppppppsbbbbb(ctx, Opcode::O_JSCLR);
  if ((ctx.header & 0xfc0a0) == 0xb80a0)
     return decode_nonp_ppppppsbbbbb(ctx, Opcode::O_JSSET);
  return 1;
}

// Decode with top bits = %101111
static int decode_idx_2f(nonp_context& ctx)
{
  if ((ctx.header & 0xfc0ff) == 0xbc0a0)
     return decode_nonp_mmmrrr(ctx, Opcode::O_JSCC);
  if ((ctx.header & 0xfc0ff) == 0xbc0a8)
     return decode_nonp_mmmrrr(ctx, Opcode::O_JSCS);
  if ((ctx.header & 0xfc0ff) == 0xbc0a5)
     return decode_nonp_mmmrrr(ctx, Opcode::O_JSEC);
  if ((ctx.header & 0xfc0ff) == 0xbc0aa)
     return decode_nonp_mmmrrr(ctx, Opcode::O_JSEQ);
  if ((ctx.header & 0xfc0ff) == 0xbc0ad)
     return decode_nonp_mmmrrr(ctx, Opcode::O_JSES);
  if ((ctx.header & 0xfc0ff) == 0xbc0a1)
     return decode_nonp_mmmrrr(ctx, Opcode::O_JSGE);
  if ((ctx.header & 0xfc0ff) == 0xbc0a7)
     return decode_nonp_mmmrrr(ctx, Opcode::O_JSGT);
  if ((ctx.header & 0xfc0ff) == 0xbc0a6)
     return decode_nonp_mmmrrr(ctx, Opcode::O_JSLC);
  if ((ctx.header & 0xfc0ff) == 0xbc0af)
     return decode_nonp_mmmrrr(ctx, Opcode::O_JSLE);
  if ((ctx.header & 0xfc0ff) == 0xbc0ae)
     return decode_nonp_mmmrrr(ctx, Opcode::O_JSLS);
  if ((ctx.header & 0xfc0ff) == 0xbc0a9)
     return decode_nonp_mmmrrr(ctx, Opcode::O_JSLT);
  if ((ctx.header & 0xfc0ff) == 0xbc0ab)
     return decode_nonp_mmmrrr(ctx, Opcode::O_JSMI);
  if ((ctx.header & 0xfc0ff) == 0xbc0a2)
     return decode_nonp_mmmrrr(ctx, Opcode::O_JSNE);
  if ((ctx.header & 0xfc0ff) == 0xbc0ac)
     return decode_nonp_mmmrrr(ctx, Opcode::O_JSNR);
  if ((ctx.header & 0xfc0ff) == 0xbc0a3)
     return decode_nonp_mmmrrr(ctx, Opcode::O_JSPL);
  if ((ctx.header & 0xfc0ff) == 0xbc0a4)
     return decode_nonp_mmmrrr(ctx, Opcode::O_JSNN);
  if ((ctx.header & 0xfc0ff) == 0xbc080)
     return decode_nonp_mmmrrr(ctx, Opcode::O_JSR);
  if ((ctx.header & 0xffce0) == 0xbc440)
     return decode_nonp_ddbbbbb(ctx, Opcode::O_BCHG);
  if ((ctx.header & 0xffce0) == 0xbc460)
     return decode_nonp_ddbbbbb(ctx, Opcode::O_BTST);
  if ((ctx.header & 0xffce0) == 0xbc400)
     return decode_nonp_ddbbbbb(ctx, Opcode::O_JSCLR);
  if ((ctx.header & 0xffce0) == 0xbc420)
     return decode_nonp_ddbbbbb(ctx, Opcode::O_JSSET);
  if ((ctx.header & 0xff8e0) == 0xbc840)
     return decode_nonp_dddbbbbb(ctx, Opcode::O_BCHG);
  if ((ctx.header & 0xff8e0) == 0xbd040)
     return decode_nonp_dddbbbbb(ctx, Opcode::O_BCHG);
  if ((ctx.header & 0xff8e0) == 0xbd840)
     return decode_nonp_dddbbbbb(ctx, Opcode::O_BCHG);
  if ((ctx.header & 0xff8e0) == 0xbe040)
     return decode_nonp_dddbbbbb(ctx, Opcode::O_BCHG);
  if ((ctx.header & 0xff8e0) == 0xbf840)
     return decode_nonp_dddbbbbb(ctx, Opcode::O_BCHG);
  if ((ctx.header & 0xff8e0) == 0xbc860)
     return decode_nonp_dddbbbbb(ctx, Opcode::O_BTST);
  if ((ctx.header & 0xff8e0) == 0xbd060)
     return decode_nonp_dddbbbbb(ctx, Opcode::O_BTST);
  if ((ctx.header & 0xff8e0) == 0xbd860)
     return decode_nonp_dddbbbbb(ctx, Opcode::O_BTST);
  if ((ctx.header & 0xff8e0) == 0xbe060)
     return decode_nonp_dddbbbbb(ctx, Opcode::O_BTST);
  if ((ctx.header & 0xff8e0) == 0xbf860)
     return decode_nonp_dddbbbbb(ctx, Opcode::O_BTST);
  if ((ctx.header & 0xff8e0) == 0xbc800)
     return decode_nonp_dddbbbbb(ctx, Opcode::O_JSCLR);
  if ((ctx.header & 0xff8e0) == 0xbd000)
     return decode_nonp_dddbbbbb(ctx, Opcode::O_JSCLR);
  if ((ctx.header & 0xff8e0) == 0xbd800)
     return decode_nonp_dddbbbbb(ctx, Opcode::O_JSCLR);
  if ((ctx.header & 0xff8e0) == 0xbe000)
     return decode_nonp_dddbbbbb(ctx, Opcode::O_JSCLR);
  if ((ctx.header & 0xff8e0) == 0xbe800)
     return decode_nonp_dddbbbbb(ctx, Opcode::O_JSCLR);
  if ((ctx.header & 0xff8e0) == 0xbc820)
     return decode_nonp_dddbbbbb(ctx, Opcode::O_JSSET);
  if ((ctx.header & 0xff8e0) == 0xbd020)
     return decode_nonp_dddbbbbb(ctx, Opcode::O_JSSET);
  if ((ctx.header & 0xff8e0) == 0xbd820)
     return decode_nonp_dddbbbbb(ctx, Opcode::O_JSSET);
  if ((ctx.header & 0xff8e0) == 0xbe020)
     return decode_nonp_dddbbbbb(ctx, Opcode::O_JSSET);
  if ((ctx.header & 0xff8e0) == 0xbe820)
     return decode_nonp_dddbbbbb(ctx, Opcode::O_JSSET);
  return 1;
}

// Decode with top bits = %110000
static int decode_idx_30(nonp_context& ctx)
{
  if ((ctx.header & 0xff000) == 0xc0000)
     return decode_nonp_aaaaaaaaaaaa(ctx, Opcode::O_JMP);
  return 1;
}

// Decode with top bits = %110001
static int decode_idx_31(nonp_context& ctx)
{
  return 1;
}

// Decode with top bits = %110010
static int decode_idx_32(nonp_context& ctx)
{
  return 1;
}

// Decode with top bits = %110011
static int decode_idx_33(nonp_context& ctx)
{
  return 1;
}

// Decode with top bits = %110100
static int decode_idx_34(nonp_context& ctx)
{
  if ((ctx.header & 0xff000) == 0xd0000)
     return decode_nonp_aaaaaaaaaaaa(ctx, Opcode::O_JSR);
  return 1;
}

// Decode with top bits = %110101
static int decode_idx_35(nonp_context& ctx)
{
  return 1;
}

// Decode with top bits = %110110
static int decode_idx_36(nonp_context& ctx)
{
  return 1;
}

// Decode with top bits = %110111
static int decode_idx_37(nonp_context& ctx)
{
  return 1;
}

// Decode with top bits = %111000
static int decode_idx_38(nonp_context& ctx)
{
  if ((ctx.header & 0xff000) == 0xe0000)
     return decode_nonp_aaaaaaaaaaaa(ctx, Opcode::O_JCC);
  if ((ctx.header & 0xff000) == 0xe1000)
     return decode_nonp_aaaaaaaaaaaa(ctx, Opcode::O_JGE);
  if ((ctx.header & 0xff000) == 0xe2000)
     return decode_nonp_aaaaaaaaaaaa(ctx, Opcode::O_JNE);
  if ((ctx.header & 0xff000) == 0xe3000)
     return decode_nonp_aaaaaaaaaaaa(ctx, Opcode::O_JPL);
  return 1;
}

// Decode with top bits = %111001
static int decode_idx_39(nonp_context& ctx)
{
  if ((ctx.header & 0xff000) == 0xe5000)
     return decode_nonp_aaaaaaaaaaaa(ctx, Opcode::O_JEC);
  if ((ctx.header & 0xff000) == 0xe7000)
     return decode_nonp_aaaaaaaaaaaa(ctx, Opcode::O_JGT);
  if ((ctx.header & 0xff000) == 0xe6000)
     return decode_nonp_aaaaaaaaaaaa(ctx, Opcode::O_JLC);
  if ((ctx.header & 0xff000) == 0xe4000)
     return decode_nonp_aaaaaaaaaaaa(ctx, Opcode::O_JNN);
  return 1;
}

// Decode with top bits = %111010
static int decode_idx_3a(nonp_context& ctx)
{
  if ((ctx.header & 0xff000) == 0xe8000)
     return decode_nonp_aaaaaaaaaaaa(ctx, Opcode::O_JCS);
  if ((ctx.header & 0xff000) == 0xea000)
     return decode_nonp_aaaaaaaaaaaa(ctx, Opcode::O_JEQ);
  if ((ctx.header & 0xff000) == 0xe9000)
     return decode_nonp_aaaaaaaaaaaa(ctx, Opcode::O_JLT);
  if ((ctx.header & 0xff000) == 0xeb000)
     return decode_nonp_aaaaaaaaaaaa(ctx, Opcode::O_JMI);
  return 1;
}

// Decode with top bits = %111011
static int decode_idx_3b(nonp_context& ctx)
{
  if ((ctx.header & 0xff000) == 0xed000)
     return decode_nonp_aaaaaaaaaaaa(ctx, Opcode::O_JES);
  if ((ctx.header & 0xff000) == 0xef000)
     return decode_nonp_aaaaaaaaaaaa(ctx, Opcode::O_JLE);
  if ((ctx.header & 0xff000) == 0xee000)
     return decode_nonp_aaaaaaaaaaaa(ctx, Opcode::O_JLS);
  if ((ctx.header & 0xff000) == 0xec000)
     return decode_nonp_aaaaaaaaaaaa(ctx, Opcode::O_JNR);
  return 1;
}

// Decode with top bits = %111100
static int decode_idx_3c(nonp_context& ctx)
{
  if ((ctx.header & 0xff000) == 0xf0000)
     return decode_nonp_aaaaaaaaaaaa(ctx, Opcode::O_JSCC);
  if ((ctx.header & 0xff000) == 0xf1000)
     return decode_nonp_aaaaaaaaaaaa(ctx, Opcode::O_JSGE);
  if ((ctx.header & 0xff000) == 0xf2000)
     return decode_nonp_aaaaaaaaaaaa(ctx, Opcode::O_JSNE);
  if ((ctx.header & 0xff000) == 0xf3000)
     return decode_nonp_aaaaaaaaaaaa(ctx, Opcode::O_JSPL);
  return 1;
}

// Decode with top bits = %111101
static int decode_idx_3d(nonp_context& ctx)
{
  if ((ctx.header & 0xff000) == 0xf5000)
     return decode_nonp_aaaaaaaaaaaa(ctx, Opcode::O_JSEC);
  if ((ctx.header & 0xff000) == 0xf7000)
     return decode_nonp_aaaaaaaaaaaa(ctx, Opcode::O_JSGT);
  if ((ctx.header & 0xff000) == 0xf6000)
     return decode_nonp_aaaaaaaaaaaa(ctx, Opcode::O_JSLC);
  if ((ctx.header & 0xff000) == 0xf4000)
     return decode_nonp_aaaaaaaaaaaa(ctx, Opcode::O_JSNN);
  return 1;
}

// Decode with top bits = %111110
static int decode_idx_3e(nonp_context& ctx)
{
  if ((ctx.header & 0xff000) == 0xf8000)
     return decode_nonp_aaaaaaaaaaaa(ctx, Opcode::O_JSCS);
  if ((ctx.header & 0xff000) == 0xfa000)
     return decode_nonp_aaaaaaaaaaaa(ctx, Opcode::O_JSEQ);
  if ((ctx.header & 0xff000) == 0xf9000)
     return decode_nonp_aaaaaaaaaaaa(ctx, Opcode::O_JSLT);
  if ((ctx.header & 0xff000) == 0xfb000)
     return decode_nonp_aaaaaaaaaaaa(ctx, Opcode::O_JSMI);
  return 1;
}

// Decode with top bits = %111111
static int decode_idx_3f(nonp_context& ctx)
{
  if ((ctx.header & 0xff000) == 0xfd000)
     return decode_nonp_aaaaaaaaaaaa(ctx, Opcode::O_JSES);
  if ((ctx.header & 0xff000) == 0xff000)
     return decode_nonp_aaaaaaaaaaaa(ctx, Opcode::O_JSLE);
  if ((ctx.header & 0xff000) == 0xfe000)
     return decode_nonp_aaaaaaaaaaaa(ctx, Opcode::O_JSLS);
  if ((ctx.header & 0xff000) == 0xfc000)
     return decode_nonp_aaaaaaaaaaaa(ctx, Opcode::O_JSNR);
  return 1;
}
// Maps top 6 bits of the instruction header to decoder table functions.
static const dsp_decoder g_nonp_tables[64] =
{
	decode_idx_00,
	decode_idx_01,
	decode_idx_02,
	decode_idx_03,
	decode_idx_04,
	decode_idx_05,
	decode_idx_06,
	decode_idx_07,
	decode_idx_08,
	decode_idx_09,
	decode_idx_0a,
	decode_idx_0b,
	decode_idx_0c,
	decode_idx_0d,
	decode_idx_0e,
	decode_idx_0f,
	decode_idx_10,
	decode_idx_11,
	decode_idx_12,
	decode_idx_13,
	decode_idx_14,
	decode_idx_15,
	decode_idx_16,
	decode_idx_17,
	decode_idx_18,
	decode_idx_19,
	decode_idx_1a,
	decode_idx_1b,
	decode_idx_1c,
	decode_idx_1d,
	decode_idx_1e,
	decode_idx_1f,
	decode_idx_20,
	decode_idx_21,
	decode_idx_22,
	decode_idx_23,
	decode_idx_24,
	decode_idx_25,
	decode_idx_26,
	decode_idx_27,
	decode_idx_28,
	decode_idx_29,
	decode_idx_2a,
	decode_idx_2b,
	decode_idx_2c,
	decode_idx_2d,
	decode_idx_2e,
	decode_idx_2f,
	decode_idx_30,
	decode_idx_31,
	decode_idx_32,
	decode_idx_33,
	decode_idx_34,
	decode_idx_35,
	decode_idx_36,
	decode_idx_37,
	decode_idx_38,
	decode_idx_39,
	decode_idx_3a,
	decode_idx_3b,
	decode_idx_3c,
	decode_idx_3d,
	decode_idx_3e,
	decode_idx_3f
};

