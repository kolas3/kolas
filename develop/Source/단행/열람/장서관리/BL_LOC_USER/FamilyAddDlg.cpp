#include "stdafx.h"
#include "FamilyAddDlg.h"
#include "..\LOC_CommonAPI\LocCommonAPI.h"
#include "FamilyPreexistenceSearchDlg.h"
#include "FamilyPreexistenceListDlg.h"

#include "..\..\..\..\공통\IndexToolKit\Index.h"

#include "PreviewImage.h"
#include "..\..\..\공통\paintlib\common\Filter\plfilterresizebilinear.h"
#include "CharConvert.h"
#include "Imm.h"

#include "BO_LOC_3000Api.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CFamilyAddDlg::CFamilyAddDlg(BL_LOC_USER_PROC* pSpfWork, CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CFamilyAddDlg::IDD, pParent)
{	
	m_pSpfWork = pSpfWork;

	m_strCMAlias = _T("CM_FAMILY_ADD_INPUT");
	m_strDMAlias = _T("DM_FAMILY_TOTAL_LIST");

	m_nDefaultMailIdx = -1;
	m_nDMIndex        = -1;

	m_bViewCivilNoCheckMsg = FALSE;
	
	m_strIsAutoSetCivilnoToPassword	=	_T("N");	
	m_strIsAutoSetUsernoToUserid	=	_T("N");	
	
	m_bAutoCivilNoInsert = FALSE;
	
	m_nAutoMode = 0;
	m_sSMSVersion = _T("");
	m_bCivilNoDisplayCheck = _T("");
	m_bCivilNoAutoCheck = 0;
	m_strLastCheckCivilNo = _T("");
	m_strLastFamilyCivilNo = _T("");
	m_bAutoCivilNoInsert = FALSE;
		
	CString m_sIsUnityLoanService;
}

CFamilyAddDlg::~CFamilyAddDlg()
{
	
	m_pSpfWork = NULL;
}

void CFamilyAddDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFamilyAddDlg) 
	//}}AFX_DATA_MAP		
}


BEGIN_MESSAGE_MAP(CFamilyAddDlg, CDialog)
	//{{AFX_MSG_MAP(CFamilyAddDlg)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BN_FAMILY_USER_NO_DUPLICATE, OnBtnFamilyUserNoDuplicate)	
	ON_BN_CLICKED(IDC_BN_FAMILY_CIVIL_NO_DUPLICATE, OnBtnFamilyCivilNoDuplicate)
	ON_BN_CLICKED(IDC_BN_FAMILY_CLEAR, OnBtnFamilyClear)	
	ON_BN_CLICKED(IDC_BN_FAMILY_LIST_DELETE, OnBtnFamilyListDelete)
	ON_BN_CLICKED(IDC_BN_FAMILY_LIST_MODIFY, OnBtnFamilyListModify)	
	ON_BN_CLICKED(IDC_BN_FAMILY_INSERT, OnBtnFamilyInsert)
	ON_BN_CLICKED(IDC_BN_FAMILY_CLOSE, OnBtnFamilyClose)	
	ON_BN_CLICKED(IDC_BN_FAMILY_ADD, OnBtnFamilyAdd)
	ON_EN_CHANGE(IDC_EDT_FAMILY_CIVIL_NO, OnChangeEdtFamilyCivilNo)
	ON_EN_KILLFOCUS(IDC_EDT_FAMILY_CIVIL_NO, OnKillfocusEdtFamilyCivilNo)
	ON_WM_CTLCOLOR()
	ON_EN_KILLFOCUS(IDC_EDIT_FAMILY_CIVILNUM1,OnKillfocusEditFamilyCivilNum1)
	ON_EN_KILLFOCUS(IDC_EDIT_FAMILY_CIVILNUM2,OnKillfocusEditFamilyCivilNum2)
	ON_BN_CLICKED(IDC_BN_FAMILY_ID_DUPLICATE, OnBnFamilyIdDuplicate)
	ON_EN_KILLFOCUS(IDC_EDT_FAMILY_USERNO, OnKillfocusEdtFamilyUserno)
	ON_EN_SETFOCUS(IDC_EDT_FAMILY_USER_NAME, OnSetfocusEdtFamilyUserName)
	ON_EN_SETFOCUS(IDC_EDT_FAMILY_USERNO, OnSetfocusEdtFamilyUserno)
	ON_EN_CHANGE(IDC_EDIT_FAMILY_CIVILNUM1, OnChangeEditFamilyCivilnum1)
	ON_EN_CHANGE(IDC_EDT_FAMILY_USERNO, OnChangeEdtFamilyUserno)
	ON_EN_CHANGE(IDC_EDT_FAMILY_PHONE2, OnChangeEdtFamilyPhone2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CFamilyAddDlg::Create(CWnd* pParentWnd) 
{
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CFamilyAddDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if( 0 > InitESL_Mgr(_T("BO_LOC_FAMILY_ADD_DLG")) )
	{
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}
	
	INT		ids = m_pSpfWork->IsAutoMode( m_nAutoMode );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("SetCtrlUserNoMode") );
	}
	
	if( 3 != m_nAutoMode )
	{		
		if(_T("Y") == m_strIsAutoSetUsernoToUserid || _T("Y") == m_strIsAutoSetCivilnoToPassword)
		{
		}
		else
		{
			SetCtrlUserNoMode();
		}
	}	
	( (CEdit*)GetDlgItem(IDC_EDIT_FAMILY_CIVILNUM1) )->SetLimitText(6);
	( (CEdit*)GetDlgItem(IDC_EDIT_FAMILY_CIVILNUM2) )->SetLimitText(7);
	
	CLocCommonAPI::MakeHPCorpNumList(GetDlgItem(IDC_CMB_FAMILY_PHONE1));
	EnableThemeDialogTexture(GetSafeHwnd()); 

	((CEdit*)GetDlgItem(IDC_EDT_FAMILY_PHONE2))->SetLimitText(4);
	((CEdit*)GetDlgItem(IDC_EDT_FAMILY_PHONE3))->SetLimitText(4);

	
	if(_T("Y")!=m_sIsUnityLoanService)
	{
		((CEdit*)GetDlgItem(IDC_EDIT_FAMILY_CIVILNUM2))->SetPasswordChar(0x2022);
	}

	return TRUE;
}

BOOL CFamilyAddDlg::PreTranslateMessage(MSG* pMsg) 
{
	return CESL_Mgr::PreTranslateMessage(pMsg);
	
}

void CFamilyAddDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);

	if( bShow )
	{
		MakeFamilyList();		

		FamilyClear();

		ControlDisplay(_T("CM_FAMILY_ADD_GRID"), _T("가족추가목록"));		
		
		SetFamilyCivilNum();
		
	}
}

void CFamilyAddDlg::OnBtnFamilyUserNoDuplicate() 
{
EFS_BEGIN

	INT ids;
	BOOL IsDuplicate;
	ids = IsDuplicateUserNo( IsDuplicate );	
	if( 0 > ids ) 
	{
		return ;
	}

	if( 0 < ids ) 
	{
		return;
	}
	
	if( IsDuplicate )
	{
		AfxMessageBox( _T("중복된 대출자 번호입니다. 다른 번호를 입력하십시오"), MB_ICONSTOP );
	}
	else
	{
		AfxMessageBox( _T("중복된 대출자 번호가 존재하지 않습니다. ") );
	}

EFS_END
}

void CFamilyAddDlg::OnBtnFamilyCivilNoDuplicate() 
{
EFS_BEGIN
	INT		ids;
	BOOL	IsDuplicate = FALSE;
	
	ids = IsDuplicateCivilNo( IsDuplicate );	
	if(0!=ids) return;
	
	if( IsDuplicate )
	{
		AfxMessageBox( _T("중복된 주민등록번호입니다. 다른 번호를 입력하십시오"), MB_ICONSTOP );
	}
	else
	{
		AfxMessageBox( _T("중복된 주민등록번호가 존재하지 않습니다. "), MB_ICONINFORMATION );
	}	
	
EFS_END
}

void CFamilyAddDlg::OnBtnFamilyClear() 
{
	FamilyClear();
}

void CFamilyAddDlg::OnBtnFamilyListDelete() 
{
	FamilyDelete();
}

void CFamilyAddDlg::OnBtnFamilyListModify() 
{
	FamilyModify();
}

void CFamilyAddDlg::OnBtnFamilyAdd() 
{
	FamilyAdd();
}

void CFamilyAddDlg::OnBtnFamilyInsert() 
{
	FamilyInsert();
}

void CFamilyAddDlg::OnBtnFamilyClose() 
{
	FamilyClose();
}

BEGIN_EVENTSINK_MAP(CFamilyAddDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CFamilyAddDlg)
	ON_EVENT(CFamilyAddDlg, IDC_GRID_FAMILY_INSERT_LIST, -600 /* Click */, OnClickGridFamilyInsertList, VTS_NONE)
	ON_EVENT(CFamilyAddDlg, IDC_GRID_FAMILY_INSERT_LIST, -601 /* DblClick */, OnDblClickGridFamilyInsertList, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CFamilyAddDlg::OnClickGridFamilyInsertList() 
{
	
}


void CFamilyAddDlg::OnDblClickGridFamilyInsertList() 
{
	if( 0 == GetRowCount(m_strDMAlias) )
	{
		return;
	}

	CESL_Grid* pGrid = GetFamilyListGrid();
	if( NULL == pGrid )
	{
		return;
	}

	INT	nRowIdx = pGrid->GetMouseRow()-1;

	if( 0 <= nRowIdx )
	{
		ShowFamilyMemberLinkDM(nRowIdx);
	}
}

INT CFamilyAddDlg::GetFamilyKey(CString &strFamilyKey)
{
EFS_BEGIN
	
	return GetControlData(_T("CM_BO_LOC_3320"), _T("가족KEY"), strFamilyKey);

EFS_END
return -1;
}

CESL_Grid* CFamilyAddDlg::GetFamilyListGrid()
{
EFS_BEGIN
	
	return (CESL_Grid*)FindControl(_T("CM_FAMILY_ADD_GRID"), _T("가족추가목록"));

EFS_END
return NULL;
}


INT	CFamilyAddDlg::SetFamilyAddMode(INT nMode)
{
EFS_BEGIN
	
	const INT nInputCtrlCnt  = 2;
	const INT nUpdateCtrlCnt = 2;
	
	INT nInputID[nInputCtrlCnt] = { IDC_BN_FAMILY_ADD , IDC_BN_FAMILY_INSERT };

	INT nUpdateID[nUpdateCtrlCnt] = { IDC_BN_FAMILY_LIST_DELETE , IDC_BN_FAMILY_LIST_MODIFY };

	INT		i;
	for( i=0 ; i<nInputCtrlCnt ; i++ )
	{
		GetDlgItem(nInputID[i])->EnableWindow(!nMode);
	}

	for( i=0 ; i<nUpdateCtrlCnt ; i++ )
	{
		GetDlgItem(nUpdateID[i])->EnableWindow(nMode);
	}

	
	if(3 != m_nAutoMode)
	{
		if(_T("Y") == m_strIsAutoSetUsernoToUserid || _T("Y") == m_strIsAutoSetCivilnoToPassword)
		{
			if(0 == nMode)
			{
				((CEdit*)GetDlgItem(IDC_EDT_FAMILY_ID))->SetReadOnly(TRUE);
			}
			else
			{
				((CEdit*)GetDlgItem(IDC_EDT_FAMILY_ID))->SetReadOnly(FALSE);
			}
		}
	}

	return 0;

EFS_END
return -1;
}

INT	CFamilyAddDlg::SetDefaultMail()
{
EFS_BEGIN

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_CMB_FAMILY_EMAIL2);

	if( -1 == m_nDefaultMailIdx )
	{
		CString defaultValue, query;
		query.Format( _T("SELECT C.DESCRIPTION FROM CD_CODE_TBL C, MN_USER_CODE_TBL U, MN_USER_TBL MU ")
					  _T("WHERE C.CLASS=50 AND U.CODE_KEY=C.REC_KEY AND U.USER_KEY=MU.REC_KEY AND MU.ID='%s' ")
					  _T("AND U.DEFAULT_VALUE='Y'"), GetUserID() );
		
		CESL_DataMgr* pDM = FindDM(m_strDMAlias);
		pDM->GetOneValueQuery(query, defaultValue);
		
		m_nDefaultMailIdx = pCombo->GetCurSel();
		INT		i, nCnt;
		nCnt	=	pCombo->GetCount();
		for( i=0 ; i<nCnt ; i++ )
		{
			pCombo->SetCurSel(i);
			pCombo->GetWindowText(query);

			if( query == defaultValue )
			{
				m_nDefaultMailIdx = i;
				return 0;
			}
		}		
		
		pCombo->SetCurSel(m_nDefaultMailIdx);	
	}
	else
	{
		pCombo->SetCurSel(m_nDefaultMailIdx);
	}

	return 0;

EFS_END
return -1;
}


