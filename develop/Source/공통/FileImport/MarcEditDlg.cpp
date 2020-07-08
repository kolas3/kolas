// MarcEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MarcEditDlg.h"
#include "efs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMarcEditDlg dialog


CMarcEditDlg::CMarcEditDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMarcEditDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	AfxInitRichEdit();
	m_nFileType = -1;
	m_bShowErrorDlg = TRUE;
	m_pMarcImpExpMgr = NULL;
}

CMarcEditDlg::~CMarcEditDlg()
{

}


BOOL CMarcEditDlg::Create(CWnd *pParentWnd)
{
	EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;

}

VOID CMarcEditDlg::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMarcEditDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CMarcEditDlg, CDialog)
	//{{AFX_MSG_MAP(CMarcEditDlg)
	ON_BN_CLICKED(IDC_bPREV, OnbPREV)
	ON_BN_CLICKED(IDC_bNEXT, OnbNEXT)
	ON_BN_CLICKED(IDC_bAPPLY_MARC, OnbAPPLYMARC)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMarcEditDlg Operations
INT CMarcEditDlg::SetParentInfo(CESL_DataMgr *pDM_Parent, 
								CESL_DataMgr *pGridDm_Parent, 
								CESL_Grid *pGrid_Parent, 
								INT nFileType,
								CString strMarcFieldAlias, 
								CString strMarcErrorFieldAlias /*= _T("")*/,
								INT nChkMultiBook)
{
	EFS_BEGIN

	if (pDM_Parent == NULL|| pGridDm_Parent == NULL || pGrid_Parent == NULL || strMarcFieldAlias.IsEmpty())
		return -1;
	
	m_pDM_Parent = pDM_Parent;
	m_pGrid_Parent = pGrid_Parent;
	m_nFileType = nFileType;
	m_strMarcFieldAlias = strMarcFieldAlias;
	m_strMarcErrorFieldAlias = strMarcErrorFieldAlias;
	m_nChkMultiBook = nChkMultiBook;
	m_pGridDm_Parent = pGridDm_Parent;

	return 0;

	EFS_END
	return -1;

}

INT CMarcEditDlg::Init()
{
	EFS_BEGIN

	if (m_pDM_Parent == NULL || m_pGridDm_Parent == NULL || 
		m_pGrid_Parent == NULL || m_strMarcFieldAlias.IsEmpty()) 
	{
		return -1;
	}

	INT ids = -1;

	if (!m_marcEditor.SubclassDlgItem(IDC_RICHEDIT_MARC, -1, this))
	{
		AfxMessageBox(_T("Subclass Marc Editor Failed"));
		return -1;
	}

	m_marcEditor.SetMarcMgr(m_pInfo->pMarcMgr);

	ids = m_pGrid_Parent->SelectMakeList();
	if (ids < 0)
	{
		AfxMessageBox(_T("선택목록을 구성할 수 없습니다."));
		return -1;
	}

	m_nCurIdx = m_pGrid_Parent->SelectGetHeadPosition();
	if (m_nCurIdx < 0)
	{
		AfxMessageBox(_T("선택목록을 구성할 수 없습니다."));
		return -1;
	}

	m_pGrid_Parent->SetReverse(m_nCurIdx);

	return 0;

	EFS_END
	return -1;

}

INT CMarcEditDlg::Display()
{
	EFS_BEGIN

	INT ids = -1;
	
	CString strDMIdx;
	strDMIdx = m_pGridDm_Parent->GetCellData(_T("순위"),m_pGrid_Parent->SelectGetIdx());

	m_nParentSelectedDMIdx = _ttoi(strDMIdx) - 1;

	if (m_nParentSelectedDMIdx < 0) return -1;

	CString strStreamMarc = m_pDM_Parent->GetCellData(m_strMarcFieldAlias, m_nParentSelectedDMIdx);
	if ( strStreamMarc.IsEmpty()) return -1;
	ids = m_pInfo->pMarcMgr->Decoding(strStreamMarc, &m_marc);
	if (ids < 0)
	{
		return -1;
	}
	m_str049.Empty();
	if(m_nFileType == 0)	
	{
		m_pInfo->pMarcMgr->GetField(&m_marc, _T("049"), m_str049);
		m_pInfo->pMarcMgr->DeleteField(&m_marc, _T("049"));
	}
	m_marcEditor.Init(m_pInfo->pMarcMgr, &m_marc);
	
	m_marcEditor.Display();

	return 0;

	EFS_END
	return -1;

}

/////////////////////////////////////////////////////////////////////////////
// CMarcEditDlg message handlers

BOOL CMarcEditDlg::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	
	if (Init() < 0)
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}
	
	Display();

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;

}

VOID CMarcEditDlg::OnbPREV() 
{
	EFS_BEGIN

	INT nIdx = m_pGrid_Parent->SelectGetPrev();
	if (nIdx < 0)
		nIdx = m_pGrid_Parent->SelectGetHeadPosition();
	
	m_nCurIdx = nIdx;
	m_pGrid_Parent->SetReverse(m_nCurIdx);
	Display();

	EFS_END
}

VOID CMarcEditDlg::OnbNEXT() 
{
	EFS_BEGIN

	INT nIdx = m_pGrid_Parent->SelectGetNext();
	if (nIdx < 0)
		nIdx = m_pGrid_Parent->SelectGetTailPosition();
	m_nCurIdx = nIdx;
	m_pGrid_Parent->SetReverse(m_nCurIdx);
	Display();

	EFS_END
}

