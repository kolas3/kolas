// FWDES.cpp: implementation of the CFWDES class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FWDES.h"

#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// ARRAY TABLE
//////////////////////////////////////////////////////////////////////

//
// INITIAL PERMUTATION
//

static uint32_t IP[64] = {
	58, 50, 42, 34, 26, 18, 10,  2,
	60, 52, 44, 36, 28, 20, 12,  4,
	62, 54, 46, 38, 30, 22, 14,  6,
	64, 56, 48, 40, 32, 24, 16,  8,
	57, 49, 41, 33, 25, 17,  9,  1,
	59, 51, 43, 35, 27, 19, 11,  3,
	61, 53, 45, 37, 29, 21, 13,  5,
	63, 55, 47, 39, 31, 23, 15,  7
};

//
// INVERSE of INITIAL PERMUTATION
//

static uint32_t IIP[64] = {
	40,  8, 48, 16, 56, 24, 64, 32,
	39,  7, 47, 15, 55, 23, 63, 31,
	38,  6, 46, 14, 54, 22, 62, 30,
	37,  5, 45, 13, 53, 21, 61, 29,
	36,  4, 44, 12, 52, 20, 60, 28,
	35,  3, 43, 11, 51, 19, 59, 27,
	34,  2, 42, 10, 50, 18, 58, 26,
	33,  1, 41,  9, 49, 17, 57, 25
};

//
// S-BOX
//

static uint32_t S_box[8][64] = {
	{ // S1
	14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,
	 0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,
	 4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,
	15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13
	},

	{ // S2
	15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,
	 3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,
	 0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15,
	13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9
	},

	{ // S3
	10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,
	13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,
	13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,
	 1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12
	},

	{ // S4
	 7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,
	13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9,
	10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,
	 3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14
	},

	{ // S5
	 2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9,
	14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6,
	 4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14,
	11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3
	},

	{ // S6
	12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,
	10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,
	 9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,
	 4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13
	},

	{ // S7
	 4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1,
	13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,
	 1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,
	 6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12
	},

	{ // S8
	13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,
	 1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,
	 7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,
	 2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11
	}
};

/*
static uint32_t S1_box[64] = {
	14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,
	 0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,
	 4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,
	15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13
};

static uint32_t S2_box[64] = {
	15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,
	 3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,
	 0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15,
	13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9
};

static uint32_t S3_box[64] = {
	10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,
	13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,
	13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,
	 1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12
};

static uint32_t S4_box[64] = {
	 7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,
	13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9,
	10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,
	 3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14
};

static uint32_t S5_box[64] = {
	 2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9,
	14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6,
	 4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14,
	11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3
};

static uint32_t S6_box[64] = {
	12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,
	10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,
	 9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,
	 4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13
};

static uint32_t S7_box[64] = {
	 4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1,
	13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,
	 1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,
	 6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12
};

static uint32_t S8_box[64] = {
	13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,
	 1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,
	 7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,
	 2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11
};
*/

//
// E BIT SELECTION TABLE ( 32bits --> 48bits )
//

static uint32_t E_table[48] = {
	32,  1,  2,  3,  4,  5,
	 4,  5,  6,  7,  8,  9,
	 8,  9, 10, 11, 12, 13,
	12, 13, 14, 15, 16, 17,
	16, 17, 18, 19, 20, 21,
	20, 21, 22, 23, 24, 25,
	24, 25, 26, 27, 28, 29,
	28, 29, 30, 31, 32,  1
};

//
// PERMUTATION FUNCTION
//

static uint32_t P_table[32] = {
	16,  7, 20, 21,
	29, 12, 28, 17,
	 1, 15, 23, 26,
	 5, 18, 31, 10,
	 2,  8, 24, 14,
	32, 27,  3,  9,
	19, 13, 30,  6,
	22, 11,  4, 25
};

//
// PERMUTATION CHOICE 1
//

static uint32_t PC1[56] = {
	//
	// C-part
	//

	57, 49, 41, 33, 25, 17,  9,
	 1, 58, 50, 42, 34, 26, 18,
	10,  2, 59, 51, 43, 35, 27,
	19, 11,  3, 60, 52, 44, 36,

	//
	// D-part
	//

	63, 55, 47, 39, 31, 23, 15,
	 7, 62, 54, 46, 38, 30, 22,
	14,  6, 61, 53, 45, 37, 29,
	21, 13,  5, 28, 20, 12,  4
};

//
// PERMUTATION CHOICE 2
//