INT	CFamilyAddDlg::GetFieldAlias(CString strFieldAlias[])
{
EFS_BEGIN
	
	strFieldAlias[0]  = _T("대출자번호");
	strFieldAlias[1]  = _T("대출자ID");
	strFieldAlias[2]  = _T("비밀번호");
	strFieldAlias[3]  = _T("대출자명");
	strFieldAlias[4]  = _T("IDX_대출자명");
	strFieldAlias[5]  = _T("주민등록번호");
	strFieldAlias[6]  = _T("생일");
	strFieldAlias[7]  = _T("생일TYPE");
	strFieldAlias[8]  = _T("핸드폰");
	strFieldAlias[9]  = _T("IDX_핸드폰");

	strFieldAlias[10] = _T("핸드폰1");
	strFieldAlias[11] = _T("핸드폰2");
	strFieldAlias[12] = _T("핸드폰3");
	strFieldAlias[13] = _T("E_MAIL");
	strFieldAlias[14] = _T("E_MAIL1");
	strFieldAlias[15] = _T("E_MAIL2");
	strFieldAlias[16] = _T("대출자소속");
	strFieldAlias[17] = _T("대출자직급");
	strFieldAlias[18] = _T("비고");
	strFieldAlias[19] = _T("SMS수신여부");

	strFieldAlias[20] = _T("메일수신여부");
	strFieldAlias[21] = _T("사진종류");
	strFieldAlias[22] = _T("사진경로");
	strFieldAlias[23] = _T("선정");
	strFieldAlias[24] = _T("대출자소속설명");
	strFieldAlias[25] = _T("대출자직급설명");
	strFieldAlias[26] = _T("대출자번호구분");
	strFieldAlias[27] = _T("REC_KEY");
	strFieldAlias[28] = _T("FLAG");
	strFieldAlias[29] = _T("번호");

	strFieldAlias[30] = _T("회원상태");
	strFieldAlias[31] = _T("가족KEY");
	strFieldAlias[32] = _T("가족상태");

	return 0;

EFS_END
return -1;
}

INT	CFamilyAddDlg::CMToDM(CString strDMAlias, INT nRowIndex/*=-1*/)
{
EFS_BEGIN

	INT		ids;
	CString strData;	

	CESL_ControlMgr* pCM = FindCM(m_strCMAlias);
	if( NULL == pCM ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("CMToDM") ); 
	}

	CESL_DataMgr* pDM = FindDM(strDMAlias);
	if( NULL == pDM ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("CMToDM") ); 
	}

	INT nRowCount = pDM->GetRowCount();

	if( -1 == nRowIndex )
	{
		nRowIndex = nRowCount - 1;
		
		strData.Format(_T("%d"), nRowCount);

		ids = pDM->SetCellData( _T("번호"), strData, nRowIndex);
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("CMToDM") );	
		}
		
	}
	
	if( 0 > nRowIndex && nRowCount <= nRowIndex ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1040 , _T("CMToDM") ); 
	}
	strData = MakeFamilyCivilNum();
	SetDlgItemText(IDC_EDT_FAMILY_CIVIL_NO, MakeCivilNumFormat(strData));
	
	const INT nCnt = 9;

	CString strFieldAlias[nCnt] = {
		_T("대출자ID"), _T("비밀번호"), _T("대출자명"),   _T("주민등록번호"),
		_T("생일"),		_T("생일TYPE"), _T("대출자소속"), _T("대출자직급"),   
		_T("비고")
	};

	CString strCtrlAlias[nCnt] = {
		_T("사용자ID"), _T("비밀번호"),	_T("대출자명"),		  _T("주민등록번호"),
		_T("생일"),	    _T("생일TYPE"), _T("대출자소속정보"), _T("대출자직급정보"), 
		_T("비고")
	};

	CString strGetData[nCnt];

	INT		i;
	for( i=0 ; i<nCnt ; i++ )
	{
		ids = pCM->GetControlMgrData( strCtrlAlias[i] , strGetData[i] );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1050 , _T("CMToDM") );
		}

		if( 3==i && _T("Y")==m_sIsUnityLoanService)
		{
			CString strEncCivilNo;
			this->ESL_DES_Encryption(strGetData[i],strEncCivilNo);
			ids = pDM->SetCellData( _T("주민등록번호_CRYPTO") , strEncCivilNo , nRowIndex );
			if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -1060 , _T("CMToDM") );

			CLocCommonAPI::ChangeSecuriyCivilNo(this,strGetData[i],m_sIsUnityLoanService);
		}

		ids = pDM->SetCellData(strFieldAlias[i], strGetData[i], nRowIndex);
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -1060 , _T("CMToDM") );	
	}
	
	ids = m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("대출소속정보") , strGetData[6] , strData );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1070 , _T("CMToDM") );
	}

	ids = SetDataMgrData( strDMAlias , _T("대출자소속설명") , strData , nRowIndex );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1080 , _T("CMToDM") );
	}
	
	ids = m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("대출직급정보") , strGetData[7] , strData );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1090 , _T("CMToDM") );
	}

	ids = SetDataMgrData( strDMAlias , _T("대출자직급설명") , strData , nRowIndex );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1100 , _T("CMToDM") );
	}
	
	INT nCurSelIdx = ((CComboBox*)GetDlgItem(IDC_CMB_FAMILY_INFERIOR))->GetCurSel();
	
	strData.Format(_T("%d"), nCurSelIdx);
	
	ids = pDM->SetCellData( _T("회원상태"), strData, nRowIndex );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1110 , _T("CMToDM") );	
	}
	
	if( 0 == nCurSelIdx )
	{
		strData = _T("정상회원");
	}
	else if( 1 == nCurSelIdx )
	{
		strData = _T("대출정지회원");
	}
	else if( 2 == nCurSelIdx )
	{
		strData = _T("제적회원");
	}
	else if( 3 == nCurSelIdx )
	{
		strData = _T("탈퇴회원");
	}
	else
	{
	}

	ids = pDM->SetCellData( _T("회원상태_설명"), strData, nRowIndex);
		
	CString strRecKey;

	ids = GetDataMgrData(strDMAlias, _T("REC_KEY"), strRecKey, nRowIndex);
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1120 , _T("CMToDM") ); 
	}

	if( TRUE == strRecKey.IsEmpty() )
	{
		if( 0 == m_nAutoMode )
		{
			
			ids = pCM->GetControlMgrData(_T("대출자번호"), strData);
			if( 0 > ids ) 
			{
				ERROR_MSG_RETURN_ERROR_CODE2( -1130 , _T("CMToDM") );
			}

			ids = pDM->SetCellData(_T("대출자번호"), strData, nRowIndex);
			if( 0 > ids ) 
			{
				ERROR_MSG_RETURN_ERROR_CODE2( -1140 , _T("CMToDM") );
			}
		}
		else if( 1 == m_nAutoMode )
		{
			
			ids =  pCM->GetControlMgrData(_T("주민등록번호"), strData);
			if( 0 > ids ) 
			{
				ERROR_MSG_RETURN_ERROR_CODE2( -1150 , _T("CMToDM") );	
			}
			
			strData.Replace(_T("-"), _T(""));
			strData.Replace(_T(" "), _T(""));
					
			ids = pDM->SetCellData(_T("대출자번호"), strData, nRowIndex);
			if( 0 > ids ) 
			{
				ERROR_MSG_RETURN_ERROR_CODE2( -1160 , _T("CMToDM") );
			}
		}
		else if( 2 == m_nAutoMode )
		{
			ids = pCM->GetControlMgrData(_T("이용자번호구분"), strData);
			if( 0 > ids ) 
			{
				ERROR_MSG_RETURN_ERROR_CODE2( -1170 , _T("CMToDM") );	
			}

			ids = pDM->SetCellData(_T("대출자번호구분"), strData, nRowIndex);
			if( 0 > ids ) 
			{
				ERROR_MSG_RETURN_ERROR_CODE2( -1180 , _T("CMToDM") );
			}

			ids = pDM->SetCellData( _T("대출자번호") , _T("마지막번호") , nRowIndex );
			if( 0 > ids ) 
			{
				ERROR_MSG_RETURN_ERROR_CODE2( -1190 , _T("CMToDM") );
			}
		}
		
		
		else if( 3 == m_nAutoMode )
		{
			ids = pCM->GetControlMgrData( _T("사용자ID") , strData );
			if( 0 > ids ) 
			{
				ERROR_MSG_RETURN_ERROR_CODE2( -1200 , _T("CMToDM") );
			}
			
			ids = pDM->SetCellData( _T("대출자번호") , strData , nRowIndex );
			if( 0 > ids )
			{
				ERROR_MSG_RETURN_ERROR_CODE2( -1210 , _T("CMToDM") );
			}
		}
		
	}
	else
	{
		ids = pCM->GetControlMgrData( _T("대출자번호") , strData );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1220 , _T("CMToDM") );
		}

		ids = pDM->SetCellData( _T("대출자번호") , strData , nRowIndex );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1230 , _T("CMToDM") );
		}
	}
		
	if(((CEdit*)GetDlgItem(IDC_EDT_FAMILY_ID))->IsWindowVisible())
	{
		if( _T("Y") == m_strIsAutoSetUsernoToUserid && 3 != m_nAutoMode )
		{
			CString strTemp;
			ids = pCM->GetControlMgrData( _T("대출자번호") , strTemp );
			if( 0 > ids ) 
			{
				ERROR_MSG_RETURN_ERROR_CODE2( -1050 , _T("CMToDM") );
			}

			ids = pDM->SetCellData( _T("대출자ID") , strTemp , nRowIndex );
			if( 0 > ids ) 
			{
				ERROR_MSG_RETURN_ERROR_CODE2( -1060 , _T("CMToDM") );		
			}
		}
	}
	
	CString strHandPhone1;
	CString strHandPhone2;
	CString strHandPhone3;
	
	CComboBox *pComboBox = (CComboBox*)GetDlgItem(IDC_CMB_FAMILY_PHONE1);
	if( NULL == pComboBox ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1240 , _T("CMToDM") );
	}

	pComboBox->GetWindowText(strHandPhone1);
	if( _T("적용안함") == strHandPhone1 )
	{
		strHandPhone1.Empty();
	}

	ids = pCM->GetControlMgrData( _T("핸드폰2") , strHandPhone2 );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1250 , _T("CMToDM") );
	}
		
	ids = pCM->GetControlMgrData( _T("핸드폰3") , strHandPhone3 );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1260 , _T("CMToDM") );
	}

	if( TRUE != strHandPhone1.IsEmpty() && 
		TRUE != strHandPhone2.IsEmpty() &&
		TRUE != strHandPhone3.IsEmpty() )
	{
		strData = strHandPhone1 + '-' + strHandPhone2 + '-' + strHandPhone3;

		ids = pDM->SetCellData( _T("핸드폰") , strData , nRowIndex );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1270 , _T("CMToDM") );
		}

		ids = pDM->SetCellData( _T("핸드폰1") , strHandPhone1 , nRowIndex );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1280 , _T("CMToDM") );
		}

		ids = pDM->SetCellData( _T("핸드폰2") , strHandPhone2 , nRowIndex );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1290 , _T("CMToDM") );
		}

		ids = pDM->SetCellData( _T("핸드폰3") , strHandPhone3 , nRowIndex );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1300 , _T("CMToDM") );
		}
	}
	else
	{	
		ids = pDM->SetCellData( _T("핸드폰") , _T("") , nRowIndex );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1310 , _T("CMToDM") );
		}

		ids = pDM->SetCellData( _T("핸드폰1") , _T("") , nRowIndex );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1320 , _T("CMToDM") );
		}

		ids = pDM->SetCellData( _T("핸드폰2") , _T("") , nRowIndex );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1330 , _T("CMToDM") );
		}

		ids = pDM->SetCellData( _T("핸드폰3") , _T("") , nRowIndex );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1340 , _T("CMToDM") );
		}
	}
	
	CString strMail1;
	CString strMail2;

	ids = pCM->GetControlMgrData( _T("EMAIL1") , strMail1 );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1350 , _T("CMToDM") );
	}

	CComboBox *pCmbEmail = (CComboBox*)GetDlgItem(IDC_CMB_FAMILY_EMAIL2);
	if( NULL == pCmbEmail ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1360 , _T("CMToDM") );
	}

	pCmbEmail->GetWindowText(strMail2);

	if( _T("적용안함") == strMail2 )
	{
		strMail2.Empty();
	}

	if( TRUE != strMail1.IsEmpty() && 
		TRUE != strMail2.IsEmpty() )
	{
		strData = strMail1 + '@' + strMail2;
		ids = pDM->SetCellData( _T("E_MAIL") , strData , nRowIndex );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1370 , _T("CMToDM") );
		}

		ids = pDM->SetCellData( _T("E_MAIL1" ), strMail1 , nRowIndex );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1380 , _T("CMToDM") );
		}

		ids = pDM->SetCellData( _T("E_MAIL2") , strMail2 , nRowIndex );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1390 , _T("CMToDM") );
		}
	}
	else
	{
		ids = pDM->SetCellData( _T("E_MAIL") , _T("") , nRowIndex );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1400 , _T("CMToDM") );
		}

		ids = pDM->SetCellData( _T("E_MAIL1") , _T("") , nRowIndex );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1410 , _T("CMToDM") );
		}

		ids = pDM->SetCellData( _T("E_MAIL2") , _T("") , nRowIndex );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1420 , _T("CMToDM") );
		}
	}
	
	CString strSMSCheck;
	CButton* pSMSBtn = (CButton*)GetDlgItem(IDC_chkSMSUSEYN);
	if( pSMSBtn->GetCheck() )	
	{
		strSMSCheck = 'Y';
	}
	else
	{
		strSMSCheck = 'N';
	}

	ids = pDM->SetCellData( _T("SMS수신여부") , strSMSCheck , nRowIndex );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1430 , _T("CMToDM") );
	}
	
	CString strMailCheck;
	CButton* pMailBtn = (CButton*)GetDlgItem (IDC_BN_CHECK_FAMILY_SEND_MAIL);	
	if( pMailBtn->GetCheck() )	
	{
		strMailCheck = 'Y';
	}
	else
	{
		strMailCheck = 'N';
	}

	ids = pDM->SetCellData( _T("메일수신여부") , strMailCheck , nRowIndex );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1440 , _T("CMToDM") );
	}
	
	CString strIdxHandPhone;
	CString strIdxUserName;

	ids = pDM->GetCellData( _T("핸드폰") , nRowIndex , strIdxHandPhone );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1450 , _T("CMToDM") );
	}

	ids = pDM->GetCellData( _T("대출자명") , nRowIndex , strIdxUserName );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1460 , _T("CMToDM") );
	}

	CIndex index;
	index.MakeIndex( strIdxHandPhone );
	index.MakeIndex( strIdxUserName );

	ids = pDM->SetCellData( _T("IDX_핸드폰") , strIdxHandPhone , nRowIndex );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1470 , _T("CMToDM") );
	}

	ids = pDM->SetCellData( _T("IDX_대출자명") , strIdxUserName , nRowIndex );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1480 , _T("CMToDM") );
	}
	

	
	ids = pDM->SetCellData( _T("발급횟수") , _T("1") , nRowIndex );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1490 , _T("CMToDM") );
	}
	
	CString strFamilyKey;

	ids = GetFamilyKey(strFamilyKey);
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1500 , _T("CMToDM") );
	}

	ids = pDM->SetCellData(_T("가족KEY"), strFamilyKey, nRowIndex);
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1510 , _T("CMToDM") );
	}
	

	return 0;

