// ShelfLocRegDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SHELF_LOC_REGISTER.h"
#include "ShelfLocRegDlg.h"
#include "SHELF_LOC_REGISTERDlg.h"
#include "..\..\단행\열람\장서관리\\Loc_CommonAPI\LocCommonAPI.h"
#include "ShelfDataInfoDLG.h"
#include "ESL_ControlMultiComboBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShelfLocRegDlg dialog


CShelfLocRegDlg::CShelfLocRegDlg(CESL_Mgr* pParent /*=NULL*/)
: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CShelfLocRegDlg)
	//}}AFX_DATA_INIT
	this->pParent = pParent;
}

CShelfLocRegDlg::~CShelfLocRegDlg()
{
}

void CShelfLocRegDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CShelfLocRegDlg)
	DDX_Control(pDX, IDC_CHK1, m_ctrCheck);
	DDX_Control(pDX, IDC_EDT_SHELF_ROW, m_EDT_SHELFROW);
	DDX_Control(pDX, IDC_EDT_SHELF_NO, m_EDT_SHELFNO);
	DDX_Control(pDX, IDC_EDT_REG_NO, m_EDT_REGNO);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CShelfLocRegDlg, CDialog)
	//{{AFX_MSG_MAP(CShelfLocRegDlg)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_REG, OnBtnReg)
	ON_CBN_SELCHANGE(IDC_cmbREQ_MANAGE_CODE, OnSelchangecmbREQMANAGECODE)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShelfLocRegDlg message handlers

HBRUSH CShelfLocRegDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

