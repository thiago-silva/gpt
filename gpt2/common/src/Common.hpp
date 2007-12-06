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
   ( (address) & SET_LOCAL_BIT )

#define IS_NEG_ADDRESS(address) \
   ( (address) & SET_NEG_BIT )

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
   OP_IGE         = 14,
   OP_SGE         = 15,
   OP_RGE         = 16,
   OP_ILE         = 17,
   OP_SLE         = 18,
   OP_RLE         = 19,
   OP_INE         = 20,
   OP_SNE         = 21,
   OP_RNE         = 22,
   OP_IGT         = 23,
   OP_SGT         = 24,
   OP_RGT         = 25,
   OP_ILT         = 26,
   OP_SLT         = 27,
   OP_RLT         = 28,
   OP_IEQ         = 29,
   OP_SEQ         = 30,
   OP_REQ         = 31,
   OP_OR          = 32,
   OP_AND         = 33,
   OP_XOR         = 34,
   OP_INEG        = 35,
   OP_RNEG        = 36,
   OP_NOT         = 37,
   OP_IINC        = 38,
   OP_IDEC        = 39,
   OP_I2C         = 40,
   OP_R2C         = 41,
   OP_S2C         = 42,
   OP_B2C         = 43,
   OP_I2R         = 44,
   OP_C2R         = 45,
   OP_S2R         = 46,
   OP_B2R         = 47,
   OP_I2B         = 48,
   OP_C2B         = 49,
   OP_R2B         = 50,
   OP_S2B         = 51,
   OP_I2S         = 52,
   OP_C2S         = 53,
   OP_R2S         = 54,
   OP_B2S         = 55,
   OP_P2S         = 56,
   OP_C2I         = 57,
   OP_R2I         = 58,
   OP_S2I         = 59,
   OP_B2I         = 60,
   OP_ISET        = 61,
   OP_SSET        = 62,
   OP_RSET        = 63,
   OP_GETA        = 64,
   OP_IGETV       = 65,
   OP_SGETV       = 66,
   OP_RGETV       = 67,
   OP_ISETV       = 68,
   OP_SSETV       = 69,
   OP_RSETV       = 70,
   OP_JMP         = 71,
   OP_IF          = 72,
   OP_IFNOT       = 73,
   OP_INCSP       = 74,
   OP_DECSP       = 75,
   OP_PUSHIV      = 76,
   OP_PUSHSV      = 77,
   OP_PUSHRV      = 78,
   OP_PUSHDV      = 79,
   OP_PUSHMV      = 80,
   OP_INCSP_4     = 81,
   OP_INCSP_8     = 82,
   OP_DECSP_4     = 83,
   OP_DECSP_8     = 84,
   OP_PCALL       = 85,
   OP_RET         = 86,
   OP_LCALL       = 87,
   OP_SALLOC      = 88,
   OP_SFREE       = 89,
   OP_SSETC       = 90,
   OP_SGETC       = 91,
   OP_M1ALLOC     = 92,
   OP_M2ALLOC     = 93,
   OP_MFREE       = 94,
   OP_M1SET       = 95,
   OP_M1GET       = 96,
   OP_M2SET       = 97,
   OP_M2GET       = 98,
   OP_MCOPY       = 99,
   OP_MGETSIZE1   = 100,
   OP_MGETSIZE2   = 101,
   OP_PUSHIT      = 102,
   OP_PUSHST      = 103,
   OP_PUSHRT      = 104,
   OP_PUSHCT      = 105,
   OP_PUSHBT      = 106,
   OP_PUSHDT      = 107,
   OP_PUSHMT      = 108,
   OP_POPIV       = 109,
   OP_POPRV       = 110,
   OP_POPSV       = 111,
   OP_POPDV       = 112,
   OP_POPMV       = 113,
   OP_PUSH_0      = 114,
   OP_PUSH_1      = 115,
   OP_PUSH_2      = 116,
   OP_PUSH_3      = 117,
   OP_PUSH_4      = 118,
   OP_PUSH_5      = 119,
   OP_EXIT_0      = 120,
   OP_EXIT_1      = 121,
   OP_PUSHIR      = 122,
   OP_PUSHSR      = 123,
   OP_PUSHRR      = 124,
   OP_PUSHDR      = 125,
   OP_PUSHMR      = 126,
   OPCODE_NUMBER  = 127
};

#endif

