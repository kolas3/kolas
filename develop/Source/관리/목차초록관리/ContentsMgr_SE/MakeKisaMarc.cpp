// MakeKisaMarc.cpp : implementation file
//

#include "stdafx.h"
#include "MakeKisaMarc.h"
#include "..\..\..\단행\정리\BO_CAT_PROC_KEYWORD\BO_CAT_PROC_KEYWORD_0000.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMakeKisaMarc dialog


CMakeKisaMarc::CMakeKisaMarc(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMakeKisaMarc)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pARTICLE_DM = NULL;
}

CMakeKisaMarc::~CMakeKisaMarc()
{
	
}



VOID CMakeKisaMarc::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMakeKisaMarc)
	DDX_Control(pDX, IDC_PROGRESS, m_TotalProgress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMakeKisaMarc, CDialog)
	//{{AFX_MSG_MAP(CMakeKisaMarc)
	ON_BN_CLICKED(IDC_bSearch, OnbSearch)
	ON_WM_CTLCOLOR()  
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMakeKisaMarc message handlers

BOOL CMakeKisaMarc::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// TODO: Add extra initialization here

	if(InitESL_Mgr(_T("SM_MakeKisaMarc"))<0)
	{
		AfxMessageBox(_T("InitESL_Mgr() Error~!")) ;
		return FALSE ;
	}

	m_pARTICLE_DM = FindDM ( _T("DM_ARTICLE") );
	if ( m_pARTICLE_DM == NULL ) 
	{
		AfxMessageBox ( _T("FindDM Error") );
		return FALSE;
	}	

