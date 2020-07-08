// SeExpectAppendixForm.cpp : implementation file
//

#include "stdafx.h"
#include "SeExpectAppendixForm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeExpectAppendixForm dialog


CSeExpectAppendixForm::CSeExpectAppendixForm(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CSeExpectAppendixForm::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeExpectAppendixForm)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_strACQ_KEY = _T("");
	m_nOpenMode = 1;
	m_pEditGrid = NULL;
	m_nCurrentGridRow = 0;
	m_pCM = NULL;
	m_pDM = NULL;
	m_pGrid = NULL;
	m_pComboFormCode = NULL; 
	m_pComboMediaCode = NULL;
	m_arrayDeleteKeyList.RemoveAll();

	m_bIsFormCodeFocused = FALSE;
	m_bIsMediaCodeFocused = FALSE;

	m_nRow = 0;
	m_nCol = 0;

	m_bIsFirstCreated = TRUE;

}

CSeExpectAppendixForm::~CSeExpectAppendixForm()
{
	FreeGridEdit();
}

VOID CSeExpectAppendixForm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeExpectAppendixForm)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSeExpectAppendixForm, CDialog)
	//{{AFX_MSG_MAP(CSeExpectAppendixForm)
	ON_CBN_KILLFOCUS(IDC_cmbEXP_APPENDIX_FORM_CODE, OnKillfocuscmbEXPAPPENDIXFORMCODE)
	ON_CBN_KILLFOCUS(IDC_cmbEXP_APPENDIX_MEDIA_CODE, OnKillfocuscmbEXPAPPENDIXMEDIACODE)
	ON_BN_CLICKED(IDC_btnEXP_APPENDIX_LINE_INSERT, OnbtnEXPAPPENDIXLINEINSERT)
	ON_BN_CLICKED(IDC_btnEXP_APPENDIX_LINE_DELETE, OnbtnEXPAPPENDIXLINEDELETE)
	ON_WM_CTLCOLOR()
	ON_MESSAGE(UPDATE_GRID_EDIT, OnUpdateGridText)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeExpectAppendixForm message handlers
BOOL CSeExpectAppendixForm::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}
BOOL CSeExpectAppendixForm::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(InitESL_Mgr(_T("연속_수서_예측_부록"))<0){
		AfxMessageBox( _T("InitESL_Mgr Error : 연속_수서_예측_부록") );
		return false;
	}

	m_pCM = FindCM(_T("CM_연속_예측_부록"));
	m_pDM = FindDM(_T("DM_연속_예측_부록"));

	if(!m_pCM || !m_pDM) {
		AfxMessageBox( _T("CM,DM Error!") );
		return false;
	}

	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("그리드"));
	m_pComboFormCode = (CComboBox*)m_pCM->FindControl(_T("형식구분"));
	m_pComboMediaCode = (CComboBox*)m_pCM->FindControl(_T("매체구분"));

	
	if(m_pComboFormCode) m_pComboFormCode->ShowWindow(SW_HIDE);
	if(m_pComboMediaCode) m_pComboMediaCode->ShowWindow(SW_HIDE);
	
	//입력 - 수정 모드에 따른 설정 
	InitOpenMode();
	ShowGrid();

	//m_Dlg_brush.CreateSolidBrush(RGB(239, 239, 239));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CSeExpectAppendixForm::InitOpenMode()
{

	//loveisverb
	/*
	if(-2 == m_nOpenMode)
	{
		//refresh
		CString strWhere;
		strWhere.Format(_T("ACQ_KEY = %s"),m_strACQ_KEY);
		m_pDM->RefreshDataManager(strWhere);

	}
	*/


	
	if(-2 == m_nOpenMode) //일반 수정
	{
		//refresh
		CString strWhere;
		strWhere.Format(_T("ACQ_KEY = %s"),m_strACQ_KEY);
		m_pDM->RefreshDataManager(strWhere);
	
	}
	else if(-1 == m_nOpenMode) //종화면 수정
	{
		
		if(m_bIsFirstCreated)
		{
			//refresh
			CString strWhere;
			strWhere.Format(_T("ACQ_KEY = %s"),m_strACQ_KEY);
			m_pDM->RefreshDataManager(strWhere);	
		}
	}

	//test
	INT nRow = m_pDM->GetRowCount();
	
	
	
	

	return 0;
}





