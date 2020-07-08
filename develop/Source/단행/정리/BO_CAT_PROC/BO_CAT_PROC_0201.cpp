// BO_CAT_PROC_0201.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "BO_CAT_PROC_0201.h"
#include "..\..\..\ESL\SIReportMgr\SIReportManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0201 dialog


CBO_CAT_PROC_0201::CBO_CAT_PROC_0201(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_PROC_0201)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBO_CAT_PROC_0201::~CBO_CAT_PROC_0201()
{
}

VOID CBO_CAT_PROC_0201::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_PROC_0201)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_PROC_0201, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_PROC_0201)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0201 message handlers

BOOL CBO_CAT_PROC_0201::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

VOID CBO_CAT_PROC_0201::OnOK() 
{
	// TODO: Add extra validation here

	CString sRegCode;
	GetControlData( _T("CM_BO_CAT_PROC_0201"), _T("등록구분_코드"), sRegCode );
	if ( sRegCode == _T("") ) {
		AfxMessageBox( _T("등록구분을 선택하세요.") );
		return;
	}


	CString strWhere;
		CString sRegNoStart, sRegNoEnd;		

		GetControlData( _T("CM_BO_CAT_PROC_0201"), _T("등록번호부터"), sRegNoStart );
		GetControlData( _T("CM_BO_CAT_PROC_0201"), _T("등록번호까지"), sRegNoEnd );
	
		if ( sRegNoStart == _T("") || sRegNoEnd == _T("") ) {
			AfxMessageBox( _T("등록번호 범위를 입력하세요.") );
			return;
		}


		CString sStart, sEnd;

		sRegNoStart = m_pInfo->MakeRegNo(sRegCode, sRegNoStart );
		sRegNoEnd = m_pInfo->MakeRegNo(sRegCode, sRegNoEnd );

		CString sWhere;
		sWhere.Format( _T("B.REG_NO >= '%s' AND B.REG_NO <= '%s'"), sRegNoStart, sRegNoEnd );	

		CString sBookWhere;
		sBookWhere.Format( _T("SELECT DISTINCT SPECIES_KEY FROM BO_BOOK_TBL WHERE REG_NO >= '%s' AND REG_NO <='%s' AND MANAGE_CODE = UDF_MANAGE_CODE"), sRegNoStart, sRegNoEnd );
		
		strWhere.Format( _T("S.REC_KEY IN (%s)"), sBookWhere) ;

	CESL_DataMgr *pDM = FindDM( _T("DM_BO_CAT_PROC_0201") );
	if ( pDM == NULL ) return;

	if (pDM->RefreshDataManager(strWhere) < 0)	return;
	if ( pDM->GetRowCount() < 1 ) {
		AfxMessageBox (_T("검색된 결과가 없습니다."));
		return;
	}

	m_marcEditor.SubclassDlgItem(IDC_REDT_MARC, -1, this);
	m_marcEditor.Init(m_pInfo->pMarcMgr, &m_marc);

	CESL_DataMgr *pDmMarcLine = FindDM( _T("DM_BO_CAT_PROC_0201_MARC") );

	if ( pDmMarcLine == NULL ) {
		return;
	}
	pDmMarcLine->FreeData();

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
			INT iLineWordCount = 300;
			while ( sData.GetLength() > iLineWordCount ) {
				CString sDataSub = sData.Left( iLineWordCount );
				BOOL bCh = IsCanCut( sDataSub, --iLineWordCount );
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
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	if (!pSIReportManager) {
		AfxMessageBox(_T("Report Manager를 생성하지 못하였습니다."));
		return;
	}
	INT ids;	
	
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

BOOL CBO_CAT_PROC_0201::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("BO_CAT_PROC_0201")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

INT CBO_CAT_PROC_0201::SIReportEventHandler( CSIReportEventInfo * EventInfo )
{
EFS_BEGIN

	CSIReportManager *pSIReportManager = (CSIReportManager *)EventInfo->m_pSIReportManager;
	if (!pSIReportManager) return -1;

	return 0;

EFS_END
return -1;

}

BOOL CBO_CAT_PROC_0201::IsCanCut(CString sStr, INT nPos)
{
EFS_BEGIN

	INT i, nLastPos = -1;

	if(nPos > sStr.GetLength())
		return FALSE;
	TCHAR ch = sStr.GetAt(nPos);

	if(nPos + 1 == sStr.GetLength() || (ch & (0x80)) != 128)
		return TRUE;
	
	if(nPos == 0)
		if((ch & (0x80)) != 128)
			return TRUE;
		else
			return FALSE;

	for(i = 0; i < nPos; i++) {
		ch = sStr.GetAt(i);
		if((ch & (0x80)) != 128)
			nLastPos = i;
	}
	if (nLastPos == -1 && nPos % 2 == 1) return FALSE;

	return !((nPos - nLastPos) % 2);

EFS_END
return FALSE;

}

HBRUSH CBO_CAT_PROC_0201::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
