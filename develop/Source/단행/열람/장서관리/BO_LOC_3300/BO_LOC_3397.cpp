// BO_LOC_3397.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_3397.h"
#include "../CO_USER_CARD/PreViewCardPrint.h"
#include "direct.h"
#include "FrameManager.h"
#include "..\\BL_LOC_USER\\UserCardHistoryDlg.h"
#include "..\\BL_LOC_USER\\KL_UserCardHistoryDlg.h"
#include "..\\BL_LOC_USER\\KL_UserManagerDlg.h"
#include "StdioFileEx.h"
#include "BO_LOC_3000Api.h"
#include "personInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3397 dialog

CBO_LOC_3397::CBO_LOC_3397(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_3397)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_SPECIAL_MODE = 0;
	m_bIsKLReady = FALSE;
	m_nKLRFIDIndex = 0;

	GetManageValue(_T("기타"), _T("공통"), _T("통합도서서비스"), _T("통합도서서비스"), m_sIsUnityLoanService);
	if(_T("Y")!=m_sIsUnityLoanService) m_sIsUnityLoanService = _T("N");
}

CBO_LOC_3397::~CBO_LOC_3397()
{

}

void CBO_LOC_3397::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3397)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_LOC_3397, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3397)
	ON_BN_CLICKED(IDC_btnADDUSER, OnbtnADDUSER)
	ON_BN_CLICKED(IDC_btnDELETE_USER, OnbtnDELETEUSER)
	ON_BN_CLICKED(IDC_btnCLOSE, OnbtnCLOSE)
	ON_BN_CLICKED(IDC_btnUserCardPrint, OnbtnUserCardPrint)
	ON_WM_SHOWWINDOW()
	ON_CBN_SELCHANGE(IDC_cmbFORMAT, OnSelchangecmbFORMAT)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_btnBarcode_Print, OnbtnBarcodePrint)
	ON_BN_CLICKED(IDC_btn1, Onbtn1)
	ON_WM_CTLCOLOR()
	ON_WM_SYSCOMMAND()
	ON_BN_CLICKED(IDC_btnIssuehistoryManager, OnbtnIssuehistoryManager)
	ON_BN_CLICKED(IDC_btnUserTagging, OnbtnUserTagging)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER+1030, OnRFIDServiceMessage)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3397 message handlers

BOOL CBO_LOC_3397::OnInitDialog() 
{
	CDialog::OnInitDialog();	
	EnableThemeDialogTexture(GetSafeHwnd()); 	

	if(InitESL_Mgr(_T("BO_LOC_3397")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	INT ids = GetManageValue( _T("열람"), _T("공통"), _T("이용자관리"), _T("대출화면주민번호표시유무"), m_strIsChangeSecuriy);
	if(0>ids) return FALSE;
	
	if( 0 > SetSpecialMode() )
	{
		return FALSE;
	}

	if ( m_SPECIAL_MODE == SPECIAL_MODE )
	{
		
		( (CButton*) GetDlgItem (IDC_btnADDUSER) )->ShowWindow(SW_HIDE);

		CRect rect1;
		CRect rect2;

		( (CButton*) GetDlgItem (IDC_btnADDUSER) )->GetWindowRect(&rect1);
		( (CButton*) GetDlgItem (IDC_btnDELETE_USER) )->GetWindowRect(&rect2);		

		ScreenToClient( &rect1 );	
		ScreenToClient( &rect2 );

		( (CButton*) GetDlgItem (IDC_btnDELETE_USER) )->MoveWindow(rect1);
		( (CButton*) GetDlgItem (IDC_btnBarcode_Print) )->MoveWindow(rect2);		
		
	}	

	InitFormatList();

	
	GetManageValue( _T("기타"), _T("공통"), _T("통합도서서비스"), _T("통합도서서비스"), m_sUseKL);
	if(m_sUseKL.IsEmpty() || _T("Y") != m_sUseKL)
	{
		m_sUseKL = _T("N");
	}

	if(_T("Y") == m_sUseKL)
	{
		GetDlgItem(IDC_btn1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_chkKLICON)->ShowWindow(SW_SHOW);
	}

	CESL_ControlMgr* pCM = FindCM(_T("CM_BO_LOC_3397"));
	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( _T("이용자그리드") ));
	if(!m_pInfo->m_bUserInfoExportYN)
	{	
		pGrid->m_uUseKeyFlag = ESL_GRID_NOT_USE_F12;
	}
	else
	{
		pGrid->m_bPrivacyPrintLogRecord = TRUE;
		pGrid->m_strPrintInfo = _T("회원증 카드 출력");
	}

	m_bRFIDYN = RecordRFIDUSEYN();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



INT CBO_LOC_3397::ShowUserList()
{
	EFS_BEGIN

	AllControlDisplay( _T("CM_BO_LOC_3397") );
	
	return 0;

	EFS_END

	return -1;

}

void CBO_LOC_3397::OnbtnADDUSER() 
{
	AddGridUser();	
}

INT CBO_LOC_3397::AddGridUser()
{
EFS_BEGIN

	INT ids;

	CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_BO_LOC_3397"),_T("이용자그리드"));
	
	CESL_DataMgr* pParentDM = FindDM( _T("DM_BO_LOC_3300_LIST") );
	if( pParentDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("AddGridUser") );

	CESL_DataMgr* pSaveUserDM = FindDM( _T("DM_BO_LOC_3300_SAVE_USER") );
	if( pSaveUserDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("AddGridUser") );

	if ( m_SPECIAL_MODE == SPECIAL_MODE )
	{
	  pSaveUserDM->FreeData();
	}

	CESL_Grid* pParentGrid = (CESL_Grid*)FindControl(_T("CM_BO_LOC_3300"),_T("MainGrid"));
	pParentGrid->SelectMakeList();
	
	INT nSelectedCount = pParentGrid->SelectGetCount();
	pParentGrid->SelectGetHeadPosition();

	const INT nCopyCnt = 5;
	CString sSrcAlias[nCopyCnt] =
	{
		_T("REC_KEY") ,   _T("대출자번호") , _T("이름") ,     _T("주민등록번호"),// , _T("대출자번호")
		_T("통합회원전환여부")
	};
	CString sDstAlias[nCopyCnt] =
	{
		_T("이용자KEY") , _T("이용자번호") , _T("이용자명") , _T("주민등록번호"),// , _T("등록번호")
		_T("통합회원전환여부")
	};

	INT nIndex;
	INT nInsertIndex;
	CString sCopyData[nCopyCnt];
	CString sUserKey;
	BOOL bInsert;
	for( INT i = 0 ; i < nSelectedCount ; i++ )
	{
		nIndex = pParentGrid->SelectGetIdx();

		pParentDM->GetCellData( sSrcAlias , nCopyCnt , sCopyData , nIndex );

		bInsert = TRUE;
		for( INT j = 0 ; j < pSaveUserDM->GetRowCount() ; j++ )
		{
			ids = pSaveUserDM->GetCellData( _T("이용자KEY") , j , sUserKey );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("AddGridUser") );

			if( sCopyData[0].Compare(sUserKey) == 0 )
				bInsert = FALSE;			
		}

		if( !bInsert )
		{
			pParentGrid->SelectGetNext();
			continue;
		}

		nInsertIndex = pSaveUserDM->GetRowCount();
		pSaveUserDM->InsertRow(-1);

		pSaveUserDM->SetCellData( sDstAlias , nCopyCnt , sCopyData , nInsertIndex );

		if(_T("Y")!=m_strIsChangeSecuriy)
		{
			CString strUserNo;
			ids = pSaveUserDM->GetCellData(_T("이용자번호"), nInsertIndex, strUserNo);
			if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2(-1040, _T("SecederSearch"));

			CLocCommonAPI::ChangeSecuriyUserNo( this , strUserNo , m_sIsUnityLoanService );
			ids = pSaveUserDM->SetCellData(_T("이용자번호"), strUserNo, nInsertIndex);
			if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -1070 , _T("SecederSearch") );
		}
	
		pParentGrid->SelectGetNext();
	}

	pGrid->Display();
	if(_T("Y") == m_sUseKL)
	{
		pGrid->SetColWidth(5, 0, 1000);
	}

	if ( m_SPECIAL_MODE == SPECIAL_MODE)
	{
		for ( i = 0; i < nSelectedCount; i++ )
			pGrid->SetAt(i, pGrid->m_nSelectIdx, pGrid->m_sSelectString);
	}

	return 0;

EFS_END
	return -1;
}

