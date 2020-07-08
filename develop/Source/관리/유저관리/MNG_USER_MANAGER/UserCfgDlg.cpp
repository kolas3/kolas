// UserCfgDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "mng_user_manager.h"
#include "UserCfgDlg.h"
#include "PMS_UPDATE.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUserCfgDlg dialog


CUserCfgDlg::CUserCfgDlg(CESL_Mgr* pParent /*=NULL*/)
: CESL_Mgr(IDD, pParent)
{
	m_pParent = pParent;
	selectItem = _T("");
	//{{AFX_DATA_INIT(CUserCfgDlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CUserCfgDlg::~CUserCfgDlg()
{
}

void CUserCfgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUserCfgDlg)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUserCfgDlg, CDialog)
//{{AFX_MSG_MAP(CUserCfgDlg)
ON_WM_CTLCOLOR()
ON_BN_CLICKED(IDC_BTN_SAVE, OnBtnSave)
ON_BN_CLICKED(IDC_OK, OnBtnOK)
	ON_BN_CLICKED(IDC_btnPMS, OnbtnPMS)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUserCfgDlg message handlers

HBRUSH CUserCfgDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}

BOOL CUserCfgDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here	
	if( InitESL_Mgr( _T("SM_USER_PERMISION") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : SM_USER_PERMISION") );
		return false;
	}
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	pDM= FindDM( _T("DM_USER_PERMISION"));
	pCM=FindCM(_T("CM_USER_PERMISION"));
	pGrid = (CESL_Grid*)pCM->FindControl(_T("CFGGRID"));
	pGrid->SetSort(FALSE);	
	

	m_pPMSDM = FindDM( _T("DM_USER_PERMISION_INFO"));	
	pPMSGrid = (CESL_Grid*)pCM->FindControl(_T("PMS_GRID"));	

	InitScreen();
	LoadMenuInfo();

	INT nOffset;
	INT nIdx;
	nOffset = m_strAlias.Find(_T("."),0);
	nIdx = _ttoi(m_strAlias.Left(nOffset));	
	
	pPMSGrid->SetReverse(0);		
	OnClickcfgGRID2();

	
	return TRUE;
}

VOID CUserCfgDlg::LoadMenuInfo(VOID)
{
	pDM->RefreshDataManager(_T("SPF_USE_FLAG ='y' AND CAPTION IS NOT NULL ORDER BY GROUP_ALIAS"));
	
	pGrid->Display();
	CString str,temp;
	temp = GetItemAlias();
	
	for(INT i=0; i<pGrid->GetCount();i++)
	{
		pGrid->GetAt(i,0,str);
		str.Replace(_T("BO1"),_T("단행_수서대상관리"));
		str.Replace(_T("BO2"),_T("단행_구입"));
		str.Replace(_T("BO3"),_T("단행_기증"));
		str.Replace(_T("BO4"),_T("단행_등록"));
		str.Replace(_T("BO5"),_T("단행_정리"));
		str.Replace(_T("BO6"),_T("단행_열람"));
		str.Replace(_T("CO1"),_T("공통_대출/반납"));
		str.Replace(_T("C01"),_T("공통_대출/반납"));
		str.Replace(_T("CO2"),_T("공통_목차/초록"));
		str.Replace(_T("CO3"),_T("공통_전거통제"));
		str.Replace(_T("CO4"),_T("공통_분류기호검색"));
		str.Replace(_T("CO5"),_T("공통_자료찾기"));
		str.Replace(_T("CO6"),_T("공통_국가자료공동목록"));
		str.Replace(_T("CO7"),_T("공통_상호대차"));
		str.Replace(_T("CO8"),_T("공통_공지사항"));
		str.Replace(_T("CO9"),_T("공통_관리"));
		str.Replace(_T("SE1"),_T("연속_구입"));
		str.Replace(_T("SE2"),_T("연속_기증"));
		str.Replace(_T("SE3"),_T("연속_제본"));
		str.Replace(_T("SE4"),_T("연속_등록"));
		str.Replace(_T("SE5"),_T("연속_정리"));
		str.Replace(_T("SE6"),_T("연속_열람"));
		
		CString str1;
		pDM->GetCellData(i,1,str1);
		str+=_T(" → ");
		str+=str1;
		pGrid->SetTextMatrix(i+1,1,str);
		
		
		
		CString charV;
		pDM->GetCellData(temp,i,charV);
		
		if(charV == _T("T"))
		{
			pGrid->SetTextMatrix(i+1,2,_T("V"));
		}
		
		
	}
}

