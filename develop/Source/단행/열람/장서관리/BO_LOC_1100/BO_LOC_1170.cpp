// BO_LOC_1170.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_1170.h"
#include "BO_LOC_3000Api.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_1170 dialog


CBO_LOC_1170::CBO_LOC_1170(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_1170)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ReceiptDlg = NULL;
	m_strBookKeyList = _T("");
	m_nReservationCnt = 0;
	m_pDM = NULL; 
	m_pManageValue = NULL;
}
CBO_LOC_1170::~CBO_LOC_1170()
{	
	if( m_ReceiptDlg )
		delete m_ReceiptDlg;
	m_ReceiptDlg = NULL;
}

VOID CBO_LOC_1170::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_1170)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_LOC_1170, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_1170)
	ON_BN_CLICKED(IDC_btnClose, OnbtnClose)
	ON_BN_CLICKED(IDC_bPRINT, OnbPRINT)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_1170 message handlers
BOOL CBO_LOC_1170::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CBO_LOC_1170::OnInitDialog() 
{
	CDialog::OnInitDialog();	
	EnableThemeDialogTexture(GetSafeHwnd()); 

	if(InitESL_Mgr(_T("BO_LOC_1170")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	m_pManageValue = new CLonaManageValue(this);
	m_pManageValue->pMain = pMain;
	m_pManageValue->pParentMgr = (CESL_Mgr*)this;
	m_pManageValue->SM_STYLE = 'T';
	m_pManageValue->m_pInfo = this->m_pInfo;

	CString strReceiptPrintUseYN;
	CString strValue;

	GetManageValue( _T("열람") , _T("공통") , _T("영수증출력") , _T("영수증출력사용여부") , strReceiptPrintUseYN ) ;
	GetReceiptAttestation(strValue);

	strReceiptPrintUseYN.Replace(_T(" "),_T(""));

	if ( strReceiptPrintUseYN == _T("Y") && strValue == _T("Y") ) 
	{
		( (CButton*) GetDlgItem(IDC_chkOnes) )->ShowWindow(TRUE);
		( (CButton*) GetDlgItem(IDC_bPRINT) )->ShowWindow(TRUE);
	}
	else
	{
		( (CButton*) GetDlgItem(IDC_chkOnes) )->ShowWindow(FALSE);
		( (CButton*) GetDlgItem(IDC_bPRINT) )->ShowWindow(FALSE);
	}

	m_pDM = FindDM( _T("DM_BO_LOC_1170") );	
	ShowReservationInfo();
	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CBO_LOC_1170::ShowReservationInfo()
{
	INT nIds;
	CString strWhere;
	CString strMsg;

	strMsg.Format(_T("배가된 자료중 비치희망시 자동 예약된 자료가 %d 건 있습니다."), m_nReservationCnt);
	( (CStatic*) GetDlgItem(IDC_sCount) )->SetWindowText(strMsg);

	strWhere.Format(_T("BOOK_KEY IN (%s)"), m_strBookKeyList );
	m_pDM->RefreshDataManager(strWhere);	

	INT nDmRowCnt;	
	CString strLoanUserNo;
	CString strLeftLoanUserNo;
	CString strRightLoanUserNo;
	nDmRowCnt = m_pDM->GetRowCount();
	CStringArray strArray1;
	CStringArray strArray2;
	CString strUserPhone;
	CString strUserHP;
	strArray1.RemoveAll();
	strArray2.RemoveAll();

	CString strEnc;
	CStringArray asDes1, asDes2;

	for( INT i = 0 ; i < nDmRowCnt ; i++ )
	{
		m_pDM->GetCellData(_T("대출자번호"), i, strLoanUserNo );
		m_pDM->GetCellData(_T("집전화번호"), i, strUserPhone );
		m_pDM->GetCellData(_T("핸드폰"), i, strUserHP );
		strArray1.Add(strUserPhone);
		strArray2.Add(strUserHP);
		CLocCommonAPI::ChangeSecuriyUserNo( this , strLoanUserNo , m_pManageValue->m_sIsUnityLoanService );
		m_pDM->SetCellData(_T("대출자번호"), strLoanUserNo, i );

		if(TRUE == m_pInfo->m_bLibSecuUse)
		{
			asDes1.Add(strUserPhone);
			asDes2.Add(strUserHP);
		}
	}
	
	if(TRUE == m_pInfo->m_bLibSecuUse && 0 < nDmRowCnt)
	{
		CBO_LOC_3000Api* pApi = new CBO_LOC_3000Api(this);
		if(NULL != pApi)
		{
			if(TRUE == pApi->CheckConnection())
			{
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("H_PHONE"), asDes1);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("HANDPHONE"), asDes2);
					pApi->CloseSocket();	
				}
				for(i = 0; i < nDmRowCnt; i++)
				{
					m_pDM->SetCellData(_T("집전화번호"), asDes1.GetAt(i), i);
					m_pDM->SetCellData(_T("핸드폰"), asDes2.GetAt(i), i);
				}
			}
			delete pApi;
			pApi = NULL;
		}
		asDes1.RemoveAll(); asDes1.FreeExtra();
		asDes2.RemoveAll(); asDes2.FreeExtra();
	}

	AllControlDisplay(_T("CM_BO_LOC_1170"));
	return 0;
}

