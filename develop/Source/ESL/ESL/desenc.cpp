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
			(C-source file) Block Cipher DES - mode function

C0000 : Created by Hyo Sun Hwang (hyosun@future.co.kr) 2000/12/31

C0001 : Modified by Hyo Sun Hwang (hyosun@future.co.kr) 2000/00/00

\*------------------------------------------------------------------------*/

#include "stdafx.h"
/*************** Header files *********************************************/
#include <stdio.h>
#include <time.h>
#include "des.h"

/*************** Assertions ***********************************************/

/*************** Definitions / Macros  ************************************/
#define BlockCopy(pbDst, pbSrc) {					\
	((DWORD *)(pbDst))[0] = ((DWORD *)(pbSrc))[0];	\
	((DWORD *)(pbDst))[1] = ((DWORD *)(pbSrc))[1];	\
}
#define BlockXor(pbDst, phSrc1, phSrc2) {			\
	((DWORD *)(pbDst))[0] = ((DWORD *)(phSrc1))[0]	\
						  ^ ((DWORD *)(phSrc2))[0];	\
	((DWORD *)(pbDst))[1] = ((DWORD *)(phSrc1))[1]	\
						  ^ ((DWORD *)(phSrc2))[1];	\
}

/*************** New Data Types *******************************************/

/*************** Global Variables *****************************************/

/*************** Prototypes ***********************************************/
void	DES_Encrypt(
		void		*CipherKey,		//	암/복호용 Round Key
		BYTE		*Data);			//	입출력을 위한 블록을 가리키는 pointer
void	DES_Decrypt(
		void		*CipherKey,		//	암/복호용 Round Key
		BYTE		*Data);			//	입출력을 위한 블록을 가리키는 pointer

/*************** Constants ************************************************/

/*************** Constants ************************************************/

/*************** Macros ***************************************************/

/*************** Global Variables *****************************************/

/*************** Function *************************************************
*
*/
void	DES_SetAlgInfo(
		DWORD			ModeID,
		DWORD			PadType,
		BYTE			*IV,
		DES_ALG_INFO	*AlgInfo)
{
	AlgInfo->ModeID = ModeID;
	AlgInfo->PadType = PadType;

	if( IV!=NULL )
		memcpy(AlgInfo->IV, IV, DES_BLOCK_LEN);
	else
		memset(AlgInfo->IV, 0, DES_BLOCK_LEN);
}

/*************** Function *************************************************
*
*/
static RET_VAL PaddSet(
			BYTE	*pbOutBuffer,
			DWORD	dRmdLen,
			DWORD	dBlockLen,
			DWORD	dPaddingType)
{
	DWORD dPadLen;

	switch( dPaddingType ) {
		case AI_NO_PADDING :
			if( dRmdLen==0 )	return 0;
			else				return CTR_DATA_LEN_ERROR;

		case AI_PKCS_PADDING :
			dPadLen = dBlockLen - dRmdLen;
			memset(pbOutBuffer+dRmdLen, (char)dPadLen, (int)dPadLen);
			return dPadLen;

		default :
			return CTR_FATAL_ERROR;
	}
}

/*************** Function *************************************************
*
*/
static RET_VAL PaddCheck(
			BYTE	*pbOutBuffer,
			DWORD	dBlockLen,
			DWORD	dPaddingType)
{
	DWORD i, dPadLen;

	switch( dPaddingType ) {
		case AI_NO_PADDING :
			return 0;			//	padding된 데이타가 0바이트임.

		case AI_PKCS_PADDING :
			dPadLen = pbOutBuffer[dBlockLen-1];
			if( ((int)dPadLen<=0) || (dPadLen>(int)dBlockLen) )
				return CTR_PAD_CHECK_ERROR;
			for( i=1; i<=dPadLen; i++)
				if( pbOutBuffer[dBlockLen-i] != dPadLen )
					return CTR_PAD_CHECK_ERROR;
			return dPadLen;

		default :
			return CTR_FATAL_ERROR;
	}
}

/**************************************************************************
*
*/
RET_VAL	DES_EncInit(
		DES_ALG_INFO	*AlgInfo)
{
	AlgInfo->BufLen = 0;
	if( AlgInfo->ModeID!=AI_ECB )
		memcpy(AlgInfo->ChainVar, AlgInfo->IV, DES_BLOCK_LEN);
	return CTR_SUCCESS;
}

/**************************************************************************
*
*/
static RET_VAL ECB_EncUpdate(
		DES_ALG_INFO	*AlgInfo,		//	
		BYTE		*PlainTxt,		//	입력되는 평문의 pointer
		DWORD		PlainTxtLen,	//	입력되는 평문의 바이트 수
		BYTE		*CipherTxt, 	//	암호문이 출력될 pointer
		DWORD		*CipherTxtLen)	//	출력되는 암호문의 바이트 수
{
	DWORD		*ScheduledKey=AlgInfo->RoundKey;
	DWORD		BlockLen=DES_BLOCK_LEN, BufLen=AlgInfo->BufLen;

	//
	*CipherTxtLen = BufLen + PlainTxtLen;

	//	No one block
	if( *CipherTxtLen<BlockLen ) {
		memcpy(AlgInfo->Buffer+BufLen, PlainTxt, (int)PlainTxtLen);
		AlgInfo->BufLen += PlainTxtLen;
		*CipherTxtLen = 0;
		return CTR_SUCCESS;
	}

	//	control the case that PlainTxt and CipherTxt are the same buffer
	if( PlainTxt==CipherTxt )
		return CTR_FATAL_ERROR;

	//	first block
	memcpy(AlgInfo->Buffer+BufLen, PlainTxt, (int)(BlockLen - BufLen));
	PlainTxt += BlockLen - BufLen;
	PlainTxtLen -= BlockLen - BufLen;

	//	core part
	BlockCopy(CipherTxt, AlgInfo->Buffer);
	DES_Encrypt(ScheduledKey, CipherTxt);
	CipherTxt += BlockLen;
	while( PlainTxtLen>=BlockLen ) {
		BlockCopy(CipherTxt, PlainTxt);
		DES_Encrypt(ScheduledKey, CipherTxt);
		PlainTxt += BlockLen;
		CipherTxt += BlockLen;
		PlainTxtLen -= BlockLen;
	}

	//	save remained data
	memcpy(AlgInfo->Buffer, PlainTxt, (int)PlainTxtLen);
	AlgInfo->BufLen = PlainTxtLen;
	*CipherTxtLen -= PlainTxtLen;

	//	control the case that PlainTxt and CipherTxt are the same buffer
	return CTR_SUCCESS;
}

