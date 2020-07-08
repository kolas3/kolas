// MNG_LIB_HOLIDAY_MGR_05.cpp : implementation file
//

#include "stdafx.h"
#include "MNG_LIB_HOLIDAY_MGR_05.h"
#include <afxdtctl.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_LIB_HOLIDAY_MGR_05 dialog


CMNG_LIB_HOLIDAY_MGR_05::CMNG_LIB_HOLIDAY_MGR_05(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_LIB_HOLIDAY_MGR_05)
	m_strDays = _T("");	
	//}}AFX_DATA_INIT

	m_pParent = pParent;
}
CMNG_LIB_HOLIDAY_MGR_05::~CMNG_LIB_HOLIDAY_MGR_05()
{
	
}

VOID CMNG_LIB_HOLIDAY_MGR_05::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_LIB_HOLIDAY_MGR_05)
	DDX_Control(pDX, IDC_MONTHCALENDAR1, m_ctlMonth);
	DDX_Text(pDX, IDC_eDAYS, m_strDays);	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_LIB_HOLIDAY_MGR_05, CDialog)
	//{{AFX_MSG_MAP(CMNG_LIB_HOLIDAY_MGR_05)
	ON_BN_CLICKED(IDC_bAPPLY, OnbAPPLY)
	ON_BN_CLICKED(IDC_bCLOSE, OnbCLOSE)
	ON_NOTIFY(MCN_SELECT, IDC_MONTHCALENDAR1, OnSelectMonthcalendar1)
	ON_NOTIFY(MCN_SELCHANGE, IDC_MONTHCALENDAR1, OnSelchangeMonthcalendar1)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_MONTHCALENDAR1, OnReleasedcaptureMonthcalendar1)
	ON_NOTIFY(MCN_GETDAYSTATE, IDC_MONTHCALENDAR1, OnGetdaystateMonthcalendar1)
	ON_EN_KILLFOCUS(IDC_eDAYS, OnKillfocuseDAYS)
	ON_EN_KILLFOCUS(IDC_eDAYS10, OnKillfocuseDAYS10)
	ON_EN_KILLFOCUS(IDC_eDAYS11, OnKillfocuseDAYS11)
	ON_EN_KILLFOCUS(IDC_eDAYS12, OnKillfocuseDAYS12)
	ON_EN_KILLFOCUS(IDC_eDAYS2, OnKillfocuseDAYS2)
	ON_EN_KILLFOCUS(IDC_eDAYS3, OnKillfocuseDAYS3)
	ON_EN_KILLFOCUS(IDC_eDAYS4, OnKillfocuseDAYS4)
	ON_EN_KILLFOCUS(IDC_eDAYS5, OnKillfocuseDAYS5)
	ON_EN_KILLFOCUS(IDC_eDAYS6, OnKillfocuseDAYS6)
	ON_EN_KILLFOCUS(IDC_eDAYS7, OnKillfocuseDAYS7)
	ON_EN_KILLFOCUS(IDC_eDAYS8, OnKillfocuseDAYS8)
	ON_EN_KILLFOCUS(IDC_eDAYS9, OnKillfocuseDAYS9)
	ON_WM_CTLCOLOR() 
	ON_BN_CLICKED(IDC_bINIT, OnbINIT)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_LIB_HOLIDAY_MGR_05 message handlers

VOID CMNG_LIB_HOLIDAY_MGR_05::OnbAPPLY() 
{
	m_strDaysList.RemoveAll();
	CString strDays;
	INT		IDC_EDIT;
	for ( int i=1 ; i<13 ; i++ )
	{					
		IDC_EDIT = GetIDC(i);
		( (CEdit*) GetDlgItem (IDC_EDIT) )->GetWindowText(strDays);
		strDays.TrimLeft();
		strDays.TrimRight();
		m_strDaysList.Add(strDays);
	}
	
	CDialog::OnOK();
	
}

VOID CMNG_LIB_HOLIDAY_MGR_05::OnbCLOSE() 
{
	CDialog::OnCancel();	
}

