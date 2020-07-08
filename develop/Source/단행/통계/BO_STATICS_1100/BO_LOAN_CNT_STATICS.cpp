// BO_LOAN_CNT_STATICS.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOAN_CNT_STATICS.h"
#include "..\..\열람\장서관리\LOC_CommonAPI\LocCommonAPI.h"
#include "BO_STATICS_11001.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOAN_CNT_STATICS dialog


CBO_LOAN_CNT_STATICS::CBO_LOAN_CNT_STATICS(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CBO_LOAN_CNT_STATICS::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOAN_CNT_STATICS)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParentGrid = NULL;
	pParentMgr = pParent;
}
CBO_LOAN_CNT_STATICS::~CBO_LOAN_CNT_STATICS()
{
	

}

VOID CBO_LOAN_CNT_STATICS::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOAN_CNT_STATICS)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_LOAN_CNT_STATICS, CDialog)
	//{{AFX_MSG_MAP(CBO_LOAN_CNT_STATICS)
	ON_BN_CLICKED(IDC_btnLoanCntStatics, OnbtnLoanCntStatics)
	ON_BN_CLICKED(IDC_btnCLOSE, OnbtnCLOSE)
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOAN_CNT_STATICS message handlers

BOOL CBO_LOAN_CNT_STATICS::OnInitDialog() 
{
	CDialog::OnInitDialog();
		
	INT ids = InitESL_Mgr( _T("SM_BO_LOAN_CNT_STATICS") );
	if( ids < 0 ) {
		AfxMessageBox ( _T("OnInitDialog Error") );
		return FALSE;
	}

	InitControl();
	
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_LOAN_CNT_STATICS::InitControl()
{
	if(_T("S") == m_pInfo->USER_PRIVILEGE)
	{
		INT nPos = 42;
		CRect rWndPos, rBtnStatics, rBtnClose;
		
		GetWindowRect(&rWndPos);
		ClientToScreen(&rWndPos);
		rWndPos.SetRect(rWndPos.left, rWndPos.top, rWndPos.right, rWndPos.bottom+nPos );
		ScreenToClient(&rWndPos);
		MoveWindow(rWndPos);

		CWnd* pWndStatics = GetDlgItem(IDC_btnLoanCntStatics);
		pWndStatics->GetWindowRect(&rBtnStatics);
		rBtnStatics.SetRect(rBtnStatics.left, rBtnStatics.top+nPos, rBtnStatics.right, rBtnStatics.bottom+nPos);
		ScreenToClient(&rBtnStatics);
		pWndStatics->MoveWindow(rBtnStatics);

		CWnd* pWndClose = GetDlgItem(IDC_btnCLOSE);
		pWndClose->GetWindowRect(&rBtnClose);
		rBtnClose.SetRect(rBtnClose.left, rBtnClose.top+nPos, rBtnClose.right, rBtnClose.bottom+nPos);
		ScreenToClient(&rBtnClose);
		pWndClose->MoveWindow(rBtnClose);

		GetDlgItem(IDC_cmbManageCode)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_staManageCode1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_staManageCode2)->ShowWindow(SW_SHOW);
	}
}

VOID CBO_LOAN_CNT_STATICS::OnbtnCLOSE() 
{
	OnCancel();	
}

VOID CBO_LOAN_CNT_STATICS::OnbtnLoanCntStatics() 
{
	RunStatics();	
}

