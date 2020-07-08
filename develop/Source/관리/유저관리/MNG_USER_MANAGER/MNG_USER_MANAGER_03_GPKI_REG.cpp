// MNG_USER_MANAGER_03_GPKI_REG.cpp : implementation file
//

#include "stdafx.h"
#include "MNG_USER_MANAGER_03_GPKI_REG.h"

#include <locale.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_USER_MANAGER_03_GPKI_REG dialog

#define POPUP_ITEM_UID_START	5000
#define POPUP_ITEM_UID_MID		5500
#define POPUP_ITEM_UID_END		6000

CMNG_USER_MANAGER_03_GPKI_REG::CMNG_USER_MANAGER_03_GPKI_REG(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CMNG_USER_MANAGER_03_GPKI_REG::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_USER_MANAGER_03_GPKI_REG)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_strUserUID = _T("");
	m_strUserKey = _T("");
	m_bGPKIDirty = FALSE;
	m_nMode = 0;

	m_nSelItemHDD = 0;
	m_nSelItemUSB = 0;

	m_bUseUSB = FALSE;

}


void CMNG_USER_MANAGER_03_GPKI_REG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_USER_MANAGER_03_GPKI_REG)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_USER_MANAGER_03_GPKI_REG, CDialog)
	//{{AFX_MSG_MAP(CMNG_USER_MANAGER_03_GPKI_REG)
	ON_BN_CLICKED(IDC_BUTTON_GPKI_DIR, OnButtonGpkiDir)
	ON_BN_CLICKED(IDC_BUTTON_OK, OnButtonOk)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnButtonCancel)
	ON_BN_CLICKED(IDC_BUTTON_GPKI_DEL, OnButtonGpkiDel)
	ON_WM_CONTEXTMENU()
	ON_COMMAND_RANGE(POPUP_ITEM_UID_START, POPUP_ITEM_UID_END, OnCommandRange)
	ON_BN_CLICKED(IDC_RADIO_HDD, OnRadioHdd)
	ON_BN_CLICKED(IDC_RADIO_USB, OnRadioUsb)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_USER_MANAGER_03_GPKI_REG message handlers

BOOL CMNG_USER_MANAGER_03_GPKI_REG::OnInitDialog() 
{
	CDialog::OnInitDialog();

	int nRst = 0;

	nRst = InitESL_Mgr(_T("K3_LOGIN_GPKI"));

	if(nRst == 0)
	{
		m_pDM = FindDM(_T("DM_K3_LOGIN_GPKI"));

		m_pUserDM = FindDM(_T("DM_MNG_USER_MANAGER_03"));
		
		AllControlDisplay( _T("CM_K3_LOGIN_GPKI"), 0 );
	}

	m_GPKICtrl.SetEditLog((CEdit*)GetDlgItem(IDC_EDIT_LOG));

	((CButton*)GetDlgItem(IDC_RADIO_HDD))->SetCheck(TRUE);

	OnCommandRange(POPUP_ITEM_UID_START);
	
	CertInfoLoad();

	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMNG_USER_MANAGER_03_GPKI_REG::OnButtonGpkiDir() 
{
	// TODO: Add your control notification handler code here
	BROWSEINFO brInfo;
	memset(&brInfo, 0x00, sizeof(BROWSEINFO));

	TCHAR szBuffer[512] = {0,};	

	brInfo.hwndOwner = GetSafeHwnd();

	brInfo.lpszTitle = _T("인증서가 저장된 [GPKI]폴더를 선택하세요");
	brInfo.ulFlags = BIF_NEWDIALOGSTYLE | BIF_EDITBOX | BIF_RETURNONLYFSDIRS;

	LPITEMIDLIST pItemIdList = ::SHBrowseForFolder(&brInfo);
	::SHGetPathFromIDList(pItemIdList, szBuffer);

	((CEdit*)GetDlgItem(IDC_EDIT_GPKI))->SetWindowText(szBuffer);

	CString strPath;

	strPath.Format(_T("%s"),szBuffer);

	if( strPath.Compare(_T("")) != 0)
	{
		m_CfgFileApi.SetData(_T("GPKI_Path"), strPath);
		m_pDM->FreeData();
		GetListGPKI(strPath);
		AllControlDisplay( _T("CM_K3_LOGIN_GPKI"), 0 );
	}
}


void CMNG_USER_MANAGER_03_GPKI_REG::GetListGPKI(CString strPath)
{	
	if(m_pDM)
	{
		CFileFind finder;

		CString strFind = _T("");
		strFind.Format(_T("%s\\%s"), strPath, _T("*.*"));

		BOOL bExt = finder.FindFile(strFind);

		while(bExt)
		{			
			bExt = finder.FindNextFile();		

			if(finder.IsDots())
				continue;

			if(finder.IsArchived())
			{				
				CString strFileName = finder.GetFileName();

				if( strFileName.Compare(_T("Thumbs.db")) == 0 ) 
					continue;
				
				int nPos = strFileName.ReverseFind('.');

				CString strExt = _T("");

				if(nPos > -1)
				{
					strExt = strFileName.Right(strFileName.GetLength() - nPos - 1);

					if(strExt.Compare(_T("cer")) == 0)
					{
						int nIdx = m_pDM->GetRowCount();

						CString strArrValue[5];

						if( m_GPKICtrl.VerifyCertList(finder.GetFilePath(), strArrValue, 4))
						{							
							m_pDM->InsertRow(-1);
							m_pDM->SetCellData(_T("인증서아이디"),strArrValue[0],nIdx);
							m_pDM->SetCellData(_T("만료일"),strArrValue[1],nIdx);
							m_pDM->SetCellData(_T("용도"),strArrValue[2],nIdx);
							m_pDM->SetCellData(_T("발급기관"),strArrValue[3],nIdx);
							m_pDM->SetCellData(_T("파일경로"),finder.GetFilePath(),nIdx);							
						}
					}
				}

			}			

			if(finder.IsDirectory())
			{
				GetListGPKI( finder.GetFilePath());
			}
		}

		finder.Close();
	}	
}

BOOL CMNG_USER_MANAGER_03_GPKI_REG::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class

	if(m_pDM)
		m_pDM->FreeData();

	if(m_pUserDM)
		m_pUserDM->FreeData();
	
	return CDialog::DestroyWindow();
}

