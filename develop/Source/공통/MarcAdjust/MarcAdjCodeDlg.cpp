// MarcAdjCodeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MarcAdjCodeDlg.h"
#include "MarcAdjFrame.h"

#include "CatESL_Mgr.h"

#include "..\COMMON_DONATOR_MANAGER\DonatorNameGetterDlg.h"
#include "..\COMMON_DONATOR_MANAGER\DonatorNameGetter_Search.h"


#include "MarcAdjResourceMgr.h"

#include "..\..\ESL\Marc\MarcToolKit\MarcDefaultValueConfigDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMarcAdjCodeDlg dialog


CMarcAdjCodeDlg::CMarcAdjCodeDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMarcAdjCodeDlg)
	m_strRegCodeField = _T("");
	m_bFixedFieldMaintenance = FALSE;
	//}}AFX_DATA_INIT

	m_strHD_Species = _T("BS");
	m_strHD_Index = _T("IBS");
	m_strHD_Book = _T("BB");

	m_pCM = NULL;

	m_nModeMONO_SERIAL = -1;
}

CMarcAdjCodeDlg::~CMarcAdjCodeDlg()
{
	
}

VOID CMarcAdjCodeDlg::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMarcAdjCodeDlg)
	DDX_Text(pDX, IDC_eREG_CLASS_CODE_MA, m_strRegCodeField);
	DDX_Check(pDX, IDC_chkFixedField, m_bFixedFieldMaintenance);
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CMarcAdjCodeDlg, CDialog)
	//{{AFX_MSG_MAP(CMarcAdjCodeDlg)
	ON_CBN_SELCHANGE(IDC_cACQ_CODE_MA, OnSelchangecACQCODEMA)
	ON_BN_CLICKED(IDC_bDONATOR, OnbDONATOR)
	ON_EN_KILLFOCUS(IDC_eREG_CLASS_CODE_MA, OnKillfocuseREGCLASS)
	ON_CBN_SELCHANGE(IDC_cFORM_CLASS_MA, OnSelchangecFORMCLASSMA)
	ON_CBN_KILLFOCUS(IDC_cFORM_CLASS_MA, OnKillfocuscFORMCLASSMA)
	ON_BN_CLICKED(IDC_chkFixedField, OnchkFiexedField)
	ON_CBN_SELCHANGE(IDC_cMED_CLASS_I, OnSelchangecMEDCLASSI)
	ON_CBN_SELCHANGE(IDC_cCONTROL_MAT_CLASS_ADJ, OnEditchangecCONTROLMATCLASSADJ)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_CONTROL_RANGE(CBN_SELCHANGE, IDC_cACQ_CODE_MA, IDC_cCODE_CAT, ApplyCodeChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMarcAdjCodeDlg Operations

INT CMarcAdjCodeDlg::ApplyCodeChange(BOOL bPostMessage /*= TRUE*/)
{
	EFS_BEGIN

	CCatESL_Mgr *pCatESL_Mgr = static_cast<CCatESL_Mgr*>(pParentMgr);
	if (pCatESL_Mgr == NULL) return -1;

	CCatApi *pCatApi = pCatESL_Mgr->GetCatApi();

	pCatApi->MakeDMFromCM(m_pCM, pCatESL_Mgr->m_pDM_Index, pCatESL_Mgr->GetAppMode(), 0, m_strHD_Index);

	pCatApi->MakeDMFromCM(m_pCM, pCatESL_Mgr->m_pDM_Species, pCatESL_Mgr->GetAppMode(), 0, m_strHD_Species );

	if (m_nModeMONO_SERIAL == MODE_SERIAL)
		pCatApi->MakeDMFromCM(m_pCM, pCatESL_Mgr->m_pDM_Purchase, pCatESL_Mgr->GetAppMode(), 0, _T("SP"));
	else 
		pCatApi->MakeDMFromCM(m_pCM, pCatESL_Mgr->m_pDM_Purchase, pCatESL_Mgr->GetAppMode(), 0, _T("BP"));

	if (bPostMessage)
		GetParentFrame()->PostMessage(WM_CODE_UPDATE, 0, 0);

	return 0;

	EFS_END
	return -1;
}

INT CMarcAdjCodeDlg::ApplyChangeAcqCode()
{
	EFS_BEGIN

	CString strCode;	
	m_pCM->GetControlMgrData(m_strHD_Index + _T("_수입구분_코드"), strCode);

	CCatESL_Mgr *pCatESL_Mgr = static_cast<CCatESL_Mgr*>(pParentMgr);
	
	pCatESL_Mgr->m_pDM_Book->SetCellData(m_strHD_Book + _T("_수입구분_코드"), strCode, 0);

	CWnd *pWndText = GetDlgItem(IDC_tPURCHASE_CODE);
	CWnd *pWndProvider = GetDlgItem(IDC_cPROVIDER_MA);
	CWnd *pWndeDonator = GetDlgItem(IDC_eDONATOR);
	CWnd *pWndbDonator = GetDlgItem(IDC_bDONATOR);

	if (pWndText == NULL || pWndProvider == NULL || pWndeDonator == NULL || pWndbDonator == NULL) return 0; 

	if (strCode.IsEmpty())
	{
		pWndText->ShowWindow(SW_HIDE);
		pWndProvider->ShowWindow(SW_HIDE);
		pWndeDonator->ShowWindow(SW_HIDE);
		pWndbDonator->ShowWindow(SW_HIDE);

		pWndProvider->EnableWindow(FALSE);
		pWndeDonator->EnableWindow(FALSE);
		pWndbDonator->EnableWindow(FALSE);

	}
	else if (strCode == _T("1"))
	{
		pWndText->SetWindowText(_T("구 입 처"));
		pWndText->ShowWindow(SW_SHOW);
		pWndProvider->ShowWindow(SW_SHOW);
		pWndeDonator->ShowWindow(SW_HIDE);
		pWndbDonator->ShowWindow(SW_HIDE);

		pWndProvider->EnableWindow(TRUE);
		pWndeDonator->EnableWindow(FALSE);
		pWndbDonator->EnableWindow(FALSE);
	}
	else if (strCode == _T("2"))
	{
		pWndText->SetWindowText(_T("기 증 자"));
		pWndText->ShowWindow(SW_SHOW);
		pWndProvider->ShowWindow(SW_HIDE);
		pWndeDonator->ShowWindow(SW_SHOW);
		pWndbDonator->ShowWindow(SW_SHOW);

		pWndProvider->EnableWindow(FALSE);
		pWndeDonator->EnableWindow(FALSE);
		pWndbDonator->EnableWindow(TRUE);
	}
	else if (strCode == _T("3"))
	{
		pWndText->ShowWindow(SW_HIDE);
		pWndProvider->ShowWindow(SW_HIDE);
		pWndeDonator->ShowWindow(SW_HIDE);
		pWndbDonator->ShowWindow(SW_HIDE);

		pWndProvider->EnableWindow(FALSE);
		pWndeDonator->EnableWindow(FALSE);
		pWndbDonator->EnableWindow(FALSE);
	}

	if  ( static_cast<CCatESL_Mgr*>(pParentMgr)->GetAppMode() == MNG_MODIFY )
	{
		GetDlgItem(IDC_cSHELF_CODE_MA)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_cPROVIDER_MA)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_bDONATOR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_tSHELF_CODE_MA)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_tPURCHASE_CODE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_eDONATOR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_ACQ_CODE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_cACQ_CODE_MA)->ShowWindow(SW_HIDE);
		
	}

	return 0;

	EFS_END
	return -1;
}

