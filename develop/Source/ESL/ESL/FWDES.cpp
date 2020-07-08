// FWDES.cpp: implementation of the CFWDES class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\\ESL\\FWDES.h"

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

static UINT IP[64] = {
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

static UINT IIP[64] = {
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

static UINT S_box[8][64] = {
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
static UINT S1_box[64] = {
	14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,
	 0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,
	 4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,
	15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13
};

static UINT S2_box[64] = {
	15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,
	 3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,
	 0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15,
	13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9
};

static UINT S3_box[64] = {
	10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,
	13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,
	13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,
	 1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12
};

static UINT S4_box[64] = {
	 7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,
	13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9,
	10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,
	 3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14
};

static UINT S5_box[64] = {
	 2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9,
	14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6,
	 4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14,
	11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3
};

static UINT S6_box[64] = {
	12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,
	10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,
	 9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,
	 4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13
};

static UINT S7_box[64] = {
	 4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1,
	13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,
	 1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,
	 6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12
};

static UINT S8_box[64] = {
	13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,
	 1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,
	 7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,
	 2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11
};
*/

//
// E BIT SELECTION TABLE ( 32bits --> 48bits )
//

static UINT E_table[48] = {
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

static UINT P_table[32] = {
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

static UINT PC1[56] = {
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

static UINT PC2[48] = {
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

static UINT LSR[16] = {
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

void CFWDES::HexToBin(const UINT nHexLen, const UCHAR *pHex, UCHAR *pBin)
{
	UINT nDec;

	for ( UINT i=0; i<nHexLen; i++ ) {
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

void CFWDES::HexToStr(const UINT nHexLen, const UCHAR *pHex, UCHAR *pStr)
{
	UINT nDec;
	UCHAR nCh;

	for ( UINT i=0; i<nHexLen; i++ ) {
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

void CFWDES::StrToHex(const UINT nStrLen, const UCHAR *pStr, UCHAR *pHex)
{
	for ( UINT i=0; i<nStrLen; i++ ) {
		sprintf((char*)(pHex+(i*2)), "%02X", pStr[i]);
	}
}

void CFWDES::StrToBin(const UINT nStrLen, const UCHAR *pStr, UCHAR *pBin)
{
	UINT nCnt = nStrLen;
	UCHAR szBin[9];
	
	for ( UINT i=0; i<nStrLen; i++ ) {
		memset(szBin, NULL, 9);
		CharToBin(pStr[i], szBin);
		sprintf((char*)(pBin+(i*8)), "%08s", szBin);
	}
}

void CFWDES::CharToBin(const UCHAR ch, UCHAR *pBin)
{
	for ( UINT i=0; i<8; i++ ) {
		if ( ch & (UCHAR)pow(2, 7-i) ) {
			pBin[i] = '1';
		} else {
			pBin[i] = '0';
		}
	}
}

void CFWDES::BinToDecNum(const UCHAR *pBin, UINT *pDec)
{
	UINT nCnt = strlen((const char*)pBin);
	UINT nDec = 0;

	for ( UINT i=0; i<nCnt; i++ ) {
		nDec += ( (pBin[i]=='1' ? 1 : 0) *
				  (UINT)pow(2, (nCnt-1-i)) );
	}

	*pDec = nDec;
}

void CFWDES::DecNumToBin(const UINT nDec, UCHAR *pBin, const UINT len)
{
	for ( UINT i=0; i<len; i++ ) {
		if ( nDec & (UINT)pow(2, len-1-i) ) {
			pBin[i] = '1';
		} else {
			pBin[i] = '0';
		}
	}
}

void CFWDES::BinToHex(const UINT nBinLen, const UCHAR *pBin, UCHAR *pHex)
{
	UINT nDec;
	UCHAR BIN4[5];
	UCHAR HEX[2];
	
	// 4bit 단위로 ... ( 즉, 16진수로 )
	for ( UINT i=0, j=0; i<nBinLen; i+=4, j++ ) {
		memset(BIN4, NULL, 5);
		memset(HEX, NULL, 2);

		strncpy((char*)BIN4, (const char*)(pBin+i), 4);
		BinToDecNum(BIN4, &nDec);
		sprintf((char*)HEX, "%X", nDec);
		
		pHex[j] = HEX[0];
	}
}

void CFWDES::InitialPermutation(const UCHAR *pInputText, UCHAR *pL, UCHAR *pR)
{
	for ( UINT i=0; i<64; i++ ) {
		if ( i<32 ) {
			pL[i] = pInputText[IP[i]-1];
		} else {
			pR[i-32] = pInputText[IP[i]-1];
		}
	}
}

void CFWDES::CipherFunctionF(UCHAR *pF, const UCHAR *pRi_1, const UCHAR *pKEYi)
{
	UCHAR szOut_E[49];
	UCHAR szOut_XOR[49];
	UCHAR szOut_Sbox[33];

	memset(szOut_E, NULL, 49);
	memset(szOut_XOR, NULL, 49);
	memset(szOut_Sbox, NULL, 33);

	for ( UINT i=0; i<48; i++ ) {
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

void CFWDES::Calc_S_Box(const UINT idxSbox, UCHAR *pOut, const UCHAR *pIn)
{
	UCHAR szRow[3], szCol[5];
	UINT row, col;

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

	UCHAR chOut = (UCHAR)S_box[idxSbox][16*row+col];
	UCHAR szCh[9];
	memset(szCh, NULL, 9);

	CharToBin(chOut, szCh);
	
	strcpy((char*)pOut, (const char*)(szCh+4));
}

void CFWDES::XOR(const UINT bitNum, UCHAR *pOut, const UCHAR *pIn1, const UCHAR *pIn2)
{
	for ( UINT i=0; i<bitNum; i++ ) {
		pOut[i] = ( pIn1[i]+pIn2[i] == 97 ) ? '1' : '0';
		//pOut[i] = (pIn1[i]=='1' && pIn2[i]=='1') || (pIn1[i]=='0' && pIn2[i]=='0') ? '0' : '1';
	}
}

void CFWDES::PermutedChoice1(const UCHAR *pKEY, UCHAR *pC, UCHAR *pD)
{
	// 64 bits ----> 56 bits
	for ( UINT i=0; i<56; i++ ) {
		if ( i<28 ) {
			pC[i] = pKEY[PC1[i]-1];
		} else {
			pD[i-28] = pKEY[PC1[i]-1];
		}
	}
}

void CFWDES::ShiftLeft(UCHAR *pShiftedBin, const UCHAR *pShiftBin, const UINT iterNum)
{
	UCHAR lastBit;

	strcpy((char*)pShiftedBin, (const char*)pShiftBin);

	for ( UINT i=0; i<LSR[iterNum-1]; i++ ) {

		lastBit = pShiftedBin[0];

		for ( UINT j=0; j<27; j++ ) {
			pShiftedBin[j] = pShiftedBin[j+1];
		}

		pShiftedBin[27] = lastBit;
	}
}

void CFWDES::KeyScheduleCalculator(UCHAR *pKEYi, UCHAR *pCi, UCHAR *pDi,
	const UCHAR *pCi_1, const UCHAR *pDi_1, const UINT iterNum)
{
	UCHAR szKEY[57];
	memset(szKEY, NULL, 57);

	ShiftLeft(pCi, pCi_1, iterNum);
	ShiftLeft(pDi, pDi_1, iterNum);

	strcpy((char*)szKEY, (const char*)pCi);
	strcat((char*)szKEY, (const char*)pDi);

	for ( int i=0; i<48; i++ ) {
		pKEYi[i] = szKEY[PC2[i]-1];
	}
}

void CFWDES::Round(UCHAR *pLi, UCHAR *pRi, const UCHAR *pLi_1, const UCHAR *pRi_1, const UCHAR *pKEYi)
{
	UCHAR szF[33];

	memset(szF, NULL, 33);

	// L[i] = R[i-1]
	strcpy((char*)pLi, (const char*)pRi_1);

	// R[i] = L[i-1] XOR F(R[i-1], KEY[i])
	CipherFunctionF(szF, pRi_1, pKEYi);
	XOR(32, pRi, pLi_1, szF);
}

void CFWDES::InverseInitialPermutation(UCHAR *pPreOut, const UCHAR *pIn1, const UCHAR *pIn2)
{
	UCHAR PRE[65];
	memset(PRE, NULL, 65);

	strcpy((char*)PRE, (const char*)pIn1);
	strcat((char*)PRE, (const char*)pIn2);

	for ( UINT i=0; i<64; i++ ) {
		pPreOut[i] = PRE[IIP[i]-1];
	}
}

void CFWDES::Encode(const UCHAR *pKEY, const UCHAR *pPlainText, UCHAR *pCipherText, const UINT roundNum/*=16*/)
{
	UCHAR L[17][33], R[17][33];
	UCHAR C[17][29], D[17][29];
	UCHAR KEY[17][49]; // KEY[0] 는 사용하지 않음

	memset(L, NULL, 17*33);
	memset(R, NULL, 17*33);
	memset(C, NULL, 17*29);
	memset(D, NULL, 17*29);
	memset(KEY, NULL, 17*49);

	InitialPermutation(pPlainText, L[0], R[0]); // Ciphering
	PermutedChoice1(pKEY, C[0], D[0]); // Key Scheduling

	for ( UINT i=1; i<=roundNum; i++ ) {
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

void CFWDES::Decode(const UCHAR *pKEY, const UCHAR *pCipherText, UCHAR *pPlainText, const UINT roundNum/*=16*/)
{
	UCHAR L[17][33], R[17][33];
	UCHAR C[17][29], D[17][29];
	UCHAR KEY[17][49]; // KEY[0] 는 사용하지 않음

	memset(L, NULL, 17*33);
	memset(R, NULL, 17*33);
	memset(C, NULL, 17*29);
	memset(D, NULL, 17*29);
	memset(KEY, NULL, 17*49);

	InitialPermutation(pCipherText, L[0], R[0]); // Ciphering

	PermutedChoice1(pKEY, C[0], D[0]); // Key Scheduling

	for ( UINT i=1; i<=roundNum; i++ ) {
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

void CFWDES::Cipher(const UCHAR *pKEY, const UINT nPlainTextLen, const UCHAR *pPlainText,
					UINT *nCipherTextLen, UCHAR *pCipherText, bool mode/*ECB*/, const UINT roundNum/*=16*/)
{
	UINT nLength = nPlainTextLen;
	UINT nQuotient = nLength/8;
	UINT nRemainder = nLength%8;
	UINT nCnt = nQuotient;

	if ( nRemainder ) {
		nCnt++;
	}

	UINT nLenBin = nCnt*8*8;

	UCHAR *pPlainTextBin = new UCHAR[nLenBin+1];
	memset(pPlainTextBin, NULL, nLenBin+1);

	UCHAR *pPlainTextBin2 = new UCHAR[nLenBin+1];
	memset(pPlainTextBin2, NULL, nLenBin+1);

	UCHAR *pCipherTextBin = new UCHAR[nLenBin+1];
	memset(pCipherTextBin, NULL, nLenBin+1);

	UCHAR *pCipherTextHex = new UCHAR[nLenBin/4+1];
	memset(pCipherTextHex, NULL, nLenBin/4+1);

	UCHAR *pKeyBin = new UCHAR[65];
	memset(pKeyBin, NULL, 65);

	UCHAR IV[65];
	memset(IV, '0', 64);
	IV[64] = NULL;

	StrToBin(8, pKEY, pKeyBin);

	for ( UINT i=0; i<nCnt; i++ ) {

		if ( mode == CBC ) {
			StrToBin(8, pPlainText+(i*8), pPlainTextBin2+(i*64));
		} else {
			StrToBin(8, pPlainText+(i*8), pPlainTextBin+(i*64));
		}

		if ( mode == CBC ) {
			XOR( 64, pPlainTextBin+(i*64), pPlainTextBin2+(i*64), IV);
		}

		Encode(pKeyBin/*pKey*/, pPlainTextBin+(i*64), pCipherTextBin+(i*64), roundNum);

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

void CFWDES::Decipher(const UCHAR *pKEY, const UINT nCipherTextLen, const UCHAR *pCipherText,
					UINT *nPlainTextLen, UCHAR *pPlainText, bool mode/*ECB*/, const UINT roundNum/*=16*/)
{
	UINT nLength = nCipherTextLen;
	UINT nQuotient = nLength/8;
	UINT nRemainder = nLength%8;
	UINT nCnt = nQuotient;

	if ( nRemainder ) {
		nCnt++;
	}

	UINT nLenBin = nCnt*8*8;

	UCHAR *pCipherTextBin = new UCHAR[nLenBin+1];
	memset(pCipherTextBin, NULL, nLenBin+1);

	UCHAR *pPlainTextBin = new UCHAR[nLenBin+1];
	memset(pPlainTextBin, NULL, nLenBin+1);

	UCHAR *pPlainTextBin2 = new UCHAR[nLenBin+1];
	memset(pPlainTextBin2, NULL, nLenBin+1);

	UCHAR *pPlainTextHex = new UCHAR[nLenBin/4+1];
	memset(pPlainTextHex, NULL, nLenBin/4+1);

	UCHAR *pKeyBin = new UCHAR[65];
	memset(pKeyBin, NULL, 65);

	UCHAR IV[65];
	memset(IV, '0', 64);
	IV[64] = NULL;

	StrToBin(8, pKEY, pKeyBin);

	for ( UINT i=0; i<nCnt; i++ ) {

		StrToBin(8, pCipherText+(i*8), pCipherTextBin+(i*64));

		if ( mode == CBC ) {
			Decode(pKEY, pCipherTextBin+(i*64), pPlainTextBin2+(i*64), roundNum);
			XOR(64, pPlainTextBin+(i*64), IV, pPlainTextBin2+(i*64));
			
			strncpy((char*)IV, (const char*)(pCipherTextBin+(i*64)), 64);
			IV[64] = NULL;
		} else {
			Decode(pKeyBin/*pKey*/, pCipherTextBin+(i*64), pPlainTextBin+(i*64), roundNum);
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

////////////////////////////////////////////////////////////////////////////////////////////
// DES 알고리즘은 8바이트 블록 알고리즘이기 때문에 항상 문자열이 8바이트의 배수여야 한다. 
// 아래 함수는 인자로 들어오는 문자열이 8바이트의 배수가 아닐경우
// 공란을 추가시킨후 암호화하기 때문에
// 복호화시 결과값에 공란이 추가될 수 있다.
////////////////////////////////////////////////////////////////////////////////////////////
// 함수이름: CFWDES::Simple_Cipher
// 함수설명: 평문을 암호문으로 암호화
// 반환  형: int 
// 인자[CString strSource] : 평문
// 인자[CString strKey] : 암호키
// 인자[CString &strResult] : 암호문
int CFWDES::Simple_Cipher(CString strSource, CString strKey, CString &strResult)
{

	// 1. CString을 UCHAR로 변환
	UCHAR* szSrc = NULL;
	UINT nSrcLen = 0;

	UCHAR* szDst = NULL;
	UINT nDstLen = 0;

	INT nSize = 0;
	UCHAR* szKey = NULL;

	{
#ifdef _UNICODE
		// 평문을 8바이트의 배수로 맞춰준다.
		while(0 != strSource.GetLength()% (8/sizeof(TCHAR)) )
		{
			strSource += _T(" ");			
		}
		nSize = strSource.GetLength();
#else
		nSize = ::MultiByteToWideChar(CP_ACP, NULL, strSource.GetBuffer(0), -1, NULL, NULL) -1;
		strSource.ReleaseBuffer();
		
 		// 평문을 8바이트의 배수로 맞춰준다.
 		while(0 != nSize % (8/sizeof(WCHAR)) )
 		{
 			strSource += _T(" ");
			nSize = ::MultiByteToWideChar(CP_ACP, NULL, strSource.GetBuffer(0), -1, NULL, NULL) -1;
			strSource.ReleaseBuffer();
 		}
#endif
	}
	{
#ifdef _UNICODE		
		// 소스 변환
		UINT LEN = strSource.GetLength();

		WCHAR* sz_Tmp = new WCHAR[LEN+1];
		ZeroMemory(sz_Tmp, LEN);
		wcscpy((WCHAR*)sz_Tmp, (WCHAR*)strSource.GetBuffer(0));
		sz_Tmp[LEN] = '\0';

		szSrc = new UCHAR[LEN*sizeof(WCHAR)+1];
		ZeroMemory(szSrc, LEN*sizeof(WCHAR));
		szSrc[LEN*sizeof(WCHAR)] = '\0';


		// WCHAR를 바이트단위로 나누어 UCHAR에 설정한다.
		for(UINT uCnt = 0; uCnt < LEN; uCnt++)
		{
			WCHAR Tmp = sz_Tmp[uCnt];
			szSrc[(uCnt*2)] = (BYTE)(Tmp & 0x00FF);
			Tmp = Tmp >> 8;
			szSrc[(uCnt*2)+1] = (BYTE)(Tmp & 0x00FF);			
		}		

		nSrcLen = (LEN) * sizeof(WCHAR);

		delete []sz_Tmp;
#else
		WCHAR* szBuff = NULL;
		nSize = ::MultiByteToWideChar(CP_ACP, NULL, strSource.GetBuffer(0), -1, NULL, NULL);	
		strSource.ReleaseBuffer();
		if(0 < nSize)
		{	
			szBuff = new WCHAR[nSize+1];
			szBuff[nSize] = '\0';
			nSize = ::MultiByteToWideChar(CP_ACP, NULL, strSource.GetBuffer(0), -1, szBuff, nSize);
			strSource.ReleaseBuffer();
		}
		UINT LEN = nSize;
		nSrcLen = (LEN-1) * sizeof(WCHAR);
		szSrc = new UCHAR[LEN*sizeof(WCHAR)+1];
		ZeroMemory(szSrc, LEN*sizeof(WCHAR));
		szSrc[LEN*sizeof(WCHAR)] = '\0';

		wcscpy((WCHAR*)szSrc, (WCHAR*)szBuff);
		delete []szBuff;
#endif
	}
	{
		// 결과물 메모리 할당
#ifdef _UNICODE
		UINT n1 = (nSize*sizeof(WCHAR))/4, n2 = (nSize*sizeof(WCHAR))%4;
		if(0 != n2)
		{
			n1++;
		}
		UINT LEN = n1*4;
		szDst = new UCHAR[LEN+1];
		ZeroMemory(szDst, LEN);
		szDst[LEN] = '\0';		
#else
		UINT n1 = (nSize*sizeof(WCHAR))/4, n2 = (nSize*sizeof(WCHAR))%4;
		if(0 != n2)
		{
			n1++;
		}
		UINT LEN = n1*4;
		szDst = new UCHAR[LEN+1];
		ZeroMemory(szDst, LEN);
		szDst[LEN] = '\0';		
#endif
	}	
	{
#ifdef _UNICODE
		// 키 메모리 할당
		const INT KEY_LENGTH = 8/sizeof(WCHAR); // 키는 8바이트
		while(KEY_LENGTH > strKey.GetLength())
		{
			strKey += _T(" ");
		}
		strKey = strKey.Left(KEY_LENGTH);

		WCHAR* sz_Tmp = new WCHAR[KEY_LENGTH+1];
		ZeroMemory(sz_Tmp, KEY_LENGTH);
		wcscpy((WCHAR*)sz_Tmp, (WCHAR*)strKey.GetBuffer(0));
		sz_Tmp[KEY_LENGTH] = '\0';

		szKey = new UCHAR[KEY_LENGTH*sizeof(WCHAR)+1];
		ZeroMemory(szKey, KEY_LENGTH*sizeof(WCHAR));
		szKey[KEY_LENGTH] = '\0';
		for(int uCnt = 0; uCnt < KEY_LENGTH; uCnt++)
		{
			WCHAR Tmp = sz_Tmp[uCnt];
			szKey[(uCnt*2)] = (BYTE)(Tmp & 0x00FF);
			Tmp = Tmp >> 8;
			szKey[(uCnt*2)+1] = (BYTE)(Tmp & 0x00FF);			
		}
		delete []sz_Tmp;
#else
		UINT LEN = strKey.GetLength();
		szKey = new UCHAR[LEN*sizeof(WCHAR)+1];
		ZeroMemory(szKey, LEN*sizeof(WCHAR));
		szKey[LEN*sizeof(WCHAR)] = '\0';

		UCHAR szTest[MAX_PATH];
		ZeroMemory(szTest, sizeof(szTest));		
		strncpy((CHAR*)szTest, (CHAR*)strKey.GetBuffer(0), 4);
		strKey.ReleaseBuffer();
		szKey[0] = szTest[0];
		szKey[2] = szTest[1];
		szKey[4] = szTest[2];
		szKey[6] = szTest[3];
#endif
	}
	
	// 2. 암호화
	Cipher(szKey, nSrcLen, szSrc, &nDstLen, szDst);
	
	// 3. CString 변환
	{
		UCHAR* sz_Tmp = new UCHAR[nDstLen+2];
		ZeroMemory(sz_Tmp, nDstLen+1);
		sz_Tmp[nDstLen+1] = '\0';

		
		for(UINT nCnt = 0; nCnt < nDstLen; nCnt++)
		{			
			sz_Tmp[nCnt] = szDst[nCnt];			
		}		
#ifdef _UNICODE
	 	strResult.Format(_T("%s"), (WCHAR*)sz_Tmp);
#else		
		wcsncpy((WCHAR*)strResult.GetBufferSetLength(nDstLen*sizeof(WCHAR)), (WCHAR*)szDst, nDstLen);
		strResult.ReleaseBuffer(nDstLen*sizeof(WCHAR));		
#endif
		delete []sz_Tmp;
	}

	// 4. 메모리 해제
	delete []szKey;
	delete []szSrc;
	delete []szDst;

	return 0;
}


////////////////////////////////////////////////////////////////////////////////////////////
// DES 알고리즘은 8바이트 블록 알고리즘이기 때문에 항상 문자열이 8바이트의 배수여야 한다. 
// 암호화한 평문이 8바이트의 배수가 아닌 경우 
// 복호화시 결과값에 공란이 추가될 수 있다.
////////////////////////////////////////////////////////////////////////////////////////////
// 함수이름: CFWDES::Simple_Decipher
// 함수설명: 암호문을 평문으로 복호화
// 반환  형: int 
// 인자[CString strSource] : 암호문
// 인자[CString strKey] : 암호키
// 인자[CString &strResult] : 평문
int CFWDES::Simple_Decipher(CString strSource, CString strKey, CString &strResult)
{

	// 1. CString을 UCHAR로 변환
	UCHAR* szSrc = NULL;
	UINT nSrcLen = 0;

	UCHAR* szDst = NULL;
	UINT nDstLen = 0;

	UCHAR* szKey = NULL;

	{
#ifdef _UNICODE
		// 암호문이 8바이트의 배수가 아니면 잘못된 문장
		if(0 != strSource.GetLength()% (8/sizeof(TCHAR)) )
		{
			return -35001;
		}

		// 소스 변환		
		INT n1 = strSource.GetLength() / 4, n2 = strSource.GetLength() % 4;
		if(0 != n2)
		{
			n1++;
		}
		UINT LEN = n1*4;

		TCHAR* sz_Tmp = new TCHAR[LEN+1];
		ZeroMemory(sz_Tmp, LEN);
		_tcscpy((TCHAR*)sz_Tmp, (TCHAR*)strSource.GetBuffer(0));
		sz_Tmp[LEN] = '\0';

		szSrc = new UCHAR[LEN*sizeof(TCHAR)+1];
		ZeroMemory(szSrc, LEN*sizeof(TCHAR));
		szSrc[LEN*sizeof(TCHAR)] = '\0';

		// TCHAR를 바이트단위로 나누어 UCHAR에 설정한다.
		for(UINT uCnt = 0; uCnt < LEN; uCnt++)
		{
			TCHAR Tmp = sz_Tmp[uCnt];
			szSrc[(uCnt*2)] = (BYTE)(Tmp & 0x00FF);
			Tmp = Tmp >> 8;
			szSrc[(uCnt*2)+1] = (BYTE)(Tmp & 0x00FF);			
		}		

		nSrcLen = (LEN) * sizeof(TCHAR);
		delete []sz_Tmp;
#else		
// 		INT nSize = ::MultiByteToWideChar(CP_ACP, NULL, strSource.GetBuffer(0), -1, NULL, NULL) -1;
// 		strSource.ReleaseBuffer();
// 		
// 		// 암호문이 8바이트의 배수가 아니면 잘못된 문장
// 		while(0 != nSize % (8/sizeof(WCHAR)) )
// 		{
// 			return -35001;
// 		}

		UINT LEN = strSource.GetLength();
		nSrcLen = (LEN);
		szSrc = new UCHAR[LEN*sizeof(WCHAR)+1];
		ZeroMemory(szSrc, LEN*sizeof(WCHAR));
		szSrc[LEN*sizeof(WCHAR)] = '\0';

		wcscpy((WCHAR*)szSrc, (WCHAR*)strSource.GetBuffer(0));
		strSource.ReleaseBuffer();
#endif
	}
	{
#ifdef _UNICODE
		// 결과물 메모리 할당
		INT n1 = strSource.GetLength() / 4, n2 = strSource.GetLength() % 4;
		if(0 != n2)
		{
			n1++;
		}
		UINT LEN = n1*4;
		szDst = new UCHAR[LEN*sizeof(TCHAR)+1];
		ZeroMemory(szDst, LEN*sizeof(TCHAR));
		szDst[LEN*sizeof(TCHAR)] = '\0';		
#else 
		UINT LEN = strSource.GetLength();
		szDst = new UCHAR[LEN*sizeof(WCHAR)+1];
		ZeroMemory(szDst, LEN*sizeof(WCHAR));
		szDst[LEN*sizeof(TCHAR)] = '\0';		
#endif
	}	
	{
#ifdef _UNICODE
		// 키 메모리 할당
		const INT KEY_LENGTH = 8/sizeof(TCHAR); // 키는 8바이트
		while(KEY_LENGTH > strKey.GetLength())
		{
			strKey += _T(" ");
		}
		strKey = strKey.Left(KEY_LENGTH);

		TCHAR* sz_Tmp = new TCHAR[KEY_LENGTH+1];
		ZeroMemory(sz_Tmp, KEY_LENGTH);
		_tcscpy((TCHAR*)sz_Tmp, (TCHAR*)strKey.GetBuffer(0));
		sz_Tmp[KEY_LENGTH] = '\0';

		szKey = new UCHAR[KEY_LENGTH*sizeof(TCHAR)+1];
		ZeroMemory(szKey, KEY_LENGTH*sizeof(TCHAR));
		szKey[KEY_LENGTH] = '\0';

		for(int uCnt = 0; uCnt < KEY_LENGTH; uCnt++)
		{
			TCHAR Tmp = sz_Tmp[uCnt];
			szKey[(uCnt*2)] = (BYTE)(Tmp & 0x00FF);
			Tmp = Tmp >> 8;
			szKey[(uCnt*2)+1] = (BYTE)(Tmp & 0x00FF);			
		}
		delete []sz_Tmp;
#else
		UINT LEN = strKey.GetLength();
		szKey = new UCHAR[LEN*sizeof(WCHAR)+1];
		ZeroMemory(szKey, LEN*sizeof(WCHAR));
		szKey[LEN*sizeof(WCHAR)] = '\0';

		UCHAR szTest[MAX_PATH];
		ZeroMemory(szTest, sizeof(szTest));		
		strncpy((CHAR*)szTest, (CHAR*)strKey.GetBuffer(0), 4);
		strKey.ReleaseBuffer();
		szKey[0] = szTest[0];
		szKey[2] = szTest[1];
		szKey[4] = szTest[2];
		szKey[6] = szTest[3];
#endif
		
	}
	
	// 2. 복호화
	Decipher(szKey, nSrcLen, szSrc, &nDstLen, szDst);
	
	// 3. CString 변환
	{
 		UCHAR* sz_Tmp = new UCHAR[nDstLen+2];
		ZeroMemory(sz_Tmp, nDstLen+1);
		sz_Tmp[nDstLen+1] = '\0';
		
		for(UINT nCnt = 0; nCnt < nDstLen; nCnt++)
		{			
			sz_Tmp[nCnt] = szDst[nCnt];			
		}
#ifdef _UNICODE
	 	strResult.Format(_T("%s"), (TCHAR*)sz_Tmp);
#else
	UINT LEN = strKey.GetLength();
	ZeroMemory(szKey, LEN*sizeof(WCHAR)+1);
	CHAR* szBuff = NULL;
	INT nSize = ::WideCharToMultiByte(CP_ACP, 0, (WCHAR*)sz_Tmp, -1, NULL, NULL, NULL, NULL);
	if(0 < nSize)
	{	
		szBuff = new CHAR[nSize+1];
		szBuff[nSize] = '\0';
		nSize = ::WideCharToMultiByte(CP_ACP, 0, (WCHAR*)sz_Tmp, -1, szBuff, nSize, NULL, NULL);
	}
	strResult.Format(_T("%s"), szBuff);
	delete []szBuff;
#endif
		delete []sz_Tmp;		
	}

	// 4. 메모리 해제
	delete []szKey;
	delete []szSrc;
	delete []szDst;

	return 0;
}


// 함수이름: CFWDES::Read_DES
// 함수설명: 파일경로로 부터 암호문을 읽어와 복호화한다.
// 반환  형: int 
// 인자[CString strFile] : 암호문이 저장된 파일의 이름
// 인자[CString strKey] : 암호키
// 인자[CString &strDES_Result] : 평문
int CFWDES::Read_DES_File(CString strFile, CString strKey, CString &strDES_Result)
{
	// 1. 암호문 파일을 읽는다.
	CFile file;
	CFileException ex;	
	BOOL bIsOpen = file.Open(strFile, CFile::modeRead, &ex);
	if(FALSE == bIsOpen)
	{
		TCHAR szMsg[1024];
		ex.GetErrorMessage(szMsg, 1024);
		TRACE(szMsg);
		return -1001;
	}

	int nLen = file.GetLength()/sizeof(WCHAR);
  	WCHAR* pChar = new WCHAR[nLen+1];
	ZeroMemory(pChar, nLen);
	pChar[nLen] = '\0';

 	file.Read((WCHAR*)pChar, nLen*sizeof(WCHAR));
	file.Close();

	// 2. 암호문을 평문으로 복호화한다.	
#ifdef _UNICODE
	CString strSrc = _T("");	
 	strSrc.Format(_T("%s"), pChar);
#else
	CString strSrc = _T("");		
	wcsncpy((WCHAR*)strSrc.GetBufferSetLength(nLen*sizeof(WCHAR)), (WCHAR*)pChar, nLen);
	strSrc.ReleaseBuffer(nLen*sizeof(WCHAR));
#endif
	delete []pChar;

	return Simple_Decipher(strSrc, strKey, strDES_Result);
}


// 함수이름: CFWDES::Write_DES_File
// 함수설명: 입력받은 평문을 암호문으로 저장한다.
// 반환  형: int 
// 인자[CString strFile] : 저장할 파일이름
// 인자[CString strKey] : 암호키
// 인자[CString& strDES_Input] : 암호화 할 평문
int CFWDES::Write_DES_File(CString strFile, CString strKey, CString& strDES_Input)
{
	// 1. 암호문으로 변경
	CString strDst = _T("");

	CFWDES DES;
	Simple_Cipher(strDES_Input, strKey, strDst);

	// 2. 파일 기록
	CFileFind filefind;	
	UINT nOpenFlags = CFile::modeWrite|CFile::modeCreate|CFile::typeBinary;
	if(TRUE == filefind.FindFile(strFile))
	{
//		nOpenFlags ^= CFile::modeCreate;
	}
	
	CFile file;
	CFileException ex;
 	BOOL bIsOpen = file.Open(strFile, nOpenFlags, &ex);
	if(FALSE == bIsOpen)
	{
		TCHAR szMsg[1024];
		ex.GetErrorMessage(szMsg, 1024);
		TRACE(szMsg);
		return -1001;
	}
#ifdef _UNICODE	
	file.SeekToEnd();
	file.Write((WCHAR*)strDst.GetBuffer(0), (strDst.GetLength())*sizeof(WCHAR));
	strDst.ReleaseBuffer();
#else
	file.SeekToEnd();
	INT nLen = strDst.GetLength();
	file.Write((WCHAR*)strDst.GetBuffer(0), (strDst.GetLength()/sizeof(WCHAR)));
	strDst.ReleaseBuffer();
#endif
	file.Close();

	return 0;
}


// 함수이름: CFWDES::EXEC_FUNC
// 함수설명: 임시 함수
// 반환  형: LRESULT 
// 인자[CString strFuncName] : 
// 인자[CStringArray &aInput] : 
// 인자[CStringArray &aOutput] : 
LRESULT CFWDES::EXEC_FUNC(CString strFuncName, CStringArray &aInput, CStringArray &aOutput)
{

	return 0;
}
