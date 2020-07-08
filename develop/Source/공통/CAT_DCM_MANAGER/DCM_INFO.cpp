// DCM_INFO.cpp : implementation file
//

#include "stdafx.h"
#include "DCM_INFO.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define BINPUT 0
#define BMODIFY 1
#define GMODIFY 2

/////////////////////////////////////////////////////////////////////////////
// CDCM_INFO dialog


CDCM_INFO::CDCM_INFO(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CDCM_INFO::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDCM_INFO)
	m_nActive = 0;
	m_nPassive = 0;
	//}}AFX_DATA_INIT
	m_pCM = NULL;
	m_pDM = NULL;
	m_nOpenMode = BINPUT; // 0(입력), 1(수정)
	m_bActiveChange = FALSE;
	m_strDbFlag = _T("M");
	m_strParentFileType = _T("PDF");
}

CDCM_INFO::~CDCM_INFO()
{

}

void CDCM_INFO::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDCM_INFO)
	DDX_Control(pDX, IDC_ipaddOutIP, m_ipaddOutIP);
	DDX_Control(pDX, IDC_ipaddInIP, m_ipaddInIP);
	DDX_Radio(pDX, IDC_rActiveY, m_nActive);
	DDX_Radio(pDX, IDC_rPassive, m_nPassive);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDCM_INFO, CDialog)
	//{{AFX_MSG_MAP(CDCM_INFO)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_btnSave, OnbtnSave)
	ON_BN_CLICKED(IDC_btnRre, OnbtnRre)
	ON_BN_CLICKED(IDC_btnNext, OnbtnNext)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDCM_INFO message handlers

BOOL CDCM_INFO::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd()); 

	if( _T("M") == m_strDbFlag )
	{
		if (InitESL_Mgr(_T("SM_BO_DCM_INFO")) < 0)
		{
			ESLAfxMessageBox(_T("InitESL_Mgr Error"));
			return FALSE;
		}	

		m_pCM = FindCM(_T("CM_BO_DCM_INFO"));
	}
	else if( _T("Y") == m_strDbFlag )
	{
		if (InitESL_Mgr(_T("SM_SE_DCM_INFO")) < 0)
		{
			ESLAfxMessageBox(_T("InitESL_Mgr Error"));
			return FALSE;
		}	

		m_pCM = FindCM(_T("CM_SE_DCM_INFO"));
	}
	else if( _T("A") == m_strDbFlag )
	{
		if (InitESL_Mgr(_T("SM_SE_ATICLE_DCM_INFO")) < 0)
		{
			ESLAfxMessageBox(_T("InitESL_Mgr Error"));
			return FALSE;
		}	

		m_pCM = FindCM(_T("CM_SE_ATICLE_DCM_INFO"));
	}
	else
	{
		AfxMessageBox(_T("DB FLAG를 설정해주세요."));
		return FALSE;
	}

	if(!Init())
	{
		ESLAfxMessageBox(_T("관리화면을 초기화 할 수 없습니다."));
		EndDialog(IDCANCEL);
		return FALSE;
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDCM_INFO::Init()
{
	CButton* pBtn	= (CButton*)GetDlgItem(IDC_btnRre);
	CButton* pBtn1	= (CButton*)GetDlgItem(IDC_btnNext);
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_cmbFileType);
	
	if( m_nOpenMode == BINPUT )
	{
		pBtn->ShowWindow(SW_HIDE);
		pBtn1->ShowWindow(SW_HIDE);
		SetWindowText(_T("입력"));
		pCmb->SelectString(0,m_strParentFileType);
		//===================================================
		//2009.04.21 UPDATE BY PJW : 현재는 PDF만 사용함으로 콤보박스를 변경할수 없도록 한다.
// 		pCmb->EnableWindow(TRUE);
		pCmb->EnableWindow(FALSE);
		//===================================================
		
	}
	else
	{
		pBtn->ShowWindow(SW_SHOW);
		pBtn1->ShowWindow(SW_SHOW);
		SetWindowText(_T("수정"));
		Display(0);				
	}
	return TRUE;
}

