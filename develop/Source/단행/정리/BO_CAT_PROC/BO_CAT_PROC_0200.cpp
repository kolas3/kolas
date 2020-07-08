// BO_CAT_PROC_0200.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "BO_CAT_PROC_0200.h"
#include "BO_CAT_PROC_0201.h"
#include "BO_CAT_PROC_0202.h"
#include "BO_CAT_PROC_0203.h"
#include "BO_CAT_PROC_0203_01.h"
#include "BO_CAT_PROC_0204.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0200 dialog


CBO_CAT_PROC_0200::CBO_CAT_PROC_0200(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_PROC_0200)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBO_CAT_PROC_0200::~CBO_CAT_PROC_0200()
{
}

VOID CBO_CAT_PROC_0200::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_PROC_0200)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_PROC_0200, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_PROC_0200)
	ON_WM_CTLCOLOR()
//	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
//	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
//	ON_BN_CLICKED(IDC_RADIO3, OnRadio3)
//	ON_BN_CLICKED(IDC_RADIO4, OnRadio4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0200 message handlers

BOOL CBO_CAT_PROC_0200::Create(CWnd* pParentWnd)
{
EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

VOID CBO_CAT_PROC_0200::OnRadio1() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CBO_CAT_PROC_0201 dlg(this);
	dlg.DoModal();	
	

EFS_END
}

VOID CBO_CAT_PROC_0200::OnRadio2() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CBO_CAT_PROC_0202 dlg(this);
	dlg.DoModal();	

EFS_END
}

VOID CBO_CAT_PROC_0200::OnRadio3() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CBO_CAT_PROC_0203 dlg(this);
	dlg.DoModal();	

EFS_END
}

VOID CBO_CAT_PROC_0200::OnRadio4() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CBO_CAT_PROC_0204 dlg(this);
	dlg.DoModal();	

EFS_END
}