void CBO_LOC_3397::OnbtnDELETEUSER() 
{
	DeleteUser();
}

void CBO_LOC_3397::OnbtnCLOSE() 
{
	OnCancel();		
}


INT CBO_LOC_3397::DeleteUser()
{
	EFS_BEGIN

	CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_BO_LOC_3397"),_T("이용자그리드"));
	CESL_DataMgr* pSaveUserDM = FindDM( _T("DM_BO_LOC_3300_SAVE_USER") );
	if( pSaveUserDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("DeleteUser") );

	pGrid->SelectMakeList();

	INT nSelectedCount = pGrid->SelectGetCount();

	pGrid->SelectGetTailPosition();

	INT nIndex;
	for( INT i = 0 ; i < nSelectedCount ; i++ )
	{
		nIndex = pGrid->SelectGetIdx();
		pSaveUserDM->DeleteRow(nIndex);
		pGrid->SelectGetPrev();
	}

	pGrid->Display();
	
	return 0;

	EFS_END

	return -1;
}

void CBO_LOC_3397::OnbtnUserCardPrint() 
{
	EFS_BEGIN

	CESL_DataMgr* pUserCardDM = FindDM(_T("DM_회원증_이용자정보"));
	CESL_DataMgr* pUserDM = FindDM(_T("DM_BO_LOC_3300_LIST"));

	CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_BO_LOC_3397"),_T("이용자그리드"));	
	if(pGrid)
	{
		pGrid->SelectMakeList();
		INT nCnt = pGrid->SelectGetCount();
		if(0 == nCnt)
		{
			AfxMessageBox(_T("이용자를 선정하여 주십시요."));
			return;
		}
	}

	PrintCard();


	EFS_END
	
}