INT CMarcAdjCodeDlg::Display()
{
	EFS_BEGIN

	if (static_cast<CCatESL_Mgr*>(pParentMgr)->GetAppMode() == MNG_IMPORT
		|| static_cast<CCatESL_Mgr*>(pParentMgr)->GetAppMode() == MNG_MODIFY
		||static_cast<CCatESL_Mgr*>(pParentMgr)->GetAppMode() == MNG_IMPORT_WORKING
		)
    {
		CString strFormCode;
		CString strMatCode ;
		CString strUseLimitCode ;
		CString strManageCode ;
	     
		CCatESL_Mgr *pCatESL_Mgr = static_cast<CCatESL_Mgr*>(pParentMgr);
		if (pCatESL_Mgr == NULL) return -1;

		CCatApi *pCatApi = pCatESL_Mgr->GetCatApi();
        pCatESL_Mgr->m_pDM_Index->GetCellData(m_strHD_Index + _T("_형식구분_코드"),0,strFormCode); 
        pCatESL_Mgr->m_pDM_Index->GetCellData(m_strHD_Index + _T("_매체구분_코드"),0,strMatCode);
		pCatESL_Mgr->m_pDM_Index->GetCellData(m_strHD_Index + _T("_관리구분_코드"),0,strManageCode); 
        pCatESL_Mgr->m_pDM_Index->GetCellData(m_strHD_Index + _T("_이용제한구분_코드"),0,strUseLimitCode);
		
		
		if(strFormCode.IsEmpty())
		{   
		   pParentMgr->m_pInfo->pCodeMgr->GetDefaultCode(_T("형식구분"),strFormCode);
           m_pCM->SetControlMgrData(_T("형식구분_코드"),strFormCode,-1);
	       pCatESL_Mgr->m_pDM_Index->SetCellData(m_strHD_Index + _T("_형식구분_코드"),strFormCode,0);
           pCatESL_Mgr->m_pDM_Species->SetCellData(m_strHD_Species + _T("_형식구분_코드"),strFormCode,0);        
		}

		if(strMatCode.IsEmpty() && static_cast<CCatESL_Mgr*>(pParentMgr)->GetAppMode() != MNG_MODIFY)
		{
           pParentMgr->m_pInfo->pCodeMgr->GetDefaultCode(_T("매체구분"),strMatCode);
           m_pCM->SetControlMgrData(_T("매체구분_코드"),strMatCode,-1);
		   pCatESL_Mgr->m_pDM_Index->SetCellData(m_strHD_Index + _T("_매체구분_코드"),strMatCode,0);
		   pCatESL_Mgr->m_pDM_Species->SetCellData(m_strHD_Species + _T("_매체구분_코드"),strMatCode,0);
		}
        
		if(strManageCode.IsEmpty())
		{
           pParentMgr->m_pInfo->pCodeMgr->GetDefaultCode(_T("관리구분"),strManageCode);
           m_pCM->SetControlMgrData(_T("관리구분_코드"),strManageCode,-1);
		   pCatESL_Mgr->m_pDM_Index->SetCellData(m_strHD_Index + _T("_관리구분_코드"),strManageCode,0);
		   pCatESL_Mgr->m_pDM_Species->SetCellData(m_strHD_Species + _T("_관리구분_코드"),strManageCode,0);
		}
        
		if(strUseLimitCode.IsEmpty())
		{
           pParentMgr->m_pInfo->pCodeMgr->GetDefaultCode(_T("이용제한구분"),strUseLimitCode);
           m_pCM->SetControlMgrData(_T("이용제한구분_코드"),strUseLimitCode,-1);
		   pCatESL_Mgr->m_pDM_Index->SetCellData(m_strHD_Index + _T("_이용제한구분_코드"),strUseLimitCode,0);
		   pCatESL_Mgr->m_pDM_Species->SetCellData(m_strHD_Species + _T("_이용제한구분_코드"),strUseLimitCode,0);
		}
		
		if (m_nModeMONO_SERIAL == MODE_SERIAL)
		{
			CString strRegCode;
			pCatESL_Mgr->m_pDM_Index->GetCellData(m_strHD_Index + _T("_등록구분_코드"),0,strRegCode);

			if(strRegCode.IsEmpty())
			{
				strFormCode.Format(_T("SE"));
				pCatESL_Mgr->m_pDM_Index->SetCellData(m_strHD_Index + _T("_등록구분_코드"),strFormCode,0);
				pCatESL_Mgr->m_pDM_Species->SetCellData(m_strHD_Species + _T("_등록구분_코드"),strFormCode,0);        
			}
		}
		else
		{
			CString strRegCode, strAcqCode;
			pCatESL_Mgr->m_pDM_Index->GetCellData(m_strHD_Index + _T("_등록구분"),0,strRegCode);
			SetDlgItemText(IDC_eREG_CLASS_CODE_MA,strRegCode);
			pCatESL_Mgr->m_pDM_Index->GetCellData(m_strHD_Index + _T("_수입구분_코드"), 0, strAcqCode);

		}
	}

	m_pCM->AllControlDisplay(0);
	
	CString strWorkingStatus;
 	CString strTemp;
	CCatESL_Mgr *pCatESL_Mgr = static_cast<CCatESL_Mgr*>(pParentMgr);
 	strWorkingStatus = pCatESL_Mgr->m_pDM_Index->GetCellData( _T("IBS_작업상태INFO"), 0);

 	strTemp = strWorkingStatus.Left(3);
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_cSPECIES_CLASS);
	strWorkingStatus.Replace(_T(" "),_T(""));
 	if( strTemp == _T("BOC") || strWorkingStatus == _T("BOL111O") )
 	{
		pCmb->SelectString(0,_T("정리자료"));
 	}
 	else 
 	{
		if( strWorkingStatus != _T("") )
		{
			pCmb->SelectString(0,_T("소장자료"));
		}
 	}

	ShowRegCodeField();

	SetMarcGroupFromFormClassCode();

	CString strMATClassCode;
	m_pCM->GetControlMgrData(m_strHD_Index + _T("_매체구분_코드"), strMATClassCode);

	CMarcAdjMgr *m_pMarcAdjMgr;
	m_pMarcAdjMgr = static_cast<CMarcAdjFrame*>(GetParentFrame())->GetMarcAdjMgr();
    CMarcAdjResourceMgr *m_pResourceMgr = m_pMarcAdjMgr->GetResourceMgr () ;

	if (strMATClassCode == _T("EB"))
	{
	  	m_pResourceMgr->HideToolBarButton (_T("ebook보기"),FALSE);
	}
	else 
	{
	 	m_pResourceMgr->HideToolBarButton (_T("ebook보기")) ;
	}

	return 0;

	EFS_END
	return -1;
}

