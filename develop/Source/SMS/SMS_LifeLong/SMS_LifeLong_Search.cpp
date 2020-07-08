// SMS_LifeLong_Search.cpp : implementation file
//

#include "stdafx.h"
#include "SMS_LifeLong_Search.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSMS_LifeLong_Search dialog


CSMS_LifeLong_Search::CSMS_LifeLong_Search(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSMS_LifeLong_Search)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_sSMSType	   = _T("");
	m_nDisplayMode = -1;
	m_pCSMS_LifeLong = (CSMS_LifeLong*)pParent;
}


VOID CSMS_LifeLong_Search::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSMS_LifeLong_Search)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSMS_LifeLong_Search, CDialog)
	//{{AFX_MSG_MAP(CSMS_LifeLong_Search)
	ON_CBN_SELCHANGE(IDC_cSMS, OnSelchangecSMS)
	ON_CBN_SELCHANGE(IDC_cUSER_GROUP, OnSelchangecUSERGROUP)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSMS_LifeLong_Search message handlers

BOOL CSMS_LifeLong_Search::Create(CWnd* pParentWnd)
{
EFS_BEGIN	
	return CDialog::Create(IDD, pParentWnd);
EFS_END
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////
// [함수설명]
// 초기화 작업
///////////////////////////////////////////////////////////////////////////
BOOL CSMS_LifeLong_Search::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	INT ids;
	CString sErrMsg = _T("");

	//////////////////////////////////////////////////////////////////
	// 0. 개발자 코드 감추기 
	//////////////////////////////////////////////////////////////////
	FILE* fp = _tfopen(_T("..\\Cfg\\_debug"),_T("rb"));
	
	if (fp == NULL)
	{
		CRect rc;
		this->GetClientRect(&rc);
		this->MoveWindow( rc.left, rc.top, rc.Width() - 305, rc.Height() + 23);
	}
	else
		fclose(fp);

	///////////////////////////////////////////////////////////////////////
	// 1. SM 초기화
	///////////////////////////////////////////////////////////////////////
	if( InitESL_Mgr( _T("SMS_DLG_LifeLong_Search") ) < 0 )
	{
		sErrMsg.Format( _T("InitESL_Mgr Error : SMS_DLG_LifeLong_Search") );
		AfxMessageBox( sErrMsg );
		return false;
	}
	
	///////////////////////////////////////////////////////////////////////
	// 2. SMS 종류 콤보박스 초기화
	///////////////////////////////////////////////////////////////////////
	ids = InitSMSGroupCombo();
	if (ids < 0)	return FALSE;
	
	
	///////////////////////////////////////////////////////////////////////
	// 3. 강좌 종류 콤보박스 초기화
	///////////////////////////////////////////////////////////////////////
	ids = InitCategoryCombo();
	if (ids < 0)	return FALSE;
	

	///////////////////////////////////////////////////////////////////////
	// 4. 이용자 그룹 콤보박스 초기화
	///////////////////////////////////////////////////////////////////////
	ids = InitUserGroupCombo();
	if (ids < 0)	return FALSE;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

////////////////////////////////////////////////////////////////////////////
// [함수설명]
// SMS종류 콤보박스를 초기화 한다.
////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong_Search::InitSMSGroupCombo()
{
EFS_BEGIN

	INT ids;
	/////////////////////////////////////////////////////////////////////////
	// 1. SMS 기본정보 초기화
	/////////////////////////////////////////////////////////////////////////
	CESL_DataMgr* pDM = FindDM( _T("DM_SMS_LifeLong_SMSGroup") );
	if (pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CSMS_LifeLong_Search::InitSMSGroupCombo") );
	pDM->RefreshDataManager( _T("") );
	
	/////////////////////////////////////////////////////////////////////////
	// 2. SMS종류 콤보박스에 입력한다.
	/////////////////////////////////////////////////////////////////////////
	INT nCnt = pDM->GetRowCount();
	if ( nCnt < 1 )
	{
		// 선택할수 있는 메일이 없다.
		return 3;
	}
	
	CString sGetSMSGroup = _T("");
	CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_cSMS);
	pCB->AddString( _T("") );

	for (INT i = 0;i < nCnt;i++)
	{
		ids = pDM->GetCellData( _T("SMS명"), i, sGetSMSGroup);
		if (ids < 0) return -2;
		sGetSMSGroup.TrimLeft();sGetSMSGroup.TrimRight();
		pCB->AddString( sGetSMSGroup );
	}

	return 0;
EFS_END
	return -1;
}

////////////////////////////////////////////////////////////////////////////
// [함수설명]
// 강좌구분 콤보박스 초기화
////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong_Search::InitCategoryCombo()
{
EFS_BEGIN

	INT ids;
	/////////////////////////////////////////////////////////////////////////
	// 1. 강좌구분 기본정보 초기화
	/////////////////////////////////////////////////////////////////////////
	CESL_DataMgr* pDM = FindDM( _T("DM_SMS_LifeLong_PGCategory") );
	if (pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CSMS_LifeLong_Search::InitCategoryCombo") );
	pDM->RefreshDataManager( _T("") );

	/////////////////////////////////////////////////////////////////////////
	// 2. 강좌구분 콤보박스에 입력한다.
	/////////////////////////////////////////////////////////////////////////
	INT nCnt = pDM->GetRowCount();
	if ( nCnt < 1 )
	{
		// 선택할수 있는 강좌구분이 없다.
		return 3;
	}
	
	CString sGetSMSGroup = _T("");
	CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_cPROGRAM_CODE);
	pCB->AddString( _T("") );

	for (INT i = 0;i < nCnt;i++)
	{
		ids = pDM->GetCellData( _T("강좌구분명"), i, sGetSMSGroup);
		if (ids < 0) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CSMS_LifeLong_Search::InitCategoryCombo") );
		sGetSMSGroup.TrimLeft();sGetSMSGroup.TrimRight();
		pCB->AddString( sGetSMSGroup );
	}

	return 0;
EFS_END
	return -1;
}

////////////////////////////////////////////////////////////////////////////
// [함수설명]
// 이용자그룹 콤보박스 초기화
////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong_Search::InitUserGroupCombo()
{
EFS_BEGIN

	INT ids;
	/////////////////////////////////////////////////////////////////////////
	// 1. 이용자그룹 기본정보 초기화
	/////////////////////////////////////////////////////////////////////////
	CESL_DataMgr* pDM = FindDM( _T("DM_SMS_LifeLong_UserGroup") );
	if (pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CSMS_LifeLong_Search::InitUserGroupCombo") );
	pDM->RefreshDataManager( _T("") );

	///////////////////////////////////////////////////////////////
	// 2. 그룹콤보박스를 멀티스타일로 구성한다.
	///////////////////////////////////////////////////////////////
	CESL_ControlMgr* pCM = FindCM(_T("CM_SMS_LifeLong_Search"));
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CSMS_LifeLong_Search::InitUserGroupCombo") );

	CESL_ControlMultiComboBox* pCMCB = (CESL_ControlMultiComboBox*)(pCM->FindControl(_T("회원그룹")));
	if( pCMCB == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CSMS_LifeLong_Search::InitUserGroupCombo") );

	///////////////////////////////////////////////////////////////
	// 3. 그룹콤보박스 내용을 구성한다.
	///////////////////////////////////////////////////////////////
	CString sInsertCode[2];

	pCMCB->InitMultiComboBox(2);
		
	sInsertCode[0] = _T("   ");
	sInsertCode[1] = _T("적용안함");
	ids = pCMCB->SetData( sInsertCode , 2 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CSMS_LifeLong_Search::InitUserGroupCombo") );

	INT cnt = pDM->GetRowCount();
	if (cnt != 0)
	{
		for (INT i = 0;i < cnt;i++)
		{
			CString sTemp =_T("");

			pDM->GetCellData(_T("그룹코드"), i, sTemp);
			sInsertCode[0] = sTemp;
			pDM->GetCellData(_T("그룹이름"), i, sTemp);
			sInsertCode[1] = sTemp;
			ids = pCMCB->SetData( sInsertCode , 2 );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CSMS_LifeLong_Search::InitUserGroupCombo") );
		}	
	}
	
	pCMCB->SetCurSel(0);

	return 0;
EFS_END
	return -1;
}

////////////////////////////////////////////////////////////////////////////
// [함수설명]
// 날짜 에디터 박스의 날짜를 반환시켜준다.
// (I N) 날짜 에디터 박스 리소스 ID
// (OUT) 날짜 'YYYY/MM/DD'
////////////////////////////////////////////////////////////////////////////
CString CSMS_LifeLong_Search::GetCtrlDate(UINT id)
{
	CDateTimeCtrl *pDate = ( CDateTimeCtrl * )GetDlgItem( id );
	if(!pDate->IsWindowEnabled()) return _T("");
	CTime time;
	if( GDT_VALID != pDate->GetTime( time ) )
		return _T("");

	CString date;
	date.Format( _T("%04d/%02d/%02d"), time.GetYear(), time.GetMonth(), time.GetDay() );
	return date;
}

////////////////////////////////////////////////////////////////////////////
// [함수설명]
// SMS 종류 콤보박스 선택 이벤트
////////////////////////////////////////////////////////////////////////////
VOID CSMS_LifeLong_Search::OnSelchangecSMS() 
{
	///////////////////////////////////////////////////////////////////////
	// 1. 선택된 콤보 박스의 SMS 이름을 구한다.
	///////////////////////////////////////////////////////////////////////
	CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_cSMS);
	INT nIndex = pCB->GetCurSel();
	CString sGetValue = _T("");
	pCB->GetLBText(nIndex, sGetValue);

	///////////////////////////////////////////////////////////////////////
	// 2. 해당 SMS의 키를 저장한다.
	///////////////////////////////////////////////////////////////////////
	CESL_DataMgr* pDM = FindDM( _T("DM_SMS_LifeLong_SMSGroup") );
	if (pDM == NULL) return;
	pDM->RefreshDataManager( _T("") );

	INT nCnt = pDM->GetRowCount();
	CString sGetSMSGroup = _T("");

	for (INT i = 0;i < nCnt;i++)
	{
		pDM->GetCellData( _T("SMS명"), i, sGetSMSGroup);
		if ( _tcscmp(sGetSMSGroup, sGetValue) == 0 )
		{
			pDM->GetCellData( _T("타입"), i, m_sSMSType);
			break;
		}
	}

	///////////////////////////////////////////////////////////////////////
	// 3. 개발자 정보
	///////////////////////////////////////////////////////////////////////
	SetDlgItemText(IDC_eTest6, m_sSMSType);
}

////////////////////////////////////////////////////////////////////////////
// [함수설명]
// m_nDisplayMode에 따라 입력할 수 있는 항목이 틀리다.
////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong_Search::DisplayCtrlMode()
{
	return 0;	
}

VOID CSMS_LifeLong_Search::OnOK() 
{
	INT ids;
	CString sErrMsg = _T("");

	///////////////////////////////////////////////////////////////////////
	// 1. 입력 체크
	///////////////////////////////////////////////////////////////////////
	if ( m_sSMSType.IsEmpty() )
	{
		sErrMsg.Format(_T("SMS종류를 선택하지 않았습니다."));
		AfxMessageBox(sErrMsg);
		return;
	}

	m_pCSMS_LifeLong->m_sSMSType = m_sSMSType;

	///////////////////////////////////////////////////////////////////////
	// 2. 모드결정
	///////////////////////////////////////////////////////////////////////
	ids = SetCurMode();
	if (ids < 0)
	{
		sErrMsg.Format( _T(" ERROR Position :: SetCurMode\r\n ERROR CODE :: %d "), ids);
		AfxMessageBox(sErrMsg);
		return;
	}
	// 2.1) 부모의 모드를 SET한다.
	m_pCSMS_LifeLong->SetDisplayMode(m_nDisplayMode);

	///////////////////////////////////////////////////////////////////////
	// 3. 모드에 따른 쿼리 생성
	///////////////////////////////////////////////////////////////////////
	ids = MakeQuery();
	if (ids < 0)
	{
		sErrMsg.Format( _T(" ERROR Position :: MakeQuery\r\n ERROR CODE :: %d "), ids);
		AfxMessageBox(sErrMsg);
		return;
	}

//	CDialog::OnOK();
}

////////////////////////////////////////////////////////////////////////////
// [함수설명]
// 생성될 쿼리의 종류를 결정한다.
////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong_Search::MakeQuery()
{
	CString str = _T("");

	///////////////////////////////////////////////////////////////////////
	// 1. 개발자 정보
	///////////////////////////////////////////////////////////////////////
	str.Format(_T("%d"), m_nDisplayMode);
	SetDlgItemText(IDC_eTest4, str);
	str = _T("");

	///////////////////////////////////////////////////////////////////////
	// 2. 모드에 따라 다르다.
	///////////////////////////////////////////////////////////////////////
	if (m_nDisplayMode == 0)
	{
		MakeQuery_Program(&str);
		MakeQuery_Member(&str);
	}
	else if (m_nDisplayMode == 1)
		MakeQuery_NonMember(&str);
	else if (m_nDisplayMode == 2)
		MakeQuery_Member(&str);
	
	m_pCSMS_LifeLong->SearchUSER(str);

	///////////////////////////////////////////////////////////////////////
	// 3. 개발자 정보
	///////////////////////////////////////////////////////////////////////
	GetUserGroup(&str);
	SetDlgItemText(IDC_eTest5, str);

	return 0;
}

////////////////////////////////////////////////////////////////////////////
// [함수설명]
// 회원용 쿼리를 만든다.
////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong_Search::MakeQuery_Member(CString *psQuery)
{
	EFS_BEGIN

	INT ids;
	CString sTemp = _T("");
	
	///////////////////////////////////////////////////////////////
	// 1. 컨트롤 입력값을 저장한다.
	///////////////////////////////////////////////////////////////	
	const INT nCount = 9;
 
	INT nCtrlID[nCount] = { 	IDC_cUSER_SEX	   , IDC_eUSER_ID		, 
								IDC_eUSER_NAME1	   , IDC_eUSER_NAME2	,
								IDC_eUSER_AGE1	   , IDC_eUSER_AGE2	    ,
								IDC_eUSER_ADDRESS  , IDC_eUSER_PHONE	,   // 전화
								IDC_eUSER_PHONE2   };		
 		
	CString sCtrlData[nCount];

	for (INT i = 0;i < nCount; i++)
	{
		GetDlgItemText(nCtrlID[i] , sCtrlData[i]);
		sCtrlData[i].TrimLeft();sCtrlData[i].TrimRight();
	}

	///////////////////////////////////////////////////////////////
	// 2. 회원 성별 조건을 입력한다.
	///////////////////////////////////////////////////////////////
	if ( !sCtrlData[0].IsEmpty() )
	{
		if ( !psQuery->IsEmpty() )
			*psQuery += _T(" AND ");

		if ( sCtrlData[0].Compare(_T("남자")) == 0 )
			sTemp = _T(" M.MB_SEX = 'M' ");
		else
			sTemp = _T(" M.MB_SEX = 'F' ");

		*psQuery += sTemp;
	}
	
	///////////////////////////////////////////////////////////////
	// 3. 회원 아이디 조건을 입력한다.
	///////////////////////////////////////////////////////////////
	if ( !sCtrlData[1].IsEmpty() )
	{
		if ( !psQuery->IsEmpty() )
			*psQuery += _T(" AND ");

		sTemp = _T(" M.MB_ID LIKE '%") + sCtrlData[1] + _T("%' ");
		*psQuery += sTemp;
	}
	
	///////////////////////////////////////////////////////////////
	// 4. 회원 이름 조건을 입력한다.
	///////////////////////////////////////////////////////////////
	if ( !sCtrlData[2].IsEmpty() )
	{
		if ( !psQuery->IsEmpty() )
			*psQuery += _T(" AND ");

		if ( !sCtrlData[3].IsEmpty() )
			sTemp = _T(" M.MB_NAME BETWEEN '") + sCtrlData[2] + _T("' AND '") + sCtrlData[3] + _T("힝' ");
		else
			sTemp = _T(" M.MB_NAME BETWEEN '") + sCtrlData[2] + _T("' AND '") + sCtrlData[2] + _T("힝' ");

		*psQuery += sTemp;
	}

	///////////////////////////////////////////////////////////////
	// 4. 회원 나이 조건을 입력한다.
	///////////////////////////////////////////////////////////////
	if ( !sCtrlData[4].IsEmpty() )
	{
		if ( !psQuery->IsEmpty() )
			*psQuery += _T(" AND ");

		///////////////////////////////////////////////////////////
		// 4 - 1. 현재 년도를 구한다.
		///////////////////////////////////////////////////////////
		CString query = _T("select sysdate from dual");
		sTemp = _T("");

		CESL_DataMgr *pDM = FindDM( _T("DM_SMS_LifeLong") );
		if(pDM==NULL) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CSMS_LifeLong_Search::MakeQuery_Member") );

		ids = pDM->GetOneValueQuery(query, sTemp);
		if (ids < 0)
		{
			CString error;
			error.Format(_T("DM_SMS_LifeLong_PGCategory::MakeQuery_User \r\n")
						 _T("		GetBirth error		"));
			return -1;
		}

		sTemp = sTemp.Mid(2,4);
		
		///////////////////////////////////////////////////////////
		// 4 - 2. 시작년도와 마지막년도 조건을 입력한다.
		///////////////////////////////////////////////////////////
		INT nStart = _ttoi(sTemp) - _ttoi(sCtrlData[4]);

		while (nStart < 0)
			nStart = 100 + nStart;

		CString sStart, sEnd;


		if ( !sCtrlData[5].IsEmpty() )
		{
			INT nEnd = _ttoi(sTemp) - _ttoi(sCtrlData[5]);
			
			while (nEnd < 0)
				nEnd = 100 + nEnd;
		
			sStart.Format(_T("%02d/01/01"), nStart);
			sEnd.Format(_T("%02d/12/31"), nEnd);			
		}
		else 
		{		
			sStart.Format(_T("%02d/01/01"), nStart);
			sEnd.Format(_T("%02d/12/31"), nStart);		
		}

		sTemp = _T(" M.MB_BIRTH BETWEEN '") + sStart + _T("' AND '") + sEnd + _T("' ");
		*psQuery += sTemp;
	}

	///////////////////////////////////////////////////////////////
	// 5. 회원 지역 조건을 입력한다.
	///////////////////////////////////////////////////////////////
	if ( !sCtrlData[6].IsEmpty() )
	{
		if ( !psQuery->IsEmpty() )
			*psQuery += _T(" AND ");

		sTemp = _T(" M.MB_ADDR1 LIKE '%") + sCtrlData[6] + _T("%' ");
		*psQuery += sTemp;
	}

	///////////////////////////////////////////////////////////////
	// 6. 회원 전화번호 조건을 입력한다.
	///////////////////////////////////////////////////////////////
	if ( !sCtrlData[7].IsEmpty() )
	{
		if ( !psQuery->IsEmpty() )
			*psQuery += _T(" AND ");

		sTemp = _T(" M.MB_TEL LIKE '%") + sCtrlData[7] + _T("%' ");
		*psQuery += sTemp;
	}

	///////////////////////////////////////////////////////////////
	// 7. 회원 핸드폰 조건을 입력한다.
	///////////////////////////////////////////////////////////////
	if ( !sCtrlData[8].IsEmpty() )
	{
		if ( !psQuery->IsEmpty() )
			*psQuery += _T(" AND ");

		sTemp = _T(" M.MB_HP LIKE '%") + sCtrlData[8] + _T("%' ");
		*psQuery += sTemp;
	}

	///////////////////////////////////////////////////////////////
	// 8. 그룹 조건을 입력한다.
	///////////////////////////////////////////////////////////////
	ids = GetUserGroup(&sTemp);

	if ( !sTemp.IsEmpty() )
	{
		if ( !psQuery->IsEmpty() )
			*psQuery += _T(" AND ");

		CString query;
		
		query.Format( _T(" CLU.REC_KEY IN (Select USER_KEY FROM MN_LOAN_USER_GROUP_LIST_TBL WHERE GROUP_CODE = '%s' )"), sTemp );
		*psQuery += query;
	}
	///////////////////////////////////////////////////////////////
	// 9. 입력 테스트
	///////////////////////////////////////////////////////////////
	SetDlgItemText(IDC_eTest2, *psQuery);
	return 0;

	EFS_END
	return -1;
}

////////////////////////////////////////////////////////////////////////////
// [함수설명]
// 비회원용 쿼리를 만든다.
////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong_Search::MakeQuery_NonMember(CString *psQuery)
{	
	EFS_BEGIN

	INT ids;
	CString sTemp = _T("");
	
	///////////////////////////////////////////////////////////////
	// 1. 컨트롤 입력값을 저장한다.
	///////////////////////////////////////////////////////////////	
	const INT nCount = 3;
 
	INT nCtrlID[nCount] = { 	IDC_eUSER_NAME1	   , IDC_eUSER_NAME2	,
								IDC_eUSER_PHONE2   };		
 		
	CString sCtrlData[nCount];

	for (INT i = 0;i < nCount; i++)
	{
		GetDlgItemText(nCtrlID[i] , sCtrlData[i]);
		sCtrlData[i].TrimLeft();sCtrlData[i].TrimRight();
	}
	
	///////////////////////////////////////////////////////////////
	// 2. 회원 이름 조건을 입력한다.
	///////////////////////////////////////////////////////////////
	if ( !sCtrlData[0].IsEmpty() )
	{
		if ( !psQuery->IsEmpty() )
			*psQuery += _T(" AND ");

		if ( !sCtrlData[1].IsEmpty() )
			sTemp = _T(" CNM.NAME BETWEEN '") + sCtrlData[0] + _T("' AND '") + sCtrlData[1] + _T("힝' ");
		else
			sTemp = _T(" CNM.NAME BETWEEN '") + sCtrlData[0] + _T("' AND '") + sCtrlData[0] + _T("힝' ");

		*psQuery += sTemp;
	}
	
	///////////////////////////////////////////////////////////////
	// 3. 회원 핸드폰 조건을 입력한다.
	///////////////////////////////////////////////////////////////
	if ( !sCtrlData[2].IsEmpty() )
	{
		if ( !psQuery->IsEmpty() )
			*psQuery += _T(" AND ");

		sTemp = _T(" CNM.HANDPHONE LIKE '") + sCtrlData[2] + _T("%' ");
		*psQuery += sTemp;
	}

	///////////////////////////////////////////////////////////////
	// 4. 그룹 조건을 입력한다.
	///////////////////////////////////////////////////////////////
	ids = GetUserGroup(&sTemp);

	if ( !sTemp.IsEmpty() )
	{
		if ( !psQuery->IsEmpty() )
			*psQuery += _T(" AND ");

		CString query;
		query.Format( _T(" CNM.GROUP_CODE = '%s'"), sTemp);
		*psQuery += query;
	}

	///////////////////////////////////////////////////////////////
	// 5. 입력 테스트
	///////////////////////////////////////////////////////////////
	SetDlgItemText(IDC_eTest3, *psQuery);
	return 0;
	EFS_END
	return -1;
}

////////////////////////////////////////////////////////////////////////////
// [함수설명]
// 강좌 쿼리를 만든다.
////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong_Search::MakeQuery_Program(CString *psQuery)
{
EFS_BEGIN
	INT ids;
	CString sTemp = _T("");
	CString sWhere = _T("");
	
	///////////////////////////////////////////////////////////////
	// 1. 컨트롤 입력값을 저장한다.
	///////////////////////////////////////////////////////////////	
	const INT nCount = 7;
 
	INT nCtrlID[nCount] = { IDC_cPROGRAM_CODE  , IDC_ePROGRAM_NAME  ,
						    IDC_dPROGRAM_OPEN1 , IDC_dPROGRAM_OPEN2 ,
 							IDC_dPROGRAM_END1  , IDC_dPROGRAM_END2  ,
							IDC_cPROGRAM_COM	   }; 
 		
	CString sCtrlData[nCount];

	for (INT i = 0;i < nCount; i++)
	{
		GetDlgItemText(nCtrlID[i] , sCtrlData[i]);
		sCtrlData[i].TrimLeft();sCtrlData[i].TrimRight();
	}
	
	///////////////////////////////////////////////////////////////
	// 2. 강좌구분 조건을 입력한다.
	///////////////////////////////////////////////////////////////
	if ( sCtrlData[0].Compare(_T("적용안함")) != 0 )
	{
		if ( !psQuery->IsEmpty() )
			*psQuery += _T(" AND ");

		///////////////////////////////////////////////////////////
		// 2 - 1. 강좌 알리아스를 코드로 변경한다.
		///////////////////////////////////////////////////////////
		CESL_DataMgr *pDM = FindDM( _T("DM_SMS_LifeLong_PGCategory") );
		if(pDM==NULL) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CSMS_LifeLong_Search::MakeQuery_Program") );

		ids = pDM->RefreshDataManager(_T(""));
		if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CSMS_LifeLong_Search::MakeQuery_Program") );

		INT nCnt = pDM->GetRowCount();

		for (INT i = 0;i < nCnt;i++)
		{
			if (sCtrlData[0].Compare( pDM->GetCellData(_T("강좌구분명"), i) ) == 0)
			{
				sTemp = pDM->GetCellData(_T("코드"), i);
				sTemp = _T(" P.CATEGORY = '") + sTemp + _T("' ");
				*psQuery += sTemp;
				break;
			}
		}
	}

	///////////////////////////////////////////////////////////////
	// 3. 강좌명 조건을 입력한다.
	///////////////////////////////////////////////////////////////
	if ( !sCtrlData[1].IsEmpty() )
	{
		if ( !psQuery->IsEmpty() )
			*psQuery += _T(" AND ");

		sTemp = _T(" P.PG_NAME LIKE '%") + sCtrlData[1] + _T("%' ");
		*psQuery += sTemp;
	}

	///////////////////////////////////////////////////////////////
	// 4. 강좌개설일 조건을 입력한다.
	///////////////////////////////////////////////////////////////
	CString sStartDay=_T(""), sEndDay=_T("");

	sStartDay = GetCtrlDate( IDC_dPROGRAM_OPEN1 );
	if ( !sStartDay.IsEmpty() )
	{
		if ( !psQuery->IsEmpty() )
			*psQuery += _T(" AND ");
	
		sEndDay = GetCtrlDate( IDC_dPROGRAM_OPEN2 );
		if ( !sEndDay.IsEmpty() )
		{
			sTemp = _T(" P.OPERATION_PERIOD1 BETWEEN '") + sStartDay + _T("' AND '") + sEndDay + _T("' ");
		}
		else
			sTemp = _T(" P.OPERATION_PERIOD1 = '") + sStartDay + _T("' ");

		*psQuery += sTemp;
	}

	///////////////////////////////////////////////////////////////
	// 5. 강좌완료일 조건을 입력한다.
	///////////////////////////////////////////////////////////////
	sStartDay = GetCtrlDate( IDC_dPROGRAM_END1 );
	if ( !sStartDay.IsEmpty() )
	{
		if ( !psQuery->IsEmpty() )
			*psQuery += _T(" AND ");

	
		sEndDay = GetCtrlDate( IDC_dPROGRAM_END2 );
		if ( !sEndDay.IsEmpty() )
		{
			sTemp = _T(" P.OPERATION_PERIOD1 BETWEEN '") + sStartDay + _T("' AND '") + sEndDay + _T("' ");
		}
		else
			sTemp = _T(" P.OPERATION_PERIOD1 = '") + sStartDay + _T("' ");

		*psQuery += sTemp;
	}

	///////////////////////////////////////////////////////////////
	// 6. 수료여부 조건을 입력한다.
	///////////////////////////////////////////////////////////////
	if ( !sCtrlData[6].IsEmpty() )
	{
		if ( !psQuery->IsEmpty() )
			*psQuery += _T(" AND ");

		sTemp = _T(" PM.GRADUATION = '") + sCtrlData[6] + _T("' ");
		*psQuery += sTemp;
	}

	///////////////////////////////////////////////////////////////
	// 7. 입력 테스트
	///////////////////////////////////////////////////////////////
	SetDlgItemText(IDC_eTest1, *psQuery);

	return 0;
EFS_END
	return -1;
}

////////////////////////////////////////////////////////////////////////////
// [함수설명]
// 이용자 그룹콤보 박스 변경 이벤트
////////////////////////////////////////////////////////////////////////////
VOID CSMS_LifeLong_Search::OnSelchangecUSERGROUP() 
{
/*	///////////////////////////////////////////////////////////////////////
	// 1. 선택된 콤보 박스의 그룹명을 구한다.
	///////////////////////////////////////////////////////////////////////
	CESL_ControlMgr* pCM = FindCM(_T("CM_BO_LOC_3822_LifeLong"));
	if( pCM == NULL ) return;

	CESL_ControlMultiComboBox* pCMCB = (CESL_ControlMultiComboBox*)(pCM->FindControl(_T("회원그룹")));
	if( pCMCB == NULL ) return;
	
	INT nIndex = pCMCB->GetCurSel();
	CString sGetValue = _T("");
	pCMCB->GetData(sGetValue, nIndex, 1);

	///////////////////////////////////////////////////////////////////////
	// 2. 해당 SMS의 키를 저장한다.
	///////////////////////////////////////////////////////////////////////
	CESL_DataMgr* pDM = FindDM( _T("DM_SMS_LifeLong_UserGroup") );
	if (pDM == NULL) return;
	pDM->RefreshDataManager( _T("") );

	INT nCnt = pDM->GetRowCount();
	CString sGetUserGroup = _T("");

	for (INT i = 0;i < nCnt;i++)
	{
		pDM->GetCellData( _T("그룹이름"), i, sGetUserGroup);
		if ( _tcscmp(sGetUserGroup, sGetValue) == 0 )
		{
			pDM->GetCellData( _T("그룹코드"), i, m_sUserGroup);
			pDM->GetCellData( _T("회원상태"), i, sGetValue);
			
			if ( _tcscmp(sGetValue, _T("Y")) )
				m_nDisplayMode = 0;
			else if ( _tcscmp(sGetValue, _T("N")) )
				m_nDisplayMode = 1;

			break;
		}
	}
*/
}

////////////////////////////////////////////////////////////////////////////
// [함수설명]
// 입력되어 있는 정보에 따라서 모드가 변경된다.
////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong_Search::SetCurMode()
{
	INT ids;
	CString strTemp = _T("");

	///////////////////////////////////////////////////////////////////////
	// 1. 프로그램 정보가 있다면 m_nDisplayMode = 0
	//    강좌 구분과 강좌명만으로 판단한다.
	///////////////////////////////////////////////////////////////////////
	const INT nCount = 2;
 
	INT nCtrlID[nCount] = { 	IDC_cPROGRAM_CODE  , IDC_ePROGRAM_NAME	};
	for (INT i = 0;i < nCount;i++)
	{
		GetDlgItemText(nCtrlID[i], strTemp);
		// 값이 있다면 프로그램 테이블을 검색해야한다.
		if ( !strTemp.IsEmpty() )
		{
			m_nDisplayMode = 0;
			return 0;
		}
	}

	///////////////////////////////////////////////////////////////////////
	// 3. 회원정보만 있다면 비회원인지 정회원인지 확인해야한다.
	// 비회원은 m_nDisplayMode = 1
	// 회원은   m_nDisplayMode = 2
	///////////////////////////////////////////////////////////////////////
	ids = GetUserGroup( &strTemp );
	if (ids < 0)	ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CSMS_LifeLong_Search::SetCurMode") );

	
	if ( !strTemp.IsEmpty() )
	{
		CESL_DataMgr* pDM = FindDM( _T("DM_SMS_LifeLong_UserGroup") );
		if (pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CSMS_LifeLong_Search::SetCurMode") );
		pDM->RefreshDataManager( _T("") );

		INT nCnt = pDM->GetRowCount();
		CString sCode = _T("");

		for (i = 0;i < nCnt;i++)
		{
			pDM->GetCellData( _T("그룹코드"), i, sCode);
			if ( _tcscmp(strTemp, sCode) == 0 )
			{
				pDM->GetCellData( _T("회원상태"), i, sCode);

				// 비회원 모드
				if ( _tcscmp(sCode, _T("N")) == 0 )
				{
					m_nDisplayMode = 1;
					return 0;
				}

				break;
			}
		}		
	}

	// 여기까지 왔다면 회원 모드
	m_nDisplayMode = 2;

	return 0;
}

////////////////////////////////////////////////////////////////////////////
// [함수설명]
// 이용자 그룹박스의 코드를 리턴한다.
////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong_Search::GetUserGroup(CString *pGetGroup)
{
	EFS_BEGIN

	///////////////////////////////////////////////////////////////
	// 1. 그룹콤보박스를 포인터를 구한다.
	///////////////////////////////////////////////////////////////
	CESL_ControlMgr* pCM = FindCM(_T("CM_SMS_LifeLong_Search"));
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CSMS_LifeLong_Search::GetUserGroup") );

	CESL_ControlMultiComboBox* pCMCB = (CESL_ControlMultiComboBox*)(pCM->FindControl(_T("회원그룹")));
	if( pCMCB == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CSMS_LifeLong_Search::GetUserGroup") );
	
	///////////////////////////////////////////////////////////////
	// 2. 그룹 코드를 리턴
	///////////////////////////////////////////////////////////////
	CString sTemp = _T("");
	pCMCB->GetData( sTemp, pCMCB->GetCurSel() );
	*pGetGroup = sTemp;

	return 0;
	EFS_END
	return -1;
}

VOID CSMS_LifeLong_Search::OnCancel() 
{
	// TODO: Add extra cleanup here
//	this->DestroyWindow();	
	CDialog::OnCancel();
}
