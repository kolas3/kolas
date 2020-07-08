// Request_StatisticDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BO_ACQ_RequestDataMgr.h"
#include "Request_StatisticDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRequest_StatisticDlg dialog


CRequest_StatisticDlg::CRequest_StatisticDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CRequest_StatisticDlg::IDD, pParent)
{
	m_pGrid=NULL;
	m_pCM = NULL;
	m_pDM = NULL;
}
CRequest_StatisticDlg::~CRequest_StatisticDlg(){

}

void CRequest_StatisticDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRequest_StatisticDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRequest_StatisticDlg, CDialog)
	//{{AFX_MSG_MAP(CRequest_StatisticDlg)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRequest_StatisticDlg message handlers



BOOL CRequest_StatisticDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("SM_희망자료통계관리")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	
	GetWindowRect(&m_rcInitial);
	m_pCM = FindCM(_T("CM_비치희망자료통계"));
	m_pDM = FindDM(_T("DM_비치희망자료통계"));
	m_pGrid = (CESL_Grid *)m_pCM->FindControl(_T("MainGrid"));
	
	if( m_pCM == NULL || m_pDM == NULL || m_pGrid == NULL ) return FALSE; 


	EnableThemeDialogTexture(GetSafeHwnd()); 
	
	return TRUE;  
}


void CRequest_StatisticDlg::OnOK() 
{
	CString start_date,end_date;

	const int nRowAlias = 5;
	
	m_pCM->GetControlMgrData(_T("통계일1"), start_date);
	m_pCM->GetControlMgrData(_T("통계일2"), end_date);
	
	if( start_date.IsEmpty() || end_date.IsEmpty() ) {
		AfxMessageBox(_T("날짜를 입력해주세요."));
		return;
	}

	INT nYear[2], nMonth[2], nDay[2];
	COleDateTime Date[2];
	_stscanf(start_date, _T("%d/%d/%d"), &nYear[0], &nMonth[0], &nDay[0]);
	_stscanf(end_date, _T("%d/%d/%d"), &nYear[1], &nMonth[1], &nDay[1]);
	
	Date[0].SetDate(nYear[0], nMonth[0], nDay[0]);
	Date[1].SetDate(nYear[1], nMonth[1], nDay[1]);

	INT nYearCnt = nYear[1] - nYear[0];
	if(0 > nYearCnt)
	{
		AfxMessageBox(_T("날짜의 범위가 잘못되었습니다."));
		return;
	}

	INT nMonthCnt = ((nYearCnt * 12) + nMonth[1]) - nMonth[0] + 1;
	if(1 > nMonthCnt)
	{
		AfxMessageBox(_T("날짜의 범위가 잘못되었습니다."));
		return;
	}

	m_pGrid->Clear();
	m_pGrid->SetRows(nRowAlias + 1 );
	m_pGrid->SetCols(0,nMonthCnt + 2);

	m_pGrid->SetColWidth(-1, 0, 1200);

	for(INT nIdx = 0; nIdx < nMonthCnt + 2; nIdx++)
	{
		m_pGrid->SetColAlignmentFixed(nIdx, 4);
		m_pGrid->SetColAlignment(nIdx, 4);
		for(INT nRow = 0; nRow < nRowAlias; nRow++)
		{
			if(0 < nRow && 0 < nIdx)
			{
				m_pGrid->SetTextMatrix(nRow, nIdx, _T("0"));
			}
		}
	}

	CString strDay;
	INT nSetYear, nSetMonth;
	for(INT nCnt = -1; nCnt < nMonthCnt-1; nCnt++)
	{		
		nSetMonth = nMonth[0] + nCnt;
		nSetYear = nYear[0] + (nSetMonth / 12);
		nSetMonth = (nSetMonth % 12) +1;
		
		strDay.Format(_T("%04d년 %02d월"), nSetYear, nSetMonth);
		m_pGrid->SetTextMatrix(0, nCnt+2, strDay);
	}
	m_pGrid->SetTextMatrix(0, nMonthCnt+1, _T("합계"));

	CString strRowAlias[nRowAlias] = {			
		_T("신청"), _T("처리"),_T("비치"),_T("취소"), _T("합계")			
	};
				
	m_pGrid->SetColAlignmentFixed(0, 4);
	for (INT i=0 ; i<nRowAlias ; i++ )
	{
		m_pGrid->SetTextMatrix(i+1,0, strRowAlias[i]);			
	}
	

	CString query;
    
	query.Format(
		_T("SELECT ")
		_T("SUBSTR(APPLICANT_DATE,1,4), ")
		_T("SUBSTR(APPLICANT_DATE,6,2), ")
		_T("FURNISH_STATUS, ")
		_T("COUNT(1) ")
		_T("FROM ")
		_T("BO_FURNISH_TBL ")
		_T("WHERE ")
		_T("MANAGE_CODE=UDF_MANAGE_CODE ")
		_T("AND APPLICANT_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')  ")
		_T("AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS') ")
		_T("GROUP BY FURNISH_STATUS ,SUBSTR(APPLICANT_DATE,1,4),SUBSTR(APPLICANT_DATE,6,2) ")
		_T("ORDER BY SUBSTR(APPLICANT_DATE,1,4), SUBSTR(APPLICANT_DATE,6,2),FURNISH_STATUS ")
		,start_date,end_date);

	m_pDM->RestructDataManager(query);

	CString year,month,status;
	CString cmAlias;
	for(int DM_i = 0 ; DM_i < m_pDM->GetRowCount() ; DM_i++){
		year = m_pDM->GetCellData(DM_i,0);		
		month = m_pDM->GetCellData(DM_i,1);
		status = m_pDM->GetCellData(DM_i,2);
		cmAlias = _T("");
		cmAlias.Format(_T("%s년 %s월"),year,month);

		SetCellGridData( cmAlias, strRowAlias[_ttoi(status) - 1 ],m_pDM->GetCellData(DM_i,3) );
	}
	

	int sum_j,sum_i,tot;
	CString val = _T(""),val2 = _T("");
	
	for ( sum_j = 1; sum_j < m_pGrid->GetColCount() ; sum_j++ )
	{
		tot = 0;
		for ( sum_i = 1; sum_i < m_pGrid->GetRows() -1  ; sum_i++ )
		{		
			val = m_pGrid->GetTextMatrix(sum_i,sum_j);
			tot += _ttoi(val);
		}
		val2.Format(_T("%d"),tot);
		m_pGrid->SetTextMatrix(m_pGrid->GetRows() -1 , sum_j,val2);
		
	}
	
	for ( sum_j = 1; sum_j < m_pGrid->GetRows() ; sum_j++ )
	{
		tot = 0;
		for ( sum_i = 1; sum_i < m_pGrid->GetColCount()  ; sum_i++ )
		{		
			val = m_pGrid->GetTextMatrix(sum_j,sum_i);
			tot += _ttoi(val);
		}
		val2.Format(_T("%d"),tot);
		m_pGrid->SetTextMatrix(sum_j, m_pGrid->GetColCount() ,val2);
		
	}

}

