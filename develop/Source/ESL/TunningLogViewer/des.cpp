/***************************************************************************
* Copyright (c) 2000-2004, Future Systems, Inc. / Seoul, Republic of Korea *
* All Rights Reserved.                                                     *
*                                                                          *
* This document contains proprietary and confidential information.  No     *
* parts of this document or the computer program it embodies may be in     *
* any way copied, duplicated, reproduced, translated into a different      *
* programming language, or distributed to any person, company, or          *
* corporation without the prior written consent of Future Systems, Inc.    *
*                              Hyo Sun Hwang                               *
*                372-2 YangJae B/D 6th Floor, Seoul, Korea                 *
*                           +82-2-578-0581 (552)                           *
***************************************************************************/

/*--------------------- [ Version/Command in detais] ---------------------*\
Description : des.c
			(C-source file) Block Cipher DES - core function

C0000 : Created by Hyo Sun Hwang (hyosun@future.co.kr) 2000/12/31

C0001 : Modified by Hyo Sun Hwang (hyosun@future.co.kr) 2000/00/00

\*------------------------------------------------------------------------*/
#include "stdafx.h"
/*************** Header files *********************************************/
#include "des.h"

/*************** Constants ************************************************/
#define NUM_WEAK_KEY	16

/* temp insert */
	#define BIG_B2D(B, D)		D = *(DWORD *)(B)
	#define BIG_D2B(D, B)		*(DWORD *)(B) = (DWORD)(D)
	#define LITTLE_B2D(B, D)	D = ENDIAN_REVERSE_DWORD(*(DWORD *)(B))
	#define LITTLE_D2B(D, B)	*(DWORD *)(B) = ENDIAN_REVERSE_DWORD(D)
/* temp insert */	
/*************** Macros ***************************************************/
//	MACROS for Key-Initialize
#define PERM_OP(a,b,t,n,m) {			\
	(t) = ((((a)>>(n))^(b))&(m));		\
	(b) ^= (t);							\
	(a) ^= ((t)<<(n));					\
}
#define HPERM_OP(a,t,n,m) {				\
	(t) = ((((a)<<(16-(n)))^(a))&(m));	\
	(a) = (a) ^ (t) ^ (t >> (16-(n)));	\
}
#define IP(l,r) {						\
	DWORD tt;							\
	PERM_OP(r,l,tt, 4,0x0f0f0f0f);		\
	PERM_OP(l,r,tt,16,0x0000ffff);		\
	PERM_OP(r,l,tt, 2,0x33333333);		\
	PERM_OP(l,r,tt, 8,0x00ff00ff);		\
	PERM_OP(r,l,tt, 1,0x55555555);		\
}
#define FP(l,r) {						\
	DWORD tt;							\
	PERM_OP(l,r,tt, 1,0x55555555);		\
	PERM_OP(r,l,tt, 8,0x00ff00ff);		\
	PERM_OP(l,r,tt, 2,0x33333333);		\
	PERM_OP(r,l,tt,16,0x0000ffff);		\
	PERM_OP(l,r,tt, 4,0x0f0f0f0f);		\
}

//	MACROS for ENCRYPTION - DES Round Function
#define DES_ENCRYPT(LL,R,S) {				\
	u = R ^ RoundKey[S  ];				\
	t = R ^ RoundKey[S+1];				\
	t = ROTR_DWORD(t,4);					\
	LL ^= des_SPtrans[0][(u>> 2)&0x3f]		\
		^ des_SPtrans[2][(u>>10)&0x3f]		\
		^ des_SPtrans[4][(u>>18)&0x3f]		\
		^ des_SPtrans[6][(u>>26)&0x3f]		\
		^ des_SPtrans[1][(t>> 2)&0x3f]		\
		^ des_SPtrans[3][(t>>10)&0x3f]		\
		^ des_SPtrans[5][(t>>18)&0x3f]		\
		^ des_SPtrans[7][(t>>26)&0x3f];		\
}

/*************** Global Variables *****************************************/
extern const BYTE	des_weak_keys[NUM_WEAK_KEY][DES_USER_KEY_LEN];
extern const DWORD	des_skb[8][64];
extern const DWORD	des_SPtrans[8][64];

