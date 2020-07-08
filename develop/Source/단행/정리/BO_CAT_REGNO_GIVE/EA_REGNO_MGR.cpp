// EA_REGNO_MGR.cpp : implementation file
//

#include "stdafx.h"
#include "EA_REGNO_MGR.h"
#include "BO_CAT_REGNO_0305.h"
#include "BO_CAT_REGNO_0310.h"
#include "..\BO_CAT_LAST_NUMBER\BO_CAT_LAST_NUMBER_0000.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEA_REGNO_MGR dialog


CEA_REGNO_MGR::CEA_REGNO_MGR(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CEA_REGNO_MGR::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEA_REGNO_MGR)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_strMainCMAlias = _T("");
	m_strMainDMAlias = _T("");
	m_strMainGridAlias = _T("");
	m_IsProcCheckIn = FALSE;
	m_strRegCode = _T("");
	m_LastRegNo = -1;
	m_idx = -1;
}
CEA_REGNO_MGR::~CEA_REGNO_MGR()
{
	
}


VOID CEA_REGNO_MGR::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEA_REGNO_MGR)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CEA_REGNO_MGR, CDialog)
	//{{AFX_MSG_MAP(CEA_REGNO_MGR)
	ON_BN_CLICKED(IDC_btnEAREG_PREV, OnbtnEAREGPREV)
	ON_BN_CLICKED(IDC_btnEAREG_NEXT, OnbtnEAREGNEXT)
	ON_BN_CLICKED(IDC_btnEAREG_SAVE, OnbtnEAREGSAVE)
	ON_BN_CLICKED(IDC_btnEAREG_CANCEL, OnbtnEAREGCANCEL)
	ON_BN_CLICKED(IDC_btnEAREG_MISS_NO, OnbtnEAREGMISSNO)
	ON_BN_CLICKED(IDC_btnEAREG_LASTNUM, OnbtnEAREGLASTNUM)
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEA_REGNO_MGR message handlers

BOOL CEA_REGNO_MGR::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UP_낱권등록")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	
	Display(0);
	idxList.RemoveAll();
	m_codeChangeMgr.InitGirdColumnHeading(this, m_strMainCMAlias, m_strMainGridAlias);

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

VOID CEA_REGNO_MGR::Display(INT ids)
{
EFS_BEGIN

	INT idx;
	switch(ids)
	{
	case 0:
		InitFirstLastIndex();
		SelectMakeList(m_strMainCMAlias, m_strMainGridAlias);
		idx = SelectGetHeadPosition(m_strMainCMAlias, m_strMainGridAlias);
		ShowNextPrevButton(idx);
		if (idx < 0)
		{
			AfxMessageBox(_T("선정된 자료가 없습니다."));	// 에러
			EndDialog(IDCANCEL);
			return;
		}
		m_idx = idx; 
		break;
	case -1:
		idx = SelectGetPrev(m_strMainCMAlias, m_strMainGridAlias);
		ShowNextPrevButton(idx);
		if (idx >= 0)
			m_idx = idx; 
		break;
	case 1:
		idx = SelectGetNext(m_strMainCMAlias, m_strMainGridAlias);
		ShowNextPrevButton(idx);
		if(idx >= 0)
			m_idx = idx; 
		break;
	}
		
	AllControlDisplay(_T("CM_낱권등록"), m_idx);
	ShowRegCodeCmbCtrl();
	ShowRegNoEdtCtrl();
	SetReverse(m_strMainCMAlias, m_strMainGridAlias, m_idx);

EFS_END
}

VOID CEA_REGNO_MGR::SetMainCMAlias(CString CMAlias)
{
EFS_BEGIN

	m_strMainCMAlias = CMAlias;

EFS_END
}

VOID CEA_REGNO_MGR::SetMainDMAlias(CString DMAlias)
{
EFS_BEGIN

	m_strMainDMAlias = DMAlias;

EFS_END
}

VOID CEA_REGNO_MGR::SetMainGridAlias(CString GridAlias)
{
EFS_BEGIN

	m_strMainGridAlias = GridAlias;

EFS_END
}

