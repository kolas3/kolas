// SeExpectPatternDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SeExpectPatternDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeExpectPatternDlg dialog


CSeExpectPatternDlg::CSeExpectPatternDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CSeExpectPatternDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeExpectPatternDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pCM = NULL;
	m_pDM_NAME = NULL;
	m_pDM = NULL;
	m_pDM_ALL = NULL;
	m_pGrid = NULL;
	m_pGrid_Search = NULL;
	m_pEditGrid = NULL;
	m_strOldPatternName = _T("");
	m_bIsLevelSignFocused = FALSE;
	m_bTabUse = FALSE;
}

CSeExpectPatternDlg::~CSeExpectPatternDlg()
{
	FreeGridEdit();
}


VOID CSeExpectPatternDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeExpectPatternDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSeExpectPatternDlg, CDialog)
	//{{AFX_MSG_MAP(CSeExpectPatternDlg)
	ON_BN_CLICKED(IDC_btnPATTERN_MGR_LINE_INSERT, OnbtnPATTERNMGRLINEINSERT)
	ON_BN_CLICKED(IDC_btnPATTERN_MGR_LINE_DELETE, OnbtnPATTERNMGRLINEDELETE)
	ON_BN_CLICKED(IDC_btnPATTERN_MGR_ALL_CLEAR, OnbtnPATTERNMGRALLCLEAR)
	ON_BN_CLICKED(IDC_btnPATTERN_MGR_SEARCH, OnbtnPATTERNMGRSEARCH)
	ON_BN_CLICKED(IDC_btnPATTERN_MGR_DELETE, OnbtnPATTERNMGRDELETE)
	ON_CBN_KILLFOCUS(IDC_cmbPATTERN_MGR_EXP_LEVEL_SIGN, OnKillfocuscmbPATTERNMGREXPLEVELSIGN)
	ON_MESSAGE(UPDATE_GRID_EDIT, OnUpdateGridText)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeExpectPatternDlg message handlers

BOOL CSeExpectPatternDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(InitESL_Mgr(_T("연속_수서_예측_패턴"))<0){
		AfxMessageBox( _T("InitESL_Mgr Error : 연속_수서_예측_패턴") );
		return false;
	}
	
	m_pCM = FindCM(_T("CM_연속_예측_패턴"));
	m_pDM_NAME = FindDM(_T("DM_연속_예측_패턴_패턴이름"));
	m_pDM = FindDM(_T("DM_연속_예측_패턴_작업"));
	m_pDM_ALL = FindDM(_T("DM_연속_예측_패턴_전체"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("그리드"));
	m_pGrid_Search = (CESL_Grid*)m_pCM->FindControl(_T("검색그리드"));

	if(!m_pCM || !m_pDM_NAME || !m_pDM || !m_pDM_ALL || !m_pGrid || !m_pGrid_Search) {
		AfxMessageBox( _T("CM,DM,grid Error! : 연속_수서_예측_패턴") );
		return FALSE;
	}
	
	m_pComboLevelSign = (CComboBox*)GetDlgItem(IDC_cmbPATTERN_MGR_EXP_LEVEL_SIGN);
	InitLevelSignCombo();
	ShowGrid();

	//패턴을 보여준다 
	RefreshAll();

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


VOID CSeExpectPatternDlg::InitLevelSignCombo()
{
	
	m_pComboLevelSign->InsertString(0, _T(""));
	INT nCodeCount = 0;
	nCodeCount = m_pInfo->pCodeMgr->GetCodeCount( _T("단계식별명") );
	INT startrow = 0;
	CString code[ 6 ];
	for( INT i = 0; i < nCodeCount; i++ ) 
	{
		startrow = m_pInfo->pCodeMgr->GetCode( _T("단계식별명"), code, 6, startrow );
		startrow++;	

		m_pComboLevelSign->InsertString( i + 1, code[ 3 ] );
	}

	INT nCount = m_pComboLevelSign->GetCount();

	m_pComboLevelSign->ShowWindow(SW_HIDE);
}

VOID CSeExpectPatternDlg::OnbtnPATTERNMGRLINEINSERT() 
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
	
	//이전라인의 단계명이 비어 있는 지를 검사 한다. 
	CString strLEVEL_NAME;
	strLEVEL_NAME = m_pDM->GetCellData(_T("단계명"),nRowCnt-1);
	if(strLEVEL_NAME.IsEmpty())
	{
		AfxMessageBox(_T("이전 라인의 단계명 입력되지 않았습니다"));
		return;
	}

	INT nRows =  m_pGrid->GetRows();  
	m_pGrid->SetRows( nRows + 1 );
	m_pDM->InsertRow(-1);
	m_pDM->SetCellData(_T("쿼리플래그"),_T("I"),m_pDM->GetRowCount()-1);
	m_pGrid->BringWindowToTop();
	m_pGrid->SetFocus();

}

