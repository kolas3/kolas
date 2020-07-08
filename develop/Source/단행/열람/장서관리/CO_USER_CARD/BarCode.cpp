// BarCode.cpp: implementation of the CBarCode class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BarCode.h"
#include "../../../../ESL/SIReportData/SIBarcodeCtrl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBarCode::CBarCode()
{

}

CBarCode::~CBarCode()
{

}


INT CBarCode::InsertValue(  CESL_DataMgr* pCardDM , INT nIndex ,INT nMode )
{
	const INT nCnt = 2;
	CString sAlias[nCnt] = 
	{
		_T("내용") , _T("바코드_타입")
	};
	
	CString sResult[nCnt];
	
	pCardDM->GetCellData( sAlias , nCnt , sResult , nIndex );
	
	sText = sResult[0];
	nClass = _ttoi(sResult[1]);

	CControlObject::InsertValue( pCardDM , nIndex , nMode );
	
	
	return 0;
}


INT CBarCode::DrawDC( CDC* pDC )
{
	
	CSIBarcodeCtrl BarCode;

	
	BarCode.SetCaption( sText );
	BarCode.SetStyle(nClass);

	BarCode.Draw( pDC , rLocation );


	CControlObject::DrawDC(pDC);

	return 0;	
}