INT CMarcAdjCodeDlg::ShowRegCodeField()
{
	EFS_BEGIN

	CString strArrCode;

	if (m_nModeMONO_SERIAL == MODE_SERIAL)
	{
		strArrCode.Format(_T("SE"));
		m_pCM->SetControlMgrData(m_strHD_Index + _T("_등록구분_코드"),strArrCode,-1);
	}
	else
	{
		CCatESL_Mgr *pCatESL_Mgr = static_cast<CCatESL_Mgr*>(pParentMgr);
		if (pCatESL_Mgr == NULL) return -1;

		CString strRegCode;
		pCatESL_Mgr->m_pDM_Index->GetCellData(m_strHD_Index + _T("_등록구분"),0,strRegCode);
		SetDlgItemText(IDC_eREG_CLASS_CODE_MA,strRegCode);
		if ( strRegCode == _T("") )
		{
			m_pInfo->pCodeMgr->GetDefaultCode(_T("등록구분"), strArrCode);
			SetDlgItemText(IDC_eREG_CLASS_CODE_MA,strArrCode);
		}
	}

	CCatApi *pCatApi = static_cast<CCatESL_Mgr*>(pParentMgr)->GetCatApi();

	if (pCatApi->GetVolumeCount(static_cast<CCatESL_Mgr*>(pParentMgr)->m_pDM_Book) == 1)
		EnableRegCodeField(TRUE);
	else
		EnableRegCodeField(FALSE);


	CArray<CCatRegCode, CCatRegCode> arrRegCode;
	pCatApi->GetRegCodeInfo(static_cast<CCatESL_Mgr*>(pParentMgr)->m_pDM_Book, &arrRegCode);
	CCatRegCode regCode;
	CString strTemp;
	CString strRegCodeField;
	INT nBookCount = 0;
	INT nArrCount = arrRegCode.GetSize();
	if (nArrCount != 0)
	{
		for (INT i = 0; i < nArrCount - 1; i++)
		{
			regCode = arrRegCode.GetAt(i);
			nBookCount += regCode.m_nCount;
			strTemp.Format(_T("%s%d/"), regCode.m_strRegCode, regCode.m_nCount);
			strRegCodeField += strTemp;
		}
		
		regCode = arrRegCode.GetAt(nArrCount - 1);	
		nBookCount += regCode.m_nCount;
		strTemp.Format(_T("%s%d"), regCode.m_strRegCode, regCode.m_nCount);
		strRegCodeField += strTemp;
		
		SetRegCodeFieldData(strRegCodeField);
	}

	return 0;

	EFS_END
	return -1;
}

