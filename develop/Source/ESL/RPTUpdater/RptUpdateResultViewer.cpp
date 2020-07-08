// RptUpdateResultViewer.cpp : implementation file
//

#include "stdafx.h"
#include "RPTUpdater.h"
#include "RptUpdateResultViewer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRptUpdateResultViewer dialog


CRptUpdateResultViewer::CRptUpdateResultViewer(CWnd* pParent /*=NULL*/)
	: CDialog(CRptUpdateResultViewer::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRptUpdateResultViewer)
	m_strTrialCnt = _T("");
	m_strTrialDate = _T("");
	m_strSeqNoDate = _T("");
	//}}AFX_DATA_INIT

	m_strSeqNo = _T("");
	m_strCurTrialCnt = _T("");
	m_strCurLibPKList = _T("");
	m_strCurFailLibPKList = _T("");
	m_strCurRptAliasList = _T("");
}


VOID CRptUpdateResultViewer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRptUpdateResultViewer)
	DDX_Control(pDX, IDC_cmbSeqNo, m_cmbSeqNo);
	DDX_Control(pDX, IDC_listRpt, m_listRpt);
	DDX_Control(pDX, IDC_listLib, m_listLib);
	DDX_Text(pDX, IDC_edtTrialCnt, m_strTrialCnt);
	DDX_Text(pDX, IDC_edtTrialDate, m_strTrialDate);
	DDX_Text(pDX, IDC_edtSeqNoDate, m_strSeqNoDate);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRptUpdateResultViewer, CDialog)
	//{{AFX_MSG_MAP(CRptUpdateResultViewer)
	ON_CBN_SELCHANGE(IDC_cmbSeqNo, OnSelchangecmbSeqNo)
	ON_BN_CLICKED(IDC_btnSELECT, OnbtnSELECT)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRptUpdateResultViewer message handlers

BOOL CRptUpdateResultViewer::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if(ReadCfg()<0)
	{
		AfxMessageBox(_T("CFG 정보를 읽을 수 없습니다!"));
		EndDialog(0);
		return FALSE;
	}

	if(BringSeqInfo()<0)
	{
		AfxMessageBox(_T("차수 정보를 가져올 수 없습니다!"));
		EndDialog(0);
		return FALSE;
	}

	if(ShowData(m_strSeqNo)<0)
	{
		AfxMessageBox(_T("LOG 정보를 보는 데 실패 했습니다!"));
		EndDialog(0);
		return FALSE;
	}
	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CRptUpdateResultViewer::ReadCfg()
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

	m_DM_SEQ.SetCONNECTION_INFO(strConnectInfo);
	m_DM_LOG.SetCONNECTION_INFO(strConnectInfo);
	m_DM_LIB.SetCONNECTION_INFO(strConnectInfo);

	return 0;
}

INT CRptUpdateResultViewer::BringSeqInfo()
{
	//차수정보 가져오기 
	CString strQuery;
	strQuery.Format(_T("SELECT SEQ_NUM,TRIAL_CNT,LIB_KEY,RPT_ALIAS,CREATE_DATE,LAST_TRIAL_DATE,")
					_T("CREATE_TIME,LAST_TRIAL_TIME ")
					_T("FROM RPT_UPDATER_SEQ_INFO_TBL ORDER BY SEQ_NUM DESC"));
	
	m_DM_SEQ.RestructDataManager(strQuery);
	
	INT nRow = m_DM_SEQ.GetRowCount();
	if(0 == nRow) 
	{
		AfxMessageBox(_T("차수 정보가 없습니다"));
		return -1;
	}

	//차수정보를 콤보에 넣는다
	CString strSeqNo;

	for(INT i=0;i<nRow;i++)
	{
		strSeqNo = m_DM_SEQ.GetCellData(i,0);
		m_cmbSeqNo.InsertString(i,strSeqNo);
	}

	

	return 0;
}