BOOL CShelfLocRegDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();		
	
	if (InitESL_Mgr(_T("SM_SHELF_LOC_REG_SEARCH")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	dbMgr_BO = FindDM(_T("DM_SHELF_LOC_REG_BO"));
	dbMgr_SE = FindDM(_T("DM_SHELF_LOC_REG_SE"));

	CStdioFile file_stdio;
	CString strLine;
	if(file_stdio.Open(_T("..\\CFG\\서가위치정보\\DMINFO.cfg"), CStdioFile::modeReadWrite|CStdioFile::typeBinary|CStdioFile::shareDenyNone))
	{
		TCHAR cUni;
		file_stdio.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			file_stdio.SeekToBegin();
		}
		file_stdio.ReadString(strLine);
	}
	else
	{
		AfxMessageBox(_T("DB접속 정보가 잘못되었습니다."));
	}

	dbMgr_BO->SetCONNECTION_INFO(strLine);
	dbMgr_SE->SetCONNECTION_INFO(strLine);

	if(dbMgr_BO == NULL || dbMgr_SE == NULL)
	{
		AfxMessageBox(_T("DM_SHELF_LOC_BO/SE 오류"));
		return TRUE;
	}

	CESL_DataMgr	pCodeDM;
	pCodeDM.SetCONNECTION_INFO( m_pInfo->CONNECTION_INFO);
	CESL_ControlMultiComboBox* pCombo;
	pCombo = (CESL_ControlMultiComboBox*) GetDlgItem(IDC_cmbREQ_MANAGE_CODE);

	pCodeDM.RestructDataManager(_T("SELECT CODE, DESCRIPTION FROM CD_CODE_TBL WHERE CLASS=1 ORDER BY CODE"));	


	pCombo->ResetContent();
	pCombo->InitMultiComboBox(2);
	CString strData[2];
	for ( int i=0 ; i<pCodeDM.GetRowCount() ; i++ )
	{
		strData[0] = pCodeDM.GetCellData(i,0 );
		strData[1] = pCodeDM.GetCellData(i,1 );		
		pCombo->SetData(strData, 2);
	}
	pCombo->SetCurSel(0);
	m_strManageCode=pCodeDM.GetCellData(pCombo->GetCurSel(),0);
	dbMgr_BO->MANAGE_CODE=m_strManageCode;
	dbMgr_SE->MANAGE_CODE=m_strManageCode;
	((CSHELF_LOC_REGISTERDlg*)pParent)->m_strManageCode=m_strManageCode;
	

	((CButton*)GetDlgItem(IDC_CHK1))->SetCheck(TRUE);

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CShelfLocRegDlg::OnBtnReg() 
{
	// TODO: Add your control notification handler code here 

	CString strOption,strRegNO,strShelfNO,strShelfRow,strTemp;
	INT nBO_SE_FLAG=-1;//0:단행 , 1:연속 , -1:해당등록번호없음
	BOOL bShelfWrite = FALSE;

	m_EDT_REGNO.GetWindowText(strRegNO);
	m_EDT_SHELFNO.GetWindowText(strShelfNO);
	m_EDT_SHELFROW.GetWindowText(strShelfRow);

	//1.정보 누락 CHK...
	BOOL bResult = CheckEdtISBlank();
	if(!bResult)
	{
		GetDlgItem(IDC_MESSAGE)->SetWindowText(_T("서가 등록 정보가 누락 되었습니다."));
		::Beep(2000,200);
		return;
	}

	//2.등록번호 존재유무 CHK...
	nBO_SE_FLAG = CheckRegNO(strRegNO);
	if(nBO_SE_FLAG==-1)
	{
		strTemp.Format(_T("등록번호 %s이 존재하지 않습니다."),strRegNO);
		GetDlgItem(IDC_MESSAGE)->SetWindowText(strTemp);
		m_EDT_REGNO.SetWindowText(_T(""));
		::Beep(2000,200);
		return;
	}

	//3.서가번호가 이미 등록되어 있는지 CHK...
	//기록되어있다면 TRUE 반환 , 비어있으면 FALSE 반환
	bShelfWrite = CheckShelfWriteYN(nBO_SE_FLAG);

	//4.서가번호가 기록되어 있다면 서가 위치정보 DLG DoModal...
	if(bShelfWrite)
	{
		CShelfDataInfoDLG dataInfoDlg(this);
		dataInfoDlg.SetBO_SE(nBO_SE_FLAG);
		dataInfoDlg.SetModifyShelfInfo(strShelfNO,strShelfRow);
		if(IDOK == dataInfoDlg.DoModal())
		{
			CString strShelfNum_MOD;
			CString strShelfRow_MOD;
			CString strShelfNum_PRE;
			CString strShelfRow_PRE;
			dataInfoDlg.GetModifyShelfInfo(strShelfNum_MOD,strShelfRow_MOD);
			dataInfoDlg.GetPreShelfInfo(strShelfNum_PRE,strShelfRow_PRE);

			INT ids = -1;
			ids = RecordShelfInfo(nBO_SE_FLAG,strRegNO,strShelfNum_MOD,strShelfRow_MOD,strShelfNum_PRE,strShelfRow_PRE);

			if(ids!=-1)
			{
				GetDlgItem(IDC_MESSAGE)->SetWindowText(_T("서가 정보가 수정 되었습니다."));
				if(m_ctrCheck.GetCheck())
				{
					m_EDT_REGNO.SetWindowText(_T(""));
					m_EDT_REGNO.SetFocus();
				}
				else
				{
					m_EDT_REGNO.SetWindowText(_T(""));
					m_EDT_SHELFNO.SetWindowText(_T(""));
					m_EDT_SHELFROW.SetWindowText(_T(""));
				}
			}
			else
			{
				GetDlgItem(IDC_MESSAGE)->SetWindowText(_T("서가 정보 수정에 실패 하였습니다."));
				::Beep(2000,200);
			}		
		}
		else 
		{
			GetDlgItem(IDC_MESSAGE)->SetWindowText(_T("작업상태표시"));
				if(m_ctrCheck.GetCheck())
				{
					m_EDT_REGNO.SetWindowText(_T(""));
				}
				else
				{
					m_EDT_REGNO.SetWindowText(_T(""));
					m_EDT_SHELFNO.SetWindowText(_T(""));
					m_EDT_SHELFROW.SetWindowText(_T(""));
				}
		}
	}

	//5.서가번호가 Emp라면 테이블에 기록...
	else
	{
		INT ids = -1;
		ids = RecordShelfInfo(nBO_SE_FLAG,strRegNO,strShelfNO,strShelfRow,_T(""),_T(""));

		if(ids!=-1)
		{
			GetDlgItem(IDC_MESSAGE)->SetWindowText(_T("서가 정보가 등록 되었습니다."));
			if(m_ctrCheck.GetCheck())
				{
					m_EDT_REGNO.SetWindowText(_T(""));
					m_EDT_REGNO.SetFocus();
				}
				else
				{
					m_EDT_REGNO.SetWindowText(_T(""));
					m_EDT_SHELFNO.SetWindowText(_T(""));
					m_EDT_SHELFROW.SetWindowText(_T(""));
				}
		}
		else
		{
			GetDlgItem(IDC_MESSAGE)->SetWindowText(_T("서가 정보 등록에 실패 하였습니다."));
			::Beep(2000,200);
		}
	}
}

INT CShelfLocRegDlg::RecordShelfInfo(INT nBO_SE,CString strRegNO,CString strShelfNO,CString strShelfRow,CString strShelfNO_PRE,CString strShelfRow_PRE)
{
	CString strOption;
	INT ids=-1;

//	CString strValue;

//	CESL_ControlMgr *pCM = FindCM(_T("CM_SHELF_LOC_REG_SEARCH"));
//	pCM->GetControlMgrData(_T("관리구분"), strValue );
//	((CSHELF_LOC_REGISTERDlg*)pParent)->m_DM.MANAGE_CODE = strValue;

//	((CSHELF_LOC_REGISTERDlg*)pParent)->m_DM.MANAGE_CODE=_T("MA");

	((CSHELF_LOC_REGISTERDlg*)pParent)->m_DM.StartFrame();	
	
	if(nBO_SE==0)
	{
		strOption.Format(_T("UPDATE BO_BOOK_TBL SET SHELF_NUM='%s',SHELF_ROW='%s' WHERE REG_NO='%s' AND MANAGE_CODE='%s';"),strShelfNO,strShelfRow,strRegNO,m_strManageCode);
	}
	
	else if(nBO_SE==1)
	{
		strOption.Format(_T("UPDATE SE_BOOK_TBL SET SHELF_NUM='%s',SHELF_ROW='%s' WHERE REG_NO='%s'AND MANAGE_CODE='%s';"),strShelfNO,strShelfRow,strRegNO,m_strManageCode);
	}
  	((CSHELF_LOC_REGISTERDlg*)pParent)->m_DM.AddFrame(strOption);
 	ids = ((CSHELF_LOC_REGISTERDlg*)pParent)->m_DM.SendFrame();
  	((CSHELF_LOC_REGISTERDlg*)pParent)->m_DM.EndFrame();

	if(ids!=-1) WriteLOG(strRegNO,strShelfNO,strShelfRow,strShelfNO_PRE,strShelfRow_PRE);

	return ids;
}

VOID CShelfLocRegDlg::WriteLOG(CString strRegNO,
							   CString strShelfNO,CString strShelfRow,
							   CString strShelfNO_PRE,CString strShelfRow_PRE)
{
	CStdioFile file_stdio;
	CString strWriteFileName;

	//수정일 경우(strShelfNO_PRE의 값이 공백이 아닌 경우) 해당 로그파일에서 등록번호를 지운다.
	if(!strShelfNO_PRE.IsEmpty())
	{
		CString strLine;
		strWriteFileName.Format(_T("..\\cfg\\서가위치정보\\LOG\\%s+%s.dat"),strShelfNO_PRE,strShelfRow_PRE);	
		if(file_stdio.Open(strWriteFileName, CStdioFile::modeReadWrite|CStdioFile::typeBinary|CStdioFile::shareDenyNone))
		{
			TCHAR cUni;
			DWORD dwPosition;
			file_stdio.Read( &cUni, sizeof(TCHAR) );
			if( 0xFEFF != cUni )
			{
				file_stdio.SeekToBegin();
			}
			while(TRUE)
			{ 
				dwPosition = file_stdio.GetPosition();
				if(file_stdio.ReadString(strLine)==FALSE) break;
//				dwPosition = file_stdio.GetPosition();
				strLine.TrimLeft();strLine.TrimRight();
				if(strLine.GetLength==0) continue;

				if(strLine==strRegNO)
				{
					file_stdio.Seek(dwPosition,CStdioFile::begin);
					file_stdio.WriteString(_T("            \r\n"));
					break;
				}
			}
			file_stdio.Close();
		}
	}

	//새로운 서가 정보를 기록한다.
	strWriteFileName.Format(_T("..\\cfg\\서가위치정보\\LOG\\%s+%s.dat"),strShelfNO,strShelfRow);	
	if(!file_stdio.Open(strWriteFileName, CStdioFile::modeRead))
	{
		// 파일이 없으면 생성
		if(file_stdio.Open(strWriteFileName, CStdioFile::modeCreate|CStdioFile::modeWrite|CStdioFile::modeNoTruncate|CStdioFile::typeBinary))
		{
			TCHAR cUni = 0xFEFF;
			file_stdio.Write( &cUni, sizeof(TCHAR));
		}
		else
		{
			AfxMessageBox(_T("Log파일 생성에 실패했습니다."));
			return;
		}		
	}
		if(file_stdio!=NULL) file_stdio.Close();
		// 잘 열리면.... 기존에 파일이 있으면.. 뒤에 이어쓰기..
		file_stdio.Open(strWriteFileName, CStdioFile::modeCreate|CStdioFile::modeWrite|CFile::modeNoTruncate|CFile::typeBinary);
		file_stdio.SeekToEnd();
		file_stdio.WriteString(strRegNO); 
		file_stdio.WriteString(_T("\r\n")); 
		if(file_stdio!=NULL) file_stdio.Close();
}

void CShelfLocRegDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
//	((CSHELF_LOC_REGISTERDlg*)pParent)->DlgQuit();
//	CDialog::OnCancel();
}

