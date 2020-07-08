// KL_UserMergeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "KL_UserMerge.h"
#include "KL_UserMergeDlg.h"
#include "FrameManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKL_UserMergeDlg dialog

CKL_UserMergeDlg::CKL_UserMergeDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CKL_UserMergeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKL_UserMergeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CKL_UserMergeDlg::~CKL_UserMergeDlg()
{
	
}

void CKL_UserMergeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKL_UserMergeDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CKL_UserMergeDlg, CDialog)
	//{{AFX_MSG_MAP(CKL_UserMergeDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_btnSearch, OnbtnSearch)
	ON_BN_CLICKED(IDC_btnClose, OnbtnClose)
	ON_BN_CLICKED(IDC_btnMerge, OnbtnMerge)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKL_UserMergeDlg message handlers

BOOL CKL_UserMergeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	SetDlgItemText(IDC_IPADDRESS1, _T("192.168.100.109"));
	SetDlgItemText(IDC_edtPort, _T("53205"));

	if(InitESL_Mgr(_T("KL_UserMerge")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CKL_UserMergeDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CKL_UserMergeDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CKL_UserMergeDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CKL_UserMergeDlg::OnbtnSearch() 
{
	CString strQuery = _T("SELECT GPIN_HASH_CI FROM KL_MASTER_USER_TBL WHERE GPIN_HASH_CI IS NOT NULL GROUP BY GPIN_HASH_CI HAVING COUNT(1)>1");
	CESL_DataMgr* pDM = FindDM(_T("DM_KL_UserMerge"));
	pDM->FreeData();
	CESL_ControlMgr* pCM = FindCM(_T("CM_KL_UserMerge"));
	pCM->ControlDisplay(_T("MAINGRID"));

	CString strResult;
	INT nIds = StartQuery(_T("TCT"), _T(""), _T("R"), strQuery, pDM, strResult);
	if(0 > nIds)
	{
		strResult.Format(_T("미들웨어와의 통신에 실패하였습니다. (%d)"), nIds);
		AfxMessageBox(strResult);
	}
	else if(100 == nIds)
	{
		AfxMessageBox(strResult);
	}
	if(1 > pDM->GetRowCount())
	{
		AfxMessageBox(_T("중복 이용자가 없습니다."));
	}

	pCM->ControlDisplay(_T("MAINGRID"));
}

void CKL_UserMergeDlg::OnbtnClose() 
{
	OnCancel();
}

void CKL_UserMergeDlg::OnbtnMerge() 
{
	CESL_DataMgr* pDM = FindDM(_T("DM_KL_UserMerge"));
	if(NULL == pDM) return;
	if(1 > pDM->GetRowCount()) return;

	CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_KL_UserMerge"),_T("MAINGRID"));
	if(NULL == pGrid) return;
	pGrid->SelectMakeList();
	pGrid->SelectGetHeadPosition();
	int nIds = 0;
	int nIndex = 0;
	CString strGetData;
	CString strResult;
	for(int i = 0; i < pGrid->SelectGetCount(); i++)
	{
		nIndex = pGrid->SelectGetIdx();
		pDM->GetCellData(_T("GPIN_HASH_CI"), nIndex, strGetData);


		nIds = StartMerge(strGetData, strResult);
		if(0 > nIds)
		{
			pDM->SetCellData(_T("RESULT"), strResult, nIndex);
			pGrid->DisplayLine(nIndex);
		}
		else
		{
			pDM->SetCellData(_T("RESULT"), _T("성공"), nIndex);
			pGrid->DisplayLine(nIndex);
		}

		pGrid->SelectGetNext();
	}
}

int CKL_UserMergeDlg::StartQuery(CString strTarget, CString strLibCode, CString strClass, CString strQuery, CESL_DataMgr* pDM, CString &strResult)
{
	CFrameWriter Writer(m_pInfo);
	Writer.setCommand(_T("IL_K95_TCT_B01_SERVICE"));
	Writer.addRecord();
	Writer.addElement(_T("대상"), strTarget);
	Writer.addElement(_T("도서관부호"), strLibCode);
	Writer.addElement(_T("DML종류"), strClass);
	Writer.addElement(_T("쿼리"), strQuery);	
	CFrameReader Reader;
	CFrameSender Sender(m_pInfo);
	CString strIP;
	GetDlgItemText(IDC_IPADDRESS1, strIP);
	Sender.SetIP(strIP);
	CString strPort;
	GetDlgItemText(IDC_edtPort, strPort);
	Sender.SetPort(_ttoi(strPort));
	INT ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
	if(0 > ids) return ids;

	if(_T("ERROR") == Reader.getCommand())
	{
		strResult = getErrorMsg(Reader.getElement(0, _T("ID")));
		return 100;
	}

	Reader.makeDM(pDM);
	
	return 0;
}

int CKL_UserMergeDlg::StartMerge(CString strData, CString &strResult)
{
	// CI값으로 이용자 조회 - create_date desc
	CString strQuery;
	strQuery.Format(_T("SELECT DISTINCT LIB_CODE FROM KL_LIBRARY_USER_TBL WHERE MASTER_USER_KEY IN (SELECT MASTER_USER_KEY FROM KL_MASTER_USER_TBL WHERE GPIN_HASH_CI='%s')"), strData);
	CESL_DataMgr dmLib;
	INT nIds = StartQuery(_T("TCT"), _T(""), _T("R"), strQuery, &dmLib, strResult);
	if(0 > nIds)
	{
		strResult.Format(_T("미들웨어와의 통신에 실패하였습니다. (%d)"), nIds);
		return nIds;
	}
	else if(100 == nIds)
	{
		return -100;
	}
	if(1 > dmLib.GetRowCount())
	{
		strResult = _T("가입도서관 조회안됨");
		return -1;
	}
	// 통합센터 중복 이용자 정보 조회
	strQuery.Format(_T("SELECT USER_NO,USER_ID,PASSWORD,AGREE_YN,AGREE_DATE,GPIN_HASH,GPIN_SEX,MASTER_USER_KEY FROM KL_MASTER_USER_TBL WHERE GPIN_HASH_CI='%s' ORDER BY CREATE_DATE DESC"), strData);
	CESL_DataMgr dmCenterUser;
	nIds = StartQuery(_T("TCT"), _T(""), _T("R"), strQuery, &dmCenterUser, strResult);
	if(0 > nIds)
	{
		strResult.Format(_T("미들웨어와의 통신에 실패하였습니다. (%d)"), nIds);
		return nIds;
	}
	else if(100 == nIds)
	{
		return -100;
	}
	if(1 > dmCenterUser.GetRowCount())
	{
		strResult = _T("조회안됨");
		return -1;
	}
	else if(2 < dmCenterUser.GetRowCount())
	{
		strResult = _T("2건초과 중복");
		return -2;
	}
	CString strUserNo_A, strUserID_A, strPassWord_A, strAgreeYN_A, strAgreeDate_A, strGpinHash_A, strGpinSex_A, strMasterUserKey_A;
	CString strUserNo_B, strUserID_B, strPassWord_B, strAgreeYN_B, strAgreeDate_B, strGpinHash_B, strGpinSex_B, strMasterUserKey_B;
	dmCenterUser.GetCellData(0, 0, strUserNo_A);
	if(strUserNo_A.IsEmpty())
	{
		strResult = _T("대출자번호 없음");
		return -3;
	}
	dmCenterUser.GetCellData(0, 1, strUserID_A);
	strUserID_A.TrimLeft(); strUserID_A.TrimRight();
	dmCenterUser.GetCellData(0, 2, strPassWord_A);
	strPassWord_A.TrimLeft(); strPassWord_A.TrimRight();
	dmCenterUser.GetCellData(0, 3, strAgreeYN_A);
	dmCenterUser.GetCellData(0, 4, strAgreeDate_A);
	dmCenterUser.GetCellData(0, 5, strGpinHash_A);
	dmCenterUser.GetCellData(0, 6, strGpinSex_A);
	if(strGpinSex_A.IsEmpty() || (_T("0") != strGpinSex_A && _T("1") != strGpinSex_A))
	{
		strResult = _T("GPIN_SEX 오류");
		return -3;
	}
	dmCenterUser.GetCellData(0, 7, strMasterUserKey_A);
	if(strMasterUserKey_A.IsEmpty())
	{
		strResult = _T("기준이용자 MASTER_USER_KEY 없음");
		return -4;
	}
	dmCenterUser.GetCellData(1, 0, strUserNo_B);
	dmCenterUser.GetCellData(1, 1, strUserID_B);
	strUserID_B.TrimLeft(); strUserID_B.TrimRight();
	dmCenterUser.GetCellData(1, 2, strPassWord_B);
	strPassWord_B.TrimLeft(); strPassWord_B.TrimRight();
	dmCenterUser.GetCellData(1, 3, strAgreeYN_B);
	dmCenterUser.GetCellData(1, 4, strAgreeDate_B);
	dmCenterUser.GetCellData(1, 5, strGpinHash_B);
	dmCenterUser.GetCellData(1, 6, strGpinSex_B);
	dmCenterUser.GetCellData(1, 7, strMasterUserKey_B);
	if(strMasterUserKey_B.IsEmpty())
	{
		strResult = _T("삭제이용자 MASTER_USER_KEY 없음");
		return -5;
	}
	CString strSet;
	if(strUserID_A.IsEmpty() && !strUserID_B.IsEmpty())
	{
		if(!strSet.IsEmpty()) strSet += _T(",");
		strSet += _T("USER_ID='") + strUserID_B + _T("'"); 
	}
	if(strPassWord_A.IsEmpty() && !strPassWord_B.IsEmpty())
	{
		if(!strSet.IsEmpty()) strSet += _T(",");
		strSet += _T("PASSWORD='") + strPassWord_B + _T("'"); 
	}
	if(strAgreeYN_A.IsEmpty() && !strAgreeYN_B.IsEmpty())
	{
		if(!strSet.IsEmpty()) strSet += _T(",");
		strSet += _T("AGREE_YN='") + strAgreeYN_B + _T("'"); 
	}
	if(strAgreeDate_A.IsEmpty() && !strAgreeDate_B.IsEmpty())
	{
		if(!strSet.IsEmpty()) strSet += _T(",");
		strSet += _T("AGREE_DATE='") + strAgreeDate_B + _T("'"); 
	}
	if(strGpinHash_A.IsEmpty() && !strGpinHash_B.IsEmpty())
	{
		if(!strSet.IsEmpty()) strSet += _T(",");
		strSet += _T("GPIN_HASH='") + strGpinHash_B + _T("'");
		strGpinHash_A = strGpinHash_B;
	}
	// 가입도서관 수정
	//  - 중복시 실패
	//  - 대출자번호 확인후 수정
	CString strWhere;
	if(strGpinHash_A.IsEmpty())
	{
		strWhere.Format(_T("IPIN_HASH='%s'"), strData);
	}
	else
	{
		strWhere.Format(_T("GPIN_HASH='%s' OR IPIN_HASH='%s'"), strGpinHash_A, strData);
	}
	strQuery.Format(_T("SELECT REC_KEY,USER_NO,GPIN_HASH,GPIN_SEX,IPIN_HASH FROM CO_LOAN_USER_TBL WHERE %s"), strWhere);
	CString strLibCode;
	CString strLocalPK, strLocalUserNo, strLocalGpinHash, strLocalGpinSex, strLocalCI, strLocalSet;
	CESL_DataMgr dmLocalUser;
	CESL_DataMgr dmLocalUp;
	CString strGetData;
	for(int i = 0; i < dmLib.GetRowCount(); i++)
	{
		dmLib.GetCellData(i, 0, strLibCode);
		if(strLibCode.IsEmpty())
		{
			strResult = _T("LIB_CODE IS NULL");
			return -6;
		}
		dmLocalUser.FreeData();
		nIds = StartQuery(_T("LOC"), strLibCode, _T("R"), strQuery, &dmLocalUser, strResult);
		if(0 > nIds)
		{
			strResult.Format(_T("미들웨어와의 통신에 실패하였습니다. (%d)"), nIds);
			return nIds;
		}
		else if(100 == nIds)
		{
			return -100;
		}
		if(1 > dmLocalUser.GetRowCount())
		{
			strResult.Format(_T("%s 도서관 이용자 조회안됨"), strLibCode);
			return -7;
		}
		else if(1 < dmLocalUser.GetRowCount())
		{
			strResult.Format(_T("%s 도서관 이용자 중복"), strLibCode);
			return -8;
		}
		dmLocalUser.GetCellData(0, 0, strLocalPK);
		if(strLocalPK.IsEmpty())
		{
			strResult.Format(_T("%s 도서관 PK IS NULL"), strLibCode);
			return -9;
		}
		dmLocalUser.GetCellData(0, 1, strLocalUserNo);
		if(strLocalUserNo.IsEmpty())
		{
			strResult.Format(_T("%s 도서관 대출자번호 없음"), strLibCode);
			return -9;
		}
		dmLocalUser.GetCellData(0, 2, strLocalGpinHash);
		dmLocalUser.GetCellData(0, 3, strLocalGpinSex);
		dmLocalUser.GetCellData(0, 4, strLocalCI);
		strLocalSet.Empty();
		if(strUserNo_A != strLocalUserNo)
		{
			if(!strLocalSet.IsEmpty()) strLocalSet += _T(",");
			strLocalSet += _T("USER_NO='") + strUserNo_A + _T("'");
		}
		if(strLocalGpinHash.IsEmpty())
		{
			if(!strLocalSet.IsEmpty()) strLocalSet += _T(",");
			strLocalSet += _T("GPIN_HASH='") + strGpinHash_A + _T("'");
		}
		if(strLocalGpinSex.IsEmpty() || (_T("0") != strLocalGpinSex && _T("1") != strLocalGpinSex))
		{
			if(!strLocalSet.IsEmpty()) strLocalSet += _T(",");
			strLocalSet += _T("GPIN_SEX='") + strLocalGpinSex + _T("'");
		}
		if(strLocalCI.IsEmpty())
		{
			if(!strLocalSet.IsEmpty()) strLocalSet += _T(",");
			strLocalSet += _T("IPIN_HASH='") + strLocalCI + _T("'");
		}
		if(!strLocalSet.IsEmpty())
		{
			strQuery.Format(_T("UPDATE CO_LOAN_USER_TBL SET %s WHERE REC_KEY=%s"), strLocalSet, strLocalPK);
			dmLocalUp.FreeData();
			nIds = StartQuery(_T("LOC"), strLibCode, _T("U"), strQuery, &dmLocalUp, strResult);
			if(0 > nIds)
			{
				strResult.Format(_T("미들웨어와의 통신에 실패하였습니다. (%d)"), nIds);
				return nIds;
			}
			else if(100 == nIds)
			{
				return -100;
			}
			if(1 != dmLocalUp.GetRowCount())
			{
				strResult.Format(_T("%s 도서관 이용자 수정실패"), strLibCode);
				return -7;
			}
			dmLocalUp.GetCellData(_T(""), 0, strGetData);
			if(strGetData.IsEmpty())
			{
				strResult.Format(_T("%s 도서관 이용자 수정실패"), strLibCode);
				return -7;
			}
			if(1 > _ttoi(strGetData))
			{
				strResult.Format(_T("%s 도서관 이용자 수정실패"), strLibCode);
				return -7;
			}
		}
	}
	// 지역센터 수정
	//  - 중복시 (중복시 기준대출자번호로 병합, 없으면 실패)
	//  - 대출자번호 확인후 수정
	// 통합센터 병합
	//  - 기준 데이터로 병합
	
	// 가입도서관에 동기화프레임 기록

	return 0;
}

CString CKL_UserMergeDlg::getErrorMsg(CString strErrorID)
{
	CString strErrorMsg = _T("");
	if(strErrorID == _T("MW95TCTB01MW01"))
	{
		strErrorMsg = _T("통합센터 프레임 파싱 오류입니다.");
	}
	else if(strErrorID == _T("MW95TCTB01SY01") || strErrorID == _T("MW95LCTB01SY01") || strErrorID == _T("MW95LOCB01SY01"))
	{
		strErrorMsg = _T("[대상] 값은 필수사항입니다. - TCT, LCT, LOC");
	}
	else if(strErrorID == _T("MW95TCTB01SY02") || strErrorID == _T("MW95LCTB01SY02") || strErrorID == _T("MW95LOCB01SY02"))
	{
		strErrorMsg = _T("[도서관부호] 값은 필수사항입니다.");
	}
	else if(strErrorID == _T("MW95TCTB01SY03") || strErrorID == _T("MW95LCTB01SY03") || strErrorID == _T("MW95LOCB01SY03"))
	{
		strErrorMsg = _T("[DML종류] 값은 필수사항입니다. - C, R, U, D");
	}
	else if(strErrorID == _T("MW95TCTB01SY04") || strErrorID == _T("MW95LCTB01SY04") || strErrorID == _T("MW95LOCB01SY04"))
	{
		strErrorMsg = _T("[쿼리] 값은 필수사항입니다.");
	}
	else if( strErrorID == _T("MW95TCTB01MW02"))
	{
		strErrorMsg = _T("통합센터 DB접속에 실패하였습니다.");
	}
	else if(strErrorID == _T("MW95TCTB01DB01"))
	{
		strErrorMsg = _T("통합센터 INSERT 쿼리에 오류가 있습니다.");
	}
	else if(strErrorID == _T("MW95TCTB01DB02"))
	{
		strErrorMsg = _T("통합센터 SELECT 쿼리에 오류가 있습니다.");
	}
	else if(strErrorID == _T("MW95TCTB01DB03"))
	{
		strErrorMsg = _T("통합센터 UPDATE 쿼리에 오류가 있습니다.");
	}
	else if(strErrorID == _T("MW95TCTB01DB04"))
	{
		strErrorMsg = _T("통합센터 DELETE 쿼리에 오류가 있습니다.");
	}
	else if(strErrorID == _T("MW95TCTB01DB05"))
	{
		strErrorMsg = _T("지역 대표 도서관부호 조회에 실패하였습니다.");
	}
	else if(strErrorID == _T("MW95TCTB01DT01"))
	{
		strErrorMsg = _T("지역 대표 도서관부호가 없습니다.");
	}
	else if(strErrorID == _T("MW95TCTB01MW03"))
	{
		strErrorMsg = _T("지역센터 접속 초기화에 실패하였습니다.");
	}
	else if(strErrorID == _T("MW95TCTB01MW04"))
	{
		strErrorMsg = _T("지역센터 접속 초기화에 실패하였습니다.");
	}
	else if(strErrorID == _T("MW95TCTB01MW05"))
	{
		strErrorMsg = _T("지역센터 요청에 실패하였습니다.");
	}
	else if(strErrorID == _T("MW95TCTB01ET02"))
	{
		strErrorMsg = _T("지역센터 응답 오류 입니다. - 지역센터에 해당 서비스가 없는 경우 발생합니다.");
	}
	else if(strErrorID == _T("MW95TCTB01ET01"))
	{
		strErrorMsg = _T("통합센터에서 예외상황이 발생하여 중지되었습니다.");
	}
	else if(strErrorID == _T("MW95LCTB01MW01"))
	{
		strErrorMsg = _T("지역센터 프레임 파싱 오류입니다.");
	}
	else if(strErrorID == _T("MW95LCTB01MW02"))
	{
		strErrorMsg = _T("지역센터 DB접속에 실패하였습니다.");
	}
	else if(strErrorID == _T("MW95LCTB01DB01"))
	{
		strErrorMsg = _T("지역센터 INSERT 쿼리에 오류가 있습니다.");
	}
	else if(strErrorID == _T("MW95LCTB01DB02"))
	{
		strErrorMsg = _T("지역센터 SELECT 쿼리에 오류가 있습니다.");
	}
	else if(strErrorID == _T("MW95LCTB01DB03"))
	{
		strErrorMsg = _T("지역센터 UPDATE 쿼리에 오류가 있습니다.");
	}
	else if(strErrorID == _T("MW95LCTB01DB04"))
	{
		strErrorMsg = _T("지역센터 DELETE 쿼리에 오류가 있습니다.");
	}
	else if(strErrorID == _T("MW95LCTB01MW04"))
	{
		strErrorMsg = _T("도서관 접속 초기화에 실패하였습니다.");
	}
	else if(strErrorID == _T("MW95LCTB01MW05"))
	{
		strErrorMsg = _T("도서관 요청에 실패하였습니다.");
	}
	else if(strErrorID == _T("MW95LCTB01ET02"))
	{
		strErrorMsg = _T("도서관 응답 오류 입니다. - 도서관에 해당 서비스가 없는 경우 발생합니다.");
	}
	else if(strErrorID == _T("MW95LCTB01ET01"))
	{
		strErrorMsg = _T("지역센터에서 예외상황이 발생하여 중지되었습니다.");
	}
	else if(strErrorID == _T("MW95LOCB01MW01"))
	{
		strErrorMsg = _T("도서관 프레임 파싱 오류입니다.");
	}
	else if(strErrorID == _T("MW95LOCB01MW02"))
	{
		strErrorMsg = _T("도서관 DB접속에 실패하였습니다.");
	}
	else if(strErrorID == _T("MW95LOCB01DB01"))
	{
		strErrorMsg = _T("도서관 INSERT 쿼리에 오류가 있습니다.");
	}
	else if(strErrorID == _T("MW95LOCB01DB02"))
	{
		strErrorMsg = _T("도서관 SELECT 쿼리에 오류가 있습니다.");
	}
	else if(strErrorID == _T("MW95LOCB01DB03"))
	{
		strErrorMsg = _T("도서관 UPDATE 쿼리에 오류가 있습니다.");
	}
	else if(strErrorID == _T("MW95LOCB01DB04"))
	{
		strErrorMsg = _T("도서관 DELETE 쿼리에 오류가 있습니다.");
	}
	else if(strErrorID == _T("MW95LOCB01ET01"))
	{
		strErrorMsg = _T("도서관에서 예외상황이 발생하여 중지되었습니다.");
	}
	else
	{
		strErrorMsg = _T("알수없는 오류가 발생하였습니다.");
	}
	return strErrorMsg;
}