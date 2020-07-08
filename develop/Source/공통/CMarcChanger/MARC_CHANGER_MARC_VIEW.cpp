// MARC_CHANGER_MARC_VIEW.cpp : implementation file
//

#include "stdafx.h"
#include "MARC_CHANGER_MARC_VIEW.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMARC_CHANGER_MARC_VIEW dialog


CMARC_CHANGER_MARC_VIEW::CMARC_CHANGER_MARC_VIEW(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CMARC_CHANGER_MARC_VIEW::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMARC_CHANGER_MARC_VIEW)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	AfxInitRichEdit();

	m_pMarcViewDM = NULL;

	m_pMarcViewCM = NULL;

	m_bShowErrorDlg = TRUE;

	m_nChangeLastIdx = -1;
	m_nChangeFirstIdx = -1;
	m_nChangeIdx = -1;

	m_pParentDM = NULL;
	m_pParentGrid = NULL;
}



CMARC_CHANGER_MARC_VIEW::~CMARC_CHANGER_MARC_VIEW()
{
	
}


void CMARC_CHANGER_MARC_VIEW::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMARC_CHANGER_MARC_VIEW)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMARC_CHANGER_MARC_VIEW, CDialog)
	//{{AFX_MSG_MAP(CMARC_CHANGER_MARC_VIEW)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_btnSave, OnbtnSave)
	ON_BN_CLICKED(IDC_tbnClose, OntbnClose)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_btnPRE_VIEW, OnbtnPREVIEW)
	ON_BN_CLICKED(IDC_btnNEXT_VIEW, OnbtnNEXTVIEW)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMARC_CHANGER_MARC_VIEW message handlers

HBRUSH CMARC_CHANGER_MARC_VIEW::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