static uint32_t PC2[48] = {
	14, 17, 11, 24,  1,  5,
	 3, 28, 15,  6, 21, 10,
	23, 19, 12,  4, 26,  8,
	16,  7, 27, 20, 13,  2,
	41, 52, 31, 37, 47, 55,
	30, 40, 51, 45, 33, 48,
	44, 49, 39, 56, 34, 53,
	46, 42, 50, 36, 29, 32
};

//
// LEFT SHIFT RULE
//

static uint32_t LSR[16] = {
	1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1
};


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFWDES::CFWDES()
{
}

CFWDES::~CFWDES()
{

}

void CFWDES::HexToBin(const uint32_t nHexLen, const uchar8_t *pHex, uchar8_t *pBin)
{
	uint32_t nDec;

	for ( uint32_t i=0; i<nHexLen; i++ ) {
		if ( pHex[i] >= 48 && pHex[i] <= 57 ) {
			nDec = pHex[i]-48;
		} else if ( pHex[i] >= 65 && pHex[i] <= 70 ) {
			nDec = pHex[i]-55;
		} else { // 97 ~ 102
			nDec = pHex[i]-87;
		}

		DecNumToBin(nDec, pBin+(i*4), 4);
	}
}

void CFWDES::HexToStr(const uint32_t nHexLen, const uchar8_t *pHex, uchar8_t *pStr)
{
	uint32_t nDec;
	uchar8_t nCh;

	for ( uint32_t i=0; i<nHexLen; i++ ) {
		if ( pHex[i] >= 48 && pHex[i] <= 57 ) {
			nDec = pHex[i]-48;
		} else if ( pHex[i] >= 65 && pHex[i] <= 70 ) {
			nDec = pHex[i]-55;
		} else { // 97 ~ 102
			nDec = pHex[i]-87;
		}

		if ( (i+1)%2 == 0 ) {
			nCh += nDec;
			pStr[i/2] = nCh;
		} else {
			nCh = 16*nDec;
		}
	}
}

void CFWDES::StrToHex(const uint32_t nStrLen, const uchar8_t *pStr, uchar8_t *pHex)
{
	for ( uint32_t i=0; i<nStrLen; i++ ) {
		sprintf((char*)(pHex+(i*2)), "%02X", pStr[i]);
	}
}

void CFWDES::StrToBin(const uint32_t nStrLen, const uchar8_t *pStr, uchar8_t *pBin)
{
	uint32_t nCnt = nStrLen;
	uchar8_t szBin[9];
	
	for ( uint32_t i=0; i<nStrLen; i++ ) {
		memset(szBin, NULL, 9);
		CharToBin(pStr[i], szBin);
		sprintf((char*)(pBin+(i*8)), "%08s", szBin);
	}
}

void CFWDES::CharToBin(const uchar8_t ch, uchar8_t *pBin)
{
	for ( uint32_t i=0; i<8; i++ ) {
		if ( ch & (uchar8_t)pow(2, 7-i) ) {
			pBin[i] = '1';
		} else {
			pBin[i] = '0';
		}
	}
}

void CFWDES::BinToDecNum(const uchar8_t *pBin, uint32_t *pDec)
{
	uint32_t nCnt = strlen((const char*)pBin);
	uint32_t nDec = 0;

	for ( uint32_t i=0; i<nCnt; i++ ) {
		nDec += ( (pBin[i]=='1' ? 1 : 0) *
				  (uint32_t)pow(2, (nCnt-1-i)) );
	}

	*pDec = nDec;
}

void CFWDES::DecNumToBin(const uint32_t nDec, uchar8_t *pBin, const uint32_t len)
{
	for ( uint32_t i=0; i<len; i++ ) {
		if ( nDec & (uint32_t)pow(2, len-1-i) ) {
			pBin[i] = '1';
		} else {
			pBin[i] = '0';
		}
	}
}

void CFWDES::BinToHex(const uint32_t nBinLen, const uchar8_t *pBin, uchar8_t *pHex)
{
	uint32_t nDec;
	uchar8_t BIN4[5];
	uchar8_t HEX[2];
	
	// 4bit 단위로 ... ( 즉, 16진수로 )
	for ( uint32_t i=0, j=0; i<nBinLen; i+=4, j++ ) {
		memset(BIN4, NULL, 5);
		memset(HEX, NULL, 2);

		strncpy((char*)BIN4, (const char*)(pBin+i), 4);
		BinToDecNum(BIN4, &nDec);
		sprintf((char*)HEX, "%X", nDec);
		
		pHex[j] = HEX[0];
	}
}

