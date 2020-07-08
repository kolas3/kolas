// ErrorAdjustDlg.cpp: implementation of the CErrorAdjustDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ErrorAdjustDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CErrorAdjustDlg::CErrorAdjustDlg(UINT nIDD, CESL_Mgr * pMgr/*=NULL*/)
	: CESL_Mgr(nIDD, pMgr)
{
	m_pDataProcessingDlg = NULL;
	m_pErrorAdjustApi = NULL;
	m_pResourceMgr = NULL;
	m_pDM = NULL;
	m_pCM = NULL;
	m_pGrid = NULL;
	m_pSearchMgr = NULL;
}

CErrorAdjustDlg::~CErrorAdjustDlg()
{
	if(m_pDataProcessingDlg)
	{
		delete m_pDataProcessingDlg;
		m_pDataProcessingDlg = NULL;
	}
}

INT CErrorAdjustDlg::CMD_VIEWERRORINFO()
{
	return 0;
}

INT CErrorAdjustDlg::CMD_REFRESH()
{
	return 0;
}

INT CErrorAdjustDlg::CMD_ERRORADJUST()
{
	return 0;
}

INT CErrorAdjustDlg::CMD_SEARCH()
{
	return 0;
}

INT CErrorAdjustDlg::CMD_ERRORADJUST1()
{
	return 0;
}

VOID CErrorAdjustDlg::ShowDataProcessingDlg()
{
	if(m_pDataProcessingDlg == NULL) m_pDataProcessingDlg = new CDataProcessingDlg(this);
	
	if(!m_pDataProcessingDlg->GetCreate())
	{
		if(!m_pDataProcessingDlg->Create(this))
		{
			delete m_pDataProcessingDlg;
			m_pDataProcessingDlg = NULL;
			return;
		}
	}
	m_pDataProcessingDlg->CenterWindow();
	m_pDataProcessingDlg->ShowWindow(SW_SHOW);
	m_pDataProcessingDlg->UpdateWindow();
	//프로그래스 컨트롤 초기화
	m_pDataProcessingDlg->ShowHideCancelButton(FALSE);
	//	m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, 0);
	m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
}

VOID CErrorAdjustDlg::DeleteDataProcessingDlg()
{
	if(m_pDataProcessingDlg)
	{
		m_pDataProcessingDlg->ShowWindow(SW_HIDE);
		delete m_pDataProcessingDlg;
		m_pDataProcessingDlg = NULL;
	}
}

INT CErrorAdjustDlg::CheckSelectDataValid(CESL_DataMgr * pDM)
{
	if(pDM->GetRowCount() == 0) return -1;
	
	if(!m_pGrid) return -1;
	INT ids = m_pGrid->SelectMakeList();
	if(ids < 0) return -1;
	ids = m_pGrid->SelectGetHeadPosition();
	if(ids < 0) return -1;
	return 0;
}