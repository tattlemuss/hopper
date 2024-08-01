#ifndef HOPPER_56_OPCODE_H
#define HOPPER_56_OPCODE_H
#include <cstdint>

namespace hop56
{
	enum Opcode
	{
		INVALID,
		O_ABS,
		O_ADC,
		O_ADD,
		O_ADDL,
		O_ADDR,
		O_AND,
		O_ANDI,
		O_ASL,
		O_ASR,
		O_BCHG,
		O_BCLR,
		O_BSET,
		O_BTST,
		O_CLR,
		O_CMP,
		O_CMPM,
		O_DEBUG,
		O_DEBUGCC,
		O_DEBUGCS,
		O_DEBUGEC,
		O_DEBUGEQ,
		O_DEBUGES,
		O_DEBUGGE,
		O_DEBUGGT,
		O_DEBUGLC,
		O_DEBUGLE,
		O_DEBUGLS,
		O_DEBUGLT,
		O_DEBUGMI,
		O_DEBUGNE,
		O_DEBUGNN,
		O_DEBUGNR,
		O_DEBUGPL,
		O_DIV,
		O_DO,
		O_ENDDO,
		O_EOR,
		O_ILLEGAL,
		O_JCC,
		O_JCLR,
		O_JCS,
		O_JEC,
		O_JEQ,
		O_JES,
		O_JGE,
		O_JGT,
		O_JLC,
		O_JLE,
		O_JLS,
		O_JLT,
		O_JMI,
		O_JMP,
		O_JNE,
		O_JNN,
		O_JNR,
		O_JPL,
		O_JSCC,
		O_JSCLR,
		O_JSCS,
		O_JSEC,
		O_JSEQ,
		O_JSES,
		O_JSET,
		O_JSGE,
		O_JSGT,
		O_JSLC,
		O_JSLE,
		O_JSLS,
		O_JSLT,
		O_JSMI,
		O_JSNE,
		O_JSNN,
		O_JSNR,
		O_JSPL,
		O_JSR,
		O_JSSET,
		O_LSL,
		O_LSR,
		O_LUA,
		O_MAC,
		O_MACR,
		O_MOVE,
		O_MOVEC,
		O_MOVEM,
		O_MOVEP,
		O_MPY,
		O_MPYR,
		O_NEG,
		O_NOP,
		O_NORM,
		O_NOT,
		O_OR,
		O_ORI,
		O_REP,
		O_RESET,
		O_RND,
		O_ROL,
		O_ROR,
		O_RTI,
		O_RTS,
		O_SBC,
		O_STOP,
		O_SUB,
		O_SUBL,
		O_SUBR,
		O_SWI,
		O_TCC,
		O_TCS,
		O_TEC,
		O_TEQ,
		O_TES,
		O_TFR,
		O_TGE,
		O_TGT,
		O_TLC,
		O_TLE,
		O_TLS,
		O_TLT,
		O_TMI,
		O_TNE,
		O_TNN,
		O_TNR,
		O_TPL,
		O_TST,
		O_WAIT,
		OPCODE_COUNT
	};

} // namespace

#endif // HOPPER_56_OPCODE_H