VOID CEA_REGNO_MGR::OnbtnEAREGPREV() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	Display(-1);	

EFS_END
}

VOID CEA_REGNO_MGR::OnbtnEAREGNEXT() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	Display(1);	

EFS_END
}

VOID CEA_REGNO_MGR::OnbtnEAREGSAVE() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	
	CRegNo_Creator impl(this);
	impl.InitTempData();
	impl.SetTempData(_T("DMAlias"), m_strMainDMAlias.GetBuffer(0));
	impl.SetTempData(_T("CMAlias"), _T("CM_낱권등록"));
	impl.SetTempData(_T("DMIndex"), m_idx);
	impl.SetTempData(_T("LastRegNo"), m_LastRegNo);
	//OperationCode 1000:낱권등록검수 -1000:낱권등록검수취소, 2000:낱권등록 , -2000:낱권등록취소
	if(m_IsProcCheckIn)
		impl.SetTempData(_T("OperationCode"), 1000);
	else
		impl.SetTempData(_T("OperationCode"), 2000);
	INT ids = impl.EARegProcessing();
	if(ids < 0)
	{
		CString errstr;
		switch(ids)
		{
		case -1000:
			errstr = _T("입력한 등록번호 자리수가 올바르지 않습니다.");
			break;
		case -1001:
			errstr = _T("입력한 등록번호 형식이 올바르지 않습니다.");
			break;
		case -2000:
			errstr = _T("중복된 등록번호 입니다.");
			break;
		case -3000:
			errstr = _T("책정보가 올바르지 않습니다.");
			break;
		case -3001:
			errstr = _T("수서정보가 올바르지 않습니다.");
			break;
		case -3002:
			errstr = _T("종정보가 올바르지 않습니다.");
			break;
		case -4000:
			errstr = _T("등록번호를 부여할 수 없습니다.");
			break;
		case -10000:
			errstr = _T("마지막 등록번호보다 큰 값입니다.");
			break;
		}
		AfxMessageBox(errstr.GetBuffer(0));
		return;
	}
	CString tmpstr;
	tmpstr.Format(_T("%d"), m_idx);
	idxList.AddTail(tmpstr);
	//this->ShowRegNoEdtCtrl();
	AfxMessageBox(_T("낱권등록 처리되었습니다."));
	//그리드갱신
	if(!m_IsProcCheckIn)
	{
		CStringArray celllist;
		celllist.Add(_T("등록번호"));
		m_codeChangeMgr.SetGridCellData(this, m_strMainCMAlias, m_strMainGridAlias, m_strMainDMAlias, m_idx, FALSE, &celllist);
	}
	Display(1);

EFS_END
}

VOID CEA_REGNO_MGR::OnbtnEAREGCANCEL() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CRegNo_Creator impl(this);
	impl.InitTempData();
	impl.SetTempData(_T("DMAlias"), m_strMainDMAlias.GetBuffer(0));
	impl.SetTempData(_T("CMAlias"), _T("CM_낱권등록"));
	impl.SetTempData(_T("DMIndex"), m_idx);
	impl.SetTempData(_T("LastRegNo"), m_LastRegNo);
	//OperationCode 1000:낱권등록검수 -1000:낱권등록검수취소, 2000:낱권등록 , -2000:낱권등록취소
	if(m_IsProcCheckIn)
		impl.SetTempData(_T("OperationCode"), -1000);
	else
		impl.SetTempData(_T("OperationCode"), -2000);
	INT ids = impl.EARegProcessing();
	if(ids < 0)
	{
		CString errstr;
		switch(ids)
		{
		case -1000:
			errstr = _T("입력한 등록번호 자리수가 올바르지 않습니다.");
			break;
		case -1001:
			errstr = _T("입력한 등록번호 형식이 올바르지 않습니다.");
			break;
		case -2000:
			errstr = _T("중복된 등록번호 입니다.");
			break;
		case -3000:
			errstr = _T("책정보가 올바르지 않습니다.");
			break;
		case -3001:
			errstr = _T("수서정보가 올바르지 않습니다.");
			break;
		case -3002:
			errstr = _T("종정보가 올바르지 않습니다.");
			break;
		case -4000:
			errstr = _T("등록번호를 제거할 수 없습니다.");
			break;
		}
		AfxMessageBox(errstr.GetBuffer(0));
		return;
	}
	CString tmpstr, tmpstr1;
	tmpstr.Format(_T("%d"), m_idx);
	POSITION pos, pos1;
	pos = idxList.GetHeadPosition();
	while(pos != NULL)
	{
		pos1 = pos;
		tmpstr1 = idxList.GetNext(pos);
		if(tmpstr == tmpstr1)
		{
			idxList.RemoveAt(pos1);
			break;
		}
	}
	this->ShowRegNoEdtCtrl();
	AfxMessageBox(_T("낱권등록 취소 되었습니다."));
	if(!m_IsProcCheckIn)
	{
		CStringArray celllist;
		celllist.Add(_T("등록번호"));
		m_codeChangeMgr.SetGridCellData(this, m_strMainCMAlias, m_strMainGridAlias, m_strMainDMAlias, m_idx, FALSE, &celllist);
	}	