/**************************************************************************
*
*/
static RET_VAL CBC_EncUpdate(
		DES_ALG_INFO	*AlgInfo,		//	
		BYTE		*PlainTxt,		//	입력되는 평문의 pointer
		DWORD		PlainTxtLen,	//	입력되는 평문의 바이트 수
		BYTE		*CipherTxt, 	//	암호문이 출력될 pointer
		DWORD		*CipherTxtLen)	//	출력되는 암호문의 바이트 수
{
	DWORD		*ScheduledKey=AlgInfo->RoundKey;
	DWORD		BlockLen=DES_BLOCK_LEN, BufLen=AlgInfo->BufLen;

	//
	*CipherTxtLen = BufLen + PlainTxtLen;

	//	No one block
	if( *CipherTxtLen<BlockLen ) {
		memcpy(AlgInfo->Buffer+BufLen, PlainTxt, (int)PlainTxtLen);
		AlgInfo->BufLen += PlainTxtLen;
		*CipherTxtLen = 0;
		return CTR_SUCCESS;
	}

	//	control the case that PlainTxt and CipherTxt are the same buffer
	if( PlainTxt==CipherTxt )
		return CTR_FATAL_ERROR;

	//	first block
	memcpy(AlgInfo->Buffer+BufLen, PlainTxt, (int)(BlockLen - BufLen));
	PlainTxt += BlockLen - BufLen;
	PlainTxtLen -= BlockLen - BufLen;

	//	core part
	BlockXor(CipherTxt, AlgInfo->ChainVar, AlgInfo->Buffer);
	DES_Encrypt(ScheduledKey, CipherTxt);
	CipherTxt += BlockLen;
	while( PlainTxtLen>=BlockLen ) {
		BlockXor(CipherTxt, CipherTxt-BlockLen, PlainTxt);
		DES_Encrypt(ScheduledKey, CipherTxt);
		PlainTxt += BlockLen;
		CipherTxt += BlockLen;
		PlainTxtLen -= BlockLen;
	}
	BlockCopy(AlgInfo->ChainVar, CipherTxt-BlockLen);

	//	save remained data
	memcpy(AlgInfo->Buffer, PlainTxt, (int)PlainTxtLen);
	AlgInfo->BufLen = PlainTxtLen;
	*CipherTxtLen -= PlainTxtLen;

	//
	return CTR_SUCCESS;
}

/**************************************************************************
*
*/
static RET_VAL OFB_EncUpdate(
		DES_ALG_INFO	*AlgInfo,		//	
		BYTE		*PlainTxt,		//	입력되는 평문의 pointer
		DWORD		PlainTxtLen,	//	입력되는 평문의 바이트 수
		BYTE		*CipherTxt, 	//	암호문이 출력될 pointer
		DWORD		*CipherTxtLen)	//	출력되는 암호문의 바이트 수
{
	DWORD		*ScheduledKey=AlgInfo->RoundKey;
	DWORD		BlockLen=DES_BLOCK_LEN;
	DWORD		BufLen=AlgInfo->BufLen;

	//	Check Output Memory Size
	*CipherTxtLen = BufLen + PlainTxtLen;

	//	No one block
	if( *CipherTxtLen<BlockLen ) {
		memcpy(AlgInfo->Buffer+BufLen, PlainTxt, (int)PlainTxtLen);
		AlgInfo->BufLen += PlainTxtLen;
		*CipherTxtLen = 0;
		return CTR_SUCCESS;
	}

	//	control the case that PlainTxt and CipherTxt are the same buffer
	if( PlainTxt==CipherTxt )
		return CTR_FATAL_ERROR;

	//	first block
	memcpy(AlgInfo->Buffer+BufLen, PlainTxt, (int)(BlockLen - BufLen));
	PlainTxt += BlockLen - BufLen;
	PlainTxtLen -= BlockLen - BufLen;

	//	core part
	DES_Encrypt(ScheduledKey, AlgInfo->ChainVar);
	BlockXor(CipherTxt, AlgInfo->ChainVar, AlgInfo->Buffer);
	CipherTxt += BlockLen;
	while( PlainTxtLen>=BlockLen ) {
		DES_Encrypt(ScheduledKey, AlgInfo->ChainVar);
		BlockXor(CipherTxt, AlgInfo->ChainVar, PlainTxt);
		PlainTxt += BlockLen;
		CipherTxt += BlockLen;
		PlainTxtLen -= BlockLen;
	}

	//	save remained data
	memcpy(AlgInfo->Buffer, PlainTxt, (int)PlainTxtLen);
	AlgInfo->BufLen = (AlgInfo->BufLen&0xF0000000) + PlainTxtLen;
	*CipherTxtLen -= PlainTxtLen;

	//
	return CTR_SUCCESS;
}

/**************************************************************************
*
*/
static RET_VAL CFB_EncUpdate(
		DES_ALG_INFO	*AlgInfo,		//	
		BYTE		*PlainTxt,		//	입력되는 평문의 pointer
		DWORD		PlainTxtLen,	//	입력되는 평문의 바이트 수
		BYTE		*CipherTxt, 	//	암호문이 출력될 pointer
		DWORD		*CipherTxtLen)	//	출력되는 암호문의 바이트 수
{
	DWORD		*ScheduledKey=AlgInfo->RoundKey;
	DWORD		BlockLen=DES_BLOCK_LEN;
	DWORD		BufLen=AlgInfo->BufLen;

	//	Check Output Memory Size
	*CipherTxtLen = BufLen + PlainTxtLen;

	//	No one block
	if( *CipherTxtLen<BlockLen ) {
		memcpy(AlgInfo->Buffer+BufLen, PlainTxt, (int)PlainTxtLen);
		AlgInfo->BufLen += PlainTxtLen;
		*CipherTxtLen = 0;
		return CTR_SUCCESS;
	}

	//	control the case that PlainTxt and CipherTxt are the same buffer
	if( PlainTxt==CipherTxt )
		return CTR_FATAL_ERROR;

	//	first block
	memcpy(AlgInfo->Buffer+BufLen, PlainTxt, (int)(BlockLen - BufLen));
	PlainTxt += BlockLen - BufLen;
	PlainTxtLen -= BlockLen - BufLen;

	//	core part
	DES_Encrypt(ScheduledKey, AlgInfo->ChainVar);
	BlockXor(AlgInfo->ChainVar, AlgInfo->ChainVar, AlgInfo->Buffer);
	BlockCopy(CipherTxt, AlgInfo->ChainVar);
	CipherTxt += BlockLen;
	while( PlainTxtLen>=BlockLen ) {
		DES_Encrypt(ScheduledKey, AlgInfo->ChainVar);
		BlockXor(AlgInfo->ChainVar, AlgInfo->ChainVar, PlainTxt);
		BlockCopy(CipherTxt, AlgInfo->ChainVar);
		PlainTxt += BlockLen;
		CipherTxt += BlockLen;
		PlainTxtLen -= BlockLen;
	}

	//	save remained data
	memcpy(AlgInfo->Buffer, PlainTxt, (int)PlainTxtLen);
	AlgInfo->BufLen = (AlgInfo->BufLen&0xF0000000) + PlainTxtLen;
	*CipherTxtLen -= PlainTxtLen;

	//
	return CTR_SUCCESS;
}