/*************** Function *************************************************
* 
*/
RET_VAL DES_KeySchedule(
		BYTE		*UserKey,		//	사용자 비밀키 입력
		DWORD		UserKeyLen,		//	사용자 비밀키의 바이트 수
		DES_ALG_INFO	*AlgInfo)	//	암호용/복호용 Round Key 생성/저장
{
	DWORD	i, shifts2[16]={0,0,1,1,1,1,1,1,0,1,1,1,1,1,1,0};
	DWORD	c, d, t, s, t2, *RoundKey=AlgInfo->RoundKey;

	////
	if( UserKeyLen!=DES_USER_KEY_LEN )
		return CTR_INVALID_USERKEYLEN;

	////
	//	Weak key check : retuen 'CTR_WEAK_KEY' if weak key
	for( i=0; i<NUM_WEAK_KEY; i++)
		if( memcmp(des_weak_keys[i], UserKey, DES_USER_KEY_LEN)==0 )
			return CTR_WEAK_KEY;

	//	get UserKey
	LITTLE_B2D( &(UserKey[0]), c);
	LITTLE_B2D( &(UserKey[4]), d);

	//	some permutations
	PERM_OP (d,c,t,4,0x0f0f0f0f);
	HPERM_OP(c,t,-2,0xcccc0000);
	HPERM_OP(d,t,-2,0xcccc0000);
	PERM_OP (d,c,t,1,0x55555555);
	PERM_OP (c,d,t,8,0x00ff00ff);
	PERM_OP (d,c,t,1,0x55555555);
	d = (((d&0x000000ff)<<16) | (d&0x0000ff00) |
		 ((d&0x00ff0000)>>16) | ((c&0xf0000000)>>4));
	c &= 0x0fffffff;

	//	generating expanded key
	for ( i=0; i<DES_NO_ROUNDS; i++ ) {
		if ( shifts2[i] ) {
			c = ( (c>>2) | (c<<26) );
			d = ( (d>>2) | (d<<26) );
		}
		else {
			c = ( (c>>1) | (c<<27) );
			d = ( (d>>1) | (d<<27) );
		}

		s = des_skb[0][ (c    )&0x3f                ] |
			des_skb[1][((c>> 6)&0x03)|((c>> 7)&0x3c)] |
			des_skb[2][((c>>13)&0x0f)|((c>>14)&0x30)] |
			des_skb[3][((c>>20)&0x01)|((c>>21)&0x06)|((c>>22)&0x38)];
		t = des_skb[4][ (d     )&0x3f               ] |
			des_skb[5][((d>> 7)&0x03)|((d>> 8)&0x3c)] |
			des_skb[6][ (d>>15)&0x3f                ] |
			des_skb[7][((d>>21)&0x0f)|((d>>22)&0x30)];

		//	table contained 0213 4657
		t2 = ( (t<<16) | (s&0x0000ffff) );
		*(RoundKey++) = ROTR_DWORD(t2,30);

		t2 = ( (s>>16) | (t&0xffff0000) );
		*(RoundKey++) = ROTR_DWORD(t2,26);
	}

	//	Remove sensitive data
	c = d = t = s = t2 = 0;

	//	return (success)
	return CTR_SUCCESS;
}

/*************** Macros ***************************************************/
#define SeedRound(A,B,C,D, K) {									\
	DWORD T0, T1;												\
	T0 = C ^ (K)[0];											\
	T1 = D ^ (K)[1];											\
	T1 ^= T0;													\
	T1 = DES_SL[0][(T1    )&0xFF] ^ DES_SL[1][(T1>> 8)&0xFF]	\
	   ^ DES_SL[2][(T1>>16)&0xFF] ^ DES_SL[3][(T1>>24)&0xFF];	\
	T0 += T1;													\
	T0 = DES_SL[0][(T0    )&0xFF] ^ DES_SL[1][(T0>> 8)&0xFF]	\
	   ^ DES_SL[2][(T0>>16)&0xFF] ^ DES_SL[3][(T0>>24)&0xFF];	\
	T1 += T0;													\
	T1 = DES_SL[0][(T1    )&0xFF] ^ DES_SL[1][(T1>> 8)&0xFF]	\
	   ^ DES_SL[2][(T1>>16)&0xFF] ^ DES_SL[3][(T1>>24)&0xFF];	\
	T0 += T1;													\
	A ^= T0; B ^= T1;											\
}