CString CMarcAdjCodeDlg::GetRegCodeFieldData()
{
	EFS_BEGIN

	UpdateData();
	return m_strRegCodeField;

	EFS_END
	return _T("");
}

VOID CMarcAdjCodeDlg::SetRegCodeFieldData(LPCTSTR lpszCodeField)
{
	EFS_BEGIN

	m_strRegCodeField = lpszCodeField;
	UpdateData(FALSE);

	EFS_END
}

VOID CMarcAdjCodeDlg::RollBackRegCodeFieldData()
{
	EFS_BEGIN

	m_strRegCodeField = m_strRegCodeField_Bak;
	UpdateData(FALSE);

	EFS_END
}

VOID CMarcAdjCodeDlg::EnableRegCodeField(BOOL bEnable)
{
	EFS_BEGIN

	CWnd *pWnd = GetDlgItem(IDC_eREG_CLASS_CODE_MA);
	if (!pWnd) return;

	pWnd->EnableWindow(bEnable);

	EFS_END
}

VOID CMarcAdjCodeDlg::SetMarcGroupFromFormClassCode()
{
	EFS_BEGIN

	if (static_cast<CCatESL_Mgr*>(pParentMgr)->m_nModeMONO_SERIAL == MODE_SERIAL) 
	{
		m_pInfo->pMarcMgr->SetMarcGroup(_T("통합마크"));
		return;
	}

	m_pCM->GetControlMgrData(m_strHD_Index + _T("_형식구분_코드"), m_strFormCode_Bak);

	m_pInfo->pMarcMgr->SetMarcGroup(_T("통합마크"));		

	EFS_END
}