/**************************************************************************
*
*/
RET_VAL	DES_EncUpdate(
		DES_ALG_INFO	*AlgInfo,
		BYTE		*PlainTxt,		//	입력되는 평문의 pointer
		DWORD		PlainTxtLen,	//	입력되는 평문의 바이트 수
		BYTE		*CipherTxt, 	//	암호문이 출력될 pointer
		DWORD		*CipherTxtLen)	//	출력되는 암호문의 바이트 수
{
	switch( AlgInfo->ModeID ) {
		case AI_ECB :	return ECB_EncUpdate(AlgInfo, PlainTxt, PlainTxtLen,
											 CipherTxt, CipherTxtLen);
		case AI_CBC :	return CBC_EncUpdate(AlgInfo, PlainTxt, PlainTxtLen,
											 CipherTxt, CipherTxtLen);
		case AI_OFB :	return OFB_EncUpdate(AlgInfo, PlainTxt, PlainTxtLen,
											 CipherTxt, CipherTxtLen);
		case AI_CFB :	return CFB_EncUpdate(AlgInfo, PlainTxt, PlainTxtLen,
											 CipherTxt, CipherTxtLen);
		default :		return CTR_FATAL_ERROR;
	}
}

/**************************************************************************
*
*/
static RET_VAL ECB_EncFinal(
		DES_ALG_INFO	*AlgInfo,		//	
		BYTE		*CipherTxt, 	//	암호문이 출력될 pointer
		DWORD		*CipherTxtLen)	//	출력되는 암호문의 바이트 수
{
	DWORD		*ScheduledKey=AlgInfo->RoundKey;
	DWORD		BlockLen=DES_BLOCK_LEN, BufLen=AlgInfo->BufLen;
	DWORD		PaddByte;

	//	Padding
	PaddByte = PaddSet(AlgInfo->Buffer, BufLen, BlockLen, AlgInfo->PadType);
	if( PaddByte>BlockLen )		return PaddByte;

	if( PaddByte==0 ) {
		*CipherTxtLen = 0;
		return CTR_SUCCESS;
	}

	//	core part
	BlockCopy(CipherTxt, AlgInfo->Buffer);
	DES_Encrypt(ScheduledKey, CipherTxt);

	//
	*CipherTxtLen = BlockLen;

	//
	return CTR_SUCCESS;
}

/**************************************************************************
*
*/
static RET_VAL CBC_EncFinal(
		DES_ALG_INFO	*AlgInfo,
		BYTE		*CipherTxt, 	//	암호문이 출력될 pointer
		DWORD		*CipherTxtLen)	//	출력되는 암호문의 바이트 수
{
	DWORD		*ScheduledKey=AlgInfo->RoundKey;
	DWORD		BlockLen=DES_BLOCK_LEN, BufLen=AlgInfo->BufLen;
	DWORD		PaddByte;

	//	Padding
	PaddByte = PaddSet(AlgInfo->Buffer, BufLen, BlockLen, AlgInfo->PadType);
	if( PaddByte>BlockLen )		return PaddByte;

	if( PaddByte==0 ) {
		*CipherTxtLen = 0;
		return CTR_SUCCESS;
	}

	//	core part
	BlockXor(CipherTxt, AlgInfo->Buffer, AlgInfo->ChainVar);
	DES_Encrypt(ScheduledKey, CipherTxt);
	BlockCopy(AlgInfo->ChainVar, CipherTxt);

	//
	*CipherTxtLen = BlockLen;

	//
	return CTR_SUCCESS;
}

/**************************************************************************
*
*/
static RET_VAL OFB_EncFinal(
		DES_ALG_INFO	*AlgInfo,
		BYTE		*CipherTxt, 	//	암호문이 출력될 pointer
		DWORD		*CipherTxtLen)	//	출력되는 암호문의 바이트 수
{
	DWORD		*ScheduledKey=AlgInfo->RoundKey;
	DWORD		BlockLen=DES_BLOCK_LEN;
	DWORD		BufLen=AlgInfo->BufLen;
	DWORD		i;

	//	Check Output Memory Size
	*CipherTxtLen = BlockLen;

	//	core part
	DES_Encrypt(ScheduledKey, AlgInfo->ChainVar);
	for( i=0; i<BufLen; i++)
		CipherTxt[i] = (BYTE) (AlgInfo->Buffer[i] ^ AlgInfo->ChainVar[i]);

	//
	*CipherTxtLen = BufLen;

	//
	return CTR_SUCCESS;
}

/**************************************************************************
*
*/
static RET_VAL CFB_EncFinal(
		DES_ALG_INFO	*AlgInfo,
		BYTE		*CipherTxt, 	//	암호문이 출력될 pointer
		DWORD		*CipherTxtLen)	//	출력되는 암호문의 바이트 수
{
	DWORD		*ScheduledKey=AlgInfo->RoundKey;
	DWORD		BufLen=AlgInfo->BufLen;

	//	Check Output Memory Size
	*CipherTxtLen = BufLen;

	//	core part
	DES_Encrypt(ScheduledKey, AlgInfo->ChainVar);
	BlockXor(AlgInfo->ChainVar, AlgInfo->ChainVar, AlgInfo->Buffer);
	memcpy(CipherTxt, AlgInfo->ChainVar, BufLen);

	//
	*CipherTxtLen = BufLen;

	//
	return CTR_SUCCESS;
}

/**************************************************************************
*
*/
RET_VAL	DES_EncFinal(
		DES_ALG_INFO	*AlgInfo,
		BYTE		*CipherTxt, 	//	암호문이 출력될 pointer
		DWORD		*CipherTxtLen)	//	출력되는 암호문의 바이트 수
{
	switch( AlgInfo->ModeID ) {
		case AI_ECB :	return ECB_EncFinal(AlgInfo, CipherTxt, CipherTxtLen);
		case AI_CBC :	return CBC_EncFinal(AlgInfo, CipherTxt, CipherTxtLen);
		case AI_OFB :	return OFB_EncFinal(AlgInfo, CipherTxt, CipherTxtLen);
		case AI_CFB :	return CFB_EncFinal(AlgInfo, CipherTxt, CipherTxtLen);
		default :		return CTR_FATAL_ERROR;
	}
}

/**************************************************************************
*
*/
RET_VAL	DES_DecInit(
		DES_ALG_INFO	*AlgInfo)
{
	AlgInfo->BufLen = 0;
	if( AlgInfo->ModeID!=AI_ECB )
		memcpy(AlgInfo->ChainVar, AlgInfo->IV, DES_BLOCK_LEN);
	return CTR_SUCCESS;
}

/**************************************************************************
*
*/
static RET_VAL ECB_DecUpdate(
		DES_ALG_INFO	*AlgInfo,
		BYTE		*CipherTxt, 	//	입력되는 암호문의 pointer
		DWORD		CipherTxtLen,	//	입력되는 암호문의 바이트 수
		BYTE		*PlainTxt,		//	평문이 출력될 pointer
		DWORD		*PlainTxtLen)	//	출력되는 평문의 바이트 수
{
	DWORD		*ScheduledKey=AlgInfo->RoundKey;
	DWORD		BlockLen=DES_BLOCK_LEN;
	DWORD		BufLen=AlgInfo->BufLen;

	//
	*PlainTxtLen = BufLen + CipherTxtLen;

	//	No one block
	if( BufLen+CipherTxtLen <= BlockLen ) {
		memcpy(AlgInfo->Buffer+BufLen, CipherTxt, (int)CipherTxtLen);
		AlgInfo->BufLen += CipherTxtLen;
		*PlainTxtLen = 0;
		return CTR_SUCCESS;
	}

	//	control the case that CipherTxt and PlainTxt are the same buffer
	if( CipherTxt==PlainTxt )	return CTR_FATAL_ERROR;

	//	first block
	*PlainTxtLen = BufLen + CipherTxtLen;
	memcpy(AlgInfo->Buffer+BufLen, CipherTxt, (int)(BlockLen - BufLen));
	CipherTxt += BlockLen - BufLen;
	CipherTxtLen -= BlockLen - BufLen;

	//	core part
	BlockCopy(PlainTxt, AlgInfo->Buffer);
	DES_Decrypt(ScheduledKey, PlainTxt);
	PlainTxt += BlockLen;
	while( CipherTxtLen>BlockLen ) {
		BlockCopy(PlainTxt, CipherTxt);
		DES_Decrypt(ScheduledKey, PlainTxt);
		CipherTxt += BlockLen;
		PlainTxt += BlockLen;
		CipherTxtLen -= BlockLen;
	}

	//	save remained data
	memcpy(AlgInfo->Buffer, CipherTxt, (int)CipherTxtLen);
	AlgInfo->BufLen = (AlgInfo->BufLen&0xF0000000) + CipherTxtLen;
	*PlainTxtLen -= CipherTxtLen;

	//
	return CTR_SUCCESS;
}