EFS_END
return -1;
}


INT	CFamilyAddDlg::SetScreen()
{
EFS_BEGIN

	INT		ids;

	ids	=	ShowCtrlUserNoMode();
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("SetScreen") );
	}
	
	if( _T("Y") == m_strIsAutoSetUsernoToUserid && 3 != m_nAutoMode )
	{
		ids = SetControlData( m_strCMAlias , _T("사용자ID") , _T("[대출자번호로생성]") );
		( (CEdit*)GetDlgItem(IDC_EDT_FAMILY_ID) )->SetReadOnly(TRUE);			
		GetDlgItem(IDC_STATIC_FAMILY_ID)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDT_FAMILY_ID)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BN_FAMILY_ID_DUPLICATE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_FAMILY_PASS)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDT_FAMILY_PASS)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_FAMILY_PASS2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDT_FAMILY_PASS2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_ID_GROUP)->ShowWindow(SW_SHOW);	
	}
	CString sValue;
	ids = GetManageValue(_T("기타"), _T("공통"), _T("메일링시스템"), _T("사용여부"), sValue);
	if( 0 > ids )
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("SetScreen") );
	}

	if( 0 != sValue.Compare(_T("Y")) )
	{ 
		
		((CButton*)GetDlgItem(IDC_BN_CHECK_FAMILY_SEND_MAIL))->SetCheck(0);
	}
	else
	{ 
		((CButton*)GetDlgItem(IDC_BN_CHECK_FAMILY_SEND_MAIL))->SetCheck(1);
	}
	
	((CComboBox*)(GetDlgItem(IDC_CMB_FAMILY_PHONE1)))->SetCurSel( 0 );
	SetDefaultMail();
	((CComboBox*)(GetDlgItem(IDC_EDT_FAMILY_BIRTH_TYPE)))->SetCurSel(0);
	if ( m_bCivilNoDisplayCheck != _T("Y") )
	{
		GetDlgItem(IDC_STATIC_FAMILY_CIVIL_NO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BN_FAMILY_CIVIL_NO_DUPLICATE)->ShowWindow(SW_HIDE);		
		GetDlgItem(IDC_EDIT_FAMILY_CIVILNUM1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_FAMILY_CIVILNUM2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_FAMILY_CIVILNUM)->ShowWindow(SW_HIDE);		
	}
	else
	{

		if( m_bCivilNoAutoCheck ) 
		{			
			(( CButton*)GetDlgItem(IDC_BN_FAMILY_CIVIL_NO_DUPLICATE))->ShowWindow(SW_HIDE);
		}
		else
		{
			(( CButton*)GetDlgItem(IDC_BN_FAMILY_CIVIL_NO_DUPLICATE))->ShowWindow(SW_SHOW);
		}
	}

	
	if( 0	==	m_sSMSVersion.Compare(_T("Y")) )
	{
		GetDlgItem(IDC_BN_CHECK_FAMILY_SEND_SMS)->ShowWindow(SW_SHOW);
		((CButton*)GetDlgItem(IDC_chkSMSUSEYN))->SetCheck(1);
	}
	else
	{
		GetDlgItem(IDC_BN_CHECK_FAMILY_SEND_SMS)->ShowWindow(SW_HIDE);
		((CButton*)GetDlgItem(IDC_chkSMSUSEYN))->SetCheck(0);
	}
	
	((CComboBox*)GetDlgItem(IDC_CMB_FAMILY_INFERIOR))->SetCurSel(0);
	
	CString strData;

	ids = GetControlData(_T("CM_BO_LOC_3320"), _T("소속"), strData);
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("SetScreen") );
	}
	
	ids = SetControlData(m_strCMAlias, _T("대출자소속정보"), strData);
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1040 , _T("SetScreen") );
	}

	ids = GetControlData(_T("CM_BO_LOC_3320"), _T("직급"), strData);
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1050 , _T("SetScreen") );
	}

	ids = SetControlData(m_strCMAlias, _T("대출자직급정보"), strData);
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1060 , _T("SetScreen") );
	}

	INT nResourceID[] = {IDC_EDT_FAMILY_USER_NAME,	IDC_EDIT_FAMILY_CIVILNUM1, IDC_EDIT_FAMILY_CIVILNUM2, 0};
	
	INT idx = 0;
	while (nResourceID[idx] != 0)
	{
		GetDlgItem(nResourceID[idx++])->EnableWindow(TRUE);
	}

	return 0;

EFS_END
return -1;
}

INT	CFamilyAddDlg::FamilyClear()
{
EFS_BEGIN
	GetDlgItem(IDC_EDIT_FAMILY_CIVILNUM1)->SetWindowText(_T(""));
	GetDlgItem(IDC_EDIT_FAMILY_CIVILNUM2)->SetWindowText(_T(""));
	
	CESL_ControlMgr* pCM = FindCM(m_strCMAlias);
	if( NULL == pCM ) ERROR_MSG_RETURN_ERROR_CODE2(-1, _T("FamilyClear"));

	pCM->AllControlClear();

	SetScreen();

	SetFamilyAddMode(0);

	m_nDMIndex = -1;

	return 0;

EFS_END
return -1;
}

INT CFamilyAddDlg::FamilyDelete()
{
EFS_BEGIN

	INT		ids;
	
	CString strFlag;

	ids = GetSelectedRowFlag(strFlag);
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2(-1, _T("CFamilyAddDlg::FamilyDelete"));
	INT	nRowIndex = 0;

	ids = GetSelectedRowTargetDMIndex(nRowIndex);
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2(-2, _T("CFamilyAddDlg::FamilyDelete"));

	if( _T("S") == strFlag )
	{
		CESL_DataMgr* pDM = FindDM(_T("DM_FAMILY_SEARCH_LIST"));
		if( NULL == pDM ) ERROR_MSG_RETURN_ERROR_CODE2(-3, _T("CFamilyAddDlg::FamilyDelete"));

		CString strMessage;
		CString strUserName;
		CString strUserNo;

		ids = pDM->GetCellData(_T("대출자명"), nRowIndex, strUserName);
		if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2(-4, _T("CFamilyAddDlg::FamilyDelete"));
		ids = pDM->GetCellData(_T("대출자번호"), nRowIndex, strUserNo);
		if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2(-4, _T("CFamilyAddDlg::FamilyDelete"));

		strMessage.Format(_T("'%s (%s)'를\r\n")
			              _T("가족 그룹에서 제외하시겠습니까?"), strUserName, strUserNo);

		if( IDNO == AfxMessageBox(strMessage, MB_ICONSTOP|MB_YESNO) )
		{
			AfxMessageBox(_T("취소되었습니다"), MB_ICONINFORMATION);
			return 0;
		}		

		ids = pDM->SetCellData(_T("가족KEY"), _T(""), nRowIndex);
		if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2(-4, _T("CFamilyAddDlg::FamilyDelete"));
	}	
	else if( _T("A") == strFlag )
	{
		CString strMessage;
		strMessage.Format(_T("입력중인 이용자 정보를 삭제하시겠습니까?"));

		if( IDNO == AfxMessageBox(strMessage, MB_ICONSTOP|MB_YESNO) )
		{
			AfxMessageBox(_T("취소되었습니다"), MB_ICONINFORMATION);
			return 0;
		}		

		CESL_DataMgr* pDM = FindDM(_T("DM_FAMILY_ADD_LIST"));
		if( NULL == pDM ) ERROR_MSG_RETURN_ERROR_CODE2(-31, _T("CFamilyAddDlg::FamilyDelete"));

		pDM->DeleteRow(nRowIndex);

		
		ids = ReIndexFamilyAddList();
		if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2(-41, _T("CFamilyAddDlg::FamilyDelete"));
	}
	ids = MergeFamilyList();
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2(-2, _T("CFamilyAddDlg::FamilyDelete"));

	ControlDisplay(_T("CM_FAMILY_ADD_GRID"), _T("가족추가목록"));
	
	FamilyClear();
	return 0;

EFS_END
return -1;
}

INT	CFamilyAddDlg::GetSelectedRowFlag(CString &strGetFlag)
{
EFS_BEGIN

	INT		ids;
	
	CESL_DataMgr* pDM = FindDM(_T("DM_FAMILY_TOTAL_LIST"));
	if( NULL == pDM ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("GetSelectedRowFlag") );
	}

	ids = pDM->GetCellData( _T("FLAG") , m_nDMIndex , strGetFlag );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("GetSelectedRowFlag") );
	}

	return 0;

EFS_END
return -1;
}

INT	CFamilyAddDlg::GetSelectedRowTargetDMIndex(INT &nGetDMIndex)
{
EFS_BEGIN

	INT		ids;
	
	CESL_DataMgr* pDM = FindDM(_T("DM_FAMILY_TOTAL_LIST"));
	if( NULL == pDM ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010, _T("GetSelectedRowTargetDMIndex") );
	}

	CString strData;

	ids = pDM->GetCellData(_T("번호"), m_nDMIndex, strData);
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("GetSelectedRowTargetDMIndex") );
	}

	nGetDMIndex = _ttoi(strData) -1;

	return 0;

EFS_END
return -1;
}

INT	CFamilyAddDlg::FamilyModify()
{
EFS_BEGIN
	
	INT		ids;
	ids = CheckInputValue(1);
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("FamilyModify") );
	}

	if( 0 < ids ) 
	{
		return 0;
	}
	
	ids = CheckDuplicateValue();
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("FamilyModify") );
	}

	if( 0 < ids ) 
	{
		return 0;
	}
		
	CString strFlag;

	ids = GetSelectedRowFlag(strFlag);
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("FamilyModify") );
	}
		
	CString strDMAlias;
	INT	nRowIndex = 0;

	ids = GetSelectedRowTargetDMIndex(nRowIndex);
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1040 , _T("FamilyModify") );
	}

	if( _T("S") == strFlag )
	{
		strDMAlias = _T("DM_FAMILY_SEARCH_LIST");
	}	
	else if( _T("A") == strFlag )
	{
		strDMAlias = _T("DM_FAMILY_ADD_LIST");
	}

	ids = CMToDM(strDMAlias, nRowIndex);
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1050 , _T("FamilyModify") );
	}
	
	CESL_DataMgr* pEncDM = FindDM(strDMAlias);
	
	ids = MergeFamilyList();
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1060 , _T("FamilyModify") );
	}

	ControlDisplay(_T("CM_FAMILY_ADD_GRID"), _T("가족추가목록"));
	
	FamilyClear();
	
	return 0;

EFS_END
return -1;
}