INT CMarcAdjCodeDlg::ChangeLinkDMtoSerial(CESL_ControlMgr *pCM)
{
	EFS_BEGIN

	if (pCM == NULL) return -1;

	CESL_Control_Element *pCE = NULL;
	POSITION pos = pCM->Control_List.GetHeadPosition();

	INT ids = -1;
	CString strLinkData;
	CString strCtrlAlias;
	CString strLinkDM;
	while (pos)
	{
		pCE = (CESL_Control_Element*)pCM->Control_List.GetNext(pos);
		if (pCE == NULL) continue;
		
		if (pCE->CTRL_TYPE == _T("G")) continue;

		 strLinkData = pCE->LINK_DATA;
		 strCtrlAlias = pCE->CTRL_ALIAS;
		
		 strLinkData.Replace(_T("단행"), _T("연속"));
		 strLinkData.Replace(_T("IBS"), _T("ISS"));
		 strLinkData.Replace(_T("BS"), _T("SS"));
		 strLinkData.Replace(_T("BB"), _T("SB"));

		 strCtrlAlias.Replace(_T("단행"), _T("연속"));
		 strCtrlAlias.Replace(_T("IBS"), _T("ISS"));
		 strCtrlAlias.Replace(_T("BS"), _T("SS"));
		 strCtrlAlias.Replace(_T("BB"), _T("SB"));

		 pCE->LINK_DATA = strLinkData;
		 pCE->CTRL_ALIAS = strCtrlAlias;
	}

	m_strHD_Species = _T("SS");
	m_strHD_Index = _T("ISS");
	m_strHD_Book = _T("SB");

	return 0;

	EFS_END
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// CMarcAdjCodeDlg message handlers

BOOL CMarcAdjCodeDlg::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();

	CString strArrCode;	

	if (InitESL_Mgr(_T("SM_마크정리_코드정보")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr ERROR!"));
		return FALSE;
	}
		

	m_pCM = FindCM(_T("CM_마크정리_코드정보"));
	if ( m_pCM == NULL )
	{
		AfxMessageBox(_T("InitESL_Mgr ERROR!"));
		return FALSE;
	}

	m_nModeMONO_SERIAL = static_cast<CCatESL_Mgr*>(pParentMgr)->m_nModeMONO_SERIAL;

	if (m_nModeMONO_SERIAL == MODE_SERIAL)
	{
		ChangeLinkDMtoSerial(m_pCM);

		
		
		GetDlgItem(IDC_tSHELF_CODE_MA)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_cSHELF_CODE_MA)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_cSHELF_CODE_MA)->EnableWindow(TRUE);

		GetDlgItem(IDC_eREG_CLASS_CODE_MA)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_eREG_CLASS_CODE_MA)->EnableWindow(FALSE);

		GetDlgItem(IDC_cREG_CODE_MA)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_cREG_CODE_MA)->EnableWindow(TRUE);
	}
	else
		OnSelchangecACQCODEMA();

	if (static_cast<CCatESL_Mgr*>(pParentMgr)->GetAppMode() == MNG_INSERT)
	{
		if (m_nModeMONO_SERIAL == MODE_SERIAL)
			((CComboBox*)GetDlgItem(IDC_cSPECIES_CLASS))->SetCurSel( 1 );
		GetDlgItem(IDC_cSPECIES_CLASS)->ShowWindow(SW_SHOW);
	}
    else
	{
		m_pCM->AllControlDisplay(0);

		CString strWorkingStatus;
 		CString strTemp;
		CCatESL_Mgr *pCatESL_Mgr = static_cast<CCatESL_Mgr*>(pParentMgr);
 		strWorkingStatus = pCatESL_Mgr->m_pDM_Index->GetCellData( _T("IBS_작업상태INFO"), 0);
 
 		strTemp = strWorkingStatus.Left(3);
		CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_cSPECIES_CLASS);
 		if( strTemp == _T("BOC") || strWorkingStatus == _T("BOL111O") )
 		{
			pCmb->SelectString(0,_T("정리자료"));
 		}
 		else 
 		{
			if( strWorkingStatus != _T("") )
			{
				pCmb->SelectString(0,_T("소장자료"));
			}
			
 		}		
		//=====================================================
	}

	ShowRegCodeField();
	
	m_pCM->GetControlMgrData(m_strHD_Index + _T("_형식구분_코드"), m_strFormCode_Bak);
	if (m_strFormCode_Bak.IsEmpty())
	{
		m_strFormCode_Bak = _T("BK");
 		m_pCM->SetControlMgrData(m_strHD_Index + _T("_형식구분_코드"), m_strFormCode_Bak);
	}

	m_pCM->GetControlMgrData(m_strHD_Species + _T("_제어자료구분_코드"), m_strControlCode_Bak);

	SetMarcGroupFromFormClassCode();

	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
	EFS_END
	return FALSE;
}