void CFWDES::InitialPermutation(const uchar8_t *pInputText, uchar8_t *pL, uchar8_t *pR)
{
	for ( uint32_t i=0; i<64; i++ ) {
		if ( i<32 ) {
			pL[i] = pInputText[IP[i]-1];
		} else {
			pR[i-32] = pInputText[IP[i]-1];
		}
	}
}

void CFWDES::CipherFunctionF(uchar8_t *pF, const uchar8_t *pRi_1, const uchar8_t *pKEYi)
{
	uchar8_t szOut_E[49];
	uchar8_t szOut_XOR[49];
	uchar8_t szOut_Sbox[33];

	memset(szOut_E, NULL, 49);
	memset(szOut_XOR, NULL, 49);
	memset(szOut_Sbox, NULL, 33);

	for ( uint32_t i=0; i<48; i++ ) {
		szOut_E[i] = pRi_1[E_table[i]-1];
	}

	XOR(48, szOut_XOR, szOut_E, pKEYi);

	for ( i=0; i<8; i++ ) {
		Calc_S_Box(i, szOut_Sbox+(i*4), szOut_XOR+(i*6));
	}

	for ( i=0; i<32; i++ ) {
		pF[i] = szOut_Sbox[P_table[i]-1];
	}
}

void CFWDES::Calc_S_Box(const uint32_t idxSbox, uchar8_t *pOut, const uchar8_t *pIn)
{
	uchar8_t szRow[3], szCol[5];
	uint32_t row, col;

	memset(szRow, NULL, 3);
	memset(szCol, NULL, 5);

	szRow[0] = pIn[0];
	szRow[1] = pIn[5];

	szCol[0] = pIn[1];
	szCol[1] = pIn[2];
	szCol[2] = pIn[3];
	szCol[3] = pIn[4];

	BinToDecNum(szRow, &row);
	BinToDecNum(szCol, &col);

	uchar8_t chOut = (uchar8_t)S_box[idxSbox][16*row+col];
	uchar8_t szCh[9];
	memset(szCh, NULL, 9);

	CharToBin(chOut, szCh);
	
	strcpy((char*)pOut, (const char*)(szCh+4));
}

void CFWDES::XOR(const uint32_t bitNum, uchar8_t *pOut, const uchar8_t *pIn1, const uchar8_t *pIn2)
{
	for ( uint32_t i=0; i<bitNum; i++ ) {
		pOut[i] = ( pIn1[i]+pIn2[i] == 97 ) ? '1' : '0';
		//pOut[i] = (pIn1[i]=='1' && pIn2[i]=='1') || (pIn1[i]=='0' && pIn2[i]=='0') ? '0' : '1';
	}
}

void CFWDES::PermutedChoice1(const uchar8_t *pKEY, uchar8_t *pC, uchar8_t *pD)
{
	// 64 bits ----> 56 bits
	for ( uint32_t i=0; i<56; i++ ) {
		if ( i<28 ) {
			pC[i] = pKEY[PC1[i]-1];
		} else {
			pD[i-28] = pKEY[PC1[i]-1];
		}
	}
}

void CFWDES::ShiftLeft(uchar8_t *pShiftedBin, const uchar8_t *pShiftBin, const uint32_t iterNum)
{
	uchar8_t lastBit;

	strcpy((char*)pShiftedBin, (const char*)pShiftBin);

	for ( uint32_t i=0; i<LSR[iterNum-1]; i++ ) {

		lastBit = pShiftedBin[0];

		for ( uint32_t j=0; j<27; j++ ) {
			pShiftedBin[j] = pShiftedBin[j+1];
		}

		pShiftedBin[27] = lastBit;
	}
}

void CFWDES::KeyScheduleCalculator(uchar8_t *pKEYi, uchar8_t *pCi, uchar8_t *pDi,
	const uchar8_t *pCi_1, const uchar8_t *pDi_1, const uint32_t iterNum)
{
	uchar8_t szKEY[57];
	memset(szKEY, NULL, 57);

	ShiftLeft(pCi, pCi_1, iterNum);
	ShiftLeft(pDi, pDi_1, iterNum);

	strcpy((char*)szKEY, (const char*)pCi);
	strcat((char*)szKEY, (const char*)pDi);

	for ( int i=0; i<48; i++ ) {
		pKEYi[i] = szKEY[PC2[i]-1];
	}
}