/*************** Function *************************************************
*	
*/
VOID	DES_Encrypt(
		VOID		*CipherKey,		//	암/복호용 Round Key
		BYTE		*Data)			//	입출력을 위한 블록을 가리키는 pointer
{
	DWORD	l, r, t, u, *RoundKey=(DWORD*)CipherKey;

	//
//DEL 	LITTLE_B2D( &(Data[0]), r);
//DEL 	LITTLE_B2D( &(Data[4]), l);
	r = *(Data+0);
	r = r << 8;
	r = r + *(Data+1);
	r = r << 8;
	r = r + *(Data+2);
	r = r << 8;
	r = r + *(Data+3);

	l = *(Data+4);
	l = l << 8;
	l = l + *(Data+5);
	l = l << 8;
	l = l + *(Data+6);
	l = l << 8;
	l = l + *(Data+7);

	IP(r,l);

	r = ROTR_DWORD(r,29);
	l = ROTR_DWORD(l,29);

	DWORD dwKey[32];
 	memset(dwKey, 0x00, 32 * sizeof(DWORD));
	_tcscpy((TCHAR*)dwKey, (TCHAR*)CipherKey);
	RoundKey = (DWORD*)dwKey[0];

	//	_tmain part of DES enc/dec-ryptin with 16rounds
	DES_ENCRYPT(l,r, 0);
	DES_ENCRYPT(r,l, 2);
	DES_ENCRYPT(l,r, 4);
	DES_ENCRYPT(r,l, 6);
	DES_ENCRYPT(l,r, 8);
	DES_ENCRYPT(r,l,10);
	DES_ENCRYPT(l,r,12);
	DES_ENCRYPT(r,l,14);
	DES_ENCRYPT(l,r,16);
	DES_ENCRYPT(r,l,18);
	DES_ENCRYPT(l,r,20);
	DES_ENCRYPT(r,l,22);
	DES_ENCRYPT(l,r,24);
	DES_ENCRYPT(r,l,26);
	DES_ENCRYPT(l,r,28);
	DES_ENCRYPT(r,l,30);

	//
	l = ROTR_DWORD(l,3);
	r = ROTR_DWORD(r,3);

	FP(r,l);

//DEL 	LITTLE_D2B(l, &(Data[0]));
//DEL 	LITTLE_D2B(r, &(Data[4]));
	*(Data+7) = (BYTE)(0x000000ff & r);
	r = r >> 8;
	*(Data+6) = (BYTE)(0x000000ff & r);
	r = r >> 8;
	*(Data+5) = (BYTE)(0x000000ff & r);
	r = r >> 8;
	*(Data+4) = (BYTE)(0x000000ff & r);
	
	*(Data+3) = (BYTE)(0x000000ff & l);
	l = l >> 8;
	*(Data+2) = (BYTE)(0x000000ff & l);
	l = l >> 8;
	*(Data+1) = (BYTE)(0x000000ff & l);
	l = l >> 8;
	*(Data+0) = (BYTE)(0x000000ff & l);
	//	Remove sensitive data
	l = r = t = u = 0;
}

/*************** Function *************************************************
*	
*/
VOID	DES_Decrypt(
		VOID		*CipherKey,		//	암/복호용 Round Key
		BYTE		*Data)			//	입출력을 위한 블록을 가리키는 pointer
{
	DWORD	l, r, t, u, *RoundKey=(DWORD*)CipherKey;

	//
//DEL 	LITTLE_B2D( &(Data[0]), r);
//DEL 	LITTLE_B2D( &(Data[4]), l);
	r = *(Data+0);
	r = r << 8;
	r = r + *(Data+1);
	r = r << 8;
	r = r + *(Data+2);
	r = r << 8;
	r = r + *(Data+3);

	l = *(Data+4);
	l = l << 8;
	l = l + *(Data+5);
	l = l << 8;
	l = l + *(Data+6);
	l = l << 8;
	l = l + *(Data+7);

	IP(r,l);

	r = ROTR_DWORD(r,29);
	l = ROTR_DWORD(l,29);

	DWORD dwKey[32];
 	memset(dwKey, 0x00, 32 * sizeof(DWORD));
	_tcscpy((TCHAR*)dwKey, (TCHAR*)CipherKey);
	RoundKey = (DWORD*)dwKey[0];

	//	_tmain part of DES enc/dec-ryptin with 16rounds
	DES_ENCRYPT(l,r,30);
	DES_ENCRYPT(r,l,28);
	DES_ENCRYPT(l,r,26);
	DES_ENCRYPT(r,l,24);
	DES_ENCRYPT(l,r,22);
	DES_ENCRYPT(r,l,20);
	DES_ENCRYPT(l,r,18);
	DES_ENCRYPT(r,l,16);
	DES_ENCRYPT(l,r,14);
	DES_ENCRYPT(r,l,12);
	DES_ENCRYPT(l,r,10);
	DES_ENCRYPT(r,l, 8);
	DES_ENCRYPT(l,r, 6);
	DES_ENCRYPT(r,l, 4);
	DES_ENCRYPT(l,r, 2);
	DES_ENCRYPT(r,l, 0);

	//
	l = ROTR_DWORD(l,3);
	r = ROTR_DWORD(r,3);

	FP(r,l);

//DEL 	LITTLE_D2B(l, &(Data[0]));
//DEL 	LITTLE_D2B(r, &(Data[4]));
	*(Data+7) = (BYTE)(0x000000ff & r);
	r = r >> 8;
	*(Data+6) = (BYTE)(0x000000ff & r);
	r = r >> 8;
	*(Data+5) = (BYTE)(0x000000ff & r);
	r = r >> 8;
	*(Data+4) = (BYTE)(0x000000ff & r);
	
	*(Data+3) = (BYTE)(0x000000ff & l);
	l = l >> 8;
	*(Data+2) = (BYTE)(0x000000ff & l);
	l = l >> 8;
	*(Data+1) = (BYTE)(0x000000ff & l);
	l = l >> 8;
	*(Data+0) = (BYTE)(0x000000ff & l);
	//	Remove sensitive data
	l = r = t = u = 0;
}

