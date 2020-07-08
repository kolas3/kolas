// Form.cpp: implementation of the CForm class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Form.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CForm::CForm()
{

}

CForm::~CForm()
{

}




INT CForm::InsertValue(  CESL_DataMgr* pCardDM , INT nIndex  )
{
	const INT nCnt = 6;
	CString sAlias[nCnt] = 
	{
		_T("대출양식KEY") , _T("도서관부호") , _T("양식이름") , _T("앞면여부") , _T("WIDTH") , 
		_T("HEIGHT") 
	};
	
	CString sResult[nCnt];
	
	pCardDM->GetCellData( sAlias , nCnt , sResult , 0 );
	
	sFormKey = sResult[0];
	sLibCode = sResult[1];
	sFormName = sResult[2];
	nPageNum = _ttoi(sResult[3]);
	nWidth = _ttoi( sResult[4] );
	nHeight = _ttoi( sResult[5] );
	
	
	return 0;
}
