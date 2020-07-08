// CardPrintDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CardPrintDlg.h"
#include "PreViewCardPrint.h"
#include "direct.h"
#include "FrameManager.h"
#include "..\\BL_LOC_USER\\KL_UserManagerDlg.h"
#include "StdioFileEx.h"
// 2011.09.08 ADD : 3000Api
#include "BO_LOC_3000Api.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCardPrintDlg dialog


CCardPrintDlg::CCardPrintDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	m_bIsKLReady = FALSE;
}
CCardPrintDlg::~CCardPrintDlg()
{
}

void CCardPrintDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCardPrintDlg, CDialog)
	//{{AFX_MSG_MAP(CCardPrintDlg)
	ON_BN_CLICKED(IDC_btnUserCardPrint, OnbtnUserCardPrint)
	ON_BN_CLICKED(IDC_btn1, Onbtn1)
	ON_BN_CLICKED(IDC_btnCLOSE, OnbtnCLOSE)
	ON_WM_SHOWWINDOW()
	ON_CBN_SELCHANGE(IDC_cmbCardFormat, OnSelchangecmbFORMAT)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER+1030, OnRFIDServiceMessage)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCardPrintDlg message handlers

BOOL CCardPrintDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	EnableThemeDialogTexture(GetSafeHwnd());

	if(0>InitESL_Mgr(_T("SM_CARD_PRINT_DLG")))
	{
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	Init();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCardPrintDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	if( bShow )
	{
		CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_cmbCardFormat);	
		pCmb->SetCurSel(GetFormat());

		LoadRFIDService();

	}
}

BOOL CCardPrintDlg::PreTranslateMessage(MSG* pMsg) 
{
	return CESL_Mgr::PreTranslateMessage(pMsg);
}

VOID CCardPrintDlg::Init()
{
	// 대출자번호가 주민번호일 경우 Show/Hide 여부 옵션
	INT ids = GetManageValue( _T("열람"), _T("공통"), _T("이용자관리"), _T("대출화면주민번호표시유무"), m_strIsChangeSecuriy);
	if(0>ids) return;
	
	InitFormatList();

	CESL_DataMgr* pUserDM = FindDM(_T("DM_회원증_이용자정보"));
	pUserDM->FreeData();

	CString sWhere;
	sWhere.Format( _T(" REC_KEY = %s ")  , m_strUserKey );
	pUserDM->RefreshDataManager( sWhere );

	// 2011.09.08 ADD : 3000Api
	if(TRUE == m_pInfo->m_bLibSecuUse && 0 < pUserDM->GetRowCount())
	{
		CStringArray asDes1;
		CString strGetData;
		for(INT i = 0; i < pUserDM->GetRowCount(); i++)
		{
			pUserDM->GetCellData(_T("주민등록번호"), i, strGetData);
			asDes1.Add(strGetData);
		}
		CBO_LOC_3000Api *p3000Api = new CBO_LOC_3000Api(this);
		if(NULL != p3000Api)
		{
			if(TRUE == p3000Api->ConnectSocket()) 
			{
				p3000Api->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("CIVIL_NO"), asDes1);
				for(i = 0; i < pUserDM->GetRowCount(); i++)
				{
					pUserDM->SetCellData(_T("주민등록번호"), asDes1.GetAt(i), i);
				}
				p3000Api->CloseSocket();
			}
			delete p3000Api;
			p3000Api = NULL;
		}
		asDes1.RemoveAll(); asDes1.FreeExtra();
	}

	if(_T("Y") == m_sIsUnityLoanService)
	{
		GetDlgItem(IDC_btn1)->ShowWindow(SW_SHOW);
		// 2012.10.29 ADD BY BJY : 2012년도 도서관 통합서비스 확대구축 사업 4회
		GetDlgItem(IDC_chkKLICON)->ShowWindow(SW_SHOW);
	}
}

void CCardPrintDlg::OnbtnUserCardPrint() 
{
	PrintCard();
}

void CCardPrintDlg::Onbtn1() 
{
	if(_T("Y") != m_sIsUnityLoanService)
	{
		m_bIsKLReady = FALSE;
		return;
	}

	CESL_DataMgr* pUserCardDM = FindDM(_T("DM_회원증_이용자정보"));
	CString strKLMemberYN = pUserCardDM->GetCellData(_T("통합회원전환여부"), 0);
	if(_T("Y") != strKLMemberYN)
	{
		AfxMessageBox(_T("선택된 이용자는 책이음회원이 아닙니다."));
		m_bIsKLReady = FALSE;
		return;
	}
	m_bIsKLReady = TRUE;
}

void CCardPrintDlg::OnbtnCLOSE() 
{
	OnCancel();	
}

void CCardPrintDlg::OnCancel()
{
	m_bIsKLReady = FALSE;
	UnLoadRFIDService();
	CDialog::OnCancel();
}

