// PrintDataInfo.cpp: implementation of the CPrintDataInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PrintDataInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPrintDataInfo::CPrintDataInfo()
{
	m_MapPrintData.RemoveAll();
}

CPrintDataInfo::~CPrintDataInfo()
{

}

INT CPrintDataInfo::AddPrintData(CString strKey, CString strValue)
{
	if(strKey.IsEmpty()) return -1;

	m_MapPrintData.SetAt(strKey, strValue);
	return 0;
}

CString CPrintDataInfo::GetPrintData(CString strKey)
{
	if(strKey.IsEmpty()) return _T("");

	CString strValue;
	m_MapPrintData.Lookup(strKey, strValue);
	return strValue;
}