//	OnbSearch();
//	OnOK();
//	OnCancel();
	
	EnableThemeDialogTexture(GetSafeHwnd());


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CMakeKisaMarc::Create(CWnd* pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CMakeKisaMarc::OnOK() 
{
	//마크 일괄 적용 버튼 클릭
	if ( !MakeKisaMarc() )
	{
		//AfxMessageBox( _T("기사마크 일괄 적용시 에러가 발생하였습니다."));
		return;
	}

	CDialog::OnOK();
}

VOID CMakeKisaMarc::OnCancel() 
{
	Close();	
}

VOID CMakeKisaMarc::OnbSearch() 
{
	ExeSearch();
}

BOOL CMakeKisaMarc::MakeKisaMarc()
{	
	//마크 일괄 생성..
	CString strQuery;
	CString	strValue;
	INT		nTotalCnt;
	INT		nProcCnt;
	INT ids = -1;

	strQuery = _T("SELECT '1' FROM DUAL WHERE ")
			   _T("EXISTS ( SELECT 'Y' FROM SE_ARTICLE_TBL WHERE ARTICLE_APPLY_YN = 'N' )");
	m_pARTICLE_DM->GetOneValueQuery(strQuery, strValue );
	nTotalCnt = _ttoi(strValue.GetBuffer(0));
	if ( nTotalCnt <= 0 ) return TRUE;

	strQuery = _T("SELECT count(*) FROM SE_ARTICLE_TBL S, IDX_SE_TBL I, SE_VOL_TBL V WHERE S.VOL_KEY = V.REC_KEY AND V.SPECIES_KEY = I.REC_KEY AND ARTICLE_APPLY_YN = 'N'");
	m_pARTICLE_DM->GetOneValueQuery(strQuery, strValue );
	nTotalCnt = _ttoi(strValue.GetBuffer(0));

	if ( nTotalCnt <= 0 ) return TRUE;

	m_TotalProgress.SetRange(0, nTotalCnt );
	m_TotalProgress.SetPos(0);
	m_TotalProgress.SetStep(1);

	//040 정보 
	CString strLibCode;
	GetLibInfo(&CString(_T("LIB_CODE")), &strLibCode, 1);
	
	nProcCnt =0;	
	while ( nTotalCnt >= nProcCnt )
	{		
		strQuery.Format(_T("S.VOL_KEY = V.REC_KEY AND V.SPECIES_KEY = I.REC_KEY AND ARTICLE_APPLY_YN = 'N' AND ROWNUM <= 100"));
		m_pARTICLE_DM->RefreshDataManager( strQuery );
		if ( m_pARTICLE_DM->GetRowCount() == 0 ) return FALSE;

		for ( INT nIdx = 0; nIdx < m_pARTICLE_DM->GetRowCount(); nIdx++ )
		{
			m_TotalProgress.StepIt();
			nProcCnt++;

			ids = m_pARTICLE_DM->StartFrame();
			if ( ids < 0 ) 
				return FALSE;

			CMarc *pMarc = NULL;
			pMarc = new CMarc;

			CString strStreamMarc;
			CString strItemData;
			strStreamMarc.Empty();
			
			//001 제어번호
			//m_pInfo->pMarcMgr->SetItem( pMarc, _T("001"), m_pARTICLE_DM->GetCellData( _T("ARTICLE_CONTROL_NO"), nIdx) );
			m_pInfo->pMarcMgr->InsertField(pMarc, _T("001") + m_pARTICLE_DM->GetCellData( _T("ARTICLE_CONTROL_NO"), nIdx) );

			//005 최종처리일시
			CTime t = CTime::GetCurrentTime () ;
			CString Current_Date;
			Current_Date.Format (_T("%04d%02d%02d%02d%02d%02d"),t.GetYear (),t.GetMonth (),t.GetDay (), t.GetHour(), t.GetMinute(), t.GetSecond() ) ;
			Current_Date = _T("005") + Current_Date;
			m_pInfo->pMarcMgr->DeleteField( pMarc, _T("005") );
			m_pInfo->pMarcMgr->InsertField( pMarc, Current_Date );			

			m_pInfo->pMarcMgr->GetItem(pMarc, _T("040$a"), strItemData);
			if (strItemData.IsEmpty())
			{
				m_pInfo->pMarcMgr->SetItem(pMarc, _T("040$a"), strLibCode);
				m_pInfo->pMarcMgr->SetItem(pMarc, _T("040$c"), strLibCode);
			}
			// 도서관 부호가 다른경우 040$d로 넣어준다. 반복 안되게..
			else if (strItemData != strLibCode)
			{
				// 삭제하고 넣는다.
				m_pInfo->pMarcMgr->DeleteItem(pMarc, _T("040$d"), strLibCode);
				m_pInfo->pMarcMgr->SetItem(pMarc, _T("040$d"), strLibCode);
			}

			//008 입력
			Current_Date = CTime::GetCurrentTime().Format(_T("%y%m%d")) ;
			for ( INT i = Current_Date.GetLength() ; i < 39 ; i++ )
			{
				Current_Date = Current_Date + _T(" ");
			}
			m_pInfo->pMarcMgr->SetItem( pMarc, _T("008@0-39"), Current_Date );

			//245 Tag
			m_pInfo->pMarcMgr->SetItem( pMarc, _T("245$a"), m_pARTICLE_DM->GetCellData( _T("ARTICLE_NAME"), nIdx) );
			m_pInfo->pMarcMgr->SetItem( pMarc, _T("245$d"), m_pARTICLE_DM->GetCellData( _T("ARTICLE_AUTHOR"), nIdx) );
			
			//041(언어부호) Tag
		//	strItemData = _T("");
		//	m_pInfo->pMarcMgr->GetItem( pMarc, _T("008$35@37"), strItemData );
		//	if ( !strItemData.IsEmpty() )
		//		m_pInfo->pMarcMgr->SetItem( pMarc, _T("041$a"), strItemData );


			// 연속 종의 Marc를 가져와서 입력
			CESL_DataMgr *pSeDM = FindDM ( _T("DM_CONTENTMGR_SE_TBL") );
			CString strSpeciesKey;
			CString sOption;
			//sOption.Format (_T("VOL_KEY = %s"),m_pARTICLE_DM->GetCellData(_T("VOL_KEY"),nIdx )) ;
			//pSeDM->RefreshDataManager (sOption); 
			//pSeDM->GetCellData(_T("종키"), 0, strSpeciesKey);

			sOption.Format( _T(" SELECT SPECIES_KEY FROM SE_CONTENTS_TBL WHERE VOL_KEY = %s "), m_pARTICLE_DM->GetCellData(_T("VOL_KEY"),nIdx ) );
			pSeDM->GetOneValueQuery(sOption,  strSpeciesKey);
		
			CString strQuery = _T("SELECT MARC FROM SE_SPECIES_TBL WHERE REC_KEY = '") + strSpeciesKey + _T("'");
			CString strSeTBLMarc;
			strSeTBLMarc.Empty();
			pSeDM->GetOneValueQuery( strQuery, strSeTBLMarc );
			
			CMarc *pSpeciesMarc;
			pSpeciesMarc = new CMarc;
			m_pInfo->pMarcMgr->Decoding( strSeTBLMarc , pSpeciesMarc );

			CString str052Field;
			m_pInfo->pMarcMgr->GetField( pSpeciesMarc, _T("052"), str052Field );
			if ( !str052Field.IsEmpty() )
			{
				m_pInfo->pMarcMgr->DeleteField( pMarc, _T("052") );
				str052Field = _T("052") + str052Field;
				m_pInfo->pMarcMgr->InsertField( pMarc, str052Field );
			}
		
			CString str260Field;
			m_pInfo->pMarcMgr->GetField( pSpeciesMarc, _T("260"), str260Field );
			if ( !str260Field.IsEmpty() )
			{
				m_pInfo->pMarcMgr->DeleteField( pMarc, _T("260") );
				str260Field = _T("260") + str260Field;
				m_pInfo->pMarcMgr->InsertField( pMarc, str260Field );
			}

			//773$t (잡지명 - 해당권호의 잡지명)
			strItemData = _T("");
			m_pInfo->pMarcMgr->GetItem( pSpeciesMarc, _T("245$a"), strItemData );
			if ( !strItemData.IsEmpty() )
				m_pInfo->pMarcMgr->SetItem( pMarc, _T("773$t"), strItemData );

			CString strVolQuery, strVolTitle;
			strVolQuery.Format(_T("SELECT VOL_TITLE FROM SE_VOL_TBL WHERE REC_KEY = %s "), m_pARTICLE_DM->GetCellData(_T("VOL_KEY"),nIdx ) );
			pSeDM->GetOneValueQuery(strVolQuery, strVolTitle);
					
			if ( !strVolTitle.IsEmpty() )
			{
				m_pInfo->pMarcMgr->SetItem( pMarc, _T("773$g"), strVolTitle );
			}

			//773$w ( 제어번호 - 해당 연속간행물종의 제어번호)
			//strItemData = _T("");
			//m_pInfo->pMarcMgr->GetField( pMarc, _T("245$c"), strItemData );
			//if ( !strItemData.IsEmpty() )
			CString strTmpData;
			strTmpData.Format(_T("(%s)%s"), strLibCode , m_pARTICLE_DM->GetCellData( _T("CONTROL_NO"), nIdx) );
			m_pInfo->pMarcMgr->SetItem( pMarc, _T("773$w"), strTmpData );

			CBO_CAT_PROC_KEYWORD_0000 *pDlg = NULL;
			pDlg = new CBO_CAT_PROC_KEYWORD_0000(this, *pMarc);
			pDlg->pMain = this->pMain;
			pDlg->pParentMgr = (CESL_Mgr*)this;
			pDlg->m_pInfo = this->m_pInfo;
			pDlg->Create(this);
			pDlg->UpdateWindow();
			pDlg->CenterWindow();
			pDlg->MakeKeyWord();
			strStreamMarc = pDlg->GetStreamMarc();

			CString strUpdateQuery;
			strStreamMarc.Replace(_T("'"), _T("''"));
			strUpdateQuery.Format(_T("UPDATE SE_ARTICLE_TBL SET ARTICLE = '%s', ARTICLE_APPLY_YN = 'Y'  WHERE REC_KEY = %s ;"), strStreamMarc, m_pARTICLE_DM->GetCellData( _T("REC_KEY"), nIdx));
			
			m_pARTICLE_DM->AddFrame ( strUpdateQuery );

			CString str653Item;
			m_pInfo->pMarcMgr->Decoding(strStreamMarc, pMarc);
			m_pInfo->pMarcMgr->GetField( pMarc, _T("653"), str653Item );
			str653Item.Replace(SUBFIELD_CODE, '$');
			if ( !str653Item.IsEmpty() )
			{
				str653Item.Replace(_T("$a"), _T(" "));
				str653Item = str653Item.Mid(3);			
			}

			str653Item.Replace( FIELD_TERMINATOR , ' ');	

			strUpdateQuery.Format(_T("UPDATE IDX_SE_TOC_TBL SET IDX_ARTICLE_KEYWORD = '%s' WHERE ARTICLE_CONTROL_NO = '%s' ;"), str653Item, m_pARTICLE_DM->GetCellData( _T("ARTICLE_CONTROL_NO"), nIdx) );
			m_pARTICLE_DM->AddFrame ( strUpdateQuery );

			/*
			CString strTmpQuery, strArticleKey;
			strTmpQuery.Format(_T("SELECT ARTICLE_KEY FROM SE_ARTICLE_TBL WHERE REC_KEY = '%s' "), m_pARTICLE_DM->GetCellData( _T("REC_KEY"), nIdx) );
			m_pARTICLE_DM->GetOneValueQuery( strTmpQuery, strArticleKey );
			*/

			CString system_Date ;
			system_Date.Format (_T("%d/%d/%d"),t.GetYear (),t.GetMonth (),t.GetDay ());

			strUpdateQuery.Format(_T("INSERT INTO IDX_TMP_SE_TOC_TBL(REC_KEY,ARTICLE_KEY,IDX_WORK_FLAG,INPUT_DATE) VALUES (ESL_SEQ.NEXTVAL,%s,'U','%s'); "), m_pARTICLE_DM->GetCellData( _T("REC_KEY"), nIdx), system_Date);
			m_pARTICLE_DM->AddFrame ( strUpdateQuery );

			ids = m_pARTICLE_DM->SendFrame();
			if ( ids < 0 ) 
				return FALSE;

			ids = m_pARTICLE_DM->EndFrame();
			if ( ids < 0 ) 
				return FALSE;

			pDlg->DestroyWindow();
			// KOL.UDF.022
			if ( &pDlg != NULL)
			{
				delete pDlg;
				pDlg = NULL;
			}
		}
	}		

	//AfxMessageBox( _T("정상적으로 기사 마크가 생성되었습니다.") );
	return TRUE;
}

VOID CMakeKisaMarc::ExeSearch()
{
	//검색
	/*
	CString strQuery;
	strQuery.Format(_T("S.VOL_KEY = V.REC_KEY AND V.SPECIES_KEY = I.REC_KEY AND ARTICLE_APPLY_YN = 'N' "));

	m_pARTICLE_DM->RefreshDataManager( strQuery );
	CString strCnt;
	strCnt.Format(_T("%d"), m_pARTICLE_DM->GetRowCount() );
	GetDlgItem(IDC_sCount)->SetWindowText( strCnt );
	*/

	/*
	if ( m_pARTICLE_DM->GetRowCount() < 1 ) 
	{
		AfxMessageBox ( _T("검색된 결과가 없습니다.") );
		GetDlgItem(IDOK)->EnableWindow(FALSE);
	}
	else 
	{
		CString strMessage;
		strMessage.Format(_T("총 [ %s ] 건의 기사 마크를 생성합니다."), strCnt );
		AfxMessageBox( strMessage );
		GetDlgItem(IDOK)->EnableWindow(TRUE);
	}
	*/
}

VOID CMakeKisaMarc::Close()
{
	CDialog::OnCancel();
}


INT CMakeKisaMarc::GetLibInfo(CString ArrFieldAttr[], 
						CString ArrResult[], 
						INT nFieldCount)
{
	if (!ArrFieldAttr || !ArrResult || nFieldCount < 1) return -1;

	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);

	//++2008.10.15 UPDATE BY PWR {{++
	// 통합시스템 적용
	CString strFieldAttr;
	for (INT idx = 0; idx < nFieldCount; idx++)
	{
		if (ArrFieldAttr[idx].IsEmpty()) continue;

		strFieldAttr += ArrFieldAttr[idx];
		if (idx != nFieldCount - 1)
			strFieldAttr += _T(", ");
	}
	CString strQuery;
	strQuery.Format(_T("SELECT %s FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE=UDF_MANAGE_CODE"), strFieldAttr );
	INT ids = dm.GetValuesQuery(strQuery, ArrResult, nFieldCount);
	if (ids < 0) return -1;
//	// 설정 값 가져오기
//	CString strManageValue;
//	CString strQuery = _T("SELECT VALUE_NAME FROM ESL_MANAGE_TBL ")
//			           _T("WHERE GROUP_1 = 'L' AND GROUP_2 = '기타' AND CLASS_ALIAS = '관리구분별도서관설정'");
//
//	INT ids = dm.GetOneValueQuery(strQuery, strManageValue);
//	if (ids < 0) return -1;
//
//	CString strFieldAttr;
//	for (INT idx = 0; idx < nFieldCount; idx++)
//	{
//		if (ArrFieldAttr[idx].IsEmpty()) continue;
//
//		strFieldAttr += ArrFieldAttr[idx];
//		if (idx != nFieldCount - 1)
//			strFieldAttr += _T(", ");
//	}
//
//	CString strManageCode;
//	if (strManageValue == _T("Y"))
//	{
//		strQuery.Format(_T("SELECT MANAGE_CODE FROM MN_USER_TBL WHERE REC_KEY = %s"), m_pInfo->USER_PK);
//		ids = dm.GetOneValueQuery(strQuery, strManageCode);
//		if (ids >= 0)
//		{
//			if (strManageCode.IsEmpty())
//				strQuery.Format(_T("SELECT %s FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE IS NULL"), strFieldAttr, strManageCode);
//			else
//				strQuery.Format(_T("SELECT %s FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE = '%s'"), strFieldAttr, strManageCode);
//
//			ids = dm.GetValuesQuery(strQuery, ArrResult, nFieldCount);
//		}
//	}
//	else
//	{
//		strQuery.Format(_T("SELECT %s FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE IS NULL"), strFieldAttr);
//		INT ids = dm.GetValuesQuery(strQuery, ArrResult, nFieldCount);
//	}
	//--2008.10.15 UPDATE BY PWR --}}

	return 0;
}




HBRUSH CMakeKisaMarc::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}