/**************************************************************************
*
*/
static RET_VAL CBC_DecUpdate(
		DES_ALG_INFO	*AlgInfo,
		BYTE		*CipherTxt, 	//	입력되는 암호문의 pointer
		DWORD		CipherTxtLen,	//	입력되는 암호문의 바이트 수
		BYTE		*PlainTxt,		//	평문이 출력될 pointer
		DWORD		*PlainTxtLen)	//	출력되는 평문의 바이트 수
{
	DWORD		*ScheduledKey=AlgInfo->RoundKey;
	DWORD		BlockLen=DES_BLOCK_LEN, BufLen=AlgInfo->BufLen;

	//	Check Output Memory Size
	*PlainTxtLen = BufLen + CipherTxtLen;

	//	No one block
	if( BufLen+CipherTxtLen <= BlockLen ) {
		memcpy(AlgInfo->Buffer+BufLen, CipherTxt, (int)CipherTxtLen);
		AlgInfo->BufLen += CipherTxtLen;
		*PlainTxtLen = 0;
		return CTR_SUCCESS;
	}

	//	control the case that CipherTxt and PlainTxt are the same buffer
	if( CipherTxt==PlainTxt )	return CTR_FATAL_ERROR;

	//	first block
	*PlainTxtLen = BufLen + CipherTxtLen;
	memcpy(AlgInfo->Buffer+BufLen, CipherTxt, (int)(BlockLen - BufLen));
	CipherTxt += BlockLen - BufLen;
	CipherTxtLen -= BlockLen - BufLen;

	//	core part
	BlockCopy(PlainTxt, AlgInfo->Buffer);
	DES_Decrypt(ScheduledKey, PlainTxt);
	BlockXor(PlainTxt, PlainTxt, AlgInfo->ChainVar);
	PlainTxt += BlockLen;
	if( CipherTxtLen>BlockLen ) {
		BlockCopy(PlainTxt, CipherTxt);
		DES_Decrypt(ScheduledKey, PlainTxt);
		BlockXor(PlainTxt, PlainTxt, AlgInfo->Buffer);
		CipherTxt += BlockLen;
		PlainTxt += BlockLen;
		CipherTxtLen -= BlockLen;
	}
	while( CipherTxtLen>BlockLen ) {
		BlockCopy(PlainTxt, CipherTxt);
		DES_Decrypt(ScheduledKey, PlainTxt);
		BlockXor(PlainTxt, PlainTxt, CipherTxt-BlockLen);
		CipherTxt += BlockLen;
		PlainTxt += BlockLen;
		CipherTxtLen -= BlockLen;
	}
	BlockCopy(AlgInfo->ChainVar, CipherTxt-BlockLen);

	//	save remained data
	memcpy(AlgInfo->Buffer, CipherTxt, (int)CipherTxtLen);
	AlgInfo->BufLen = (AlgInfo->BufLen&0xF0000000) + CipherTxtLen;
	*PlainTxtLen -= CipherTxtLen;

	//
	return CTR_SUCCESS;
}

/**************************************************************************
*
*/
static RET_VAL OFB_DecUpdate(
		DES_ALG_INFO	*AlgInfo,
		BYTE		*CipherTxt, 	//	입력되는 암호문의 pointer
		DWORD		CipherTxtLen,	//	입력되는 암호문의 바이트 수
		BYTE		*PlainTxt,		//	평문이 출력될 pointer
		DWORD		*PlainTxtLen)	//	출력되는 평문의 바이트 수
{
	DWORD		*ScheduledKey=AlgInfo->RoundKey;
	DWORD		BlockLen=DES_BLOCK_LEN;
	DWORD		BufLen=AlgInfo->BufLen;

	//	Check Output Memory Size
	*PlainTxtLen = BufLen + CipherTxtLen;

	//	No one block
	if( BufLen+CipherTxtLen <= BlockLen ) {
		memcpy(AlgInfo->Buffer+BufLen, CipherTxt, (int)CipherTxtLen);
		AlgInfo->BufLen += CipherTxtLen;
		*PlainTxtLen = 0;
		return CTR_SUCCESS;
	}

	//	control the case that CipherTxt and PlainTxt are the same buffer
	if( PlainTxt==CipherTxt )
		return CTR_FATAL_ERROR;

	//	first block
	*PlainTxtLen = BufLen + CipherTxtLen;
	memcpy(AlgInfo->Buffer+BufLen, CipherTxt, (int)(BlockLen - BufLen));
	CipherTxt += BlockLen - BufLen;
	CipherTxtLen -= BlockLen - BufLen;

	//	core part
	DES_Encrypt(ScheduledKey, AlgInfo->ChainVar);
	BlockXor(PlainTxt, AlgInfo->ChainVar, AlgInfo->Buffer);
	PlainTxt += BlockLen;
	while( CipherTxtLen>BlockLen ) {
		DES_Encrypt(ScheduledKey, AlgInfo->ChainVar);
		BlockXor(PlainTxt, AlgInfo->ChainVar, CipherTxt);
		CipherTxt += BlockLen;
		PlainTxt += BlockLen;
		CipherTxtLen -= BlockLen;
	}

	//	save remained data
	memcpy(AlgInfo->Buffer, CipherTxt, (int)CipherTxtLen);
	AlgInfo->BufLen = (AlgInfo->BufLen&0xF0000000) + CipherTxtLen;
	*PlainTxtLen -= CipherTxtLen;

	//
	return CTR_SUCCESS;
}

