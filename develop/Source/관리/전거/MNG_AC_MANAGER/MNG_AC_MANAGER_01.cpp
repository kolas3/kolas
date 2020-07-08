// MNG_AC_MANAGER_01.cpp : implementation file
//

#include "stdafx.h"
#include "MNG_AC_MANAGER_01.h"
#include "MNG_AC_MANAGER_02.h"
#include "MNG_AC_MANAGER_03.h"
#include "MNG_AC_MANAGER_04.h"
#include "MNG_AC_MANAGER_04_1.h"

#include "MarcExportDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_AC_MANAGER_01 dialog


CMNG_AC_MANAGER_01::CMNG_AC_MANAGER_01(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_AC_MANAGER_01)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDM = NULL;
	m_pCM = NULL;
	m_pGrid = NULL;
	m_pDlg = NULL;
	//디폴드 저자명
}

CMNG_AC_MANAGER_01::~CMNG_AC_MANAGER_01()
{
}
BOOL CMNG_AC_MANAGER_01::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	
	
VOID CMNG_AC_MANAGER_01::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_AC_MANAGER_01)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_AC_MANAGER_01, CDialog)
	//{{AFX_MSG_MAP(CMNG_AC_MANAGER_01)
	ON_WM_SIZE()
	ON_MESSAGE(IDS_MAKEDM, OnMakeDM)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_AC_MANAGER_01 message handlers