INT CBO_LOAN_CNT_STATICS::GetControlSQL(CString &strLoanDateSql, CString &strShelfDateSql, CString &strLoanCntSql, CString &strRankSql, CString &strManageCode, CString &strClassNumSql)
{
	CESL_ControlMgr* pCM = FindCM(_T("CM_BO_LOAN_CNT_STATICS"));
	if( !pCM ){
		AfxMessageBox( _T("FindCM( CM_BO_LOAN_CNT_STATICS ) ERROR ") );
		return -1;
	}

	const INT nControlCnt = 10;
	CString strCtrlAlias[nControlCnt] =
	{
		_T("대출일1") , _T("대출일2"), _T("배가일1"), _T("배가일2"),
		_T("대출횟수1"), _T("대출횟수2"), _T("순위1"), _T("관리구분"),
		///
		_T("분류번호1"), _T("분류번호2")
	};

	CString strCtrlData[nControlCnt];

	CESL_Control_Element *pCE = NULL;

	for( INT i=0;i<4;i++ )
	{		
		pCE = (CESL_Control_Element*)pCM->FindControlElement( strCtrlAlias[i] );
		strCtrlData[i] = CLocCommonAPI::GetDate(this,pCE->CTRL_ID);
		strCtrlData[i].TrimLeft(); strCtrlData[i].TrimRight();
	}
	
	for( INT b=8;b<10;b++ )
	{		
		pCE = (CESL_Control_Element*)pCM->FindControlElement( strCtrlAlias[b] );
		strCtrlData[b] = CLocCommonAPI::GetDate(this,pCE->CTRL_ID);
		strCtrlData[b].TrimLeft(); strCtrlData[b].TrimRight();
	}


	if( strCtrlData[0].IsEmpty() && strCtrlData[2].IsEmpty() )
	{
		AfxMessageBox( _T("대출일 또는 배가일을 입력해야합니다!") );
		return 1;
	}

	for( i=4;i<nControlCnt;i++ )
	{
		pCM->GetControlMgrData( strCtrlAlias[i], strCtrlData[i] );
		strCtrlData[i].TrimLeft(); strCtrlData[i].TrimRight();
	}	
	
	if( strCtrlData[4].Compare(_T("0")) == 0 )
	{	 
		if( strCtrlData[2].IsEmpty() )
		{
			AfxMessageBox(_T("대출횟수가 0이 포함된 경우 배가일을 반드시 입력하셔야 합니다!!"));
			SetDlgItemText(IDC_edtLoanCnt1,_T("1"));
			return 1;
		}	
	}

	if( strCtrlData[4].Compare(_T("0")) == 0 )
	{		
		if( strCtrlData[6].IsEmpty() )
		{
			if(IDCANCEL==AfxMessageBox(_T("대출횟수가 0이 포함된 경우 순위를 설정하지 않으면 검색시간이 오래 걸릴 수 있습니다! 계속하시겠습니까?"), MB_OKCANCEL))
			{
				return 1;
			}			
		}
	}

	if( !strCtrlData[0].IsEmpty() )
	{
		if( strCtrlData[1].IsEmpty() )
		{
			strCtrlData[1] = strCtrlData[0];
		}

		strLoanDateSql.Format( _T("AND CL.LOAN_DATE BETWEEN TO_DATE('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') ")
			                   _T("AND TO_DATE('%s 23:59:59','YYYY/MM/DD HH24:MI:SS') "),
			                     strCtrlData[0], strCtrlData[1] );
	}
	
	if( !strCtrlData[2].IsEmpty() )
	{
		if( strCtrlData[3].IsEmpty() )
		{
			strCtrlData[3] = strCtrlData[2];
		}

		strShelfDateSql.Format( _T("AND B.SHELF_DATE BETWEEN TO_DATE('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') ")
			                    _T("AND TO_DATE('%s 23:59:59','YYYY/MM/DD HH24:MI:SS') "),
								strCtrlData[2], strCtrlData[3] );
	}

	if ( !strCtrlData[8].IsEmpty() )
	{
		if( strCtrlData[9].IsEmpty() )
		{
			strClassNumSql.Format( _T("AND B.CLASS_NO BETWEEN '%s' AND '999.9'"), strCtrlData[8]);
		}
		else
		{
		strClassNumSql.Format( _T("AND B.CLASS_NO BETWEEN '%s' AND '%s'"), strCtrlData[8], strCtrlData[9] );
		}
	}

	if( !strCtrlData[4].IsEmpty() )
	{
		if( strCtrlData[5].IsEmpty() )
		{
			strLoanCntSql.Format( _T("AND VV.CNT>=%s "), strCtrlData[4] );
		}
		else
		{
			strLoanCntSql.Format( _T("AND VV.CNT BETWEEN %s AND %s "), strCtrlData[4], strCtrlData[5] );
		}
	}
	
	if( !strCtrlData[6].IsEmpty() )
	{
		strRankSql.Format( _T("AND ROWNUM<=%s "), strCtrlData[6] );
	}

	if(_T("S") == m_pInfo->USER_PRIVILEGE)
	{
		if( !strCtrlData[7].IsEmpty() )
		{
			strManageCode.Format( _T("'%s'"), strCtrlData[7] );
		}
	}
	else
	{
		strManageCode.Format( _T("UDF_MANAGE_CODE"));
	}

	return 0;
	
	

}