void CCardPrintDlg::OnSelchangecmbFORMAT() 
{
	SetCheckBackPagePrint();	
}

INT CCardPrintDlg::GetFormat()
{
	CString sFileName = _T("..\\cfg\\UserCardFormat.cfg");
	CFileFind ff;
	if(!ff.FindFile(sFileName))
	{ // 파일 없다면 0
		return 0;
	}
	else
	{ // 파일있다면 파일에 설정된 값으로 설정
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

INT CCardPrintDlg::SetFormat(INT nIdx)
{
	CString sFileName = _T("..\\cfg\\UserCardFormat.cfg");
	FILE *fp = _tfopen(sFileName, _T("w+b"));
	fputc(0xFF, fp);
	fputc(0xFE, fp);
	_ftprintf( fp, _T("%d"), nIdx);
	fclose( fp );

	return 0;
}

INT CCardPrintDlg::SetCheckBackPagePrint()
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbCardFormat);
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

	// 2009.10.15 ADD BY PWR : 마지막 선택한 양식으로 설정
	SetFormat(nIndex);

	return 0;
}

INT CCardPrintDlg::InitFormatList()
{

	INT ids;
	
	// ================================================================================
	// 1. 멀티콤보박스 초기화
	// ================================================================================
	CESL_ControlMgr* pCM = FindCM( _T("CM_CARD_PRINT_DLG") );
	
	CESL_ControlMultiComboBox* pCMCB = (CESL_ControlMultiComboBox*)(pCM->FindControl(_T("양식리스트")));
	if( NULL==pCMCB ) ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("InitFormatList") );
	pCMCB->InitMultiComboBox(2);
	
	CString sInsertCode[2] = { _T("적용안함                                    "), _T("") };
	ids = pCMCB->SetData( sInsertCode , 2 );
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("InitFormatList") );
	
	// ================================================================================
	// 2. 회원증DM  데이터를 멀티콤보에 입력 
	// ================================================================================
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
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("InitFormatList") );
	}
	
	// ================================================================================
	// 3. 마무리 
	// ================================================================================
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

INT CCardPrintDlg::LoadRFIDService()
{
 	if(_T("Y") != m_sIsUnityLoanService)
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

	// 임시
// 	return 0;

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

	// 이다음에 자식 프로세스가 종료되면 처리할 내용을 코딩
	CloseHandle( pinfo.hProcess );
	CloseHandle( pinfo.hThread );

	return 0;
}