HBRUSH CDCM_INFO::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CDCM_INFO::OnbtnSave() 
{
	CString strPw;
	CString strChkPw;
	CEdit* pEditPw = (CEdit*)GetDlgItem(IDC_edtPW);
	CEdit* pEditChkPw = (CEdit*)GetDlgItem(IDC_edtChkPW);
	pEditPw->GetWindowText(strPw);
	pEditChkPw->GetWindowText(strChkPw);

	if( strPw.IsEmpty() )
	{
		AfxMessageBox(_T("비밀번호를 입력해 주세요."));

		pEditPw->SetFocus();

		return;
	}
	else if( strPw != strChkPw )
	{
		pEditPw->SetWindowText(_T(""));
		pEditChkPw->SetWindowText(_T(""));

		AfxMessageBox(_T("비밀번호가 다릅니다."));

		pEditPw->SetFocus();

		return;
	}

	if( m_nOpenMode == BINPUT )
	{
		OnSave(0);
		AfxMessageBox(_T("저장 하였습니다."));
// 		m_pParentGrid->DisplayLine(m_pDM->GetRowCount()-1);

		m_pCM->AllControlClear();

		CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_cmbFileType);
		pCmb->SelectString(0,m_strParentFileType);

		m_ipaddInIP.ClearAddress();
		m_ipaddOutIP.ClearAddress();

		CButton* pBtnPassiveY	= (CButton*)GetDlgItem(IDC_rPassive);
		CButton* pBtnPassiveN	= (CButton*)GetDlgItem(IDC_rActive);
		CButton* pBtnActiveY	= (CButton*)GetDlgItem(IDC_rActiveY);
		CButton* pBtnActiveN	= (CButton*)GetDlgItem(IDC_rActiveN);
		
		pBtnPassiveY->SetCheck(1);
 		pBtnPassiveN->SetCheck(0);
		pBtnActiveY->SetCheck(1);
 		pBtnActiveN->SetCheck(0);

		m_nPassive = 0;
		m_nActive = 0;
	}
	else
	{
		OnSave(1);
		OnbtnNext() ;
		SetDmToCm( m_nIdx );
		AfxMessageBox(_T("저장 하였습니다."));
	}
	
	m_pParentGrid->SelectMakeList();
	m_pParentGrid->SetProtectSelectColumn(TRUE);
	m_pParentGrid->Display();
	m_pParentGrid->SetProtectSelectColumn(FALSE);
	m_pParentGrid->SetReverse(m_nIdx);
}

void CDCM_INFO::OnbtnRre() 
{
	Display(-1);	
}

void CDCM_INFO::OnbtnNext() 
{
	Display(1);	
}

INT CDCM_INFO::GetFirstSelectIndex()
{
	INT nFirstSelIdx = -1;

	if( m_nOpenMode == GMODIFY )
	{
		nFirstSelIdx = m_pParentGrid->GetRow() - 1;
	}
	else if(m_nOpenMode == BMODIFY)
	{
		if( m_bGridSelectYN == TRUE )	nFirstSelIdx = m_nFirstIdx;
		else							nFirstSelIdx = m_pParentGrid->GetRow() - 1;
	}

	if( nFirstSelIdx < 0 )	nFirstSelIdx = 0;

	return nFirstSelIdx;
}