VOID CSeExpectPatternDlg::OnbtnPATTERNMGRLINEDELETE() 
{
	// TODO: Add your control notification handler code here
	INT nRowCnt = m_pDM->GetRowCount();

	if(1 == nRowCnt && !m_strOldPatternName.IsEmpty())
	{
		AfxMessageBox(_T("기존에 존재하는 패턴의 마지막 라인은 삭제할 수 없습니다! \n 패턴 삭제를 원할 경우 패턴 삭제 버튼을 눌러 주십시오."));
		return;
	}

	if(0 == nRowCnt)
	{
		AfxMessageBox(_T("삭제할 라인이 없습니다!"));
		return;
	}
	else 
	{
		CString strREC_KEY;
		strREC_KEY = m_pDM->GetCellData(_T("권호패턴KEY"),nRowCnt-1);
		if(!strREC_KEY.IsEmpty()) m_arrayDeleteKeyList.Add(strREC_KEY);

		m_pDM->DeleteRow(nRowCnt-1);
		
		if(1 ==	nRowCnt)
			m_pGrid->SetRowHeight( 1, 0 );	
		else 
			ShowGrid();
	}

	m_pGrid->BringWindowToTop();
	m_pGrid->SetFocus();

}

VOID CSeExpectPatternDlg::OnbtnPATTERNMGRALLCLEAR() 
{
	// TODO: Add your control notification handler code here
	m_pDM->FreeData();
	m_pCM->SetControlMgrData(_T("패턴명"),_T(""));
	m_strOldPatternName = _T("");
	ShowGrid();
}

VOID CSeExpectPatternDlg::ShowGrid()
{
	m_pGrid->Display();
	if(0 == m_pDM->GetRowCount())  
		m_pGrid->SetRowHeight( 1, 0 );	
	
	m_pGrid->BringWindowToTop();
	m_pGrid->SetFocus();
}

VOID CSeExpectPatternDlg::MakeSubControl()
{
	if(0 == m_pDM->GetRowCount()) return; 

	INT nRow = m_pGrid->GetRow();
	INT nCol = m_pGrid->GetCol();
	
	CString tmp;
	tmp = m_pGrid->GetTextMatrix(nRow,nCol);
	m_pGrid->SetReverse(nRow-1);

	switch(nCol)
	{
	case 1:
	case 3:
	case 4:
	case 5:
	case 6:
		CreateGridEdit(nRow,nCol,tmp);
		break;
	case 2:
		SetLevelSign(nRow,nCol,tmp);
		break;
	default:
		break;
	}

}

VOID CSeExpectPatternDlg::MakeNextSubControl(INT row, INT col)
{
	INT nRowCnt = m_pDM->GetRowCount();

	if(0 == nRowCnt) return; 

	if(col<6)
	{
		col++;
	}
	else if(6 == col) //마지막 컬럼 
	{
		row++;
		col = 1;
		if(row > nRowCnt) return;
	}
	
	m_pGrid->SetCol(col);
	m_pGrid->SetRow(row);

	MakeSubControl();

}

VOID CSeExpectPatternDlg::FreeGridEdit()
{
	if(m_pEditGrid)
	{
		delete m_pEditGrid;
		m_pEditGrid = NULL;
	}
}