VOID CMarcEditDlg::OnbAPPLYMARC() 
{
	EFS_BEGIN

	INT ids = -1;
	INT nIdx;

	if (m_bShowErrorDlg)
		ids = m_marcEditor.ApplyEdit(0);
	else 
		ids = m_marcEditor.ApplyEdit(1);		

	CString strMarcCheckResult;
	if(m_pMarcImpExpMgr)
	{
		m_pMarcImpExpMgr->CheckMarcSyntax(&m_marc, strMarcCheckResult);
	}
	else
	{
		ESLAfxMessageBox(_T("마크 유효성 검사를 수행할 수 없습니다."));
		return;
	}

	nIdx = m_pGrid_Parent->GetRow() - 1;

	CString strMsg;
	if (!strMarcCheckResult.IsEmpty())
	{
		strMsg.Format(_T("%s\n마크오류가 있습니다. 적용하시겠습니까?"), strMarcCheckResult);

		if (MessageBox(strMsg.GetBuffer(0), _T("마크문법오류"), MB_YESNO | MB_ICONQUESTION) == IDNO)
			return;

		if (!m_strMarcErrorFieldAlias.IsEmpty())
		{
			m_pDM_Parent->SetCellData(m_strMarcErrorFieldAlias, strMarcCheckResult, m_nParentSelectedDMIdx);

		}
	}
	else
	{
		if (!m_strMarcErrorFieldAlias.IsEmpty())
		{
			m_pDM_Parent->SetCellData(m_strMarcErrorFieldAlias, _T(""), m_nParentSelectedDMIdx);
		}	
	}

	CString strStremMarc;
	if(m_nFileType == 0)
	{	
		m_str049 = _T("049") + m_str049;
		m_pInfo->pMarcMgr->InsertField(&m_marc, m_str049);
	}
	ids = m_pInfo->pMarcMgr->Encoding(&m_marc, strStremMarc);
	if (ids < 0)
	{
		AfxMessageBox(_T("적용실패"));
	}

	m_pDM_Parent->SetCellData(m_strMarcFieldAlias, strStremMarc, m_nParentSelectedDMIdx);

	ApplySpeciesDmToImportDm( m_nParentSelectedDMIdx, nIdx );
	
	AfxMessageBox(_T("적용하였습니다"));
	
	if(m_nFileType == 0)
	{
		m_pInfo->pMarcMgr->DeleteField(&m_marc, _T("049"));
	}

	PostMessage(WM_COMMAND, IDC_bNEXT, BN_CLICKED);

	EFS_END
}

HBRUSH CMarcEditDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

CMarcEditDlg::ApplySpeciesDmToImportDm( INT nDmIdx, INT nCridIdx )
{
	CESL_DataMgr* pDM = NULL;

	INT nRegCodeLength;
	INT i;
	INT nColCnt;

	CString strAlias;
	CStringArray astrAlias;
	CString strData;

	pDM = m_pGridDm_Parent;	

	nColCnt = pDM->GetRefColumnCount();
	nRegCodeLength = m_pInfo->nREG_CODE_LENGTH;

	for( i = 0 ; i < nColCnt ; i++ )
	{
		CString strFieldName;
		CString strFieldType;
		CString strDataType;

		pDM->FindColumnFieldName( i , strAlias , strFieldName ,strFieldType,strDataType,0,strData);
		astrAlias.Add(strAlias);
	}

	for( i = 0 ; i < nColCnt ; i++ )
	{
		strAlias = astrAlias.GetAt(i);
		if( _T("선정") == strAlias || _T("순위") == strAlias ) 
		{
			continue;
		}
		else if( _T("BS_파일반입책수") == strAlias )
		{
			if( m_nChkMultiBook )
			{
				strData = m_pDM_Parent->GetCellData(_T("BS_책수"),nDmIdx);
			}
			else
			{
				strData = _T("1");
			}
		}
		else if( _T("BS_파일반입오류") == strAlias )
		{
			strData = m_pDM_Parent->GetCellData(_T("BS_파일반입오류"),nDmIdx);
		}
		else if( _T("IB_목차유무") == strAlias )
		{
			strData = m_pDM_Parent->GetCellData(_T("IB_목차유무"),nDmIdx);
		}
		else if( _T("IB_초록유무") == strAlias )
		{
			strData = m_pDM_Parent->GetCellData(_T("IB_초록유무"),nDmIdx);
		}
		else if( _T("낱권ISBN") == strAlias )
		{
			strData = m_pDM_Parent->GetCellData(_T("BS_MARC.020$a"),nDmIdx);
		}
		else if( _T("본표제") == strAlias )
		{
			strData = m_pDM_Parent->GetCellData(_T("BS_MARC.서명"),nDmIdx);
		}
		else if( _T("편권차") == strAlias )
		{
			strData = m_pDM_Parent->GetCellData(_T("BS_MARC.245$n"),nDmIdx);
		}
		else if( _T("가격") == strAlias )
		{
			strData = m_pDM_Parent->GetCellData(_T("BS_MARC.950$b"),nDmIdx);
		}
		else
		{
			m_pDM_Parent->GetCellData(("BS_MARC.") + strAlias, nDmIdx, strData);
		}
		pDM->SetCellData( nCridIdx, i, strData);
	}

	return 0 ;
}
void CMarcEditDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}
