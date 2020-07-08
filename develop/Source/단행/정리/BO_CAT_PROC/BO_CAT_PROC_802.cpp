// BO_CAT_PROC_802.cpp : implementation file
//

#include "stdafx.h"
#include "BO_CAT_PROC_802.h"
#include "..\..\..\ESL\ESL\DlgClassCode.h"
#include "..\..\..\관리\KOMARC\KOMarcGetter\KOMarcGetterDlg.h"
#include "..\..\..\공통\BO_VOLUME_FORM\BOVolumeDlg.h"

#include "..\BO_CAT_PROC_FIXEDFIELD\BO_CAT_PROC_0320_01.h"
#include "..\BO_CAT_PROC_FIXEDFIELD\BO_CAT_PROC_0321.h"
#include "..\BO_CAT_PROC_FIXEDFIELD\BO_CAT_PROC_0322.h"
#include "..\BO_CAT_PROC_FIXEDFIELD\BO_CAT_PROC_0323.h"
#include "..\BO_CAT_PROC_FIXEDFIELD\BO_CAT_PROC_0324.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0801 dialog


CBO_CAT_PROC_0801::CBO_CAT_PROC_0801(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	m_bInitDlg = TRUE;

	//{{AFX_DATA_INIT(CBO_CAT_PROC_0801)
	m_nClass = -1;
	m_nMode = -1;
	//}}AFX_DATA_INIT

	m_nModifyMode = -1;
	m_nAddMode = -1;
	m_nDeleteMode = -1;

	m_idx = -1;

	m_iPreModifyCount = 0;

	m_iPostModifyCount = 0;

	AfxInitRichEdit();
}


VOID CBO_CAT_PROC_0801::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_PROC_0801)
	DDX_Radio(pDX, IDC_rNORMAL_MARC, m_nClass);
	DDX_Radio(pDX, IDC_rUPDATE_MODE, m_nMode);
	//}}AFX_DATA_MAP

EFS_END
}

BOOL CBO_CAT_PROC_0801::Create(CWnd* pParentWnd)
{
EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}


BEGIN_MESSAGE_MAP(CBO_CAT_PROC_0801, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_PROC_0801)
	ON_BN_CLICKED(IDC_bCat0801START, OnbCat0801START)
	ON_BN_CLICKED(IDC_bAPPLY, OnbAPPLY)
	ON_BN_CLICKED(IDC_bAPPLY_ALL, OnbAPPLYALL)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MODIFY, OnSelchangeTabModify)
	ON_BN_CLICKED(IDC_bCat0801PREV, OnbCat0801PREV)
	ON_BN_CLICKED(IDC_bCat0801NEXT, OnbCat0801NEXT)
	ON_BN_CLICKED(IDC_r008_MARC, Onr008MARC)
	ON_BN_CLICKED(IDC_rNORMAL_MARC, OnrNORMALMARC)
	ON_CBN_SELCHANGE(IDC_cFORM, OnSelchangecFORM)
	ON_BN_CLICKED(IDC_bFIND_CODE_FROM, OnbFINDCODEFROM)
	ON_BN_CLICKED(IDC_bSTATIC, OnbSTATIC)
	ON_BN_CLICKED(IDC_rINSERT_MODE, OnrINSERTMODE)
	ON_BN_CLICKED(IDC_rDELETE_MODE, OnrDELETEMODE)
	ON_BN_CLICKED(IDC_rUPDATE_MODE, OnrUPDATEMODE)
	ON_BN_CLICKED(IDC_bFIND_CODE_FROM_TO, OnbFINDCODEFROMTO)
	ON_CBN_SELCHANGE(IDC_c0300FORM_CLASS, OnSelchangec0300FORMCLASS)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0801 Operations

INT CBO_CAT_PROC_0801::CheckMode(CString strTag, CString strInd, CString strCode, CString strData)
{
EFS_BEGIN

	// 수정
	if (m_nMode == MODE_MODIFY)
	{
		if (!strTag.IsEmpty() && !strCode.IsEmpty() && !strData.IsEmpty())
		{
			m_nModifyMode = MODIFY_TAG_SUBFIELD_DATA;
		}
		else if (!strTag.IsEmpty() && !strCode.IsEmpty())
		{
			m_nModifyMode = MODIFY_TAG_SUBFIELD;
		}
		else if (!strTag.IsEmpty())
		{
			m_nModifyMode = MODIFY_TAG;
		}
		else
		{	m_nModifyMode = -1;
			return -1;
		}

	}
	// 추가
	else if (m_nMode == MODE_ADD)
	{
		
		if (!strTag.IsEmpty() && !strInd.IsEmpty() && !strCode.IsEmpty() && !strData.IsEmpty())
		{
			m_nAddMode = ADD_ALL;
		}
		else 
		{
			m_nAddMode = -1;
			return -1;
		}
	}
	// 삭제
	else if (m_nMode == MODE_DELETE)
	{

		if (!strTag.IsEmpty() && !strCode.IsEmpty())
		{
			m_nDeleteMode = DELETE_TAG_SUBFIELD;
		}
		else if (!strTag.IsEmpty())
		{
			m_nDeleteMode = DELETE_TAG;
		}
		else 
		{
			m_nDeleteMode = -1;
			return -1;
		}
	}
	else
	{
		return -1;
	}

	return 0;

EFS_END
return -1;

}

INT CBO_CAT_PROC_0801::VerifyData(CString strTag, CString strInd, CString strCode, CString strData, CString strNewData)
{
EFS_BEGIN
	
	if (m_nMode == MODE_ADD && m_nAddMode == ADD_ALL)
	{
		if (strTag.IsEmpty() || strInd.IsEmpty() || strCode.IsEmpty() || strData.IsEmpty())
			{
				AfxMessageBox ( _T("변경후 항목이 틀렸습니다.") );
				return -1;
			}
	}
	// 삭제
	else if (m_nMode == MODE_DELETE)
	{
		if (m_nDeleteMode == DELETE_TAG_SUBFIELD)
		{
			if (strTag.IsEmpty() || strCode.IsEmpty())
			{
				AfxMessageBox ( _T("변경후 항목이 틀렸습니다.") );
				return -1;
			}
		}		
		else if (m_nDeleteMode == DELETE_TAG)
		{
			if (strTag.IsEmpty())
			{
				AfxMessageBox ( _T("변경후 항목이 틀렸습니다.") );
				return -1;
			}
		}		
		else 
		{		
			return -1;
		}
	}

	return 0;

EFS_END
return -1;

}

INT CBO_CAT_PROC_0801::MakeList()
{
EFS_BEGIN

	INT ids = -1;
	// Search	
	/*
	CESL_DataMgr * pDM;
	pDM = FindDM( m_strParentDMAlias );
	if ( pDM == NULL ) return -1;
	*/

	CMarc marc;
	CString strStreamMarc;
	CString strReckey;
	CArray<CString, CString> arrayErrorReckey;
	arrayErrorReckey.RemoveAll();
	
	CArray<CString, CString> arrayModifyReckey;
	arrayModifyReckey.RemoveAll();

	SelectMakeList(m_strParentCMAlias, m_strParentGridAlias);		
	INT idx = SelectGetHeadPosition(m_strParentCMAlias, m_strParentGridAlias);
	
	//Marc를 가져와 modify Likst 나 Error List 생성
	while ( idx != -1 ) {
		strStreamMarc = m_pDM_Main->GetCellData( _T("BS_MARC") , idx );
		
		//decoding
		ids = m_pInfo->pMarcMgr->Decoding(strStreamMarc, &m_marc);
		ids = m_pInfo->pMarcMgr->Decoding(strStreamMarc, &marc);
		if (ids >= 0)
		{
			// Check Match
			ids = CheckMatchCondition(&marc);
			if (ids == 0)
			{
				strReckey = m_pDM_Main->GetCellData(_T("BS_종KEY"), idx);				
				// Get Modify Reckey
				if (!strReckey.IsEmpty()) {
					arrayModifyReckey.Add(strReckey);
					m_aIdx.Add( idx );
				}
			}
			else if (ids == -200)
			{
				strReckey = m_pDM_Main->GetCellData(_T("BS_종KEY"), idx);
				// Get Error Reckey
				if (!strReckey.IsEmpty()) {
					arrayErrorReckey.Add(strReckey);
					m_aIdx.Add( idx );
				}
			}
		}

		// GetNext
		idx = SelectGetNext(m_strParentCMAlias, m_strParentGridAlias);
	}
	
	
	if ( arrayModifyReckey.GetSize() > 0)
	{
		//수정 리스트 보여주기
		ShowModifyList(arrayModifyReckey);
	}
	else if ( arrayErrorReckey.GetSize() > 0)
	{	
		// 에러 리스트 보여주기
		ShowErrorList(arrayErrorReckey);
	}

	return 0;

EFS_END
return -1;

}

INT CBO_CAT_PROC_0801::CheckMatchCondition(CMarc *pMarc)
{
EFS_BEGIN

	if (pMarc == NULL) return -1;

	// for 변경전	
	CString strTag;
	CString strInd;
	CString strCode;
	CString strData;

	GetControlData(_T("CM_BO_CAT_PROC_0800"), _T("TAG"), strTag);
	GetControlData(_T("CM_BO_CAT_PROC_0800"), _T("지시기호"), strInd);
	if (m_nClass == GENERIC)
	{
		GetControlData(_T("CM_BO_CAT_PROC_0800"), _T("식별기호"), strCode);
	}
	else if (m_nClass == STATIC)
	{
		GetControlData(_T("CM_BO_CAT_PROC_0800"), _T("식별기호콤보"), strCode);		
	}
	GetControlData(_T("CM_BO_CAT_PROC_0800"), _T("변경전DATA"), strData);

	// for 변경후
	CString strNewData;
	GetControlData(_T("CM_BO_CAT_PROC_0800"), _T("변경후DATA"), strNewData);

	INT ids = -1;
	CString strResult;

	CString sFormCode;
	GetControlData( _T("CM_BO_CAT_PROC_0800"), _T("형식구분_코드"), sFormCode );

	// 수정
	if ( m_nMode == MODE_MODIFY )
	{
		//008
		CString sData;
		if (m_nClass == STATIC)	{

			//홍수 수정
			CString sData;
			CString s008StaticLength =  Get008StaticLengthRange(strCode , sFormCode, strNewData);

			m_pInfo->pMarcMgr->GetItem( pMarc, s008StaticLength, sData );

			if ( sData == _T("") ) {
				return -200;
			}
			//입력날자 비교
			sData.TrimRight();
			if ( sData == strData ) {
				return 0;
			}
			else {
				return -100;
			}			

		}

		//일반
		if ( m_nModifyMode == MODIFY_TAG )
		{
			if (!CheckMarcData(pMarc, strTag, strInd))
				return -100;

			return 0;
		}
		else if (m_nModifyMode == MODIFY_TAG_SUBFIELD)
		{
			if (!CheckMarcData(pMarc, strTag, strInd, strCode))
				return -100;

			return 0;
		}	
		else if (m_nModifyMode == MODIFY_TAG_SUBFIELD_DATA)
		{
			if (!CheckMarcData(pMarc, strTag, strInd, strCode, strData))
				return -100;
			
			return 0;
		}
		else 
		{
			return -1;
		}
	}
	// 추가
	else if (m_nMode == MODE_ADD)
	{
		//008
		CString sData;
		if (m_nClass == STATIC)	{			
			return 0;
		}
		
		if (m_nAddMode == ADD_ALL)
		{
			if (CheckMarcData(pMarc, strTag, _T(""), strCode, _T("")))
			{				
				strCode.Replace(SUBFIELD_CODE, '$');
				ids = m_pInfo->pMarcMgr->CheckSimpleSyntax(strTag + strCode, CHK_REPEAT_SUBFIELD, strResult, FALSE);
				if (ids < 0)
				{
					return -200;
				}
			}
			return 0;
		}
		else 
		{			
			return -1;
		}
	}
	// 삭제
	else if (m_nMode == MODE_DELETE)
	{
		//008
		CString sData;
		if (m_nClass == STATIC)	{					
			return 0;
		}

		if (m_nDeleteMode == DELETE_TAG_SUBFIELD)
		{
			if (!CheckMarcData(pMarc, strTag, strInd, strCode))
			{
				return -100;
			}
		}		
		else if (m_nDeleteMode == DELETE_TAG)
		{
			if (!CheckMarcData(pMarc, strTag, strInd, strCode, strData))
			{
				return -100;
			}
		}		
		else 
		{		
			return -1;
		}
	}
	else
	{
		return -1;
	}

	return 0;

EFS_END
return -1;

}

BOOL CBO_CAT_PROC_0801::CheckMarcData(CMarc *pMarc, CString strTag /*= _T("")*/, CString strInd /*= _T("")*/, CString strSubfieldCode /*= _T("")*/, CString strData /*= _T("")*/)
{
EFS_BEGIN

	if (pMarc == NULL) return FALSE;

	CString strTemp = strSubfieldCode;
	if (!strTemp.IsEmpty())
		strSubfieldCode.Format(_T("%c%s"), SUBFIELD_CODE, strTemp);

	INT nRequestCount = 0;
	INT nResultCount = 0;

	if (!strTag.IsEmpty())
		nRequestCount ++;
	if (!strInd.IsEmpty())
		nRequestCount ++;
	if (!strSubfieldCode.IsEmpty())
		nRequestCount ++;
	if (!strData.IsEmpty())
		nRequestCount ++;
	

	POSITION posTag = NULL;
	POSITION posSubfield = NULL;
	
	CTag *pTag = NULL;
	CSubfield *pSubfield = NULL;

	posTag = pMarc->m_listTag.GetHeadPosition();
	while (posTag)
	{
		// Tag Information
		pTag = (CTag*)pMarc->m_listTag.GetNext(posTag);
		if (pTag == NULL) continue;

		nResultCount = 0;

		if (!strTag.IsEmpty())
		{
			if (pTag->GetTagCode() == strTag)
				nResultCount++;
			else
				continue;
		}

		if (!strInd.IsEmpty())
		{
			strTemp = pTag->GetFirstIndicator() + pTag->GetSecondIndicator();
			if (strInd == strTemp) 
				nResultCount++;
			else 
				continue;
		}

		// Check Match Count;
		if (nRequestCount == nResultCount)
		{
			// Find...
			return TRUE;
		}

		if (strSubfieldCode.IsEmpty())
			continue;

		// Subfield Information
		posSubfield = pTag->m_listSubfield.GetHeadPosition();
		while (posSubfield)
		{
			pSubfield = (CSubfield*)pTag->m_listSubfield.GetNext(posSubfield);
			if (pSubfield == NULL) continue;

			INT nCount = 0;
			if (strSubfieldCode == pSubfield->GetSubfieldCode())
			{
				nCount++;
				if (!strData.IsEmpty())
				{
					if (pSubfield->GetItemData().Find(strData) >= 0)
					{
						nCount ++;
					}
				}
			}

			nResultCount += nCount;
			
			// Ceck Match Count;
			if (nRequestCount == nResultCount)
			{
				// Find...
				return TRUE;
			}
			else
			{
				nResultCount -= nCount;
			}
		}
	}

	return FALSE;

EFS_END
return FALSE;

}