INT CFamilyAddDlg::FamilyAdd()
{
EFS_BEGIN

	INT		ids;
	CFamilyPreexistenceSearchDlg SearchDlg(this);
	
	SearchDlg.m_sIsUnityLoanService = m_sIsUnityLoanService;
	if( IDCANCEL == SearchDlg.DoModal() )
	{
		return 0;
	}
	CString strWhere;
	SearchDlg.GetSearchWhere(strWhere);

	if( strWhere.IsEmpty() )
	{
		AfxMessageBox(_T("검색 조건을 입력해야 합니다"), MB_ICONEXCLAMATION);
		return 0;
	}
	CESL_DataMgr* pSearchDM = FindDM(_T("DM_FAMILY_SEARCH_LIST"));
	if( NULL == pSearchDM ) ERROR_MSG_RETURN_ERROR_CODE2(-21, _T("CFamilyAddDlg::FamilyAdd"));

	CString strFamilyKey;
	CString strRecKey;
	CString strNotInKeyList;
	CString strInKeyList;
	
	INT nRowCount = pSearchDM->GetRowCount();
	for( INT i=0; i<nRowCount; i++ )
	{
		ids = pSearchDM->GetCellData(_T("가족KEY"), i, strFamilyKey);
		if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2(-22, _T("CFamilyAddDlg::FamilyAdd"));

		ids = pSearchDM->GetCellData(_T("REC_KEY"), i, strRecKey);
		if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2(-23, _T("CFamilyAddDlg::FamilyAdd"));

		if( strFamilyKey.IsEmpty() )
		{
			if( !strInKeyList.IsEmpty() )
			{
				strInKeyList += _T(",");
			}
			strInKeyList += strRecKey;
		}
		else
		{
			if( !strNotInKeyList.IsEmpty() )
			{
				strNotInKeyList += _T(",");
			}
			strNotInKeyList += strRecKey;
		}
	}

	if( !strNotInKeyList.IsEmpty() )
	{		
		strWhere.TrimRight(_T(") "));
		strWhere += ( _T(" AND CLU.REC_KEY NOT IN ( ") + strNotInKeyList + _T(" )) ") );
	}

	if( !strInKeyList.IsEmpty() )
	{
		strWhere.TrimRight(_T(") "));
		strWhere += ( _T(" OR CLU.REC_KEY IN ( ") + strInKeyList  + _T(" )) ") );
	}
	

	CESL_DataMgr* pDM = FindDM(_T("DM_FAMILY_PREEXISTENCE_LIST"));
	if( NULL == pDM ) ERROR_MSG_RETURN_ERROR_CODE2(-1, _T("CFamilyAddDlg::FamilyAdd"));

	ids = pDM->RefreshDataManager( strWhere );
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2(-2, _T("CFamilyAddDlg::FamilyAdd"));
	

	INT nSearchUserCnt = pDM->GetRowCount();
	if( 0 == nSearchUserCnt )
	{
		AfxMessageBox(_T("조건에 일치하는 이용자가 존재하지 않습니다"), MB_ICONEXCLAMATION);
		return 0;
	}

	
	if(TRUE == m_pInfo->m_bLibSecuUse)
	{
		CString strGetData;
		CStringArray asDes1,asDes2,asDes3,asDes4,asDes5,asDes6,asDes7,asDes8,asDes9,asDes10,asDes11;
		for(INT i = 0; i < nSearchUserCnt; i++)
		{ 
			pDM->GetCellData(_T("비밀번호"), i, strGetData);
			asDes1.Add(strGetData);
			pDM->GetCellData(_T("주민등록번호"), i, strGetData);
			asDes2.Add(strGetData);
			pDM->GetCellData(_T("핸드폰"), i, strGetData);
			asDes3.Add(strGetData);
			pDM->GetCellData(_T("IDX_핸드폰"), i, strGetData);
			asDes4.Add(strGetData);
			pDM->GetCellData(_T("E_MAIL"), i, strGetData);
			asDes5.Add(strGetData);
		}
		CBO_LOC_3000Api *p3000Api = new CBO_LOC_3000Api(this);
		if(NULL != p3000Api)
		{
			if(TRUE == p3000Api->CheckConnection())
			{
				if(TRUE == p3000Api->ConnectSocket()) 
				{
					p3000Api->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("PASSWORD"), asDes1);
					p3000Api->CloseSocket();	
				}
				if(TRUE == p3000Api->ConnectSocket()) 
				{
					p3000Api->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("CIVIL_NO"), asDes2);
					p3000Api->CloseSocket();	
				}
				if(TRUE == p3000Api->ConnectSocket()) 
				{
					p3000Api->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("HANDPHONE"), asDes3);
					p3000Api->CloseSocket();	
				}
				if(TRUE == p3000Api->ConnectSocket()) 
				{
					p3000Api->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("IDX_HANDPHONE"), asDes4);
					p3000Api->CloseSocket();	
				}
				if(TRUE == p3000Api->ConnectSocket()) 
				{
					p3000Api->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("E_MAIL"), asDes5);
					p3000Api->CloseSocket();	
				}
				for(i = 0; i < nSearchUserCnt; i++)
				{
					pDM->SetCellData(_T("비밀번호"), asDes1.GetAt(i), i);
					pDM->SetCellData(_T("주민등록번호"), asDes2.GetAt(i), i);
					pDM->SetCellData(_T("핸드폰"), asDes3.GetAt(i), i);
					pDM->SetCellData(_T("IDX_핸드폰"), asDes4.GetAt(i), i);
					pDM->SetCellData(_T("E_MAIL"), asDes5.GetAt(i), i);
				}
			}
			delete p3000Api;
			p3000Api = NULL;
		}
		asDes1.RemoveAll(); asDes1.FreeExtra();
		asDes2.RemoveAll(); asDes2.FreeExtra();
		asDes3.RemoveAll(); asDes3.FreeExtra();
		asDes4.RemoveAll(); asDes4.FreeExtra();
		asDes5.RemoveAll(); asDes5.FreeExtra();
	}

	INT nSelectRowIndex = 0;
	
	
	if( 1 < nSearchUserCnt )
	{
		CFamilyPreexistenceListDlg ListDlg(this);
		
		ListDlg.m_sIsUnityLoanService = m_sIsUnityLoanService;
		if( IDCANCEL == ListDlg.DoModal() )
		{
			return 0;
		}

		nSelectRowIndex = ListDlg.GetSelectedIndex();
	}
		
	if( 0 > nSelectRowIndex )
	{
		return 0;
	}

	ids = FamilyInsertSearchDM(nSelectRowIndex);
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2(-3, _T("CFamilyAddDlg::FamilyAdd"));
	
	
	ids = MergeFamilyList();
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2(-4, _T("CFamilyAddDlg::FamilyAdd"));

	ControlDisplay(_T("CM_FAMILY_ADD_GRID"), _T("가족추가목록"));
	
	FamilyClear();
	

	return 0;

EFS_END
return -1;
}

INT	CFamilyAddDlg::FamilyInsertSearchDM(INT nRowIndex/*=0*/)
{
EFS_BEGIN
	
	INT		ids;
	CString strData;
	
	CESL_DataMgr* pDstDM = FindDM(_T("DM_FAMILY_SEARCH_LIST"));
	if( NULL == pDstDM ) ERROR_MSG_RETURN_ERROR_CODE2(-1, _T("FamilyInsertSearchDM"));

	CESL_DataMgr* pSrcDM = FindDM(_T("DM_FAMILY_PREEXISTENCE_LIST"));
	if( NULL == pSrcDM ) ERROR_MSG_RETURN_ERROR_CODE2(-2, _T("FamilyInsertSearchDM"));
	
	INT nDstIndex = pDstDM->GetRowCount();
	pDstDM->InsertRow(-1);
	
	for( INT i = 0; i < pDstDM->m_nCols; i++ )
	{
		CString sField, sTemp;
		pDstDM->GetColumnRefInfo(i, sField, sTemp, sTemp, sTemp, sTemp);
		ids = pSrcDM->GetCellData(sField, nRowIndex, strData);
		if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2(-3, _T("FamilyInsertSearchDM"));

		ids = pDstDM->SetCellData(sField, strData, nDstIndex);
		if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2(-4, _T("FamilyInsertSearchDM"));
	}

	strData.Format(_T("%d"), nDstIndex+1);

	ids = pDstDM->SetCellData( _T("번호"), strData, nDstIndex);
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -201 , _T("CMToDM") );	
	
	CString strFamilyKey;

	ids = GetFamilyKey(strFamilyKey);
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("FamilyInsertSearchDM") );

	ids = pDstDM->SetCellData(_T("가족KEY"), strFamilyKey, nDstIndex);
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("FamilyInsertSearchDM") );
	
	ids = pDstDM->SetCellData(_T("가족상태"), _T("가족추가"), nDstIndex);
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("FamilyInsertSearchDM") );

	CString sCode, sDesc;
	
	ids = pDstDM->GetCellData(_T("대출자소속"), nDstIndex, sCode);
	if(0 > ids)
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1000, _T("FamilyInsertSearchDM"));
	}

	m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("대출소속정보"), sCode, sDesc);
	ids = pDstDM->SetCellData(_T("대출자소속설명"), sDesc, nDstIndex);
	if(0 > ids)
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1010, _T("FamilyInsertSearchDM"));
	}
	
	sDesc.Empty();
	ids = pDstDM->GetCellData(_T("대출자직급"), nDstIndex, sCode);
	if(0 > ids)
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1020, _T("FamilyInsertSearchDM"));
	}

	m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("대출직급정보"), sCode, sDesc);
	ids = pDstDM->SetCellData(_T("대출자직급설명"), sDesc, nDstIndex);
	if(0 > ids) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1030, _T("FamilyInsertSearchDM"));
	}

	return 0;

EFS_END
return -1;
}

INT CFamilyAddDlg::FamilyInsert()
{
EFS_BEGIN

	INT		ids;

	ids = CheckInputValue(0);
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("FamilyInsert") );
	}

	if( 0 < ids ) 
	{
		return 0;
	}
		
	ids = CheckDuplicateValue();
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("FamilyInsert") );
	}

	if( 0 < ids ) 
	{
		return 0;
	}
	
	ids	 =	CheckIsPhone();
	if( 0 > ids )	
	{
		return -1;
	}

	ids	 =	CheckIsEMail();
	if( 0 > ids )	
	{
		return -1;
	}
	
	ids = FamilyInsertAddDM();
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("FamilyInsert") );
	}
	
	ids = MergeFamilyList();
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1040 , _T("FamilyInsert") );
	}

	ControlDisplay( _T("CM_FAMILY_ADD_GRID") , _T("가족추가목록") );
	
	FamilyClear();
	

	return 0;

EFS_END
return -1;
}

INT	CFamilyAddDlg::FamilyInsertAddDM()
{
EFS_BEGIN
	
	INT		ids;	
	CString strDMAlias = _T("DM_FAMILY_ADD_LIST");
	
	CESL_DataMgr* pDM = FindDM(strDMAlias);
	if( NULL == pDM ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("FamilyInsertAddDM") );
	}
	
	ids = pDM->InsertRow(-1);
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("FamilyInsertAddDM") );
	}
	ids = CMToDM(strDMAlias);
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("FamilyInsertAddDM") );
	}
	

	return 0;

EFS_END
return -1;
}

INT CFamilyAddDlg::FamilyClose()
{
EFS_BEGIN
	
	OnCancel();
	return 0;

EFS_END
return -1;
}

INT CFamilyAddDlg::MakeFamilyList()
{
EFS_BEGIN

	INT		ids;
	ids = SearchFamilyList();
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2(-1, _T("CFamilyAddDlg::MakeFamilyList"));
	if( 0 < ids ) return ids;
	
	ids = MergeFamilyList();
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2(-2, _T("CFamilyAddDlg::MakeFamilyList"));
	

	return 0;

EFS_END
return -1;
}

INT CFamilyAddDlg::SearchFamilyList()
{
EFS_BEGIN

	INT		ids;
	
	CString strFamilyKey;
	ids = GetFamilyKey(strFamilyKey);
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2(-1, _T("SearchFamilyList"));
	
	if( strFamilyKey.IsEmpty() )	return 0;
	
	CString strMode;
	CString strRecKey;
	ids = GetControlData(_T("CM_BO_LOC_3320"), _T("모드"), strMode);
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2(-2, _T("SearchFamilyList"));
	if( 0 != strMode.Compare(_T("0")) )
	{
		ids = GetDataMgrData(_T("DM_BO_LOC_3300"), _T("REC_KEY"), strRecKey, 0);
		if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2(-3, _T("SearchFamilyList"));		
	}	
	
	CESL_DataMgr* pDM = FindDM(_T("DM_FAMILY_SEARCH_LIST"));
	if( NULL == pDM ) ERROR_MSG_RETURN_ERROR_CODE2(-4, _T("SearchFamilyList"));	
	
	if( 0 < pDM->GetRowCount() )
	{				
		CString strInputFamilyKey;
		ids = pDM->GetCellData(_T("가족KEY"), 0, strInputFamilyKey);
		if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2(-5, _T("SearchFamilyList"));
		
		if( 0 == strFamilyKey.Compare(strInputFamilyKey) )
			return 0;
	}

	CString strWhere;

	if( !strRecKey.IsEmpty() )
	{
		strWhere.Format(_T("CLU.FID_KEY=%s AND CLU.REC_KEY!=%s"), strFamilyKey, strRecKey);
	}
	else
	{
		strWhere.Format(_T("CLU.FID_KEY=%s"), strFamilyKey);
	}

	ids = pDM->RefreshDataManager(strWhere);
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2(-5, _T("SearchFamilyList"));

	
	CStringArray asDes1,asDes2,asDes3,asDes4,asDes5;
	
	for(INT i = 0; i < pDM->GetRowCount(); i++)
	{
		CString sCode, sDesc;
		
		ids = pDM->GetCellData(_T("대출자소속"), i, sCode);
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2(-5, _T("SearchFamilyList"));
		m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("대출소속정보"), sCode, sDesc);
		ids = pDM->SetCellData(_T("대출자소속설명"), sDesc, i);
		
		sDesc.Empty();
		ids = pDM->GetCellData(_T("대출자직급"), i, sCode);
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2(-5, _T("SearchFamilyList"));
		m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("대출직급정보"), sCode, sDesc);
		ids = pDM->SetCellData(_T("대출자직급설명"), sDesc, i);
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2(-5, _T("SearchFamilyList"));
		
		if(TRUE == m_pInfo->m_bLibSecuUse)
		{
			CString strGetData;
			pDM->GetCellData(_T("비밀번호"), i, strGetData);
			asDes1.Add(strGetData);
			pDM->GetCellData(_T("주민등록번호"), i, strGetData);
			asDes2.Add(strGetData);
			pDM->GetCellData(_T("핸드폰"), i, strGetData);
			asDes3.Add(strGetData);
			pDM->GetCellData(_T("IDX_핸드폰"), i, strGetData);
			asDes4.Add(strGetData);
			pDM->GetCellData(_T("E_MAIL"), i, strGetData);
			asDes5.Add(strGetData);
		}
	}
	
	if(TRUE == m_pInfo->m_bLibSecuUse && 0 < pDM->GetRowCount())
	{
		CBO_LOC_3000Api *p3000Api = new CBO_LOC_3000Api(this);
		if(NULL != p3000Api)
		{
			if(TRUE == p3000Api->CheckConnection())
			{
				if(TRUE == p3000Api->ConnectSocket()) 
				{
					p3000Api->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("PASSWORD"), asDes1);
					p3000Api->CloseSocket();	
				}
				if(TRUE == p3000Api->ConnectSocket()) 
				{
					p3000Api->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("CIVIL_NO"), asDes2);
					p3000Api->CloseSocket();	
				}
				if(TRUE == p3000Api->ConnectSocket()) 
				{
					p3000Api->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("HANDPHONE"), asDes3);
					p3000Api->CloseSocket();	
				}
				if(TRUE == p3000Api->ConnectSocket()) 
				{
					p3000Api->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("IDX_HANDPHONE"), asDes4);
					p3000Api->CloseSocket();	
				}
				if(TRUE == p3000Api->ConnectSocket()) 
				{
					p3000Api->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("E_MAIL"), asDes5);
					p3000Api->CloseSocket();	
				}
				for(i = 0; i < pDM->GetRowCount(); i++)
				{
					pDM->SetCellData(_T("비밀번호"), asDes1.GetAt(i), i);
					pDM->SetCellData(_T("주민등록번호"), asDes2.GetAt(i), i);
					pDM->SetCellData(_T("핸드폰"), asDes3.GetAt(i), i);
					pDM->SetCellData(_T("IDX_핸드폰"), asDes4.GetAt(i), i);
					pDM->SetCellData(_T("E_MAIL"), asDes5.GetAt(i), i);
				}
			}
			delete p3000Api;
			p3000Api = NULL;
		}
		asDes1.RemoveAll(); asDes1.FreeExtra();
		asDes2.RemoveAll(); asDes2.FreeExtra();
		asDes3.RemoveAll(); asDes3.FreeExtra();
		asDes4.RemoveAll(); asDes4.FreeExtra();
		asDes5.RemoveAll(); asDes5.FreeExtra();
	}

	return 0;