/**************************************************************************
*
*/
static RET_VAL CFB_DecUpdate(
		DES_ALG_INFO	*AlgInfo,
		BYTE		*CipherTxt, 	//	입력되는 암호문의 pointer
		DWORD		CipherTxtLen,	//	입력되는 암호문의 바이트 수
		BYTE		*PlainTxt,		//	평문이 출력될 pointer
		DWORD		*PlainTxtLen)	//	출력되는 평문의 바이트 수
{
	DWORD		*ScheduledKey=AlgInfo->RoundKey;
	DWORD		BlockLen=DES_BLOCK_LEN;
	DWORD		BufLen=AlgInfo->BufLen;

	//	Check Output Memory Size
	*PlainTxtLen = BufLen + CipherTxtLen;

	//	No one block
	if( BufLen+CipherTxtLen <= BlockLen ) {
		memcpy(AlgInfo->Buffer+BufLen, CipherTxt, (int)CipherTxtLen);
		AlgInfo->BufLen += CipherTxtLen;
		*PlainTxtLen = 0;
		return CTR_SUCCESS;
	}

	//	control the case that CipherTxt and PlainTxt are the same buffer
	if( PlainTxt==CipherTxt )
		return CTR_FATAL_ERROR;

	//	first block
	*PlainTxtLen = BufLen + CipherTxtLen;
	memcpy(AlgInfo->Buffer+BufLen, CipherTxt, (int)(BlockLen - BufLen));
	CipherTxt += BlockLen - BufLen;
	CipherTxtLen -= BlockLen - BufLen;

	//	core part
	DES_Encrypt(ScheduledKey, AlgInfo->ChainVar);
	BlockXor(PlainTxt, AlgInfo->ChainVar, AlgInfo->Buffer);
	BlockCopy(AlgInfo->ChainVar, AlgInfo->Buffer);
	PlainTxt += BlockLen;
	while( CipherTxtLen>BlockLen ) {
		DES_Encrypt(ScheduledKey, AlgInfo->ChainVar);
		BlockXor(PlainTxt, AlgInfo->ChainVar, CipherTxt);
		BlockCopy(AlgInfo->ChainVar, CipherTxt);
		CipherTxt += BlockLen;
		PlainTxt += BlockLen;
		CipherTxtLen -= BlockLen;
	}

	//	save remained data
	memcpy(AlgInfo->Buffer, CipherTxt, (int)CipherTxtLen);
	AlgInfo->BufLen = (AlgInfo->BufLen&0xF0000000) + CipherTxtLen;
	*PlainTxtLen -= CipherTxtLen;

	//
	return CTR_SUCCESS;
}

/**************************************************************************
*
*/
RET_VAL	DES_DecUpdate(
		DES_ALG_INFO	*AlgInfo,
		BYTE		*CipherTxt, 	//	암호문이 출력될 pointer
		DWORD		CipherTxtLen,	//	출력되는 암호문의 바이트 수
		BYTE		*PlainTxt,		//	입력되는 평문의 pointer
		DWORD		*PlainTxtLen)	//	입력되는 평문의 바이트 수
{
	switch( AlgInfo->ModeID ) {
		case AI_ECB :	return ECB_DecUpdate(AlgInfo, CipherTxt, CipherTxtLen,
											 PlainTxt, PlainTxtLen);
		case AI_CBC :	return CBC_DecUpdate(AlgInfo, CipherTxt, CipherTxtLen,
											 PlainTxt, PlainTxtLen);
		case AI_OFB :	return OFB_DecUpdate(AlgInfo, CipherTxt, CipherTxtLen,
											 PlainTxt, PlainTxtLen);
		case AI_CFB :	return CFB_DecUpdate(AlgInfo, CipherTxt, CipherTxtLen,
											 PlainTxt, PlainTxtLen);
		default :		return CTR_FATAL_ERROR;
	}
}

/**************************************************************************
*
*/
RET_VAL ECB_DecFinal(
		DES_ALG_INFO	*AlgInfo,
		BYTE		*PlainTxt,		//	평문이 출력될 pointer
		DWORD		*PlainTxtLen)	//	출력되는 평문의 바이트 수
{
	DWORD		*ScheduledKey=AlgInfo->RoundKey;
	DWORD		BlockLen=DES_BLOCK_LEN, BufLen=AlgInfo->BufLen;
	RET_VAL		ret;

	//	Check Output Memory Size
	if( BufLen==0 ) {
		*PlainTxtLen = 0;
		return CTR_SUCCESS;
	}
	*PlainTxtLen = BlockLen;

	if( BufLen!=BlockLen )	return CTR_CIPHER_LEN_ERROR;

	//	core part
	BlockCopy(PlainTxt, AlgInfo->Buffer);
	DES_Decrypt(ScheduledKey, PlainTxt);

	//	Padding Check
	ret = PaddCheck(PlainTxt, BlockLen, AlgInfo->PadType);
	if( ret==(DWORD)-3 )	return CTR_PAD_CHECK_ERROR;
	if( ret==(DWORD)-1 )	return CTR_FATAL_ERROR;

	*PlainTxtLen = BlockLen - ret;

	//
	return CTR_SUCCESS;
}

/**************************************************************************
*
*/
RET_VAL CBC_DecFinal(
		DES_ALG_INFO	*AlgInfo,
		BYTE		*PlainTxt,		//	평문이 출력될 pointer
		DWORD		*PlainTxtLen)	//	출력되는 평문의 바이트 수
{
	DWORD		*ScheduledKey=AlgInfo->RoundKey;
	DWORD		BlockLen=DES_BLOCK_LEN, BufLen=AlgInfo->BufLen;
	RET_VAL		ret;

	//	Check Output Memory Size
	if( BufLen==0 ) {
		*PlainTxtLen = 0;
		return CTR_SUCCESS;
	}
	*PlainTxtLen = BlockLen;

	if( BufLen!=BlockLen )	return CTR_CIPHER_LEN_ERROR;

	//	core part
	BlockCopy(PlainTxt, AlgInfo->Buffer);
	DES_Decrypt(ScheduledKey, PlainTxt);
	BlockXor(PlainTxt, PlainTxt, AlgInfo->ChainVar);
	BlockCopy(AlgInfo->ChainVar, AlgInfo->Buffer);

	//	Padding Check
	ret = PaddCheck(PlainTxt, BlockLen, AlgInfo->PadType);
	if( ret==(DWORD)-3 )	return CTR_PAD_CHECK_ERROR;
	if( ret==(DWORD)-1 )	return CTR_FATAL_ERROR;

	*PlainTxtLen = BlockLen - ret;

	//
	return CTR_SUCCESS;
}

/**************************************************************************
*
*/
RET_VAL OFB_DecFinal(
		DES_ALG_INFO	*AlgInfo,
		BYTE		*PlainTxt,		//	평문이 출력될 pointer
		DWORD		*PlainTxtLen)	//	출력되는 평문의 바이트 수
{
	DWORD		*ScheduledKey=AlgInfo->RoundKey;
	DWORD		i, BufLen=AlgInfo->BufLen;

	//	Check Output Memory Size
	*PlainTxtLen = BufLen;

	//	core part
	DES_Encrypt(ScheduledKey, AlgInfo->ChainVar);
	for( i=0; i<BufLen; i++)
		PlainTxt[i] = (BYTE) (AlgInfo->Buffer[i] ^ AlgInfo->ChainVar[i]);

	*PlainTxtLen = BufLen;

	//
	return CTR_SUCCESS;
}


/**************************************************************************
*
*/
RET_VAL CFB_DecFinal(
		DES_ALG_INFO	*AlgInfo,
		BYTE		*PlainTxt,		//	평문이 출력될 pointer
		DWORD		*PlainTxtLen)	//	출력되는 평문의 바이트 수
{
	DWORD		*ScheduledKey=AlgInfo->RoundKey;
	DWORD		BufLen=AlgInfo->BufLen;

	//	Check Output Memory Size
	*PlainTxtLen = BufLen;

	//	core part
	DES_Encrypt(ScheduledKey, AlgInfo->ChainVar);
	BlockXor(AlgInfo->ChainVar, AlgInfo->ChainVar, AlgInfo->Buffer);
	memcpy(PlainTxt, AlgInfo->ChainVar, BufLen);

	*PlainTxtLen = BufLen;

	//
	return CTR_SUCCESS;
}