/***************** CONST DATA *********************************************/
const BYTE des_weak_keys[NUM_WEAK_KEY][DES_USER_KEY_LEN]={
	//	weak keys
	{0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01},
	{0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE},
	{0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F},
	{0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0},
	//	semi-weak keys
	{0x01,0xFE,0x01,0xFE,0x01,0xFE,0x01,0xFE},
	{0xFE,0x01,0xFE,0x01,0xFE,0x01,0xFE,0x01},
	{0x1F,0xE0,0x1F,0xE0,0x0E,0xF1,0x0E,0xF1},
	{0xE0,0x1F,0xE0,0x1F,0xF1,0x0E,0xF1,0x0E},
	{0x01,0xE0,0x01,0xE0,0x01,0xF1,0x01,0xF1},
	{0xE0,0x01,0xE0,0x01,0xF1,0x01,0xF1,0x01},
	{0x1F,0xFE,0x1F,0xFE,0x0E,0xFE,0x0E,0xFE},
	{0xFE,0x1F,0xFE,0x1F,0xFE,0x0E,0xFE,0x0E},
	{0x01,0x1F,0x01,0x1F,0x01,0x0E,0x01,0x0E},
	{0x1F,0x01,0x1F,0x01,0x0E,0x01,0x0E,0x01},
	{0xE0,0xFE,0xE0,0xFE,0xF1,0xFE,0xF1,0xFE},
	{0xFE,0xE0,0xFE,0xE0,0xFE,0xF1,0xFE,0xF1}};