INT CBO_LOC_3397::PrintCard()
{
	INT ids;

	bool bPrintBackPage;

	if( ((CButton*)GetDlgItem(IDC_chkTOGETHER))->GetCheck() == 1 )
		bPrintBackPage = TRUE;
	else
		bPrintBackPage = FALSE;

	CESL_ControlMultiComboBox* pCombo = (CESL_ControlMultiComboBox*)GetDlgItem(IDC_cmbFORMAT);
	INT nFormatIndex = pCombo->GetCurSel();	
	if( nFormatIndex == 0 )
	{
		AfxMessageBox(_T("양식을 선택해주십시요"));
		return 0;
	}

	CESL_DataMgr* pFormatDM = FindDM( _T("DM_회원증양식종류") );
	CString sFrontFormKey , sBackFormKey;
	CString sFormName , sFormName2;

	pFormatDM->GetCellData( _T("양식이름") , nFormatIndex-1 , sFormName );
	pFormatDM->GetCellData( _T("대출양식KEY") , nFormatIndex-1 , sFrontFormKey );

	if( bPrintBackPage )
	{
		
		pFormatDM->GetCellData( _T("양식이름") , nFormatIndex , sFormName2 );
		pFormatDM->GetCellData( _T("대출양식KEY") , nFormatIndex , sBackFormKey );
		
		if( sFormName.Compare(sFormName2) != 0 )
		{
			AfxMessageBox(_T("뒷면 양식이 존재하지 않아 뒷면을 출력할수 없습니다.") );
			return 0;
		}
	}

	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_BO_LOC_3397") , _T("이용자그리드") );

	pGrid->SelectMakeList();

	INT nSelectCnt = pGrid->SelectGetCount();

	if( nSelectCnt == 0 )
	{
		AfxMessageBox( _T("출력할 이용자를 선택해주십시요") );
		return 0;
	}

	if( nSelectCnt >= 50 )
	{
		AfxMessageBox( _T("한번에 50장 이상 출력할수 없습니다.") );
		return 0;
	}
	
	CESL_DataMgr* pUserSaveDM = FindDM(_T("DM_BO_LOC_3300_SAVE_USER"));

	pGrid->SelectGetHeadPosition();
	INT nIndex;
	CString sRecKeyList;
	CString sRecKey;
	CStringArray saRecKeyList;
	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		nIndex = pGrid->SelectGetIdx();
		
		pUserSaveDM->GetCellData( _T("이용자KEY") , nIndex , sRecKey );

		if(i != 0) sRecKeyList += _T(",");

		sRecKeyList += sRecKey;

		saRecKeyList.Add( sRecKey );

		pGrid->SelectGetNext();
	}

	CESL_DataMgr* pUserDM = FindDM(_T("DM_회원증_이용자정보"));
	pUserDM->FreeData();

	CESL_DataMgr TmpDM;
	CLocCommonAPI::MakeCopyDM( this , pUserDM , &TmpDM );

	CString sWhere;
	sWhere.Format( _T(" REC_KEY IN ( %s ) ")  , sRecKeyList );

	TmpDM.RefreshDataManager( sWhere );

	if(TRUE == m_pInfo->m_bLibSecuUse && 0 < TmpDM.GetRowCount())
	{
		CStringArray asDes1;
		CString strGetData;
		for(INT i = 0; i < TmpDM.GetRowCount(); i++)
		{
			TmpDM.GetCellData(_T("주민등록번호"), i, strGetData);
			asDes1.Add(strGetData);
		}
		CBO_LOC_3000Api *p3000Api = new CBO_LOC_3000Api(this);
		if(NULL != p3000Api)
		{
			if(TRUE == p3000Api->ConnectSocket()) 
			{
				p3000Api->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("CIVIL_NO"), asDes1);
				for(i = 0; i < TmpDM.GetRowCount(); i++)
				{
					TmpDM.SetCellData(_T("주민등록번호"), asDes1.GetAt(i), i);
				}
				p3000Api->CloseSocket();
			}
			delete p3000Api;
			p3000Api = NULL;
		}
		asDes1.RemoveAll(); asDes1.FreeExtra();
	}

	const INT nCnt = 2;

	CString sAlias[nCnt] =
	{
		_T("주민등록번호") , _T("사진")
	};

	CString strKLMemberYN;
	INT nKLMemberCnt , nMemberCnt;
	nKLMemberCnt = 0;
	nMemberCnt = 0;

	CString sResult[nCnt];
	INT nSrcIndex = 0 ;
	INT nDstIndex = 0 ;
	CString sCRecKey;
	for( i = 0 ; i < TmpDM.GetRowCount() ; i++ )
	{
		sRecKey = saRecKeyList.GetAt(0);

		for( INT j = 0 ; j < TmpDM.GetRowCount() ; j++ )
		{
			TmpDM.GetCellData( _T("이용자KEY") , j , sCRecKey );
			if( sCRecKey.Compare( sRecKey ) == 0 ) break;
		}

		nSrcIndex = j;
		saRecKeyList.RemoveAt(0);

		CLocCommonAPI::AddDM( &TmpDM , pUserDM , nSrcIndex , -1 , this );

		pUserDM->GetCellData( sAlias[1] , nDstIndex , sResult[1] );

		CString sFileName;
		if( !sResult[1].IsEmpty() )
		{
			_tmkdir(_T("../대출증"));
			_tmkdir(_T("../대출증/이용자사진"));
			
			CString sFileName;
			sFileName.Format(_T("../대출증/이용자사진/%s.%s"),sRecKey,sResult[1]);
			BYTE *pBuffer = (BYTE *)malloc(MAX_BLOB_BUFFER);
			
			INT nSize;
			ids = TmpDM.LOBGet( _T("CO_LOAN_USER_TBL") , _T("REC_KEY") , _T("NUMERIC") , sRecKey , _T("USER_PICTURE") , pBuffer , &nSize );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeRegNoFileFromDBLOBField") );
			
			CFile fd;
			if (fd.Open( sFileName , CFile::modeCreate|CFile::modeWrite | CFile::typeBinary) == NULL) {
				AfxMessageBox(_T("이용자 관리 Tmp File Open Error~"));
				free( pBuffer );
				return 1;
			}
			
			fd.WriteHuge(pBuffer, nSize);
			free( pBuffer );
			
			fd.Close();

			pUserDM->SetCellData( sAlias[1] , sFileName , nDstIndex );
		}

		pUserDM->GetCellData( sAlias[0] , nDstIndex , sResult[0] );

		if( !sResult[0].IsEmpty() )
		{
			CLocCommonAPI::ChangeSecuriyCivilNo( this , sResult[0] , m_sIsUnityLoanService );
			pUserDM->SetCellData( sAlias[0] , sResult[0], nDstIndex  );
		}
		
		pUserDM->SetCellData( _T("양식정보KEY") , sFrontFormKey , nDstIndex );

		if(_T("Y")==m_sIsUnityLoanService)
		{
			pUserDM->GetCellData( _T("통합회원전환여부") , nDstIndex , strKLMemberYN );
			strKLMemberYN.TrimLeft();	strKLMemberYN.TrimRight();
			strKLMemberYN.MakeUpper();
			if(_T("Y")==strKLMemberYN)
			{
				nKLMemberCnt++;
			}
			else
			{
				nMemberCnt++;
			}
		}

		nDstIndex++;
		
		if( bPrintBackPage )
		{
			CLocCommonAPI::AddDM( &TmpDM , pUserDM , nSrcIndex , -1 , this );

			pUserDM->GetCellData( sAlias[0] , nDstIndex , sResult[0] );
			
			if( !sResult[0].IsEmpty() )
			{
				CLocCommonAPI::ChangeSecuriyCivilNo( this , sResult[0] , m_sIsUnityLoanService );
				pUserDM->SetCellData( sAlias[0] , sResult[0], nDstIndex  );
			}
			
			pUserDM->SetCellData( _T("양식정보KEY") , sBackFormKey , nDstIndex );

			pUserDM->SetCellData( sAlias[1] , sFileName , nDstIndex );

			nDstIndex++;
		}
	}

	if(_T("Y")==m_sIsUnityLoanService)
	{
		if(0<nKLMemberCnt && 0<nMemberCnt)
		{
			AfxMessageBox(	_T("책이음회원과 일반회원이 같이 선택 되었습니다.\r\n")
							_T("책이음회원 또는 일반회원만 선택해주십시요."));
			return 1010;
		}

		if(0<nKLMemberCnt)
		{
			CString strCode;
			ids = pCombo->GetData(strCode, nFormatIndex, 0);
			if(_T("책이음회원증")!=strCode)
			{				
				if(IDYES != AfxMessageBox(_T("책이음회원입니다. 기존 양식으로 출력하시겠습니까?"),MB_YESNO))
				{
					return 1020;
				}
			}
		}
	}

	CPreViewCardPrint Dlg(this);
	Dlg.SetPrintList( pUserDM );
	if(1 == ((CButton*)GetDlgItem(IDC_chkKLICON))->GetCheck())
	{
		Dlg.m_bShowKLIconYN = TRUE;		
	}
	else
	{
		Dlg.m_bShowKLIconYN = FALSE;
	}
	Dlg.DoModal();

	if(Dlg.m_bIsPrint)
	{		
		CESL_DataMgr *pDM = FindDM( _T("DM_MNG_USER_MANAGER_PRINT_LOG") );

		std::vector<CString> list;
		CString strKey;
		for(i=0; i<TmpDM.GetRowCount(); i++)
		{
			ids = TmpDM.GetCellData(_T("이용자KEY"), i, strKey );
			if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_3397::PrintCard()") );
			list.push_back(strKey);
		}

		CPersonInfo::SetPrintLog(this, pDM, PERINFO_PRINT, m_pInfo, _T("회원증출력"), TmpDM.GetRowCount(), &list);
		list.clear();
	}

	return 0;
}