INT CBO_CAT_PROC_0801::Display(INT idx)
{
EFS_BEGIN

	
	if (idx < 0 )
	{
		AfxMessageBox(_T("처음 자료입니다."));
		m_idx = 0;
		return -1;
	}
	
	INT ids;

	CESL_DataMgr *pDM = FindDM(_T("DM_BO_CAT_PROC_0800_MODIFY"));
	if (pDM == NULL) return -1;

	INT nRowCount = pDM->GetRowCount();
	if (idx >= nRowCount)
	{
		if ( idx != 0 ) {
			AfxMessageBox(_T("마지막 자료입니다."));
		}
		m_idx = nRowCount - 1;
		return -1;
	}
	

	CString strMarc;
	ids = pDM->GetCellData(_T("BS_MARC"), idx, strMarc);
	if (ids < 0)
		return -1;

	CMarc marc;
	m_pInfo->pMarcMgr->Decoding(strMarc, &marc);	
	m_marcEditor.Init(m_pInfo->pMarcMgr, &marc);	
	m_pInfo->pMarcMgr->CheckDeepSyntax(&marc);
	m_marcEditor.Display();

	CString strLeader;
	marc.GetLeader(strLeader);
	SetControlData(_T("CM_BO_CAT_PROC_0800"), _T("리더"), strLeader);

	CString strRegNo;
	pDM->GetCellData(_T("시작등록번호"), idx, strRegNo);
	SetControlData(_T("CM_BO_CAT_PROC_0800"), _T("등록번호"), strRegNo);	

	GetDlgItem(IDC_bAPPLY)->EnableWindow(true);

	return 0;

EFS_END
return -1;

}

INT CBO_CAT_PROC_0801::Update(INT idx)
{
EFS_BEGIN

	if (idx < 0)
		return -1;

	INT ids;

	CESL_DataMgr *pDM = FindDM(_T("DM_BO_CAT_PROC_0800_MODIFY"));
	if (pDM == NULL) return -1;

	if (idx >= pDM->GetRowCount())
		return -1;
	

	CString strMarc;
	ids = pDM->GetCellData(_T("BS_MARC"), idx, strMarc);
	if (ids < 0)
		return -1;

	CMarc marc;
	m_pInfo->pMarcMgr->Decoding(strMarc, &marc);
	
	// for 변경전	
	CString strTag;
	CString strInd;
	CString strCode;
	CString strData;

	GetControlData(_T("CM_BO_CAT_PROC_0800"), _T("TAG"), strTag);
	GetControlData(_T("CM_BO_CAT_PROC_0800"), _T("지시기호"), strInd);

	if (m_nClass == GENERIC)
	{
		GetControlData(_T("CM_BO_CAT_PROC_0800"), _T("식별기호"), strCode);
	}
	else if (m_nClass == STATIC)
	{
		GetControlData(_T("CM_BO_CAT_PROC_0800"), _T("유형요소"), strCode);
	}
	GetControlData(_T("CM_BO_CAT_PROC_0800"), _T("변경전DATA"), strData);

	CComboBox *pComboBox = (CComboBox*)GetDlgItem(IDC_cFORM);
	INT nCurSel = pComboBox->GetCurSel();

	CString sComboAlias;
	if (nCurSel >= 0) {		
		pComboBox->GetLBText(nCurSel, sComboAlias);
	}
	
	// for 변경후
	CString strNewData;

	GetControlData(_T("CM_BO_CAT_PROC_0800"), _T("변경후DATA"), strNewData);

	CString strResult;

	CString sFormCode;
	GetControlData( _T("CM_BO_CAT_PROC_0800"), _T("형식구분_코드"), sFormCode );

	if ( m_nMode == MODE_MODIFY )
	{
		
		CString sData;
		if (m_nClass == STATIC)	{	

			//형식구분에 따라 008 고정길이 얻어오기
			CString s008StaticLength = Get008StaticLengthRange(sComboAlias, sFormCode, strNewData);

			//008 변경값을 Marc와 DB에 저장
			if ( !Update008ToMarcAndDB( &marc, pDM, s008StaticLength, strNewData, idx ) ) return -1;

			return 0;
		}
		//일반
		if ( m_nModifyMode == MODIFY_TAG )
		{
			if ( !ExecuteModifyTag( &marc, pDM, strTag, strInd, strNewData, idx ) ) return -1;
		}
		else if (m_nModifyMode == MODIFY_TAG_SUBFIELD)
		{
			if ( !ExecuteModifyTagSubfield( &marc, pDM, strTag, strCode, strNewData, idx) ) return -1;
		}	
		else if (m_nModifyMode == MODIFY_TAG_SUBFIELD_DATA)		//일반 수정
		{			
			if ( !ExecuteModifyTagSubfieldData( &marc, pDM, strTag, strCode, strData, strNewData, idx) ) return -1;
		}
		else 
		{
			return -1;
		}

	}
	else if (m_nMode == MODE_ADD)
	{
		
		CString sData;
		if (m_nClass == STATIC)	{	

			//형식구분에 따라 008 고정길이 얻어오기
			CString s008StaticLength = Get008StaticLengthRange(sComboAlias, sFormCode, strNewData);
			
			//008 변경값을 Marc와 DB에 저장
			if ( !Insert008ToMarcAndDB( &marc, pDM, s008StaticLength, strData, idx ) ) return -1;
		
			return 0;

		}
		
		if (m_nAddMode == ADD_ALL)	//일반 추가
		{
			if ( !ExecuteAddAll( &marc, pDM, strTag, strCode, strInd, strData, strNewData, idx ) ) return -1;
		}
		else 
		{			
			return -1;
		}
	}
	else if (m_nMode == MODE_DELETE)
	{
		
		CString sData;
		if (m_nClass == STATIC)	{	

			//형식구분에 따라 008 고정길이 얻어오기
			CString s008StaticLength = Get008StaticLengthRange(sComboAlias, sFormCode, strNewData);

			//008 변경값을 Marc와 DB에 저장
			if ( !Delete008FromMarcAndDB( &marc, pDM, s008StaticLength, sComboAlias, strData, idx ) ) return -1;

			return 0;
		}
		
		if (m_nDeleteMode == DELETE_TAG)
		{
			if ( !ExecuteDeleteTag(&marc, pDM, strTag, idx) ) return -1;
		}		
		else if (m_nDeleteMode == DELETE_TAG_SUBFIELD)	
		{
			if ( !ExecuteDeleteTagSubfield(&marc, pDM, strTag, strCode, idx) ) return -1;
		}
		else 
		{		
			return -1;
		}
	}
	else
	{
		return -1;
	}

	GetDlgItem(IDC_bAPPLY)->EnableWindow(false);

	return 0;

EFS_END
return -1;

}