EFS_END
return -1;
}


INT	CFamilyAddDlg::ReIndexFamilyAddList()
{
EFS_BEGIN
	
	INT		ids;

	CESL_DataMgr* pDM = FindDM(_T("DM_FAMILY_ADD_LIST"));
	if( NULL == pDM ) ERROR_MSG_RETURN_ERROR_CODE2(-1, _T("CFamilyAddDlg::ReIndexFamilyAddList"));

	INT	nRowCount = pDM->GetRowCount();

	CString strData;

	for( INT i=0; i<nRowCount; i++ )
	{
		strData.Format(_T("%d"), i);

		ids = pDM->SetCellData(_T("번호"), strData, i);
		if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2(-2, _T("CFamilyAddDlg::ReIndexFamilyAddList"));
	}

	return 0;

EFS_END
return -1;
}

INT	CFamilyAddDlg::MergeFamilyList()
{
EFS_BEGIN

	INT		ids;
	
	CESL_DataMgr* pSrcSearchDM = FindDM(_T("DM_FAMILY_SEARCH_LIST"));
	if( NULL == pSrcSearchDM ) ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("MergeFamilyList") );
	
	CESL_DataMgr* pSrcAddDM = FindDM(_T("DM_FAMILY_ADD_LIST"));
	if( NULL == pSrcAddDM ) ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("MergeFamilyList") );

	CESL_DataMgr* pDstDM = FindDM(_T("DM_FAMILY_TOTAL_LIST"));
	if( NULL == pDstDM ) ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("MergeFamilyList") );

	pDstDM->FreeData();

	ids = CLocCommonAPI::AddDM(pSrcSearchDM, pDstDM);
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -1060 , _T("MergeFamilyList") );

	ids = CLocCommonAPI::AddDM(pSrcAddDM, pDstDM);
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -1070 , _T("MergeFamilyList") );
		
	CString strUserNoCivilNoCheck;
	this->GetManageValue(	_T("열람"), _T("공통"), _T("이용자관리"), 
							_T("대출화면주민번호표시유무"), strUserNoCivilNoCheck );
	strUserNoCivilNoCheck.TrimLeft();	strUserNoCivilNoCheck.TrimRight();
	strUserNoCivilNoCheck.MakeUpper();
	
	CString strData;
	INT		nRowCount = pDstDM->GetRowCount();

	for( INT i=nRowCount; i>0; i-- )
	{				
		ids = pDstDM->GetCellData(_T("가족KEY"), i-1, strData);
		if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -1080 , _T("MergeFamilyList") );

		if( strData.IsEmpty() )
		{
			ids = pDstDM->DeleteRow(i-1);
			if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -1090 , _T("MergeFamilyList") );

			continue;
		}

		if( _T("Y") != strUserNoCivilNoCheck )
		{
			ids = pDstDM->GetCellData(_T("대출자번호"), i-1, strData);
			if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -1100 , _T("MergeFamilyList") );

			CLocCommonAPI::ChangeSecuriyUserNo( this , strData , m_sIsUnityLoanService );
			ids = pDstDM->SetCellData(_T("대출자번호"), strData , i-1 );
			if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -1130 , _T("MergeFamilyList") );
		}
	}
	

	return 0;

EFS_END
return -1;
}

INT	CFamilyAddDlg::ShowFamilyMemberLinkDM(INT nIndex)
{
EFS_BEGIN
	
	INT		ids;
	CString strData;

	CString strHandPhone1;
	CString strHandPhone2;
	CString strHandPhone3;

	ids = GetDataMgrData(m_strDMAlias, _T("핸드폰1"), strHandPhone1, nIndex);
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2(-31, _T("CFamilyAddDlg::ShowFamilyMemberLinkDM"));

	ids = GetDataMgrData(m_strDMAlias, _T("핸드폰2"), strHandPhone2, nIndex);
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2(-32, _T("CFamilyAddDlg::ShowFamilyMemberLinkDM"));

	ids = GetDataMgrData(m_strDMAlias, _T("핸드폰3"), strHandPhone3, nIndex);
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2(-33, _T("CFamilyAddDlg::ShowFamilyMemberLinkDM"));

	if( strHandPhone1.IsEmpty() ||
		strHandPhone2.IsEmpty() ||
		strHandPhone3.IsEmpty() )
	{
		ids = GetDataMgrData(m_strDMAlias, _T("핸드폰"), strData, nIndex);
		if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2(-31, _T("CFamilyAddDlg::ShowFamilyMemberLinkDM"));	

		if( !strData.IsEmpty() )
		{
			CStringArray PhoneArray;

			CLocCommonAPI::MakeArrayList(strData , "-" , PhoneArray);

			if( 3 == PhoneArray.GetSize() )
			{
				ids = SetDataMgrData(m_strDMAlias, _T("핸드폰1"), PhoneArray.GetAt(0), nIndex);
				if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2(-32, _T("CFamilyAddDlg::ShowFamilyMemberLinkDM"));	

				ids = SetDataMgrData(m_strDMAlias, _T("핸드폰2"), PhoneArray.GetAt(1), nIndex);
				if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2(-32, _T("CFamilyAddDlg::ShowFamilyMemberLinkDM"));	

				ids = SetDataMgrData(m_strDMAlias, _T("핸드폰3"), PhoneArray.GetAt(2), nIndex);
				if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2(-32, _T("CFamilyAddDlg::ShowFamilyMemberLinkDM"));
			}
		}
	}

	CString strEmail1;
	CString strEmail2;

	ids = GetDataMgrData(m_strDMAlias, _T("E_MAIL1"), strEmail1, nIndex);
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2(-41, _T("CFamilyAddDlg::ShowFamilyMemberLinkDM"));

	ids = GetDataMgrData(m_strDMAlias, _T("E_MAIL2"), strEmail2, nIndex);
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2(-41, _T("CFamilyAddDlg::ShowFamilyMemberLinkDM"));

	if( strEmail1.IsEmpty() ||
		strEmail2.IsEmpty() )
	{
		ids = GetDataMgrData(m_strDMAlias, _T("E_MAIL"), strData, nIndex);
		if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2(-41, _T("CFamilyAddDlg::ShowFamilyMemberLinkDM"));

		if( !strData.IsEmpty() )
		{
			CStringArray EmailArray;

			CLocCommonAPI::MakeArrayList(strData , "@" , EmailArray);

			if( 2 == EmailArray.GetSize() )
			{
				ids = SetDataMgrData(m_strDMAlias, _T("E_MAIL1"), EmailArray.GetAt(0), nIndex);
				if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2(-32, _T("CFamilyAddDlg::ShowFamilyMemberLinkDM"));	

				ids = SetDataMgrData(m_strDMAlias, _T("E_MAIL2"), EmailArray.GetAt(1), nIndex);
				if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2(-32, _T("CFamilyAddDlg::ShowFamilyMemberLinkDM"));
			}

		}
	}
	

 	if( TRUE == GetDlgItem(IDC_EDT_FAMILY_ID)->IsWindowVisible() )
  	{	
 		((CEdit*)GetDlgItem(IDC_EDT_FAMILY_PASS))->SetRedraw(FALSE);
 		((CEdit*)GetDlgItem(IDC_EDT_FAMILY_PASS2))->SetRedraw(FALSE);
  	}

	ids = AllControlDisplay(m_strCMAlias, nIndex);
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2(-1, _T("CFamilyAddDlg::ShowFamilyMemberLinkDM"));

 	if( TRUE == GetDlgItem(IDC_EDT_FAMILY_ID)->IsWindowVisible() )
  	{
 		((CEdit*)GetDlgItem(IDC_EDT_FAMILY_PASS))->SetPasswordChar(0x2022);
 		((CEdit*)GetDlgItem(IDC_EDT_FAMILY_PASS2))->SetPasswordChar(0x2022);
 		((CEdit*)GetDlgItem(IDC_EDT_FAMILY_PASS))->SetRedraw(TRUE);
 		((CEdit*)GetDlgItem(IDC_EDT_FAMILY_PASS2))->SetRedraw(TRUE);
  	}
	
	
	
	ids = GetDataMgrData(m_strDMAlias, _T("생일TYPE"), strData, nIndex);
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2(-2, _T("CFamilyAddDlg::ShowFamilyMemberLinkDM"));
	
	CComboBox* pBox = (CComboBox*)GetDlgItem(IDC_EDT_FAMILY_BIRTH_TYPE);
	if( NULL == pBox ) ERROR_MSG_RETURN_ERROR_CODE2(-21, _T("CFamilyAddDlg::ShowFamilyMemberLinkDM"));	
	if( _T("+") == strData )	pBox->SetCurSel(0);
	else						pBox->SetCurSel(1);
		
	ids = GetDataMgrData(m_strDMAlias, _T("핸드폰1"), strData, nIndex);
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2(-3, _T("CFamilyAddDlg::ShowFamilyMemberLinkDM"));

	pBox = (CComboBox*)GetDlgItem(IDC_CMB_FAMILY_PHONE1);
	if( NULL == pBox ) ERROR_MSG_RETURN_ERROR_CODE2(-31, _T("CFamilyAddDlg::ShowFamilyMemberLinkDM"));
	ids = CLocCommonAPI::SetPhoneNumCel( pBox, strData );
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2(-32, _T("CFamilyAddDlg::ShowFamilyMemberLinkDM"));
	
	ids = GetDataMgrData(m_strDMAlias, _T("E_MAIL2"), strData, nIndex);
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2(-4 , _T("CFamilyAddDlg::ShowDMLinkFamilyMember"));

	pBox = (CComboBox*)(GetDlgItem(IDC_CMB_FAMILY_EMAIL2));
	if( NULL == pBox ) ERROR_MSG_RETURN_ERROR_CODE2(-41 , _T("CFamilyAddDlg::ShowDMLinkFamilyMember"));

	CString strBoxData;
	INT nBoxCount = pBox->GetCount();
	for( INT i=0; i<nBoxCount; i++ )
	{
		pBox->GetLBText(i, strBoxData);
		if( 0 == strBoxData.Compare(strData) )
			break;
	}

	if( i == nBoxCount )
	{
		pBox->AddString(strData);
	}
	pBox->SetWindowText(strData);

	ids = GetDataMgrData(m_strDMAlias, _T("SMS수신여부"), strData, nIndex);
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2(-5, _T("CFamilyAddDlg::ShowDMLinkFamilyMember"));
	
	CButton* pBtn = (CButton*)GetDlgItem(IDC_BN_CHECK_FAMILY_SEND_SMS);
	if( NULL == pBtn ) ERROR_MSG_RETURN_ERROR_CODE2(-51, _T("CFamilyAddDlg::ShowDMLinkFamilyMember"));
	
	if( _T("Y") == strData )	pBtn->SetCheck(1);	
	else						pBtn->SetCheck(0);
	
	ids = GetDataMgrData(m_strDMAlias, _T("메일수신여부"), strData, nIndex);
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2(-6, _T("CFamilyAddDlg::ShowDMLinkFamilyMember"));
	
	pBtn = (CButton*)GetDlgItem(IDC_BN_CHECK_FAMILY_SEND_MAIL);
	if( NULL == pBtn ) ERROR_MSG_RETURN_ERROR_CODE2(-61, _T("CFamilyAddDlg::ShowDMLinkFamilyMember"));		

	if( _T("Y") == strData )	pBtn->SetCheck(1);
	else						pBtn->SetCheck(0);
	
	ids = GetDataMgrData(m_strDMAlias, _T("회원상태"), strData, nIndex);
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2(-7, _T("CFamilyAddDlg::ShowDMLinkFamilyMember"));
	
	((CComboBox*)GetDlgItem(IDC_CMB_FAMILY_INFERIOR))->SetCurSel(_ttoi(strData));
	
	m_bAutoCivilNoInsert = TRUE;
	SetFamilyCivilNum();
	m_bAutoCivilNoInsert = FALSE;
	
	ids = SetFamilyAddMode(1);
	
	m_nDMIndex = nIndex;
	
	((CEdit*)GetDlgItem(IDC_EDT_FAMILY_PASS))->Invalidate();
 	((CEdit*)GetDlgItem(IDC_EDT_FAMILY_PASS2))->Invalidate();

	ids = GetDataMgrData(m_strDMAlias, _T("실명인증코드"), strData, nIndex);
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2(-7, _T("CFamilyAddDlg::ShowDMLinkFamilyMember"));
	
	INT nResourceID[] = {IDC_EDT_FAMILY_USER_NAME,	IDC_EDIT_FAMILY_CIVILNUM1, IDC_EDIT_FAMILY_CIVILNUM2, 0};
	
	INT idx = 0;
	while (nResourceID[idx] != 0)
	{
		GetDlgItem(nResourceID[idx++])->EnableWindow(strData.IsEmpty());
	}

	return 0;

EFS_END
return -1;
}