const DWORD des_skb[8][64]={
	//  for C bits (numbered as per FIPS 46) 1 2 3 4 5 6
   {0x00000000, 0x00000010, 0x20000000, 0x20000010, 0x00010000, 0x00010010, 0x20010000, 0x20010010,
	0x00000800, 0x00000810, 0x20000800, 0x20000810, 0x00010800, 0x00010810, 0x20010800, 0x20010810,
	0x00000020, 0x00000030, 0x20000020, 0x20000030, 0x00010020, 0x00010030, 0x20010020, 0x20010030,
	0x00000820, 0x00000830, 0x20000820, 0x20000830, 0x00010820, 0x00010830, 0x20010820, 0x20010830,
	0x00080000, 0x00080010, 0x20080000, 0x20080010, 0x00090000, 0x00090010, 0x20090000, 0x20090010,
	0x00080800, 0x00080810, 0x20080800, 0x20080810, 0x00090800, 0x00090810, 0x20090800, 0x20090810,
	0x00080020, 0x00080030, 0x20080020, 0x20080030, 0x00090020, 0x00090030, 0x20090020, 0x20090030,
	0x00080820, 0x00080830, 0x20080820, 0x20080830, 0x00090820, 0x00090830, 0x20090820, 0x20090830},

	//  for C bits (numbered as per FIPS 46) 7 8 10 11 12 13
   {0x00000000, 0x02000000, 0x00002000, 0x02002000, 0x00200000, 0x02200000, 0x00202000, 0x02202000,
	0x00000004, 0x02000004, 0x00002004, 0x02002004, 0x00200004, 0x02200004, 0x00202004, 0x02202004,
	0x00000400, 0x02000400, 0x00002400, 0x02002400, 0x00200400, 0x02200400, 0x00202400, 0x02202400,
	0x00000404, 0x02000404, 0x00002404, 0x02002404, 0x00200404, 0x02200404, 0x00202404, 0x02202404,
	0x10000000, 0x12000000, 0x10002000, 0x12002000, 0x10200000, 0x12200000, 0x10202000, 0x12202000,
	0x10000004, 0x12000004, 0x10002004, 0x12002004, 0x10200004, 0x12200004, 0x10202004, 0x12202004,
	0x10000400, 0x12000400, 0x10002400, 0x12002400, 0x10200400, 0x12200400, 0x10202400, 0x12202400,
	0x10000404, 0x12000404, 0x10002404, 0x12002404, 0x10200404, 0x12200404, 0x10202404, 0x12202404},

	//  for C bits (numbered as per FIPS 46) 14 15 16 17 19 20
   {0x00000000, 0x00000001, 0x00040000, 0x00040001, 0x01000000, 0x01000001, 0x01040000, 0x01040001,
	0x00000002, 0x00000003, 0x00040002, 0x00040003, 0x01000002, 0x01000003, 0x01040002, 0x01040003,
	0x00000200, 0x00000201, 0x00040200, 0x00040201, 0x01000200, 0x01000201, 0x01040200, 0x01040201,
	0x00000202, 0x00000203, 0x00040202, 0x00040203, 0x01000202, 0x01000203, 0x01040202, 0x01040203,
	0x08000000, 0x08000001, 0x08040000, 0x08040001, 0x09000000, 0x09000001, 0x09040000, 0x09040001,
	0x08000002, 0x08000003, 0x08040002, 0x08040003, 0x09000002, 0x09000003, 0x09040002, 0x09040003,
	0x08000200, 0x08000201, 0x08040200, 0x08040201, 0x09000200, 0x09000201, 0x09040200, 0x09040201,
	0x08000202, 0x08000203, 0x08040202, 0x08040203, 0x09000202, 0x09000203, 0x09040202, 0x09040203},

	//  for C bits (numbered as per FIPS 46) 21 23 24 26 27 28
   {0x00000000, 0x00100000, 0x00000100, 0x00100100, 0x00000008, 0x00100008, 0x00000108, 0x00100108,
	0x00001000, 0x00101000, 0x00001100, 0x00101100, 0x00001008, 0x00101008, 0x00001108, 0x00101108,
	0x04000000, 0x04100000, 0x04000100, 0x04100100, 0x04000008, 0x04100008, 0x04000108, 0x04100108,
	0x04001000, 0x04101000, 0x04001100, 0x04101100, 0x04001008, 0x04101008, 0x04001108, 0x04101108,
	0x00020000, 0x00120000, 0x00020100, 0x00120100, 0x00020008, 0x00120008, 0x00020108, 0x00120108,
	0x00021000, 0x00121000, 0x00021100, 0x00121100, 0x00021008, 0x00121008, 0x00021108, 0x00121108,
	0x04020000, 0x04120000, 0x04020100, 0x04120100, 0x04020008, 0x04120008, 0x04020108, 0x04120108,
	0x04021000, 0x04121000, 0x04021100, 0x04121100, 0x04021008, 0x04121008, 0x04021108, 0x04121108},

	//  for D bits (numbered as per FIPS 46) 1 2 3 4 5 6
   {0x00000000, 0x10000000, 0x00010000, 0x10010000, 0x00000004, 0x10000004, 0x00010004, 0x10010004,
	0x20000000, 0x30000000, 0x20010000, 0x30010000, 0x20000004, 0x30000004, 0x20010004, 0x30010004,
	0x00100000, 0x10100000, 0x00110000, 0x10110000, 0x00100004, 0x10100004, 0x00110004, 0x10110004,
	0x20100000, 0x30100000, 0x20110000, 0x30110000, 0x20100004, 0x30100004, 0x20110004, 0x30110004,
	0x00001000, 0x10001000, 0x00011000, 0x10011000, 0x00001004, 0x10001004, 0x00011004, 0x10011004,
	0x20001000, 0x30001000, 0x20011000, 0x30011000, 0x20001004, 0x30001004, 0x20011004, 0x30011004,
	0x00101000, 0x10101000, 0x00111000, 0x10111000, 0x00101004, 0x10101004, 0x00111004, 0x10111004,
	0x20101000, 0x30101000, 0x20111000, 0x30111000, 0x20101004, 0x30101004, 0x20111004, 0x30111004},

	//  for D bits (numbered as per FIPS 46) 8 9 11 12 13 14
   {0x00000000, 0x08000000, 0x00000008, 0x08000008, 0x00000400, 0x08000400, 0x00000408, 0x08000408,
	0x00020000, 0x08020000, 0x00020008, 0x08020008, 0x00020400, 0x08020400, 0x00020408, 0x08020408,
	0x00000001, 0x08000001, 0x00000009, 0x08000009, 0x00000401, 0x08000401, 0x00000409, 0x08000409,
	0x00020001, 0x08020001, 0x00020009, 0x08020009, 0x00020401, 0x08020401, 0x00020409, 0x08020409,
	0x02000000, 0x0A000000, 0x02000008, 0x0A000008, 0x02000400, 0x0A000400, 0x02000408, 0x0A000408,
	0x02020000, 0x0A020000, 0x02020008, 0x0A020008, 0x02020400, 0x0A020400, 0x02020408, 0x0A020408,
	0x02000001, 0x0A000001, 0x02000009, 0x0A000009, 0x02000401, 0x0A000401, 0x02000409, 0x0A000409,
	0x02020001, 0x0A020001, 0x02020009, 0x0A020009, 0x02020401, 0x0A020401, 0x02020409, 0x0A020409},

	//  for D bits (numbered as per FIPS 46) 16 17 18 19 20 21
   {0x00000000, 0x00000100, 0x00080000, 0x00080100, 0x01000000, 0x01000100, 0x01080000, 0x01080100,
	0x00000010, 0x00000110, 0x00080010, 0x00080110, 0x01000010, 0x01000110, 0x01080010, 0x01080110,
	0x00200000, 0x00200100, 0x00280000, 0x00280100, 0x01200000, 0x01200100, 0x01280000, 0x01280100,
	0x00200010, 0x00200110, 0x00280010, 0x00280110, 0x01200010, 0x01200110, 0x01280010, 0x01280110,
	0x00000200, 0x00000300, 0x00080200, 0x00080300, 0x01000200, 0x01000300, 0x01080200, 0x01080300,
	0x00000210, 0x00000310, 0x00080210, 0x00080310, 0x01000210, 0x01000310, 0x01080210, 0x01080310,
	0x00200200, 0x00200300, 0x00280200, 0x00280300, 0x01200200, 0x01200300, 0x01280200, 0x01280300,
	0x00200210, 0x00200310, 0x00280210, 0x00280310, 0x01200210, 0x01200310, 0x01280210, 0x01280310},

	//  for D bits (numbered as per FIPS 46) 22 23 24 25 27 28
   {0x00000000, 0x04000000, 0x00040000, 0x04040000, 0x00000002, 0x04000002, 0x00040002, 0x04040002,
	0x00002000, 0x04002000, 0x00042000, 0x04042000, 0x00002002, 0x04002002, 0x00042002, 0x04042002,
	0x00000020, 0x04000020, 0x00040020, 0x04040020, 0x00000022, 0x04000022, 0x00040022, 0x04040022,
	0x00002020, 0x04002020, 0x00042020, 0x04042020, 0x00002022, 0x04002022, 0x00042022, 0x04042022,
	0x00000800, 0x04000800, 0x00040800, 0x04040800, 0x00000802, 0x04000802, 0x00040802, 0x04040802,
	0x00002800, 0x04002800, 0x00042800, 0x04042800, 0x00002802, 0x04002802, 0x00042802, 0x04042802,
	0x00000820, 0x04000820, 0x00040820, 0x04040820, 0x00000822, 0x04000822, 0x00040822, 0x04040822,
	0x00002820, 0x04002820, 0x00042820, 0x04042820, 0x00002822, 0x04002822, 0x00042822, 0x04042822}
};

