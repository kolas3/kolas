// SIReportObject_Barcode.cpp: implementation of the CSIReportObject_Barcode class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SIReportObject_Barcode.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#include "efs.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSIReportObject_Barcode::CSIReportObject_Barcode()
{
	SetObjType(OBJECT_BARCODE);
	m_strName = _T("BARCODE");
	m_pBC = NULL;
	m_pBC = new CSIBarcodeCtrl;

	SetStyle(3);

	InitStyleData();
}

VOID CSIReportObject_Barcode::Clone(CSIReportObject *pObject)
{
EFS_BEGIN

	CSIReportObject::Clone(pObject);

	((CSIReportObject_Barcode*)pObject)->m_nStyle = m_nStyle;
	((CSIReportObject_Barcode*)pObject)->m_strCaption = m_strCaption;

	CSIBarcodeCtrl* pBC = new CSIBarcodeCtrl;
	pBC->SetStyle(m_nStyle);
	pBC->SetCaption(m_strCaption);

	CSIBarcodeCtrl *pOldBC = ((CSIReportObject_Barcode*)pObject)->m_pBC;
	if (pOldBC) {
		delete pOldBC;
		pOldBC = NULL;
	}


	((CSIReportObject_Barcode*)pObject)->m_pBC = pBC;

EFS_END
}

CSIReportObject_Barcode::~CSIReportObject_Barcode()
{
	if (m_pBC) {
		delete m_pBC;
		m_pBC = NULL;
	}
}

VOID CSIReportObject_Barcode::Draw(CDC *pDC, BOOL bIsVisible /*= TRUE*/)
{
EFS_BEGIN

	CRect rect = GetRect();
	pDC->Rectangle(rect);

	Draw(pDC, rect);

EFS_END
}

VOID CSIReportObject_Barcode::Draw( CDC * pDC, CRect rcBounds )
{
EFS_BEGIN

	if (!m_pBC) return;
	if (!pDC) return;
	m_pBC->Draw(pDC, rcBounds);

EFS_END
}

INT CSIReportObject_Barcode::GetCaption(CString &strCaption)
{
EFS_BEGIN

	m_pBC->GetCaption(strCaption);
	strCaption = m_strCaption;
	return 0;

EFS_END
return -1;

}

INT CSIReportObject_Barcode::SetCaption(CString strCaption)
{
EFS_BEGIN

	m_strCaption = strCaption;
	m_pBC->SetCaption(strCaption);
	return 0;

EFS_END
return -1;

}

INT CSIReportObject_Barcode::GetStyle(CString &strStyle)
{
EFS_BEGIN

	return StyleCodeToDesc(m_nStyle, strStyle);

EFS_END
return -1;

}

INT CSIReportObject_Barcode::GetStyle(INT &nStyle)
{
EFS_BEGIN

	nStyle = m_nStyle;
	return 0;

EFS_END
return -1;

}

INT CSIReportObject_Barcode::SetStyle(CString strStyle)
{
EFS_BEGIN

	INT nStyle;
	INT ids = StyleDescToCode(strStyle, nStyle);
	if (ids < 0) return ids;
	m_nStyle = nStyle;
	m_pBC->SetStyle(m_nStyle);
	return 0;

EFS_END
return -1;

}

INT CSIReportObject_Barcode::SetStyle(INT nStyle)
{
EFS_BEGIN

	m_nStyle = nStyle;
	m_pBC->SetStyle(nStyle);
	return 0;

EFS_END
return -1;

}

INT CSIReportObject_Barcode::StyleCodeToDesc(INT nStyle, CString &strStyle)
{
EFS_BEGIN

	/*
	{ 1,  FALSE, 1, a2of5,		NULL,	NULL,			  NULL },
	{ 2,  TRUE,	 1, aI2of5,		NULL,	NULL,			  NULL },
	{ 3,  FALSE, 1, a3of9,		NULL,	NULL,			  NULL },
	{ 4,  FALSE, 1, aCodabar,	NULL,	NULL,			  NULL },
	{ 5,  FALSE, 1, a3of9,		a3of9e,	NULL,			  NULL },
	{ 6,  FALSE, 1, aCode128A,	NULL,	CheckSum128AInit, CheckSum128 },
	{ 7,  FALSE, 1, aCode128B,	NULL,	CheckSum128BInit, CheckSum128 },
	{ 8,  TRUE,	 1, aCode128C,	NULL,	CheckSum128CInit, CheckSum128 },
	{ 9,  TRUE,	 1, aUPCOdd,	NULL,	NULL,			  NULL },
	{ 10, FALSE, 1, aMSI,		NULL,	CheckSumMSIInit,  CheckSumMSI },
	{ 11, FALSE, 1, a93,		NULL,	CheckSum93Init,	  CheckSum93 },
	{ 12, FALSE, 1, a93,		a93e,	CheckSum93Init,	  CheckSum93 },
	{ 13,  TRUE, 1, aEANA,		NULL,	NULL,			  NULL },
	{ 14,  TRUE, 1, aEANA,		NULL,	NULL,			  NULL },
	{ 15,  TRUE, 1, aPostNet,	NULL,	CheckSumPNInit,	  CheckSumPostNet },
	{ 16, FALSE, 1, aANSI3of9,	NULL,	NULL,			  NULL },
	{ 17, FALSE, 1, aANSI3of9,	a3of9e,	NULL,			  NULL },
	{ 18,  TRUE, 1, aCode128A,	NULL,	CheckSum128AInit, CheckSum128 },
	{ 19,  TRUE, 1, aCode128A,	NULL,	CheckSum128AInit, CheckSum128 },
	{ 20,  TRUE, 1, aUPCEOdd,	NULL,	NULL,			  NULL },
	{ 21,  TRUE, 1, NULL,		NULL,	NULL,			  NULL },
	{ 22, FALSE, 1, aMSI,		NULL,	CheckSumMSIInit,  CheckSumMSI },
	{ 23,  TRUE, 1, aI2of5,		NULL,	NULL,			  NULL },
	*/
	/*
	if (nStyle == 3) {
		strStyle = _T("Code3of9");
		return 0;
	}
	*/

	strStyle = m_strStyles[nStyle][3];

	return -1;

EFS_END
return -1;

}