void CRequest_StatisticDlg::SetCellGridData(CString alias_col,CString alias_row ,CString data){
	
	INT x = -1,y = -1;

	int cols = m_pGrid->GetColCount();
	int rows = m_pGrid->GetRows();

	CString colname;
	for (int i = 0; i < cols ; i++)
	{
		colname = m_pGrid->GetTextMatrix(0 , i+1);
		if (colname == alias_col)
		{
			x = i;
			break;
		}
	}

	CString rowname;
	for (int j = 0; j < rows ; j++)
	{
		rowname = m_pGrid->GetTextMatrix(j+1,0);
		if (rowname == alias_row)
		{
			y = j;
			break;
		}
	}
	if (x > -1 && x < cols && y > -1 && y < rows)
	{
		m_pGrid->SetTextMatrix( y+1, x+1, data);
	}
	
	
}

void CRequest_StatisticDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if(m_pGrid->GetSafeHwnd() == NULL) return;
	
	CRect rcWnd;
	GetWindowRect(rcWnd);
	
	if (rcWnd.Width() < m_rcInitial.Width())
	{
		rcWnd.right = rcWnd.left + m_rcInitial.Width();
		if (rcWnd.Height() < m_rcInitial.Height())
			rcWnd.bottom = rcWnd.top + m_rcInitial.Height();	
		
		MoveWindow(rcWnd);
		return;
	}
	
	if (rcWnd.Height() < m_rcInitial.Height())
	{
		rcWnd.bottom = rcWnd.top + m_rcInitial.Height();	
		if (rcWnd.Width() < m_rcInitial.Width())
			rcWnd.right = rcWnd.left + m_rcInitial.Width();			
		
		MoveWindow(rcWnd);
		return;
	}
	
	CRect rect;
	GetClientRect(rect);
	rect.top += 51;
	rect.bottom -= 3;
	rect.left += 3;
	rect.right -= 3;
	
	m_pGrid->MoveWindow(rect);	
	
	// TODO: Add your message handler code here
}

HBRUSH CRequest_StatisticDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	return hbr;
}


void CRequest_StatisticDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();
}

BOOL CRequest_StatisticDlg::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message == WM_KEYDOWN )
	{
		if(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN) 
			OnClose();	
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

BEGIN_EVENTSINK_MAP(CRequest_StatisticDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CRequest_StatisticDlg)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