VOID CSeExpectAppendixForm::CreateGridEdit(INT row, INT col, CString strInitValue)
{
	
	FreeGridEdit();
	m_pEditGrid = new CEditGrid;
	
	m_pEditGrid->m_nRow = row;
	m_pEditGrid->m_nCol = col;

	CWnd *pWnd = ( CWnd * )m_pGrid;

	m_pEditGrid->parent = ( CWnd * )this;
	
	m_pEditGrid->Create( ES_LEFT|ES_MULTILINE|ES_AUTOVSCROLL|WS_CHILD, CRect( 0 , 0, 10, 20 ), pWnd->GetParent(), 645 );
	
	CRect rect = GetGridCellRect();
	//m_pEditGrid->MoveWindow( rect.left - 1, rect.top - 1, rect.Width(), rect.Height() );
	m_pEditGrid->MoveWindow( rect.left, rect.top, rect.Width(), rect.Height() );


	m_pEditGrid->ShowWindow( true );
	m_pEditGrid->SetFocus();

	m_pEditGrid->SetWindowText( strInitValue );
    m_pEditGrid->SetSel( 0, -1 );

}

CRect CSeExpectAppendixForm::GetGridCellRect()
{
	CWnd *pWnd = ( CWnd * )m_pGrid;
	
	CRect rect,gridRect;
	RECT parentRect;
	pWnd->GetWindowRect( &gridRect );
	pWnd->GetParent()->GetClientRect( &parentRect );

	::ClientToScreen( pWnd->GetParent()->m_hWnd, ( POINT * )&( parentRect.left ) );

	gridRect.top -= parentRect.top;
	gridRect.left -= parentRect.left;

	rect.left   = gridRect.left + ( long )( ( float )m_pGrid->GetCellLeft()   / 15.0f );
	rect.top    = gridRect.top  + ( long )( ( float )m_pGrid->GetCellTop()    / 15.0f );
	rect.right  = rect.left     + ( long )( ( float )m_pGrid->GetCellWidth()  / 15.0f );
	rect.bottom = rect.top      + ( long )( ( float )m_pGrid->GetCellHeight() / 15.0f );
	
	return rect;
}

VOID CSeExpectAppendixForm::FreeGridEdit()
{
	if(m_pEditGrid)
	{
		delete m_pEditGrid;
		m_pEditGrid = NULL;
	}
}

VOID CSeExpectAppendixForm::OnUpdateGridText(WPARAM w, LPARAM l)
{
	CString tmp;
	INT row,col;
	BOOL bTab = FALSE;
	
	row = m_pEditGrid->m_nRow;
	col = m_pEditGrid->m_nCol;
	bTab = m_pEditGrid->m_bTab;


	if(w) //엔터와 탭키를 사용했을 경우 
	{
		INT nCurRow = m_pGrid->GetRow();
		INT nCurCol = m_pGrid->GetCol();
		
		m_pEditGrid->GetWindowText(tmp);
		
		m_pGrid->SetRow(row);
		m_pGrid->SetCol(col);
		m_pGrid->SetText(tmp);
		
		m_pGrid->SetRow(nCurRow);
		m_pGrid->SetCol(nCurCol);
	
		CString strAlias = _T("");
		strAlias = _T("부록명"); 

		m_pDM->SetCellData(strAlias,tmp,row-1);
	}

	FreeGridEdit();
		
	if(bTab)  //탭키를 사용했으면 옆간에 새로운 에디트 셀을 만들어 준다
	{
		MakeNextSubControl(row, col);
	}

}

