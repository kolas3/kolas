// BO_ClassCodeVolumeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BO_ClassCodeVolumeDlg.h"
#include "..\..\..\공통\MarcAdjustApi\CatApi_BO.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_ClassCodeVolumeDlg dialog


CBO_ClassCodeVolumeDlg::CBO_ClassCodeVolumeDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_ClassCodeVolumeDlg)
	m_str056 = _T("");
	m_str082 = _T("");
	m_str085 = _T("");
	//}}AFX_DATA_INIT

	m_strFileName = _T("..\\cfg\\분류기호판사항.txt");
}

CBO_ClassCodeVolumeDlg::~CBO_ClassCodeVolumeDlg()
{
}

VOID CBO_ClassCodeVolumeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_ClassCodeVolumeDlg)
	DDX_Text(pDX, IDC_e056, m_str056);	
	DDX_Text(pDX, IDC_e082, m_str082);	
	DDX_Text(pDX, IDC_e085, m_str085);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_ClassCodeVolumeDlg, CDialog)
	//{{AFX_MSG_MAP(CBO_ClassCodeVolumeDlg)
	ON_BN_CLICKED(IDC_bSAVE, OnbSAVE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_ClassCodeVolumeDlg message handlers

INT CBO_ClassCodeVolumeDlg::LoadConfig()
{
	CCatApi_BO CatApi(this);

	CString n056;
	CString n082;
	CString n085;	
	
	if (CatApi.LoadClassCodeVolumeDlg(m_strFileName, n056, n082, n085 ) < 0) return -1;

	if ( n056 == _T("-1") ) m_str056 = _T("");
	else m_str056 = n056;

	if ( n082 == _T("-1") ) m_str082 = _T("");
	else m_str082 = n082 ;
	
	if ( n085 == _T("-1") ) m_str085 = _T("");
	else m_str085 = n085;

	UpdateData(FALSE);

	return 0;
}

INT CBO_ClassCodeVolumeDlg::SaveConfig()
{
	UpdateData();

	INT n056;
	INT n082;
	INT n085;

	if ( m_str056.IsEmpty() ) n056 = -1;
	else n056 = _ttoi(m_str056.GetBuffer(0));
	
	if ( m_str082.IsEmpty() ) n082 = -1;
	else n082 = _ttoi(m_str082.GetBuffer(0));

	if ( m_str085.IsEmpty() ) n085 = -1;
	else n085 = _ttoi(m_str085.GetBuffer(0));

	CCatApi_BO CatApi(this);

	if (CatApi.SaveClassCodeVolumeDlg(m_strFileName, m_str056, m_str082, m_str085) < 0) return -1;

	INT ids = -1;	
	
	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);

	CString strQuery = _T("SELECT REC_KEY, FILE_NAME, VERSION FROM MN_MANAGE_FILE_TBL WHERE ALIAS = '분류판사항관리'");
	ids = dm.RestructDataManager(strQuery);
	if (ids < 0) return -1;

	INT nVersion = -1;
	CString strReckey;
	
	if (dm.GetRowCount() == 0)
	{
		TCHAR drive[_MAX_DRIVE];
		TCHAR dir[_MAX_DIR];
		TCHAR fname[_MAX_FNAME];
		TCHAR ext[_MAX_EXT];
		_tsplitpath( m_strFileName.GetBuffer(0), drive, dir, fname, ext);

		ids = dm.MakeRecKey(strReckey);
		if ( 0 > ids ) return -1;
		nVersion = 0;
		
		strQuery.Format(_T("INSERT INTO MN_MANAGE_FILE_TBL (FIRST_WORK, LAST_WORK, REC_KEY, ALIAS, FILE_NAME, VERSION) VALUES "
			_T("('%s','%s', %s, '분류판사항관리','%s.txt', 0);")),
			GetWorkLogMsg(_T("분류판사항관리"),WORK_LOG), GetWorkLogMsg(_T("분류판사항관리"),WORK_LOG), strReckey, fname);
		
		ids = dm.StartFrame();		
		if ( ids < 0 ) 
		{
			AfxMessageBox(_T("SendFrame 를 확인하세요"));
			return -1;
		}

		ids = dm.AddFrame(strQuery);
		if ( 0 > ids ) return -1;
		ids = dm.SendFrame();
		if (ids < 0) return ids;
		ids = dm.EndFrame();		
		if ( ids < 0 ) 
		{
			AfxMessageBox(_T("EndFrame 를 확인하세요"));
			return -1;
		}
	}
	else
	{
		strReckey = dm.GetCellData(0, 0);
		nVersion = _ttoi(dm.GetCellData(0, 2));
	}

	if (strReckey.IsEmpty() || nVersion < 0)
		return -1;

	CStdioFile file;
	file.Open(m_strFileName, CFile::modeRead | CFile::typeBinary);
	
		TCHAR cUni;
		file.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			file.SeekToBegin();
		}

	INT nFileSize = file.GetLength();
	BYTE *pBuffer = new BYTE[sizeof(BYTE) * (nFileSize) + 1024];
	ZeroMemory(pBuffer, nFileSize + 1024);
	nFileSize = file.ReadHuge(pBuffer, nFileSize);
	file.Close();

	ids = dm.LOBUpdate(_T("MN_MANAGE_FILE_TBL"), _T("REC_KEY"), _T("NUMERIC"), strReckey, _T("FILE_DATA"), pBuffer, nFileSize);
	if (ids < 0) goto ERR;

	strQuery.Format(_T("UPDATE MN_MANAGE_FILE_TBL SET VERSION = %d, FILE_SIZE = %d, LAST_WORK= '%s' WHERE REC_KEY = %s;")
					   , ++nVersion, nFileSize, GetWorkLogMsg(_T("분류판사항관리"),WORK_LOG), strReckey);

	ids = dm.StartFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("StartFrame 를 확인하세요"));
		return -1;
	}

	ids = dm.AddFrame(strQuery);
	if ( 0 > ids ) return -1;
	ids = dm.SendFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("SendFrame 를 확인하세요"));
		return -1;
	}

	if (ids < 0) goto ERR;
	ids = dm.EndFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("EndFrame 를 확인하세요"));
		return -1;
	}


	delete [] pBuffer;
	return 0;

ERR:
	delete [] pBuffer;
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// CBO_ClassCodeVolumeDlg message handlers

BOOL CBO_ClassCodeVolumeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (LoadConfig() < 0) 
	{
		AfxMessageBox(_T("설정값을 읽을수 없습니다."));
		EndDialog(IDCANCEL);
		return FALSE;
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_ClassCodeVolumeDlg::OnbSAVE() 
{
	if (SaveConfig() < 0)
		AfxMessageBox(_T("저장에 실패했습니다."));
	else
		AfxMessageBox(_T("저장하였습니다."));
}

HBRUSH CBO_ClassCodeVolumeDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