// Return
// 100 : 사용자 입력
// 200 : 유형부호 선택
// 300 : 부호찾기
INT CBO_CAT_PROC_0801::ShowFormCodeStatus(CComboBox *pComboBox)
{
EFS_BEGIN

	if (pComboBox == NULL) return -1;

	const INT nReturn100Cnt = 9;
	const INT nReturn200Cnt = 29;
	const INT nReturn300Cnt = 4;

	CString strReturn100Data[nReturn100Cnt] = {
		_T("입력일자"), _T("발행년1"), _T("발행년2"), _T("삽도표시"),
		_T("상영시간"),		//MU
		_T("모형도")		//MP
	};

	CString strReturn200Data[nReturn200Cnt] = {
		_T("발행년유형"),
		_T("이용수준"), _T("자료식별"),
		_T("내용형식"),
		_T("수정"), _T("회의"),
		_T("기념논문"), _T("색인"),
		_T("목록전거"), _T("문학형식"),
		_T("전기"),
		_T("딸림자료1"), _T("딸림자료2"), _T("딸림자료3"), _T("딸림자료4"),
		_T("자료유형"), _T("기술수준"),
		_T("녹음자료의내용"),
		//MU
		_T("음악형식"), _T("악보형식"), _T("자료형식"),
		//MP
		_T("도법"), _T("본초자오선"), _T("지도자료형태"), _T("형태상의특징1"), _T("형태상의특징2"),
		//CF
		_T("이용대상자수준"), _T("컴퓨터파일유형"), _T("수정레코드")
	};

	CString strReturn300Data[nReturn300Cnt] = {
		_T("발행국부호"), 
		_T("한국대학부호"),
		_T("언어구분부호"),
		_T("한국정부기관부호")
	};

	INT nCurSel = pComboBox->GetCurSel();
	if (nCurSel < 0) return -1;

	CString strData;
	pComboBox->GetLBText(nCurSel, strData);
	if (strData.IsEmpty()) return -1;

	INT i = 0;
	// Check return 100
	for (i = 0; i <nReturn100Cnt; i++)
	{
		if (strData == strReturn100Data[i])
			return 100;
	}

	// Check return 200
	for (i = 0; i <nReturn200Cnt; i++)
	{
		if (strData == strReturn200Data[i])
			return 200;
	}

	// Check return 300
	for (i = 0; i <nReturn300Cnt; i++)
	{
		if (strData == strReturn300Data[i])
			return 300;
	}
		
	return -1;

EFS_END
return -1;

}

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0801 message handlers
BOOL CBO_CAT_PROC_0801::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	m_idx = 0;

	m_pDM_Main = NULL;

	if (InitESL_Mgr(_T("BO_CAT_PROC_0800")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	m_pDM_Main = FindDM( m_strParentDMAlias );
	if ( m_pDM_Main == NULL ) return -1;
	
	if (m_strParentCMAlias.IsEmpty())
		m_strParentCMAlias = _T("CM_BO_CAT_PROC_0000");
	if (m_strParentDMAlias.IsEmpty())
		m_strParentDMAlias = _T("DM_BO_CAT_PROC_0000");
	if (m_strParentGridAlias.IsEmpty())
		m_strParentGridAlias = _T("ProcGrid");

	INT ids = SelectMakeList(m_strParentCMAlias, m_strParentGridAlias);
	if ( 0 != ids ) {
		AfxMessageBox ( _T("SelectMakeList error") );
	}
	INT idx = SelectGetHeadPosition(m_strParentCMAlias, m_strParentGridAlias);
	if (idx < 0)
	{
		AfxMessageBox(_T("선정된 자료가 없습니다."));	// 에러
		EndDialog(IDCANCEL);
		m_bInitDlg = FALSE;
		return -1;
	}

	// Iinital Check Radio Box
	CheckDlgButton(IDC_rNORMAL_MARC, 1);
	CheckDlgButton(IDC_rUPDATE_MODE, 1);

	// Init Tab Ctrl
	CTabCtrl *pTabCtrl = (CTabCtrl*)GetDlgItem(IDC_TAB_MODIFY);
	if (pTabCtrl)
	{
		pTabCtrl->InsertItem(0, _T("수정리스트"));
		pTabCtrl->InsertItem(1, _T("마크"));
		pTabCtrl->InsertItem(2, _T("에러리스트"));

		pTabCtrl->SetCurFocus(1);
		pTabCtrl->SetCurFocus(0);
	}

	CString sFormCode;
	GetControlData( _T("CM_BO_CAT_PROC_0800"), _T("형식구분_코드"), sFormCode );



	const INT nComboData = 19;	
	CString strComboData[nComboData] = {
		_T("입력일자"), _T("발행년유형"),
		_T("발행년1"), _T("발행년2"),
		_T("발행국부호"), _T("삽도표시"),
		_T("이용수준"), _T("자료식별"),
		_T("내용형식"), _T("한국대학부호"),
		_T("수정"), _T("회의"),
		_T("기념논문"), _T("색인"),
		_T("목록전거"), _T("문학형식"),
		_T("전기"), _T("언어구분부호"),
		_T("한국정부기관부호")
	};
	CComboBox *pCombo = NULL;

	
	pCombo = (CComboBox*)GetDlgItem(IDC_cFORM);
	if (pCombo == NULL) return FALSE;

	for (INT j = 0; j < nComboData; j++)
	{
		pCombo->InsertString(j, strComboData[j]);
	}


	//버튼 활성화
	UpdateData();
	if (m_nClass == 0)
	{
		GetDlgItem(IDC_cFORM)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_eSUBFIELDCODE)->ShowWindow(SW_SHOW);
	}

	// Init Marc Editor
	m_marcEditor.SubclassDlgItem(IDC_REDIT_0800, -1, this);
	m_pInfo->pMarcMgr->SetMarcGroup(_T("통합마크"));

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

VOID CBO_CAT_PROC_0801::OnbCat0801START() 
{
EFS_BEGIN

	INT ids;

	UpdateData();
	
	// 데이터 가져오기
	CString strTag, strInd, strCode, strData;
	
	GetControlData(_T("CM_BO_CAT_PROC_0800"), _T("TAG"), strTag);
	GetControlData(_T("CM_BO_CAT_PROC_0800"), _T("지시기호"), strInd);
	
	if (m_nClass == GENERIC)
	{
		GetControlData(_T("CM_BO_CAT_PROC_0800"), _T("식별기호"), strCode);
	}
	else if (m_nClass == STATIC)
	{
		GetControlData(_T("CM_BO_CAT_PROC_0800"), _T("유형요소"), strCode);
	}
	GetControlData(_T("CM_BO_CAT_PROC_0800"), _T("변경전DATA"), strData);
	
	if (strTag.IsEmpty()) return;
	
	// for 변경후
	CString strNewData;
	GetControlData(_T("CM_BO_CAT_PROC_0800"), _T("변경후DATA"), strNewData);
	

	//거부리스트에 있는지 확인
	if ( !CheckNoChangeTag(strTag, strCode) ) return;


	// 모드 체크
	ids = CheckMode(strTag, strInd, strCode, strData);
	if (ids < 0) {
		AfxMessageBox ( _T("입력사항이 틀립니다.") );
		return;
	}

	// 데이터 검증
	ids = VerifyData(strTag, strInd, strCode, strData, strNewData);
	if (ids < 0) {
		AfxMessageBox ( _T("데이터 검증 에러") );
		return;
	}

	// 리스트 만들기
	ids = MakeList();
	if (ids < 0) {
		AfxMessageBox ( _T("MakeList 에러") );
		return;
	}

	// Show First Data
	if ( !ShowFirstData() ) return;

EFS_END
}

VOID CBO_CAT_PROC_0801::OnbAPPLY() 
{
EFS_BEGIN

	INT ids = Update(m_idx);
	if ( ids < 0 ) return;

	AllControlDisplay( _T("CM_BO_CAT_PROC_0800") );
	AllControlDisplay( m_strParentCMAlias );

	AfxMessageBox( _T("수정했습니다.") );

EFS_END
}

VOID CBO_CAT_PROC_0801::OnbAPPLYALL() 
{
EFS_BEGIN

	CESL_DataMgr *pDM = FindDM(_T("DM_BO_CAT_PROC_0800_MODIFY"));
	if (pDM == NULL) return;

	INT nRowCount = pDM->GetRowCount();
	for ( INT i = 0 ; i < nRowCount ; i++ ) {
		Update(i);
	}

	AllControlDisplay( _T("CM_BO_CAT_PROC_0800") );
	AllControlDisplay( m_strParentCMAlias );

	CString sMessageFormat;
	CString sPreModifyCount;

	sPreModifyCount.Format(_T("%d"), m_iPreModifyCount);

	CString sPostModifyCount;
	sPostModifyCount.Format(_T("%d"), m_iPostModifyCount);
	

		
	sMessageFormat =  _T("수정될 자료 건수 : ") + sPreModifyCount;
	sMessageFormat = sMessageFormat + _T(" 종중 \n수정된 자료 건수 : ");
	sMessageFormat = sMessageFormat + sPostModifyCount;
	sMessageFormat = sMessageFormat + _T(" 종을 \n 일괄 변경 했습니다.");

	AfxMessageBox( sMessageFormat );
	
	//m_iPreModifyCount = 0;
	m_iPostModifyCount = 0;

	GetDlgItem(IDC_bAPPLY)->EnableWindow(false);

EFS_END
}

VOID CBO_CAT_PROC_0801::OnSelchangeTabModify(NMHDR* pNMHDR, LRESULT* pResult) 
{
EFS_BEGIN

	CTabCtrl *pTabCtrl = (CTabCtrl*)GetDlgItem(IDC_TAB_MODIFY);
	if (pTabCtrl == NULL) return;

	const INT nMarcResource = 8;
	INT nArrMarcResource[nMarcResource] = {
		IDC_STATIC_REG, IDC_eACCESSION_NO,
		IDC_eLEADER, IDC_REDIT_0800,
		IDC_bCat0801PREV, IDC_bCat0801NEXT,
		IDC_bSTATIC, IDC_bAPPLY,		
	};
	//IDC_bAPPLY_ALL

	INT i = 0;
	CMSHFlexGrid *pGrid_Modify = (CMSHFlexGrid*)GetDlgItem(IDC_GRD_LIST);
	CMSHFlexGrid *pGrid_Error = (CMSHFlexGrid*)GetDlgItem(IDC_GRD_ERROR);
	if (pGrid_Error == NULL || pGrid_Modify == NULL) return;
	
	INT nCurSel = pTabCtrl->GetCurSel();
	if (nCurSel == 0)
	{
		pGrid_Modify->ShowWindow(SW_SHOW);
		pGrid_Error->ShowWindow(SW_HIDE);

		for (i = 0; i < nMarcResource; i++)
			GetDlgItem(nArrMarcResource[i])->ShowWindow(SW_HIDE);
	}
	else if (nCurSel == 1)
	{
		pGrid_Modify->ShowWindow(SW_HIDE);
		pGrid_Error->ShowWindow(SW_HIDE);

		for (i = 0; i < nMarcResource; i++)
			GetDlgItem(nArrMarcResource[i])->ShowWindow(SW_SHOW);
	}
	else if (nCurSel == 2)
	{
		pGrid_Modify->ShowWindow(SW_HIDE);
		pGrid_Error->ShowWindow(SW_SHOW);

		for (i = 0; i < nMarcResource; i++)
			GetDlgItem(nArrMarcResource[i])->ShowWindow(SW_HIDE);
	}

	

	*pResult = 0;

EFS_END
}

VOID CBO_CAT_PROC_0801::OnbCat0801PREV() 
{
EFS_BEGIN

	Display(--m_idx);
	CESL_DataMgr *pDM = FindDM(_T("DM_BO_CAT_PROC_0800_MODIFY"));
	if (pDM == NULL) return;
	INT nRowCount = pDM->GetRowCount();
	CString sRowCount;
	sRowCount.Format( _T("%d / %d") , m_idx+1, nRowCount );
	SetControlData( _T("CM_BO_CAT_PROC_0800") , _T("진행상황"), sRowCount );

	CString sFormCode = pDM->GetCellData( _T("BS_형식구분_코드"), m_idx );
	SetControlData( _T("CM_BO_CAT_PROC_0800"), _T("형식구분_코드"), sFormCode );

EFS_END
}

VOID CBO_CAT_PROC_0801::OnbCat0801NEXT() 
{
EFS_BEGIN

	Display(++m_idx);

	CESL_DataMgr *pDM = FindDM(_T("DM_BO_CAT_PROC_0800_MODIFY"));
	if (pDM == NULL) return;
	INT nRowCount = pDM->GetRowCount();
	CString sRowCount;
	sRowCount.Format( _T("%d / %d") , m_idx+1, nRowCount );
	SetControlData( _T("CM_BO_CAT_PROC_0800") , _T("진행상황"), sRowCount );

	CString sFormCode = pDM->GetCellData( _T("BS_형식구분_코드"), m_idx );
	SetControlData( _T("CM_BO_CAT_PROC_0800"), _T("형식구분_코드"), sFormCode );	

EFS_END
}

VOID CBO_CAT_PROC_0801::OnrNORMALMARC() 
{
EFS_BEGIN

	UpdateData();
	if (m_nClass == 0)
	{
		GetDlgItem(IDC_cFORM)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_eSUBFIELDCODE)->ShowWindow(SW_SHOW);
	}
	SetControlData( _T("CM_BO_CAT_PROC_0800"), _T("TAG") , _T("") );
	GetDlgItem(IDC_eTAGNO)->EnableWindow(TRUE);
	GetDlgItem(IDC_eJISI_CODE)->EnableWindow(TRUE);

	if ( m_nMode == MODE_MODIFY ) {		
		GetDlgItem(IDC_eOLD_DATA)->EnableWindow(TRUE);
		GetDlgItem(IDC_bFIND_CODE_FROM)->EnableWindow(FALSE);

		GetDlgItem(IDC_eNEW_DATA)->EnableWindow(true);
	}

	GetDlgItem(IDC_STATIC1_BCP)->EnableWindow(true);
	

	//검색 HIDE
	GetDlgItem(IDC_bFIND_CODE_FROM)->ShowWindow(SW_HIDE);

EFS_END
}

VOID CBO_CAT_PROC_0801::Onr008MARC() 
{
	EFS_BEGIN

	UpdateData();
	if (m_nClass == 1)
	{
		GetDlgItem(IDC_cFORM)->ShowWindow(SW_SHOW);	
		GetDlgItem(IDC_eSUBFIELDCODE)->ShowWindow(SW_HIDE);
	}
	SetControlData( _T("CM_BO_CAT_PROC_0800"), _T("TAG") , _T("008") );
	GetDlgItem(IDC_eTAGNO)->EnableWindow(FALSE);
	GetDlgItem(IDC_eJISI_CODE)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC1_BCP)->EnableWindow(FALSE);

	GetDlgItem(IDC_bFIND_CODE_FROM)->ShowWindow(SW_SHOW);

	//검색 나타내기
	GetDlgItem(IDC_bFIND_CODE_FROM)->ShowWindow(SW_SHOW);

	EFS_END
}

VOID CBO_CAT_PROC_0801::OnSelchangecFORM() 
{
EFS_BEGIN

	INT ids;
	// Check Show Status
	CComboBox *pComboBox = (CComboBox*)GetDlgItem(IDC_cFORM);
	if (pComboBox == NULL) return;	

	ids = ShowFormCodeStatus(pComboBox);
	if (ids == 100)
	{
		GetDlgItem(IDC_eOLD_DATA)->EnableWindow(TRUE);
		GetDlgItem(IDC_bFIND_CODE_FROM)->EnableWindow(FALSE);

		GetDlgItem(IDC_eNEW_DATA)->EnableWindow(TRUE);
	}
	else if (ids == 200)
	{
		GetDlgItem(IDC_eOLD_DATA)->EnableWindow(FALSE);
		GetDlgItem(IDC_bFIND_CODE_FROM)->EnableWindow(TRUE);

		GetDlgItem(IDC_eNEW_DATA)->EnableWindow(FALSE);
	}
	else if (ids == 300)
	{
		GetDlgItem(IDC_eOLD_DATA)->EnableWindow(FALSE);
		GetDlgItem(IDC_bFIND_CODE_FROM)->EnableWindow(TRUE);

		GetDlgItem(IDC_eNEW_DATA)->EnableWindow(FALSE);
	}
	
	CString sData;
	GetControlData( _T("CM_BO_CAT_PROC_0800"), _T("식별기호콤보"), sData );

	//pComboBoxLater->SetWindowText(sData);

EFS_END
}


VOID CBO_CAT_PROC_0801::OnbFINDCODEFROM() 
{
EFS_BEGIN

	INT ids;
	// Check Show Status
	CComboBox *pComboBox = (CComboBox*)GetDlgItem(IDC_cFORM);
	if (pComboBox == NULL) return;
	//자료유형콤보 값 가져오기
	CString sFormValue;
	pComboBox->GetWindowText(sFormValue);
	
	//형식구분코드
	CString sFormCode;
	GetControlData( _T("CM_BO_CAT_PROC_0800"), _T("형식구분_코드"), sFormCode );

	ids = ShowFormCodeStatus(pComboBox);
	if (ids == 200)
	{
		CDlgClassCode dlg;
		//파일읽기
		CString sPath = _T("..\\CFG\\008TAG코드.txt");
		//파일읽어오기
		FILE *fp;
		TCHAR cLine[256];
		fp = _tfopen( sPath , _T("rb") );
		if ( fp == NULL ) {
			AfxMessageBox( _T("008TAG코드 파일이 없습니다.") );
			return;
		}
		
		//[TX]찾기
		BOOL bFormYn = FALSE;
		while( _fgetts( cLine, 256, fp) ) {
			CString sLine;
			sLine.Format( _T("%s"), cLine );
			sLine.Replace( sLine.Right(1), NULL );
			//[TX]찾기
			CString sFormCodeCom = _T("[") + sFormCode + _T("]");
			if ( sLine == sFormCodeCom ) {
				bFormYn = TRUE;
				break;
			}
		}
		if ( bFormYn == FALSE ) {
			AfxMessageBox( _T("형식구분코드가 없습니다.") );
			return;
		}
		//(발행년유형)... 찾기
		CString sValue;
		BOOL bControlYn = FALSE;
		while( _fgetts( cLine, 256, fp) ) {
			CString sLine;
			sLine.Format( _T("%s"), cLine );
			if ( sLine.Left(1) == _T("[") ) {
				AfxMessageBox( _T("발행년유형이 없습니다.") );
				return;
			}
			sLine.Replace( sLine.Right(1), NULL );

			if ( sFormValue == _T("수정") ) {
				sFormValue = _T("수정레코드");
			}
			else if ( sFormValue == _T("딸림자료1") || sFormValue == _T("딸림자료2") || sFormValue == _T("딸림자료3") || sFormValue == _T("딸림자료4") ) {
				sFormValue = _T("008딸린자료");
			}			

			if ( sFormCode == _T("MP") ) {
				if ( sFormValue == _T("형태상의특징1") || sFormValue == _T("형태상의특징2") ) {
					sFormValue = _T("형태상특징");
				}
			}
			if ( sFormCode == _T("MU") ) {
				if ( sFormValue == _T("녹음자료의내용") ) {
					sFormValue = _T("녹음자료내용");
				}
			}
			if ( sFormCode == _T("VM") ) {
				if ( sFormValue == _T("이용수준") ) {
					sFormValue = _T("이용대상자수준");
				}
			}
			if ( sFormValue == _T("자료유형") ) {
				sFormValue = _T("시청각자료유형");
			}
			sValue = _T("(발행년유형)");
			sValue = _T("(") + sFormValue + _T(")");
			if ( sLine == sValue ) {
				bControlYn = TRUE;
				break;
			}
		}
		if ( bControlYn == FALSE ) {
			AfxMessageBox( _T("발행년유형이 없습니다.") );
			return;
		}
		
		//값 가져오기
		while( _fgetts( cLine, 256, fp) ) {
			CString sLine;
			sLine.Format( _T("%s"), cLine );
			if ( sLine.Left(1) == _T(".") ) {
				break;
			}
			sLine.Replace( sLine.Right(1), NULL );

			if ( sValue == _T("(음악형식)") || sValue == _T("(도법)") ) {
				CString sCode, sDesc;
				sCode = sLine.Left(2);
				sDesc = sLine.Mid(3);			
				dlg.AddData( sCode, sDesc );
			}
			else {
				CString sCode, sDesc;
				sCode = sLine.Left(1);
				sDesc = sLine.Mid(2);			
				dlg.AddData( sCode, sDesc );
			}
		}

		if (dlg.DoModal() == IDOK)
		{
			if ( m_iUpdateLater == 1 ) {
				SetControlData( _T("CM_BO_CAT_PROC_0800"), _T("변경후DATA"), dlg.m_strSelCode );
			}
			else {
				SetControlData( _T("CM_BO_CAT_PROC_0800"), _T("변경전DATA"), dlg.m_strSelCode );
			}
		}
	}
	else if (ids == 300) {
		CKOMarcGetterDlg dlg( this );
		INT iMode;
		if ( sFormValue == _T("발행국부호") ) {
			iMode = 0;
		}
		else if ( sFormValue == _T("한국대학부호") ) {
			iMode = 1;
		}
		else if ( sFormValue == _T("언어구분부호") ) {
			iMode = 2;
		}
		else if ( sFormValue == _T("한국정부기관부호") ) {
			iMode = 3;
		}
		dlg.InitSetProperty( iMode );
		if ( dlg.DoModal() == IDOK ) {
			CString sCode = dlg.m_code;
			if ( m_iUpdateLater == 1 ) {
				SetControlData( _T("CM_BO_CAT_PROC_0800"), _T("변경후DATA"), sCode );
			}
			else {
				SetControlData( _T("CM_BO_CAT_PROC_0800"), _T("변경전DATA"), sCode );
			}
			return;
		}
	}

EFS_END
}