/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 그리드의 이전 인덱스를 찾는다.
// [ 미니스펙 ] - 생략
/////////////////////////////////////////////////////////////////////////////
INT CDCM_INFO::GetPrevSelectIndex()
{
	INT nPrevSelIndex = 0;
	
	if( m_bGridSelectYN == TRUE )
	{
		while( 1 )
		{
			if(nPrevSelIndex == m_nIdx || nPrevSelIndex < 0 )
			{
				break;
			}
			else if( nPrevSelIndex < m_nIdx )
			{
				nPrevSelIndex = m_pParentGrid->SelectGetNext();
				if( nPrevSelIndex < 0 )
				{
					nPrevSelIndex = m_pParentGrid->SelectGetTailPosition();
					break;
				}
			}
			else if( nPrevSelIndex > m_nIdx )
			{
				nPrevSelIndex = m_pParentGrid->SelectGetPrev();
			}
		}
	
		nPrevSelIndex = m_pParentGrid->SelectGetPrev();
		if( nPrevSelIndex < 0 )		
		{
			nPrevSelIndex = m_pParentGrid->SelectGetHeadPosition();
		}
	}
	else
	{
		if( m_nIdx != m_nFirstIdx )	
		{
			nPrevSelIndex = m_nIdx - 1;
		}
	}

	return nPrevSelIndex;
}

/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 그리드의 다음 인덱스를 찾는다.
// [ 미니스펙 ] - 생략
/////////////////////////////////////////////////////////////////////////////
INT CDCM_INFO::GetNextSelectIndex()
{
 	INT nNextSelIndex = 0;

	if( m_bGridSelectYN == TRUE )
	{
		while( 1 )
		{
			if(nNextSelIndex == m_nIdx || nNextSelIndex < 0 )
			{
				break;
			}
			else if( nNextSelIndex < m_nIdx )
			{
				nNextSelIndex = m_pParentGrid->SelectGetNext();
			}
			else if( nNextSelIndex > m_nIdx )
			{
				nNextSelIndex = m_pParentGrid->SelectGetPrev();
			}
		}
	
		nNextSelIndex = m_pParentGrid->SelectGetNext();
		if(nNextSelIndex < 0)		
		{
			nNextSelIndex = m_pParentGrid->SelectGetTailPosition();
		}
		
	}
	else
	{
		if( m_nIdx != m_nLastIdx )	
		{
			nNextSelIndex = m_nIdx + 1;
		}
		else 
		{
			nNextSelIndex = m_nIdx;
		}
	}

	return nNextSelIndex;
}


VOID CDCM_INFO::Display(INT nIdx)
{
	switch( nIdx )
	{
	// 선정 체크표시 Refresh - 첫번째 인덱스를 설정
	case 0:
		{
		InitFirstLastIndex();
		m_nIdx = GetFirstSelectIndex();
		ShowNextPrevButton(m_nIdx);
		}
		break;
	// 인덱스를 이전 체크표시로 설정
	case -1:
		{
		m_nIdx = GetPrevSelectIndex();
		ShowNextPrevButton(m_nIdx);
		}
		break;
	// 인덱스를 다음 체크표시로 설정
	case 1:
		{
		m_nIdx = GetNextSelectIndex();
		ShowNextPrevButton(m_nIdx);
		}
		break;
	}

	SetDmToCm( m_nIdx );
	
	m_pParentGrid->SetReverse( m_nIdx );

	CString strPw;

	CEdit* pEditPw		= (CEdit*)GetDlgItem( IDC_edtPW );
	CEdit* pEditChkPw	= (CEdit*)GetDlgItem( IDC_edtChkPW );

	pEditPw->GetWindowText(strPw);
	pEditChkPw->SetWindowText(strPw);
}

VOID CDCM_INFO::InitFirstLastIndex()
{
	INT ids = m_pParentGrid->SelectMakeList();
	ids = m_pParentGrid->SelectGetHeadPosition();
	if( ids < 0 )
	{
		m_bGridSelectYN = FALSE;
		INT cnt = m_pParentDM->GetRowCount();
		m_nFirstIdx = 0;
		m_nLastIdx = cnt-1;
	}
	else
	{
		m_bGridSelectYN = TRUE;
		m_nLastIdx = m_pParentGrid->SelectGetTailPosition();
		m_nFirstIdx = m_pParentGrid->SelectGetHeadPosition();
	}
}

