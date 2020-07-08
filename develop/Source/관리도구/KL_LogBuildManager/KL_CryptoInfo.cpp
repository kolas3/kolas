// KL_CryptoInfo.cpp: implementation of the CKL_CryptoInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KL_CryptoManager.h"
#include "KL_CryptoInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKL_CryptoInfo::CKL_CryptoInfo()
{
	m_nCryptoOpt = 0;
	m_nCryptoDisableOpt = 0;

	m_nUserCount = 0;
	m_nLoanUserCount = 0;
}

CKL_CryptoInfo::~CKL_CryptoInfo()
{

}