void CBO_LOC_3397::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);	

	if( bShow )
	{
		ShowUserList();

		CString strFlag;
		GetManageValue( _T("기타") , _T("공통") , _T("국방대") , _T("") , strFlag );	
		if ( strFlag != _T("Y") )
		{
			GetDlgItem(IDC_btnBarcode_Print)->ShowWindow( SW_HIDE );
		}

		if ( m_SPECIAL_MODE == SPECIAL_MODE )
		{
			AddGridUser();
		}

		CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_cmbFORMAT);	
		pCmb->SetCurSel(GetFormat());

		LoadRFIDService();
	}
	else
	{
	}
}

INT CBO_LOC_3397::InitFormatList()
{
	INT ids;
	
	CESL_ControlMgr* pCM = FindCM( _T("CM_BO_LOC_3397_ETC") );
	
	CESL_ControlMultiComboBox* pCMCB = (CESL_ControlMultiComboBox*)(pCM->FindControl(_T("양식리스트")));
	if( pCMCB == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("ChangeScreenFromRFID") );
	pCMCB->InitMultiComboBox(2);
	
	CString sInsertCode[2] = { _T("적용안함                                    "), _T("") };
	ids = pCMCB->SetData( sInsertCode , 2 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("ChangeScreenFromRFID") );
	
	CESL_DataMgr* pFormatDM = FindDM( _T("DM_회원증양식종류") );
	pFormatDM->RefreshDataManager(_T(""));

	for( INT i = 0 ; i < pFormatDM->GetRowCount() ; i++ )
	{
		pFormatDM->GetCellData( _T("양식이름") , i , sInsertCode[0] );
		pFormatDM->GetCellData( _T("앞면여부") , i , sInsertCode[1] );
		
		if( sInsertCode[1].Compare(_T("0")) == 0 )
			sInsertCode[1] = _T("앞면");
		else
			sInsertCode[1] = _T("뒷면");

		ids = pCMCB->SetData( sInsertCode , 2 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("ChangeScreenFromRFID") );
	}
	
	if(1 < pCMCB->GetRowCount())
	{
 		pCMCB->SetCurSel(GetFormat());
	}
 	else
	{
 		pCMCB->SetCurSel(0);
	}

	SetCheckBackPagePrint();

	return 0;
}

void CBO_LOC_3397::OnSelchangecmbFORMAT() 
{
	SetCheckBackPagePrint();
}


INT CBO_LOC_3397::SetCheckBackPagePrint()
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbFORMAT);
	INT nIndex = pCombo->GetCurSel();

	if( nIndex == 0 )
		return 0;
	
	CESL_DataMgr* pFormatDM = FindDM( _T("DM_회원증양식종류") );
	
	CString sData;
	pFormatDM->GetCellData( _T("앞면여부") , nIndex-1 , sData );
	
	if( sData.Compare(_T("1")) == 0 )
	{
		((CButton*)GetDlgItem(IDC_chkTOGETHER))->SetCheck(0);
		GetDlgItem(IDC_chkTOGETHER)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_chkTOGETHER)->EnableWindow(TRUE);
	}

	SetFormat(nIndex);

	return 0;
}

BOOL CBO_LOC_3397::Create(CWnd* pParentWnd) 
{
	return CDialog::Create(IDD, pParentWnd);
}

void CBO_LOC_3397::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnClose();
}

void CBO_LOC_3397::OnbtnBarcodePrint() 
{
	CEquipPrintPropertyDlg dlg(this , 1);
		
	CESL_DataMgr* pUserSaveDM = FindDM(_T("DM_BO_LOC_3300_SAVE_USER"));
	CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_BO_LOC_3397"),_T("이용자그리드"));

	INT n = pGrid->SelectGetCount();

	dlg.SetParentInfo(pUserSaveDM, pGrid);
	dlg.m_bBarcodeUserno = TRUE;
	dlg.DoModal();	
}

INT CBO_LOC_3397::SetSpecialMode()
{
	INT			 ids;
	CESL_DataMgr DM;
	CString strQuery, strResult;

	DM.SetCONNECTION_INFO( m_pInfo->CONNECTION_INFO );
	
	DM.MANAGE_CODE = m_pInfo->MANAGE_CODE;
	
	strQuery.Format( _T("SELECT VALUE_NAME FROM ESL_MANAGE_TBL ")
		             _T(" WHERE GROUP_1='L' ")
					   _T(" AND GROUP_2='열람' ")
					   _T(" AND GROUP_3='공통' ")
					   _T(" AND CLASS_ALIAS='회원증그리드자동선정여부' AND MANAGE_CODE = UDF_MANAGE_CODE ") );
	ids = DM.GetOneValueQuery( strQuery, strResult );
	if( 0 > ids && -4007 != ids )	ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SetSpecialMode") );
	if( -4007 == ids )	strResult = _T("N");
	if( 0 == strResult.GetLength() ) strResult = _T("N");

	strResult.MakeUpper();
	if( _T("N") == strResult )	m_SPECIAL_MODE = 0;
	else if( _T("Y") == strResult )	m_SPECIAL_MODE = 2;
	else	ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SetSpecialMode") );

	return 0;
}

HBRUSH CBO_LOC_3397::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}

INT CBO_LOC_3397::SetFormat(INT nIdx)
{
	CString sFileName = _T("..\\cfg\\UserCardFormat.cfg");
	FILE *fp = _tfopen(sFileName, _T("w+b"));
	fputc(0xFF, fp);
	fputc(0xFE, fp);
	_ftprintf( fp, _T("%d"), nIdx);
	fclose( fp );

	return 0;
}

INT CBO_LOC_3397::GetFormat()
{
	CString sFileName = _T("..\\cfg\\UserCardFormat.cfg");
	CFileFind ff;
	if(!ff.FindFile(sFileName))
	{
		return 0;
	}
	else
	{
		CString sValue;
		CStdioFile fp;
		if(!fp.Open(sFileName, CFile::modeRead | CFile::typeBinary))
		{
			return 0;
		}
		TCHAR cUni;
		fp.Read(&cUni, sizeof(TCHAR));
		if(0xFEFF != cUni)
		{
			fp.SeekToBegin();
		}
		
		fp.ReadString(sValue);
		fp.Close();
		sValue.TrimLeft();	sValue.TrimRight();
		
		return _ttoi(sValue);
	}

	return 0;
}

