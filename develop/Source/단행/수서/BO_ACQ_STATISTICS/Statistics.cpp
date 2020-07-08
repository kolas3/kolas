// Statistics.cpp: implementation of the CStatistics class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Statistics.h"
#include "StatisticsConditionDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// 생성자
CStatistics::CStatistics(UINT IDD, CESL_Mgr * pParentMgr):CESL_Mgr(IDD, pParentMgr)
{
	m_nDialogID = IDD;
	m_pCondition = NULL;
	m_pConditionDlg = NULL;
	m_pPrintMgr = NULL;
}
// 소멸자
CStatistics::~CStatistics()
{
	if(m_pCondition)
	{
		delete m_pCondition;
		m_pCondition = NULL;
	}
	if(m_pConditionDlg)
	{
		delete m_pConditionDlg;
		m_pConditionDlg = NULL;
	}
	if(m_pPrintMgr)
	{
		delete m_pPrintMgr;
		m_pPrintMgr = NULL;
	}
}

BOOL CStatistics::Create(CWnd * pWnd)
{
	return CDialog::Create(m_nDialogID, pWnd);
}

VOID CStatistics::ShowConditionDlg()
{
	if(!m_pConditionDlg->GetCreate()) m_pConditionDlg->Create(this);
	m_pConditionDlg->ShowWindow(SW_SHOW);
	UpdateWindow();
	CenterWindow(this);
}

//////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 통계물 출력버튼
// [ 미니스펙 ]
//////////////////////////////////////////////////////////////////////
VOID CStatistics::PrintStatistics()
{
	if( m_pDM->GetRowCount() == 0 )
	{
		ESLAfxMessageBox(_T("출력할 통계정보가 없습니다."));
		return;
	}
	CString strClassNoType, strSIRfile;

	strClassNoType = m_pCondition->GetClassNoType();
	
	m_pPrintMgr->InitPrintData();
	m_pPrintMgr->AddPrintData(_T("제목"), m_pCondition->GetStatType());
	m_pPrintMgr->AddPrintData(_T("자료구분"), m_pCondition->GetVerticalDesc());
	m_pPrintMgr->AddPrintData(_T("조건"), m_pCondition->GetCondition());
	m_pPrintMgr->AddPrintData(_T("분류표구분"), strClassNoType);
	m_pPrintMgr->SetPrintDM(m_pDM);
	
	if		( strClassNoType.Left(3) == _T("KDC") )	strSIRfile = _T("K2UP_통계자료_KDC");
	else if	( strClassNoType.Left(3) == _T("DDC") )	strSIRfile = _T("K2UP_통계자료_DDC");
	else if ( strClassNoType.Left(2) == _T("LC")  )	strSIRfile = _T("K2UP_통계자료_LC");
	else											strSIRfile = _T("K2UP_통계자료");
	m_pPrintMgr->SetSIReportFilename(strSIRfile);
	m_pPrintMgr->Print();
}

VOID CStatistics::DisplayGrid()
{
	m_pGrid->Display();
}

INT CStatistics::SetStatisticsInfo()
{
	if(m_pCondition->GetAcqType() == PURCHASE) m_pCM->SetControlMgrData(_T("수입년도"), _T("구입"));
	else if(m_pCondition->GetAcqType() == DONATE) m_pCM->SetControlMgrData(_T("수입년도"), _T("기증"));
	
	m_pCM->SetControlMgrData(_T("통계물"), m_pCondition->GetStatType());
	m_pCM->SetControlMgrData(_T("조건"), m_pCondition->GetCondition());
	return 0;
}

VOID CStatistics::CreateConditionDlg(ACQ_TYPE nAcqType)
{
	if( m_pConditionDlg )
	{
		delete m_pConditionDlg;
		m_pConditionDlg = NULL;
	}

	if( m_pPrintMgr )
	{
		delete m_pPrintMgr;
		m_pPrintMgr = NULL;
	}

	m_pConditionDlg = new CStatisticsConditionDlg(this);
	m_pPrintMgr = new CBasicPrintMgr(this);
	CreateCondition();
	m_pConditionDlg->SetAcqType(nAcqType);
	m_pConditionDlg->SetCondition(m_pCondition);
	m_pConditionDlg->SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
}

//////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 그리드 헤더를 설정한다.
//
// [ 미니스펙 ]
// 1. 그리드 헤더를 숫자를 기본값으로 설정한다.
// 2. 임시DM에 접속정보를 설정한다.
// 3. 조건설정 클래스에 저장되어 있는 _T("자료구분") 선택 값을 가져온다. (KDC, DDC, LC)
// 4. 저장된 자료구분값에 대한 코드값들의 설명값을 가져와 DM을 구성하고 그리드 헤더에 설정한다.
//
//////////////////////////////////////////////////////////////////////
VOID CStatistics::ChangeColumnHeaderOfSubjectCode()
{
	//================================================================
	// 변수정의
	//================================================================
	INT i, idx, nCodeCnt;	
	CString codeClassName, strQuery, strResult;
	CESL_DataMgr tmpDM;

	//================================================================
	// 1. 그리드 헤더를 숫자를 기본값으로 설정한다.
	//================================================================
	for(i=0; i<m_pGrid->GetColCount()-4; i++)
	{
		strResult.Format(_T("%d"), i);
		m_pGrid->SetTextMatrix(0, i+3, strResult);
	}

	//================================================================
	// 2. 임시DM에 접속정보를 설정한다.
	//================================================================
	tmpDM.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);

	//================================================================
	// 3. 조건설정 클래스에 저장되어 있는 _T("자료구분") 선택 값을 가져온다. (KDC, DDC, LC)
	//================================================================
	codeClassName = m_pCondition->GetClassNoType();

	if( codeClassName.Left(2) != _T("LC") && m_pGrid->GetColCount() > 14 )
	{
		for( i=12; i<24; i++ )
		{
			m_pGrid->SetColWidth(i,0,0);
		}
	}
	

	//================================================================
	// 4. 저장된 자료구분값에 대한 코드값들의 설명값을 가져와 DM을 구성하고 그리드 헤더에 설정한다.
	//================================================================
	strQuery.Format(_T("SELECT CLASS_CODE FROM CD_CLASS_CODE_TBL WHERE CLASS_NAME = '%s'"), codeClassName);
	tmpDM.GetOneValueQuery(strQuery, strResult);
	
	if( strResult.IsEmpty() == FALSE )
	{
		strQuery.Format(_T("SELECT DESCRIPTION, CODE FROM CD_CODE_TBL WHERE CLASS = '%s' ORDER BY CODE"), strResult);
		tmpDM.RestructDataManager(strQuery);

		idx = 0;
		nCodeCnt = tmpDM.GetRowCount();
		for( i=0; i<nCodeCnt; i++ )
		{
			strResult.Empty();
			strResult = tmpDM.GetCellData(i,0);
			if( strResult.IsEmpty() == TRUE || strResult == _T("적용안함") )	continue;
			m_pGrid->SetTextMatrix(0, idx+3, strResult );
			idx++;
		}
	}
}

VOID CStatistics::CreateCondition()
{
	if(m_pCondition)
	{
		delete m_pCondition;
		m_pCondition = NULL;
	}
	m_pCondition = new CStatisticsCondition(this);
}

VOID CStatistics::InitPopupMenuItem()
{
	CArray <CString, CString> RemoveListArray;
	RemoveListArray.RemoveAll();
	m_pGrid->InitPopupMenuItem();
	m_pGrid->AddPopupMenuItem((CView*)pMain, VIEW_STYLE, &RemoveListArray);	
}