VOID CBO_CAT_PROC_0801::OnbSTATIC() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CESL_DataMgr *pDM_Modify = FindDM(_T("DM_BO_CAT_PROC_0800_MODIFY"));
	CESL_DataMgr *pDM_Error = FindDM(_T("DM_BO_CAT_PROC_0800_ERROR"));
	if (pDM_Modify == NULL || pDM_Error == NULL)
		return;

	m_strStreamMarc = pDM_Modify->GetCellData( _T("BS_MARC"), m_idx );
	m_pInfo->pMarcMgr->Decoding(m_strStreamMarc, &m_marc);

	CString sFormCode;
	GetControlData(_T("CM_BO_CAT_PROC_0800"), _T("형식구분_코드"), sFormCode);

	if ( sFormCode == _T("BK") ) {
		CBO_CAT_PROC_0320_01 dlg(this, m_strParentDMAlias, m_marc);

		if( IDOK == dlg.DoModal() ) {
			CString strStreamMarc = m_strStreamMarc;
			m_pInfo->pMarcMgr->Encoding(&dlg.m_marc, m_strStreamMarc);

			//decoding
			CString strTagCode, strErrMsg, strHelpMsg;
		
			if(m_pInfo->pMarcMgr->Decoding(m_strStreamMarc, &m_marc))
			{
				POSITION pos;
				pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
				while (pos)
				{
					m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);
					UpdateData(FALSE);
				}	
			}

			m_pInfo->pMarcMgr->SortMarc( &m_marc );

			m_marcEditor.InitLimitTag();

			m_marcEditor.Init(m_pInfo->pMarcMgr, &m_marc);
			m_pInfo->pMarcMgr->CheckDeepSyntax(&m_marc);
			m_marcEditor.Display();

			//리더
			TCHAR szLeader[25];
			m_marc.GetLeader(szLeader);
			CString sLeader;
			sLeader.Format( _T("%s") , szLeader );
			SetControlData(_T("CM_BO_CAT_PROC_0800"), _T("리더"), sLeader);
		}
	}
	else if ( sFormCode == _T("CF") ) {
		CBO_CAT_PROC_0321 dlg(this, m_strParentDMAlias, m_marc);

		if( IDOK == dlg.DoModal() ) {			
			CString strStreamMarc = m_strStreamMarc;
			m_pInfo->pMarcMgr->Encoding(&dlg.m_marc, m_strStreamMarc);

			//decoding
			CString strTagCode, strErrMsg, strHelpMsg;
		
			if(m_pInfo->pMarcMgr->Decoding(m_strStreamMarc, &m_marc))
			{
				POSITION pos;
				pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
				while (pos)
				{
					m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);		
					UpdateData(FALSE);
				}	
			}

			m_pInfo->pMarcMgr->SortMarc( &m_marc );
			m_marcEditor.Init(m_pInfo->pMarcMgr, &m_marc);
			m_pInfo->pMarcMgr->CheckDeepSyntax(&m_marc);
			m_marcEditor.Display();

			//리더
			TCHAR szLeader[25];
			m_marc.GetLeader(szLeader);
			CString sLeader;
			sLeader.Format( _T("%s") , szLeader );
			SetControlData(_T("CM_BO_CAT_PROC_0800"), _T("리더"), sLeader);
		}
	}
	else if ( sFormCode == _T("MP") ) {
		CBO_CAT_PROC_0322 dlg(this, m_strParentDMAlias, m_marc);

		if( IDOK == dlg.DoModal() ) {			
			CString strStreamMarc = m_strStreamMarc;
			m_pInfo->pMarcMgr->Encoding(&dlg.m_marc, m_strStreamMarc);

			//decoding
			CString strTagCode, strErrMsg, strHelpMsg;
		
			if(m_pInfo->pMarcMgr->Decoding(m_strStreamMarc, &m_marc))
			{
				POSITION pos;
				pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
				while (pos)
				{
					m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);		
					UpdateData(FALSE);
				}	
			}

			m_pInfo->pMarcMgr->SortMarc( &m_marc );
			m_marcEditor.Init(m_pInfo->pMarcMgr, &m_marc);
			m_pInfo->pMarcMgr->CheckDeepSyntax(&m_marc);
			m_marcEditor.Display();

			//리더
			TCHAR szLeader[25];
			m_marc.GetLeader(szLeader);
			CString sLeader;
			sLeader.Format( _T("%s") , szLeader );
			SetControlData(_T("CM_BO_CAT_PROC_0800"), _T("리더"), sLeader);
		}
	}	
	else if ( sFormCode == _T("MU") ) {
		CBO_CAT_PROC_0323 dlg(this, m_strParentDMAlias, m_marc);
		if( IDOK == dlg.DoModal() ) {			
			CString strStreamMarc = m_strStreamMarc;
			m_pInfo->pMarcMgr->Encoding(&dlg.m_marc, m_strStreamMarc);

			//decoding
			CString strTagCode, strErrMsg, strHelpMsg;
		
			if(m_pInfo->pMarcMgr->Decoding(m_strStreamMarc, &m_marc))
			{
				POSITION pos;
				pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
				while (pos)
				{
					m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);		
					UpdateData(FALSE);
				}	
			}

			m_pInfo->pMarcMgr->SortMarc( &m_marc );
			m_marcEditor.Init(m_pInfo->pMarcMgr, &m_marc);
			m_pInfo->pMarcMgr->CheckDeepSyntax(&m_marc);
			m_marcEditor.Display();

			//리더
			TCHAR szLeader[25];
			m_marc.GetLeader(szLeader);
			CString sLeader;
			sLeader.Format( _T("%s") , szLeader );
			SetControlData(_T("CM_BO_CAT_PROC_0800"), _T("리더"), sLeader);
		}
	}
	else if ( sFormCode == _T("VM") ) {
		CBO_CAT_PROC_0324 dlg(this, m_strParentDMAlias, m_marc);
		if( IDOK == dlg.DoModal() ) {			
			CString strStreamMarc = m_strStreamMarc;
			m_pInfo->pMarcMgr->Encoding(&dlg.m_marc, m_strStreamMarc);

			//decoding
			CString strTagCode, strErrMsg, strHelpMsg;
		
			if(m_pInfo->pMarcMgr->Decoding(m_strStreamMarc, &m_marc))
			{
				POSITION pos;
				pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
				while (pos)
				{
					m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);		
					UpdateData(FALSE);
				}	
			}

			m_pInfo->pMarcMgr->SortMarc( &m_marc );
			m_marcEditor.Init(m_pInfo->pMarcMgr, &m_marc);
			m_pInfo->pMarcMgr->CheckDeepSyntax(&m_marc);
			m_marcEditor.Display();

			//리더
			TCHAR szLeader[25];
			m_marc.GetLeader(szLeader);
			CString sLeader;
			sLeader.Format( _T("%s") , szLeader );
			SetControlData(_T("CM_BO_CAT_PROC_0800"), _T("리더"), sLeader);
		}
	}
	else {
		//CBO_CAT_PROC_0325 dlg(this, m_marc);
		CBO_CAT_PROC_0320_01 dlg(this, m_strParentDMAlias, m_marc);
		if( IDOK == dlg.DoModal() ) {			
			CString strStreamMarc = m_strStreamMarc;
			m_pInfo->pMarcMgr->Encoding(&dlg.m_marc, m_strStreamMarc);

			//decoding
			CString strTagCode, strErrMsg, strHelpMsg;
		
			if(m_pInfo->pMarcMgr->Decoding(m_strStreamMarc, &m_marc))
			{
				POSITION pos;
				pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
				while (pos)
				{
					m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);		
					UpdateData(FALSE);
				}	
			}

			m_pInfo->pMarcMgr->SortMarc( &m_marc );
			m_marcEditor.Init(m_pInfo->pMarcMgr, &m_marc);
			m_pInfo->pMarcMgr->CheckDeepSyntax(&m_marc);
			m_marcEditor.Display();

			//리더
			TCHAR szLeader[25];
			m_marc.GetLeader(szLeader);
			CString sLeader;
			sLeader.Format( _T("%s") , szLeader );
			SetControlData(_T("CM_BO_CAT_PROC_0800"), _T("리더"), sLeader);
		}
	}
	

EFS_END
}

VOID CBO_CAT_PROC_0801::OnrINSERTMODE() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	const INT nMarcResource = 2;
	INT nArrMarcResource[nMarcResource] = {
		IDC_STATIC_DATA,
		IDC_eNEW_DATA
	};
	
	GetDlgItem(IDC_STATIC6_BCP)->EnableWindow(TRUE);
	GetDlgItem(IDC_eJISI_CODE)->EnableWindow(TRUE);
	GetDlgItem(IDC_STATIC1_BCP)->EnableWindow(TRUE);

	for ( INT i = 0; i < nMarcResource; i++) {
		GetDlgItem(nArrMarcResource[i])->EnableWindow(false);
	}

EFS_END
}

VOID CBO_CAT_PROC_0801::OnrDELETEMODE() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	const INT nMarcResource = 4;
	INT nArrMarcResource[nMarcResource] = {
		IDC_STATIC6_BCP,
		IDC_eJISI_CODE,
		IDC_STATIC_DATA,
		IDC_eNEW_DATA
	};


	GetDlgItem(IDC_STATIC1_BCP)->EnableWindow(TRUE);

	for ( INT i = 0; i < nMarcResource; i++) {
		GetDlgItem(nArrMarcResource[i])->EnableWindow(false);
	}	

EFS_END
}

VOID CBO_CAT_PROC_0801::OnrUPDATEMODE() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	const INT nMarcResource = 5;
	INT nArrMarcResource[nMarcResource] = {
			IDC_STATIC_DATA,
			IDC_eNEW_DATA, 
			IDC_STATIC1_BCP, 
			IDC_eJISI_CODE, 
			IDC_STATIC6_BCP
		};

	for ( INT i = 0; i < nMarcResource; i++) {
		GetDlgItem(nArrMarcResource[i])->EnableWindow(true);
	}

	CButton * pStartReg;
	pStartReg=(CButton *)GetDlgItem(IDC_r008_MARC);
	if ( pStartReg->GetCheck() == 1 ) {
		GetDlgItem(IDC_eNEW_DATA)->EnableWindow(FALSE);
		
	}
	else {
	}
	

EFS_END
}

VOID CBO_CAT_PROC_0801::OnCancel() 
{
EFS_BEGIN

	// TODO: Add extra cleanup here
	//DestroyWindow();
	CDialog::OnCancel();

EFS_END
}