VOID CBO_LOC_1170::SetBookKeyList(CStringArray *strArrayList,  INT nReservationCount)
{
	INT i;
	CString strKey;
	
	for ( i = 0 ; i < strArrayList->GetSize() ; i++ )
	{
		strKey = strArrayList->GetAt(i);
		m_strBookKeyList += strKey + _T(",");
	}

	m_strBookKeyList = m_strBookKeyList.Left(m_strBookKeyList.GetLength() -1 );
	m_nReservationCnt = nReservationCount;
}

VOID CBO_LOC_1170::OnbtnClose() 
{		
	CDialog::OnCancel();	
}

VOID CBO_LOC_1170::OnbPRINT() 
{

	BOOL bPreview;

	bPreview = ( (CButton*) GetDlgItem (IDC_chkOnes) )->GetCheck();

	if ( m_ReceiptDlg->GetSafeHwnd() == NULL )
	{
		m_ReceiptDlg = new CBO_LOC_RECEIPT_PRINT_DLG( 3, this , m_pManageValue );
		m_ReceiptDlg->pMain = this->pMain;
		m_ReceiptDlg->pParentMgr = (CESL_Mgr*)this;
		m_ReceiptDlg->m_pInfo = this->m_pInfo;				
		if (m_ReceiptDlg->Create(this))
		{
			m_ReceiptDlg->CenterWindow();
			m_ReceiptDlg->UpdateWindow();				
		}
		else
		{
			delete m_ReceiptDlg;
		}
	}
	
	INT i;

	for ( i=0 ; i<m_pDM->GetRowCount(); i++ )
	{
		m_ReceiptDlg->PrintReservationInfo(_T("DM_BO_LOC_1170"), i, bPreview);		
	}	
}

VOID CBO_LOC_1170::GetReceiptAttestation(CString &Value)
{
	CStdioFile file ;		
	CString strTemp;		
	CString strLine;
	INT		ids;
	
	strTemp = _T("..\\cfg\\ReceiptPrintCode.cfg") ;	
	if(!file.Open (strTemp,CFile::modeRead | CFile::typeBinary) ) 
	{	
		Value = _T("N");
		return;
	}
		TCHAR cUni;
		file.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			file.SeekToBegin();
		}
	
	ids = file.ReadString (strLine);	
	if ( strLine == _T("Y") )
	{		
		Value = _T("Y");
	}
	else
	{
		Value = _T("N");
	}
	file.Close();
}

HBRUSH CBO_LOC_1170::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