void CBO_LOC_3397::Onbtn1() 
{
	// TODO: Add your control notification handler code here
	if(_T("Y") != m_sUseKL)
	{
		m_bIsKLReady = FALSE;
		return;
	}

	CString strData;
	CESL_DataMgr* pUserCardDM = FindDM(_T("DM_회원증_이용자정보"));
	CESL_DataMgr* pUserDM = FindDM(_T("DM_BO_LOC_3300_SAVE_USER"));

	CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_BO_LOC_3397"),_T("이용자그리드"));	
	if(pGrid)
	{
		pGrid->SelectMakeList();
		INT nCnt = pGrid->SelectGetCount();
		if(0 == nCnt)
		{
			AfxMessageBox(_T("이용자를 선정하여 주십시요."));
			m_bIsKLReady = FALSE;
			return;
		}
		else if(nCnt > 1)
		{
			AfxMessageBox(_T("한 명의 이용자만 선정하여 주십시요."));
			m_bIsKLReady = FALSE;
			return;
		}

 		INT nIdx = pGrid->SelectGetTailPosition();

		strData = pUserDM->GetCellData(_T("통합회원전환여부"), nIdx);
		if(_T("Y") != strData)
		{
			AfxMessageBox(_T("선택된 이용자는 책이음회원이 아닙니다."));
			m_bIsKLReady = FALSE;
			return;
		}
	}

	m_bIsKLReady = TRUE;
}


INT CBO_LOC_3397::HandphoneRFIDInput(CString& strUserKey, CString& strUID)
{
	CString strQuery;

	CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_BO_LOC_3397"),_T("이용자그리드"));	
	
	pGrid->SelectMakeList();
	INT nSIndex = pGrid->SelectGetIdx();

	CESL_DataMgr* pDM = FindDM(_T("DM_BO_LOC_3300_SAVE_USER"));
	
	if(strUserKey.IsEmpty())
	{
		strUserKey = pDM->GetCellData(_T("이용자KEY") , nSIndex);		
	}

	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(_T(""));
	TmpDM.StartFrame();
	strQuery.Format(_T("UPDATE BO_BOOK_TBL SET WORKNO = NULL WHERE WORKNO = '%s';"), strUID);
	TmpDM.AddFrame(strQuery);
	strQuery.Format(_T("UPDATE SE_BOOK_TBL SET WORKNO = NULL WHERE WORKNO = '%s';"), strUID);
	TmpDM.AddFrame(strQuery);
	strQuery.Format(_T("UPDATE CO_LOAN_USER_TBL SET WORKNO = NULL WHERE WORKNO = '%s';"), strUID);
	TmpDM.AddFrame(strQuery);
	strQuery.Format(_T("UPDATE CO_LOAN_USER_TBL SET PB_WORKNO = NULL WHERE PB_WORKNO = '%s';"), strUID);
	TmpDM.AddFrame(strQuery);
	strQuery.Format(_T("UPDATE CO_LOAN_USER_TBL SET WORKNO = '%s' WHERE REC_KEY = %s;"), strUID, strUserKey);
	TmpDM.AddFrame(strQuery);
	INT ids = TmpDM.SendFrame();
	TmpDM.FreeFrame();
	TmpDM.EndFrame();

	if(0 == ids)
	{
		CString strUserNo = pDM->GetCellData(_T("이용자번호") , nSIndex);

		CFrameWriter Writer(m_pInfo);
		Writer.setCommand(_T("IL_K24_LOC_B01_SERVICE"));
		Writer.addRecord();
		Writer.addElement(_T("통합대출자번호"), strUserNo);
		Writer.addElement(_T("회원증RFID"), strUID);
		
		CString strModifyDate;
		CTime t = CLocCommonAPI::GetCurrentDateTime(this, strModifyDate);
		Writer.addElement(_T("수정일자"), strModifyDate);
		Writer.addElement(_T("수정도서관부호"), m_pInfo->LIB_CODE);

		CFrameSender Sender(m_pInfo);
		CFrameReader Reader;
		Sender.SendTempTbl(&Writer, GetWorkLogMsg( _T("통합RFID등록"), __WFILE__, __LINE__ ));
	}

	return ids;

}

LRESULT CBO_LOC_3397::OnRFIDServiceMessage(WPARAM wParam, LPARAM lParam)
{
	INT ids = 0;
	CStdioFileEx File;
	CString strLine, strCommand, strData1, strData2, strUserNo, strUserKey;

	if(TRUE == wParam)
	{
		strLine = *(CString*)lParam;
	}
	else
	{
		if(!File.Open(_T("..\\cfg\\KL\\KLRFIDS"), CFile::modeRead|CFile::typeText))
		{
			return -10000;
		}		
		File.ReadString(strLine);
		File.Close();
	}
	AfxExtractSubString(strCommand, strLine, 0, '|');
	AfxExtractSubString(strData1, strLine, 1, '|');
	AfxExtractSubString(strData2, strLine, 2, '|');

	if(!m_bIsKLReady)
	{		
		ids = -10005;
	}

	CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_BO_LOC_3397"),_T("이용자그리드"));	
	if(NULL == pGrid)
	{
		ids = -10001;
	}
	pGrid->SelectMakeList();
	if(0 >= pGrid->SelectGetCount())
	{
		ids = -10002;
	}
	INT nSIndex = pGrid->SelectGetIdx();

	CESL_DataMgr* pDM = FindDM(_T("DM_BO_LOC_3300_SAVE_USER"));
	if(NULL == pDM)
	{
		ids = -10003;
	}
	pDM->GetCellData( _T("이용자KEY") , nSIndex , strUserKey);
	pDM->GetCellData( _T("이용자번호") , nSIndex , strUserNo);

	if(!File.Open(_T("..\\cfg\\KL\\KLRFIDR"), CFile::modeWrite|CFile::modeCreate|CStdioFileEx::modeWriteUnicode))
	{
		ids = -10004;
	}

	if(_T("KL_MEMBER_CARD_OPEN") == strCommand)
	{
		if(m_bIsKLReady && ids >= 0 && !strUserKey.IsEmpty() && !strUserNo.IsEmpty())
		{
			File.WriteString(_T("KL_MEMBER_CARD_OPEN|성공"));
		}
		else
		{
			if(0 >= ids)
			{
				ids = -10100;
			}
			File.WriteString(_T("KL_MEMBER_CARD_OPEN|실패"));
		}
		File.Close();
		return ids;
	}
	else if(_T("KL_MEMBER_STATUS") == strCommand)
	{
		BOOL bIsRegister = FALSE;
		ids = IsRegsisterUID(strData1, strData2, bIsRegister);
		if(bIsRegister)
		{
			File.WriteString(_T("KL_MEMBER_STATUS|TRUE"));
		}
		else
		{
			File.WriteString(_T("KL_MEMBER_STATUS|FALSE"));
		}
		File.Close();
		return ids;
	}
	else if(_T("KL_LIBRARY") == strCommand)
	{
		File.WriteString(_T("KL_LIBRARY|") + m_pInfo->LIB_CODE);
		File.Close();
		return ids;
	}
	else if(_T("KL_MEMBER_NO") == strCommand)
	{
		File.WriteString(_T("KL_MEMBER_NO|") + strUserNo);
		File.Close();
		return ids;
	}
	else if(_T("KL_MEMBER_WRTIE_RFID_UID") == strCommand)
	{
		if(!strData1.IsEmpty())
		{
			if(strData1 != strUserNo)
			{
				File.WriteString(_T("KL_MEMBER_WRTIE_RFID_UID|실패"));
				ids = -10500;
			}
		}

		ids = HandphoneRFIDInput(strUserKey, strData2);
		if(0 == ids)
		{
			File.WriteString(_T("KL_MEMBER_WRTIE_RFID_UID|성공"));
		}
		else
		{
			File.WriteString(_T("KL_MEMBER_WRTIE_RFID_UID|실패"));
		}
		File.Close();
		return ids;
	}
	else if(_T("KL_MEMBER_CARD_CLOSE") == strCommand)
	{
		INT nIds = pGrid->SetAt(nSIndex, 0, _T(""));
		if(0 == nIds)
		{
			File.WriteString(_T("KL_MEMBER_CARD_CLOSE|성공"));
		}
		else
		{
			File.WriteString(_T("KL_MEMBER_CARD_CLOSE|실패"));
		}

		m_bIsKLReady = FALSE;
		File.Close();
		return ids;
	}		
	File.Close();

	return ids;
}