VOID CMarcAdjCodeDlg::OnSelchangecACQCODEMA() 
{
	EFS_BEGIN

	ApplyChangeAcqCode();

	EFS_END
}

VOID CMarcAdjCodeDlg::OnbDONATOR() 
{
	EFS_BEGIN

	CDonatorNameGetter_Search searchDlg(this);
	searchDlg.SetOpenMode(-1000);
	
	if( IDOK == searchDlg.DoModal() )
	{
		CDonatorNameGetterDlg searchResult(this);
		if( IDOK == searchResult.DoModal() )
		{
			m_pCM->SetControlMgrData(_T("기증자"),searchResult.GetDonatorName());

			INT nRowCnt = 0;
			CCatESL_Mgr *pCatESL_Mgr = static_cast<CCatESL_Mgr*>(pParentMgr);
			nRowCnt = pCatESL_Mgr->m_pDM_Index->GetRowCount();
			if( nRowCnt == 1 )
			{
				pCatESL_Mgr->m_pDM_Index->SetCellData( _T("IBS_기증자")	  , searchResult.GetDonatorName(), 0 );
				pCatESL_Mgr->m_pDM_Index->SetCellData( _T("IBS_기증자KEY"), searchResult.GetDonatorKey() , 0 );
			}
		}
	}

	EFS_END
}