void CMNG_USER_MANAGER_03_GPKI_REG::CertInfoLoad()
{
	if(!m_pUserDM)
		return;

	if(m_nMode == 0)
	{
		if(m_strUserUID.Compare(_T("")) != 0)
		{
			((CEdit*)GetDlgItem(IDC_EDIT_REG_GPKI))->SetWindowText(m_strUserUID);
			((CButton*)GetDlgItem(IDC_BUTTON_GPKI_DEL))->EnableWindow(TRUE);
		}
		else
		{
			((CButton*)GetDlgItem(IDC_BUTTON_GPKI_DEL))->EnableWindow(FALSE);
		}
	}
	else if(m_nMode == 2)
	{		
		((CButton*)GetDlgItem(IDC_BUTTON_GPKI_DEL))->EnableWindow(FALSE);		
	}
	else
	{
		CString strQuery = _T("SELECT GPKI_UID_VALUE FROM MN_USER_TBL WHERE ID = '");
		
		strQuery += m_strID;

		strQuery += _T("'");

		CString strUserUID;

		m_pUserDM->GetOneValueQuery(strQuery, strUserUID);

		if( strUserUID.Compare(_T("")) != 0)
		{
			((CEdit*)GetDlgItem(IDC_EDIT_REG_GPKI))->SetWindowText(strUserUID);
			((CButton*)GetDlgItem(IDC_BUTTON_GPKI_DEL))->EnableWindow(TRUE);			
		}
		else
		{
			((CButton*)GetDlgItem(IDC_BUTTON_GPKI_DEL))->EnableWindow(FALSE);
		}
	}
}

void CMNG_USER_MANAGER_03_GPKI_REG::OnButtonOk() 
{
	// TODO: Add your control notification handler code here

	CESL_Grid* pGrid = static_cast<CESL_Grid*>(FindControl(_T("CM_K3_LOGIN_GPKI"), _T("GRID_GPKI_LIST")));

	if(pGrid)
	{
		int nRow = pGrid->GetRow();
		CString strFile = m_pDM->GetCellData(nRow-1, 4); 

		CString strPwd = _T("");
		((CEdit*)GetDlgItem(IDC_EDIT_PWD))->GetWindowText(strPwd);

		if(strPwd.Compare(_T("")) == 0)
		{
			AfxMessageBox ( _T("인증서 암호를 입력 바랍니다."), MB_OK);
			return;
		}
		else
		{
			
			CString strUserUID = _T("");
			CString strUserKey = _T("");		

			if(m_GPKICtrl.VerifyGPKI(strFile, strPwd, strUserUID, strUserKey))
			{
				CString strQuery = _T("SELECT GPKI_UID_VALUE FROM MN_USER_TBL WHERE GPKI_KEY_VALUE = '");
				strQuery += strUserKey;
				strQuery += _T("'");
				CString strDupl = _T("");		
				m_pUserDM->GetOneValueQuery(strQuery, strDupl);

				if(strDupl.Compare(_T("")) == 0)
				{
					m_strUserUID = strUserUID;
					m_strUserKey = strUserKey;

					m_nMode = 1;

					CertInfoLoad();
					m_bGPKIDirty = TRUE;
					AfxMessageBox(_T("수정하였습니다.\n\n사용자 저장 바랍니다."), MB_OK);
					CDialog::OnOK();
				}		
				else
				{
					AfxMessageBox(_T("중복 사용자입니다.\n\n다른 인증서로 시도바랍니다."), MB_OK);
				}
			}
			
		}
	}	

}