BOOL CMNG_LIB_HOLIDAY_MGR_05::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	ShowTitle();	

	CTime CurTime(m_nYear,m_nMonth,01, 01,01,01);

	m_ctlMonth.SetCurSel(CurTime);

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CMNG_LIB_HOLIDAY_MGR_05::ShowTitle(VOID)
{
	CString str;
	str.Format(_T("휴관일 수정 (%s년)"), m_strParentYear);
	SetWindowText(str);


	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )m_pParent->GetDlgItem( IDC_mngGRID_1 );
	if (pGrid == NULL) return ;

	CString strDays;
	INT		IDC_EDIT;
	for ( int i=1 ; i<13 ; i++ )
	{	
		strDays = pGrid->GetTextMatrix(i, 2);
		strDays.TrimLeft();
		strDays.TrimRight();
		IDC_EDIT = GetIDC(i);
		( (CEdit*) GetDlgItem (IDC_EDIT) )->SetWindowText(strDays);
	}

	return;
}
void CMNG_LIB_HOLIDAY_MGR_05::OnSelectMonthcalendar1(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	LPNMSELCHANGE pSelChange = (LPNMSELCHANGE) pNMHDR;
	*pResult = 0;
	
	NMSELCHANGE *pNMHead = (NMSELCHANGE*)pNMHDR;
	SYSTEMTIME TempTime = pNMHead->stSelStart;

	CString strYear;

	strYear.Format(_T("%d"), TempTime.wYear );	
	
	// m_Date.Format(_T("%d/%d/%d"), TempTime.wYear, TempTime.wMonth, TempTime.wDay);	
	SetDate(TempTime.wYear, TempTime.wMonth, TempTime.wDay);
}

void CMNG_LIB_HOLIDAY_MGR_05::OnSelchangeMonthcalendar1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LPNMSELCHANGE pSelChange = (LPNMSELCHANGE) pNMHDR;
	*pResult = 0;
	
	NMSELCHANGE *pNMHead = (NMSELCHANGE*)pNMHDR;
	SYSTEMTIME TempTime = pNMHead->stSelStart;

	CString strYear;

	strYear.Format(_T("%d"), TempTime.wYear );

	if ( strYear != m_strParentYear )
	{
		( (CEdit*) m_pParent->GetDlgItem(IDC_eYEAR) )->SetWindowText(strYear);	
		m_pParent->SendMessage(WM_USER+1000, NULL, NULL);		
		m_strParentYear = strYear;
		ShowTitle();
	}
}

void CMNG_LIB_HOLIDAY_MGR_05::OnReleasedcaptureMonthcalendar1(NMHDR* pNMHDR, LRESULT* pResult) 
{		
	*pResult = 0;
}

void CMNG_LIB_HOLIDAY_MGR_05::OnGetdaystateMonthcalendar1(NMHDR* pNMHDR, LRESULT* pResult) 
{
		
	*pResult = 0;
}

INT CMNG_LIB_HOLIDAY_MGR_05::GetIDC(INT nMonth)
{
	INT IDC_EDIT;

	switch ( nMonth )
	{
		case 1:
			IDC_EDIT = IDC_eDAYS;			
			break;
		case 2:
			IDC_EDIT = IDC_eDAYS2;
			break;
		case 3:
			IDC_EDIT = IDC_eDAYS3;
			break;
		case 4:
			IDC_EDIT = IDC_eDAYS4;
			break;
		case 5:
			IDC_EDIT = IDC_eDAYS5;
			break;
		case 6:
			IDC_EDIT = IDC_eDAYS6;
			break;
		case 7:
			IDC_EDIT = IDC_eDAYS7;
			break;
		case 8:
			IDC_EDIT = IDC_eDAYS8;
			break;
		case 9:
			IDC_EDIT = IDC_eDAYS9;
			break;
		case 10:
			IDC_EDIT = IDC_eDAYS10;
			break;
		case 11:
			IDC_EDIT = IDC_eDAYS11;
			break;
		case 12:
			IDC_EDIT = IDC_eDAYS12;
			break;
		default:
			IDC_EDIT = 0;
			break;
	}

	return IDC_EDIT;
}