BOOL CShelfLocRegDlg::CheckEdtISBlank()
{
	BOOL bResult=TRUE;
	CString strRegNO,strShelfNO,strShelfRow;
	m_EDT_REGNO.GetWindowText(strRegNO);
	m_EDT_SHELFNO.GetWindowText(strShelfNO);
	m_EDT_SHELFROW.GetWindowText(strShelfRow);
	if(strRegNO.IsEmpty()||strShelfNO.IsEmpty()||strShelfRow.IsEmpty())
	{
		bResult = FALSE;
	}
	return bResult;
}

INT CShelfLocRegDlg::CheckRegNO(CString strRegNO)
{
	INT nBO_SE_FLAG=-1;
	INT nResultCNT=0;
	CString strOption;

//	dbMgr_BO->MANAGE_CODE = _T("MA");
//	dbMgr_SE->MANAGE_CODE = _T("MA");

	strOption.Format(_T(" BB.REG_NO='%s' AND IB.MANAGE_CODE=UDF_MANAGE_CODE"),strRegNO);	
	dbMgr_BO->RefreshDataManager(strOption);
	
	 nResultCNT = dbMgr_BO->GetRowCount();
	 
	 if(nResultCNT > 0)
	 {
		 //단행에 자료가 존재한다면 청구기호 생성 후 0 RETURN
		CLocCommonAPI::MakeShelfCode(this,_T("DM_SHELF_LOC_REG_BO"));
		nBO_SE_FLAG = 0;
		return nBO_SE_FLAG;
	 }
	
	dbMgr_SE->RefreshDataManager(strOption);
	nResultCNT = dbMgr_SE->GetRowCount();
	if(nResultCNT > 0)
	 {
		//연속에 자료가 존재한다면 청구기호 생성 후 1 RETURN
		CLocCommonAPI::MakeShelfCode(this,_T("DM_SHELF_LOC_REG_SE"));
		nBO_SE_FLAG = 1;
		return nBO_SE_FLAG;
	 }
	//단행/연속에 해당 등록번호가 없다면 -1 RETURN
	return nBO_SE_FLAG;
}