VOID CBO_CAT_PROC_0801::OnbFINDCODEFROMTO() 
{
	// TODO: Add your control notification handler code here
	m_iUpdateLater = 1;
	OnbFINDCODEFROM();
	m_iUpdateLater = 0;
	return;

	INT ids;
	// Check Show Status
	CComboBox *pComboBox = (CComboBox*)GetDlgItem(IDC_cFORM);
	if (pComboBox == NULL) return;
	//자료유형콤보 값 가져오기
	CString sFormValue;
	pComboBox->GetWindowText(sFormValue);
	
	ids = ShowFormCodeStatus(pComboBox);
	if (ids == 200)
	{
		CDlgClassCode dlg;
		//파일읽기
		CString sPath = _T("..\\CFG\\008TAG코드.txt");
		//파일읽어오기
		FILE *fp;
		TCHAR cLine[256];
		fp = _tfopen( sPath , _T("rb") );
		if ( fp == NULL ) {
			AfxMessageBox( _T("008TAG코드 파일이 없습니다.") );
			return;
		}
		
		//[TX]찾기
		BOOL bFormYn = FALSE;
		while( _fgetts( cLine, 256, fp) ) {
			CString sLine;
			sLine.Format( _T("%s"), cLine );
			sLine.Replace( sLine.Right(1), NULL );
			//[TX]찾기						
			if ( sLine == _T("[TX]") ) {
				bFormYn = TRUE;
				break;
			}
		}
		if ( bFormYn == FALSE ) {
			AfxMessageBox( _T("[TX]가 없습니다.") );
			return;
		}
		//(발행년유형)... 찾기
		BOOL bControlYn = FALSE;
		while( _fgetts( cLine, 256, fp) ) {
			CString sLine;
			sLine.Format( _T("%s"), cLine );
			if ( sLine.Left(1) == _T("[") ) {
				return;
			}
			sLine.Replace( sLine.Right(1), NULL );
			CString sValue = _T("(발행년유형)");
			if ( sLine == sValue ) {
				bControlYn = TRUE;
				break;
			}
		}
		//값 가져오기
		while( _fgetts( cLine, 256, fp) ) {
			CString sLine;
			sLine.Format( _T("%s"), cLine );
			if ( sLine.Left(1) == _T(".") ) {
				break;
			}
			sLine.Replace( sLine.Right(1), NULL );
			CString sCode, sDesc;
			sCode = sLine.Left(1);
			sDesc = sLine.Mid(2);			
			dlg.AddData( sCode, sDesc );
		}

		if (dlg.DoModal() == IDOK)
		{
			SetControlData( _T("CM_BO_CAT_PROC_0800"), _T("변경후DATA"), dlg.m_strSelCode );
		}
	}
	else if (ids == 300) {
		CKOMarcGetterDlg dlg( this );
		INT iMode;
		if ( sFormValue == _T("발행국부호") ) {
			iMode = 0;
		}
		else if ( sFormValue == _T("한국대학부호") ) {
			iMode = 1;
		}
		else if ( sFormValue == _T("언어구분부호") ) {
			iMode = 2;
		}
		else if ( sFormValue == _T("한국정부기관부호") ) {
			iMode = 3;
		}
		dlg.InitSetProperty( iMode );
		if ( dlg.DoModal() == IDOK ) {
			CString sCode = dlg.m_code;
			SetControlData( _T("CM_BO_CAT_PROC_0800"), _T("변경후DATA"), sCode );
			return;
		}
	}
}

VOID CBO_CAT_PROC_0801::OnSelchangec0300FORMCLASS() 
{
EFS_BEGIN


	CString sFormCode;
	GetControlData( _T("CM_BO_CAT_PROC_0800"), _T("형식구분_코드"), sFormCode );

	// 문자열 콤보박스에 저장
	SetInsertStringToCombo(sFormCode);

EFS_END	
}

CString CBO_CAT_PROC_0801::Get008StaticLengthRange(CString sComboAlias, CString sFormCode, CString strNewData)
{
EFS_BEGIN

	if ( sComboAlias == _T("") ) return _T("");

	CString sStaticLengthRange;

	if ( sComboAlias == _T("입력일자") ) {
			sStaticLengthRange= _T("008@0-5");
		}
		else if ( sComboAlias == _T("발행년유형") ) {
			sStaticLengthRange= _T("008@6");
		}
		else if ( sComboAlias == _T("발행년1") ) {
			sStaticLengthRange= _T("008@7-10");
		}
		else if ( sComboAlias == _T("발행년2") ) {
			sStaticLengthRange= _T("008@11-14");
		}
		else if ( sComboAlias == _T("발행국부호") ) {
			sStaticLengthRange= _T("008@15-17");
			if ( strNewData.GetLength() == 2 ) {
				strNewData = strNewData + _T(" ");
			}
		}
		//TX
		else if ( sComboAlias == _T("삽도표시") ) {
			sStaticLengthRange= _T("008@18-21");
		}
		//TX
		else if ( sComboAlias == _T("컴퓨터파일유형") ) {
			sStaticLengthRange= _T("008@25");
		}
		//MU
		else if ( sComboAlias == _T("음악형식") ) {
			sStaticLengthRange= _T("008@18-19");
		}
		else if ( sComboAlias == _T("악보형식") ) {
			sStaticLengthRange= _T("008@20");
		}
		else if ( sComboAlias == _T("자료형식") ) {
			sStaticLengthRange= _T("008@23");
		}			
		else if ( sComboAlias == _T("딸림자료1") && sFormCode == _T("MU") ) {
			sStaticLengthRange= _T("008@24");
		}
		else if ( sComboAlias == _T("딸림자료2") && sFormCode == _T("MU") ) {
			sStaticLengthRange= _T("008@25");
		}
		else if ( sComboAlias == _T("녹음자료의내용1") ) {
			sStaticLengthRange= _T("008@30");
		}
		else if ( sComboAlias == _T("녹음자료의내용2") ) {
			sStaticLengthRange= _T("008@31");
		}

		//VM
		else if ( sComboAlias == _T("상영시간") ) {
			sStaticLengthRange= _T("008@18-20");
		}
		else if ( sComboAlias == _T("이용수준") || sComboAlias == _T("이용대상자수준") ) {
			sStaticLengthRange= _T("008@22");
		}
		else if ( sComboAlias == _T("딸림자료1") ) {
			sStaticLengthRange= _T("008@23");
		}
		else if ( sComboAlias == _T("딸림자료2") ) {
			sStaticLengthRange= _T("008@24");
		}
		else if ( sComboAlias == _T("딸림자료3") ) {
			sStaticLengthRange= _T("008@25");
		}
		else if ( sComboAlias == _T("자료유형") ) {
			sStaticLengthRange= _T("008@33");
		}
		else if ( sComboAlias == _T("기술수준") ) {
			sStaticLengthRange= _T("008@34");
		}

		
		else if ( sComboAlias == _T("자료식별") ) {
			sStaticLengthRange= _T("008@23");
		}
		else if ( sComboAlias == _T("내용형식") ) {
			sStaticLengthRange= _T("008@24-25");
		}
		else if ( sComboAlias == _T("한국대학부호") ) {
			sStaticLengthRange= _T("008@26-27");				
		}
		else if ( sComboAlias == _T("수정") ) {
			sStaticLengthRange= _T("008@28");
		}
		else if ( sComboAlias == _T("회의") ) {
			sStaticLengthRange= _T("008@29");
		}
		else if ( sComboAlias == _T("기념논문") ) {
			sStaticLengthRange= _T("008@30");
		}
		else if ( sComboAlias == _T("색인") ) {
			sStaticLengthRange= _T("008@31");
		}
		else if ( sComboAlias == _T("목록전거") ) {
			sStaticLengthRange= _T("008@32");
		}
		else if ( sComboAlias == _T("문학형식") ) {
			sStaticLengthRange= _T("008@33");
		}			
		//MP
		else if ( sComboAlias == _T("모형도") ) {
			sStaticLengthRange= _T("008@18-21");
		}
		else if ( sComboAlias == _T("도법") ) {
			sStaticLengthRange= _T("008@22-23");
		}
		else if ( sComboAlias == _T("본초자오선") ) {
			sStaticLengthRange= _T("008@24");
		}
		else if ( sComboAlias == _T("지도자료형태") ) {
			sStaticLengthRange= _T("008@25");
		}
		else if ( sComboAlias == _T("형태상의특징1") ) {
			sStaticLengthRange= _T("008@33");
		}
		else if ( sComboAlias == _T("형태상의특징2") ) {
			sStaticLengthRange= _T("008@34");
		}

		else if ( sComboAlias == _T("언어구분부호") ) {
			sStaticLengthRange= _T("008@35-37");
		}
		else if ( sComboAlias == _T("한국정부기관부호") ) {
			sStaticLengthRange= _T("008@38-39");
		}

		return sStaticLengthRange;

EFS_END
return _T("");
}

VOID CBO_CAT_PROC_0801::SetInsertStringToCombo(CString sFormCode)
{
EFS_BEGIN

	if ( sFormCode == _T("") ) return;	

	CComboBox *pComboPre = NULL;
	pComboPre = (CComboBox*)GetDlgItem(IDC_cFORM);

	//콤보의 갯수 확인
	INT iCount = pComboPre->GetCount();

	for ( INT i = iCount-1 ; i >= 0 ; i-- ) {
		pComboPre->DeleteString(i);		
	}

	// 문자열 저장
	if ( sFormCode == _T("BK") ) {
		const INT nComboData = 19;	
		CString strComboData[nComboData] = {
			_T("입력일자"), _T("발행년유형"),
			_T("발행년1"), _T("발행년2"),
			_T("발행국부호"), _T("삽도표시"),
			_T("이용수준"), _T("자료식별"),
			_T("내용형식"), _T("한국대학부호"),
			_T("수정"), _T("회의"),
			_T("기념논문"), _T("색인"),
			_T("목록전거"), _T("문학형식"),
			_T("전기"), _T("언어구분부호"),
			_T("한국정부기관부호")
		};
		CComboBox *pCombo = NULL;
		
		pCombo = (CComboBox*)GetDlgItem(IDC_cFORM);
		if (pCombo == NULL) return;

		for (INT j = 0; j < nComboData; j++)
		{
			pCombo->InsertString(j, strComboData[j]);
		}
	
	}	
	else if ( sFormCode == _T("VM") ) {
		const INT nComboData = 17;
		CString strComboData[nComboData] = {
			_T("입력일자"), _T("발행년유형"),
			_T("발행년1"), _T("발행년2"),
			_T("발행국부호"), _T("상영시간"),
			_T("이용수준"), _T("딸림자료1"), _T("딸림자료2"),
			_T("딸림자료3"),
			_T("한국대학부호"), _T("수정"), 
			_T("목록전거"), _T("자료유형"), _T("기술수준"),
			_T("언어구분부호"),	_T("한국정부기관부호")
		};
		CComboBox *pCombo = NULL;
		
		pCombo = (CComboBox*)GetDlgItem(IDC_cFORM);
		if (pCombo == NULL) return;
		
		for (INT j = 0; j < nComboData; j++)
		{
			pCombo->InsertString(j, strComboData[j]);
		}

	}
	else if ( sFormCode == _T("MU") ) {
		const INT nComboData = 18;	
		CString strComboData[nComboData] = {
			_T("입력일자"), _T("발행년유형"),
			_T("발행년1"), _T("발행년2"),
			_T("발행국부호"), _T("음악형식"),
			_T("악보형식"), _T("이용대상자수준"), _T("자료형식"),
			_T("딸림자료1"), _T("딸림자료2"),
			_T("한국대학부호"), _T("수정"),
			_T("녹음자료의내용1"), _T("녹음자료의내용2"), _T("목록전거"),
			_T("언어구분부호"),	_T("한국정부기관부호")
		};
		CComboBox *pCombo = NULL;
		
		pCombo = (CComboBox*)GetDlgItem(IDC_cFORM);
		if (pCombo == NULL) return;

		for (INT j = 0; j < nComboData; j++)
		{
			pCombo->InsertString(j, strComboData[j]);
		}
	
	}
	else if ( sFormCode == _T("MP") ) {
		const INT nComboData = 17;	
		CString strComboData[nComboData] = {
			_T("입력일자"), _T("발행년유형"),
			_T("발행년1"), _T("발행년2"),
			_T("발행국부호"), _T("모형도"),
			_T("도법"), _T("본초자오선"), _T("지도자료형태"),			
			_T("한국대학부호"), _T("수정"), _T("색인"),
			_T("목록전거"), _T("형태상의특징1"), _T("형태상의특징2"),
			_T("언어구분부호"),	_T("한국정부기관부호")
		};
		CComboBox *pCombo = NULL;
		
		pCombo = (CComboBox*)GetDlgItem(IDC_cFORM);
		if (pCombo == NULL) return;

		for (INT j = 0; j < nComboData; j++)
		{
			pCombo->InsertString(j, strComboData[j]);
		}

	}
	else if ( sFormCode == _T("CF") ) {
		const INT nComboData = 12;	
		CString strComboData[nComboData] = {
			_T("입력일자"), _T("발행년유형"),
			_T("발행년1"), _T("발행년2"),
			_T("발행국부호"), _T("이용대상자수준"),
			_T("컴퓨터파일유형"),
			_T("한국대학부호"), _T("수정"),
			_T("목록전거"),
			_T("언어구분부호"),	_T("한국정부기관부호")
		};
		CComboBox *pCombo = NULL;
		
		pCombo = (CComboBox*)GetDlgItem(IDC_cFORM);
		if (pCombo == NULL) return ;

		for (INT j = 0; j < nComboData; j++)
		{
			pCombo->InsertString(j, strComboData[j]);
		}
	
	}


EFS_END
return;
}