INT CSIReportObject_Barcode::StyleDescToCode(CString strStyle, INT &nStyle)
{
EFS_BEGIN

	strStyle.TrimLeft();		strStyle.TrimRight();
	//strStyle.MakeLower();
	/*if (strStyle == _T("code3of9")) {
		nStyle = 3;
		return 0;
	}
	*/

	for (INT i = 0; i < m_nStyles; i++)
	{
		if (m_strStyles[i][2] == strStyle)
		{
			nStyle = _ttoi(m_strStyles[i][0]);
			return 0;
		}
	}
	return -1;

EFS_END
return -1;

}

INT CSIReportObject_Barcode::InitStyleData(VOID)
{
EFS_BEGIN

	m_nStyles = 0;

	m_strStyles[m_nStyles][0] = _T("1");
	m_strStyles[m_nStyles][1] = _T("a2of5");
	m_strStyles[m_nStyles][2] = _T("Code 2 of 5");
	m_nStyles++;

	m_strStyles[m_nStyles][0] = _T("2");
	m_strStyles[m_nStyles][1] = _T("aI2of5");
	m_strStyles[m_nStyles][2] = _T("Interleaved 2 of 5");
	m_nStyles++;

	m_strStyles[m_nStyles][0] = _T("3");
	m_strStyles[m_nStyles][1] = _T("a3of9");
	m_strStyles[m_nStyles][2] = _T("Code 3 of 9");
	m_nStyles++;

	m_strStyles[m_nStyles][0] = _T("4");
	m_strStyles[m_nStyles][1] = _T("aCodabar");
	m_strStyles[m_nStyles][2] = _T("Codabar(Rationalized)");
	m_nStyles++;

	m_strStyles[m_nStyles][0] = _T("5");
	m_strStyles[m_nStyles][1] = _T("a3of9");
	m_strStyles[m_nStyles][2] = _T("Extended Code 3 of 9");
	m_nStyles++;

	m_strStyles[m_nStyles][0] = _T("6");
	m_strStyles[m_nStyles][1] = _T("aCode128A");
	m_strStyles[m_nStyles][2] = _T("Code 128A");
	m_nStyles++;

	m_strStyles[m_nStyles][0] = _T("7");
	m_strStyles[m_nStyles][1] = _T("aCode128B");
	m_strStyles[m_nStyles][2] = _T("Code 128B");
	m_nStyles++;

	m_strStyles[m_nStyles][0] = _T("8");
	m_strStyles[m_nStyles][1] = _T("aCode128C");
	m_strStyles[m_nStyles][2] = _T("Code 128C");
	m_nStyles++;

	m_strStyles[m_nStyles][0] = _T("9");
	m_strStyles[m_nStyles][1] = _T("aUPCOdd");
	m_strStyles[m_nStyles][2] = _T("UPC-A");
	m_nStyles++;

	m_strStyles[m_nStyles][0] = _T("10");
	m_strStyles[m_nStyles][1] = _T("aMSI");
	m_strStyles[m_nStyles][2] = _T("MSI (Plessey)");
	m_nStyles++;

	m_strStyles[m_nStyles][0] = _T("11");
	m_strStyles[m_nStyles][1] = _T("a93");
	m_strStyles[m_nStyles][2] = _T("Code 93");
	m_nStyles++;

	m_strStyles[m_nStyles][0] = _T("12");
	m_strStyles[m_nStyles][1] = _T("a93");
	m_strStyles[m_nStyles][2] = _T("Extended Code 93");
	m_nStyles++;

	m_strStyles[m_nStyles][0] = _T("13");
	m_strStyles[m_nStyles][1] = _T("aEANA");
	m_strStyles[m_nStyles][2] = _T("EAN-13");
	m_nStyles++;

	m_strStyles[m_nStyles][0] = _T("14");
	m_strStyles[m_nStyles][1] = _T("aEANA");
	m_strStyles[m_nStyles][2] = _T("EAN-8");
	m_nStyles++;

	m_strStyles[m_nStyles][0] = _T("15");
	m_strStyles[m_nStyles][1] = _T("aPostNet");
	m_strStyles[m_nStyles][2] = _T("PostNet");
	m_nStyles++;

	m_strStyles[m_nStyles][0] = _T("16");
	m_strStyles[m_nStyles][1] = _T("aANSI3of9");
	m_strStyles[m_nStyles][2] = _T("ANSI 3 of 9");
	m_nStyles++;

	m_strStyles[m_nStyles][0] = _T("17");
	m_strStyles[m_nStyles][1] = _T("aANSI3of9");
	m_strStyles[m_nStyles][2] = _T("Extended ANSI 3 of 9");
	m_nStyles++;

	m_strStyles[m_nStyles][0] = _T("18");
	m_strStyles[m_nStyles][1] = _T("aCode128A");
	m_strStyles[m_nStyles][2] = _T("Code 128(auto-select)");
	m_nStyles++;

	m_strStyles[m_nStyles][0] = _T("19");
	m_strStyles[m_nStyles][1] = _T("aCode128A");
	m_strStyles[m_nStyles][2] = _T("UCC/EAN-128");
	m_nStyles++;

	return 0;

EFS_END
return -1;

}