BOOL CShelfLocRegDlg::CheckShelfWriteYN(INT nBO_SE)
{
	INT nBO_SE_FLAG = nBO_SE; //0:단행 1:연속
	BOOL bResult = FALSE;
	CString strResult;

	if(nBO_SE_FLAG==0)
	{
		dbMgr_BO->GetCellData(_T("서가번호"),0,strResult);
	}
	
	else if(nBO_SE_FLAG==1)
	{
		dbMgr_SE->GetCellData(_T("서가번호"),0,strResult);
	}

	if(!strResult.IsEmpty()) bResult = TRUE;

	return bResult;
}

BOOL CShelfLocRegDlg::PreTranslateMessage(MSG* pMsg) 
{
	if ( pMsg->message == WM_KEYDOWN )
	{
		if ( pMsg->wParam == VK_RETURN )
		{
			OnBtnReg();
			return TRUE;
		}
	}
		
	return CDialog::PreTranslateMessage(pMsg);
}



//	strOption.Format(_T(" BB.REG_NO='%s' "),strRegNO);
//	dbMgr_BO->RefreshDataManager(strOption);
	


/*
	strOption.Format(_T(" BB.REG_NO='%s' "),strRegNO);

	dbMgr_BO = FindDM(_T("DM_SHELF_LOC_REG_BO"));
	dbMgr_BO->RefreshDataManager(strOption);

	CLocCommonAPI::MakeShelfCode(this,_T("DM_SHELF_LOC_REG"));
//	CString result;
//  CString sQuery;

	INT nResultCNT = dbMgr_BO->GetRowCount();
//	dbMgr->GetCellData(_T("청구기호"),0,result);
//	dbMgr->GetCellData(_T("서명"),0,result);

	((CSHELF_LOC_REGISTERDlg*)pParent)->m_DM.StartFrame();	
  	strOption.Format(_T("UPDATE BO_BOOK_TBL SET SHELF_NUM='%s',SHELF_ROW='%s' WHERE REG_NO='%s';"),strShelfNO,strShelfRow,strRegNO);
  	((CSHELF_LOC_REGISTERDlg*)pParent)->m_DM.AddFrame(strOption);
 	((CSHELF_LOC_REGISTERDlg*)pParent)->m_DM.SendFrame();
  	((CSHELF_LOC_REGISTERDlg*)pParent)->m_DM.EndFrame();




	m_EDT_REGNO.SetWindowText(_T(""));
	m_EDT_SHELFNO.SetWindowText(_T(""));
	m_EDT_SHELFROW.SetWindowText(_T(""));
*/

void CShelfLocRegDlg::OnSelchangecmbREQMANAGECODE() 
{
	// TODO: Add your control notification handler code here
	CESL_DataMgr	pCodeDM;
	pCodeDM.SetCONNECTION_INFO( m_pInfo->CONNECTION_INFO);
	CESL_ControlMultiComboBox* pCombo;
	pCombo = (CESL_ControlMultiComboBox*) GetDlgItem(IDC_cmbREQ_MANAGE_CODE);

	pCodeDM.RestructDataManager(_T("SELECT CODE, DESCRIPTION FROM CD_CODE_TBL WHERE CLASS=1 ORDER BY CODE"));	

	m_strManageCode=pCodeDM.GetCellData(pCombo->GetCurSel(),0);
	dbMgr_BO->MANAGE_CODE=m_strManageCode;;
	dbMgr_SE->MANAGE_CODE=m_strManageCode;
	
	((CSHELF_LOC_REGISTERDlg*)pParent)->m_strManageCode=m_strManageCode;

}