BOOL CBO_CAT_PROC_0801::ShowModifyList(CArray<CString, CString> &arrayModifyReckey)
{
EFS_BEGIN

	//수정 리스트 생성
	CESL_DataMgr *pDM = FindDM(_T("DM_BO_CAT_PROC_0800_MODIFY"));
	if ( pDM == NULL ) return FALSE;

	CString strOption;
	CString sBookKeyList;
	CStringArray aBookKeyList;

	for ( INT i = 0 ; i < arrayModifyReckey.GetSize() ; i++ ) {
		sBookKeyList = sBookKeyList + _T(", ") + arrayModifyReckey.GetAt(i);
		if ( ((i+1) % 999 ) == 0 ) {
			aBookKeyList.Add( sBookKeyList );
			sBookKeyList = _T("");
		}			
	}

	aBookKeyList.Add( sBookKeyList );
	sBookKeyList = aBookKeyList.GetAt(0).Mid(2);
	
	strOption = _T(" I.REC_KEY = S.REC_KEY AND I.REC_KEY IN ( ") + sBookKeyList + _T(" )");
	
	INT ids = pDM->RefreshDataManager(strOption);
	if (ids < 0 )
	{
		AfxMessageBox(_T("RefreshDataManager 를 확인해 주세요"));
		return FALSE;
	}
	
	for ( i = 1 ; i < aBookKeyList.GetSize(); i++ ) {
		CESL_DataMgr dm;
		dm.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
		sBookKeyList = aBookKeyList.GetAt(i).Mid(2);
		strOption = _T("SELECT I.AUTHOR, I.FORM_CODE, I.MEDIA_CODE, I.PUBLISHER, I.PUB_YEAR, I.REC_KEY, I.TITLE, S.ACQ_YEAR,S.DUP_FLAG,S.MARC,bo_get_max_reg_no(S.rec_key),bo_get_min_reg_no(S.rec_key) FROM IDX_BO_TBL I, BO_SPECIES_TBL S WHERE I.REC_KEY = S.REC_KEY AND I.MANAGE_CODE = UDF_MANAGE_CODE AND I.REC_KEY IN ( ") + sBookKeyList + _T(" )");
		dm.RestructDataManager(strOption);
		for ( INT j = 0 ; j < dm.GetRowCount() ; j++ ) {
			INT nModifyCount = pDM->GetRowCount();
			pDM->InsertRow(-1);
			
			CString sItem = dm.GetCellData( j, 0 );
			pDM->SetCellData( _T("IBS_저작자"), sItem, nModifyCount );
			sItem = dm.GetCellData( j, 1 );				
			pDM->SetCellData( _T("BS_형식구분_코드"), sItem, nModifyCount );
			sItem = dm.GetCellData( j, 2 );
			pDM->SetCellData( _T("BS_매체구분_코드"), sItem, nModifyCount );
			sItem = dm.GetCellData( j, 3 );
			pDM->SetCellData( _T("IBS_발행자"), sItem, nModifyCount );
			sItem = dm.GetCellData( j, 4 );
			pDM->SetCellData( _T("IBS_발행년도2"), sItem, nModifyCount );
			sItem = dm.GetCellData( j, 5 );
			pDM->SetCellData( _T("BS_종KEY"), sItem, nModifyCount );
			sItem = dm.GetCellData( j, 6 );
			pDM->SetCellData( _T("IBS_본표제"), sItem, nModifyCount );
			sItem = dm.GetCellData( j, 7 );
			pDM->SetCellData( _T("BS_수입년도"), sItem, nModifyCount );
			sItem = dm.GetCellData( j, 8 );
			pDM->SetCellData( _T("BS_복본여부"), sItem, nModifyCount );
			sItem = dm.GetCellData( j, 9 );
			pDM->SetCellData( _T("BS_MARC"), sItem, nModifyCount );
			sItem = dm.GetCellData( j, 10 );
			pDM->SetCellData( _T("끝등록번호"), sItem, nModifyCount );
			sItem = dm.GetCellData( j, 11 );
			pDM->SetCellData( _T("시작등록번호"), sItem, nModifyCount );

			
		}
	}
		
	m_iPreModifyCount = aBookKeyList.GetSize() + 1;


	pDM->SORT( _T("시작등록번호"), TRUE );
	AllControlDisplay(_T("CM_BO_CAT_PROC_0800"));

	return TRUE;

EFS_END
return FALSE;
}

BOOL CBO_CAT_PROC_0801::ShowErrorList(CArray<CString, CString> &arrayErrorReckey)
{
EFS_BEGIN

	CESL_DataMgr *pDM = FindDM(_T("DM_BO_CAT_PROC_0800_ERROR"));
	if ( pDM == NULL ) return FALSE;

	CString strOption;
	CString sBookKeyList;
	CStringArray aBookKeyList;

	
	for ( INT i = 0 ; i < arrayErrorReckey.GetSize() ; i++ ) {
		sBookKeyList = sBookKeyList + _T(", ") + arrayErrorReckey.GetAt(i);
		if ( ((i+1) % 999 ) == 0 ) {
			aBookKeyList.Add( sBookKeyList );
			sBookKeyList = _T("");
		}			
	}
	aBookKeyList.Add( sBookKeyList );
	sBookKeyList = aBookKeyList.GetAt(0).Mid(2);

	strOption = _T(" I.REC_KEY = S.REC_KEY AND I.REC_KEY IN ( ") + sBookKeyList + _T(" )");

	INT ids = pDM->RefreshDataManager(strOption);
	if (ids < 0 )
	{
		AfxMessageBox(_T("RefreshDataManager 를 확인해 주세요"));
		return FALSE;
	}		

	for ( i = 1 ; i < aBookKeyList.GetSize(); i++ ) {
		CESL_DataMgr dm;
		dm.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
		sBookKeyList = aBookKeyList.GetAt(i).Mid(2);
		strOption = _T("SELECT I.AUTHOR, I.FORM_CODE, I.MEDIA_CODE, I.PUBLISHER, I.PUB_YEAR, I.REC_KEY, I.TITLE, S.ACQ_YEAR,S.DUP_FLAG,S.MARC,bo_get_max_reg_no(S.rec_key),bo_get_min_reg_no(S.rec_key) FROM IDX_BO_TBL I, BO_SPECIES_TBL S WHERE I.REC_KEY = S.REC_KEY AND I.MANAGE_CODE = UDF_MANAGE_CODE AND I.REC_KEY IN ( ") + sBookKeyList + _T(" )");
		dm.RestructDataManager(strOption);
		for ( INT j = 0 ; j < dm.GetRowCount() ; j++ ) {
			INT nErrorCount = pDM->GetRowCount();
			pDM->InsertRow(-1);
			
			CString sItem = dm.GetCellData( j, 0 );
			pDM->SetCellData( _T("IBS_저작자"), sItem, nErrorCount );
			sItem = dm.GetCellData( j, 1 );				
			pDM->SetCellData( _T("BS_형식구분_코드"), sItem, nErrorCount );
			sItem = dm.GetCellData( j, 2 );
			pDM->SetCellData( _T("BS_매체구분_코드"), sItem, nErrorCount );
			sItem = dm.GetCellData( j, 3 );
			pDM->SetCellData( _T("IBS_발행자"), sItem, nErrorCount );
			sItem = dm.GetCellData( j, 4 );
			pDM->SetCellData( _T("IBS_발행년도2"), sItem, nErrorCount );
			sItem = dm.GetCellData( j, 5 );
			pDM->SetCellData( _T("BS_종KEY"), sItem, nErrorCount );
			sItem = dm.GetCellData( j, 6 );
			pDM->SetCellData( _T("IBS_본표제"), sItem, nErrorCount );
			sItem = dm.GetCellData( j, 7 );
			pDM->SetCellData( _T("BS_수입년도"), sItem, nErrorCount );
			sItem = dm.GetCellData( j, 8 );
			pDM->SetCellData( _T("BS_복본여부"), sItem, nErrorCount );
			sItem = dm.GetCellData( j, 9 );
			pDM->SetCellData( _T("BS_MARC"), sItem, nErrorCount );
			sItem = dm.GetCellData( j, 10 );
			pDM->SetCellData( _T("끝등록번호"), sItem, nErrorCount );
			sItem = dm.GetCellData( j, 11 );
			pDM->SetCellData( _T("시작등록번호"), sItem, nErrorCount );
			
		}
	}

	pDM->SORT( _T("시작등록번호"), TRUE );
	AllControlDisplay(_T("CM_BO_CAT_PROC_0800"));
	return TRUE;
	
EFS_END
return FALSE;

}

BOOL CBO_CAT_PROC_0801::Update008ToMarcAndDB(CMarc *marc, CESL_DataMgr *pDM, CString s008StaticLength, CString strNewData, INT idx)
{
EFS_BEGIN

	m_pInfo->pMarcMgr->SetItem( marc, s008StaticLength, strNewData );
	m_marcEditor.Init(m_pInfo->pMarcMgr, marc);	
	m_pInfo->pMarcMgr->CheckDeepSyntax(marc);
	m_marcEditor.Display();

	//db update	
	CString strMarc;
	m_pInfo->pMarcMgr->Encoding(marc, strMarc);
	m_strStreamMarc = strMarc;
	m_pInfo->pMarcMgr->Decoding( strMarc, &m_marc );

	CString sRecKey = pDM->GetCellData( _T("BS_종KEY") , idx );
	INT iCh;	
	iCh = pDM->StartFrame();
	if ( 0 > iCh ) return FALSE;

	pDM->InitDBFieldData();
	pDM->AddDBFieldData(_T("MARC"), _T("STRING"), strMarc, 1);
	pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), GetWorkLogMsg(_T("데이터일괄변경"),WORK_LOG), 1);
	INT idsTmp = pDM->MakeUpdateFrame( _T("BO_SPECIES_TBL"), _T("REC_KEY"), _T("NUMERIC"), sRecKey );
	if ( 0 > idsTmp ) return FALSE;

	iCh = pDM->SendFrame();
	m_iPostModifyCount++;

	if ( iCh == -3 || iCh == -1) {
		AfxMessageBox(_T("SendFrame Query Error!!"));
		return FALSE;
	}
	iCh = pDM->EndFrame();	
	if ( 0 > iCh ) return FALSE;

	pDM->SetCellData( _T("BS_MARC") , strMarc, idx );
	INT iMainIdx = m_aIdx.GetAt(idx);
	m_pDM_Main->SetCellData( _T("BS_MARC") , strMarc, iMainIdx );

	GetDlgItem(IDC_bAPPLY)->EnableWindow(false);



EFS_END
return FALSE;
}

BOOL CBO_CAT_PROC_0801::Insert008ToMarcAndDB(CMarc *marc, CESL_DataMgr *pDM, CString s008StaticLength, CString strData, INT idx)
{
EFS_BEGIN

	m_pInfo->pMarcMgr->SetItem( marc, s008StaticLength, strData );
	m_marcEditor.Init(m_pInfo->pMarcMgr, marc);	
	m_pInfo->pMarcMgr->CheckDeepSyntax(marc);
	m_marcEditor.Display();

	//db update			
	CString strMarc;

	m_pInfo->pMarcMgr->Encoding(marc, strMarc);
	CString sRecKey = pDM->GetCellData( _T("BS_종KEY") , idx );
	INT iCh;	
	iCh = pDM->StartFrame();
	if ( 0 > iCh ) return FALSE;

	pDM->InitDBFieldData();				
	pDM->AddDBFieldData(_T("MARC"), _T("STRING"), strMarc, 1);
	pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), GetWorkLogMsg(_T("데이터일괄변경"),WORK_LOG), 1);

	INT idsTmp = pDM->MakeUpdateFrame( _T("BO_SPECIES_TBL"), _T("REC_KEY"), _T("NUMERIC"), sRecKey );
	if ( 0 > idsTmp ) return FALSE;

	iCh = pDM->SendFrame();
	m_iPostModifyCount++;

	if ( iCh == -3 || iCh == -1) {
		AfxMessageBox(_T("SendFrame Query Error!!"));
		return -1;
	}
	iCh = pDM->EndFrame();	
	if ( 0 > iCh ) return FALSE;

	pDM->SetCellData( _T("BS_MARC") , strMarc, idx );
	INT iMainIdx = m_aIdx.GetAt(idx);
	m_pDM_Main->SetCellData( _T("BS_MARC") , strMarc, iMainIdx );

	return TRUE;

EFS_END
return FALSE;
}

/*
BOOL CBO_CAT_PROC_0801::ㅋ(CMarc *marc, CESL_DataMgr *pDM, CString s008StaticLength)
{
EFS_BEGIN

	m_pInfo->pMarcMgr->SetItem( &marc, s008StaticLength, sData );
	m_marcEditor.Init(m_pInfo->pMarcMgr, &marc);	
	m_marcEditor.Display();

	m_pInfo->pMarcMgr->Encoding(&marc, strMarc);
	CString sRecKey = pDM->GetCellData( _T("BS_종KEY") , idx );
	INT iCh;	
	iCh = pDM->StartFrame();
	if ( 0 > iCh ) return FALSE;

	pDM->InitDBFieldData();				
	pDM->AddDBFieldData(_T("MARC"), _T("STRING"), strMarc, 1);
	pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), GetWorkLogMsg(_T("데이터일괄변경"),WORK_LOG), 1);

	INT idsTmp = pDM->MakeUpdateFrame( _T("BO_SPECIES_TBL"), _T("REC_KEY"), _T("NUMERIC"), sRecKey );
	if ( 0 > idsTmp ) return FALSE;

	iCh = pDM->SendFrame();
	if ( iCh == -3 || iCh == -1) {
					AfxMessageBox(_T("SendFrame Query Error!!"));
					return FALSE;
	}
	iCh = pDM->EndFrame();
	if ( 0 > iCh ) return FALSE;

	pDM->SetCellData( _T("BS_MARC") , strMarc, idx );
	INT iMainIdx = m_aIdx.GetAt(idx);
	pDM_Main->SetCellData( _T("BS_MARC") , strMarc, iMainIdx );

	return TRUE;

EFS_END
return FALSE;
}
*/


BOOL CBO_CAT_PROC_0801::ExecuteDeleteTag(CMarc *marc, CESL_DataMgr *pDM, CString strTag, INT idx)
{
EFS_BEGIN

	m_pInfo->pMarcMgr->DeleteField(marc, strTag);
	m_marcEditor.Init(m_pInfo->pMarcMgr, marc);	
	m_pInfo->pMarcMgr->CheckDeepSyntax(marc);
	m_marcEditor.Display();

	//db update			
	CString strMarc;

	m_pInfo->pMarcMgr->Encoding(marc, strMarc);
	CString sRecKey = pDM->GetCellData( _T("BS_종KEY") , idx );
	INT iCh;	
	iCh = pDM->StartFrame();
	if ( 0 > iCh ) return -1;

	pDM->InitDBFieldData();				
	pDM->AddDBFieldData(_T("MARC"), _T("STRING"), strMarc, 1);
	pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), GetWorkLogMsg(_T("데이터일괄변경"),WORK_LOG), 1);
	INT idsTmp = pDM->MakeUpdateFrame( _T("BO_SPECIES_TBL"), _T("REC_KEY"), _T("NUMERIC"), sRecKey );
	if ( 0 > idsTmp ) return idsTmp;

	iCh = pDM->SendFrame();
	m_iPostModifyCount++;

	if ( iCh == -3 || iCh == -1) {
					AfxMessageBox(_T("SendFrame Query Error!!"));
					return -1;
	}
	iCh = pDM->EndFrame();	
	if ( 0 > iCh ) return -1;

	pDM->SetCellData( _T("BS_MARC") , strMarc, idx );
	INT iMainIdx = m_aIdx.GetAt(idx);
	m_pDM_Main->SetCellData( _T("BS_MARC") , strMarc, iMainIdx );

	return TRUE;

