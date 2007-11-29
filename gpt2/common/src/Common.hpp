#ifndef GPT_COMMON_HPP
#define GPT_COMMON_HPP

/*
Mapa de bits para o endereco:
31: Nao utilizado :-)
30: Ligado indica endereco local, desligado indica endereco global
29: Ligado indica endereco negativo, desligado indica endereco positivo
28..0: valor do endereco
*/

//const int SET_LOCAL_BIT   = 0x80000000;
//const int UNSET_LOCAL_BIT = 0x3FFFFFFF;

const int SET_LOCAL_BIT   = 0x40000000; // 01000000 (binario)
const int UNSET_LOCAL_BIT = 0xBFFFFFFF; // 10111111 (binario)

const int SET_NEG_BIT     = 0x20000000; // 00100000 (binario)
const int UNSET_NEG_BIT   = 0xDFFFFFFF; // 11011111 (binario)

#define IS_LOCAL_ADDRESS(address) \
   (address) & SET_LOCAL_BIT

#define IS_NEG_ADDRESS(address) \
   (address) & SET_NEG_BIT

enum opcode {
   OP_NOP         = 0,
   OP_HLT         = 1,
   OP_EXIT        = 2,
   OP_ISUM        = 3,
   OP_SSUM        = 4,
   OP_RSUM        = 5,
   OP_ISUB        = 6,
   OP_SSUB        = 7,
   OP_RSUB        = 8,
   OP_IMUL        = 9,
   OP_RMUL        = 10,
   OP_IDIV        = 11,
   OP_RDIV        = 12,
   OP_IMOD        = 13,
   OP_RMOD        = 14,
   OP_IGE         = 15,
   OP_SGE         = 16,
   OP_RGE         = 17,
   OP_ILE         = 18,
   OP_SLE         = 19,
   OP_RLE         = 20,
   OP_INE         = 21,
   OP_SNE         = 22,
   OP_RNE         = 23,
   OP_IGT         = 24,
   OP_SGT         = 25,
   OP_RGT         = 26,
   OP_ILT         = 27,
   OP_SLT         = 28,
   OP_RLT         = 29,
   OP_IEQ         = 30,
   OP_SEQ         = 31,
   OP_REQ         = 32,
   OP_OR          = 33,
   OP_AND         = 34,
   OP_XOR         = 35,
   OP_INEG        = 36,
   OP_RNEG        = 37,
   OP_NOT         = 38,
   OP_IINC        = 39,
   OP_IDEC        = 40,
   OP_I2C         = 41,
   OP_R2C         = 42,
   OP_S2C         = 43,
   OP_B2C         = 44,
   OP_I2R         = 45,
   OP_C2R         = 46,
   OP_S2R         = 47,
   OP_B2R         = 48,
   OP_I2B         = 49,
   OP_C2B         = 50,
   OP_R2B         = 51,
   OP_S2B         = 52,
   OP_I2S         = 53,
   OP_C2S         = 54,
   OP_R2S         = 55,
   OP_B2S         = 56,
   OP_P2S         = 57,
   OP_C2I         = 58,
   OP_R2I         = 59,
   OP_S2I         = 60,
   OP_B2I         = 61,
   OP_ISET        = 62,
   OP_SSET        = 63,
   OP_RSET        = 64,
   OP_GETA        = 65,
   OP_IGETV       = 66,
   OP_SGETV       = 67,
   OP_RGETV       = 68,
   OP_ISETV       = 69,
   OP_SSETV       = 70,
   OP_RSETV       = 71,
   OP_JMP         = 72,
   OP_IF          = 73,
   OP_IFNOT       = 74,
   OP_INCSP       = 77,
   OP_DECSP       = 78,
   OP_PUSHIV      = 79,
   OP_PUSHSV      = 80,
   OP_PUSHRV      = 81,
   OP_PUSHMV      = 84,
   OP_INCSP_4     = 87,
   OP_INCSP_8     = 88,
   OP_DECSP_4     = 89,
   OP_DECSP_8     = 90,
   OP_PCALL       = 91,
   OP_RET         = 92,
   OP_LCALL       = 93,
   OP_SALLOC      = 94,
   OP_SFREE       = 95,
   OP_SSETC       = 96,
   OP_SGETC       = 97,
   OP_M1ALLOC     = 98,
   OP_M2ALLOC     = 99,
   OP_MFREE       = 100,
   OP_M1SET       = 101,
   OP_M1GET       = 102,
   OP_M2SET       = 103,
   OP_M2GET       = 104,
   OP_MCOPY       = 105,
   OP_MGETSIZE1   = 106,
   OP_MGETSIZE2   = 107,
   OP_PUSHIT      = 108,
   OP_PUSHST      = 109,
   OP_PUSHRT      = 110,
   OP_PUSHCT      = 111,
   OP_PUSHBT      = 112,
   OP_PUSHMT      = 113,
   OP_POPIV       = 114,
   OP_POPRV       = 115,
   OP_POPSV       = 116,
   OP_POPMV       = 117,
   OP_PUSH_0      = 118,
   OP_PUSH_1      = 119,
   OP_PUSH_2      = 120,
   OP_PUSH_3      = 121,
   OP_PUSH_4      = 122,
   OP_PUSH_5      = 123,
   OP_EXIT_0      = 124,
   OP_EXIT_1      = 125,
   OPCODE_NUMBER  = 125
};

#endif