VOID CMarcAdjCodeDlg::OnKillfocuseREGCLASS() 
{
	EFS_BEGIN

	if (!IsWindowVisible()) return;
	CCatESL_Mgr *pCatMgr = static_cast<CCatESL_Mgr*>(pParentMgr);
	CCatApi *pCatApi = pCatMgr->GetCatApi();

	CString strRegCodeField = GetRegCodeFieldData();

	
	CArray<CCatRegCode, CCatRegCode> arrRegCode;
	INT ids = pCatApi->FetchRegCode(strRegCodeField, &arrRegCode);
	if (ids < 0)
	{
		MessageBox(_T("없는 등록구분입니다."));
		RollBackRegCodeFieldData();
		return;
	}


	INT nRegCodeCount = arrRegCode.GetSize();
	if (nRegCodeCount <= 0) return;
	
	if (pCatMgr->m_nModeMONO_SERIAL == MODE_MONO)
	{	
		CString strVol;

		INT nBookIdx = -1;

		if (pCatApi->GetRepresentationBook(pCatMgr->m_pDM_Book, m_strHD_Book , nBookIdx) < 0)
			nBookIdx = pCatApi->SetRepresentationBook(pCatMgr->m_pDM_Book, m_strHD_Book );

		
		pCatMgr->m_pDM_Book->GetCellData(m_strHD_Book + _T("_편권차"), nBookIdx, strVol);
		if (pCatApi->IsRegCodeModify(&arrRegCode, pCatMgr->m_pDM_Book, strVol, m_strHD_Book ))
		{
			AfxMessageBox(_T("등록구분이 수정이 될수 있습니다. 책정보에서 수정해주세요"));
			RollBackRegCodeFieldData();
			return;			
		}
	}	

	m_strRegCodeField_Bak = strRegCodeField;

	EFS_END
}

VOID CMarcAdjCodeDlg::OnSelchangecFORMCLASSMA() 
{
	EFS_BEGIN

	CString strFormClassCode;
	m_pCM->GetControlMgrData(m_strHD_Index + _T("_형식구분_코드"), strFormClassCode);

	if (strFormClassCode.IsEmpty())
	{
		MessageBox(_T("형식구분은 적용안함을 선택할 수 없습니다1."));	
		if( m_strFormCode_Bak.IsEmpty() )
		{
			m_pCM->SetControlMgrData(m_strHD_Index + _T("_형식구분_코드"), _T("BK"));
		}
		else
		{
			m_pCM->SetControlMgrData(m_strHD_Index + _T("_형식구분_코드"), m_strFormCode_Bak);
		}
		
		GetDlgItem(IDC_cFORM_CLASS_MA)->SetFocus();
		return;
	}
	
	if ( m_strFormCode_Bak == strFormClassCode ) return;

	LPCTSTR lpszText = _T("형식구분을 변경하면 기존의 MARC의 007 형태기술필드, 008 부호화 정보필드가 삭제됩니다. 수정하시겠습니까 ?\n형식구분 변경후에는 알맞은 매체구분값을 세팅했는지 확인하시기 바랍니다.");

	UINT rc = MessageBox(lpszText, _T("형식구분 변경 확인"), MB_YESNO | MB_ICONQUESTION);
	if (IDYES!=rc) 
	{
		m_pCM->SetControlMgrData(m_strHD_Index + _T("_형식구분_코드"), m_strFormCode_Bak);
		return;
	}

	m_pInfo->pMarcMgr->DeleteField(static_cast<CMarcAdjFrame*>(GetParentFrame())->GetMarcAdjMgr()->GetMarc(), _T("007"));
	m_pInfo->pMarcMgr->DeleteField(static_cast<CMarcAdjFrame*>(GetParentFrame())->GetMarcAdjMgr()->GetMarc(), _T("008"));
	
	static_cast<CCatESL_Mgr*>(pParentMgr)->m_pDM_Index->SetCellData(m_strHD_Index + _T("_형식구분_코드"), strFormClassCode, 0); 
	m_strFormCode_Bak = strFormClassCode;
	
	SetMarcGroupFromFormClassCode();
	
	ApplyCodeChange(FALSE);

	m_pInfo->pMarcMgr->CheckSimpleSyntax(static_cast<CMarcAdjFrame*>(GetParentFrame())->GetMarcAdjMgr()->GetMarc());
	m_pInfo->pMarcMgr->CheckDeepSyntax(static_cast<CMarcAdjFrame*>(GetParentFrame())->GetMarcAdjMgr()->GetMarc());

	static_cast<CMarcAdjFrame*>(GetParentFrame())->GetMarcAdjMgr()->InitMarcEditor();
	static_cast<CMarcAdjFrame*>(GetParentFrame())->GetMarcAdjMgr()->ApplyMarcChange();
	static_cast<CMarcAdjFrame*>(GetParentFrame())->GetMarcAdjMgr()->GetResourceMgr()->GetMarcEditor()->Display();
	

	EFS_END
}