EFS_END
}

VOID CEA_REGNO_MGR::OnbtnEAREGMISSNO() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CString sTmp;
	CESL_DataMgr DM_SUB;
	CString sWhereGlo, sBookKey;
	CESL_DataMgr * pDM = FindDM(m_strMainDMAlias);
	pDM->GetCellData(_T("BB_책KEY"), m_idx, sBookKey);
	sWhereGlo.Format( _T(" REC_KEY IN (%s) GROUP BY REG_CODE"), sBookKey );
	CBO_CAT_REGNO_0310 dlg(this,sWhereGlo);
	dlg.SetOpenMode(1000);
	//dlg.DoModal();
	if( IDOK == dlg.DoModal() ) {
		//dlg.pDM_0310_DROP
//		iDropCh = 1;

		//그리드에 넣기
		CESL_DataMgr *pDM_0310_DROP;
		pDM_0310_DROP = FindDM(_T("DM_BO_CAT_REG_311_DROP"));
		
		INT iRowCount = pDM_0310_DROP->GetRowCount();
		if ( iRowCount < 1 ) {
			return;
		}
		CESL_ControlMgr * pCM = FindCM(_T("CM_낱권등록"));
		CString tmpstr;
		pDM_0310_DROP->GetCellData(0, 0, tmpstr);
		pCM->SetControlMgrData(_T("등록번호"), tmpstr);
	}

EFS_END
}

VOID CEA_REGNO_MGR::OnCancel() 
{
EFS_BEGIN

	// TODO: Add extra cleanup here
	CESL_DataMgr * pDM = FindDM(m_strMainDMAlias);
	CESL_ControlMgr * pCM = FindCM(m_strMainCMAlias);
	if(m_IsProcCheckIn)
	{
		INT cnt = idxList.GetCount();
		CString tmpstr;
		POSITION pos = idxList.GetTailPosition();
		for(INT i=0 ; i<cnt && pos != NULL; i++)
		{
			tmpstr = idxList.GetPrev(pos);
			pDM->DeleteRow(_ttoi(tmpstr.GetBuffer(0)));
		}
	}
	pCM->ControlDisplay(m_strMainGridAlias);
	
	CDialog::OnCancel();

EFS_END
}

VOID CEA_REGNO_MGR::OnbtnEAREGLASTNUM() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	BO_CAT_LAST_NUMBER_0000 dlg(0,this);
	dlg.DoModal();

EFS_END
}