EFS_END
return FALSE;

}

BOOL CBO_CAT_PROC_0801::ExecuteDeleteTagSubfield(CMarc *marc, CESL_DataMgr *pDM, CString strTag, CString strCode, INT idx)
{
EFS_BEGIN

	//책dm
	CESL_DataMgr *pDmBook = FindDM(_T("DM_BO_CAT_PROC_0800_책"));
	if (pDmBook == NULL) return -1;

	m_pInfo->pMarcMgr->DeleteItem(marc, strTag + _T("$") + strCode);
	m_marcEditor.Init(m_pInfo->pMarcMgr, marc);	
	m_pInfo->pMarcMgr->CheckDeepSyntax(marc);
	m_marcEditor.Display();

	//db update			
	CString strMarc;
	m_pInfo->pMarcMgr->Encoding(marc, strMarc);
	
	CString sRecKey = pDM->GetCellData( _T("BS_종KEY") , idx );

	//책 별치기호 update
	if ( strTag == _T("049") && strCode == _T("f") ) {
		CString strWhere = _T("species_key = '") + sRecKey + _T("'");
		if ( pDmBook->RefreshDataManager(strWhere) < 0 ) return -1;

		//별치기호
		CString sLocCode;
		CArray<CString, CString&> pArray049;
		m_pInfo->pMarcMgr->GetField( marc, _T("049"), sLocCode, &pArray049 );
		CString sGt, sFt;
		sGt.Format( _T("%c"), 31 );
		sFt.Format( _T("%c"), 30 );
		CString sIndL, sIndF;
		sIndL.Format( _T("%cl"), 31 );
		sIndF.Format( _T("%cf"), 31 );

		INT iPosL = sLocCode.Find( sIndL );

		//별치기호 책 초기화
		for ( INT i = 0 ; i < pDmBook->GetRowCount() ; i++ ) {
			pDmBook->SetCellData( _T("BB_별치기호_코드"), _T(""), i );
			pDmBook->SetCellData( _T("BB_DB_FLAG"), _T("U"), i );
		}
	}

	INT iCh;	
	iCh = pDM->StartFrame();
	if ( 0 > iCh ) return -1;
	CString sQuery;

	pDM->InitDBFieldData();				
	pDM->AddDBFieldData(_T("MARC"), _T("STRING"), strMarc, 1);
	pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), GetWorkLogMsg(_T("데이터일괄변경"),WORK_LOG), 1);

	INT idsTmp = pDM->MakeUpdateFrame( _T("BO_SPECIES_TBL"), _T("REC_KEY"), _T("NUMERIC"), sRecKey );
	if ( 0 > idsTmp ) return idsTmp;

	pDM->SetCellData( _T("BS_MARC") , strMarc, idx );
	INT iMainIdx = m_aIdx.GetAt(idx);
	m_pDM_Main->SetCellData( _T("BS_MARC") , strMarc, iMainIdx );


	// 책 QUERY
	CBO_SPECIES API(this);
	API.MakeBookQueryFrame(pDmBook, sRecKey, _T("0"), pDM );

	iCh = pDM->SendFrame();
	m_iPostModifyCount++;

	if ( iCh == -3 || iCh == -1) {
		AfxMessageBox(_T("SendFrame Query Error!!"));
		return -1;
	}
	iCh = pDM->EndFrame();	
	if ( 0 > iCh ) return -1;

	return 0;

EFS_END
return -1;
}

BOOL CBO_CAT_PROC_0801::ExecuteModifyTag(CMarc *marc, CESL_DataMgr *pDM, CString strTag, CString strInd, CString strNewData, INT idx)
{
EFS_BEGIN

	CArray<CString, CString&> array;
	CString strDataFrom;
	CString strDataTo;
	m_pInfo->pMarcMgr->GetField(marc, strTag, strDataTo, &array);


	INT nCount = array.GetSize();
	for (INT i = 0; i < nCount; i++)
	{
		strDataFrom = array.GetAt(i);
		if (!strInd.IsEmpty())
		{
			// Check Indicator
			if (strInd != strDataFrom.Mid(0, 2))
				continue;
			
			strDataTo = strDataFrom.Mid(2);
		}
		else
		{
			strDataTo = strDataFrom;
		}
		
		CArray<CString, CString&> arrayGetItem;
		CString sItem = strTag + _T("$") + strDataFrom.Mid(3,1);
		CString sData;
		m_pInfo->pMarcMgr->GetItem(marc, sItem, sData, &arrayGetItem);
		
		if ( arrayGetItem.GetSize() >= i ) {
			CString sGetItem = arrayGetItem.GetAt(i);
			m_pInfo->pMarcMgr->DeleteItem(marc, sItem, sData);
		}
		
		m_pInfo->pMarcMgr->InsertField(marc, strDataTo);
					
	}
	m_pInfo->pMarcMgr->DeleteEmptySubfield(marc);
	m_pInfo->pMarcMgr->DeleteEmptyField(marc);

	m_marcEditor.Init(m_pInfo->pMarcMgr, marc);	
	m_pInfo->pMarcMgr->CheckDeepSyntax(marc);
	m_marcEditor.Display();

	//db update			
	CString strMarc;
	m_pInfo->pMarcMgr->Encoding(marc, strMarc);
	CString sRecKey = pDM->GetCellData( _T("BS_종KEY") , idx );
	INT iCh;	
	iCh = pDM->StartFrame();
	if ( 0 > iCh ) return -1;

	pDM->InitDBFieldData();
	pDM->AddDBFieldData(_T("MARC"), _T("STRING"), strMarc, 1);
	pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), GetWorkLogMsg(_T("데이터일괄변경"),WORK_LOG), 1);

	INT idsTmp = pDM->MakeUpdateFrame( _T("BO_SPECIES_TBL"), _T("REC_KEY"), _T("NUMERIC"), sRecKey );
	if ( 0 > idsTmp ) return -1;

	iCh = pDM->SendFrame();
	m_iPostModifyCount++;

	if ( iCh == -3 || iCh == -1) {
					AfxMessageBox(_T("SendFrame Query Error!!"));
					return -1;
	}
	iCh = pDM->EndFrame();	
	if ( 0 > iCh ) return -1;

	pDM->SetCellData( _T("BS_MARC") , strMarc, idx );
	INT iMainIdx = m_aIdx.GetAt(idx);
	m_pDM_Main->SetCellData( _T("BS_MARC") , strMarc, iMainIdx );

	return 0;

EFS_END
return -1;
}

BOOL CBO_CAT_PROC_0801::ExecuteModifyTagSubfield(CMarc *marc, CESL_DataMgr *pDM, CString strTag, CString strCode, CString strNewData, INT idx)
{
EFS_BEGIN

	CString strDataTo;
	m_pInfo->pMarcMgr->GetItem(marc, strTag+_T("$")+strCode, strDataTo);
	m_pInfo->pMarcMgr->DeleteItem(marc, strTag+_T("$")+strCode);

	//m_pInfo->pMarcMgr->SetItem(marc, strTag+_T("$")+strCode2, strDataTo);

	m_pInfo->pMarcMgr->DeleteEmptySubfield( marc );
	m_pInfo->pMarcMgr->DeleteEmptyField( marc );

	m_marcEditor.Init(m_pInfo->pMarcMgr, marc);	
	m_pInfo->pMarcMgr->CheckDeepSyntax(marc);
	m_marcEditor.Display();

	//db update			
	CString strMarc;
	m_pInfo->pMarcMgr->Encoding(marc, strMarc);
	CString sRecKey = pDM->GetCellData( _T("BS_종KEY") , idx );
	INT iCh;	
	iCh = pDM->StartFrame();
	if ( 0 > iCh ) return -1;

	pDM->InitDBFieldData();
	pDM->AddDBFieldData(_T("MARC"), _T("STRING"), strMarc, 1);
	pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), GetWorkLogMsg(_T("데이터일괄변경"),WORK_LOG), 1);

	INT idsTmp = pDM->MakeUpdateFrame( _T("BO_SPECIES_TBL"), _T("REC_KEY"), _T("NUMERIC"), sRecKey );
	if ( 0 > idsTmp ) return -1;

	iCh = pDM->SendFrame();
	m_iPostModifyCount++;

	if ( iCh == -3 || iCh == -1) {
					AfxMessageBox(_T("SendFrame Query Error!!"));
					return -1;
	}
	iCh = pDM->EndFrame();	
	if ( 0 > iCh ) return -1;

	pDM->SetCellData( _T("BS_MARC") , strMarc, idx );
	INT iMainIdx = m_aIdx.GetAt(idx);
	m_pDM_Main->SetCellData( _T("BS_MARC") , strMarc, iMainIdx );

	return 0;

EFS_END
return -1;
}

BOOL CBO_CAT_PROC_0801::ExecuteModifyTagSubfieldData(CMarc *marc, CESL_DataMgr *pDM, CString strTag, CString strCode, CString strData, CString strNewData, INT idx)
{
EFS_BEGIN

	CESL_DataMgr *pDmBook = FindDM(_T("DM_BO_CAT_PROC_0800_책"));
	if (pDmBook == NULL) return -1;

	CString strDataTo;
	m_pInfo->pMarcMgr->GetItem(marc, strTag+_T("$")+strCode, strDataTo);

	CString strDataFrom = strDataTo;
	strDataFrom.Replace(strData, strNewData);

	m_pInfo->pMarcMgr->DeleteItem(marc, strTag+_T("$")+strCode);
	m_pInfo->pMarcMgr->SetItem(marc, strTag+_T("$")+strCode, strDataFrom);			
	m_marcEditor.Init(m_pInfo->pMarcMgr, marc);	

	m_pInfo->pMarcMgr->DeleteEmptySubfield( marc );
	m_pInfo->pMarcMgr->DeleteEmptyField( marc );
	m_pInfo->pMarcMgr->CheckDeepSyntax(marc);
	m_marcEditor.Display();
	
	//db update						
	CString strMarc;
	m_pInfo->pMarcMgr->Encoding(marc, strMarc);
	CString sTmp;
	m_pInfo->pMarcMgr->GetItem(marc, strTag+_T("$")+strCode, sTmp );

	CString sRecKey = pDM->GetCellData( _T("BS_종KEY") , idx );

	//책 별치기호 update
	if ( strTag == _T("049") && strCode == _T("f") ) {
		CString strWhere = _T("species_key = '") + sRecKey + _T("'");
		if ( pDmBook->RefreshDataManager(strWhere) < 0 ) return -1;

		//별치기호
		CString sLocCode;
		CArray<CString, CString&> pArray049;
		m_pInfo->pMarcMgr->GetField( marc, _T("049"), sLocCode, &pArray049 );
		CString sGt, sFt;
		sGt.Format( _T("%c"), 31 );
		sFt.Format( _T("%c"), 30 );
		CString sIndL, sIndF;
		sIndL.Format( _T("%cl"), 31 );
		sIndF.Format( _T("%cf"), 31 );

		INT iPosL = sLocCode.Find( sIndL );

		//별치기호 책 초기화
		for ( INT i = 0 ; i < pDmBook->GetRowCount() ; i++ ) {
			pDmBook->SetCellData( _T("BB_별치기호_코드"), _T(""), i );
		}
		INT iPosF = sLocCode.Find( sIndF );

		CString sOriLocCode = sLocCode;

		while ( iPosF != -1 ) {		//별치기호가 없을때 까지
			iPosL = sLocCode.Find( sIndL );

			CString sRegNo;
			sRegNo = sLocCode.Mid( iPosL+2, 12 );

			while ( iPosL != -1 && iPosL < iPosF ) {	//마지막이거나 등록번호 위치가 클 경우까지
				sRegNo = sLocCode.Mid( iPosL+2, 12 );
				sLocCode = sLocCode.Mid( iPosL+1 );
				iPosL = sLocCode.Find( sIndL );
			}
			iPosF = sLocCode.Find( sIndF );

			INT iPosGT = sLocCode.Find( sGt, iPosF+1 );
			if ( iPosGT == -1 ) {
				iPosGT = sLocCode.Find( sFt, iPosF+1 );
			}
			CString sLoc = sLocCode.Mid( iPosF+2, iPosGT-iPosF-2 );
			if ( sLoc.GetLength() > 3 ) {
				AfxMessageBox( _T("별치기호는 3자리 이하입니다.") );
				return -1;
			}

			//책에 넣기
			for ( INT i = 0 ; i < pDmBook->GetRowCount() ; i++ ) {
				CString sRegNoDm = pDmBook->GetCellData( _T("BB_등록번호"), i );
				
				if ( sRegNoDm == sRegNo ) {				
					pDmBook->SetCellData( _T("BB_별치기호_코드"), sLoc, i );
					pDmBook->SetCellData( _T("BB_DB_FLAG"), _T("U"), i );
				}

			}
			sLocCode = sLocCode.Mid( iPosF+1 );
			iPosF = sLocCode.Find( sIndF );
		}				
	}


	INT iCh;	
	iCh = pDM->StartFrame();			
	if ( 0 > iCh ) return -1;
	
	CString sQuery;

	pDM->InitDBFieldData();				
	pDM->AddDBFieldData(_T("MARC"), _T("STRING"), strMarc, 1);
	pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), GetWorkLogMsg(_T("데이터일괄변경"),WORK_LOG), 1);
	INT idsTmp = pDM->MakeUpdateFrame( _T("BO_SPECIES_TBL"), _T("REC_KEY"), _T("NUMERIC"), sRecKey );
	if ( 0 > idsTmp ) return idsTmp;


	pDM->SetCellData( _T("BS_MARC") , strMarc, idx );
	INT iMainIdx = m_aIdx.GetAt(idx);
	m_pDM_Main->SetCellData( _T("BS_MARC") , strMarc, iMainIdx );


	// 책 QUERY
	CBO_SPECIES API(this);
	API.MakeBookQueryFrame(pDmBook, sRecKey, _T("0"), pDM );

	iCh = pDM->SendFrame();
	m_iPostModifyCount++;

	if ( iCh == -3 || iCh == -1) {
		AfxMessageBox(_T("SendFrame Query Error!!"));
		return -1;
	}
	iCh = pDM->EndFrame();
	if ( 0 > iCh ) return -1;