void CFWDES::Round(uchar8_t *pLi, uchar8_t *pRi, const uchar8_t *pLi_1, const uchar8_t *pRi_1, const uchar8_t *pKEYi)
{
	uchar8_t szF[33];

	memset(szF, NULL, 33);

	// L[i] = R[i-1]
	strcpy((char*)pLi, (const char*)pRi_1);

	// R[i] = L[i-1] XOR F(R[i-1], KEY[i])
	CipherFunctionF(szF, pRi_1, pKEYi);
	XOR(32, pRi, pLi_1, szF);
}

void CFWDES::InverseInitialPermutation(uchar8_t *pPreOut, const uchar8_t *pIn1, const uchar8_t *pIn2)
{
	uchar8_t PRE[65];
	memset(PRE, NULL, 65);

	strcpy((char*)PRE, (const char*)pIn1);
	strcat((char*)PRE, (const char*)pIn2);

	for ( uint32_t i=0; i<64; i++ ) {
		pPreOut[i] = PRE[IIP[i]-1];
	}
}

void CFWDES::Encode(const uchar8_t *pKEY, const uchar8_t *pPlainText, uchar8_t *pCipherText, const uint32_t roundNum/*=16*/)
{
	uchar8_t L[17][33], R[17][33];
	uchar8_t C[17][29], D[17][29];
	uchar8_t KEY[17][49]; // KEY[0] 는 사용하지 않음

	memset(L, NULL, 17*33);
	memset(R, NULL, 17*33);
	memset(C, NULL, 17*29);
	memset(D, NULL, 17*29);
	memset(KEY, NULL, 17*49);

	InitialPermutation(pPlainText, L[0], R[0]); // Ciphering
	PermutedChoice1(pKEY, C[0], D[0]); // Key Scheduling

	for ( uint32_t i=1; i<=roundNum; i++ ) {
		KeyScheduleCalculator(KEY[i], C[i], D[i], C[i-1], D[i-1], i);
		Round(L[i], R[i], L[i-1], R[i-1], KEY[i]);
	}

	i--;

//	if ( i == roundNum ) {
		InverseInitialPermutation(pCipherText, R[i], L[i]);
//	} else {
//		InverseInitialPermutation(pCipherText, L[i], R[i]);
//	}
}

void CFWDES::Decode(const uchar8_t *pKEY, const uchar8_t *pCipherText, uchar8_t *pPlainText, const uint32_t roundNum/*=16*/)
{
	uchar8_t L[17][33], R[17][33];
	uchar8_t C[17][29], D[17][29];
	uchar8_t KEY[17][49]; // KEY[0] 는 사용하지 않음

	memset(L, NULL, 17*33);
	memset(R, NULL, 17*33);
	memset(C, NULL, 17*29);
	memset(D, NULL, 17*29);
	memset(KEY, NULL, 17*49);

	InitialPermutation(pCipherText, L[0], R[0]); // Ciphering

	PermutedChoice1(pKEY, C[0], D[0]); // Key Scheduling

	for ( uint32_t i=1; i<=roundNum; i++ ) {
		KeyScheduleCalculator(KEY[i], C[i], D[i], C[i-1], D[i-1], i);
	}

	for ( i=1; i<=roundNum; i++ ) {
		Round(L[i], R[i], L[i-1], R[i-1], KEY[roundNum+1-i]);
	}

	i--;

//	if ( i == roundNum ) {
		InverseInitialPermutation(pPlainText, R[i], L[i]);
//	} else {
//		InverseInitialPermutation(pPlainText, L[i], R[i]);
//	}
}