VOID CDCM_INFO::ShowNextPrevButton(INT nIdx)
{
	GetDlgItem(IDC_btnRre)->EnableWindow(TRUE);
	GetDlgItem(IDC_btnNext)->EnableWindow(TRUE);

	if( m_nFirstIdx >= nIdx || nIdx < 0 )		
	{
		GetDlgItem(IDC_btnRre)->EnableWindow(FALSE);
	}
	
	if( m_nLastIdx <= nIdx  || nIdx < 0 )		
	{
		GetDlgItem(IDC_btnNext)->EnableWindow(FALSE);
	}
}

INT CDCM_INFO::GetControlToIp( CIPAddressCtrl* pIpAdd, CString& strIp )
{
	BYTE nField0;
	BYTE nField1;
	BYTE nField2;
	BYTE nField3;

	pIpAdd->GetAddress( nField0, nField1, nField2, nField3 );
	strIp.Format(_T("%d.%d.%d.%d"), nField0, nField1, nField2, nField3);

	return 0;
}

INT CDCM_INFO::SetControlToIp( CIPAddressCtrl* pIpAdd, CString strIp )
{
	BYTE nField0;
	BYTE nField1;
	BYTE nField2;
	BYTE nField3;

	_stscanf(strIp.GetBuffer(0), _T("%d.%d.%d.%d"), &nField0, &nField1, &nField2, &nField3);

	pIpAdd->SetAddress(nField0, nField1, nField2, nField3);

	return 0;
}

INT CDCM_INFO::OnSave( INT nQueryMode )
{
	INT nIdx;
	INT nIds;
	
	nIds = m_pParentDM->StartFrame();
	if ( nIds < 0 )
  	{
  		AfxMessageBox(_T("StartFrame 작업이 실패했습니다."));
  		return -1;
  	}

	if(0 == nQueryMode )
	{
		nIdx = m_pParentDM->GetRowCount();
		m_pParentDM->InsertRow(-1);
		SetCmToDm( nIdx );

		nIds = MakeInsertQueryByDm( nIdx );
		if ( nIds < 0 )
  		{
  			AfxMessageBox(_T("Insert Query 생성에 실패했습니다."));
  			return -1;
  		}
	}
	else
	{
		nIdx = m_pParentGrid->GetIdx();
		nIds = SetCmToDm( nIdx );		
		nIds = MakeUpdateQueryByDm( nIdx );
		if ( nIds < 0 )
  		{
  			AfxMessageBox(_T("Update Query 생성에 실패했습니다."));
  			return -1;
  		}
	}
	
	nIds = m_pParentDM->SendFrame();
	if ( nIds < 0 )
  	{
  		AfxMessageBox(_T("SendFrame 작업이 실패했습니다."));
  		return -1;
  	}

	nIds = m_pParentDM->EndFrame();
	if ( nIds < 0 )
  	{
  		AfxMessageBox(_T("EndFrame 작업이 실패했습니다."));
  		return -1;
  	}

	return 0 ;
}

INT CDCM_INFO::SetCmToDm( INT nDmIdx )
{
	CString strData;

	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_cmbFileType);
	pCmb->GetLBText( pCmb->GetCurSel() , strData );
	m_pParentDM->SetCellData(_T("CONTENTS_TYPE"), strData, nDmIdx);

	GetControlToIp( &m_ipaddInIP, strData );
	m_pParentDM->SetCellData(_T("내부서버IP"), strData, nDmIdx);

	GetControlToIp( &m_ipaddOutIP, strData );
	m_pParentDM->SetCellData(_T("외부서버IP"), strData, nDmIdx);

	m_pCM->GetControlMgrData( _T("PORT"), strData );
	m_pParentDM->SetCellData(_T("서버PORT"), strData, nDmIdx);

	m_pCM->GetControlMgrData( _T("이미지PORT"), strData );
	m_pParentDM->SetCellData(_T("이미지PORT"), strData, nDmIdx);

	m_pCM->GetControlMgrData( _T("ROOT_DIR"), strData );
	m_pParentDM->SetCellData(_T("ROOT_DIR"), strData, nDmIdx);

	m_pCM->GetControlMgrData( _T("ID"), strData );
	m_pParentDM->SetCellData(_T("USERID"), strData, nDmIdx);

	m_pCM->GetControlMgrData( _T("PW"), strData );
	m_pParentDM->SetCellData(_T("PASSWD"), strData, nDmIdx);

	CButton* pBtnPassiveY = (CButton*)GetDlgItem(IDC_rPassive);

	if( 1 == pBtnPassiveY->GetCheck() )
	{
		strData = _T("Y");
		m_nPassive = 0;
	}
	else
	{
		strData = _T("N");
		m_nPassive = 1;
	}
	m_pParentDM->SetCellData(_T("PASSIVE모드"), strData, nDmIdx);

	CButton* pBtnActiveY = (CButton*)GetDlgItem(IDC_rActiveY);

	if( 1 == pBtnActiveY->GetCheck() )
	{
		strData = _T("Y");
		m_nActive = 0;
	}
	else
	{
		strData = _T("N");
		m_nActive = 1;
	}
	m_pParentDM->SetCellData(_T("활성화여부"), strData, nDmIdx);

	return 0 ;
}