VOID CSeExpectPatternDlg::CreateGridEdit(INT row, INT col, CString strInitValue)
{
	FreeGridEdit();
	m_pEditGrid = new CEditGrid;
	
	m_pEditGrid->m_nRow = row;
	m_pEditGrid->m_nCol = col;

	CWnd *pWnd = ( CWnd * )m_pGrid;

	m_pEditGrid->parent = ( CWnd * )this;
	
	m_pEditGrid->Create( ES_LEFT|ES_MULTILINE|ES_AUTOVSCROLL|WS_CHILD, CRect( 0 , 0, 10, 20 ), pWnd->GetParent(), 645 );
	
	CRect rect = GetGridCellRect();
	m_pEditGrid->MoveWindow( rect.left - 1, rect.top - 1, rect.Width(), rect.Height() );

	m_pEditGrid->ShowWindow( true );
	m_pEditGrid->SetFocus();

	m_pEditGrid->SetWindowText( strInitValue );
    m_pEditGrid->SetSel( 0, -1 );

}

CRect CSeExpectPatternDlg::GetGridCellRect()
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

VOID CSeExpectPatternDlg::SetLevelSign(INT nRow,INT nCol,CString tmp)
{
	CRect rect = GetGridCellRect();
	m_pComboLevelSign->MoveWindow( rect.left - 2, rect.top - 3, rect.Width() + 2, rect.Height()  );

	INT idx = -1;
	CString strCurrentText;
	INT nCount = m_pComboLevelSign->GetCount();

	for(INT i=0; i<nCount;i++)
	{
		m_pComboLevelSign->SetCurSel(i);
		m_pComboLevelSign->GetWindowText(strCurrentText);
		if( tmp == strCurrentText ) {
			idx = i;
			break;
		}		
	}

	if( -1 == idx ) {
		m_pComboLevelSign->SetCurSel(0);
		m_pComboLevelSign->SetWindowText(tmp);
	}
	else
		m_pComboLevelSign->SetCurSel(idx);
	
	m_pComboLevelSign->BringWindowToTop();
	m_pComboLevelSign->ShowWindow(SW_SHOW);
	m_pComboLevelSign->SetFocus();
	m_bIsLevelSignFocused = TRUE;
	m_bTabUse = FALSE;
}

VOID CSeExpectPatternDlg::OnUpdateGridText(WPARAM w, LPARAM l)
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

		switch(col)
		{
		case 1:
			strAlias = _T("단계명"); 
			break;
		case 3:
			strAlias = _T("시작수"); 
			break;
		case 4:
			strAlias = _T("끝수"); 
			break;
		case 5:
			strAlias = _T("단계증가값"); 
			break;
		case 6:
			strAlias = _T("증가단위"); 
			break;
		default:
			break;

		}

		m_pDM->SetCellData(strAlias,tmp,row-1);
	}
	
	FreeGridEdit();
		
	if(bTab)  //탭키를 사용했으면 옆간에 새로운 에디트 셀을 만들어 준다
	{
		MakeNextSubControl(row, col);
	}
}

VOID CSeExpectPatternDlg::OnbtnPATTERNMGRSEARCH() 
{
	// TODO: Add your control notification handler code here
	RefreshAll();
}

VOID CSeExpectPatternDlg::RefreshAll()
{
	// 패턴명으로 LIKE 검색을 한다.
	CString strPATTERN_NAME = _T("");
	m_pCM->GetControlMgrData(_T("검색패턴명"),strPATTERN_NAME);	
	CString strWhere;
	if(strPATTERN_NAME.IsEmpty())
		strWhere = _T("");
	else 
		strWhere.Format(_T("PATTERN_NAME LIKE '%%%s%%'"),strPATTERN_NAME);
	
	m_pDM_ALL->RefreshDataManager(strWhere);
	m_pDM_NAME->FreeData();

	CString tmp;
	BOOL IsLegal = TRUE;
	INT nRow;

	for(INT i=0;i<m_pDM_ALL->GetRowCount();i++)
	{
		strPATTERN_NAME = m_pDM_ALL->GetCellData(_T("패턴이름"),i);
		
		for(INT j=0;j<m_pDM_NAME->GetRowCount();j++)
		{
			tmp = m_pDM_NAME->GetCellData(_T("UDF_패턴이름"),j);
			if(tmp == strPATTERN_NAME) IsLegal = FALSE;
		}

		if(IsLegal)
		{
			m_pDM_NAME->InsertRow(-1);
			nRow = m_pDM_NAME->GetRowCount()-1;
			m_pDM_NAME->SetCellData(_T("UDF_패턴이름"),strPATTERN_NAME,nRow);
		}

		IsLegal = TRUE;
	}
	
	m_pGrid_Search->Display();
	m_pDM->FreeData();
	m_strOldPatternName = _T("");
	ShowGrid();
}