VOID CBO_CAT_PROC_0200::OnOK() 
{
EFS_BEGIN

	// TODO: Add extra validation here
	CButton *pPrint01;
	pPrint01 = (CButton *)GetDlgItem(IDC_rPrint01);
	
	CButton *pPrint02;
	pPrint02 = (CButton *)GetDlgItem(IDC_rPrint02);

	CButton *pPrint03;
	pPrint03 = (CButton *)GetDlgItem(IDC_rPrint03);

	CButton *pPrint04;
	pPrint04 = (CButton *)GetDlgItem(IDC_rPrint04);

	if ( pPrint01->GetCheck() == 1 ) {
		INT ids = SelectMakeList(_T("CM_BO_CAT_PROC_0000"), _T("ProcGrid"));
		if ( 0 != ids ) {
			AfxMessageBox ( _T("SelectMakeList error") );
		}
		INT idx = SelectGetHeadPosition(_T("CM_BO_CAT_PROC_0000"), _T("ProcGrid"));

		CBO_CAT_PROC_0201 dlg(this);
		if (idx < 0)
		{
			dlg.DoModal();
			return;
		}

		CESL_DataMgr *pDM_0000;
		pDM_0000 = FindDM(_T("DM_BO_CAT_PROC_0000"));
		if ( NULL == pDM_0000 ) {
			AfxMessageBox ( _T("DM_BO_CAT_PROC_0000 error") );
			return;
		}

		CString sSpeciesKeyAll;
		while ( idx != -1 ) {
			CString sRecKeyTmp = pDM_0000->GetCellData( _T("BS_종KEY") , idx );
			sSpeciesKeyAll = sSpeciesKeyAll + _T(", ") + sRecKeyTmp;
			idx = SelectGetNext(_T("CM_BO_CAT_PROC_0000"), _T("ProcGrid"));
		}		
		if ( sSpeciesKeyAll != _T("") ) {
			sSpeciesKeyAll = sSpeciesKeyAll.Mid(2);
		}

		CString strWhere;
		
		strWhere.Format( _T("S.REC_KEY IN (%s)"), sSpeciesKeyAll) ;

		CESL_DataMgr *pDM = FindDM( _T("DM_BO_CAT_PROC_0201") );
		if ( pDM == NULL ) return;

		if (pDM->RefreshDataManager(strWhere) < 0)	return;
		if ( pDM->GetRowCount() < 1 ) {
			AfxMessageBox (_T("검색된 결과가 없습니다."));
		}
		INT iRowCount0201 = pDM->GetRowCount();

		m_marcEditor.SubclassDlgItem(IDC_REDT_MARC, -1, this);
		m_marcEditor.Init(m_pInfo->pMarcMgr, &m_marc);
		
		CESL_DataMgr *pDmMarcLine = FindDM( _T("DM_BO_CAT_PROC_0201_MARC") );

		if ( pDmMarcLine == NULL ) {
			return;
		}
		pDmMarcLine->FreeData();

		CBO_CAT_PROC_0201 cProc0201;

		INT iCount = 0;
		INT iRowCount = pDM->GetRowCount();
		for( INT i = 0 ; i < iRowCount ; i++ ) {
			CString sStreamMarc;
			CMarc marc;
			sStreamMarc = pDM->GetCellData( _T("BS_MARC"), i );
			m_pInfo->pMarcMgr->Decoding( sStreamMarc, &m_marc );
			m_marcEditor.Display();

			CString sEditMarc;		
			GetDlgItem(IDC_REDT_MARC)->GetWindowText( sEditMarc );

			CString str;
			str.Format(_T("%c"), SUBFIELD_CODE);
			sEditMarc.Replace(str, _T("▼"));

			str.Format(_T("%c"), FIELD_TERMINATOR);
			sEditMarc.Replace(str, _T("▲"));

			CString sLeader;
			m_marc.GetLeader(sLeader);
			sEditMarc = sLeader + _T("▲") + sEditMarc;
			pDM->SetCellData( _T("EDITMARC"), sEditMarc, i );

			INT iStart = 0;
			while ( true ) {
				INT iPos = sEditMarc.Find(_T("▲"));
				if ( iPos == 0 ) {
					iPos = sEditMarc.Find(_T("\r\n"));
				}
				CString sData = sEditMarc.Mid( iStart, iPos+2 );			
				sEditMarc = sEditMarc.Mid( iPos+2 );
				sEditMarc.TrimLeft();
				INT iLineWordCount = 200;
				while ( sData.GetLength() > iLineWordCount ) {
					CString sDataSub = sData.Left( iLineWordCount );
	
					BOOL bCh = cProc0201.IsCanCut( sDataSub, --iLineWordCount );
					if ( bCh == FALSE ) {
						iLineWordCount;
						sDataSub = sData.Left( iLineWordCount );
					}
					pDmMarcLine->InsertRow(-1);
					pDmMarcLine->SetCellData( _T("MarcLine"), sDataSub, iCount );
					iCount++;
					sData = sData.Mid(iLineWordCount);
				}
				iStart = 0;
				pDmMarcLine->InsertRow(-1);
				pDmMarcLine->SetCellData( _T("MarcLine"), sData, iCount );
				iCount++;
				if ( iPos == -1 ) {
					break;
				}
			}

			pDmMarcLine->InsertRow(-1);
			pDmMarcLine->SetCellData( _T("MarcLine"), _T(""), iCount );
			iCount++;			
		}

		CSIReportManager *pSIReportManager = new CSIReportManager(this);
		pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )cProc0201.SIReportEventHandler;
		if (!pSIReportManager) {
			AfxMessageBox(_T("Report Manager를 생성하지 못하였습니다."));
			return;
		}
		
		pSIReportManager->SetCONNECTION_INFO(pDmMarcLine->CONNECTION_INFO);
		ids = pSIReportManager->GetSIReportFileFromDB(_T("입력교열지"));
		if (ids < 0) {
			AfxMessageBox(_T("DB로 부터 SI Report 정보를 가져오지 못하였습니다."));
			return;
		}

		pSIReportManager->SetDataMgr(0, pDmMarcLine);
		ids = pSIReportManager->Print();
		if (pSIReportManager) {
			delete pSIReportManager;
			pSIReportManager = NULL;
		}		
	}
	else if ( pPrint02->GetCheck() == 1 ) {
		CBO_CAT_PROC_0202 dlg(this);
		dlg.DoModal();
	}
	else if ( pPrint03->GetCheck() == 1 ) {
		INT ids = SelectMakeList(_T("CM_BO_CAT_PROC_0000"), _T("ProcGrid"));
		if ( 0 != ids ) {
			AfxMessageBox ( _T("SelectMakeList error") );
		}
		INT idx = SelectGetHeadPosition(_T("CM_BO_CAT_PROC_0000"), _T("ProcGrid"));
		if (idx < 0)
		{
			AfxMessageBox(_T("선정된 자료가 없습니다."));
			return;
		}
		CBO_CAT_PROC_0203 dlg(this);
		dlg.DoModal();
	}
	else if ( pPrint04->GetCheck() == 1 ) {
		CBO_CAT_PROC_0204 dlg(this);
		dlg.DoModal();
	}	

EFS_END
}

BOOL CBO_CAT_PROC_0200::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CButton * pPrint01;
	pPrint01=(CButton *)GetDlgItem(IDC_rPrint01);
	pPrint01->SetCheck(1);	

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}
HBRUSH CBO_CAT_PROC_0200::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