BOOL CMARC_CHANGER_MARC_VIEW::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(InitESL_Mgr(_T("SM_MARC_CHANGER_MARC_VIEW")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return -1;
	}

	m_pMarcViewCM = FindCM(_T("CM_MARC_CHANGER_MARC_VIEW"));

	m_pMarcViewDM = FindDM(_T("DM_MARC_CHANGER_MARC_VIEW"));
	
	if(!Init())
	{
		ESLAfxMessageBox(_T("마크교체 상세보기 화면을 초기화 할 수 없습니다."));
		EndDialog(IDCANCEL);
		return FALSE;
	}

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CMARC_CHANGER_MARC_VIEW::Init()
{
	INT ids;
	ids = 0;
	if(m_pParentDM == NULL || m_pParentGrid == NULL) return FALSE;
	m_Marceditor_Sourse.SubclassDlgItem(IDC_richSOURCE_MARC, -1, this);
	m_Marceditor_Target.SubclassDlgItem(IDC_richTARGET_MARC, -1, this);
	m_pInfo->pMarcMgr->SetMarcGroup(_T("통합마크"));
	Display(0);
	if(ids < 0) return FALSE;
	return TRUE;
}

void CMARC_CHANGER_MARC_VIEW::OnbtnSave() 
{
	m_Marceditor_Target.ApplyEdit(0);
	
	CString strMarcCheckResult;


	INT ids = m_pInfo->pMarcMgr->CheckSimpleSyntax(&m_Marc_Target);

	CString strMsg;
	if (0 > ids)
	{
		strMsg.Format(_T("%s\n마크오류가 있습니다. 적용하시겠습니까?"), strMarcCheckResult);

		if (MessageBox(strMsg.GetBuffer(0), _T("마크문법오류"), MB_YESNO | MB_ICONQUESTION) == IDNO)
		{
			m_Marceditor_Target.Display();
			return;
		}
	}
	CString strTargetMarc;
	m_pInfo->pMarcMgr->Encoding(&m_Marc_Target, strTargetMarc);
	strTargetMarc.Replace(_T("'"), _T("''"));
	
	m_pParentDM->SetCellData(_T("TARGET_MARC"), strTargetMarc, m_nChangeIdx);

	CArray<CString, CString&> arrItem;
	CString		strISBN;
	CString strTagData;
	CString str245TagData;
	CString str260TagData;
	CString strTemp1;
	CString strTemp2;
	CString strISBNList;
	CString strIsbnCnt;
	CString strSourceIsbnCnt;
	INT nItemCount;

	CString strSetISBN;
	ids = m_pInfo->pMarcMgr->GetItem(&m_Marc_Target, _T("세트ISBN"), strSetISBN,&arrItem);	
	ids = m_pInfo->pMarcMgr->GetItem(&m_Marc_Target, _T("020$a"), strISBN, &arrItem);
	nItemCount = arrItem.GetSize();
	for (INT i = 0; i < nItemCount; i++)
	{
		
		strISBN = arrItem.GetAt(i);
		if( strISBN == strSetISBN )
		{
			strISBN.Empty();
			continue;
		}
		strISBNList += _T(" ") + strISBN;
		strISBNList.TrimLeft();
	}
	if( !strSetISBN.IsEmpty() )
	{
		--nItemCount;
	}	

	strTemp1.Empty();
	strTemp2.Empty();
	
	ids = m_pInfo->pMarcMgr->GetItem(&m_Marc_Target, _T("245$a"), strTemp1);
	ids = m_pInfo->pMarcMgr->GetItem(&m_Marc_Target, _T("245$d"), strTemp2);

	str245TagData.Format(_T("%s/%s"),strTemp1, strTemp2 );

	strTemp1.Empty();
	strTemp2.Empty();
	
	ids = m_pInfo->pMarcMgr->GetItem(&m_Marc_Target, _T("260$b"), strTemp1);
	ids = m_pInfo->pMarcMgr->GetItem(&m_Marc_Target, _T("260$c"), strTemp2);
	
	str260TagData.Format(_T("%s,%s"),strTemp1, strTemp2 );

	strISBNList.TrimLeft();
	strTagData.Format( _T("%s\n%s\n%s"), strISBNList, str245TagData, str260TagData );

	m_pParentDM->SetCellData(_T("TARGET_TAG_DATA"), strTagData, m_nChangeIdx);
	
	strIsbnCnt.Empty();

	if( strSetISBN.IsEmpty() )
	{
		strIsbnCnt.Format(_T("%d"),nItemCount);
	}
	else
	{
		strIsbnCnt.Format(_T("%d"),nItemCount-1);
	}

	m_pParentDM->SetCellData(_T("TARGET_ISBN_CNT"), strIsbnCnt, m_nChangeIdx);

	m_pParentDM->GetCellData(_T("SOURCE_ISBN_CNT"), m_nChangeIdx, strSourceIsbnCnt);
	if( strSourceIsbnCnt == strIsbnCnt ) 
	{
		m_pParentDM->SetCellData(_T("비고"), _T(""), m_nChangeIdx);
	}
	else
	{
		m_pParentDM->SetCellData(_T("비고"), _T("편권차를 확인해주세요."), m_nChangeIdx);
	}

	AfxMessageBox(_T("저장하였습니다."));
	m_Marceditor_Target.Display();

	if( m_nChangeLastIdx > m_nChangeIdx )
	{
		Display(1);
	}

}


void CMARC_CHANGER_MARC_VIEW::OntbnClose() 
{
	CDialog::OnOK();
	
}

void CMARC_CHANGER_MARC_VIEW::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}

void CMARC_CHANGER_MARC_VIEW::OnbtnPREVIEW() 
{
	Display(-1);	
}

void CMARC_CHANGER_MARC_VIEW::OnbtnNEXTVIEW() 
{
	Display(1);	
}