BEGIN_EVENTSINK_MAP(CSeExpectPatternDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CSeExpectPatternDlg)
	ON_EVENT(CSeExpectPatternDlg, IDC_gridPATTERN_MGR, -601 /* DblClick */, OnDblClickgridPATTERNMGR, VTS_NONE)
	ON_EVENT(CSeExpectPatternDlg, IDC_gridPATTERN_MGR_SEARCH, -601 /* DblClick */, OnDblClickgridPATTERNMGRSEARCH, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CSeExpectPatternDlg::OnDblClickgridPATTERNMGR() 
{
	// TODO: Add your control notification handler code here
	MakeSubControl();
}

VOID CSeExpectPatternDlg::OnDblClickgridPATTERNMGRSEARCH() 
{
	// TODO: Add your control notification handler code here
	
	INT nRow = m_pGrid_Search->GetRow();
	if(nRow == 0) return;

	CString strPATTERN_NAME,tmp;
	strPATTERN_NAME = m_pDM_NAME->GetCellData(_T("UDF_패턴이름"),nRow-1);

	if(strPATTERN_NAME.IsEmpty()) return;
	m_pCM->SetControlMgrData(_T("패턴명"),strPATTERN_NAME);
	m_strOldPatternName = strPATTERN_NAME; 

	BOOL Isfreed = FALSE;
	
	INT nRowIdx;

	for(INT i=0;i<m_pDM_ALL->GetRowCount();i++)
	{
		tmp = m_pDM_ALL->GetCellData(_T("패턴이름"),i);

		if(tmp == strPATTERN_NAME)
		{
			if(!Isfreed) {
				m_pDM->FreeData();
				Isfreed = TRUE;
			}

			m_pDM->InsertRow(-1);
			nRowIdx = m_pDM->GetRowCount()-1;
			CopyDMToDMByAlias(m_pDM_ALL,i,m_pDM,nRowIdx);	
		}
	}
	
	m_arrayDeleteKeyList.RemoveAll();
	m_pGrid_Search->SetReverse(nRow-1);	
	ShowGrid();
}

INT CSeExpectPatternDlg::CopyDMToDMByAlias(CESL_DataMgr *pDM_SOURCE, INT SourceRowIdx, CESL_DataMgr *pDM_TARGET, INT TargetRowIdx)
{
	if (!pDM_SOURCE || !pDM_TARGET) return -1;

	INT nSourceColumnCount = pDM_SOURCE->GetRefColumnCount();
	CString field_alias, field_name, field_type, data_type, strData;
	INT ids;
	for (INT col = 0; col < nSourceColumnCount; col++) {
		strData = _T("");
		ids = pDM_SOURCE->FindColumnFieldName(col, field_alias, field_name, field_type, data_type, SourceRowIdx, strData);
		ids = pDM_TARGET->SetCellData(field_alias, strData, TargetRowIdx);
	}
	
	return 0;
}

VOID CSeExpectPatternDlg::OnOK() 
{
	// TODO: Add extra validation here
	CString strPATTERN_NAME;
	m_pCM->GetControlMgrData(_T("패턴명"),strPATTERN_NAME);
	if(strPATTERN_NAME.IsEmpty())
	{
		AfxMessageBox(_T("패턴명이 입력되지 않았습니다!"));
		return;
	}
	
	INT nRow = m_pDM->GetRowCount();
	if(0 == nRow) 
	{
		AfxMessageBox(_T("저장할 내용이 없습니다!"));
		return;
	}
	
	BOOL bIsInsert = TRUE; 
	CString strQueryFlag;

	for(INT i=0;i<m_pDM->GetRowCount();i++)
	{
		strQueryFlag = _T("");
		strQueryFlag = m_pDM->GetCellData(_T("쿼리플래그"),i);
		if(strQueryFlag != _T("I")) bIsInsert = FALSE; 
	}
	
	//새로 입력시에는 패턴이름의 중복성을 조사한다.
	//수정시에는 패턴이름이 변화되었는 지를 확인해 변화되었다면, 새 패턴이 중복인지를 파악한다.  
	if(bIsInsert)
	{
	    if(InspectDuplicatedName(strPATTERN_NAME)<0) return;
	}
	else
	{
		if(strPATTERN_NAME != m_strOldPatternName) 
		{
			if(InspectDuplicatedName(strPATTERN_NAME)<0) return;
		}
	}


	if(SaveAllData(strPATTERN_NAME)<0)
	{
		AfxMessageBox(_T("저장에 실패하였습니다!"));		
	}
	else 
	{
		AfxMessageBox(_T("저장하였습니다!"));
		RefreshAll();
	}

	return;

	//CDialog::OnOK();
}

INT CSeExpectPatternDlg::SaveAllData(CString strPATTERN_NAME)
{
	
	m_pDM->StartFrame();

	CArray<CString,CString> RemoveAliasList;
	m_pDM->InitDBFieldData();
	CString strREC_KEY;

	CString strQueryFlag;
	CString strQuery;
		
	for(INT i=0;i<m_pDM->GetRowCount();i++)
	{
		strQueryFlag = _T("");
		strREC_KEY = _T("");
		strQueryFlag = m_pDM->GetCellData(_T("쿼리플래그"),i);
			
		CString strEXP_LEVEL_NO;
		strEXP_LEVEL_NO.Format(_T("%d"),i+1);
		m_pDM->SetCellData(_T("단계번호"),strEXP_LEVEL_NO,i);
		RemoveAliasList.RemoveAll();

		if(_T("I") == strQueryFlag) //INSERT
		{
			m_pDM->MakeRecKey(strREC_KEY);
			m_pDM->SetCellData(_T("권호패턴KEY"),strREC_KEY,i);
			m_pDM->SetCellData(_T("패턴이름"),strPATTERN_NAME,i);
			m_pDM->MakeQueryByDM(RemoveAliasList, m_pDM , _T("SE_VOL_EXP_PATTERN_TBL"), i , m_pDM);
		}
		else //UPDATE
		{
			strREC_KEY = m_pDM->GetCellData(_T("권호패턴KEY"),i);
			m_pDM->SetCellData(_T("패턴이름"),strPATTERN_NAME,i);
			RemoveAliasList.Add(_T("권호패턴KEY"));
			m_pDM->MakeQueryByDM(RemoveAliasList, m_pDM, _T("SE_VOL_EXP_PATTERN_TBL"), i , m_pDM ,_T("REC_KEY"),_T("NUMERIC"),strREC_KEY);
		}

	}

	//DELETE 
	for(i=0;i<m_arrayDeleteKeyList.GetSize();i++)
	{
		strREC_KEY = _T("");
		strREC_KEY = m_arrayDeleteKeyList.GetAt(i);
		if(strREC_KEY.IsEmpty()) continue;		
		strQuery.Format(_T("DELETE FROM SE_VOL_EXP_PATTERN_TBL WHERE REC_KEY = %s;"),strREC_KEY);
		m_pDM->AddFrame(strQuery);
	}
	
	INT ids = m_pDM->SendFrame();
	m_pDM->EndFrame();
	if(ids<0) return -1;
	
	return 0;
}

INT CSeExpectPatternDlg::InspectDuplicatedName(CString strPATTERN_NAME)
{
	//패턴명의 중복성을 검사 한다 
	CString strQuery;
	CString strCount = _T("");
	INT nCount = 0;
	strQuery.Format(_T("SELECT COUNT(*) FROM SE_VOL_EXP_PATTERN_TBL WHERE PATTERN_NAME = '%s'"),strPATTERN_NAME);
	m_pDM->GetOneValueQuery(strQuery,strCount);

	if(!strCount.IsEmpty())
	{
		nCount = _ttoi(strCount.GetBuffer(0));
	}

	if(nCount>0) {
		AfxMessageBox(_T("기존에 동일한 패턴명이 존재 합니다!"));	
		return -1; 
	}

	return 0;
}

VOID CSeExpectPatternDlg::OnbtnPATTERNMGRDELETE() 
{
	// TODO: Add your control notification handler code here
	// 패턴 삭제 함수 
	INT nRow = m_pGrid_Search->GetRow();
	if(nRow == 0 || 0 == m_pDM->GetRowCount()) {
		AfxMessageBox(_T("선택된 패턴이 없습니다!"));	
		return;
	}

	CString strPATTERN_NAME,tmp;
	strPATTERN_NAME = m_pDM_NAME->GetCellData(_T("UDF_패턴이름"),nRow-1);
	if(strPATTERN_NAME.IsEmpty())
	{
		AfxMessageBox(_T("패턴이름을 찾을 수 없습니다!"));	
		return;
	}
	
	CString msg;
	msg.Format(_T("[ %s ] 패턴을 삭제 하시겠습니까?"),strPATTERN_NAME);
	if(AfxMessageBox(msg,MB_YESNO) != IDYES) return;	


	m_pDM->StartFrame();
	m_pDM->InitDBFieldData();
	CString strQuery;
	strQuery.Format(_T("DELETE FROM SE_VOL_EXP_PATTERN_TBL WHERE PATTERN_NAME = '%s';"),strPATTERN_NAME);
	m_pDM->AddFrame(strQuery);
	
	INT ids = 0;
	ids = m_pDM->SendFrame();
	m_pDM->EndFrame();

	if(ids<0)
	{
		AfxMessageBox(_T("삭제에 실패 하였습니다!"));	
	}
	else 
	{
		AfxMessageBox(_T("삭제하였습니다!"));	
		RefreshAll();
	}

}

VOID CSeExpectPatternDlg::OnKillfocuscmbPATTERNMGREXPLEVELSIGN() 
{
	// TODO: Add your control notification handler code here
	CString strLEVEL_SIGN;
	m_pComboLevelSign->GetWindowText(strLEVEL_SIGN);

	INT nRow = m_pGrid->GetRow();
	INT nCol = m_pGrid->GetCol();
	
	m_pGrid->SetTextMatrix(nRow,nCol,strLEVEL_SIGN);
	m_pComboLevelSign->ShowWindow(SW_HIDE);
	m_pGrid->BringWindowToTop();	
	m_pGrid->SetFocus();
	m_pDM->SetCellData(_T("단계식별명"),strLEVEL_SIGN,nRow-1);
	m_bIsLevelSignFocused = FALSE;

	if(m_bTabUse) MakeNextSubControl(nRow,nCol);
}

BOOL CSeExpectPatternDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message == WM_KEYDOWN )
	{
		if( pMsg->wParam == VK_RETURN )
		{
			PostMessage(WM_LBUTTONDBLCLK, NULL, NULL);
			
			if(m_bIsLevelSignFocused)
			{
				OnKillfocuscmbPATTERNMGREXPLEVELSIGN();
			}

			return TRUE;
		}
		else if( pMsg->wParam == VK_TAB)
		{
			PostMessage(WM_LBUTTONDBLCLK, NULL, NULL);
			
			if(m_bIsLevelSignFocused)
			{
				m_bTabUse = TRUE;
				OnKillfocuscmbPATTERNMGREXPLEVELSIGN();
			}

			return TRUE;
		}
		else if( pMsg->wParam == VK_ESCAPE)
		{
			if(m_bIsLevelSignFocused)
			{
				OnKillfocuscmbPATTERNMGREXPLEVELSIGN();
			}

			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CSeExpectPatternDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