INT	CFamilyAddDlg::CheckInputValue( INT nMode )
{
EFS_BEGIN

	INT		ids;
	CString strData;

	
	ids = GetControlData( m_strCMAlias, _T("대출자명"), strData );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("CheckInputValue") ); 	
	}

	if( strData.IsEmpty() )
	{
		AfxMessageBox(_T("이름을 먼저 입력해주십시오"), MB_ICONINFORMATION);		
		GetDlgItem(IDC_EDT_FAMILY_USER_NAME)->SetFocus();
		return 1;
	}

	
	if( _T("Y") == m_bCivilNoDisplayCheck )
	{
		ids = GetControlData( m_strCMAlias, _T("주민등록번호"), strData );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("CheckInputValue") ); 	
		}

		if( strData.IsEmpty() )
		{
			AfxMessageBox(_T("주민등록번호를 먼저 입력해주십시오"), MB_ICONINFORMATION);		
			GetDlgItem(IDC_EDIT_FAMILY_CIVILNUM1)->SetFocus();
			return 1;
		}

		
		CButton* pBtn = (CButton*)pParentMgr->GetDlgItem(IDC_btnCHECK_CIVIL_NO);
		if( NULL == pBtn ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("CheckInputValue") ); 
		}

		if( pBtn->GetCheck() )
		{
			if( 14 != strData.GetLength() && 6 < strData.GetLength() )
			{
				CString strForWord  = strData.Mid(0,6);
				CString strBackWord = strData.Mid(6);

				strData = strForWord + _T("-") + strBackWord;
			}

			
			if( 1==nMode && _T("Y")==m_sIsUnityLoanService)
			{
				CString strDMCivilNo;
				ids = GetDataMgrData(m_strDMAlias, _T("주민등록번호"), strDMCivilNo, m_nDMIndex);
				if(strDMCivilNo==strData)
				{
					ids = GetDataMgrData(m_strDMAlias, _T("주민등록번호_CRYPTO"), strDMCivilNo, m_nDMIndex);
					this->ESL_DES_Decryption(strDMCivilNo,strData);
				}
			}

			ids = CLocCommonAPI::CivilNoCheck( strData );
			if( 0 < ids )
			{
				AfxMessageBox(_T("잘못된 주민등록번호입니다. \r\n올바른 주민등록번호를 입력해주십시오"), MB_ICONINFORMATION);
				GetDlgItem(IDC_EDIT_FAMILY_CIVILNUM1)->SetFocus();
				return 1;
			}
		}
	}

	
	ids = GetControlData( m_strCMAlias, _T("대출자소속정보"), strData );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1040 , _T("CheckInputValue") ); 	
	}

	if( strData.IsEmpty() )
	{
		AfxMessageBox(_T("소속을 먼저 입력해주십시오"), MB_ICONINFORMATION);		
		GetDlgItem(IDC_CMB_FAMILY_POSITION)->SetFocus();
		return 1;
	}

	ids = GetControlData( m_strCMAlias, _T("대출자직급정보"), strData );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1050 , _T("CheckInputValue") ); 	
	}

	if( strData.IsEmpty() )
	{
		AfxMessageBox(_T("대출자직급을 먼저 입력해주십시오"), MB_ICONINFORMATION);		
		GetDlgItem(IDC_CMB_FAMILY_CLASS)->SetFocus();
		return 1;
	}

	
	if( 3 == m_nAutoMode )
	{
		ids = GetControlData( m_strCMAlias, _T("사용자ID"), strData );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1060 , _T("CheckInputValue") );
		}

		if( TRUE == strData.IsEmpty() )
		{
			AfxMessageBox( _T("사용자ID를 입력해주십시오") , MB_ICONINFORMATION );
			GetDlgItem(IDC_EDT_FAMILY_ID)->SetFocus();
			return 1;
		}	
	}

	
	ids = GetControlData( m_strCMAlias, _T("사용자ID"), strData );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1070 , _T("CheckInputValue") ); 	
	}

	if( TRUE != strData.IsEmpty() )
	{
		ids = GetControlData( m_strCMAlias, _T("비밀번호"), strData );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1080 , _T("CheckInputValue") ); 	
		}
		
		if( TRUE == strData.IsEmpty() )
		{
			AfxMessageBox(_T("비밀번호를 먼저 입력해주십시오"), MB_ICONINFORMATION);		
			GetDlgItem(IDC_EDT_FAMILY_PASSWORD1)->SetFocus();
			return 1;
		}
		
		CString strPassWord;
		ids = GetControlData( m_strCMAlias, _T("비밀번호확인"), strPassWord );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1090 , _T("CheckInputValue") ); 	
		}
		
		if( TRUE == strPassWord.IsEmpty() )
		{
			AfxMessageBox(_T("비밀번호확인을 먼저 입력해주십시오"), MB_ICONINFORMATION);		
			GetDlgItem(IDC_EDT_FAMILY_PASSWORD2)->SetFocus();
			return 1;
		}

		if( strPassWord != strData )
		{			
			AfxMessageBox(_T("비밀번호와 비밀번호 확인이 다릅니다. 다시 입력해주십시오"), MB_ICONINFORMATION);		
			GetDlgItem(IDC_EDT_FAMILY_PASSWORD1)->SetFocus();
			return 1;
		}
	}

	
	ids = GetControlData( m_strCMAlias, _T("대출자번호"), strData );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1100 , _T("CheckInputValue") ); 	
	}

	if( TRUE == strData.IsEmpty() )
	{
		AfxMessageBox(_T("대출자번호를 먼저 입력해주십시오"), MB_ICONINFORMATION);		
		GetDlgItem(IDC_EDT_FAMILY_USERNO)->SetFocus();
		return 1;
	}

	
	if( 2 == m_nAutoMode )
	{
		ids = GetControlData( m_strCMAlias , _T("이용자번호구분") , strData );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1110 , _T("CheckInputValue") ); 	
		}

		if ( TRUE == strData.IsEmpty() )
		{
			AfxMessageBox( _T("이용자번호구분이 선택되지 않았습니다.") , MB_ICONINFORMATION );
			return 1;
		}
	}

	
	ids = GetControlData( m_strCMAlias, _T("생일"), strData );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1120 , _T("CheckInputValue") ); 	
	}

	
	if( TRUE != strData.IsEmpty() )
	{
		INT		nYear;
		INT		nMonth;
		INT		nDay;		
		BOOL	bVaildDate = FALSE;

		_stscanf( strData.GetBuffer(0), _T("%d/%d/%d"), &nYear, &nMonth, &nDay );

		
		
		if( 100 <= nYear && 4712 >= nYear )
		{
			if( 1 <= nMonth && 12 >= nMonth )
			{
				
				INT nMonthDays[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

				
				
				if( !(nYear % 400) || ((nYear % 100) && !(nYear % 4)) )
				{
					
					nMonthDays[1]++;
				}

				if( 1 <= nDay && nMonthDays[nMonth-1] >= nDay )
				{
					
					bVaildDate = TRUE;
				}
			}
		}

		if( TRUE != bVaildDate )
		{
			AfxMessageBox(_T("정확한 생일을 입력해주십시오"), MB_ICONINFORMATION);		
			GetDlgItem(IDC_EDT_FAMILY_BIRTH)->SetFocus();
			return 1;
		}
	}

	return 0;

EFS_END
return -1;
}

INT	CFamilyAddDlg::CheckDuplicateValue()
{
EFS_BEGIN

	INT		ids;
	BOOL    bDuplicate = FALSE;

	
	
	ids = IsDuplicateUserNo(bDuplicate);
	if( 0 > ids )	
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("CheckDuplicateValue") );
	}

	if( TRUE == bDuplicate )
	{
		AfxMessageBox( _T("중복된 대출자번호입니다. 다른 대출자번호를 입력하십시오"), MB_ICONSTOP );
		return 1;
	}
	
	if( _T("Y") == m_bCivilNoDisplayCheck )
	{ 
		CButton* pBtn = (CButton*)pParentMgr->GetDlgItem(IDC_btnCHECK_CIVIL_NO2);
		if( NULL == pBtn ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("CheckDuplicateValue") );
		}

		if( pBtn->GetCheck() )
		{
			ids = IsDuplicateCivilNo( bDuplicate );
			if(0!=ids) return ids;
			
			if( TRUE == bDuplicate )
			{
				AfxMessageBox( _T("중복된 주민등록번호입니다. 다른 주민등록번호를 입력하십시오"), MB_ICONSTOP );
				return 1;
			}
		}
	}

	ids = IsDuplicateUserID( bDuplicate );
	if( 0 > ids )	
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("CheckDuplicateValue") );
	}

	if( TRUE == bDuplicate )
	{
		AfxMessageBox( _T("중복된 사용자ID입니다. 다른 사용자ID를 입력하십시오"), MB_ICONSTOP );
		
		return 1;
	}
	

	return 0;

EFS_END
return -1;
}

INT	CFamilyAddDlg::IsDuplicateUserNo(BOOL &bDuplicate)
{
EFS_BEGIN

	INT		ids;
	bDuplicate = FALSE;
	
	CString sSql, sResult, sBeforeUserNo, sAfterUserNo;
	CESL_DataMgr *pDM = FindDM( m_strDMAlias );
	if( NULL == pDM ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010, _T("IsDuplicateUserNo") );
	}

	ids = GetControlData( m_strCMAlias, _T("대출자번호"), sAfterUserNo );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1020, _T("IsDuplicateUserNo") );
	}

	if( TRUE == sAfterUserNo.IsEmpty() )
	{
		AfxMessageBox( _T("대출자번호를 입력하시고 중복조회 기능을 사용하십시오") );
		bDuplicate = FALSE;
		return 1;
	}

	if( -1 == m_nDMIndex )
	{ 
		if( 1 == m_nAutoMode )
		{ 
			sAfterUserNo = MakeFamilyCivilNum();
			sAfterUserNo.Replace( _T(" "), _T("") );
			if( TRUE == sAfterUserNo.IsEmpty() && _T("Y") == m_bCivilNoDisplayCheck )
			{
				AfxMessageBox( _T("주민등록번호를 입력하여 주십시오") );
				bDuplicate = FALSE;
				return 2;
			}
		}
		else if( 2 == m_nAutoMode )
		{ 
			CString sValue;
			ids = GetControlData( m_strCMAlias, _T("이용자번호구분"), sValue );
			if( 0 > ids ) 
			{
				ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("IsDuplicateUserNo") );			
			}

			if ( TRUE == sValue.IsEmpty() )
			{
				AfxMessageBox( _T("번호구분을 선택하여 주십시오") );
				bDuplicate = FALSE;
				return 3;
			}

			ids = m_pSpfWork->GetAutoUserNo( sAfterUserNo , sValue , _T("") , FALSE );
			if( 0 > ids ) 
			{
				ERROR_MSG_RETURN_ERROR_CODE2( -1040, _T("IsDuplicateUserNo") );
			}
		}
		else if( 3 == m_nAutoMode )
		{ 
			CEdit *pEdt = (CEdit*)GetDlgItem(IDC_EDT_FAMILY_ID);
			pEdt->GetWindowText( sAfterUserNo );
			if( TRUE == sAfterUserNo.IsEmpty() && _T("Y") == m_bCivilNoDisplayCheck )
			{
				AfxMessageBox( _T("사용자ID를 입력하여 주십시오") );
				bDuplicate = FALSE;
				return 4;
			}
		}
		else
		{ 
			
		}

		sSql.Format( _T("SELECT COUNT(*) FROM CO_LOAN_USER_TBL WHERE USER_NO='%s'"), sAfterUserNo );
		ids = CLocCommonAPI::GetOneValueQuery( this, pDM, sSql, sResult );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1050 , _T("IsDuplicateUserNo") );
		}

		if( 0 < _ttoi(sResult) ) 
		{
			bDuplicate = TRUE;
			return 0;
		}
	}
	else
	{ 
		ids = pDM->GetCellData( _T("대출자번호") , m_nDMIndex , sBeforeUserNo );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1060 , _T("IsDuplicateUserNo") );
		}

		
		CString strFlag;
		ids = GetSelectedRowFlag(strFlag);
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("FamilyModify") );
		}

		
		if( (sBeforeUserNo == sAfterUserNo) && (_T("S") == strFlag) )
		{
			
			CString sRecKey;
			ids = pDM->GetCellData(_T("REC_KEY"), m_nDMIndex, sRecKey);
			if( 0 > ids ) 
			{
				ERROR_MSG_RETURN_ERROR_CODE2( -1070 , _T("IsDuplicateUserNo") );
			}

			sSql.Format( _T("SELECT COUNT(*) FROM CO_LOAN_USER_TBL ")
						 _T("WHERE USER_NO='%s' AND REC_KEY NOT IN(%s)"), sAfterUserNo, sRecKey );
			
			
			ids = CLocCommonAPI::GetOneValueQuery( this, pDM, sSql, sResult );
			if( 0 > ids )
			{
				ERROR_MSG_RETURN_ERROR_CODE2( -1080 , _T("IsDuplicateUserNo") );
			}
		}
		else
		{
			sSql.Format( _T("SELECT COUNT(*) FROM CO_LOAN_USER_TBL WHERE USER_NO='%s'"), sAfterUserNo );
			ids = CLocCommonAPI::GetOneValueQuery( this, pDM, sSql, sResult );
			if( 0 > ids ) 
			{
				ERROR_MSG_RETURN_ERROR_CODE2( -1090 , _T("IsDuplicateUserNo") );
			}
		}

		if( 0 < _ttoi(sResult) ) 
		{
			bDuplicate = TRUE;
			return 0;
		}
	}
	
	ids = IsDuplicateDM(_T("대출자번호"), sAfterUserNo, bDuplicate);
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1100, _T("IsDuplicateUserNo") ); 
	}

	return 0;

EFS_END
return -1;
}

