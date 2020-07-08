// YearButton.cpp : implementation file
//

#include "stdafx.h"
#include "Kalendar.h"
#include "YearButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CYearButton

CYearButton::CYearButton()
{
	CTime t = CTime::GetCurrentTime();
	m_uYear = t.GetYear();		
}

CYearButton::~CYearButton()
{
}


BEGIN_MESSAGE_MAP(CYearButton, CButton)
	//{{AFX_MSG_MAP(CYearButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CYearButton message handlers

void CYearButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item

}

void CYearButton::SetYear(UINT uYear)
{
	m_uYear = uYear;
}