INT CDCM_INFO::SetDmToCm( INT nDmIdx )
{
	CString strData;

	strData = m_pParentDM->GetCellData(_T("CONTENTS_TYPE"), nDmIdx);
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_cmbFileType);
	pCmb->SelectString(0,strData);
	pCmb->EnableWindow(FALSE);

	m_pCM->SetControlMgrData( _T("파일타입"), strData );

	strData = m_pParentDM->GetCellData(_T("내부서버IP"), m_nIdx);
	SetControlToIp( &m_ipaddInIP, strData );

	strData = m_pParentDM->GetCellData(_T("외부서버IP"), m_nIdx);
	SetControlToIp( &m_ipaddOutIP, strData );

	strData = m_pParentDM->GetCellData(_T("서버PORT"), m_nIdx);
	m_pCM->SetControlMgrData( _T("PORT"), strData );

	strData = m_pParentDM->GetCellData(_T("이미지PORT"), m_nIdx);
	m_pCM->SetControlMgrData( _T("이미지PORT"), strData );

	strData = m_pParentDM->GetCellData(_T("ROOT_DIR"), m_nIdx);
	m_pCM->SetControlMgrData( _T("ROOT_DIR"), strData );

	strData = m_pParentDM->GetCellData(_T("USERID"), m_nIdx);
	m_pCM->SetControlMgrData( _T("ID"), strData );

	strData = m_pParentDM->GetCellData(_T("PASSWD"), m_nIdx);
	m_pCM->SetControlMgrData( _T("PW"), strData );

	strData = m_pParentDM->GetCellData(_T("PASSIVE모드"), m_nIdx);

	CButton* pBtnPassiveY = (CButton*)GetDlgItem(IDC_rPassive);
	CButton* pBtnPassiveN = (CButton*)GetDlgItem(IDC_rActive);

	if( strData == _T("Y") )
	{
		pBtnPassiveY->SetCheck(1);
 		pBtnPassiveN->SetCheck(0);
		
		m_nPassive = 0;
	}
	else
	{
		pBtnPassiveY->SetCheck(0);
 		pBtnPassiveN->SetCheck(1);
		
		m_nPassive = 1;
	}

	CButton* pBtnActiveY = (CButton*)GetDlgItem(IDC_rActiveY);
	CButton* pBtnActiveN = (CButton*)GetDlgItem(IDC_rActiveN);
	strData = m_pParentDM->GetCellData(_T("활성화여부"), m_nIdx);
	if( strData == _T("Y") )
	{
		pBtnActiveY->SetCheck(1);
 		pBtnActiveN->SetCheck(0);

		m_nActive = 0;		
	}
	else
	{
		pBtnActiveY->SetCheck(0);
 		pBtnActiveN->SetCheck(1);
		
		m_nActive = 1;
	}

	return 0 ;
}