INT CBO_LOC_3397::IsRegsisterUID(CString& strUserNo, CString& strUID, BOOL& bRegister)
{
	bRegister = FALSE;
	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(_T(""));

	CString strQuery, strResult;
	if(strUserNo.IsEmpty())
	{
		strQuery.Format(
			_T("SELECT COUNT(1) FROM CO_LOAN_USER_TBL WHERE WORKNO = '%s' OR PB_WORKNO = '%s'"),
			strUID, strUID);
	}
	else
	{
		strQuery.Format(
			_T("SELECT COUNT(1) FROM CO_LOAN_USER_TBL WHERE USER_NO = '%s' OR WORKNO = '%s' OR PB_WORKNO = '%s'"),
			strUserNo, strUID, strUID);
	}
	TmpDM.GetOneValueQuery(strQuery, strResult);

	if(strResult.IsEmpty())
	{
		return -10200;
	}
	if(_ttoi(strResult) > 0)
	{
		bRegister = TRUE;
	}

	return 0;
}

INT CBO_LOC_3397::LoadRFIDService()
{
 	if(_T("Y") != m_sUseKL)
	{
	 	return 1000;
	}

	CStdioFileEx File;
	if(!File.Open(_T("..\\cfg\\KL\\KLRFID"), CFile::modeCreate|CFile::modeWrite|CStdioFileEx::modeWriteUnicode))
	{
		return -1000;
	}

	CString strData;
	strData.Format(_T("%d"), GetSafeHwnd());
	File.WriteString(strData);
	File.Close();

	CString strCmd = _T("MLoader.exe KLRFIDService.exe");
	TCHAR lpCurrentDirectory[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, lpCurrentDirectory);

	PROCESS_INFORMATION pinfo;
	STARTUPINFO Startupinfo1;
	ZeroMemory(&Startupinfo1, sizeof(Startupinfo1));
	Startupinfo1.cb = sizeof(STARTUPINFO);	
   	Startupinfo1.dwFlags = STARTF_USESHOWWINDOW;
	Startupinfo1.wShowWindow = SW_SHOW;
	INT result = ::CreateProcess( NULL, strCmd.GetBuffer(0), NULL, NULL, FALSE, 0, NULL, lpCurrentDirectory, &Startupinfo1, &pinfo);
	strCmd.ReleaseBuffer();

	CloseHandle( pinfo.hProcess );
	CloseHandle( pinfo.hThread );

	return 0;
}

INT CBO_LOC_3397::UnLoadRFIDService()
{
 	if(_T("Y") != m_sUseKL)
	{
	 	return 1000;
	}

	CStdioFileEx File;
	if(File.Open(_T("..\\cfg\\KL\\KLRFID"), CFile::modeRead|CFile::typeText))
	{
		CString strData1, strData2;	
		File.ReadString(strData1);
		File.Close();
		
		strData2.Format(_T("%d"), GetSafeHwnd());
		if(strData1 != strData2)
		{
			return 1000;
		}
	}
	
	HWND hWnd = ::FindWindow(NULL, _T("KLRFIDService"));
	if(hWnd)
	{
		::PostMessage(hWnd, WM_QUIT, NULL, NULL);
	}

	return 0;
}

void CBO_LOC_3397::OnCancel()
{
	m_bIsKLReady = FALSE;
	UnLoadRFIDService();
	CDialog::OnCancel();
}

void CBO_LOC_3397::OnSysCommand(UINT nID, LPARAM lParam) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnSysCommand(nID, lParam);
}

void CBO_LOC_3397::OnbtnIssuehistoryManager() 
{
	CESL_DataMgr* pUserDM = FindDM(_T("DM_BO_LOC_3300_SAVE_USER"));
	CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_BO_LOC_3397"),_T("이용자그리드"));	
	if(NULL==pGrid) return;

	pGrid->SelectMakeList();
	INT nCnt = pGrid->SelectGetCount();
	if(0 == nCnt)
	{
		AfxMessageBox(_T("이용자를 선정하여 주십시요."));
		return;
	}
	else if(nCnt > 1)
	{
		AfxMessageBox(_T("한 명의 이용자만 선정하여 주십시요."));
		return;
	}

	INT nIdx = pGrid->SelectGetTailPosition();
	
	CString strRecKey,strUserName,strUserNo,strKLMemberYN;
	strRecKey = pUserDM->GetCellData(_T("이용자KEY"), nIdx);
	strUserNo = pUserDM->GetCellData(_T("이용자번호"), nIdx);
	strUserName = pUserDM->GetCellData(_T("이용자명"), nIdx);
	strKLMemberYN = pUserDM->GetCellData(_T("통합회원전환여부"), nIdx);
	if(_T("Y")==strKLMemberYN)
	{
		CKL_UserCardHistoryDlg Dlg(this);
		Dlg.SetUserKey(strRecKey);
		Dlg.SetUserNo(strUserNo);
		Dlg.SetUserName(strUserName);
		Dlg.DoModal();
	}
	else
	{
		CUserCardHistoryDlg Dlg(this);
		Dlg.SetUserKey(strRecKey);
		Dlg.SetUserNo(strUserNo);
		Dlg.SetUserName(strUserName);
		Dlg.DoModal();
	}
}