VOID CMARC_CHANGER_MARC_VIEW::Display(INT nIdx)
{
	switch(nIdx)
	{
	case 0:
		InitFirstLastIndex();
		GetFirstSelectIndex(m_nChangeIdx);
		ShowChangeNextPrevButton(m_nChangeIdx);
		break;
	case -1:
		GetPrevSelectIndex(m_nChangeIdx);
		ShowChangeNextPrevButton(m_nChangeIdx);
		break;
	case 1:
		GetNextSelectIndex(m_nChangeIdx);
		ShowChangeNextPrevButton(m_nChangeIdx);
		break;
	default:
		return ;
	}


	CString strMarcSourse;
	CString strMarcTarget;
	CString strTagCode;
	CString strErrMsg;
	CString strHelpMsg;
	
	strMarcSourse = m_pParentDM->GetCellData( _T("SOURCE_MARC"), m_nChangeIdx );
	strMarcTarget = m_pParentDM->GetCellData( _T("TARGET_MARC") , m_nChangeIdx );

	if(m_pInfo->pMarcMgr->Decoding(strMarcSourse, &m_Marc_Sourse))
	{
		POSITION pos;
		pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
		while (pos)
		{
			m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);
			UpdateData(FALSE);
		}	
	}

	m_Marceditor_Sourse.SetMarcMgr(m_pInfo->pMarcMgr);
 	m_Marceditor_Sourse.InitLimitTag();
	m_Marceditor_Sourse.Init(m_pInfo->pMarcMgr, &m_Marc_Sourse);
	m_Marceditor_Sourse.Display();




	if(m_pInfo->pMarcMgr->Decoding(strMarcTarget, &m_Marc_Target))
	{
		POSITION pos;
		pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
		while (pos)
		{
			m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);
			UpdateData(FALSE);
		}	
	}
	m_Marceditor_Sourse.SetMarcMgr(m_pInfo->pMarcMgr);
 	m_Marceditor_Sourse.InitLimitTag();
	m_Marceditor_Target.Init(m_pInfo->pMarcMgr, &m_Marc_Target);
	m_Marceditor_Target.Display();

	m_pParentGrid->SetReverse(m_nChangeIdx);
}

VOID CMARC_CHANGER_MARC_VIEW::InitFirstLastIndex()
{
	INT ids = m_pParentGrid->SelectMakeList();
	m_nChangeFirstIdx = m_pParentGrid->SelectGetHeadPosition();
	m_nChangeLastIdx = m_pParentGrid->SelectGetTailPosition();
}

INT CMARC_CHANGER_MARC_VIEW::GetFirstSelectIndex(INT &m_nChangeIdx)
{
	INT nFirstSelIdx = -1;
	nFirstSelIdx = m_nChangeFirstIdx;
	
	if(nFirstSelIdx < 0) 
	{
		nFirstSelIdx = 0;
	}
	m_nChangeIdx = nFirstSelIdx;

	return nFirstSelIdx;
}

VOID CMARC_CHANGER_MARC_VIEW::ShowChangeNextPrevButton(INT idx)
{
	GetDlgItem(IDC_btnPRE_VIEW)->EnableWindow(TRUE);
	GetDlgItem(IDC_btnNEXT_VIEW)->EnableWindow(TRUE);

	if(m_nChangeFirstIdx >= idx || idx < 0)
		GetDlgItem(IDC_btnPRE_VIEW)->EnableWindow(FALSE);
	if(m_nChangeLastIdx <= idx || idx < 0)
		GetDlgItem(IDC_btnNEXT_VIEW)->EnableWindow(FALSE);
}

INT CMARC_CHANGER_MARC_VIEW::GetPrevSelectIndex(INT &m_nChangeIdx)
{
	INT nPrevSelIndex = -1;

	nPrevSelIndex = m_pParentGrid->SelectGetTailPosition();
	while(1)
	{
		if( m_nChangeIdx == nPrevSelIndex )
		{
			nPrevSelIndex = m_pParentGrid->SelectGetPrev();
			if(nPrevSelIndex < 0)
			{
				nPrevSelIndex = m_nChangeIdx;	
			}	
			break;
		}
		nPrevSelIndex = m_pParentGrid->SelectGetPrev();		
	}

	m_nChangeIdx = nPrevSelIndex;
	return nPrevSelIndex;
}

INT CMARC_CHANGER_MARC_VIEW::GetNextSelectIndex(INT &m_nChangeIdx)
{
	INT nNextSelIndex = -1;

	nNextSelIndex = m_pParentGrid->SelectGetHeadPosition();
	while(1)
	{
		if( m_nChangeIdx == nNextSelIndex )
		{
			nNextSelIndex = m_pParentGrid->SelectGetNext();
			if(nNextSelIndex < 0)
			{
				nNextSelIndex = m_nChangeIdx;	
			}	
			break;
		}
		nNextSelIndex = m_pParentGrid->SelectGetNext();		
	}
	m_nChangeIdx = nNextSelIndex;
	return nNextSelIndex;
}