INT	CFamilyAddDlg::IsDuplicateCivilNo(BOOL &bDuplicate)
{
EFS_BEGIN

	INT		ids;

	bDuplicate = FALSE;
	
	
	CString strCivilNo;
	
	strCivilNo = MakeFamilyCivilNum();
	if( strCivilNo.IsEmpty() )
 	{
 		return 100;
 	}

	strCivilNo.Replace(_T("-"),_T(""));

	CString strHalfLineCivilNo = strCivilNo;
	
	if( 6 < strHalfLineCivilNo.GetLength() )
	{
		strHalfLineCivilNo = strHalfLineCivilNo.Mid(0,6) + _T("-") + strHalfLineCivilNo.Mid(6);
	}
		
	CString strRecKey;

	if( -1 != m_nDMIndex )
	{
		ids = GetDataMgrData(m_strDMAlias, _T("REC_KEY"), strRecKey, m_nDMIndex);
		if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CFamilyAddDlg::IsDuplicateUserNo") ); 	
	}
	
	CString strQuery;
	
	if( strRecKey.IsEmpty() )
	{
		strQuery.Format( _T("SELECT COUNT(1) FROM CO_LOAN_USER_TBL ")
									   _T(" WHERE CIVIL_NO IN ('%s','%s')")
       									 _T(" AND USER_NO IS NOT NULL")
										 _T(" AND (USER_CLASS!='3' OR MEMBER_CLASS!='1')")
										 , strCivilNo, strHalfLineCivilNo );
	}
	else
	{
		strQuery.Format( _T("SELECT COUNT(1) FROM CO_LOAN_USER_TBL ")
									   _T(" WHERE CIVIL_NO IN ('%s','%s')")
       									 _T(" AND USER_NO IS NOT NULL")
										 _T(" AND REC_KEY!=%s")
										 _T(" AND (USER_CLASS!='3' OR MEMBER_CLASS!='1')")
										 , strCivilNo, strHalfLineCivilNo, strRecKey );
	}

	
	if(_T("Y")==m_sIsUnityLoanService)
	{
 		if(13!=strCivilNo.GetLength())
		{
			AfxMessageBox(_T("잘못된 주민등록번호입니다.\r\n")
						  _T("올바른 주민등록번호를 입력해주십시오"));
			return 1010;
		}
		
		if( strRecKey.IsEmpty() )
		{
			this->ESL_DES_Encryption(strHalfLineCivilNo,strHalfLineCivilNo);
			strQuery.Format( _T("SELECT COUNT(1) FROM CO_LOAN_USER_TBL ")
										   _T(" WHERE CRYPTO_CIVIL_NO='%s'")
       										 _T(" AND USER_NO IS NOT NULL")
											 _T(" AND (USER_CLASS!='3' OR MEMBER_CLASS!='1')")
											 , strHalfLineCivilNo );
		}
		else
		{
			CString strDMCivilNo;
			ids = GetDataMgrData(m_strDMAlias, _T("주민등록번호"), strDMCivilNo, m_nDMIndex);
			if(strHalfLineCivilNo==strDMCivilNo)
			{
				ids = GetDataMgrData(m_strDMAlias, _T("주민등록번호_CRYPTO"), strHalfLineCivilNo, m_nDMIndex);
			}
			else
			{
				this->ESL_DES_Encryption(strHalfLineCivilNo,strHalfLineCivilNo);
			}
			strQuery.Format( _T("SELECT COUNT(1) FROM CO_LOAN_USER_TBL ")
										   _T(" WHERE CRYPTO_CIVIL_NO='%s'")
       										 _T(" AND USER_NO IS NOT NULL")
											 _T(" AND REC_KEY!=%s")
											 _T(" AND (USER_CLASS!='3' OR MEMBER_CLASS!='1')")
											 , strHalfLineCivilNo, strRecKey );
		}
	}


	
	CESL_DataMgr* pDM = FindDM(m_strDMAlias);
	if( NULL == pDM ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CFamilyAddDlg::IsDuplicateCivilNo") ); 
	
	CString strResult;
	ids = CLocCommonAPI::GetOneValueQuery( this , pDM , strQuery , strResult );
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CFamilyAddDlg::IsDuplicateCivilNo") ); 

	if( 0 < _ttoi(strResult) )
	{
		bDuplicate = TRUE;
		return 0;
	}

	
	ids = IsDuplicateDM(_T("주민등록번호"), strCivilNo, bDuplicate);
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CFamilyAddDlg::IsDuplicateCivilNo") ); 

	
	m_strLastCheckCivilNo = strCivilNo;

	return 0;

EFS_END
return -1;
}

INT	CFamilyAddDlg::IsDuplicateDM(CString strFieldAlias, CString strValue, BOOL &bDuplicate)
{
EFS_BEGIN

	INT		ids;
	CString strData;
	CESL_DataMgr* pDM = FindDM(m_strDMAlias);
	if( NULL == pDM ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CFamilyAddDlg::IsDuplicateDM") );
	
	
	INT	nRowCount = pDM->GetRowCount();	

	for( INT i=0; i< nRowCount; i++ )
	{
		
		if( i == m_nDMIndex ) continue;

		ids = pDM->GetCellData(strFieldAlias, i, strData);
		if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CFamilyAddDlg::IsDuplicateDM") );

		
		if( _T("주민등록번호") == strFieldAlias )
		{
			strData.Replace(_T("-"),_T(""));
			strValue.Replace(_T("-"),_T(""));		
		}
		else
		{
			strData.MakeUpper();
			strValue.MakeUpper();			
		}	

		if( 0 == strData.Compare(strValue) )
		{
			bDuplicate = TRUE;
			return 1;
		}
	}

	return 0;

EFS_END
return -1;
}

void CFamilyAddDlg::OnChangeEdtFamilyCivilNo() 
{
	CString strCivilNo;

	GetDlgItemText(IDC_EDT_FAMILY_CIVIL_NO, strCivilNo);

	if ( 0 != m_strLastCheckCivilNo.Compare(strCivilNo) )
	{
		m_bViewCivilNoCheckMsg = FALSE;	
	}
}

void CFamilyAddDlg::OnKillfocusEdtFamilyCivilNo() 
{
	INT		ids;

	
	
	if( m_bViewCivilNoCheckMsg )
	{
		return;
	}
	

	m_bViewCivilNoCheckMsg = TRUE;

	
	
	if( m_bCivilNoAutoCheck )
	{
		BOOL IsDuplicate = false;
		ids = IsDuplicateCivilNo( IsDuplicate );	
		if(0!=ids) return;

		if( IsDuplicate )
		{
			AfxMessageBox( _T("중복된 주민등록번호입니다. 다른 번호를 입력하십시오"), MB_ICONSTOP );
			
			
			( (CEdit*)GetDlgItem(IDC_EDIT_FAMILY_CIVILNUM1) )->SetFocus();
			return;
		}
	}
	

	
	
	CString strCivilNo;
	ids = GetControlData(m_strCMAlias, _T("주민등록번호"), strCivilNo);
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2(-2 , _T("OnKillfocusEdtFamilyCivilNo"));

	SetBirthDay(strCivilNo);
	
	CString strHalfLineCivilNo = strCivilNo;
	
	if( 6 < strHalfLineCivilNo.GetLength() )
	{
		strHalfLineCivilNo = strHalfLineCivilNo.Mid(0,6) + _T("-") + strHalfLineCivilNo.Mid(6);
		ids = SetControlData(m_strCMAlias, _T("주민등록번호"), strHalfLineCivilNo);
		if( ids < 0 ) ERROR_MSG_RETURN_VOID2(-4 , _T("OnKillfocusEdtFamilyCivilNo"));
	}
	
}


VOID CFamilyAddDlg::SetBirthDay(CString sCivilNo)
{
EFS_BEGIN

	INT ids;

	CString strBirthDay;
	ids = GetControlData(m_strCMAlias, _T("생일"), strBirthDay);
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2(-2 , _T("OnKillfocusEdtFamilyCivilNo"));

	
	if( !strBirthDay.IsEmpty() )	return;

	ids = GetControlData( m_strCMAlias , _T("주민등록번호") , sCivilNo );
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("SetBirthDay") );

	
	sCivilNo.Replace( _T("-") , _T("") );
	sCivilNo.TrimLeft();	sCivilNo.TrimRight();

	
	if( sCivilNo.GetLength() < 8 ) return;

	CString sBirthDay;
	
	
	
	INT nSex = _ttoi(sCivilNo.Mid(6,1));
	if(nSex==1||nSex==2||nSex==5||nSex==6) 
	{ 
		sBirthDay = _T("19");
	}
	else if(nSex==3||nSex==4||nSex==7||nSex==8) 
	{ 
		sBirthDay = _T("20");
	}
	else if(nSex==9||nSex==0) 
	{ 
		sBirthDay = _T("18");
	}
	else 
	{ 
		return;
	}

	INT nYear = _ttoi(sBirthDay + sCivilNo.Left(2));
	INT nMonth	= _ttoi(sCivilNo.Mid(2,2));
	INT nDay	= _ttoi(sCivilNo.Mid(4,2));	
	
	BOOL bVaildDate = FALSE;
	if( 1 <= nMonth && 12 >= nMonth )
	{
		
		INT nMonthDays[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
		
		if( !(nYear % 400) || ((nYear % 100) && !(nYear % 4)) )
		{ 
			nMonthDays[1] = 29;
		}
		if( 1 <= nDay && nMonthDays[nMonth-1] >= nDay )
		{ 
			bVaildDate = TRUE;
		}
	}
	
	if(!bVaildDate) return;

	
	sBirthDay.Format( _T("%d/%d/%d"), nYear, nMonth, nDay );
	ids = SetControlData( m_strCMAlias , _T("생일") , sBirthDay );
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("SetBirthDay") );

EFS_END
}

HBRUSH CFamilyAddDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}

INT CFamilyAddDlg::CheckIsPhone()
{
EFS_BEGIN

	
	if(	 0 == m_sSMSVersion.Compare(_T("Y")) )
	{
		
		if( 1 == ((CButton*)GetDlgItem(IDC_BN_CHECK_FAMILY_SEND_SMS))->GetCheck() )
		{
			CString		strPhone1,strPhone2;
			( (CEdit*)GetDlgItem(IDC_EDT_FAMILY_PHONE2) )->GetWindowText(strPhone1);
			( (CEdit*)GetDlgItem(IDC_EDT_FAMILY_PHONE3) )->GetWindowText(strPhone2);

			if( strPhone1.IsEmpty()	||	strPhone2.IsEmpty() )
			{
				AfxMessageBox(_T("SMS 서비스 수신을 신청 하셨습니다.\r\n해당 서비스를 받으시려면 핸드폰 번호를 입력하셔야 합니다"),MB_ICONINFORMATION);
				return	-1;
			}
			m_pSpfWork->SetTempData(_T("SMS수신여부"), CComVariant(_T("Y")) );
		}
	}
	return	0;

EFS_END
return	-1;
}

INT CFamilyAddDlg::CheckIsEMail()
{
EFS_BEGIN

	if( 1 == ((CButton*)GetDlgItem(IDC_BN_CHECK_FAMILY_SEND_MAIL))->GetCheck() )
	{
		CString		strMail1,strMail2;
		( (CEdit*)GetDlgItem(IDC_EDT_FAMILY_EMAIL1) )->GetWindowText(strMail1);
		( (CComboBox*)GetDlgItem(IDC_CMB_FAMILY_EMAIL2) )->GetWindowText(strMail2);

		if(  strMail1.IsEmpty()	||	strMail2.IsEmpty() )
		{
			AfxMessageBox(_T("E-Mail 수신을 신청하셨습니다.\r\n해당 서비스를 받으시려면 주소를 입력하셔야 합니다."));
			return	-1;
		}

		if( 0	>=	strMail2.Find( _T('.')) )
		{
			AfxMessageBox(_T("E-Mail 주소를 정확히 입력하여 주십시요"));
			return	-1;
		}
	}
	return	0;

EFS_END

	return -1;
}

void CFamilyAddDlg::OnKillfocusEditFamilyCivilNum1()
{
EFS_BEGIN

	CString sCivilNo = MakeFamilyCivilNum();

	CString strLastFamilyCivilNo;
	INT ids = GetControlData(_T("CM_FAMILY_ADD_INPUT"), _T("주민등록번호"), strLastFamilyCivilNo);
	if (0 > ids) ERROR_MSG_RETURN_VOID2( 1010 ,_T("OnKillfocusEditFamilyCivilNum1"));

	if (strLastFamilyCivilNo.CompareNoCase(sCivilNo) != 0)
	{
		ids = SetControlData(_T("CM_FAMILY_ADD_INPUT"), _T("주민등록번호"), sCivilNo);
		if (0 > ids) ERROR_MSG_RETURN_VOID2( 1020 ,_T("OnKillfocusEditFamilyCivilNum1"));
	}

EFS_END	
}

void CFamilyAddDlg::OnKillfocusEditFamilyCivilNum2()
{
EFS_BEGIN

	CString sCivilNo = MakeFamilyCivilNum();


	CString strLastFamilyCivilNo;
	INT ids = GetControlData(_T("CM_FAMILY_ADD_INPUT"), _T("주민등록번호"), strLastFamilyCivilNo);
	if (0 > ids) ERROR_MSG_RETURN_VOID2( 1010 ,_T("OnKillfocusEditFamilyCivilNum2"));

	if (strLastFamilyCivilNo.CompareNoCase(sCivilNo) != 0)
	{
				
		if(_T("Y") == m_strIsAutoSetCivilnoToPassword)
		{
			CString		strCivilNo2;
			GetDlgItem(IDC_EDIT_FAMILY_CIVILNUM2)->GetWindowText(strCivilNo2);

			GetDlgItem(IDC_EDT_FAMILY_PASS)->SetWindowText(strCivilNo2);
			GetDlgItem(IDC_EDT_FAMILY_PASS2)->SetWindowText(strCivilNo2);
		}
		
		INT ids = SetControlData( _T("CM_FAMILY_ADD_INPUT"), _T("주민등록번호"), sCivilNo );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_VOID2( -1010 , _T("OnKillfocusEditFamilyCivilNum2") );
		}
		
		BOOL	IsDuplicate = FALSE;
		ids = IsDuplicateCivilNo( IsDuplicate );
		if(0!=ids) return;

		if( IsDuplicate )
		{
			AfxMessageBox( _T("중복된 주민등록번호입니다. 다른 번호를 입력하십시오"), MB_ICONSTOP );
			( (CEdit*)GetDlgItem(IDC_EDIT_FAMILY_CIVILNUM1) )->SetFocus();			
		}
				
		
		SetBirthDay(sCivilNo);

		
		m_strLastFamilyCivilNo = sCivilNo;
	}