void CFWDES::Cipher(mode_t mode, const uchar8_t *pKEY, const uint32_t nPlainTextLen, const uchar8_t *pPlainText,
					uint32_t *nCipherTextLen, uchar8_t *pCipherText, const uint32_t roundNum/*=16*/)
{
	uint32_t nLength = nPlainTextLen;
	uint32_t nQuotient = nLength/8;
	uint32_t nRemainder = nLength%8;
	uint32_t nCnt = nQuotient;

	if ( nRemainder ) {
		nCnt++;
	}

	uint32_t nLenBin = nCnt*8*8;

	uchar8_t *pPlainTextBin = new uchar8_t[nLenBin+1];
	memset(pPlainTextBin, NULL, nLenBin+1);

	uchar8_t *pPlainTextBin2 = new uchar8_t[nLenBin+1];
	memset(pPlainTextBin2, NULL, nLenBin+1);

	uchar8_t *pCipherTextBin = new uchar8_t[nLenBin+1];
	memset(pCipherTextBin, NULL, nLenBin+1);

	uchar8_t *pCipherTextHex = new uchar8_t[nLenBin/4+1];
	memset(pCipherTextHex, NULL, nLenBin/4+1);

	uchar8_t *pKeyBin = new uchar8_t[65];
	memset(pKeyBin, NULL, 65);

	uchar8_t IV[65];
	memset(IV, '0', 64);
	IV[64] = NULL;

	StrToBin(8, pKEY, pKeyBin);

	for ( uint32_t i=0; i<nCnt; i++ ) {

		if ( mode == CBC ) {
			StrToBin(8, pPlainText+(i*8), pPlainTextBin2+(i*64));
		} else {
			StrToBin(8, pPlainText+(i*8), pPlainTextBin+(i*64));
		}

		if ( mode == CBC ) {
			XOR( 64, pPlainTextBin+(i*64), pPlainTextBin2+(i*64), IV);
		}

		Encode(pKEY, pPlainTextBin+(i*64), pCipherTextBin+(i*64), roundNum);

		if ( mode == CBC ) {
			strncpy((char*)IV, (const char*)(pCipherTextBin+(i*64)), 64);
			IV[64] = NULL;
		}

		BinToHex(64, pCipherTextBin+(i*64), pCipherTextHex+(i*16));
		HexToStr(16, pCipherTextHex+(i*16), pCipherText+(i*8));
	}

	*nCipherTextLen = nCnt*8;//nPlainTextLen;

	delete [] pPlainTextBin;
	delete [] pPlainTextBin2;
	delete [] pCipherTextBin;
	delete [] pCipherTextHex;
	delete [] pKeyBin;
}

void CFWDES::Decipher(mode_t mode, const uchar8_t *pKEY, const uint32_t nCipherTextLen, const uchar8_t *pCipherText,
					uint32_t *nPlainTextLen, uchar8_t *pPlainText, const uint32_t roundNum/*=16*/)
{
	uint32_t nLength = nCipherTextLen;
	uint32_t nQuotient = nLength/8;
	uint32_t nRemainder = nLength%8;
	uint32_t nCnt = nQuotient;

	if ( nRemainder ) {
		nCnt++;
	}

	uint32_t nLenBin = nCnt*8*8;

	uchar8_t *pCipherTextBin = new uchar8_t[nLenBin+1];
	memset(pCipherTextBin, NULL, nLenBin+1);

	uchar8_t *pPlainTextBin = new uchar8_t[nLenBin+1];
	memset(pPlainTextBin, NULL, nLenBin+1);

	uchar8_t *pPlainTextBin2 = new uchar8_t[nLenBin+1];
	memset(pPlainTextBin2, NULL, nLenBin+1);

	uchar8_t *pPlainTextHex = new uchar8_t[nLenBin/4+1];
	memset(pPlainTextHex, NULL, nLenBin/4+1);

	uchar8_t *pKeyBin = new uchar8_t[65];
	memset(pKeyBin, NULL, 65);

	uchar8_t IV[65];
	memset(IV, '0', 64);
	IV[64] = NULL;

	StrToBin(8, pKEY, pKeyBin);

	for ( uint32_t i=0; i<nCnt; i++ ) {

		StrToBin(8, pCipherText+(i*8), pCipherTextBin+(i*64));

		if ( mode == CBC ) {
			Decode(pKEY, pCipherTextBin+(i*64), pPlainTextBin2+(i*64), roundNum);
			XOR(64, pPlainTextBin+(i*64), IV, pPlainTextBin2+(i*64));
			
			strncpy((char*)IV, (const char*)(pCipherTextBin+(i*64)), 64);
			IV[64] = NULL;
		} else {
			Decode(pKEY, pCipherTextBin+(i*64), pPlainTextBin+(i*64), roundNum);
		}

		BinToHex(64, pPlainTextBin+(i*64), pPlainTextHex+(i*16));
		HexToStr(16, pPlainTextHex+(i*16), pPlainText+(i*8));
	}

	*nPlainTextLen = nCnt*8;

/*
	for ( i=nCnt*8-1; i>=(nCnt-1)*8; i-- ) {
		if ( pPlainText[i] == NULL )
			(*nPlainTextLen)--;
		else
			break;

		if ( i == 0 )
			break;
	}
*/

	delete [] pCipherTextBin;
	delete [] pPlainTextBin;
	delete [] pPlainTextBin2;
	delete [] pPlainTextHex;
	delete [] pKeyBin;
}