BOOL CMNG_AC_MANAGER_01::OnInitDialog() 
{
	CDialog::OnInitDialog();	
	
	if( InitESL_Mgr( _T("MNG_AC_MANAGER_01") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : MNG_AC_MANAGER_01") );
		return false;
	}
	
	m_pDM = FindDM(_T("DM_MNG_AC_MANAGER_01_MASTER"));
	if(m_pDM == NULL)
	{
		AfxMessageBox(_T("메인 DM 초기화 실패!!"));
		return FALSE;
	}
	m_pDM->FreeData();

	m_pCM = FindCM(_T("CM_MNG_AC_MANAGER_01"));
	
	if( m_pCM == NULL )
	{
		AfxMessageBox(_T("메인 CM 초기화 실패!!"));
		return FALSE;
	}

	CArray< CString, CString >RemoveListArray;
	RemoveListArray.RemoveAll();
	m_pGrid = ( CESL_Grid * )FindControl( _T("CM_MNG_AC_MANAGER_01"), (_T("그리드")) );
	
	if( m_pGrid )
	{
		//RemoveListArray.Add( _T("") );

		m_pGrid->InitPopupMenuItem();
		m_pGrid->AddPopupMenuItem( ( CView * )pMain, this->VIEW_STYLE, &RemoveListArray );
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CMNG_AC_MANAGER_01::OnMakeDM()
{
	INT nCnt = m_pDlg->m_pDM_TOTAL->GetRowCount();
	if(nCnt > 0)
	{
		//m_pDM->FreeData();
	//	for( INT i = 0 ; i<nCnt ; i++ )
	//	{
			m_pDM->InsertRow(-1);

			INT nIdx = m_pDM->GetRowCount() -1;
			CString sMarc, sTmp;
					
			sMarc = m_pDlg->m_pDM_TOTAL->GetCellData(_T("MARC"),0);
			m_pDM->SetCellData(_T("REC_KEY"),m_pDlg->m_pDM_TOTAL->GetCellData(_T("REC_KEY"),0),nIdx);
			m_pDM->SetCellData(_T("MARC"),m_pDlg->m_pDM_TOTAL->GetCellData(_T("MARC"),0),nIdx);
			m_pDM->SetCellData(_T("AC_CONTROL_NO"),m_pDlg->m_pDM_TOTAL->GetCellData(_T("AC_CONTROL_NO"),0),nIdx);
			m_pDM->SetCellData(_T("AC_TYPE"),m_pDlg->m_pDM_TOTAL->GetCellData(_T("AC_TYPE"),0),nIdx);
			m_pDM->SetCellData(_T("AC_CLASS"),m_pDlg->m_pDM_TOTAL->GetCellData(_T("AC_CLASS"),0),nIdx);
			m_pDM->SetCellData(_T("SEE_ALSO_REFERENCE"),m_pDlg->m_pDM_TOTAL->GetCellData(_T("SEE_ALSO_REFERENCE"),0),nIdx);
			//m_pDM->SetCellData(_T("채택표목"),m_pDlg->m_pDM_TOTAL->GetCellData(_T("전거표목"),i),nIdx);
			//m_pDM->SetCellData(_T("주기"),m_pDlg->m_pDM_TOTAL->GetCellData(_T("주기"),i),nIdx);
			//m_pDM->SetCellData(_T("생몰년"),m_pDlg->m_pDM_TOTAL->GetCellData(_T("생몰년"),i),nIdx);
	
			CMarc marc ;
			m_pInfo->pMarcMgr->Decoding(sMarc,&marc);

			MakeDMbyMarc(nIdx, &marc);				
	//	}
	}
		
	m_pCM->AllControlDisplay();	
}

VOID CMNG_AC_MANAGER_01::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here

	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_1 );
	
	if( NULL == pGrid->GetSafeHwnd() )
		return;

	pGrid->MoveWindow( 0, 0, cx, cy );	
}


BEGIN_EVENTSINK_MAP(CMNG_AC_MANAGER_01, CDialog)
    //{{AFX_EVENTSINK_MAP(CMNG_AC_MANAGER_01)
//	ON_EVENT(CMNG_AC_MANAGER_01, IDC_mngGRID_1, -600 /* Click */, OnClickmngGRID1, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

//DM,Grid를 리프레쉬
VOID CMNG_AC_MANAGER_01::RefreshAll()
{
	CString strWhere, strTmp;
	INT nCnt = m_pDM->GetRowCount();
	strWhere = _T(" REC_KEY IN ( ");

	for(INT i = 0; i< nCnt ; i++)
	{
		strTmp = m_pDM->GetCellData(_T("REC_KEY"),i);
		
		if(i == nCnt-1 )
			strWhere += strTmp;
		else
		{
			strWhere += strTmp;
			strWhere += _T(" , ");
		}
	}

	strWhere += _T(" )");

	m_pDM->FreeData();
	m_pDM->RefreshDataManager(strWhere);

	CMarc marc;
	for(i = 0 ; i<nCnt ; i++)
	{
		strTmp = m_pDM->GetCellData(_T("MARC"),i);
		m_pInfo->pMarcMgr->Decoding(strTmp,&marc);
		MakeDMbyMarc(i,&marc);
	}

	m_pCM->AllControlDisplay();
}

VOID CMNG_AC_MANAGER_01::ACInput()
{
EFS_BEGIN

	//입력
	CMNG_AC_MANAGER_03 Dlg(this);

	//전거통제 모드 설정: 기본 저자명
	//Dlg.SetACMode(AUTHOR_MODE);

	if( IDOK == Dlg.DoModal())
	{
		m_pDM->InsertRow(-1);

		INT nIdx = m_pDM->GetRowCount() -1;
		CString sMarc, sTmp;
				
		sMarc = Dlg.m_pDM->GetCellData(_T("MARC"),0);
		m_pDM->SetCellData(_T("REC_KEY"),Dlg.m_pDM->GetCellData(_T("REC_KEY"),0),nIdx);
		m_pDM->SetCellData(_T("MARC"),Dlg.m_pDM->GetCellData(_T("MARC"),0),nIdx);
		m_pDM->SetCellData(_T("AC_CONTROL_NO"),Dlg.m_pDM->GetCellData(_T("AC_CONTROL_NO"),0),nIdx);
		m_pDM->SetCellData(_T("AC_TYPE"),Dlg.m_pDM->GetCellData(_T("AC_TYPE"),0),nIdx);
		m_pDM->SetCellData(_T("AC_CLASS"),Dlg.m_pDM->GetCellData(_T("AC_CLASS"),0),nIdx);
		m_pDM->SetCellData(_T("SEE_ALSO_REFERENCE"),Dlg.m_pDM->GetCellData(_T("SEE_ALSO_REFERENCE"),0),nIdx);
		
		CMarc marc ;
		m_pInfo->pMarcMgr->Decoding(sMarc,&marc);

		MakeDMbyMarc(nIdx, &marc);				
	}		
	m_pCM->AllControlDisplay();

	return;

EFS_END
}

INT CMNG_AC_MANAGER_01::CopyDMbyDMAlias(CESL_DataMgr * pSourceDM)
{
	INT ids = -1;
	
	if( pSourceDM == NULL || m_pDM == NULL )
	{
		AfxMessageBox(_T("Source or Target DM Not Exist!!"));
		return -1;
	}
	
	return ids;
}
VOID CMNG_AC_MANAGER_01::ACDelete()
{
EFS_BEGIN

	INT ids = m_pGrid->SelectMakeList();
	if(ids < 0)
	{
		AfxMessageBox(_T("선택된 항목이 없습니다."));
		return;
	}
		
	INT nRowIndex = m_pGrid->SelectGetHeadPosition();
	if(nRowIndex<0)
	{
		AfxMessageBox(_T("선택된 항목이 없습니다."));
		return;
	}

	CESL_DataMgr pDMTmp;

	pDMTmp.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	pDMTmp.StartFrame();

	CString sACType, sMasterReckey, sTblName;;
	CArray<INT,INT> arrayDeletedRow;
	
	INT nACType;
	while(nRowIndex>=0)
	{

		m_pDM->GetCellData(_T("AC_TYPE"),nRowIndex,sACType);
		m_pDM->GetCellData(_T("REC_KEY"),nRowIndex,sMasterReckey);

		nACType = _ttoi(sACType);
		switch( nACType )
		{
		case 0 :
		case 1 :
		case 2 :
			sTblName = _T("AC_INDEX_AUTHOR_TBL");
			break;
		case 3 :
			sTblName = _T("AC_INDEX_TITLE_TBL");
			break;
		case 4 :
			sTblName = _T("AC_INDEX_SUBJECT_TBL");
			break;
		default :
			return;
			break;
		}

		CString strQuery;
		strQuery.Format(_T("DELETE %s WHERE AC_MASTER_MAT_KEY = %s; "), sTblName, sMasterReckey );
		INT ids = pDMTmp.AddFrame(strQuery);
		if ( ids < 0 ) return;

		strQuery.Format(_T("DELETE AC_MASTER_MAT_TBL WHERE REC_KEY = %s; "), sMasterReckey );
		ids = pDMTmp.AddFrame(strQuery);
		if ( ids < 0 ) return;

		arrayDeletedRow.Add(nRowIndex);

		nRowIndex = m_pGrid->SelectGetNext();
	}

	INT nSize = arrayDeletedRow.GetSize();
	INT nDeletedRow;

	for(INT i=0;i<nSize;i++)
	{
		nDeletedRow = arrayDeletedRow.GetAt(nSize-i-1);
		m_pDM->DeleteRow(nDeletedRow);
	}

	ids = pDMTmp.SendFrame();
	if ( ids < 0 ) return;
	
	ids = pDMTmp.EndFrame();
	if ( ids < 0 ) return;
	
	AfxMessageBox(_T("삭제되었습니다."));
	
	m_pCM->AllControlDisplay();
	return;

EFS_END
}

VOID CMNG_AC_MANAGER_01::ACSearch()
{
EFS_BEGIN
	
	CMNG_AC_MANAGER_02 Dlg(this);
	Dlg.DoModal();
	
EFS_END
}

VOID CMNG_AC_MANAGER_01::ACMarcExport()
{
EFS_BEGIN

	CMarcExportDlg Dlg( this );
	Dlg.DoModal();

	return;

EFS_END
}

VOID CMNG_AC_MANAGER_01::ACMarcImport()
{
EFS_BEGIN

	m_pDlg = new CMarcImportDlg( this );
	m_pDlg->DoModal();

	delete m_pDlg;

	return;
	
EFS_END
}

//==========================================================
// 전거 수정
//==========================================================
VOID CMNG_AC_MANAGER_01::ACUpdate()
{
EFS_BEGIN

	INT ids = m_pGrid->SelectMakeList();
	if(ids<0)
	{
		AfxMessageBox(_T("선정된 항목이 없습니다."));
		return;
	}
	
	INT idx = m_pGrid->SelectGetHeadPosition();
	if(idx<0)
	{
		AfxMessageBox(_T("선정된 항목이 없습니다."));
		return;
	}
	
	ids = m_pGrid->SelectGetCount();
	CString* sSelectList = new CString[ids];
	
	CString strTmp;
	for(INT i = 0; i<ids ; i++)
	{
		m_pDM->GetCellData(_T("REC_KEY"),idx,strTmp);
		sSelectList[i] = strTmp;
		idx = m_pGrid->SelectGetNext();
	}

	CMNG_AC_MANAGER_04_1 Dlg(this);
	Dlg.SetKeyList(sSelectList, ids);
	Dlg.DoModal();
	
	delete [] sSelectList;

	RefreshAll();

EFS_END
}

VOID CMNG_AC_MANAGER_01::MakeDMbyMarc(INT nIdx, CMarc * marc)
{
	CString sTmp;
	// 전거종류/생몰년/채택표목
	m_pDM->GetCellData(_T("AC_TYPE"), nIdx, sTmp);
	if(sTmp == _T("0") || sTmp == _T("1") || sTmp == _T("2") )
	{
		m_pDM->SetCellData(_T("전거종류"),_T("저자명"), nIdx);
		if( sTmp == _T("0"))
		{
			m_pInfo->pMarcMgr->GetItem(marc,_T("100$d"),sTmp);
			m_pDM->SetCellData(_T("생몰년"),sTmp,nIdx);
			m_pInfo->pMarcMgr->GetItem(marc,_T("100$a"),sTmp);
			m_pDM->SetCellData(_T("채택표목"),sTmp,nIdx);
		}
		else if( sTmp == _T("1"))
		{
			m_pInfo->pMarcMgr->GetItem(marc,_T("110$d"),sTmp);
			m_pDM->SetCellData(_T("생몰년"),sTmp,nIdx);
			m_pInfo->pMarcMgr->GetItem(marc,_T("110$a"),sTmp);
			m_pDM->SetCellData(_T("채택표목"),sTmp,nIdx);
		}
		else
		{
			m_pInfo->pMarcMgr->GetItem(marc,_T("111$d"),sTmp);
			m_pDM->SetCellData(_T("생몰년"),sTmp,nIdx);
			m_pInfo->pMarcMgr->GetItem(marc,_T("111$a"),sTmp);
			m_pDM->SetCellData(_T("채택표목"),sTmp,nIdx);
		}
	}
	else if(sTmp == _T("3"))
	{
		m_pDM->SetCellData(_T("전거종류"), _T("서명"), nIdx);
		m_pInfo->pMarcMgr->GetItem(marc,_T("130$a"),sTmp);
		m_pDM->SetCellData(_T("채택표목"),sTmp,nIdx);
	}
	else
	{
		m_pDM->SetCellData(_T("전거종류"), _T("주제명"), nIdx);
		m_pInfo->pMarcMgr->GetItem(marc,_T("150$a"),sTmp);
		m_pDM->SetCellData(_T("채택표목"),sTmp,nIdx);
	}
	//주기
	m_pInfo->pMarcMgr->GetItem(marc,_T("678$a"),sTmp);
			m_pDM->SetCellData(_T("주기"),sTmp,nIdx);
}
HBRUSH CMNG_AC_MANAGER_01::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}