INT CCardPrintDlg::UnLoadRFIDService()
{
 	if(_T("Y") != m_sIsUnityLoanService)
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

INT CCardPrintDlg::IsRegsisterUID(CString& strUserNo, CString& strUID, BOOL& bRegister)
{ // 10200
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

INT CCardPrintDlg::PrintCard()
{
	INT ids;

	// 뒷면 출력 여부
	BOOL bPrintBackPage;

	if( ((CButton*)GetDlgItem(IDC_chkTOGETHER))->GetCheck() == 1 )
		bPrintBackPage = TRUE;
	else
		bPrintBackPage = FALSE;

	CESL_ControlMultiComboBox* pCombo = (CESL_ControlMultiComboBox*)GetDlgItem(IDC_cmbCardFormat);
	INT nFormatIndex = pCombo->GetCurSel();	
	if( 0 == nFormatIndex )
	{
		AfxMessageBox(_T("양식을 선택해주십시요"));
		return 0;
	}

	// 뒷면을 출력할수 있는지 알아본다.
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
	
	// 구성한다.
	CESL_DataMgr* pUserDM = FindDM(_T("DM_회원증_이용자정보"));

	const INT nCnt = 2;
	CString sAlias[nCnt] = {_T("주민등록번호") , _T("사진")};

	CString sResult[nCnt];
	/// 사진 내려받기
	pUserDM->GetCellData( sAlias[1] , 0 , sResult[1] );

	CString sFileName;
	if( !sResult[1].IsEmpty() )
	{
		_tmkdir(_T("../대출증"));
		_tmkdir(_T("../대출증/이용자사진"));
		
		CString sFileName;
		sFileName.Format(_T("../대출증/이용자사진/%s.%s"),m_strUserKey,sResult[1]);
		// 2. 이용자 파일을 DB에서 가져오기
		BYTE *pBuffer = (BYTE *)malloc(MAX_BLOB_BUFFER);
		
		INT nSize;
		ids = pUserDM->LOBGet( _T("CO_LOAN_USER_TBL") , _T("REC_KEY") , _T("NUMERIC") , m_strUserKey , _T("USER_PICTURE") , pBuffer , &nSize );
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("PrintCard") );
		
		CFile fd;
		if (fd.Open( sFileName , CFile::modeCreate|CFile::modeWrite | CFile::typeBinary) == NULL)
		{
			AfxMessageBox(_T("대출/반납 Tmp File Open Error~"));
			free( pBuffer );
			return 1;
		}

		fd.WriteHuge(pBuffer, nSize);
		free( pBuffer );
		
		fd.Close();

		pUserDM->SetCellData( sAlias[1] , sFileName , 0 );
	}

	/// 주민등록번호 입력
	pUserDM->GetCellData( sAlias[0] , 0 , sResult[0] );
	if( !sResult[0].IsEmpty() )
	{
		CLocCommonAPI::ChangeSecuriyCivilNo( this , sResult[0] , m_sIsUnityLoanService );
		pUserDM->SetCellData( sAlias[0] , sResult[0], 0  );
	}
	
	pUserDM->SetCellData( _T("양식정보KEY") , sFrontFormKey , 0 );

	CString strKLMemberYN;
	if(_T("Y")==m_sIsUnityLoanService)
	{
		pUserDM->GetCellData( _T("통합회원전환여부") , 0 , strKLMemberYN );
		strKLMemberYN.TrimLeft();	strKLMemberYN.TrimRight();
		strKLMemberYN.MakeUpper();
	}
	
	if( bPrintBackPage )
	{
		/// 주민등록번호 입력
		pUserDM->GetCellData( sAlias[0] , 0 , sResult[0] );
		
		if( !sResult[0].IsEmpty() )
		{
			CLocCommonAPI::ChangeSecuriyCivilNo( this , sResult[0] , m_sIsUnityLoanService );
			pUserDM->SetCellData( sAlias[0] , sResult[0], 0  );
		}
		
		pUserDM->SetCellData( _T("양식정보KEY") , sBackFormKey , 0 );
		pUserDM->SetCellData( sAlias[1] , sFileName , 0 );
	}

	if(_T("Y")==m_sIsUnityLoanService)
	{
		if(_T("Y")==strKLMemberYN)
		{/// 통합회원의 회원증 출력시 양식은 '통합회원증'이 선택되어 있어야 한다.
			CString strCode;
			ids = pCombo->GetData(strCode, nFormatIndex, 0);
			if(_T("통합회원증")!=strCode)
			{
				if(IDYES != AfxMessageBox(_T("통합회원입니다. 기존 양식으로 출력하시겠습니까?"),MB_YESNO))
				{
					return 1020;
				}
			}
		}
		else
		{/// 일반회원의 회원증 출력시 양식은 '통합회원증'이 아닌 양식이 선택되어 있어야 한다.
// 			CString strCode;
// 			ids = pCombo->GetData(strCode, nFormatIndex, 0);
// 			if(_T("통합회원증")==strCode)
// 			{
// 				AfxMessageBox(	_T("일반회원의 회원증 출력시 양식은 '통합회원증'이 아닌 양식이 선택 되어 있어야 합니다.\r\n")
// 								_T("출력 양식을 확인해주십시요."));
// 				return 1030;
// 			}
		}
	}

	CPreViewCardPrint Dlg(this);
	// 2012.10.29 ADD BY BJY : 2012년도 도서관 통합서비스 확대구축 사업 4회
	if(1 == ((CButton*)GetDlgItem(IDC_chkKLICON))->GetCheck())
	{
		Dlg.m_bShowKLIconYN = TRUE;		
	}
	else
	{
		Dlg.m_bShowKLIconYN = FALSE;
	}
	Dlg.SetPrintList( pUserDM );
	Dlg.DoModal();

	// 2011.01.10 DELETE BY KSJ : 통합회원증 발급 이력 관리 이용자수정창에서 하도록 수정
//DEL 	if(Dlg.m_bIsPrint)
//DEL 	{
//DEL 		if(_T("Y") == m_sIsUnityLoanService)
//DEL 		{
//DEL 			// 통합 회원 관리 시스템 - 통합회원증 발급 이력 관리	
//DEL 			CString strKLMemberYN, strCardIssueYN;
//DEL 			INT nRowCnt = pUserDM->GetRowCount();
//DEL 
//DEL 			for(INT nRow = 0; nRow < nRowCnt; nRow++)
//DEL 			{
//DEL 				strKLMemberYN = pUserDM->GetCellData(_T("통합회원전환여부"), nRow);
//DEL 				strKLMemberYN.TrimLeft();	strKLMemberYN.TrimRight();
//DEL 
//DEL 				strCardIssueYN = pUserDM->GetCellData(_T("회원증출력여부"), nRow);
//DEL 				strCardIssueYN.TrimLeft();	strCardIssueYN.TrimRight();
//DEL 
//DEL 				if(_T("Y")==strKLMemberYN && _T("Y")==strCardIssueYN)
//DEL 				{
//DEL 					CKL_UserManagerDlg dlg(this);
//DEL 					dlg.SetMode(6);
//DEL 					dlg.SetUserNo(pUserDM->GetCellData(_T("이용자번호(TEXT)"), nRow));
//DEL 					// 2011.01.10 UPDATE BY KSJ : 미들웨어 통신이 안 됐을 경우 처리
//DEL 					//DEL dlg.DoModal();
//DEL 					dlg.KL_Membershipcard();
//DEL 				}
//DEL 			}
//DEL 		}
//DEL 	}

	return 0;
}

LRESULT CCardPrintDlg::OnRFIDServiceMessage(WPARAM wParam, LPARAM lParam)
{
	INT ids = 0;
	CStdioFileEx File;
	CString strLine, strCommand, strData1, strData2, strUserNo;

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

	if(!m_bIsKLReady) ids = -10005;

	CESL_DataMgr* pDM = FindDM(_T("DM_회원증_이용자정보"));
	if(NULL == pDM) ids = -10003;

	pDM->GetCellData( _T("이용자번호(TEXT)") , 0 , strUserNo);

	if(!File.Open(_T("..\\cfg\\KL\\KLRFIDR"), CFile::modeWrite|CFile::modeCreate|CStdioFileEx::modeWriteUnicode))
	{
		ids = -10004;
	}

	if(_T("KL_MEMBER_CARD_OPEN") == strCommand)
	{ // 10100
		if(m_bIsKLReady && ids >= 0 && !m_strUserKey.IsEmpty() && !strUserNo.IsEmpty())
		{
			File.WriteString(_T("KL_MEMBER_CARD_OPEN|성공"));
		}
		else
		{
			if(0 >= ids) ids = -10100;
			File.WriteString(_T("KL_MEMBER_CARD_OPEN|실패"));
		}
		File.Close();
		return ids;
	}
	else if(_T("KL_MEMBER_STATUS") == strCommand)
	{ // 10200
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
	{ // 10300
		File.WriteString(_T("KL_LIBRARY|") + m_pInfo->LIB_CODE);
		File.Close();
		return ids;
	}
	else if(_T("KL_MEMBER_NO") == strCommand)
	{ // 10400
		File.WriteString(_T("KL_MEMBER_NO|") + strUserNo);
		File.Close();
		return ids;
	}
	else if(_T("KL_MEMBER_WRTIE_RFID_UID") == strCommand)
	{ // 10500
		if(!strData1.IsEmpty())
		{
			if(strData1 != strUserNo)
			{
				File.WriteString(_T("KL_MEMBER_WRTIE_RFID_UID|실패"));
				ids = -10500;
			}
		}

		ids = HandphoneRFIDInput(m_strUserKey, strData2);
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
	{ // 10700
		File.WriteString(_T("KL_MEMBER_CARD_CLOSE|성공"));

		m_bIsKLReady = FALSE;
		File.Close();
		return ids;
	}		
	File.Close();

	return ids;
}

INT CCardPrintDlg::HandphoneRFIDInput(CString& strUserKey, CString& strUID)
{
	CString strQuery;
	CESL_DataMgr* pDM = FindDM(_T("DM_회원증_이용자정보"));

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
	strQuery.Format(_T("UPDATE CO_LOAN_USER_TBL SET WORKNO = '%s' WHERE REC_KEY = %s;"), strUID, m_strUserKey);
	TmpDM.AddFrame(strQuery);
	INT ids = TmpDM.SendFrame();
	TmpDM.FreeFrame();
	TmpDM.EndFrame();

	if(0 == ids)
	{
		CString strUserNo = pDM->GetCellData(_T("이용자번호(TEXT)") , 0);

		CFrameWriter Writer(m_pInfo);
		Writer.setCommand(_T("IL_K24_LOC_B01_SERVICE"));
		Writer.addRecord();
		Writer.addElement(_T("통합대출자번호"), strUserNo);
		Writer.addElement(_T("회원증RFID"), strUID);

		// 2011.02.02 ADD BY KSJ : 수정일자 추가
		CString strModifyDate;
		// 2011.02.15 ADD BY KSJ : 함수변경
		//DEL strModifyDate.Format(_T("%d/%d/%d %02d:%02d:%02d"), tCurrentTime.GetYear(), tCurrentTime.GetMonth(), tCurrentTime.GetDay() , tCurrentTime.GetHour(), tCurrentTime.GetMinute(), tCurrentTime.GetSecond());
		CTime t = CLocCommonAPI::GetCurrentDateTime(this, strModifyDate);
		Writer.addElement(_T("수정일자"), strModifyDate);
		Writer.addElement(_T("수정도서관부호"), m_pInfo->LIB_CODE);

		CFrameSender Sender(m_pInfo);
		CFrameReader Reader;
		Sender.SendTempTbl(&Writer, GetWorkLogMsg( _T("통합RFID등록"), __WFILE__, __LINE__ ));
	}

	return ids;
}