EFS_END
return FALSE;
}

BOOL CBO_CAT_PROC_0801::ExecuteAddAll(CMarc *marc, CESL_DataMgr *pDM, CString strTag, CString strCode, CString strInd, CString strData, CString strNewData, INT idx)
{
EFS_BEGIN

	//049$f 가 기존에 있는지 확인

	if ( strTag == _T("049") && strCode == _T("f") )
	{
		CString sTmp049Check = _T("");
		m_pInfo->pMarcMgr->GetItem(marc, _T("049$f") , sTmp049Check);
		if ( !sTmp049Check.IsEmpty() ) return FALSE;
	}
	
	m_pInfo->pMarcMgr->SetItem(marc, strTag + _T("$") + strCode + _T("!") +  strInd, strData);		
	INT ids = m_pInfo->pMarcMgr->CheckDeepSyntax(marc);
	m_marcEditor.Init(m_pInfo->pMarcMgr, marc);	
	m_marcEditor.Display();

	//db update		
	CString strMarc;
	m_pInfo->pMarcMgr->Encoding(marc, strMarc);
	CString sRecKey = pDM->GetCellData( _T("BS_종KEY") , idx );	


	//책 별치기호 update
	if ( strTag == _T("049") && strCode == _T("f") ) {
		
		//별치기호
		CString sLocCode;
		CArray<CString, CString&> pArray049;
		m_pInfo->pMarcMgr->GetField( marc, _T("049"), sLocCode, &pArray049 );
		
//////////////////////////////////////////////////////////////
//  지시기호 판별
//  0 - 맨 마지막 책의 별치기호를 모든 책에 별치기호로 넣는다.	
//  1 - 해당 등록번호의 별치기호를 해당 등록번호의 별치기호에 넣는다.
//////////////////////////////////////////////////////////////

		//049Tag 값을 책에 반영
		CString s049Ind;
		s049Ind = sLocCode.Left(2);
				
		if ( s049Ind == _T("0 ") )
		{
			//지시기호가 0_ 일 경우
			if ( !Insert049ValueToBook_Ind0(marc, sRecKey ) )  return -1;
		}
		else if ( s049Ind == _T("1 ") )
		{
			//지시기호가 1_ 일 경우
			if ( !Insert049ValueToBook_Ind1( sRecKey, sLocCode ) ) return -1;
		}
		else
			return -1;
			
	}

	INT iCh;
	iCh = pDM->StartFrame();
	if ( 0 > iCh ) return -1;
	
	CString sQuery;

	pDM->InitDBFieldData();				
	pDM->AddDBFieldData(_T("MARC"), _T("STRING"), strMarc, 1);
	pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), GetWorkLogMsg(_T("데이터일괄변경"),WORK_LOG), 1);
	INT idsTmp = pDM->MakeUpdateFrame( _T("BO_SPECIES_TBL"), _T("REC_KEY"), _T("NUMERIC"), sRecKey );
	if ( 0 > idsTmp ) return idsTmp;

	pDM->SetCellData( _T("BS_MARC") , strMarc, idx );
	INT iMainIdx = m_aIdx.GetAt(idx);
	m_pDM_Main->SetCellData( _T("BS_MARC") , strMarc, iMainIdx );

	// 책 QUERY
	//CBO_SPECIES API(this);
	//API.MakeBookQueryFrame(pDmBook, sRecKey, _T("0"), pDM );

	iCh = pDM->SendFrame();
	m_iPostModifyCount++;


	if ( iCh == -3 || iCh == -1) {
		AfxMessageBox(_T("SendFrame Query Error!!"));
		return -1;
	}
	iCh = pDM->EndFrame();
	if ( 0 > iCh ) return -1;

EFS_END
return FALSE;
}

BOOL CBO_CAT_PROC_0801::ShowFirstData()
{
EFS_BEGIN

	Display(m_idx);

	CESL_DataMgr *pDM = FindDM(_T("DM_BO_CAT_PROC_0800_MODIFY"));
	if (pDM == NULL) return FALSE;


	INT nRowCount = pDM->GetRowCount();
	if ( nRowCount < 1 ) {
		AfxMessageBox( _T("수정될 자료가 없습니다.") );
		GetDlgItem(IDC_bAPPLY_ALL)->EnableWindow(false);
		GetDlgItem(IDC_bAPPLY)->EnableWindow(false);
		GetDlgItem(IDC_bSTATIC)->EnableWindow(false);
		GetDlgItem(IDC_bCat0801PREV)->EnableWindow(false);
		GetDlgItem(IDC_bCat0801NEXT)->EnableWindow(false);
		return FALSE;
	}
	CString sRowCount;
	sRowCount.Format( _T("1 / %d") , nRowCount );
	SetControlData( _T("CM_BO_CAT_PROC_0800") , _T("진행상황"), sRowCount );

	if ( nRowCount > 0 ) {
		GetDlgItem(IDC_bAPPLY_ALL)->EnableWindow(TRUE);
		GetDlgItem(IDC_bAPPLY)->EnableWindow(TRUE);
		GetDlgItem(IDC_bSTATIC)->EnableWindow(TRUE);
		GetDlgItem(IDC_bCat0801PREV)->EnableWindow(TRUE);
		GetDlgItem(IDC_bCat0801NEXT)->EnableWindow(TRUE);		
	}
	else {
		AfxMessageBox( _T("수정될 자료가 없습니다.") );
		return FALSE;
	}

	CString sFormCode = pDM->GetCellData( _T("BS_형식구분_코드"), m_idx );
	SetControlData( _T("CM_BO_CAT_PROC_0800"), _T("형식구분_코드"), sFormCode );

	return TRUE;

EFS_END
return FALSE;
}

BOOL CBO_CAT_PROC_0801::Delete008FromMarcAndDB(CMarc *marc, CESL_DataMgr *pDM, CString s008StaticLength, CString sComboAlias, CString strData, INT idx)
{
EFS_BEGIN

	m_pInfo->pMarcMgr->SetItem( marc, sComboAlias, strData );
	m_marcEditor.Init(m_pInfo->pMarcMgr, marc);	
	m_pInfo->pMarcMgr->CheckDeepSyntax(marc);
	m_marcEditor.Display();

	//db update			
	CString strMarc;
	m_pInfo->pMarcMgr->Encoding(marc, strMarc);
	CString sRecKey = pDM->GetCellData( _T("BS_종KEY") , idx );
	INT iCh;	
	iCh = pDM->StartFrame();
	if ( 0 > iCh ) return -1;

	pDM->InitDBFieldData();				
	pDM->AddDBFieldData(_T("MARC"), _T("STRING"), strMarc, 1);
	pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), GetWorkLogMsg(_T("데이터일괄변경"),WORK_LOG), 1);
	INT idsTmp = pDM->MakeUpdateFrame( _T("BO_SPECIES_TBL"), _T("REC_KEY"), _T("NUMERIC"), sRecKey );
	if ( 0 > idsTmp ) return idsTmp;

	iCh = pDM->SendFrame();
	m_iPostModifyCount++;


	if ( iCh == -3 || iCh == -1) {
					AfxMessageBox(_T("SendFrame Query Error!!"));
					return -1;
	}
	iCh = pDM->EndFrame();
	if ( 0 > iCh ) return -1;

	pDM->SetCellData( _T("BS_MARC") , strMarc, idx );
	INT iMainIdx = m_aIdx.GetAt(idx);
	m_pDM_Main->SetCellData( _T("BS_MARC") , strMarc, iMainIdx );

EFS_END
return FALSE;
}

BOOL CBO_CAT_PROC_0801::CheckNoChangeTag(CString strTag, CString strCode)
{
EFS_BEGIN

	strTag.TrimLeft();
	strTag.TrimRight();
	
	strCode.TrimLeft();
	strCode.TrimRight();

	//049$l
	if ( strTag == _T("049") && strCode == _T("l")) 	
	{
		AfxMessageBox(_T("049$l은 일괄 변경 하실 수 없습니다."))	;
		return FALSE;
	}
	

	//020
	if ( strTag == _T("020") ) 	
	{
		AfxMessageBox(_T("020 Tag는 일괄 변경 하실 수 없습니다.\n 권별정보에서 수정해 주세요."))	;
		return FALSE;
	}

	return TRUE;
	
EFS_END
return FALSE;
}

CBO_CAT_PROC_0801::~CBO_CAT_PROC_0801()
{
EFS_BEGIN

	m_nModifyMode = NULL;
	m_nAddMode = NULL;
	m_nDeleteMode = NULL;
	m_marcEditor = NULL;
	m_strStreamMarc = _T("");
	m_bInitDlg = NULL;
	m_iPreModifyCount = 0;
	m_iPostModifyCount = 0;
	

EFS_END
}

BOOL CBO_CAT_PROC_0801::Insert049ValueToBook_Ind0(CMarc *pMarc, CString sSpeciesKey)
{
EFS_BEGIN
///////////////////////////////////////////////////////////////
//  0 - 맨 마지막 책의 별치기호를 모든 책에 별치기호로 넣는다.	
///////////////////////////////////////////////////////////////

	INT ids;

	//책dm
	CESL_DataMgr *pDmBook = FindDM(_T("DM_BO_CAT_PROC_0800_책"));
	if (pDmBook == NULL) return -1;

	CString strWhere = _T("species_key = '") + sSpeciesKey + _T("'");
	if ( pDmBook->RefreshDataManager(strWhere) < 0 ) return FALSE;

	CString s049ShelfCode;
	ids = m_pInfo->pMarcMgr->GetItem(pMarc, _T("049$f") , s049ShelfCode);
	if ( ids < 0 ) return FALSE;

	
	//별치기호 책 초기화
	for ( INT j = 0 ; j < pDmBook->GetRowCount() ; j++ ) {
		ids = pDmBook->SetCellData( _T("BB_별치기호_코드"), _T(""), j );
		if ( ids < 0 ) return FALSE;
	}

	//책에 넣기
	for ( INT i = 0 ; i < pDmBook->GetRowCount() ; i++ ) {
					
		CString sBookKey, sQuery;
		pDmBook->GetCellData( _T("BB_책KEY"), i, sBookKey );
		
		sQuery.Format(_T("UPDATE BO_BOOK_TBL SET SEPARATE_SHELF_CODE = '%s' WHERE rec_key = '%s';"), s049ShelfCode, sBookKey );
		ids = pDmBook->ExecuteQuery(sQuery, 1);
		if ( ids < 0 ) return FALSE;
	}

	return TRUE;

EFS_END
return FALSE;
}

BOOL CBO_CAT_PROC_0801::Insert049ValueToBook_Ind1(CString sSpeciesKey, CString sLocCode)
{
EFS_BEGIN
/////////////////////////////////////////////////////////////////////
//  1 - 해당 등록번호의 별치기호를 해당 등록번호의 별치기호에 넣는다.
/////////////////////////////////////////////////////////////////////
	
	INT ids;

	//책dm
	CESL_DataMgr *pDmBook = FindDM(_T("DM_BO_CAT_PROC_0800_책"));
	if (pDmBook == NULL) return -1;

	CString strWhere = _T("species_key = '") + sSpeciesKey + _T("'");
	if ( pDmBook->RefreshDataManager(strWhere) < 0 ) return -1;

	CString sGt, sFt;
	sGt.Format( _T("%c"), 31 );
	sFt.Format( _T("%c"), 30 );
	
	CString sIndL, sIndF;
	sIndL.Format( _T("%cl"), 31 );
	sIndF.Format( _T("%cf"), 31 );
	
	INT iPosL = sLocCode.Find( sIndL );
	INT iPosF = sLocCode.Find( sIndF );

	CString sOriLocCode = sLocCode;
	
	while ( iPosF != -1 ) {		//별치기호가 없을때 까지
		iPosL = sLocCode.Find( sIndL );
		
		CString sRegNo;
		
		while ( iPosL != -1 && iPosL < iPosF ) {	//마지막이거나 등록번호 위치가 클 경우까지
			
			//Marc에서 등록번호 가져옴
			sRegNo = sLocCode.Mid( iPosL+2, 12 );
			
			if ( sRegNo.Find( sGt ) > 0 ) sRegNo.Left( sRegNo.Find( sGt ) );
			
			if ( sRegNo.GetLength() < 12 ) sRegNo.Format( _T("%s10d"), sRegNo );
			
			sLocCode = sLocCode.Mid( iPosL+1 );
			iPosL = sLocCode.Find( sIndL );
		}
		iPosF = sLocCode.Find( sIndF );
		
		INT iPosGT = sLocCode.Find( sGt, iPosF+1 );
		if ( iPosGT == -1 ) {
			iPosGT = sLocCode.Find( sFt, iPosF+1 );
		}
		CString sLoc = sLocCode.Mid( iPosF+2, iPosGT-iPosF-2 );

		//책에 넣기
		for ( INT i = 0 ; i < pDmBook->GetRowCount() ; i++ ) {
		
			CString sRegNoDm = pDmBook->GetCellData( _T("BB_등록번호"), i );
			
			if ( sRegNoDm == sRegNo ) {				

				CString sBookKey, sQuery;
				pDmBook->GetCellData( _T("BB_책KEY"), i, sBookKey );
				
				sQuery.Format(_T("UPDATE BO_BOOK_TBL SET SEPARATE_SHELF_CODE = '%s' WHERE rec_key = '%s';"), sLoc, sBookKey );
				ids = pDmBook->ExecuteQuery(sQuery, 1);
				if ( ids < 0 ) return FALSE;

			}
		}

		sLocCode = sLocCode.Mid( iPosF+1 );
		iPosF = sLocCode.Find( sIndF );
	}	

	return TRUE;

EFS_END
return FALSE;
}