INT CRptUpdateResultViewer::ShowData(CString strSeqNo)
{
	m_strCurLibPKList = _T("");
	m_strCurFailLibPKList = _T("");
	m_strCurRptAliasList = _T("");
	

	
	INT nSeqRow = m_DM_SEQ.GetRowCount();
	
	INT nSeqIdx = -1;
	CString strTmp;
	
	CString strCurSeqNo;

	if(strSeqNo.IsEmpty()) 
	{
		nSeqIdx = 0;
		strCurSeqNo = m_DM_SEQ.GetCellData(0,0);

	}
	else
	{
		for(INT i=0;i<nSeqRow;i++)
		{
			strTmp = m_DM_SEQ.GetCellData(i,0);

			if(strSeqNo == strTmp)
			{
				nSeqIdx = i;
			}
		}

		strCurSeqNo = strSeqNo;
	}

	m_cmbSeqNo.SetCurSel(nSeqIdx);
	m_strTrialCnt = m_DM_SEQ.GetCellData(nSeqIdx,1);
	m_strCurTrialCnt = m_strTrialCnt;

	CString strDate,strTime;
	strDate = m_DM_SEQ.GetCellData(nSeqIdx,4);
	strTime = m_DM_SEQ.GetCellData(nSeqIdx,6);
	m_strSeqNoDate = strDate + _T(" ") + strTime;
	
	strDate = m_DM_SEQ.GetCellData(nSeqIdx,5);
	strTime = m_DM_SEQ.GetCellData(nSeqIdx,7);
	m_strTrialDate = strDate + _T(" ") + strTime;

		
	//alias정보를 list에 뿌려준다
	CString strAliasList;
	strAliasList = m_DM_SEQ.GetCellData(nSeqIdx,3);
	m_strCurRptAliasList = strAliasList;
	m_listRpt.ResetContent();
	
	INT ids = -1;
	INT nRptListIdx = 0;
	CString strData;
	
	m_listRpt.ResetContent();

	while(1)
	{
		ids = GetParseString(_T(","),strAliasList,strData);
		if(ids<0) break;
		m_listRpt.InsertString(nRptListIdx,strData);
		nRptListIdx++;
	}
	

	//도서관정보를 가져온다 
	CString strLibKeyList;
	strLibKeyList = m_DM_SEQ.GetCellData(nSeqIdx,2);
	m_strCurLibPKList = strLibKeyList;
	CString strQuery;
	strQuery.Format(_T("SELECT PK,LIBRARY_NAME FROM PATCH_LIBRARY_TBL WHERE PK IN (%s)"),strLibKeyList);
	m_DM_LIB.RestructDataManager(strQuery);
	
	INT nLibRow = m_DM_LIB.GetRowCount();
	if(0 == nLibRow) 
	{
		AfxMessageBox(_T("도서관 정보가 없습니다"));
		return -1;
	}
	
	//로그 정보를 가지고 온다
	strQuery.Format(_T("SELECT DISTINCT(LIB_KEY) FROM RPT_UPDATER_LOG_TBL ")
					_T("WHERE SEQ_NUM = %s AND TRIAL_CNT = %s AND RESULT != '3'")
					,strCurSeqNo,m_strTrialCnt);
	
	m_DM_LOG.RestructDataManager(strQuery);
	INT nLogRow = 0;
	nLogRow = m_DM_LOG.GetRowCount();

	
	//도서관 정보와 로그정보를 비교해 데이터를 뿌린다
	CString strPK;
	CString strLibName;
	CString strCompare;
	BOOL bFail = FALSE;
	INT nLibListIdx = 0;
	INT nFailCnt = 0;
	INT nAddCnt = 0;
	

	m_listLib.ResetContent();


	for(INT i=0;i<nLibRow;i++)
	{
		bFail = FALSE;
		strPK = m_DM_LIB.GetCellData(i,0);
		strLibName = m_DM_LIB.GetCellData(i,1);

		for(INT j=0;j<nLogRow;j++)
		{
			strCompare = m_DM_LOG.GetCellData(j,0);
			if(strCompare == strPK)
			{
				bFail = TRUE;
				break;
			}
		}
		
		if(bFail)
		{
			strData.Format(_T("▶▶▶%s"),strLibName);
			
			if(0 == nAddCnt)
			{
				m_strCurFailLibPKList = strPK;
			}
			else
			{
				m_strCurFailLibPKList += _T(",") + strPK;
			}
			
			nAddCnt++;
			nFailCnt++;
		}
		else 
		{
			strData.Format(_T("%s"),strLibName);
		}
	

		m_listLib.InsertString(nLibListIdx,strData);
		nLibListIdx++;
	}
	
	
	CString msg;
		
	msg.Format(_T("[레포트 수  : %d ]"),nRptListIdx);
	GetDlgItem(IDC_stcRptSum)->SetWindowText(msg);
	
	msg.Format(_T("[도서관수 : %d][실패 : %d]"),nLibListIdx,nFailCnt); 
	GetDlgItem(IDC_stcLibSum)->SetWindowText(msg);
	
	UpdateData(FALSE);

	return 0;
}

INT CRptUpdateResultViewer::MakeUpdateResult()
{


	return 0;
}

INT CRptUpdateResultViewer::GetParseString(CString strDelim,CString& strList,CString &strOutput)
{
	strList.TrimLeft();
	strList.TrimRight();

	INT nPos = strList.Find(strDelim);

	if(nPos<0)
	{
		if(strList.IsEmpty()) 
		{
			return -1;
		}
		else
		{
			strOutput = strList;
			strList.Empty();
		}
	}
	else
	{
		
		INT lastidx = strList.GetLength();
		INT startidx = nPos + 1;
		
		strOutput = strList.Left(nPos);
		strOutput.TrimLeft();
		strOutput.TrimRight();
		if(strOutput.IsEmpty()) return -1;

		strList = strList.Right(lastidx - startidx);
	}

	return 0;
}

VOID CRptUpdateResultViewer::OnSelchangecmbSeqNo() 
{
	// TODO: Add your control notification handler code here
	INT nCurSel = m_cmbSeqNo.GetCurSel();

	CString strSeqNo;
	strSeqNo = m_DM_SEQ.GetCellData(nCurSel,0);
	ShowData(strSeqNo);

}

VOID CRptUpdateResultViewer::OnbtnSELECT() 
{
	// TODO: Add your control notification handler code here
	//기존 차수 가져오기
	
	INT nCurSel = m_cmbSeqNo.GetCurSel();

	CString strSeqNo;
	strSeqNo = m_DM_SEQ.GetCellData(nCurSel,0);
	m_strSeqNo = strSeqNo;
	CString msg;

	if(m_strCurFailLibPKList.IsEmpty())
	{
		msg.Format(_T("[%s] 차수는 실패한 도서관이 없습니다!"),m_strSeqNo);
		AfxMessageBox(msg);
		return;
	}

	if(m_strSeqNo.IsEmpty() || m_strCurLibPKList.IsEmpty() || m_strCurRptAliasList.IsEmpty())
	{
		AfxMessageBox(_T("선택된 차수 정보를 사용할 수 없습니다!"));
		return;
	}

	EndDialog(11);

}