VOID CUserCfgDlg::InitScreen(VOID)
{
	CString strAlias;
	m_pPMSDM->RefreshDataManager(
		_T("AUTHORITY IN ( 'USER_AU_A','USER_AU_B','USER_AU_C','USER_AU_D','USER_AU_E','USER_AU_F','USER_AU_G','USER_AU_H','USER_AU_I','USER_AU_J','USER_AU_K','USER_AU_L','USER_AU_M','USER_AU_N','USER_AU_O','USER_AU_P','USER_AU_Q','USER_AU_R','USER_AU_S','USER_AU_T') ")
	);

	for ( int i=0 ; i<m_pPMSDM->GetRowCount() ; i++ )
	{
		strAlias.Format(_T("사용자 정의 권한 %d"), i+1);
		m_pPMSDM->SetCellData(_T("ALIAS"), strAlias, i);
	}

	pCM->AllControlDisplay(-1);

}

VOID CUserCfgDlg::OnBtnSave() 
{
	INT idx = AfxMessageBox(_T("변경 내용을 저장 하시겠습니까?"),MB_OKCANCEL);
	
	if(idx ==IDCANCEL)
		return;
	
	SaveAliasAndDESC();
	SaveSelectedGrid();
	
	AfxMessageBox(_T("변경 내용이 저장 되었습니다."));
}

VOID CUserCfgDlg::SaveSelectedGrid(VOID)
{
	CString temp = GetItemAlias();
	
	pDM->StartFrame();
	CString query;

	CStringArray astrList;
	CMapStringToString Map;
	CString strIsChange, strMenuID, strParentMenuID, strCheck, sCheck = _T("V");
	CString strValue;
	const INT ROW_COUNT = pDM->GetRowCount();
	for(INT i = 0; i < ROW_COUNT; i++)
	{		
		pDM->GetCellData(_T("MENU_ID"), i, strMenuID);
		strParentMenuID = strMenuID.Left(strMenuID.GetLength()-1);

		strCheck = pGrid->GetTextMatrix(i+1,2);
		if(_T("V") == strCheck)
		{
			Map.SetAt(strParentMenuID, sCheck);
			if(_T("37") == strParentMenuID.Left(2))
			{
				Map.SetAt(_T("37"), sCheck);
			}
		}
		
		pDM->GetCellData(_T("변경여부"), i, strIsChange);
		if(_T("O") == strIsChange)
		{
			if(_T("V")==strCheck)
			{
				strValue = _T("T");
			}
			else
			{
				strValue = _T("F");
			}
			query.Format(
				_T("UPDATE ESL_NEWMENU_TBL SET %s = '%s' WHERE MENU_ID = '%s';"), 
				temp, strValue, strMenuID);
			pDM->AddFrame(query);
			astrList.Add(strMenuID);
		}
	}

	const INT SIZE = astrList.GetSize();
	for(i = 0; i < SIZE; i++)
	{
		strCheck = _T("");
		strMenuID = astrList.GetAt(i);
		strParentMenuID = strMenuID.Left(strMenuID.GetLength()-1);
		Map.Lookup(strParentMenuID, strCheck);

		if(_T("V")==strCheck)
		{
			strValue = _T("T");
		}
		else
		{
			strValue = _T("F");
		}
		query.Format(
			_T("UPDATE ESL_NEWMENU_TBL SET %s = '%s' WHERE MENU_ID = '%s';"), 
			temp, strValue, strParentMenuID);
		pDM->AddFrame(query);

		if(_T("37") == strParentMenuID.Left(2))
		{
			query.Format(
				_T("UPDATE ESL_NEWMENU_TBL SET %s = '%s' WHERE MENU_ID = '371';"), 
				temp, strValue);
			pDM->AddFrame(query);
		}		
	}
	pDM->SendFrame();
	pDM->EndFrame();
}