VOID CMNG_LIB_HOLIDAY_MGR_05::SetDate(INT nYear, INT nMonth, INT nDay)
{
	CString strMonth;
	CString strDays;
	CString strDay;
	INT		IDC_EDIT;

	strDay.Format(_T("%d"), nDay );

	IDC_EDIT = GetIDC(nMonth);	

	( (CEdit*) GetDlgItem (IDC_EDIT) )->GetWindowText(strDays);

	CStringArray strDayList;
	CString strTmp;
	BOOL bFind;

	INT offset = 0;
	INT nPos = 0;
	
	strDayList.RemoveAll();
	bFind = FALSE;
	while ( TRUE )
	{
		nPos = strDays.Find(_T(","), offset );
		if ( nPos == -1 )
		{
			strTmp = strDays.Mid(offset);
			strTmp.TrimLeft();
			strTmp.TrimRight();		
			if ( strTmp == strDay )
			{
				offset = nPos + 1;
				bFind = TRUE;				
			}
			else
			{
				strDayList.Add(strTmp);			
			}
			break;
		}		
		strTmp = strDays.Mid(offset, nPos - offset);
		strTmp.TrimLeft();
		strTmp.TrimRight();		
		if ( strTmp == strDay )
		{
			offset = nPos + 1;
			bFind = TRUE;
			continue;
		}
		strDayList.Add(strTmp);
		offset = nPos + 1;
	}

	if ( bFind == FALSE )
	{
		strDayList.Add(strDay);
	}
	
	// 정렬
	int i, j;
	CString temp;	
	for (i = strDayList.GetSize()-1 ; i > 0 ; i--)		
	{ 		
		for (j = 1 ; j <= i ; j++)			
		{			
			if ( _ttoi(strDayList.GetAt(j-1)) > _ttoi(strDayList.GetAt(j)) )
			{    
				temp = strDayList.GetAt(j-1);			
				strDayList.SetAt(j-1, strDayList.GetAt(j));
				strDayList.SetAt(j, temp);
			}			
		}		
	}
	
	strDays = _T("");
	for (i=0 ; i<strDayList.GetSize() ; i++ )
	{		
		if ( !strDays.IsEmpty() ) strDays += ", ";			
		strDays += strDayList.GetAt(i);		
	}
	

	( (CEdit*) GetDlgItem (IDC_EDIT) )->SetWindowText(strDays);
	
}

VOID CMNG_LIB_HOLIDAY_MGR_05::SortItem(INT nMonth)
{
	CString strMonth;
	CString strDays;
	CString strDay;
	INT		IDC_EDIT;

	IDC_EDIT = GetIDC(nMonth);	

	( (CEdit*) GetDlgItem (IDC_EDIT) )->GetWindowText(strDays);

	CStringArray strDayList;
	CString strTmp;
	
	INT offset = 0;
	INT nPos = 0;
	
	strDayList.RemoveAll();	
	while ( TRUE )
	{
		nPos = strDays.Find(_T(","), offset );
		if ( nPos == -1 )
		{
			strTmp = strDays.Mid(offset);
			strTmp.TrimLeft();
			strTmp.TrimRight();		
			strDayList.Add(strTmp);						
			break;
		}		
		strTmp = strDays.Mid(offset, nPos - offset);
		strTmp.TrimLeft();
		strTmp.TrimRight();				
		strDayList.Add(strTmp);
		offset = nPos + 1;
	}	
	
	// 정렬
	int i, j;
	CString temp;	
	for (i = strDayList.GetSize()-1 ; i > 0 ; i--)		
	{ 		
		for (j = 1 ; j <= i ; j++)			
		{			
			if ( _ttoi(strDayList.GetAt(j-1)) > _ttoi(strDayList.GetAt(j)) )
			{    
				temp = strDayList.GetAt(j-1);			
				strDayList.SetAt(j-1, strDayList.GetAt(j));
				strDayList.SetAt(j, temp);
			}			
		}		
	}
	
	strDays = _T("");
	for (i=0 ; i<strDayList.GetSize() ; i++ )
	{		
		if ( !strDays.IsEmpty() ) strDays += ", ";			
		strDays += strDayList.GetAt(i);		
	}

	( (CEdit*) GetDlgItem (IDC_EDIT) )->SetWindowText(strDays);
}