/**************************************************************************
*
*/
RET_VAL	DES_DecFinal(
		DES_ALG_INFO	*AlgInfo,
		BYTE		*PlainTxt,		//	입력되는 평문의 pointer
		DWORD		*PlainTxtLen)	//	입력되는 평문의 바이트 수
{
	switch( AlgInfo->ModeID ) {
		case AI_ECB :	return ECB_DecFinal(AlgInfo, PlainTxt, PlainTxtLen);
		case AI_CBC :	return CBC_DecFinal(AlgInfo, PlainTxt, PlainTxtLen);
		case AI_OFB :	return OFB_DecFinal(AlgInfo, PlainTxt, PlainTxtLen);
		case AI_CFB :	return CFB_DecFinal(AlgInfo, PlainTxt, PlainTxtLen);
		default :		return CTR_FATAL_ERROR;
	}
}

/////////////////////////////////////////////////////////////////////////
// DUC 추가
/*************** Assertions ***********************************************/

/*************** Definitions / Macros  ************************************/
#define KILO	1024
#define MEGA	(KILO*KILO)

////	Prine out BYTE data in ascending order and with no '0x'(hexa type)
#undef PrintBYTE
#define PrintBYTE(pfile, msg, Data, DataLen) {			\
	int idx;											\
	fprintf(pfile, "%5s =", msg);						\
	for( idx=0; idx<(int)DataLen; idx++) {				\
		if( (idx==0) || ((idx%16)!=0) )					\
			fprintf(pfile, " %.2x", Data[idx]);			\
		else											\
			fprintf(pfile, "\n\t%.2x", Data[idx]);		\
	}													\
	fprintf(pfile, "\n");								\
}

////	한번에 'ByteLen'-bytes의 데이타를 처리하는 연산 'Oper'을
////	'Iter'번 수행하고, 그 결과를 KByte/sec단위로 출력함.
#define SPEED_TEST(str1, str2, Iter, ByteLen, Oper) {	\
	unsigned idx;										\
	clock_t start, finish;								\
	double Sec, Mbps=0.0;								\
	start = clock();									\
	for( idx=0; idx<Iter; idx++)						\
		{	Oper;	}									\
	finish = clock();									\
	Sec = (double)(finish-start) / CLOCKS_PER_SEC;		\
	if( Sec!=0 )	Mbps = 8.0*ByteLen*idx/Sec/MEGA;	\
	printf("%s%7.3fMbps(=%d*", str1, Mbps, Iter);		\
	printf("%d", ByteLen);								\
	printf("/%3.0f)%s", 1000.0*Sec, str2);				\
}

/*************** New Data Types *******************************************/

/*************** Constant (Error Code) ************************************/
#define CTR_USAGE_ERROR		0x2001
#define CTR_KEYFILE_ERROR	0x2002

/*************** Global Variables *****************************************/
char	Help[] = "\
Usage1 : -[T/S]   (Test Value/Test Speed)\n\
Usage2 : -[E/D] -[ECB/CBC/OFB/CFB/ECBPAD/CBCPAD/OFBPAD/CFBPAD] infile outfile\n\
            (file 'key.dat' contains UserKey and IV)\n";

/*************** Prototypes ***********************************************/

//########################################################//
//######		Error Code 관리 함수				######//
//########################################################//

/*************************************************************************/
typedef struct{
	DWORD		ErrorCode;
	BYTE		Message[32];
} ERROR_MESSAGE;

ERROR_MESSAGE	ErrorMessage[] = {
	{CTR_FATAL_ERROR,		"CTR_FATAL_ERROR"},
	{CTR_INVALID_USERKEYLEN,"CTR_INVALID_USERKEYLEN"},
	{CTR_PAD_CHECK_ERROR,	"CTR_PAD_CHECK_ERROR"},
	{CTR_DATA_LEN_ERROR,	"CTR_DATA_LEN_ERROR"},
	{CTR_CIPHER_LEN_ERROR,	"CTR_CIPHER_LEN_ERROR"},
	{CTR_USAGE_ERROR,		"CTR_USAGE_ERROR"},
	{CTR_KEYFILE_ERROR,		"CTR_KEYFILE_ERROR"},
	{0, ""},
};

/**************************************************************************
*
*/
void	Error(
		DWORD	ErrorCode,
		char	*Message)
{
	DWORD	i;

	for( i=0; ErrorMessage[i].ErrorCode!=0; i++)
		if( ErrorMessage[i].ErrorCode==ErrorCode )	break;

	printf("ERROR(%s) :::: %s\n", ErrorMessage[i].Message, Message);
	if( ErrorCode==CTR_USAGE_ERROR )	printf("\n%s", Help);
	exit(1);
}

//########################################################//
//######		Validity Test 관련 함수				######//
//########################################################//

/*************************************************************************/
typedef struct{
	DWORD		ModeType;
	DWORD		PadType;
	char		Description[32];
	DWORD		UkLen;
	BYTE		UK[56];
	DWORD		IVLen;
	BYTE		IV[16];
	DWORD		PtLen;
	BYTE		PT[48];
	DWORD		EtLen;
	BYTE		ET[48];
} TEST_STRUCT;