VOID CEA_REGNO_MGR::ShowRegNoEdtCtrl()
{
EFS_BEGIN

	CEdit * pEdt = (CEdit*)GetDlgItem(IDC_edtEAREG_REGNO);
	CButton * pCancelBtn = (CButton*)GetDlgItem(IDC_btnEAREG_CANCEL);
	CButton * pOkBtn = (CButton*)GetDlgItem(IDC_btnEAREG_SAVE);

	pEdt->EnableWindow(TRUE);
	pCancelBtn->EnableWindow(TRUE);
	pOkBtn->EnableWindow(TRUE);
	GetDlgItem(IDC_btnEAREG_MISS_NO)->EnableWindow(TRUE);

	CString tmpstr;
	CESL_DataMgr * pDM = FindDM(m_strMainDMAlias);
	pDM->GetCellData(_T("BB_등록번호"), m_idx, tmpstr);
	if(tmpstr.IsEmpty())
	{
		//=====================================================
		//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 		tmpstr.Format(_T("%s%010d"), m_strRegCode, m_LastRegNo+1);
		//nRegCodeLength = m_pParent->m_pInfo->nREG_CODE_LENGTH;]
		CString strTempRegNum;
		strTempRegNum.Format(_T("%d"), m_LastRegNo+1);
		tmpstr = m_pInfo->MakeRegNo(m_strRegCode, strTempRegNum);
		//=====================================================
		
		SetControlData(_T("CM_낱권등록"), _T("등록번호"), tmpstr);
		pCancelBtn->EnableWindow(FALSE);
	}
	else
	{
		SetControlData(_T("CM_낱권등록"), _T("등록번호"), tmpstr);
		pEdt->EnableWindow(FALSE);
		pOkBtn->EnableWindow(FALSE);
		GetDlgItem(IDC_btnEAREG_MISS_NO)->EnableWindow(FALSE);
	}

EFS_END
}

VOID CEA_REGNO_MGR::SetIsProcCheckIn(BOOL procCheckIn)
{
EFS_BEGIN

	m_IsProcCheckIn = procCheckIn;

EFS_END
}

VOID CEA_REGNO_MGR::ShowNextPrevButton(INT idx)
{
EFS_BEGIN

	GetDlgItem(IDC_btnEAREG_PREV)->EnableWindow(TRUE);
	GetDlgItem(IDC_btnEAREG_NEXT)->EnableWindow(TRUE);
	if(idx == m_firstidx || idx < 0)
		GetDlgItem(IDC_btnEAREG_PREV)->EnableWindow(FALSE);
	if(idx == m_lastidx || idx < 0)
		GetDlgItem(IDC_btnEAREG_NEXT)->EnableWindow(FALSE);

EFS_END
}

VOID CEA_REGNO_MGR::InitFirstLastIndex()
{
EFS_BEGIN

	INT ids = SelectMakeList(m_strMainCMAlias, m_strMainGridAlias);
	if(ids < 0)
	{
		AfxMessageBox(_T("선정된 자료가 없습니다."));
		return;
	}
	m_firstidx = SelectGetHeadPosition(m_strMainCMAlias, m_strMainGridAlias);
	m_lastidx = SelectGetTailPosition(m_strMainCMAlias, m_strMainGridAlias);

EFS_END
}

VOID CEA_REGNO_MGR::ShowRegCodeCmbCtrl()
{
EFS_BEGIN

	CESL_DataMgr * pDM = FindDM(m_strMainDMAlias);
	pDM->GetCellData(_T("BB_등록구분"), m_idx, m_strRegCode);
	CComboBox * pCmb = (CComboBox*)GetDlgItem(IDC_cmbEAREG_REGCODE);
	CRegNo_Creator impl(this);
	impl.InitTempData();
	impl.SetTempData(_T("RegCode"), m_strRegCode.GetBuffer(0));
	impl.SetTempData(_T("CMAlias"), _T("CM_낱권등록"));
	impl.SetTempData(_T("DMAlias"), m_strMainDMAlias.GetBuffer(0));
	if(m_strRegCode.IsEmpty())
	{
		pCmb->EnableWindow(TRUE);
		impl.GetLastRegNo();
	}
	else
	{
		pCmb->EnableWindow(FALSE);
		impl.GetLastRegNo();
	}
	impl.GetTempData(_T("LastRegNo"), m_LastRegNo);

EFS_END
}


HBRUSH CEA_REGNO_MGR::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}