void CMNG_USER_MANAGER_03_GPKI_REG::OnButtonCancel() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();	
}

void CMNG_USER_MANAGER_03_GPKI_REG::OnButtonGpkiDel() 
{
	// TODO: Add your control notification handler code here
	
	m_strUserUID = _T("");
	m_strUserKey = _T("");
	m_nMode = 2;
	CertInfoLoad();
	m_bGPKIDirty = TRUE;
	AfxMessageBox(_T("삭제하였습니다.\n\n사용자 저장 바랍니다."), MB_OK);
	CDialog::OnOK();
	
}

void CMNG_USER_MANAGER_03_GPKI_REG::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here

	CRect rect;

	((CButton*)GetDlgItem(IDC_RADIO_HDD))->GetWindowRect(&rect);

	if(((CButton*)GetDlgItem(IDC_RADIO_HDD))->GetCheck() && rect.PtInRect(point))
	{

		TCHAR szDirves[256] = {0,};	
		 
		int nDriveCnt = GetLogicalDriveStrings(256, szDirves);
	
		CMenu menu;

		if(menu.CreatePopupMenu())
		{
			for(int i = 0; i < nDriveCnt / 4; i++)
			{
				if( GetDriveType(szDirves+(i*4)) == DRIVE_FIXED)
				{
					menu.AppendMenu(MF_STRING | MF_ENABLED, POPUP_ITEM_UID_START + i, szDirves+(i*4));				
				}
			}					

			menu.CheckMenuRadioItem(POPUP_ITEM_UID_START, POPUP_ITEM_UID_START + nDriveCnt , POPUP_ITEM_UID_START + m_nSelItemHDD, MF_CHECKED | MF_BYCOMMAND);

			menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
		}				
	}

	((CButton*)GetDlgItem(IDC_RADIO_USB))->GetWindowRect(&rect);

	if(((CButton*)GetDlgItem(IDC_RADIO_USB))->GetCheck() && rect.PtInRect(point))
	{
		TCHAR szDirves[256] = {0,};	
		 
		int nDriveCnt = GetLogicalDriveStrings(256, szDirves);
	
		CMenu menu;

		m_bUseUSB = FALSE;
		
		if(menu.CreatePopupMenu())
		{
			for(int i = 0; i < nDriveCnt / 4; i++)
			{
				if( GetDriveType(szDirves+(i*4)) == DRIVE_REMOVABLE)
				{
					menu.AppendMenu(MF_STRING | MF_ENABLED, POPUP_ITEM_UID_MID + i, szDirves+(i*4));				
					m_bUseUSB = TRUE;
				}
			}					

			menu.CheckMenuRadioItem(POPUP_ITEM_UID_MID, POPUP_ITEM_UID_MID + nDriveCnt , POPUP_ITEM_UID_MID + m_nSelItemUSB, MF_CHECKED | MF_BYCOMMAND);

			menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
		}
	}
	
}

void CMNG_USER_MANAGER_03_GPKI_REG::OnCommandRange(UINT nCommandID)
{
	TCHAR szDirves[256] = {0,};		

	int nSel = nCommandID % 100;

	int nDriveCnt = 0;

	if(POPUP_ITEM_UID_MID > nCommandID)
	{
		m_nSelItemHDD = nSel;
	}
	else
	{
		m_nSelItemUSB = nSel;

		if(m_bUseUSB == FALSE)
		{
			m_pDM->FreeData();
			AllControlDisplay( _T("CM_K3_LOGIN_GPKI"), 0 );

			BOOL bFind = FALSE;

			nDriveCnt = GetLogicalDriveStrings(256, szDirves);

			for(int i = 0; i < nDriveCnt / 4; i++)
			{
				if( GetDriveType(szDirves+(i*4)) == DRIVE_REMOVABLE)
				{
					bFind = TRUE;
					m_nSelItemUSB = nSel = i;					
					break;
				}
			}	

			if(!bFind)
				return;
		}
	}
		 
	nDriveCnt = GetLogicalDriveStrings(256, szDirves);

	CString strDrive = _T("");

	strDrive.Format(_T("%s"), szDirves+(nSel*4));	

	strDrive += _T("GPKI");

	m_pDM->FreeData();
	GetListGPKI(strDrive);
	AllControlDisplay( _T("CM_K3_LOGIN_GPKI"), 0 );
}

void CMNG_USER_MANAGER_03_GPKI_REG::OnRadioHdd() 
{
	// TODO: Add your control notification handler code here
	OnCommandRange(POPUP_ITEM_UID_START+ m_nSelItemHDD);
}

void CMNG_USER_MANAGER_03_GPKI_REG::OnRadioUsb() 
{
	// TODO: Add your control notification handler code here
	OnCommandRange(POPUP_ITEM_UID_MID+ m_nSelItemUSB);	
}
