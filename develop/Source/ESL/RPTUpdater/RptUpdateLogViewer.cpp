// RptUpdateLogViewer.cpp : implementation file
//

#include "stdafx.h"
#include "RPTUpdater.h"
#include "RptUpdateLogViewer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRptUpdateLogViewer dialog


CRptUpdateLogViewer::CRptUpdateLogViewer(CWnd* pParent /*=NULL*/)
	: CDialog(CRptUpdateLogViewer::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRptUpdateLogViewer)
	m_strDate1 = _T("");
	m_strDate2 = _T("");
	m_strLibName = _T("");
	m_strSeqNo = _T("");
	//}}AFX_DATA_INIT
	
	m_strRptAlias = _T("");
	m_pGridLog = NULL;

}

CRptUpdateLogViewer::~CRptUpdateLogViewer()
{
	if(m_pGridLog)
	{
		delete m_pGridLog;
		m_pGridLog = NULL;
	}

}

VOID CRptUpdateLogViewer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRptUpdateLogViewer)
	DDX_Text(pDX, IDC_edtLOG_DATE1, m_strDate1);
	DDX_Text(pDX, IDC_edtLOG_DATE2, m_strDate2);
	DDX_Text(pDX, IDC_edtLOG_LIBNAME, m_strLibName);
	DDX_Text(pDX, IDC_edtLOG_SEQ, m_strSeqNo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRptUpdateLogViewer, CDialog)
	//{{AFX_MSG_MAP(CRptUpdateLogViewer)
	ON_BN_CLICKED(IDC_btnLOG_SEARCH, OnbtnLOGSEARCH)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRptUpdateLogViewer message handlers

BOOL CRptUpdateLogViewer::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if(m_strRptAlias.IsEmpty())
	{
		AfxMessageBox(_T("레포트 알리아스가 없습니다!"));
		EndDialog(0);		
		return FALSE;
	}

	CString strTmp;
	strTmp.Format(_T("[ %s ] 로그 보기"),m_strRptAlias);
	SetWindowText(strTmp);
	

	if(ReadCfg()<0)
	{
		AfxMessageBox(_T("CFG 정보를 읽을 수 없습니다!"));
		EndDialog(0);
		return FALSE;
	}
	
	InitGrid();

	if(ShowLog(_T(""))<0)
	{
		EndDialog(0);
		return FALSE;
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CRptUpdateLogViewer::ReadCfg()
{
	CStdioFile fileLogCfg;
	CString strPath;

	strPath.Format(_T("%sLog.cfg"),CFG_DIRECTORY);
	

	if (!fileLogCfg.Open(strPath, CFile::modeRead | CFile::typeBinary))
	{
		AfxMessageBox(_T("Log.cfg file을 찾을 수 없습니다!"));
		return -1;
	}
	
	TCHAR cUni;
	fileLogCfg.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fileLogCfg.SeekToBegin();
	}
	
	CString strLine;
	CString strConnectInfo;

	while(fileLogCfg.ReadString(strLine))
	{
		strLine.TrimRight();
		strLine.TrimRight();
		if(strLine.IsEmpty()) continue;
	
		strConnectInfo = strLine;
	
	}
	
	if(strConnectInfo.IsEmpty()) return -1;

	m_DM_LOG.SetCONNECTION_INFO(strConnectInfo);

	return 0;
}

INT CRptUpdateLogViewer::ShowLog(CString strEtcWhere,BOOL bFirstShow)
{

/*
		   PK INT NOT NULL,
           SEQ_NUM INT NOT NULL,
           TRIAL_CNT INT,
           LIB_KEY INT NOT NULL,
           RPT_ALIAS VARCHAR(255),
           QUERY_TYPE TCHAR(1),        -- T(TRIAL), I(INSERT), U(UPDATE) , F(FIRE WALL)
           DATE DATE,
           TIME TIME,
           RESULT TCHAR(1),			  -- 0(CONNECTION FAIL) , 1(SQL FAIL) , 2(LOB FAILE), 3(SUCCESS), 4(WAIT)	
           PRIMARY KEY (PK));
*/

	CString strQuery;

	if(strEtcWhere.IsEmpty())
	{
		strQuery.Format(_T("SELECT P.LIBRARY_NAME,R.SEQ_NUM,R.TRIAL_CNT,R.QUERY_TYPE,R.DATE,R.TIME,R.RESULT ")
						_T("FROM RPT_UPDATER_LOG_TBL R,PATCH_LIBRARY_TBL P ")
						_T("WHERE R.LIB_KEY = P.PK AND R.RPT_ALIAS = '%s'")
						,m_strRptAlias);

	}
	else 
	{
		strQuery.Format(_T("SELECT P.LIBRARY_NAME,R.SEQ_NUM,R.TRIAL_CNT,R.QUERY_TYPE,R.DATE,R.TIME,R.RESULT ")
						_T("FROM RPT_UPDATER_LOG_TBL R,PATCH_LIBRARY_TBL P ")
						_T("WHERE R.LIB_KEY = P.PK AND R.RPT_ALIAS = '%s'%s")
						,m_strRptAlias,strEtcWhere);

	}
	
	m_DM_LOG.RestructDataManager(strQuery);

	INT nRow = m_DM_LOG.GetRowCount();
	CString msg;

	if(0 == nRow) 
	{
		
		if(bFirstShow)
		{
			msg.Format(_T("[ %s ] 알리아스에 대한 로그가 없습니다!"),m_strRptAlias);
			AfxMessageBox(msg);
			return -1;
		}
		else 
		{
			
			msg.Format(_T("검색 결과가 없습니다!"));
			AfxMessageBox(msg);
		}
	
	
	}

	ShowGrid();

	return 0;

}