EFS_END
}

CString CFamilyAddDlg::MakeFamilyCivilNum()
{
	CString sCivilNo;
	CString strCivilNum1;
	CString strCivilNum2;
	
	GetDlgItem(IDC_EDIT_FAMILY_CIVILNUM1)->GetWindowText(strCivilNum1);
	GetDlgItem(IDC_EDIT_FAMILY_CIVILNUM2)->GetWindowText(strCivilNum2);

	sCivilNo.Format(_T("%s%s"),strCivilNum1,strCivilNum2);

	return sCivilNo;
}

void CFamilyAddDlg::SetFamilyCivilNum()
{
EFS_BEGIN

	CString	sCivilNo;
	CString	strTemp;
	CString	strMessage;

	INT nLength = 0;
	INT	ids = GetControlData( _T("CM_FAMILY_ADD_INPUT") , _T("주민등록번호") , sCivilNo );
	if(0>ids) ERROR_MSG_RETURN_VOID2( -1 , _T("SetFamilyCivilNum") );

	strTemp = sCivilNo;

	sCivilNo.Replace(_T("-"),_T(""));

	TCHAR cNum;
	CString sIndex;
	nLength = sCivilNo.GetLength();
	for( INT i=0 ; i<nLength ; i++ )
	{
		cNum = sCivilNo.GetAt( i );
		if(_T("Y")==m_sIsUnityLoanService)
		{
			if( (47<cNum && 58>cNum) || 42==cNum ) sIndex += cNum;
		}
		else
		{
			if( 47<cNum && 58>cNum ) sIndex += cNum;
		}
	}

	CString sLeftCivilNo = sIndex.Left(6);
	CString sRightCivilNo = sIndex.Mid(sLeftCivilNo.GetLength());
	GetDlgItem(IDC_EDIT_FAMILY_CIVILNUM1)->SetWindowText(sLeftCivilNo);
	GetDlgItem(IDC_EDIT_FAMILY_CIVILNUM2)->SetWindowText(sRightCivilNo);

EFS_END
}

INT CFamilyAddDlg::ShowCtrlUserNoMode()
{
EFS_BEGIN

	INT	ids;
	
	switch( m_nAutoMode )
	{
		case 0 :
			ids = SetControlData( m_strCMAlias , _T("대출자번호") , _T("") );
			( (CEdit*)GetDlgItem(IDC_EDT_FAMILY_USERNO) )->SetReadOnly(FALSE);
			break;
		case 1 :
			ids = SetControlData( m_strCMAlias , _T("대출자번호") , _T("[주민번호로생성]") );
			( (CEdit*)GetDlgItem(IDC_EDT_FAMILY_USERNO) )->SetReadOnly(TRUE);
			break;
		case 2 :			
			ids = SetControlData( m_strCMAlias , _T("대출자번호") , _T("[자동생성]") );
			( (CEdit*)GetDlgItem(IDC_EDT_FAMILY_USERNO) )->SetReadOnly(TRUE);
			GetDlgItem(IDC_STATIC_FAMILY_NUMTYPE)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_CMB_FAMILY_NUMTYPE)->ShowWindow(SW_SHOW);
			break;
		case 3 :
			ids = SetControlData( m_strCMAlias , _T("대출자번호") , _T("[사용자ID로생성]") );
			( (CEdit*)GetDlgItem(IDC_EDT_FAMILY_USERNO) )->SetReadOnly(TRUE);		
			GetDlgItem(IDC_STATIC_FAMILY_ID)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDT_FAMILY_ID)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BN_FAMILY_ID_DUPLICATE)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_FAMILY_PASS)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDT_FAMILY_PASS)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_FAMILY_PASS2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDT_FAMILY_PASS2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_ID_GROUP)->ShowWindow(SW_SHOW);
			break;
		default :
			break;
	}
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("SetCtrlUserNoMode") ); ;
	}
	
	return 0;
	
EFS_END
return -1;
}

void CFamilyAddDlg::SetCtrlUserNoMode()
{
	const int nMoveCtrlCount =6;
	UINT	uMoveCtrlID[nMoveCtrlCount]	=
	{
		IDC_BN_FAMILY_CLEAR,	IDC_BN_FAMILY_LIST_DELETE,	IDC_BN_FAMILY_LIST_MODIFY,
		IDC_BN_FAMILY_ADD,		IDC_BN_FAMILY_INSERT,		IDC_BN_FAMILY_CLOSE
	};

	CWnd* pWnd;
	CRect rCtrl;
	
	for( INT i=0; i<nMoveCtrlCount; i++ )
	{
		pWnd = GetDlgItem(uMoveCtrlID[i]);
		if(NULL != pWnd && NULL != pWnd->GetSafeHwnd())
		{
			pWnd->GetWindowRect(rCtrl);
			ScreenToClient(rCtrl);
			rCtrl.OffsetRect(CPoint(0, -44));
			pWnd->MoveWindow(rCtrl);
		}
	}

	
	this->GetWindowRect(rCtrl);
	ScreenToClient(rCtrl);
	rCtrl.bottom	-=	44;
	this->MoveWindow(rCtrl);	
}


void CFamilyAddDlg::OnBnFamilyIdDuplicate() 
{
EFS_BEGIN
	
	INT ids;
	BOOL IsDuplicate = FALSE;
	ids = IsDuplicateUserID( IsDuplicate );	
	if( ids < 0 ) return ;
	if( ids > 0 ) return;

	if( IsDuplicate )
	{
		AfxMessageBox( _T("중복된 사용자ID입니다. 다른 ID를 사용하십시오."), MB_ICONSTOP );

	}
	else
	{
		AfxMessageBox( _T("중복된 사용자ID가 존재하지 않습니다.") );
	}	

EFS_END	
}

INT CFamilyAddDlg::IsDuplicateUserID( BOOL &IsDuplicate )
{
EFS_BEGIN

	INT ids;
	CString sSql;

	CString sGetData;
	
	if(		_T("Y") == m_strIsAutoSetUsernoToUserid 
		&&	3 != m_nAutoMode
		&&	-1 == m_nDMIndex)
	{
		if( 1 == m_nAutoMode )
		{
			sGetData = MakeFamilyCivilNum();
			sGetData.Replace( _T(" "), _T("") );
			if( TRUE == sGetData.IsEmpty() && _T("Y") == m_bCivilNoDisplayCheck )
			{
				AfxMessageBox( _T("주민등록번호를 입력하여 주십시오") );
				return 1;
			}
		}
		else
		{
			ids = GetControlData( m_strCMAlias, _T("대출자번호") , sGetData );
			if( 0 > ids )
			{
				return -1;	
			}
		}
	}
	else
	{
		ids = GetControlData(m_strCMAlias, _T("사용자ID") , sGetData );
		if( 0 > ids )
		{
			return -1;	
		}
	}

	if( sGetData.IsEmpty() )
	{
		IsDuplicate = FALSE;
		return 2;
	}
	sSql.Format( _T("SELECT count(*) FROM CO_LOAN_USER_TBL WHERE USER_ID = '%s'") , sGetData );

	
	if( -1 != m_nDMIndex )
	{
		CString strRecKey;

		CESL_DataMgr *pDM = FindDM( m_strDMAlias ); 
		ids = pDM->GetCellData( _T("REC_KEY") , m_nDMIndex , strRecKey );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("IsDuplicateUserNo") );
		}

		if( TRUE != strRecKey.IsEmpty())
		{
			sSql += _T(" AND REC_KEY NOT IN (")+ strRecKey +_T(")");
		}
	}
	
	CESL_DataMgr *pDM = FindDM( _T("DM_BO_LOC_3300") );
	if( pDM == NULL ) return -1;

	CString sResult;
	ids = CLocCommonAPI::GetOneValueQuery( this , pDM , sSql , sResult );

	if( _ttoi(sResult) > 0 ) IsDuplicate = TRUE;
	else  IsDuplicate = FALSE;

	return 0;	

EFS_END
return -1;
}

CString CFamilyAddDlg::MakeCivilNumFormat(CString civilNum)
{
EFS_BEGIN
	
	civilNum = MakeCivilNumIndex( civilNum );
	if( 13 != civilNum.GetLength() )
		return civilNum;

	return civilNum.Left( 6 ) + _T("-") + civilNum.Right( 7 );

EFS_END
return _T("");
}

CString CFamilyAddDlg::MakeCivilNumIndex(CString civilNum)
{
	EFS_BEGIN
	
	INT nLength = civilNum.GetLength();

	CString sIndex;
	TCHAR cNum;
	for( INT i = 0; i < nLength; i++ )
	{
		cNum = civilNum.GetAt( i );


		if(_T("Y")==m_sIsUnityLoanService)
		{
			if( (cNum > 47 && cNum < 58) || 42==cNum )
				sIndex += cNum;
		}
		else
		{
			if( cNum > 47 && cNum < 58 )
				sIndex += cNum;
		}
	}

	return sIndex;

	EFS_END
	return _T("");
}

INT CFamilyAddDlg::ControlDisplay(CString cmalias, CString ctrlalias, INT index)
{
	int ids = 0;
	CString strFuncName = _T("ControlDisplay");

	CESL_DataMgr* pDstDM = FindDM(_T("DM_FAMILY_TOTAL_LIST"));
	if( NULL == pDstDM ) ERROR_MSG_RETURN_ERROR_CODE2( -1010 , strFuncName );

	
	
	const int ROW_COUNT = pDstDM->GetRowCount();
	for(int nCnt = 0; nCnt < ROW_COUNT; nCnt++)
	{
		CString strCivilNo;
		ids = pDstDM->GetCellData(_T("주민등록번호"), nCnt, strCivilNo);
		if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -1020 , strFuncName );


		INT ids = CLocCommonAPI::ChangeSecuriyCivilNo(this,strCivilNo,m_sIsUnityLoanService);

		
		
		ids = pDstDM->SetCellData(_T("주민등록번호_VIEW"), strCivilNo, nCnt);
		if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -1040 , strFuncName );

		
	}
	
	int nResult = CESL_Mgr::ControlDisplay(cmalias, ctrlalias, index);

	
	return ids;
}

void CFamilyAddDlg::OnKillfocusEdtFamilyUserno() 
{
	
}

void CFamilyAddDlg::OnSetfocusEdtFamilyUserName() 
{
	HIMC hIME;
    DWORD dwConversion, dwSentence;

    hIME = ::ImmGetContext(GetDlgItem(IDC_EDT_FAMILY_USER_NAME)->m_hWnd);
    if(hIME) 
    {
        ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		
		dwConversion=IME_CMODE_HANGUL;

		ImmSetConversionStatus(hIME,dwConversion,dwSentence );

        ImmReleaseContext(GetDlgItem(IDC_EDT_FAMILY_USER_NAME)->m_hWnd,hIME); 
    } 
}

void CFamilyAddDlg::OnSetfocusEdtFamilyUserno() 
{
	HIMC hIME;
    DWORD dwConversion, dwSentence;

    hIME = ::ImmGetContext(GetDlgItem(IDC_EDT_FAMILY_USERNO)->m_hWnd);
    if(hIME) 
    {
        ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		
		dwConversion=IME_CMODE_ALPHANUMERIC;

		ImmSetConversionStatus(hIME,dwConversion,dwSentence );

        ImmReleaseContext(GetDlgItem(IDC_EDT_FAMILY_USERNO)->m_hWnd,hIME); 
    } 
}

void CFamilyAddDlg::OnChangeEditFamilyCivilnum1() 
{
	
	if(m_bAutoCivilNoInsert==TRUE)
	{
		return;
	}

	CString strText;
	GetDlgItemText(IDC_EDIT_FAMILY_CIVILNUM1, strText);
	
	
	if( 6 == strText.GetLength())
	{
		CWnd* pWnd = GetDlgItem(IDC_EDIT_FAMILY_CIVILNUM2);
		if(NULL != pWnd)
		{
			pWnd->SetFocus();
		}
	}
}

void CFamilyAddDlg::OnChangeEdtFamilyUserno() 
{
	if(-1 == m_nDMIndex)
	{
		INT ids;

		CString strUserNo;

		ids = GetControlData( m_strCMAlias, _T("대출자번호") , strUserNo );
		if(0 > ids) 
		{
			ERROR_MSG_RETURN_VOID2( -1000 , _T("OnChangeedtUSER1") ); 
		}
		strUserNo.TrimLeft(); 
		strUserNo.TrimRight();

		CString strBuf;
		if( 13 <= strUserNo.GetLength() )
		{
			strBuf = strUserNo.Mid(0,6) + _T("-") + strUserNo.Mid(6);
		}
		ids = CLocCommonAPI::CivilNoCheck( strBuf );
		if(0 == ids)
		{
			strUserNo = strUserNo.Mid(0,6) + _T("-") + strUserNo.Mid(6,7);
			ids = SetControlData( m_strCMAlias, _T("주민등록번호"), strUserNo );
			if(0 > ids) 
			{
				ERROR_MSG_RETURN_VOID2( -1010, _T("OnChangeedtUSER1") ); 
			}
			
			m_bAutoCivilNoInsert = TRUE;
			SetFamilyCivilNum();
			
			OnKillfocusEditFamilyCivilNum2();
			m_bAutoCivilNoInsert = FALSE;
		}
	}
}

void CFamilyAddDlg::OnChangeEdtFamilyPhone2() 
{	
	CString strText;
	GetDlgItemText(IDC_EDT_FAMILY_PHONE2, strText);
	if( 4 == strText.GetLength())
	{
		CWnd* pWnd = GetDlgItem(IDC_EDT_FAMILY_PHONE3);
		if(NULL != pWnd)
		{
			pWnd->SetFocus();
		}
	}
}