INT CDCM_INFO::MakeInsertQueryByDm( INT nDmIdx )
{
	CString strData;
	CString strServerId;
	CString strActiveFlag;
	CString strContentsType;
	CString strQuery;
	m_pParentDM->InitDBFieldData() ;

	strQuery.Format( _T("SELECT MAX(SERVER_ID)+1 FROM DL_CONTENTS_SERVER_TBL") );
	m_pParentDM->GetOneValueQuery(strQuery, strServerId);
	m_pParentDM->SetCellData(_T("서버ID"), strServerId, nDmIdx);
	m_pParentDM->AddDBFieldData( _T("SERVER_ID")   ,_T("NUMERIC"), strServerId);

	strData = m_pParentDM->GetCellData(_T("내부서버IP"), nDmIdx);
	m_pParentDM->AddDBFieldData( _T("IP_ADDR")     ,_T("STRING") , strData );

	strData = m_pParentDM->GetCellData(_T("서버PORT"), nDmIdx);
	m_pParentDM->AddDBFieldData( _T("IN_PORT")     ,_T("NUMERIC"), strData );

	strData = m_pParentDM->GetCellData(_T("이미지PORT"), nDmIdx);
	m_pParentDM->AddDBFieldData( _T("OUT_PORT")    ,_T("NUMERIC"), strData );
	
// 	m_pParentDM->AddDBFieldData( _T("OUT_PORT_UDP"),_T("NUMERIC"), m_pParentDM->GetCellData(_T("")   , nDmIdx) );
	strData = m_pParentDM->GetCellData(_T("ROOT_DIR"), nDmIdx);
	m_pParentDM->AddDBFieldData( _T("ROOT_DIR")	   ,_T("STRING"), strData );

	strData = m_pParentDM->GetCellData(_T("USERID"), nDmIdx);
	m_pParentDM->AddDBFieldData( _T("USERID")	   ,_T("STRING"), strData );

	strData = m_pParentDM->GetCellData(_T("PASSWD"), nDmIdx);
	m_pParentDM->AddDBFieldData( _T("PASSWD")	   ,_T("STRING"), strData );

	strActiveFlag = m_pParentDM->GetCellData(_T("활성화여부"), nDmIdx);
	m_pParentDM->AddDBFieldData( _T("ACTIVE_FLAG") ,_T("STRING"), strActiveFlag );

	strData = m_pParentDM->GetCellData(_T("외부서버IP"), nDmIdx);
	m_pParentDM->AddDBFieldData( _T("OUT_IP")      ,_T("STRING"), strData );

	strContentsType = m_pParentDM->GetCellData(_T("CONTENTS_TYPE"), nDmIdx);
	m_pParentDM->AddDBFieldData( _T("TYPE")        ,_T("STRING"), strContentsType );

	strData = m_pParentDM->GetCellData(_T("PASSIVE모드"), nDmIdx);
	m_pParentDM->AddDBFieldData( _T("PASSIVEY")    ,_T("STRING"), strData );

	m_pParentDM->MakeInsertFrame( _T("DL_CONTENTS_SERVER_TBL") );

	if( _T("Y") == strActiveFlag )
	{
		strQuery.Format(_T("UPDATE DL_CONTENTS_SERVER_TBL SET ACTIVE_FLAG = 'N' WHERE TYPE = '%s' AND ACTIVE_FLAG = 'Y' AND SERVER_ID <> %s;"), strContentsType, strServerId );
		m_pParentDM->AddFrame(strQuery);

		INT nDmRowCnt;
		nDmRowCnt = m_pParentDM->GetRowCount();

		for( INT i = 0 ; i < nDmRowCnt ; i++ )
		{
			strData = m_pParentDM->GetCellData(_T("CONTENTS_TYPE"),i);			
			if( strContentsType == strData )
			{
				m_pParentDM->SetCellData(_T("활성화여부"),_T("N"),i);
			}
		}

		m_pParentDM->SetCellData(_T("활성화여부"),_T("Y"), nDmIdx);
		m_bActiveChange = TRUE;
	}

	return 0;
}