VOID CRptUpdateLogViewer::InitGrid()
{
	
	// Grid 초기화
	m_pGridLog = new CESL_Grid(NULL);
	m_pGridLog->SubclassDlgItem(IDC_gridLOG, this);


}

VOID CRptUpdateLogViewer::ShowGrid()
{

	m_pGridLog->SetCols(0,8);
    m_pGridLog->SetColumnInfo(0-1, _T(""),					0,				1);
	m_pGridLog->SetColumnInfo(1-1, _T("도서관명"),			200,			1);
	m_pGridLog->SetColumnInfo(2-1, _T("차수"),				40,  			1);
	m_pGridLog->SetColumnInfo(3-1, _T("횟수"),				40,				1);
	m_pGridLog->SetColumnInfo(4-1, _T("타입"),				80,				1);
	m_pGridLog->SetColumnInfo(5-1, _T("실행일"),			100,  			1);
	m_pGridLog->SetColumnInfo(6-1, _T("실행시각"),			100,  			1);
	m_pGridLog->SetColumnInfo(7-1, _T("결과"),				120,			1);

	INT nRow = m_DM_LOG.GetRowCount();

	if(nRow>0)
	{
		m_pGridLog->SetRows(nRow+1);
	}
	else
	{
		m_pGridLog->SetRows(2);
		m_pGridLog->SetRow(1);
		
		for(INT i=0;i<8;i++)
		{
			m_pGridLog->SetCol(i);
			m_pGridLog->SetText(_T(""));
		
		}

	}

	CString strLibName;
	CString strSeq;
	CString strTrial;
	CString strType;
	CString strDate;
	CString strTime;
	CString strResult;

	for(INT i=0;i<nRow;i++)
	{		
		m_pGridLog->SetRow(i+1);
		
		m_pGridLog->SetCol(1);
		strLibName = m_DM_LOG.GetCellData(i,0);
		m_pGridLog->SetText(strLibName);

		m_pGridLog->SetCol(2);
		strSeq = m_DM_LOG.GetCellData(i,1);
		m_pGridLog->SetText(strSeq);

		m_pGridLog->SetCol(3);
		strTrial = m_DM_LOG.GetCellData(i,2);
		m_pGridLog->SetText(strTrial);
	

		m_pGridLog->SetCol(4);
		strType = m_DM_LOG.GetCellData(i,3);

		if(_T("T") == strType)
		{
			m_pGridLog->SetText(_T("SELECT"));
		}
		else if(_T("U") == strType)
		{
			m_pGridLog->SetText(_T("UPDATE"));
		}
		else if(_T("I") == strType)
		{
			m_pGridLog->SetText(_T("INSERT"));
		}
		else if(_T("F") == strType)
		{
			m_pGridLog->SetText(_T("FIRE WALL"));
		}


		m_pGridLog->SetCol(5);
		strDate = m_DM_LOG.GetCellData(i,4);
		m_pGridLog->SetText(strDate);


		m_pGridLog->SetCol(6);
		strTime = m_DM_LOG.GetCellData(i,5);
		m_pGridLog->SetText(strTime);

		
		m_pGridLog->SetCol(7);
		strResult = m_DM_LOG.GetCellData(i,6);
		if(_T("0") == strResult)
		{
			m_pGridLog->SetText(_T("CONNECTION FAIL"));
		}
		else if(_T("1") == strResult)
		{
			m_pGridLog->SetText(_T("SQL FAIL"));
		}
		else if(_T("2") == strResult)
		{
			m_pGridLog->SetText(_T("LOB FAIL"));
		}
		else if(_T("3") == strResult)
		{
			m_pGridLog->SetText(_T("SUCCESS"));
		}
		else if(_T("4") == strResult)
		{
			m_pGridLog->SetText(_T("WAIT"));
		}
	}	


}

VOID CRptUpdateLogViewer::OnbtnLOGSEARCH() 
{
	// TODO: Add your control notification handler code here
	
	//검색 조건을 만든다 
	//RPT_UPDATER_LOG_TBL R,PATCH_LIBRARY_TBL P

	UpdateData(TRUE);

	CString strWhere;
	CString strTmp;

	if(!m_strSeqNo.IsEmpty())
	{
		strTmp.Format(_T(" AND R.SEQ_NUM = %s"),m_strSeqNo);
		strWhere += strTmp;
	}

	if(!m_strLibName.IsEmpty())
	{
		strTmp.Format(_T(" AND P.LIBRARY_NAME LIKE '%%%s%%'"),m_strLibName);
		strWhere += strTmp;
	}
	
	if(m_strDate1.IsEmpty() && !m_strDate2.IsEmpty())
	{
		AfxMessageBox(_T("실행일 부분의 뒷부분만 입력되었습니다!"));
		return;
	}
	
	BOOL bExistDate2 = FALSE;
	if(!m_strDate2.IsEmpty()) bExistDate2 = TRUE;


	if(!m_strDate1.IsEmpty())
	{
		if(bExistDate2) //양쪽 범위 
		{
			strTmp.Format(_T(" AND R.DATE BETWEEN '%s' AND '%s'"),m_strDate1,m_strDate2);
		}
		else //일치 검색
		{
			strTmp.Format(_T(" AND R.DATE = '%s'"),m_strDate1);	
		}
	
		strWhere += strTmp;
	}


	if(ShowLog(strWhere,FALSE)<0) 
	{
		AfxMessageBox(_T("검색 조건에 문제가 있습니다!"));
		return;
	}
}