TEST_STRUCT		TestData[] = {
	{	AI_ECB, AI_NO_PADDING, "ECB-NO_PADDING",
		 8,		{0x01,0x23,0x45,0x67,0x89,0xAB,0xCD,0xEF},
		 0,		{0},
		24,		"Now is the time for all ",
		24,		{0x3F,0xA4,0x0E,0x8A,0x98,0x4D,0x48,0x15,
				 0x6A,0x27,0x17,0x87,0xAB,0x88,0x83,0xF9,
				 0x89,0x3D,0x51,0xEC,0x4B,0x56,0x3B,0x53}	},
	{	AI_CBC, AI_NO_PADDING, "CBC-NO_PADDING",
		 8,		{0x01,0x23,0x45,0x67,0x89,0xAB,0xCD,0xEF},
		 8,		{0x12,0x34,0x56,0x78,0x90,0xAB,0xCD,0xEF},
		24,		"Now is the time for all ",
		24,		{0xE5,0xC7,0xCD,0xDE,0x87,0x2B,0xF2,0x7C,
				 0x43,0xE9,0x34,0x00,0x8C,0x38,0x9C,0x0F,
				 0x68,0x37,0x88,0x49,0x9A,0x7C,0x05,0xF6}	},
	{	AI_OFB, 0, "OFB",
		 8,		{0x01,0x23,0x45,0x67,0x89,0xAB,0xCD,0xEF},
		 8,		{0x12,0x34,0x56,0x78,0x90,0xAB,0xCD,0xEF},
		24,		"Now is the time for all ",
		24,		{0xF3,0x09,0x62,0x49,0xC7,0xF4,0x6E,0x51,
				 0x35,0xF2,0x4A,0x24,0x2E,0xEB,0x3D,0x3F,
				 0x3D,0x6D,0x5B,0xE3,0x25,0x5A,0xF8,0xC3}	},
	{	AI_CFB, 0, "CFB",
		 8,		{0x01,0x23,0x45,0x67,0x89,0xAB,0xCD,0xEF},
		 8,		{0x12,0x34,0x56,0x78,0x90,0xAB,0xCD,0xEF},
		 3,		"Now",
		 3,		{0xF3,0x1F,0xDA}	},
	{	AI_ECB, AI_NO_PADDING, "ECB-NO_PADDING",
		 8,		{0x01,0x23,0x45,0x67,0x89,0xAB,0xCD,0xEF},
		 0,		{0},
		32,		{0x37,0x36,0x35,0x34,0x33,0x32,0x31,0x20,
				 0x4E,0x6F,0x77,0x20,0x69,0x73,0x20,0x74,
				 0x68,0x65,0x20,0x74,0x69,0x6D,0x65,0x20,
				 0x66,0x6F,0x72,0x20,0x00,0x00,0x00,0x00},
		32,		{0x21,0xFB,0x19,0x36,0x93,0xA1,0x6C,0x28,
				 0x3F,0xA4,0x0E,0x8A,0x98,0x4D,0x48,0x15,
				 0x6A,0x27,0x17,0x87,0xAB,0x88,0x83,0xF9,
				 0x32,0x87,0x62,0x45,0xE9,0x2B,0xF2,0x29}	},
	{	AI_CBC, AI_NO_PADDING, "CBC-NO_PADDING",
		 8,		{0x01,0x23,0x45,0x67,0x89,0xAB,0xCD,0xEF},
		 8,		{0,0,0,0,0,0,0,0},
		32,		{0x37,0x36,0x35,0x34,0x33,0x32,0x31,0x20,
				 0x4E,0x6F,0x77,0x20,0x69,0x73,0x20,0x74,
				 0x68,0x65,0x20,0x74,0x69,0x6D,0x65,0x20,
				 0x66,0x6F,0x72,0x20,0x00,0x00,0x00,0x00},
		32,		{0x21,0xFB,0x19,0x36,0x93,0xA1,0x6C,0x28,
				 0x6C,0x46,0x3F,0x0C,0xB7,0x16,0x7A,0x6F,
				 0x95,0x6E,0xE8,0x91,0xE8,0x89,0xD9,0x1E,
				 0xF1,0xD3,0x0F,0x68,0x49,0x31,0x2C,0xA4}	},
	{	AI_CBC, AI_PKCS_PADDING, "CBC-PKCS_PADDING",
		 8,		{0x01,0x23,0x45,0x67,0x89,0xAB,0xCD,0xEF},
		 8,		{0x12,0x34,0x56,0x78,0x90,0xAB,0xCD,0xEF},
		24,		"Now is the time for all ",
		32,		{0xE5,0xC7,0xCD,0xDE,0x87,0x2B,0xF2,0x7C,
				 0x43,0xE9,0x34,0x00,0x8C,0x38,0x9C,0x0F,
				 0x68,0x37,0x88,0x49,0x9A,0x7C,0x05,0xF6,
				 0x62,0xC1,0x6A,0x27,0xE4,0xFC,0xF2,0x77}	},
	{	AI_OFB, 0, "OFB",
		 8,		{0x01,0x23,0x45,0x67,0x89,0xAB,0xCD,0xEF},
		 8,		{0,0,0,0,0,0,0,0},
		28,		"7654321 Now is the time for ",
		28,		{0xE2,0xE2,0x7A,0xC3,0x13,0x5A,0x0C,0x2D,
				 0x18,0x0E,0x9E,0xA0,0x26,0x9B,0x5B,0x03,
				 0xF2,0x88,0x10,0xCF,0x48,0xF2,0xBF,0xB1,
				 0xE1,0x47,0xB2,0x95}	},
	{	AI_CFB, 0, "CFB",
		 8,		{0x01,0x23,0x45,0x67,0x89,0xAB,0xCD,0xEF},
		 8,		{0,0,0,0,0,0,0,0},
		28,		"7654321 Now is the time for ",
		28,		{0xE2,0xE2,0x7A,0xC3,0x13,0x5A,0x0C,0x2D,
				 0x25,0x95,0x36,0x18,0xA5,0xFB,0x7D,0xB7,
				 0xB6,0x02,0x8F,0x98,0x47,0xC4,0x8C,0x70,
				 0x7B,0x89,0x1A,0xD1}	},
	{	0, 0, ""},
};

/**************************************************************************
*
*/
void	ValidityTest()
{
	BYTE	EncText[4*DES_BLOCK_LEN], DecText[4*DES_BLOCK_LEN];
	DWORD	i, tt, EncLen, DecLen;
	RET_VAL	ret;
	DES_ALG_INFO	AlgInfo;

	//
	for( i=0; TestData[i].ModeType!=0; i++) {
		DES_SetAlgInfo(TestData[i].ModeType, TestData[i].PadType,
						TestData[i].IV, &AlgInfo);

		//
		ret = DES_KeySchedule(TestData[i].UK, TestData[i].UkLen, &AlgInfo);
		if( ret!=CTR_SUCCESS )	Error(ret, "DES_KeySchedule() returns.");

		//
		EncLen = tt = 0;
		ret = DES_EncInit(&AlgInfo);
		if( ret!=CTR_SUCCESS )	Error(ret, "DES_EncInit() returns.");
		ret = DES_EncUpdate(&AlgInfo, TestData[i].PT, TestData[i].PtLen,
							EncText, &EncLen);
		if( ret!=CTR_SUCCESS )	Error(ret, "DES_EncUpdate() returns.");
		ret = DES_EncFinal(&AlgInfo, EncText+EncLen, &tt);
		if( ret!=CTR_SUCCESS )	Error(ret, "DES_EncFinal() returns.");
		EncLen += tt;

		//
		DecLen = tt = 0;
		ret = DES_DecInit(&AlgInfo);
		if( ret!=CTR_SUCCESS )	Error(ret, "DES_DecInit() returns.");
		ret = DES_DecUpdate(&AlgInfo, EncText, EncLen, DecText, &DecLen);
		if( ret!=CTR_SUCCESS )	Error(ret, "DES_DecUpdate() returns.");
		ret = DES_DecFinal(&AlgInfo, DecText+DecLen, &tt);
		if( ret!=CTR_SUCCESS )	Error(ret, "DES_DecFinal() returns.");
		DecLen += tt;

		//
		fprintf(stdout, "\n==== test %d : %s\n", i, TestData[i].Description);
		PrintBYTE(stdout, "UK", TestData[i].UK, TestData[i].UkLen);
		if( TestData[i].IVLen!=0 )
			PrintBYTE(stdout, "IV", TestData[i].IV, TestData[i].IVLen);
		PrintBYTE(stdout, "PT", TestData[i].PT, TestData[i].PtLen);
		PrintBYTE(stdout, "ET", EncText, EncLen);
		if( memcmp(TestData[i].PT, DecText, TestData[i].PtLen)==0 )
			printf("   DT = (OK)\n");
		else
			PrintBYTE(stdout, "DT", DecText, DecLen);
	}
}