INT CDCM_INFO::MakeUpdateQueryByDm( INT nDmIdx )
{
	CString strData;
	CString strServerId;
	CString strActiveFlag;
	CString strContentsType;
	CString strQuery;

	m_pParentDM->InitDBFieldData () ;
// 	m_pParentDM->AddDBFieldData( _T("SERVER_ID")   ,_T("NUMERIC"), m_pParentDM->GetCellData(_T("서버ID")       , nDmIdx) );
	strData = m_pParentDM->GetCellData(_T("내부서버IP"), nDmIdx);
	m_pParentDM->AddDBFieldData( _T("IP_ADDR")     ,_T("STRING") , strData );

	strData = m_pParentDM->GetCellData(_T("서버PORT"), nDmIdx);
	m_pParentDM->AddDBFieldData( _T("IN_PORT")     ,_T("NUMERIC"), strData );

	strData = m_pParentDM->GetCellData(_T("이미지PORT"), nDmIdx);
	m_pParentDM->AddDBFieldData( _T("OUT_PORT")    ,_T("NUMERIC"), strData );
	
// 	m_pParentDM->AddDBFieldData( _T("OUT_PORT_UDP"),_T("NUMERIC"), m_pParentDM->GetCellData(_T("")   , nDmIdx) );

	strData = m_pParentDM->GetCellData(_T("ROOT_DIR"), nDmIdx);
	m_pParentDM->AddDBFieldData( _T("ROOT_DIR")	   ,_T("STRING"), strData );

	strData = m_pParentDM->GetCellData(_T("USERID"), nDmIdx);
	m_pParentDM->AddDBFieldData( _T("USERID")	   ,_T("STRING"), strData );

	strData = m_pParentDM->GetCellData(_T("PASSWD"), nDmIdx);
	m_pParentDM->AddDBFieldData( _T("PASSWD")	   ,_T("STRING"), strData );

	strActiveFlag = m_pParentDM->GetCellData(_T("활성화여부"), nDmIdx);
	m_pParentDM->AddDBFieldData( _T("ACTIVE_FLAG") ,_T("STRING"), strActiveFlag );

	strData = m_pParentDM->GetCellData(_T("외부서버IP"), nDmIdx);
	m_pParentDM->AddDBFieldData( _T("OUT_IP")      ,_T("STRING"), strData );

	strContentsType = m_pParentDM->GetCellData(_T("CONTENTS_TYPE"), nDmIdx);
	m_pParentDM->AddDBFieldData( _T("TYPE")        ,_T("STRING"), strContentsType );

	strData = m_pParentDM->GetCellData(_T("PASSIVE모드"), nDmIdx);
	m_pParentDM->AddDBFieldData( _T("PASSIVEY")    ,_T("STRING"), strData );
	
	strServerId = m_pParentDM->GetCellData(_T("서버ID"), nDmIdx);
	m_pParentDM->MakeUpdateFrame( _T("DL_CONTENTS_SERVER_TBL"), _T("SERVER_ID"), _T("NUMERIC"), strServerId );

	if( _T("Y") == strActiveFlag )
	{
		strQuery.Format(_T("UPDATE DL_CONTENTS_SERVER_TBL SET ACTIVE_FLAG = 'N' WHERE TYPE = '%s' AND ACTIVE_FLAG = 'Y' AND SERVER_ID <> %s;"), strContentsType, strServerId );
		m_pParentDM->AddFrame(strQuery);

		INT nDmRowCnt;
		nDmRowCnt = m_pParentDM->GetRowCount();
		for( INT i = 0 ; i < nDmRowCnt ; i++ )
		{
			strData = m_pParentDM->GetCellData(_T("CONTENTS_TYPE"), i);			
			if( strContentsType == strData )
			{
				m_pParentDM->SetCellData(_T("활성화여부"),_T("N"), i);
			}
		}

		m_pParentDM->SetCellData(_T("활성화여부"),_T("Y"), nDmIdx);
		m_bActiveChange = TRUE;
	}

	return 0;
}