BEGIN_EVENTSINK_MAP(CSeExpectAppendixForm, CDialog)
    //{{AFX_EVENTSINK_MAP(CSeExpectAppendixForm)
	ON_EVENT(CSeExpectAppendixForm, IDC_gridEXP_APPENDIX, -600 /* Click */, OnClickgridEXPAPPENDIX, VTS_NONE)
	ON_EVENT(CSeExpectAppendixForm, IDC_gridEXP_APPENDIX, -601 /* DblClick */, OnDblClickgridEXPAPPENDIX, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CSeExpectAppendixForm::MakeSubControl()
{
	if(0 == m_pDM->GetRowCount()) return; 

	INT nRow = m_pGrid->GetRow();
	INT nCol = m_pGrid->GetCol();
	
	CString tmp;
	tmp = m_pGrid->GetTextMatrix(nRow,nCol);
	m_pGrid->SetReverse(nRow-1);
	m_nCurrentGridRow = nRow;

	switch(nCol)
	{
	case 1:
		//부록명 
		CreateGridEdit(nRow,nCol,tmp);
		break;
	case 2: 
		//형식구분
		SetFormCode(nRow,nCol,tmp);
		break;
	case 3:
		//매체구분 
		SetMediaCode(nRow,nCol,tmp);
		break;
	case 4:
		//폐기여부
		SetDiscardYN(nRow,nCol,tmp);
		break;

	default:
		break;
	}

}


VOID CSeExpectAppendixForm::MakeNextSubControl(INT row, INT col)
{
	INT nRowCnt = m_pDM->GetRowCount();

	if(0 == nRowCnt) return; 

	if(col<3)
	{
		col++;
	}
	else if(3 == col) //마지막 컬럼 
	{
		row++;
		col = 1;
		if(row > nRowCnt) return;
	}
	else
	{
		return;
	}
	
	m_pGrid->SetCol(col);
	m_pGrid->SetRow(row);

	MakeSubControl();

}

VOID CSeExpectAppendixForm::OnClickgridEXPAPPENDIX() 
{
	// TODO: Add your control notification handler code here
	INT row = m_pGrid->GetRow();
	if (row == 0) return;
	if (row > 0) {
		
		if(m_pDM->GetRowCount()>0)
		{
			  m_nCurrentIdx = row-1;
		}
	}

}

VOID CSeExpectAppendixForm::OnDblClickgridEXPAPPENDIX() 
{
	// TODO: Add your control notification handler code here
	MakeSubControl();
}


VOID CSeExpectAppendixForm::SetFormCode(INT nRow,INT nCol,CString tmp)
{
	INT nPos = tmp.Find(_T(":"));
	CString strCode = _T("");	
	
	if(nPos > 0) {
		strCode = tmp.Left(nPos);
		strCode.TrimLeft();
		strCode.TrimRight();
	}
	
	m_pCM->SetControlMgrData(_T("형식구분"),strCode);

	CRect rect = GetGridCellRect();
	m_pComboFormCode->MoveWindow( rect.left - 2, rect.top - 3, rect.Width() + 2, rect.Height()  );

	m_pComboFormCode->ShowWindow(SW_SHOW);
	m_pComboFormCode->BringWindowToTop();
	m_pComboFormCode->SetFocus();

	m_nRow = nRow;
	m_nCol = nCol;

	m_bIsFormCodeFocused = TRUE;
	m_bTabUse = FALSE;
}

VOID CSeExpectAppendixForm::SetMediaCode(INT nRow,INT nCol,CString tmp)
{
	INT nPos = tmp.Find(_T(":"));
	CString strCode = _T("");	
	
	if(nPos > 0) {
		strCode = tmp.Left(nPos);
		strCode.TrimLeft();
		strCode.TrimRight();
	}
	
	m_pCM->SetControlMgrData(_T("매체구분"),strCode);

	CRect rect = GetGridCellRect();
	m_pComboMediaCode->MoveWindow( rect.left - 2, rect.top - 3, rect.Width() + 2, rect.Height()  );

	m_pComboMediaCode->ShowWindow(SW_SHOW);
	m_pComboMediaCode->BringWindowToTop();
	m_pComboMediaCode->SetFocus();
	
	m_nRow = nRow;
	m_nCol = nCol;

	m_bIsMediaCodeFocused = TRUE;
	m_bTabUse = FALSE;
}

VOID CSeExpectAppendixForm::SetDiscardYN(INT nRow,INT nCol,CString tmp)
{
	if(_T("Y") == tmp){
		m_pGrid->SetTextMatrix(nRow,nCol,_T("N"));
		m_pDM->SetCellData(_T("폐기여부"),_T("N"),nRow-1);

	}
	else 
	{	
		m_pGrid->SetTextMatrix(nRow,nCol,_T("Y"));
		m_pDM->SetCellData(_T("폐기여부"),_T("Y"),nRow-1);
	}

}


VOID CSeExpectAppendixForm::OnKillfocuscmbEXPAPPENDIXFORMCODE() 
{
	// TODO: Add your control notification handler code here
	m_bIsFormCodeFocused = FALSE;
	m_pComboFormCode->ShowWindow(SW_HIDE);
	SaveMultiComboData(_T("형식구분"));
}

VOID CSeExpectAppendixForm::OnKillfocuscmbEXPAPPENDIXMEDIACODE() 
{
	// TODO: Add your control notification handler code here
	m_bIsMediaCodeFocused = FALSE;
	m_pComboMediaCode->ShowWindow(SW_HIDE);
	SaveMultiComboData(_T("매체구분"));
}


VOID CSeExpectAppendixForm::SaveMultiComboData(CString alias)
{
	CString strCode,strDescrip,strValue;

	strCode = GetMultiComboData(alias, 0);
	strDescrip = GetMultiComboData(alias, 1);
	
	if(strCode.IsEmpty())  strValue = _T("");
	else strValue = strCode + _T(" : ") + strDescrip;

	
	
	INT nCurRow = m_pGrid->GetRow();
	INT nCurCol = m_pGrid->GetCol();

	
	m_pGrid->SetRow(m_nRow);
	m_pGrid->SetCol(m_nCol);
	m_pGrid->SetText(strValue);

	
	m_pGrid->SetRow(nCurRow);
	m_pGrid->SetCol(nCurCol);
	
	m_pDM->SetCellData(alias,strCode,m_nRow-1);
	m_pGrid->SetFocus();


	if(m_bTabUse) MakeNextSubControl(m_nRow, m_nCol);
}

CString CSeExpectAppendixForm::GetMultiComboData(CString alias, INT option)
{
	// option == 0 : 멀티콤보의 코드값을 가져온다. 
	// option == 1 : 멀티콤보의 코드설명값을 가져온다. 
	CString result = _T("");
	GetControlData( _T("CM_연속_예측_부록") , alias, result, -1, option );
	
	switch( option )
	{
		case 0 :
			if( _T("0") == result )
				return _T("");
			break;
		case 1 :
			if( _T("적용안함") == result )
				return _T("");
			break;
		default :
			break;
	}

	return result;
}

VOID CSeExpectAppendixForm::OnbtnEXPAPPENDIXLINEINSERT() 
{
	// TODO: Add your control notification handler code here
	INT nRowCnt =  m_pDM->GetRowCount();

	if(0 == nRowCnt)
	{
		m_pDM->InsertRow(-1);
		ShowGrid();
		m_pDM->SetCellData(_T("쿼리플래그"),_T("I"),m_pDM->GetRowCount()-1);
		m_pGrid->BringWindowToTop();
		m_pGrid->SetFocus();
		return;
	}
	
	//이전라인의 부록명이 비어 있는 지를 검사 한다. 
	CString strAPPENDIX_NAME;
	strAPPENDIX_NAME = m_pDM->GetCellData(_T("부록명"),nRowCnt-1);
	if(strAPPENDIX_NAME.IsEmpty())
	{
		AfxMessageBox(_T("이전 라인의 부록명이 입력되지 않았습니다"));
		return;
	}
		

	INT nRows =  m_pGrid->GetRows();  
	m_pGrid->SetRows( nRows + 1 );
	m_pDM->InsertRow(-1);
	m_pDM->SetCellData(_T("쿼리플래그"),_T("I"),m_pDM->GetRowCount()-1);
	m_pGrid->BringWindowToTop();
	m_pGrid->SetFocus();

}

VOID CSeExpectAppendixForm::OnbtnEXPAPPENDIXLINEDELETE() 
{
	// TODO: Add your control notification handler code here
	// 해당 라인 삭제 
	// 입력모드에서는 그냥 삭제하고 수정모드에서 데이터가 있는 경우는 detete key list에 추가
	// 황정영 추가 : 선택 라인 삭제
	INT nRowCnt = m_pDM->GetRowCount();

	if(0 == nRowCnt)
	{
		AfxMessageBox(_T("삭제할 라인이 없습니다!"));
		return;
	}
	else if(m_nCurrentIdx < 0)
	{
		AfxMessageBox(_T("선택된 정보가 없습니다."));
		return;
	}
	else
	{
		if(m_nOpenMode<0) { //수정시
			CString strREC_KEY;
			strREC_KEY = m_pDM->GetCellData(_T("부록예측KEY"),m_nCurrentIdx);
			if(!strREC_KEY.IsEmpty()) m_arrayDeleteKeyList.Add(strREC_KEY);
		}

		m_pDM->DeleteRow(m_nCurrentIdx);
		
		if(1 ==	nRowCnt)
			m_pGrid->SetRowHeight( 1, 0 );	
		else 
			ShowGrid();
	}
/*	else 
	{
		
		if(m_nOpenMode<0) { //수정시
			CString strREC_KEY;
			strREC_KEY = m_pDM->GetCellData(_T("부록예측KEY"),nRowCnt-1);
			if(!strREC_KEY.IsEmpty()) m_arrayDeleteKeyList.Add(strREC_KEY);
		}

		m_pDM->DeleteRow(nRowCnt-1);
		
		if(1 ==	nRowCnt)
			m_pGrid->SetRowHeight( 1, 0 );	
		else 
			ShowGrid();
	}
*/
	m_pGrid->BringWindowToTop();
	m_pGrid->SetFocus();

}

VOID CSeExpectAppendixForm::ShowGrid()
{
	
	ControlDisplay(_T("CM_연속_예측_부록"),_T("그리드"));
	INT nRow = m_pDM->GetRowCount();
	if(0 == nRow)  m_pGrid->SetRowHeight( 1, 0 );	

	CString strCode,strDesc;
	CString strOutput = _T("");
	for(INT i=0;i<nRow;i++)
	{
		strCode = m_pDM->GetCellData(_T("형식구분"),i);
		if(!strCode.IsEmpty())
		{
			m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("형식구분"),strCode,strDesc);
			strOutput.Format( _T("%s : %s"),strCode,strDesc);
			m_pGrid->SetTextMatrix(i+1,FORM_CODE_GRID_COL,strOutput);
		
		}
		strCode = _T("");
		strCode = m_pDM->GetCellData(_T("매체구분"),i);
		if(!strCode.IsEmpty())
		{
			m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("매체구분"),strCode,strDesc);
			strOutput.Format( _T("%s : %s"),strCode,strDesc);
			m_pGrid->SetTextMatrix(i+1,MEDIA_CODE_GRID_COL,strOutput);
		}
	}

	m_pGrid->BringWindowToTop();
	m_pGrid->SetFocus();

}