VOID CBO_LOC_3397::OnbtnUserTagging()
{
EFS_BEGIN
	INT ids;

	ids = RecordRFIDUserInfo();	
EFS_END
}
BOOL CBO_LOC_3397::RecordRFIDUSEYN()
{
EFS_BEGIN
	m_bRFIDYN = FALSE;
	CString strRfidUseYN =_T("N");
	CString sQuery = _T("SELECT VALUE_NAME FROM ESL_MANAGE_TBL WHERE GROUP_1 = 'L' AND GROUP_2 = '기타' AND GROUP_3 = '공통' AND CLASS_ALIAS = 'RFID사용여부' AND MANAGE_CODE = UDF_MANAGE_CODE ");
	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	TmpDM.GetOneValueQuery( sQuery , strRfidUseYN );	
	
	if(_T("Y") == strRfidUseYN )
	{
		GetDlgItem(IDC_btnUserTagging)->ShowWindow(TRUE);	
		m_bRFIDYN = TRUE;
	}
	return m_bRFIDYN;

EFS_END
return FALSE;
}
INT CBO_LOC_3397::RecordRFIDUserInfo()
{
EFS_BEGIN
	if(!m_bRFIDYN) return -1;
	
	CString strFuncName = _T("RecordRFIDUserInfo");
	CESL_DataMgr* pDM = FindDM(_T("DM_BO_LOC_3300_SAVE_USER"));

	CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_BO_LOC_3397"),_T("이용자그리드"));	
	if(NULL==pGrid) return -2;
	INT	ids = 0;

	pGrid->SelectMakeList();
	INT nCnt = pGrid->SelectGetCount();
	if(0 == nCnt)
	{
		AfxMessageBox(_T("이용자를 선정하여 주십시요."));
		return -3;
	}
	else if(nCnt > 1)
	{
		AfxMessageBox(_T("한 명의 이용자만 선정하여 주십시요."));
		return -4;
	}
	INT nIdx = pGrid->SelectGetTailPosition();
	
	CString strLibCode = m_pInfo->LIB_CODE;
	CString strUserNo = _T("");
	CString strPassWord = _T("");
	CString strEncryptionPassWord = _T("");
	CString strRecKey;
	CString strUserName;
	CString strKlUserYN;
	ids = pDM->GetCellData(_T("이용자번호"), nIdx, strUserNo);
	ids = pDM->GetCellData(_T("이용자KEY"), nIdx, strRecKey);
	ids = pDM->GetCellData(_T("이용자명"), nIdx, strUserName);
	ids = pDM->GetCellData(_T("통합회원전환여부"), nIdx, strKlUserYN);

	CRFID_Interface RFID(this);
	INT nTagCnt;
	CStringArray SERIAL_LIST;
	CString strFuncInfo;
	CString strQuery, strValue;

	
	ids = RFID.GET_STATUS();
	if(0 > ids)
	{
		AfxMessageBox(_T("리더기를 찾을 수 없습니다."));
		return 1020;
	}

	ids = RFID.GetTagSerial(SERIAL_LIST);
	nTagCnt = SERIAL_LIST.GetSize();
	
	strFuncInfo.Format(_T("RFID_GetTagSerials(SerialList, %d)"), nTagCnt);
	CLocCommonAPI::MakeRFIDLog(strFuncInfo, _T("RFID Tag 동기화"), __WFILE__, __LINE__, ids);

	if(1 != nTagCnt)
	{
		if(0 == nTagCnt)
		{
			AfxMessageBox(_T("회원증이 존재하지 않습니다."));
			return 1030;
		}
		else if(1 < nTagCnt)
		{
			AfxMessageBox(_T("하나의 회원증만 올려주십시오."));
			return 1040;
		}	
		
		return 1050;
	}

	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	TmpDM.CheckManageCode();
	strQuery.Format(
		_T("SELECT BB.MANAGE_CODE, '단행테이블 : '||IB.TITLE_INFO||'('||BB.REG_NO||')', NULL ")
		_T("FROM BO_BOOK_TBL BB, IDX_BO_TBL IB ")
		_T("WHERE BB.SPECIES_KEY=IB.REC_KEY AND BB.WORKNO='%s' AND BB.REC_KEY <> %s ")
		_T("UNION ")
		_T("SELECT SB.MANAGE_CODE, '연속테이블 : '||SS.TITLE_INFO||'('||SB.REG_NO||')', NULL ")
		_T("FROM SE_BOOK_TBL SB, IDX_SE_TBL SS ")
		_T("WHERE SB.SPECIES_KEY=SS.REC_KEY AND SB.WORKNO='%s' AND SB.REC_KEY <> %s ")
		_T("UNION ")
		_T("SELECT CLU.MANAGE_CODE, '이용자테이블 : '||CLU.NAME||'('||CLU.USER_NO||')', CLU.WORKNO ")
		_T("FROM CO_LOAN_USER_TBL CLU ")
		_T("WHERE CLU.WORKNO='%s' AND CLU.REC_KEY <> %s")
		, SERIAL_LIST.GetAt(0), strRecKey, SERIAL_LIST.GetAt(0), strRecKey, SERIAL_LIST.GetAt(0), strRecKey);

	ids = TmpDM.RestructDataManager(strQuery);
	if(0 > ids){ERROR_MSG_RETURN_ERROR_CODE2(-1030, strFuncName);}

	INT nRowCnt = 0;	
	const INT ROW_COUNT = TmpDM.GetRowCount();
	const INT COL_CNT = 3;
		
	CString strMsg, strSerial, strManageCode, strSerialList;
	TmpDM.StartFrame();
	if(0 < ROW_COUNT)
	{
		for(nRowCnt = 0; nRowCnt < ROW_COUNT; nRowCnt++)
		{
			ids = TmpDM.GetCellData(nRowCnt, 0, strManageCode);
			if(0 > ids){ERROR_MSG_RETURN_ERROR_CODE2(-1040, strFuncName);}
			ids = TmpDM.GetCellData(nRowCnt, 1, strSerial);
			if(0 > ids){ERROR_MSG_RETURN_ERROR_CODE2(-1050, strFuncName);}
			if(0 > strSerial.Find(_T("이용자테이블")))
			{
				if(strManageCode != TmpDM.MANAGE_CODE)
				{
					TmpDM.EndFrame();
					strMsg.Format(_T("이 태그는 다른 도서관에서 등록된 태그이기 때문에 기록할 수 없습니다."));
					AfxMessageBox(strMsg);
					return 1060;
				}
			}
			strSerialList = strSerialList + _T("\r\n") + strSerial;
		}
		
		strMsg.Format(_T("현재 사용중인 태그입니다. 다시 입력하시겠습니까?\r\n%s"), strSerialList);
		if(IDYES == AfxMessageBox(strMsg, MB_YESNO)) 
		{
			strQuery.Format(_T("UPDATE BO_BOOK_TBL SET WORKNO=NULL WHERE WORKNO='%s';"), SERIAL_LIST.GetAt(0));
			TmpDM.AddFrame(strQuery);
			strQuery.Format(_T("UPDATE SE_BOOK_TBL SET WORKNO=NULL WHERE WORKNO='%s';"), SERIAL_LIST.GetAt(0));
			TmpDM.AddFrame(strQuery);

			CString strWorkNo;
			ids = TmpDM.GetCellData(0, 2, strWorkNo);
			if(0 > ids){ERROR_MSG_RETURN_ERROR_CODE2(-1060, strFuncName);}
			
			if(TRUE != strWorkNo.IsEmpty())
			{ 
				if(SERIAL_LIST.GetAt(0) == strWorkNo)
				{
					strQuery.Format(_T("UPDATE CO_LOAN_USER_TBL SET WORKNO=NULL WHERE WORKNO='%s';"), strWorkNo);
				}
			}
			else
			{
				if(SERIAL_LIST.GetAt(0) == strWorkNo)
				{
					strQuery.Format(_T("UPDATE CO_LOAN_USER_TBL SET WORKNO=NULL, CARD_PASSWORD=NULL WHERE WORKNO='%s';"), strWorkNo);
				}				
			}
			TmpDM.AddFrame(strQuery);
			ids = TmpDM.SendFrame();
			if(0 > ids){ERROR_MSG_RETURN_ERROR_CODE2(-1080, strFuncName);}
			TmpDM.EndFrame();
		}
		else
		{
			TmpDM.EndFrame();
			return 1070;
		}
	}

	TmpDM.EndFrame();
	
	strQuery.Format(_T("SELECT WORKNO FROM CO_LOAN_USER_TBL WHERE REC_KEY=%s"), strRecKey);
	ids = pDM->GetOneValueQuery(strQuery, strValue);
	if(0 > ids && -4007 != ids){ERROR_MSG_RETURN_ERROR_CODE2(-1120, strFuncName);}

	CInsertPasswordDlg InsertPasswordDlg(this);
	if(!strValue.IsEmpty())
	{
		if(strValue != SERIAL_LIST.GetAt(0))
		{
			InsertPasswordDlg.m_nViewMode = 1;
		}
	}

	InsertPasswordDlg.DoModal();

	INT nCloseMode = InsertPasswordDlg.GetCloseMode();
	if(nCloseMode == 0)
	{
		return 0;
	}
	strPassWord = InsertPasswordDlg.GetPassword();

	if(TRUE == m_pInfo->m_bCardPwEncryption && 64 > strPassWord.GetLength())
	{
		strEncryptionPassWord = CLocCommonAPI::StringToSHA256(strPassWord);
	}

	CString strUserNoAndName;
	CString sTempUserName;
	if(strUserName.GetLength() > 8)
	{
		sTempUserName = strUserName.Mid(0,8);
	}
	else
	{
		sTempUserName = strUserName;
	}
	strUserNoAndName.Format(_T("%s:%s"), strUserNo, sTempUserName);

	CString strRFID_Serial;	
	ids = RecordRFIDFromUserInfo(strLibCode, strUserNoAndName, strPassWord, strRecKey, strRFID_Serial);
	if(0 > ids)
	{
		strMsg.Format(_T("기록에 실패하였습니다."), strUserNo, strUserName);
		AfxMessageBox(strMsg);
		return 1;
	}
	else if(1 == ids)
	{
		return 1;
	}
	else if(2 == ids)
	{
		return 2;
	}
	else if(3 == ids)
	{
		AfxMessageBox(_T("책이 검색되어 진행 할 수 없습니다."));
		return 3;
	}

	CESL_DataMgr UpdateDM;
	UpdateDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	CKolas2up_MobileApi* pMobileApi = CLocCommonAPI::InitMobileApi(this, &UpdateDM);
	if(NULL == pMobileApi){ERROR_MSG_RETURN_ERROR_CODE2(-1140, strFuncName);}
	pMobileApi->StartFrame();
	pMobileApi->InitDBFieldData();

	if(TRUE == m_pInfo->m_bCardPwEncryption && 64 > strPassWord.GetLength()) {
		pMobileApi->AddDBFieldData(_T("CARD_PASSWORD"), _T("STRING"), strEncryptionPassWord);
	}
	else {
		pMobileApi->AddDBFieldData(_T("CARD_PASSWORD"), _T("STRING"), strPassWord);
	}
	
	pMobileApi->AddDBFieldData(_T("WORKNO"), _T("STRING"), strRFID_Serial);
	ids = pMobileApi->MakeUpdateFrame(_T("CO_LOAN_USER_TBL"), _T("REC_KEY"), _T("NUMERIC"), strRecKey);
	if(0 > ids){ERROR_MSG_RETURN_ERROR_CODE2(-1150, strFuncName);}
	ids = pMobileApi->SendFrame();
	if(0 > ids){ERROR_MSG_RETURN_ERROR_CODE2(-1160, strFuncName);}
	pMobileApi->EndFrame();
	CLocCommonAPI::MobileApiEndLog(pMobileApi);

	if(_T("Y") == m_sUseKL && _T("Y") == strKlUserYN)
	{
		CFrameWriter Writer(m_pInfo);
		Writer.setCommand(_T("IL_K24_LOC_B01_SERVICE"));
		Writer.addRecord();
		Writer.addElement(_T("통합대출자번호"), strUserNo);
		Writer.addElement(_T("회원증RFID"), SERIAL_LIST.GetAt(0));
		
		CString strModifyDate;
		CTime t = CLocCommonAPI::GetCurrentDateTime(this, strModifyDate);
		Writer.addElement(_T("수정일자"), strModifyDate);
		Writer.addElement(_T("수정도서관부호"), m_pInfo->LIB_CODE);
		
		CFrameSender Sender(m_pInfo);
		CFrameReader Reader;
		Sender.SendTempTbl(&Writer, GetWorkLogMsg( _T("통합RFID등록"), __WFILE__, __LINE__ ));	
	}

	strMsg.Format(_T("이용자번호 : [ %s ],  이름 : [ %s ]\r\n성공적으로 기록하였습니다."), strUserNo, strUserName);
	AfxMessageBox(strMsg);

	return 0;

EFS_END	
return -1;
}
INT CBO_LOC_3397::RecordRFIDFromUserInfo(CString strLibCode, CString strUserNo, CString strPassWord, CString strRecKey, CString &strRFID_Serail)
{
EFS_BEGIN
	INT ids;
	CRFID_Change RFID_Job;
	RFID_Job.m_pParentMgr = this;
	ids = RFID_Job.RecordRFIDFromUserInfo(strLibCode, strUserNo, strPassWord, strRFID_Serail);
	return ids;

EFS_END
return -1;
}