INT CBO_LOAN_CNT_STATICS::RunStatics()
{
	INT		ids;

	CString strLoanDateSql, strShelfDateSql, strLoanCntSql, strRankSql, strCLManageCodeSql, strIBManageCodeSql, strManageCode, strClassNumSql;
	
	if( 0 != GetControlSQL(strLoanDateSql, strShelfDateSql, strLoanCntSql, strRankSql, strManageCode, strClassNumSql) )
		return 0;

	if(!strManageCode.IsEmpty())
	{	
		strCLManageCodeSql.Format(_T("AND CL.BOOK_MANAGE_CODE=%s "), strManageCode);
		strIBManageCodeSql.Format(_T("AND IB.MANAGE_CODE=%s "), strManageCode);
	}

	CESL_DataMgr* pDM = FindDM(_T("DM_BO_LOAN_CNT_STATICS"));
	if( !pDM ){
		AfxMessageBox(_T("CBO_LOAN_CNT_STATICS::RunStatics() ERROR -1"));
		return 0;
	}

	pDM->FreeData();

	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO( m_pInfo->CONNECTION_INFO );
	
	CString strQuery;
	CString strSecQuery;
	CString strLastQuery;
	CString strZeroQuery;
	CString strDlg1,strDlg2;

	GetDlgItemTextW(IDC_edtLoanCnt1,strDlg1);
	GetDlgItemTextW(IDC_edtLoanCnt2,strDlg2);

	if(m_sStaticsClass.Compare(_T("대출횟수통계(책두레포함)"))==0)
	{
		strQuery.Format( _T("SELECT ROWNUM, REG_NO, TITLE, AUTHOR, PUBLISHER, CNT ")
		             _T("FROM ") 	 
					 _T("(SELECT BB.REG_NO AS REG_NO, IB.TITLE AS TITLE, IB.AUTHOR AS AUTHOR, IB.PUBLISHER AS PUBLISHER, VV.CNT AS CNT ")
					 _T("FROM BO_BOOK_TBL BB, IDX_BO_TBL IB, ")
					 _T("(SELECT CL.BOOK_KEY AS REC_KEY , COUNT(1) AS CNT ")
					 _T("FROM LH_STORAGE_V01 CL, BO_BOOK_TBL B ")
					 _T("WHERE CL.BOOK_KEY=B.REC_KEY ")
					 _T("AND (CL.PUBLISH_FORM_CODE)||''='MO' ")
				    _T("AND CL.STATUS IN ('0','1','2','5','6','L','R','D') ")
					_T("%s ")
					_T("%s ")
					_T("%s ")
					_T("%s ")
					_T("GROUP BY CL.BOOK_KEY ")
					_T("ORDER BY CNT DESC) VV ")
					_T("WHERE BB.REC_KEY=VV.REC_KEY ")
					_T("AND BB.SPECIES_KEY=IB.REC_KEY ")
					_T("%s ")
					_T("%s ")
					_T("UNION ALL ")
					_T("SELECT BB.REG_NO AS REG_NO, IB.TITLE AS TITLE, IB.AUTHOR AS AUTHOR, IB.PUBLISHER AS PUBLISHER, VV.CNT AS CNT ")
					_T("FROM SE_BOOK_TBL BB, IDX_SE_TBL IB, ")
					_T("(SELECT CL.BOOK_KEY AS REC_KEY , COUNT(1) AS CNT ")
					_T("FROM LH_STORAGE_V01 CL, SE_BOOK_TBL B ")
					_T("WHERE CL.BOOK_KEY=B.REC_KEY ")
					_T("AND (CL.PUBLISH_FORM_CODE)||''='SE' ")
					_T("AND CL.STATUS IN ('0','1','2','5','6','L','R','D') ")
					_T("%s ")
					_T("%s ")
					_T("%s ")
					_T("%s")
					_T("GROUP BY CL.BOOK_KEY ")
					_T("ORDER BY CNT DESC) VV ")
					_T("WHERE BB.REC_KEY=VV.REC_KEY ")
					_T("AND BB.SPECIES_KEY=IB.REC_KEY ")
					_T("%s ")
					_T("%s "),strLoanDateSql, strShelfDateSql, strClassNumSql, strCLManageCodeSql, strIBManageCodeSql, strLoanCntSql,
					strLoanDateSql, strShelfDateSql, strClassNumSql, strCLManageCodeSql, strIBManageCodeSql, strLoanCntSql);
	}
	else
	{
		strQuery.Format( _T("SELECT ROWNUM, REG_NO, TITLE, AUTHOR, PUBLISHER, CNT ")
		             _T("FROM ") 	 
					 _T("(SELECT BB.REG_NO AS REG_NO, IB.TITLE AS TITLE, IB.AUTHOR AS AUTHOR, IB.PUBLISHER AS PUBLISHER, VV.CNT AS CNT ")
					 _T("FROM BO_BOOK_TBL BB, IDX_BO_TBL IB, ")
					 _T("(SELECT CL.BOOK_KEY AS REC_KEY , COUNT(1) AS CNT ")
					 _T("FROM LH_STORAGE_V01 CL, BO_BOOK_TBL B ")
					 _T("WHERE CL.BOOK_KEY=B.REC_KEY ")
					 _T("AND (CL.PUBLISH_FORM_CODE)||''='MO' ")
					 
					 _T("AND CL.STATUS IN ('0','1','2','5','6') ")
					 _T("%s ")
					 _T("%s ")
					 _T("%s ")			 
					 _T("%s ")
					 _T("GROUP BY CL.BOOK_KEY ")
					 _T("ORDER BY CNT DESC) VV ")
					 _T("WHERE BB.REC_KEY=VV.REC_KEY ")
					 _T("AND BB.SPECIES_KEY=IB.REC_KEY ")
					 _T("%s ")
					 _T("%s ")
                     _T("UNION ALL ")
					 _T("SELECT BB.REG_NO AS REG_NO, IB.TITLE AS TITLE, IB.AUTHOR AS AUTHOR, IB.PUBLISHER AS PUBLISHER, VV.CNT AS CNT ")
					 _T("FROM SE_BOOK_TBL BB, IDX_SE_TBL IB, ")
					 _T("(SELECT CL.BOOK_KEY AS REC_KEY , COUNT(1) AS CNT ")
					 _T("FROM LH_STORAGE_V01 CL, SE_BOOK_TBL B ")
					 _T("WHERE CL.BOOK_KEY=B.REC_KEY ")
					 _T("AND (CL.PUBLISH_FORM_CODE)||''='SE' ")
					 _T("AND CL.STATUS IN ('0','1','2','5','6') ")
					 _T("%s ")
					 _T("%s ") 
					 _T("%s ")
					 _T("%s")
					 _T("GROUP BY CL.BOOK_KEY ")
					 _T("ORDER BY CNT DESC) VV ")
					 _T("WHERE BB.REC_KEY=VV.REC_KEY ")
					 _T("AND BB.SPECIES_KEY=IB.REC_KEY ")
					 _T("%s ")
					 _T("%s "),strLoanDateSql, strShelfDateSql, strClassNumSql, strCLManageCodeSql, strIBManageCodeSql, strLoanCntSql,
					 strLoanDateSql, strShelfDateSql, strClassNumSql, strCLManageCodeSql, strIBManageCodeSql, strLoanCntSql);
	}
	
		if( ('0'==strDlg1 && '0'!=strDlg2) || _T(" ") == strDlg1)
		{
			strSecQuery.Format(_T("UNION ALL ")
				_T("SELECT B.REG_NO,IB.TITLE,IB.AUTHOR,IB.PUBLISHER,0 AS CNT ")
				_T("FROM BO_BOOK_TBL B,IDX_BO_TBL IB ")
				_T("WHERE B.SPECIES_KEY=IB.REC_KEY ")
				_T("%s ")
				_T("AND B.REG_NO IS NOT NULL ")
				_T("AND B.REG_NO NOT IN(SELECT B.REG_NO ")
				_T("FROM BO_BOOK_TBL B, ")
				_T("LH_STORAGE_V01 CL ")
				_T("WHERE  B.REG_NO(+) = CL.REG_NO ")
				_T("%s ")
				_T("AND (CL.PUBLISH_FORM_CODE)||''='MO' ")
				_T("%s ")
				_T("%s ")			
				_T("GROUP BY B.REG_NO) ")
				_T("%s ")
				_T("%s ")
				_T("UNION ALL ")
				_T("SELECT B.REG_NO,IB.TITLE,IB.AUTHOR,IB.PUBLISHER,0 AS CNT ")
				_T("FROM SE_BOOK_TBL B, IDX_SE_TBL IB ")
				_T("WHERE B.SPECIES_KEY=IB.REC_KEY ")
				_T("%s ")
				_T("AND B.REG_NO IS NOT NULL ")
				_T("AND B.REG_NO NOT IN(SELECT B.REG_NO ")
				_T("FROM SE_BOOK_TBL B,LH_STORAGE_V01 CL ")
				_T("WHERE B.REG_NO(+) = CL.REG_NO ")
				_T("%s ")
				_T("AND (CL.PUBLISH_FORM_CODE)||''='SE' ")
				_T("%s ")
				_T("%s ")		
				_T("GROUP BY B.REG_NO) ")
				_T("%s ")
				_T("%s "),strIBManageCodeSql,strCLManageCodeSql,strLoanDateSql,strShelfDateSql, strClassNumSql, strShelfDateSql,strIBManageCodeSql,strCLManageCodeSql,strLoanDateSql,strShelfDateSql, strClassNumSql, strShelfDateSql);

			strQuery += strSecQuery;
		}
		
		strLastQuery.Format( _T(" )")
			_T("WHERE 1=1 ")
			_T("%s ")
			_T("ORDER BY CNT DESC "),strRankSql);
		
		strQuery += strLastQuery;
	
		if('0'==strDlg1 && '0'==strDlg2)
		{
			strZeroQuery.Format(_T("SELECT ROWNUM,AC.REG_NO,AC.TITLE,AC.AUTHOR,AC.PUBLISHER,0 AS CNT ")
				_T("FROM ")
				_T("( ")
				_T("SELECT ROWNUM,B.REG_NO,IB.TITLE,IB.AUTHOR,IB.PUBLISHER,0 AS CNT ")
				_T("FROM BO_BOOK_TBL B,IDX_BO_TBL IB ")
				_T("WHERE B.SPECIES_KEY=IB.REC_KEY ")
				_T("%s ")
				_T("AND B.REG_NO IS NOT NULL ")
				_T("AND B.REG_NO NOT IN(SELECT B.REG_NO ")
				_T("FROM BO_BOOK_TBL B, ")
				_T("LH_STORAGE_V01 CL ")
				_T("WHERE B.REG_NO(+) = CL.REG_NO ")
				_T("%s ")
				_T("AND CL.STATUS IN ('0','1','2','5','6') ")
				_T("%s ")
				_T("%s ")			
				_T("GROUP BY B.REG_NO) ")
				_T("%s ")
				_T("%s ")
				_T("UNION ALL ")
				_T("SELECT ROWNUM,B.REG_NO,IB.TITLE,IB.AUTHOR,IB.PUBLISHER,0 AS CNT ")
				_T("FROM SE_BOOK_TBL B, IDX_SE_TBL IB ")
				_T("WHERE B.SPECIES_KEY=IB.REC_KEY ")
				_T("%s ")
				_T("AND B.REG_NO IS NOT NULL ")
				_T("AND B.REG_NO NOT IN(SELECT B.REG_NO ")
				_T("FROM SE_BOOK_TBL B, ")
				_T("LH_STORAGE_V01 CL ")
				_T("WHERE B.REG_NO(+) = CL.REG_NO ")
				_T("%s ")
				_T("AND CL.STATUS IN ('0','1','2','5','6') ")
				_T("%s ")
				_T("%s ")		
				_T("GROUP BY B.REG_NO) ")
				_T("%s ")
				_T("%s ")
				_T(") AC ")
				_T("WHERE 1=1 ")
				_T("%s ")				
				_T("ORDER BY CNT DESC "),strIBManageCodeSql,strCLManageCodeSql,strLoanDateSql,strShelfDateSql, strClassNumSql, strShelfDateSql,strIBManageCodeSql,strCLManageCodeSql,strLoanDateSql,strShelfDateSql, strClassNumSql, strShelfDateSql,strRankSql);


				strQuery = strZeroQuery;
		}					 

	ids = dm.RestructDataManager( strQuery );
	if(0>ids){
		AfxMessageBox( _T("CBO_LOAN_CNT_STATICS::RunStatics() ERROR -2") );
		return ids;
	}

	if( !m_pParentGrid ){
		AfxMessageBox( _T("CBO_LOAN_CNT_STATICS::RunStatics() ERROR -3") );
		return -3;
	}

	m_pParentGrid->Clear();

	INT nRowCnt = dm.GetRowCount();

	if( !nRowCnt )
	{
		m_pParentGrid->SetRows(2);
	}
	else
		m_pParentGrid->SetRows( nRowCnt+1 );

	m_pParentGrid->SetCols(0,pDM->RefList.GetCount()); 

	m_pParentGrid->SetTextMatrix( 0 , 0 ,  _T("순위") );
	m_pParentGrid->SetTextMatrix( 0 , 1 ,  _T("등록번호") );
	m_pParentGrid->SetTextMatrix( 0 , 2 ,  _T("서명") );
	m_pParentGrid->SetTextMatrix( 0 , 3 ,  _T("저자") );
	m_pParentGrid->SetTextMatrix( 0 , 4 ,  _T("발행자") );
	m_pParentGrid->SetTextMatrix( 0 , 5 ,  _T("대출횟수") );

	for( INT i=0;i<nRowCnt;i++ )
	{
		pDM->InsertRow(-1);
		
		CString sRank;
		sRank.Format(_T("%d"), i+1 );
		ids = pDM->SetCellData(_T("순위"), sRank, i);
		m_pParentGrid->SetTextMatrix( i+1 , 0 , sRank );
		
		ids = pDM->SetCellData(_T("등록번호"), dm.GetCellData(i,1), i);
		m_pParentGrid->SetTextMatrix( i+1 , 1 , dm.GetCellData(i,1) );
		
		ids = pDM->SetCellData(_T("서명"), dm.GetCellData(i,2), i);
		m_pParentGrid->SetTextMatrix( i+1 , 2 , dm.GetCellData(i,2) );
		
		ids = pDM->SetCellData(_T("저자"), dm.GetCellData(i,3), i);
		m_pParentGrid->SetTextMatrix( i+1 , 3 , dm.GetCellData(i,3) );
		
		ids = pDM->SetCellData(_T("발행자"), dm.GetCellData(i,4), i);
		m_pParentGrid->SetTextMatrix( i+1 , 4 , dm.GetCellData(i,4) );
		
		ids = pDM->SetCellData(_T("대출횟수"), dm.GetCellData(i,5), i);
		m_pParentGrid->SetTextMatrix( i+1 , 5 , dm.GetCellData(i,5) );
	}
	
	m_pParentGrid->SetFixedCols(1);
	m_pParentGrid->SetFixedRows(1);
	
	for( i = 0; i < m_pParentGrid->GetCols( 0 ); i++ )
		m_pParentGrid->SetColAlignment( i, 1 );
	
	m_pParentGrid->SetRedraw(TRUE);
	m_pParentGrid->Refresh();

	CBO_STATICS_11001* pMgr = (CBO_STATICS_11001*)pParentMgr;
	if( pMgr )
		pMgr->m_sLastStat = _T("대출횟수통계");

	if( !nRowCnt )
	{
		AfxMessageBox(_T("통계결과가 없습니다."));
	}
	else
	{
		AfxMessageBox(_T("완료되었습니다"));
	}
	return 0;
}




HBRUSH CBO_LOAN_CNT_STATICS::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

BOOL CBO_LOAN_CNT_STATICS::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}