VOID CSeExpectAppendixForm::SetGridRowHeight()
{
	for( INT i = 0; i < m_pGrid->GetRows(); i++ )
		m_pGrid->SetRowHeight( i, 310 );
}


INT CSeExpectAppendixForm::SaveAllData(CESL_DataMgr* pDM_OUT,CString strACQ_TYPE,CString strACQ_KEY)
{
	CArray<CString,CString> RemoveAliasList;
	pDM_OUT->InitDBFieldData();
	CString strREC_KEY = _T("");
	
	if(m_nOpenMode>0) //입력 (INSERT)
	{ 

		for(INT i=0;i<m_pDM->GetRowCount();i++)
		{
			m_pDM->MakeRecKey(strREC_KEY);
			m_pDM->SetCellData(_T("부록예측KEY"),strREC_KEY,i);
			m_pDM->SetCellData(_T("수입구분"),strACQ_TYPE,i);
			m_pDM->SetCellData(_T("수입구분KEY"),strACQ_KEY,i);
			RemoveAliasList.RemoveAll();
			pDM_OUT->MakeQueryByDM(RemoveAliasList, m_pDM , _T("SE_APPENDIX_EXP_TBL"), i , pDM_OUT);
			strREC_KEY = _T("");
		}

	}
	else // 수정( INSERT , UPDATE , DELETE)
	{
		CString strQueryFlag = _T("");
		CString strQuery;
		
		for(INT i=0;i<m_pDM->GetRowCount();i++)
		{
			strQueryFlag = m_pDM->GetCellData(_T("쿼리플래그"),i);
			
			if(_T("I") == strQueryFlag) //INSERT
			{
				m_pDM->MakeRecKey(strREC_KEY);
				m_pDM->SetCellData(_T("부록예측KEY"),strREC_KEY,i);
				m_pDM->SetCellData(_T("수입구분"),strACQ_TYPE,i);
				m_pDM->SetCellData(_T("수입구분KEY"),strACQ_KEY,i);
				RemoveAliasList.RemoveAll();
				pDM_OUT->MakeQueryByDM(RemoveAliasList, m_pDM , _T("SE_APPENDIX_EXP_TBL"), i , pDM_OUT);
			}
			else //UPDATE
			{
				strREC_KEY = m_pDM->GetCellData(_T("부록예측KEY"),i);
				RemoveAliasList.RemoveAll();
				RemoveAliasList.Add(_T("부록예측KEY"));
				pDM_OUT->MakeQueryByDM(RemoveAliasList, m_pDM,_T("SE_APPENDIX_EXP_TBL"), i , pDM_OUT,_T("REC_KEY"),_T("NUMERIC"),strREC_KEY);
			}
			
			strQueryFlag = _T("");
			strREC_KEY = _T("");
		}

		//DELETE 
		for(INT j=0;j<m_arrayDeleteKeyList.GetSize();j++)
		{
			strREC_KEY = m_arrayDeleteKeyList.GetAt(j);
			strQuery.Format(_T("DELETE FROM SE_APPENDIX_EXP_TBL WHERE REC_KEY = %s;"),strREC_KEY);
			pDM_OUT->AddFrame(strQuery);
		}
			
	}

	return 0;
}