/**************************************************************************
*
*/
void	SpeedTest()
{
#define DATA_LEN	1024
	BYTE	UserKey[DES_USER_KEY_LEN], IV[DES_BLOCK_LEN];
	BYTE	PT[DATA_LEN+32], ET[DATA_LEN+32], DT[DATA_LEN+32];
	DWORD	i, tt, Iter, EncLen=0, DecLen;
	RET_VAL	ret;
	DES_ALG_INFO	AlgInfo;

	//
	for( tt=0; tt<DES_USER_KEY_LEN; tt++)	UserKey[tt] = 0;
	for( tt=0; tt<DES_BLOCK_LEN; tt++)		IV[tt] = 0;
	DES_SetAlgInfo(AI_CBC, AI_NO_PADDING, IV, &AlgInfo);

	//
	ret = DES_KeySchedule(UserKey, DES_USER_KEY_LEN, &AlgInfo);
	if( ret!=CTR_SUCCESS )	Error(ret, "DES_KeySchedule() returns.");

	for( i=0; i<3; i++) {
		fprintf(stdout, "\n==== test %d\n", i);
		Iter = 2 * 7600;

		//
		for( tt=0; tt<DATA_LEN; tt++)	PT[tt] = 0;

		//
		SPEED_TEST("Enc:", " / ", Iter, DATA_LEN,
			{	EncLen = tt = 0;
				ret = DES_EncInit(&AlgInfo);
				ret = DES_EncUpdate(&AlgInfo, PT, DATA_LEN,
									ET, &EncLen);
				ret = DES_EncFinal(&AlgInfo, ET+EncLen, &tt);
				EncLen += tt;									}	);
		//
		SPEED_TEST("Dec:", " // ", Iter, DATA_LEN,
			{	DecLen = tt = 0;
				ret = DES_DecInit(&AlgInfo);
				ret = DES_DecUpdate(&AlgInfo, ET, EncLen,
									DT, &DecLen);
				ret = DES_DecFinal(&AlgInfo, DT+DecLen, &tt);
				DecLen += tt;									}	);

		//
		for( tt=0; tt<DATA_LEN; tt++)
			if( DT[tt]!=0 )	break;
		if( tt==DATA_LEN )	printf("OK");
		else				printf("FAIL");
	}
#undef DATA_LEN
}

/**************************************************************************
*
*/
void	GetKeyIV(
		BYTE	Key[DES_USER_KEY_LEN],
		DWORD	*KeyLen,
		BYTE	IV[DES_BLOCK_LEN],
		DWORD	*IVLen)
{
	DWORD	ch, i, j;
	FILE	*pfile;

	if( (pfile=fopen("key.dat", "r"))==NULL ) {
		printf( "The file 'key.dat' was not opened\n");
		Error(0, "File(key.dat) Open Error");
	}

	fscanf(pfile, "%d", &j);
	*KeyLen = j;
	for( i=0; i<j; i++) {
		fscanf(pfile, "%X", &ch);
		Key[i] = (BYTE) ch;
	}

	fscanf(pfile, "%d", &j);
	if( j!=DES_BLOCK_LEN ) {
		*IVLen = 0;
		for( i=0; i<DES_BLOCK_LEN; i++)
			IV[i] = (BYTE) 0;
	}
	else {
		*IVLen = DES_BLOCK_LEN;
		for( i=0; i<DES_BLOCK_LEN; i++) {
			fscanf(pfile, "%X", &ch);
			IV[i] = (BYTE) ch;
		}
	}

	fclose(pfile);
}

int Encryption(char *SrcData, int SrcLen, char** DstData, int *DstLen, int EncType)
{
	int i;
	char	UserKey[DES_USER_KEY_LEN];
	char	IV[DES_BLOCK_LEN];
	char    *tDstData;
	int 	UKLen, IVLen;
	int tDstLen;
	RET_VAL	ret;
	DES_ALG_INFO	AlgInfo;
	int	ModeType=2;
	int	PadType=2;
	char SrcUserKey[8][3]={"01","23","45","67","89","AB","CD","EF"};
	char SrvIV[8][3]={"12","34","56","78","90","AB","CD","EF"};
	int	ch;
	int tSrcLen;
	int tEncLen=ENCLEN;

	// DstData 크기를 잡는다. 
	*DstData=(char*)malloc(SrcLen+32);
	tDstData=*DstData;
	*DstLen=0;

	// 키값을 정한다.
	for(i=0;i<8;i++)
	{
		sscanf(SrcUserKey[i],"%X",&ch);
		UserKey[i] = (BYTE) ch;
	}
	UKLen=8;

	for(i=0;i<8;i++)
	{
		sscanf(SrvIV[i],"%X",&ch);
		IV[i] = (BYTE) ch;
	}
	IVLen=8;

	DES_SetAlgInfo(ModeType, PadType, (BYTE*)IV, &AlgInfo);
	ret = DES_KeySchedule((BYTE*)UserKey, UKLen, &AlgInfo);
	if( ret!=CTR_SUCCESS )	return -1;
	
	if(EncType==0)
	{
		ret = DES_EncInit(&AlgInfo);

		tSrcLen=0;
		while(1)
		{
			if(SrcLen-tSrcLen<ENCLEN)
				tEncLen=SrcLen-tSrcLen;
			tDstLen=ENCLEN;
			ret = DES_EncUpdate(&AlgInfo, (BYTE*)(SrcData+tSrcLen),  tEncLen , (BYTE*)(tDstData+(*DstLen)), (DWORD*)&tDstLen);
			if( ret!=CTR_SUCCESS )	return -1;
			tSrcLen+=tEncLen;
			if( tDstLen >=0)
			{
				*DstLen+=tDstLen;
			}
			if(tEncLen<ENCLEN)
				break;
		}
		
		tDstLen=ENCLEN;
		ret = DES_EncFinal(&AlgInfo, (BYTE*)(tDstData+(*DstLen)), (DWORD*)&tDstLen);
		if( ret!=CTR_SUCCESS )	return -1;
		
		if( tDstLen >=0)
		{
			*DstLen+=tDstLen;
		}
	}
	else
	{
		ret = DES_DecInit(&AlgInfo);

		tSrcLen=0;
		while(1)
		{
			if(SrcLen-tSrcLen<ENCLEN)
				tEncLen=SrcLen-tSrcLen;

			tDstLen=ENCLEN;
			ret = DES_DecUpdate(&AlgInfo, (BYTE*)(SrcData+tSrcLen),  tEncLen , (BYTE*)tDstData+(*DstLen), (DWORD*)&tDstLen);
			if( ret!=CTR_SUCCESS )	return -1;
			tSrcLen+=tEncLen;

			if( tDstLen >=0)
			{
				*DstLen+=tDstLen;
			}
			
			if(tEncLen<ENCLEN)
				break;
		}
			
		tDstLen=ENCLEN;
		ret = DES_DecFinal(&AlgInfo, (BYTE*)tDstData+(*DstLen), (DWORD*)&tDstLen);
		if( ret!=CTR_SUCCESS )	return -1;
		if( tDstLen >=0)
		{
			*DstLen+=tDstLen;
		}
	}

	return 0;
}

/*************** END OF FILE **********************************************/