const DWORD des_SPtrans[8][64]={
	//  nibble 0
   {0x02080800, 0x00080000, 0x02000002, 0x02080802, 0x02000000, 0x00080802, 0x00080002, 0x02000002,
	0x00080802, 0x02080800, 0x02080000, 0x00000802, 0x02000802, 0x02000000, 0x00000000, 0x00080002,
	0x00080000, 0x00000002, 0x02000800, 0x00080800, 0x02080802, 0x02080000, 0x00000802, 0x02000800,
	0x00000002, 0x00000800, 0x00080800, 0x02080002, 0x00000800, 0x02000802, 0x02080002, 0x00000000,
	0x00000000, 0x02080802, 0x02000800, 0x00080002, 0x02080800, 0x00080000, 0x00000802, 0x02000800,
	0x02080002, 0x00000800, 0x00080800, 0x02000002, 0x00080802, 0x00000002, 0x02000002, 0x02080000,
	0x02080802, 0x00080800, 0x02080000, 0x02000802, 0x02000000, 0x00000802, 0x00080002, 0x00000000,
	0x00080000, 0x02000000, 0x02000802, 0x02080800, 0x00000002, 0x02080002, 0x00000800, 0x00080802},

	//  nibble 1
   {0x40108010, 0x00000000, 0x00108000, 0x40100000, 0x40000010, 0x00008010, 0x40008000, 0x00108000,
	0x00008000, 0x40100010, 0x00000010, 0x40008000, 0x00100010, 0x40108000, 0x40100000, 0x00000010,
	0x00100000, 0x40008010, 0x40100010, 0x00008000, 0x00108010, 0x40000000, 0x00000000, 0x00100010,
	0x40008010, 0x00108010, 0x40108000, 0x40000010, 0x40000000, 0x00100000, 0x00008010, 0x40108010,
	0x00100010, 0x40108000, 0x40008000, 0x00108010, 0x40108010, 0x00100010, 0x40000010, 0x00000000,
	0x40000000, 0x00008010, 0x00100000, 0x40100010, 0x00008000, 0x40000000, 0x00108010, 0x40008010,
	0x40108000, 0x00008000, 0x00000000, 0x40000010, 0x00000010, 0x40108010, 0x00108000, 0x40100000,
	0x40100010, 0x00100000, 0x00008010, 0x40008000, 0x40008010, 0x00000010, 0x40100000, 0x00108000},

	//  nibble 2
   {0x04000001, 0x04040100, 0x00000100, 0x04000101, 0x00040001, 0x04000000, 0x04000101, 0x00040100,
	0x04000100, 0x00040000, 0x04040000, 0x00000001, 0x04040101, 0x00000101, 0x00000001, 0x04040001,
	0x00000000, 0x00040001, 0x04040100, 0x00000100, 0x00000101, 0x04040101, 0x00040000, 0x04000001,
	0x04040001, 0x04000100, 0x00040101, 0x04040000, 0x00040100, 0x00000000, 0x04000000, 0x00040101,
	0x04040100, 0x00000100, 0x00000001, 0x00040000, 0x00000101, 0x00040001, 0x04040000, 0x04000101,
	0x00000000, 0x04040100, 0x00040100, 0x04040001, 0x00040001, 0x04000000, 0x04040101, 0x00000001,
	0x00040101, 0x04000001, 0x04000000, 0x04040101, 0x00040000, 0x04000100, 0x04000101, 0x00040100,
	0x04000100, 0x00000000, 0x04040001, 0x00000101, 0x04000001, 0x00040101, 0x00000100, 0x04040000},

	//  nibble 3
   {0x00401008, 0x10001000, 0x00000008, 0x10401008, 0x00000000, 0x10400000, 0x10001008, 0x00400008,
	0x10401000, 0x10000008, 0x10000000, 0x00001008, 0x10000008, 0x00401008, 0x00400000, 0x10000000,
	0x10400008, 0x00401000, 0x00001000, 0x00000008, 0x00401000, 0x10001008, 0x10400000, 0x00001000,
	0x00001008, 0x00000000, 0x00400008, 0x10401000, 0x10001000, 0x10400008, 0x10401008, 0x00400000,
	0x10400008, 0x00001008, 0x00400000, 0x10000008, 0x00401000, 0x10001000, 0x00000008, 0x10400000,
	0x10001008, 0x00000000, 0x00001000, 0x00400008, 0x00000000, 0x10400008, 0x10401000, 0x00001000,
	0x10000000, 0x10401008, 0x00401008, 0x00400000, 0x10401008, 0x00000008, 0x10001000, 0x00401008,
	0x00400008, 0x00401000, 0x10400000, 0x10001008, 0x00001008, 0x10000000, 0x10000008, 0x10401000},

	//  nibble 4
   {0x08000000, 0x00010000, 0x00000400, 0x08010420, 0x08010020, 0x08000400, 0x00010420, 0x08010000,
	0x00010000, 0x00000020, 0x08000020, 0x00010400, 0x08000420, 0x08010020, 0x08010400, 0x00000000,
	0x00010400, 0x08000000, 0x00010020, 0x00000420, 0x08000400, 0x00010420, 0x00000000, 0x08000020,
	0x00000020, 0x08000420, 0x08010420, 0x00010020, 0x08010000, 0x00000400, 0x00000420, 0x08010400,
	0x08010400, 0x08000420, 0x00010020, 0x08010000, 0x00010000, 0x00000020, 0x08000020, 0x08000400,
	0x08000000, 0x00010400, 0x08010420, 0x00000000, 0x00010420, 0x08000000, 0x00000400, 0x00010020,
	0x08000420, 0x00000400, 0x00000000, 0x08010420, 0x08010020, 0x08010400, 0x00000420, 0x00010000,
	0x00010400, 0x08010020, 0x08000400, 0x00000420, 0x00000020, 0x00010420, 0x08010000, 0x08000020},

	//  nibble 5
   {0x80000040, 0x00200040, 0x00000000, 0x80202000, 0x00200040, 0x00002000, 0x80002040, 0x00200000,
	0x00002040, 0x80202040, 0x00202000, 0x80000000, 0x80002000, 0x80000040, 0x80200000, 0x00202040,
	0x00200000, 0x80002040, 0x80200040, 0x00000000, 0x00002000, 0x00000040, 0x80202000, 0x80200040,
	0x80202040, 0x80200000, 0x80000000, 0x00002040, 0x00000040, 0x00202000, 0x00202040, 0x80002000,
	0x00002040, 0x80000000, 0x80002000, 0x00202040, 0x80202000, 0x00200040, 0x00000000, 0x80002000,
	0x80000000, 0x00002000, 0x80200040, 0x00200000, 0x00200040, 0x80202040, 0x00202000, 0x00000040,
	0x80202040, 0x00202000, 0x00200000, 0x80002040, 0x80000040, 0x80200000, 0x00202040, 0x00000000,
	0x00002000, 0x80000040, 0x80002040, 0x80202000, 0x80200000, 0x00002040, 0x00000040, 0x80200040},

	//  nibble 6
   {0x00004000, 0x00000200, 0x01000200, 0x01000004, 0x01004204, 0x00004004, 0x00004200, 0x00000000,
	0x01000000, 0x01000204, 0x00000204, 0x01004000, 0x00000004, 0x01004200, 0x01004000, 0x00000204,
	0x01000204, 0x00004000, 0x00004004, 0x01004204, 0x00000000, 0x01000200, 0x01000004, 0x00004200,
	0x01004004, 0x00004204, 0x01004200, 0x00000004, 0x00004204, 0x01004004, 0x00000200, 0x01000000,
	0x00004204, 0x01004000, 0x01004004, 0x00000204, 0x00004000, 0x00000200, 0x01000000, 0x01004004,
	0x01000204, 0x00004204, 0x00004200, 0x00000000, 0x00000200, 0x01000004, 0x00000004, 0x01000200,
	0x00000000, 0x01000204, 0x01000200, 0x00004200, 0x00000204, 0x00004000, 0x01004204, 0x01000000,
	0x01004200, 0x00000004, 0x00004004, 0x01004204, 0x01000004, 0x01004200, 0x01004000, 0x00004004},

	//  nibble 7
   {0x20800080, 0x20820000, 0x00020080, 0x00000000, 0x20020000, 0x00800080, 0x20800000, 0x20820080,
	0x00000080, 0x20000000, 0x00820000, 0x00020080, 0x00820080, 0x20020080, 0x20000080, 0x20800000,
	0x00020000, 0x00820080, 0x00800080, 0x20020000, 0x20820080, 0x20000080, 0x00000000, 0x00820000,
	0x20000000, 0x00800000, 0x20020080, 0x20800080, 0x00800000, 0x00020000, 0x20820000, 0x00000080,
	0x00800000, 0x00020000, 0x20000080, 0x20820080, 0x00020080, 0x20000000, 0x00000000, 0x00820000,
	0x20800080, 0x20020080, 0x20020000, 0x00800080, 0x20820000, 0x00000080, 0x00800080, 0x20020000,
	0x20820080, 0x00800000, 0x20800000, 0x20000080, 0x00820000, 0x00020080, 0x20020080, 0x20800000,
	0x00000080, 0x20820000, 0x00820080, 0x00000000, 0x20000000, 0x20800080, 0x00020000, 0x00820080}
};

/*************** END OF FILE **********************************************/