BOOL CSeExpectAppendixForm::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if( pMsg->message == WM_KEYDOWN )
	{
		if( pMsg->wParam == VK_RETURN )
		{
			if(m_bIsFormCodeFocused)
			{
				OnKillfocuscmbEXPAPPENDIXFORMCODE();
			}
			else if(m_bIsMediaCodeFocused)
			{
				OnKillfocuscmbEXPAPPENDIXMEDIACODE();
			}
			else
			{
				PostMessage(WM_LBUTTONDBLCLK, NULL, NULL);
			}

			return TRUE;
		}
		else if( pMsg->wParam == VK_TAB)
		{
				
			if(m_bIsFormCodeFocused)
			{
				m_bTabUse = TRUE;
				OnKillfocuscmbEXPAPPENDIXFORMCODE();
			}
			else if(m_bIsMediaCodeFocused)
			{
				m_bTabUse = TRUE;
				OnKillfocuscmbEXPAPPENDIXMEDIACODE();
			}

			return TRUE;
		}
		else if( pMsg->wParam == VK_ESCAPE)
		{
			if(m_bIsFormCodeFocused)
			{
				OnKillfocuscmbEXPAPPENDIXFORMCODE();
			}
			else if(m_bIsMediaCodeFocused)
			{
				OnKillfocuscmbEXPAPPENDIXMEDIACODE();
			}

			return TRUE;
		}
	}
	


	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CSeExpectAppendixForm::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