VOID CMarcAdjCodeDlg::OnKillfocuscFORMCLASSMA() 
{
	EFS_BEGIN

	CString strFormClassCode;
	m_pCM->GetControlMgrData(m_strHD_Index + _T("_형식구분_코드"), strFormClassCode);

	if (strFormClassCode.IsEmpty())
	{
		if( m_strFormCode_Bak.IsEmpty() )
		{
			m_pCM->SetControlMgrData(m_strHD_Index + _T("_형식구분_코드"), _T("BK"));
		}
		else
		{
			m_pCM->SetControlMgrData(m_strHD_Index + _T("_형식구분_코드"), m_strFormCode_Bak);
		}

		GetDlgItem(IDC_cFORM_CLASS_MA)->SetFocus();
		return;	
	}

	EFS_END
}

VOID CMarcAdjCodeDlg::OnchkFiexedField() 
{
	EFS_BEGIN

	UpdateData();	

	EFS_END
}

VOID CMarcAdjCodeDlg::OnSelchangecMEDCLASSI() 
{
	// TODO: Add your control notification handler code here
		
	EFS_BEGIN

	CString strMATClassCode;
	m_pCM->GetControlMgrData(m_strHD_Index + _T("_매체구분_코드"), strMATClassCode);

	CMarcAdjMgr *m_pMarcAdjMgr;
	m_pMarcAdjMgr = static_cast<CMarcAdjFrame*>(GetParentFrame())->GetMarcAdjMgr();
    CMarcAdjResourceMgr *m_pResourceMgr = m_pMarcAdjMgr->GetResourceMgr () ;

	if (strMATClassCode == _T("EB"))
	{
	  	m_pResourceMgr->HideToolBarButton (_T("ebook보기"),FALSE);
	}
	else 
	{
		m_pResourceMgr->HideToolBarButton (_T("ebook보기")) ;
	}

	EFS_END
}

VOID CMarcAdjCodeDlg::OnEditchangecCONTROLMATCLASSADJ() 
{
EFS_BEGIN

	CString strControlMatCode;
	m_pCM->GetControlMgrData(m_strHD_Species + _T("_제어자료구분_코드"), strControlMatCode);

	if (strControlMatCode.IsEmpty())
	{
		GetDlgItem(IDC_cCONTROL_MAT_CLASS_ADJ)->SetFocus();
		return;
	}

	if ( !m_strControlCode_Bak.Compare(strControlMatCode) ) return;

	LPCTSTR lpszText = _T("제어자료구분을 변경하면 기존의 제어번호가 삭제되고,\n새로 발급받게 됩니다. 변경하시겠습니까 ?");
	if (MessageBox(lpszText, _T("제어자료구분 변경 확인"), MB_YESNO | MB_ICONQUESTION) == IDYES) 
	{
		m_pInfo->pMarcMgr->DeleteField(static_cast<CMarcAdjFrame*>(GetParentFrame())->GetMarcAdjMgr()->GetMarc(), _T("001"));

		m_strControlCode_Bak = strControlMatCode;
	}
	else
	{
		m_pCM->SetControlMgrData(m_strHD_Species + _T("_제어자료구분_코드"), m_strControlCode_Bak);
	}

	ApplyCodeChange();

EFS_END
}

HBRUSH CMarcAdjCodeDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}


BOOL CMarcAdjCodeDlg::PreTranslateMessage(MSG* pMsg) 
{
	EFS_BEGIN

	CString strKey;
    INT nIds;
	if (pMsg->message == WM_KEYDOWN)
	{		
		if ( GetKeyState(VK_CONTROL) < 0 )
		{
			strKey.Format(_T("CTRL+%c"), pMsg->wParam);
			if (strKey == _T("CTRL+S")) 
			{
				pMsg->wParam = 0;
				nIds = static_cast<CMarcAdjFrame*>(GetParentFrame())->GetMarcAdjMgr()->CMD_Save();
				if( 0 <= nIds )
				{
					return TRUE;
				}
				else
				{
					return FALSE;
				}
			}
		}
	}
		
	return CESL_Mgr::PreTranslateMessage(pMsg);

	EFS_END
	return FALSE;
}