void CMNG_LIB_HOLIDAY_MGR_05::OnKillfocuseDAYS() 
{
	SortItem(1);
	
}

void CMNG_LIB_HOLIDAY_MGR_05::OnKillfocuseDAYS10() 
{
	SortItem(10);
	
}

void CMNG_LIB_HOLIDAY_MGR_05::OnKillfocuseDAYS11() 
{
	SortItem(11);
	
}

void CMNG_LIB_HOLIDAY_MGR_05::OnKillfocuseDAYS12() 
{
	SortItem(12);
	
}

void CMNG_LIB_HOLIDAY_MGR_05::OnKillfocuseDAYS2() 
{
	SortItem(2);
	
}

void CMNG_LIB_HOLIDAY_MGR_05::OnKillfocuseDAYS3() 
{
	SortItem(3);
	
}

void CMNG_LIB_HOLIDAY_MGR_05::OnKillfocuseDAYS4() 
{
	SortItem(4);
	
}

void CMNG_LIB_HOLIDAY_MGR_05::OnKillfocuseDAYS5() 
{
	SortItem(5);	
}

void CMNG_LIB_HOLIDAY_MGR_05::OnKillfocuseDAYS6() 
{
	SortItem(6);	
}

void CMNG_LIB_HOLIDAY_MGR_05::OnKillfocuseDAYS7() 
{
	SortItem(7);
	
}

void CMNG_LIB_HOLIDAY_MGR_05::OnKillfocuseDAYS8() 
{
	SortItem(8);	
}

void CMNG_LIB_HOLIDAY_MGR_05::OnKillfocuseDAYS9() 
{
	SortItem(9);
	
}

void CMNG_LIB_HOLIDAY_MGR_05::OnbINIT() 
{	
	for ( int i=1 ; i<13 ; i++ )
	{			
		( (CEdit*) GetDlgItem (GetIDC(i)) )->SetWindowText(_T(""));
	}		
}

VOID CMNG_LIB_HOLIDAY_MGR_05::SetDefaultYear()
{
	// Gain a pointer to the control

   CMonthCalCtrl* pCtrl = (CMonthCalCtrl*) GetDlgItem(IDC_MONTHCALENDAR1);
   ASSERT(pCtrl != NULL);

   // First, we must find the visible range. The array we pass to the
   // SetDayState() function must be large enough to hold days for all
   // of the visible months. Even if a month is _partially_ visible,
   // we must have MONTHDAYSTATE data for it in the array we pass.
   // GetMonthRange() returns the range of days currently visible in
   // the control, along with a count of visible months. This array
   // will be up to 2 months larger than the number of "pages" visible
   // in the control.

   SYSTEMTIME timeFrom;
   SYSTEMTIME timeUntil;
   int nCount = pCtrl->GetMonthRange(&timeFrom, &timeUntil, GMR_DAYSTATE);

   // Allocate the state array based on the return value.

   LPMONTHDAYSTATE pDayState;
   pDayState = new MONTHDAYSTATE[nCount];
   memset(pDayState, 0, sizeof(MONTHDAYSTATE) * nCount);

   // Find the first fully visible month.

   int nIndex = (timeFrom.wDay == 1) ? 0 : 1;

   // Set the 4th day, 19th day, and 26th day of the first
   // _fully_ visible month as bold.

   pDayState[nIndex] |= 1 << 3;   // 4th day
   pDayState[nIndex] |= 1 << 18;   // 19th day
   pDayState[nIndex] |= 1 << 25;   // 25th day

   // Set state and clean up

   VERIFY(pCtrl->SetDayState(nCount, pDayState));
   delete [] pDayState;   

}
HBRUSH CMNG_LIB_HOLIDAY_MGR_05::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
