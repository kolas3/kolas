// SubField.cpp: implementation of the CSubField class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SubField.h"
#include "efs.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSubfield::CSubfield()
{
	m_strItemData.Empty();
	m_strPunctuationMark.Empty();
	m_strSubfieldCode.Empty();
}

CSubfield *CSubfield::Clone()
{
	EFS_BEGIN

	CSubfield *pSubfield = new CSubfield;
	pSubfield->SetItemData(GetItemData());
	pSubfield->SetPunctuationMark(GetPunctuationMark());
	pSubfield->SetSubfieldCode(GetSubfieldCode());

	return pSubfield;

	EFS_END
	return NULL;

}

CSubfield::~CSubfield()
{

}