VOID CUserCfgDlg::SaveAliasAndDESC(VOID)
{	
	
}

void CUserCfgDlg::OnBtnOK() 
{
	// TODO: Add your control notification handler code here
	selectItem = GetItemAlias();
	CDialog::OnOK();
}

CString CUserCfgDlg::GetItemAlias(VOID)
{
	CString strAlias;
	INT nIdx = pPMSGrid->GetRow()-1;
	m_pPMSDM->GetCellData(_T("AUTHORITY"), nIdx, strAlias );

	return strAlias;

}

BEGIN_EVENTSINK_MAP(CUserCfgDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CUserCfgDlg)
	ON_EVENT(CUserCfgDlg, IDC_cfgGRID2, -600 /* Click */, OnClickcfgGRID2, VTS_NONE)
	ON_EVENT(CUserCfgDlg, IDC_cfgGRID, -600 /* Click */, OnClickcfgGRID, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CUserCfgDlg::OnClickcfgGRID() 
{
	CMSHFlexGrid* pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_cfgGRID);
	INT nRowIdx = pGrid->GetRow() -1;
	INT nColIdx = pGrid->GetCol() -1;

	if(1 == nColIdx)
	{
		pDM->SetCellData(_T("변경여부"), _T("O"), nRowIdx);
	}
}
void CUserCfgDlg::OnClickcfgGRID2() 
{
	CString strAlias;
	CString strType;
	CString strMessage;
	INT nIdx = pPMSGrid->GetRow()-1;
	m_pPMSDM->GetCellData(_T("AUTHORITY"), nIdx, strAlias );
	m_pPMSDM->GetCellData(_T("ALIAS"), nIdx, strType );

	strMessage.Format(_T("%s 의 접근 가능 매뉴 설정"), strType);

	( (CStatic*) GetDlgItem (IDC_stcSEL) )->SetWindowText(strMessage);
	
	
	CString temp;
	pDM->RefreshDataManager(_T("SPF_USE_FLAG ='y'AND CAPTION IS NOT NULL ORDER BY GROUP_ALIAS"));

	for(INT i=0; i<pGrid->GetCount();i++)
	{	
		CString charV;
		pDM->GetCellData(strAlias,i,charV);
		
		if(charV == _T("T"))
		{
			pGrid->SetTextMatrix(i+1,2,_T("V"));
		}
		else pGrid->SetTextMatrix(i+1,2,_T(""));
	}
	
}

void CUserCfgDlg::OnbtnPMS() 
{
	CString strAlias;
	CString strName;
	CString strDesc;
	CString strAuthority;
	INT nIdx;

	nIdx = pPMSGrid->GetRow()-1;
	m_pPMSDM->GetCellData(_T("AUTHORITY_NAME"), nIdx, strName);
	m_pPMSDM->GetCellData(_T("DESCRIPTION"), nIdx, strDesc);
	m_pPMSDM->GetCellData(_T("AUTHORITY"), nIdx, strAuthority);
	m_pPMSDM->GetCellData(_T("ALIAS"), nIdx, strAlias);
		

	CPMS_UPDATE dlg(this);
	dlg.m_strAlias = strAlias;
	dlg.SetData(strName, strDesc);
	if ( dlg.DoModal() == IDOK )
	{		
		dlg.GetData(strName, strDesc);
		
		m_pPMSDM->SetCellData(_T("AUTHORITY_NAME"),strName , nIdx );
		m_pPMSDM->SetCellData(_T("DESCRIPTION"),strDesc , nIdx );

		pPMSGrid->SetTextMatrix(nIdx+1, 3, strName);
		pPMSGrid->SetTextMatrix(nIdx+1, 4, strDesc);
		
		CString strUpdateQuery;

		strUpdateQuery.Format(_T("UPDATE MN_PERMISION_TBL SET AUTHORITY_NAME='%s', DESCRIPTION='%s' WHERE AUTHORITY='%s';"), strName, strDesc, strAuthority);
		m_pPMSDM->StartFrame();
		m_pPMSDM->AddFrame(strUpdateQuery);		
		m_pPMSDM->SendFrame();
		m_pPMSDM->EndFrame();
	}	
}
