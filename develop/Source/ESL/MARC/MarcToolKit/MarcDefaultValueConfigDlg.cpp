// MarcDefaultValueConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MarcDefaultValueConfigDlg.h"
#include "efs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMarcDefaultValueConfigDlg dialog


CMarcDefaultValueConfigDlg::CMarcDefaultValueConfigDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent) , m_cFormCode(NULL)
{
	//{{AFX_DATA_INIT(CMarcDefaultValueConfigDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_strDelimiterGroup.Format(_T("%c"), 05);
	m_strDelimiterSub.Format(_T("%c"), 06);

	m_strFileName = _T("..\\cfg\\Marc_Default_Value.cfg");
}

CMarcDefaultValueConfigDlg::~CMarcDefaultValueConfigDlg()
{
	FreeUserAliases(&m_MapUserAlias);
	FreeUserMarcs(&m_MapUserMarc);
}

VOID CMarcDefaultValueConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMarcDefaultValueConfigDlg)
	//}}AFX_DATA_MAP

	EFS_END
}

VOID CMarcDefaultValueConfigDlg::FreeUserAliases(CMapStringToPtr *pMapStringToPtr)
{
	EFS_BEGIN

	CString strKey;
	CStringArray *pStringArray = NULL;
	POSITION pos = pMapStringToPtr->GetStartPosition();
	while (pos)
	{
		pMapStringToPtr->GetNextAssoc(pos, strKey, (VOID*&)pStringArray);
		if (pStringArray)
			delete pStringArray;
	}

	pMapStringToPtr->RemoveAll();

	EFS_END
}

VOID CMarcDefaultValueConfigDlg::FreeUserMarcs(CMapStringToPtr *pMapStringToPtr)
{
	EFS_BEGIN

	CString strKey;
	CMarc *pMarc = NULL;
	POSITION pos = pMapStringToPtr->GetStartPosition();
	while (pos)
	{
		pMapStringToPtr->GetNextAssoc(pos, strKey, (VOID*&)pMarc);
		if (pMarc)
			delete pMarc;
	}

	pMapStringToPtr->RemoveAll();


	EFS_END
}

BEGIN_MESSAGE_MAP(CMarcDefaultValueConfigDlg, CDialog)
	//{{AFX_MSG_MAP(CMarcDefaultValueConfigDlg)
	ON_BN_CLICKED(IDC_bSAVE_MARC_DEFAULT_VALUE, OnbSAVEMARCDEFAULTVALUE)
	ON_BN_CLICKED(IDC_bMDF_CLOSE, OnbMDFCLOSE)
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_cFORM_CLASS_MDS, OnSelchangecFORMCLASSMDS)
	ON_BN_CLICKED(IDC_bUP, OnbUP)
	ON_BN_CLICKED(IDC_bDOWN, OnbDOWN)
	ON_BN_CLICKED(IDC_bAUTO, OnbAUTO)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMarcDefaultValueConfigDlg Operations

INT CMarcDefaultValueConfigDlg::SetMarcGroupFormCode(CString strFormCode, CString strMarcGroup)
{
	m_strFormCode = strFormCode;
	if (strMarcGroup == _T("연속"))
	{
		m_strMarcGroup = strMarcGroup;
		m_strFormCode = strFormCode;
	}
	else
	{
		m_strMarcGroup = _T("단행");
		m_strFormCode = strFormCode;
	}

	return 0;
}

INT CMarcDefaultValueConfigDlg::InitMultiComboBox()
{
	// 콤보 박스를 보여준다.
	CESL_Control_Element *pCE = new CESL_Control_Element(this);
	pCE->WorkWnd		= this;
	pCE->CM_ALIAS		= _T("");
	pCE->CTRL_ID		= IDC_MUUTICOMBO_BOX;
	pCE->CTRL_TYPE		= _T("M");
	pCE->LIST_DATA		= _T("!형식구분.코드,!형식구분.설명");
	pCE->LINK_DATA		= m_strFormCode;
	
	m_cFormCode.ELEMENT = pCE;

	m_cFormCode.FreeComboData();
	m_cFormCode.PrevRow = -1;
	m_cFormCode.PrevCol = -1;
	m_cFormCode.m_nMaxRowCnt = 0;
	
	/// 데이터 초기화
	m_cFormCode.ResetContent();
	m_cFormCode.InitMultiComboBoxRef();

	m_cFormCode.SelectString(m_strFormCode);
	
	delete pCE;

	return 0;
}

INT CMarcDefaultValueConfigDlg::InitMarcGridCtrl()
{
	EFS_BEGIN

	// Init MarcGridCtrl Alias
	if (m_pInfo->pMarcMgr == NULL) return -1;

	m_MarcGridCtrl.SetMode(SETUP_MODE);
	m_MarcGridCtrl.InitUserAlias();

	CStringArray *pStringArray = NULL;

	INT ids = -1;

	ids = GetUserMarcAlias(&m_MapUserAlias, m_strMarcGroup + m_strFormCode, &pStringArray);
	if (ids < 0)
	{
		// 없으면 새로 생성
		pStringArray = new CStringArray;
		m_MapUserAlias.SetAt(m_strMarcGroup + m_strFormCode, pStringArray);
	}

	INT nCount = pStringArray->GetSize();
	for (INT idx = 0; idx < nCount; idx++)
	{
		m_MarcGridCtrl.AddUserAlias(pStringArray->GetAt(idx));
	}


	// Init MarcGridCtrl Marc 
	CMarc *pMarc = NULL;
	ids = GetUserMarc(&m_MapUserMarc, m_strMarcGroup + m_strFormCode, &pMarc);
	if (ids < 0)
	{
		// 없으면 새로 새성
		pMarc = new CMarc;

		m_MapUserMarc.SetAt(m_strMarcGroup + m_strFormCode, pMarc);
	}

	m_MarcGridCtrl.Init(m_pInfo->pMarcMgr, pMarc);

	return 0;

	EFS_END
	return -1;

}

INT CMarcDefaultValueConfigDlg::GetUserMarcAlias(LPCTSTR lpszGroup, CStringArray **pStringArray)
{
	return GetUserMarcAlias(&m_MapUserAlias, lpszGroup, pStringArray);
}

INT CMarcDefaultValueConfigDlg::GetUserMarcAlias(CMapStringToPtr *pMapStringToPtr, LPCTSTR lpszGroup, CStringArray **pStringArray)
{
	EFS_BEGIN

	if (pMapStringToPtr == NULL || lpszGroup == NULL) return -1;

	if (!pMapStringToPtr->Lookup(lpszGroup, (VOID*&)*pStringArray))
		return -1;

	return 0;

	EFS_END
	return -1;

}

INT CMarcDefaultValueConfigDlg::GetUserMarc(LPCTSTR lpszGroup, CMarc **pMarc)
{
	return GetUserMarc(&m_MapUserMarc, lpszGroup, pMarc);
}

INT CMarcDefaultValueConfigDlg::GetUserMarc(CMapStringToPtr *pMapStringToPtr, LPCTSTR lpszGroup, CMarc **pMarc)
{
	EFS_BEGIN

	if (pMapStringToPtr == NULL || lpszGroup == NULL) return -1;

	if (!pMapStringToPtr->Lookup(lpszGroup, (VOID*&)*pMarc))
		return -1;

	return 0;

	EFS_END
	return -1;

}

INT CMarcDefaultValueConfigDlg::SortUserAlias(CStringArray *pStringArray)
{
	EFS_BEGIN

	if (pStringArray == NULL) return -1;

	INT nCount = pStringArray->GetSize();

	CString (*pStr)[2] = new CString[nCount][2];

	// KOL.UDF.022
	CString strAlias = _T("");
	CString strKeyValue = _T("   ");

	INT n = 0;

	
	for (INT idx = 0; idx < nCount; idx++)
	{
		strAlias = pStringArray->GetAt(idx);
		if (strAlias.IsEmpty()) continue;

		m_pInfo->pMarcMgr->m_mapMarcAlias.Lookup(strAlias, strKeyValue);
		if (strKeyValue.GetLength() < 4) continue;

		pStr[n][0] = strAlias;
		pStr[n][1] = strKeyValue.Left(3);

		n++;
	}

	CString strTemp;
	for (idx = 0; idx < n - 1; idx++)
	{
		for (INT j = idx + 1; j < n; j++)
		{
			if (_ttoi(pStr[idx][1]) > _ttoi(pStr[j][1]))
			{
				// Swap
				strTemp = pStr[j][0];
				pStr[j][0] = pStr[idx][0];
				pStr[idx][0] = strTemp;

				strTemp = pStr[j][1];
				pStr[j][1] = pStr[idx][1];
				pStr[idx][1] = strTemp;

			}
		}
	}

	// 다시 넣어준다.
	pStringArray->RemoveAll();
	for (idx = 0; idx < n; idx++)
	{
		pStringArray->Add(pStr[idx][0]);
	}

	if (pStr)
		delete [] pStr;

	
	return 0;

	EFS_END
	return -1;

}

INT CMarcDefaultValueConfigDlg::LoadString(LPCTSTR lpszFileName, CString &str, INT nCount /*=0*/)
{
	if (lpszFileName == NULL) return -1;

	str.Empty();

	CStdioFile file;
	if (!file.Open(lpszFileName, CFile::modeRead | CFile::typeBinary))
	{
		if (!file.Open(lpszFileName, CFile::modeRead | CFile::modeCreate | CFile::typeBinary)) 
			return -1;
		
		if( 0 == file.GetLength() )
		{			
			TCHAR cUni = 0xFEFF;
			file.Write( &cUni, sizeof(TCHAR));
		}
		
		file.Close();
	}

	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}
	
	INT n = 0;
	CString strLine;
	while (file.ReadString(strLine))
	{
		strLine.TrimLeft();
		strLine.TrimRight();
		if (strLine.IsEmpty()) continue;
		
		if (nCount == n)
		{
			str = strLine;
			break;
		}

		n++;
	}
	
	file.Close();

	return 0;
}

/*INT CMarcDefaultValueConfigDlg::SaveString(LPCTSTR lpszFileName, CString str)
{
	if (lpszFileName == NULL) return -1;

	CStdioFile file;
	if (!file.Open(m_strFileName, CFile::modeWrite | CFile::modeCreate)) return -1;

	str += _T("\n");
	
	file.WriteString(str);	
	
	file.Close();

	return 0;
}
*/

INT CMarcDefaultValueConfigDlg::LoadDefaultUserMarcAlias()
{
	return LoadDefaultUserMarcAlias(&m_MapUserAlias);
}

INT CMarcDefaultValueConfigDlg::LoadDefaultUserMarcAlias(CMapStringToPtr *pMapStringToPtr)
{
	EFS_BEGIN

	FreeUserAliases(pMapStringToPtr);

	CString str;

	LoadString(m_strFileName, str, 0);	

	if (str.IsEmpty()) return 0;

	CString strUserMarcAlias;

	CString str2 = str;
	CString strTmp;
	// 마크 그룹별로 파싱해서 가져온다.
	TCHAR szGroupName[256];
	TCHAR szAlias[1024];

	TCHAR *lpszToken = _tcstok(str2.GetBuffer(0), _T(";"));
	while (lpszToken)
	{
		// 그룹 이름과 Alias 파싱
		szGroupName[0] = 0;
		szAlias[0] = 0;
		_stscanf(lpszToken, _T("%[^:]:%s"), szGroupName, szAlias);
		
		CStringArray *pStringArray = new CStringArray;

		TCHAR *lpszSubToken = _tcstok(szAlias, _T(","));
		while (lpszSubToken)
		{
			pStringArray->Add(lpszSubToken);
			lpszSubToken = _tcstok(NULL, _T(","));
		}

		// 정렬하기
		//SortUserAlias(pStringArray);

		// Map 에 넣기
		pMapStringToPtr->SetAt(szGroupName, pStringArray);

		strTmp = str.Mid(str.Find(lpszToken) + _tcslen(lpszToken) + 1);

		lpszToken = _tcstok(strTmp.GetBuffer(0), _T(";"));
	}
	
	return 0;
	
	EFS_END
	return -1;

}

INT CMarcDefaultValueConfigDlg::LoadDefaultMarc()
{
	return LoadDefaultMarc(&m_MapUserMarc);
}

INT CMarcDefaultValueConfigDlg::LoadDefaultMarc(CMapStringToPtr *pMapStringToPtr)
{
	EFS_BEGIN

	FreeUserMarcs(pMapStringToPtr);

	CString strStreamMarc;
	
	LoadString(m_strFileName, strStreamMarc, 1);

	if (strStreamMarc.IsEmpty()) return 0;

	// 마크 그룹별로 파싱해서 가져온다.

	TCHAR szGroupName[256];
	TCHAR szMarcStream[10000];

	CString strFormat;
	strFormat.Format(_T("%%[^%s]%s%%[^\n]"), m_strDelimiterSub, m_strDelimiterSub);

	CMarc *pMarc = NULL;
	TCHAR *lpszToken = _tcstok(strStreamMarc.GetBuffer(0), m_strDelimiterGroup.GetBuffer(0));
	while (lpszToken)
	{
		// 그룹 이름과 Alias 파싱
		_stscanf(lpszToken, strFormat.GetBuffer(0), szGroupName, szMarcStream);
		if (_tcslen(szGroupName) <= 0) continue;

		pMarc = new CMarc;

		if (m_pInfo->pMarcMgr->Decoding(szMarcStream, pMarc) < 0)
		{
			delete pMarc;
		}
		else
		{	
			// Map에 넣기
			m_pInfo->pMarcMgr->CheckSimpleSyntax(pMarc);
			pMapStringToPtr->SetAt(szGroupName, pMarc);
		}

		lpszToken = _tcstok(NULL, m_strDelimiterGroup.GetBuffer(0));
	}

	return 0;

	EFS_END
	return -1;

}

INT CMarcDefaultValueConfigDlg::SaveDefaultUserMarcAlias(CString &strDefaultUserMarcAlias)
{
	EFS_BEGIN

	// 현재 그룹의 Marc Alias Array가져온다.
	CStringArray *pStringArray = NULL;
	if (!m_MapUserAlias.Lookup(m_strMarcGroup + m_strFormCode, (VOID*&)pStringArray)) return -1;
	
	// Ctrl에서 Alias를 가져와 Update한다.
	pStringArray->RemoveAll();
	m_MarcGridCtrl.GetCurrentUserMarcAlias(pStringArray);

	CString str;
	CString strKey;
	pStringArray = NULL;
	POSITION pos = m_MapUserAlias.GetStartPosition();
	while (pos)
	{
		str.Empty();
		m_MapUserAlias.GetNextAssoc(pos, strKey, (VOID*&)pStringArray);

		INT nCount = pStringArray->GetSize();
		if (nCount != 0)
		{
			// 정렬한다.
			// 2005.11.9 REM BY PDJ
			// 자동정렬은 별도로 지원, 사용자 정의 배열을 할 수 있도록 수정
			//SortUserAlias(pStringArray);
			
			for (INT idx = 0; idx < nCount -1; idx++)
				str += pStringArray->GetAt(idx) + _T(",");
			str += pStringArray->GetAt(idx);
		}

		strDefaultUserMarcAlias += strKey + _T(":") + str + _T(";");
	}

	return 0;

	EFS_END
	return -1;

}

INT CMarcDefaultValueConfigDlg::SaveDefualtMarc(CString &strDefaultMarc)
{
	EFS_BEGIN

	// 그리드의 디폴트 마크를 적용시킨다.
	m_MarcGridCtrl.MakeDefaultMarc();

	CString strKey;
	CString strStreamMarc;
	CMarc *pMarc = NULL;

	m_pInfo->pMarcMgr->SetMarcGroup(m_pInfo->pMarcMgr->GetMarcGroup());

	POSITION pos = m_MapUserMarc.GetStartPosition();
	while (pos)
	{
		m_MapUserMarc.GetNextAssoc(pos, strKey, (VOID*&)pMarc);
		if (pMarc == NULL) continue;

		if (m_pInfo->pMarcMgr->Encoding(pMarc, strStreamMarc) < 0) continue;

		strDefaultMarc += strKey + m_strDelimiterSub + strStreamMarc + m_strDelimiterGroup;
	}

	return 0;

	EFS_END
	return -1;

}

INT CMarcDefaultValueConfigDlg::Display()
{
	EFS_BEGIN

	CString strWindowText;
	strWindowText.Format(_T("마크 기본값 설정 - [%s]"), m_strMarcGroup + m_strFormCode);
	
	SetWindowText(strWindowText);

	m_MarcGridCtrl.Display();

	return 0;

	EFS_END
	return -1;

}

/////////////////////////////////////////////////////////////////////////////
// CMarcDefaultValueConfigDlg message handlers

BOOL CMarcDefaultValueConfigDlg::Create(CWnd* pParentWnd)
{
	EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;

}

BOOL CMarcDefaultValueConfigDlg::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();

	CString strMsg;

	if (m_pInfo == NULL)
	{
		strMsg = _T("m_pInfo Is NULL");
		goto ERR;
	}

	// Marc Group
	if (m_strMarcGroup.IsEmpty() || m_strFormCode.IsEmpty())
	{
		strMsg = _T("m_strMarcGroup + m_strFormCode is Empty");
		goto ERR;
	}
	
	// 멀티 콤보에서 보여준다.
	if (!m_cFormCode.SubclassDlgItem(IDC_cFORM_CLASS_MDS, this)) 
	{
		strMsg == _T("Subclass ComboBox Error");
		goto ERR;
	}

	if (InitMultiComboBox()) 
	{
		strMsg == _T("Init MulticomboBox Error");
		goto ERR;
	}

	// Subclassing Grid 
	if (!m_MarcGridCtrl.SubclassDlgItem(IDC_gMARC_DEFAULT_VALUE, this))
	{
		strMsg = _T("Subclass Marc Grid is Failed");
		goto ERR;
	}

	if (LoadDefaultUserMarcAlias() < 0)
	{
		strMsg = _T("Load Default User Marc Alias Failed");
		goto ERR;
	}

	if (LoadDefaultMarc() < 0)
	{
		strMsg = _T("Load Default Marc Failed");
		goto ERR;
	}

	if (InitMarcGridCtrl() < 0)
	{
		strMsg = _T("Init MarcGridCtrl Failed");
		goto ERR;
	}

	//=====================================================
	//2008.11.19 ADD BY PJW : XP테마 적용
	EnableThemeDialogTexture(GetSafeHwnd());
	//=====================================================
	
	
	Display();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

ERR:
	MessageBox(strMsg);
	EndDialog(IDCANCEL);

	EFS_END
	return FALSE;

}

VOID CMarcDefaultValueConfigDlg::OnbSAVEMARCDEFAULTVALUE() 
{	
	EFS_BEGIN

	CStdioFile file;
	CString strDefaultMarc;
	CString strDefaultUserMarcAlias;

	if (SaveDefualtMarc(strDefaultMarc) < 0)
		goto ERR;		
	if (SaveDefaultUserMarcAlias(strDefaultUserMarcAlias) < 0)
		goto ERR;

	// 파일에 저장한다.

	if (!file.Open(m_strFileName, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary)) goto ERR;

	if( 0 == file.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		file.Write( &cUni, sizeof(TCHAR));
	}
		
	file.WriteString(strDefaultUserMarcAlias + _T("\r\n"));
	file.WriteString(strDefaultMarc + _T("\r\n"));
	
	file.Close();
	
	goto SUCCESS;

ERR:
	MessageBox(_T("저장에 실패했습니다."));
	return;
SUCCESS:
	MessageBox(_T("마크 기본값을 저장하였습니다"));

	EFS_END
}

VOID CMarcDefaultValueConfigDlg::OnbMDFCLOSE() 
{
	EFS_BEGIN

	EndDialog(IDOK);	

	EFS_END
}

VOID CMarcDefaultValueConfigDlg::OnDestroy() 
{
	EFS_BEGIN

	CDialog::OnDestroy();

	EFS_END
}

VOID CMarcDefaultValueConfigDlg::OnSelchangecFORMCLASSMDS() 
{
	m_cFormCode.GetData(m_strFormCode);

	m_MarcGridCtrl.SetGridColWidth(120, 60, 200);
	InitMarcGridCtrl();		
	Display();
}

BEGIN_EVENTSINK_MAP(CMarcDefaultValueConfigDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CMarcDefaultValueConfigDlg)
	ON_EVENT(CMarcDefaultValueConfigDlg, IDC_gMARC_DEFAULT_VALUE, 1550 /* OLEStartDrag */, OnOLEStartDraggMARCDEFAULTVALUE, VTS_PDISPATCH VTS_PI4)
	ON_EVENT(CMarcDefaultValueConfigDlg, IDC_gMARC_DEFAULT_VALUE, 1555 /* OLEDragDrop */, OnOLEDragDropgMARCDEFAULTVALUE, VTS_PDISPATCH VTS_PI4 VTS_PI2 VTS_PI2 VTS_PR4 VTS_PR4)
	ON_EVENT(CMarcDefaultValueConfigDlg, IDC_gMARC_DEFAULT_VALUE, 1553 /* OLECompleteDrag */, OnOLECompleteDraggMARCDEFAULTVALUE, VTS_PI4)
	ON_EVENT(CMarcDefaultValueConfigDlg, IDC_gMARC_DEFAULT_VALUE, 1554 /* OLEDragOver */, OnOLEDragOvergMARCDEFAULTVALUE, VTS_PDISPATCH VTS_PI4 VTS_PI2 VTS_PI2 VTS_PR4 VTS_PR4 VTS_PI2)
	ON_EVENT(CMarcDefaultValueConfigDlg, IDC_gMARC_DEFAULT_VALUE, -606 /* MouseMove */, OnMouseMovegMARCDEFAULTVALUE, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMarcDefaultValueConfigDlg, IDC_gMARC_DEFAULT_VALUE, 1551 /* OLEGiveFeedback */, OnOLEGiveFeedbackgMARCDEFAULTVALUE, VTS_PI4 VTS_PBOOL)
	ON_EVENT(CMarcDefaultValueConfigDlg, IDC_gMARC_DEFAULT_VALUE, 1552 /* OLESetData */, OnOLESetDatagMARCDEFAULTVALUE, VTS_PDISPATCH VTS_PI2)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CMarcDefaultValueConfigDlg::OnOLEStartDraggMARCDEFAULTVALUE(LPDISPATCH FAR* Data, long FAR* AllowedEffects) 
{
	Beep(1000,70);
	
}

VOID CMarcDefaultValueConfigDlg::OnOLEDragDropgMARCDEFAULTVALUE(LPDISPATCH FAR* Data, long FAR* Effect, short FAR* Button, short FAR* Shift, float FAR* x, float FAR* y) 
{
	Beep(1000,70);	
}

VOID CMarcDefaultValueConfigDlg::OnOLECompleteDraggMARCDEFAULTVALUE(long FAR* Effect) 
{
	Beep(1000,70);	
}

VOID CMarcDefaultValueConfigDlg::OnOLEDragOvergMARCDEFAULTVALUE(LPDISPATCH FAR* Data, long FAR* Effect, short FAR* Button, short FAR* Shift, float FAR* x, float FAR* y, short FAR* State) 
{
	//Beep(1000,70);
	
}

VOID CMarcDefaultValueConfigDlg::OnMouseMovegMARCDEFAULTVALUE(short Button, short Shift, long x, long y) 
{
	//Beep(1000,70);
	
}

VOID CMarcDefaultValueConfigDlg::OnOLEGiveFeedbackgMARCDEFAULTVALUE(long FAR* Effect, BOOL FAR* DefaultCursors) 
{
	Beep(1000,70);	
	
}

VOID CMarcDefaultValueConfigDlg::OnOLESetDatagMARCDEFAULTVALUE(LPDISPATCH FAR* Data, short FAR* DataFormat) 
{
	Beep(1000,70);	
	
}

VOID CMarcDefaultValueConfigDlg::OnbUP() 
{
	MoveAlias(0);
}

VOID CMarcDefaultValueConfigDlg::OnbDOWN() 
{
	MoveAlias(1);		
}

// nFlag : 0 위로이동, 1 : 아래로 이동
INT CMarcDefaultValueConfigDlg::MoveAlias(INT nFlag)
{
	EFS_BEGIN

	CString strTemp1,strTemp2, strTemp3;
	CString strValue1,strValue2,strValue3;
	
	INT		nCount;
	INT		nRowIndex;
	INT		nMoveIdx;

	if ( nFlag == 0 ) nMoveIdx = 1;
	else nMoveIdx = -1;

	CESL_Grid* pGrid = (( CESL_Grid* ) GetDlgItem (IDC_gMARC_DEFAULT_VALUE));		
	nRowIndex = pGrid->GetRowSel();

	nCount = pGrid->GetCount();
	
	if ( nFlag == 0  && nRowIndex <= 1 ) return 0;	
	if ( nFlag == 1  && nRowIndex >= nCount-1 ) return 0;	

	strTemp1 = pGrid->GetTextMatrix(nRowIndex, 1);
	strTemp2 = pGrid->GetTextMatrix(nRowIndex, 2);
	strTemp3 = pGrid->GetTextMatrix(nRowIndex, 3);

	strValue1 = pGrid->GetTextMatrix(nRowIndex-nMoveIdx, 1);
	strValue2 = pGrid->GetTextMatrix(nRowIndex-nMoveIdx, 2);
	strValue3 = pGrid->GetTextMatrix(nRowIndex-nMoveIdx, 3);

	pGrid->SetTextMatrix(nRowIndex, 1, strValue1 );
	pGrid->SetTextMatrix(nRowIndex, 2, strValue2 );
	pGrid->SetTextMatrix(nRowIndex, 3, strValue3 );

	pGrid->SetTextMatrix(nRowIndex-nMoveIdx, 1, strTemp1 );
	pGrid->SetTextMatrix(nRowIndex-nMoveIdx, 2, strTemp2 );
	pGrid->SetTextMatrix(nRowIndex-nMoveIdx, 3, strTemp3 );			
	
	pGrid->SetReverse(nRowIndex-1);	
	pGrid->SetReverse(nRowIndex-nMoveIdx-1);	
		
	return 0;

	EFS_END
	return -1;	
}


VOID CMarcDefaultValueConfigDlg::OnbAUTO() 
{
	EFS_BEGIN	

	CString strFormCode;

	FreeUserAliases(&m_MapUserAlias);	
	m_MarcGridCtrl.SetMode(SETUP_MODE);	
	m_MarcGridCtrl.InitUserAlias();	
	m_MarcGridCtrl.SetGridColWidth(120, 60, 200);
	

	CString str;

	LoadString(m_strFileName, str, 0);	

	if (str.IsEmpty()) return ;

	CString strUserMarcAlias;

	CString str2 = str;
	CString strTmp;
	// 마크 그룹별로 파싱해서 가져온다.
	TCHAR szGroupName[256];
	TCHAR szAlias[1024];

	TCHAR *lpszToken = _tcstok(str2.GetBuffer(0), _T(";"));
	while (lpszToken)
	{
		// 그룹 이름과 Alias 파싱
		szGroupName[0] = 0;
		szAlias[0] = 0;
		_stscanf(lpszToken, _T("%[^:]:%s"), szGroupName, szAlias);
		
		CStringArray *pStringArray = new CStringArray;

		TCHAR *lpszSubToken = _tcstok(szAlias, _T(","));
		while (lpszSubToken)
		{
			pStringArray->Add(lpszSubToken);
			lpszSubToken = _tcstok(NULL, _T(","));
		}

		// 정렬하기
		if ( m_strMarcGroup+m_strFormCode == szGroupName )
		{
			// 2005.11.9 DESC BY PDJ
			// 콤보박스의 항목 표시 이므로 정렬을 사용한다.
			SortUserAlias(pStringArray);
		}

		// Map 에 넣기
		m_MapUserAlias.SetAt(szGroupName, pStringArray);

		strTmp = str.Mid(str.Find(lpszToken) + _tcslen(lpszToken) + 1);

		lpszToken = _tcstok(strTmp.GetBuffer(0), _T(";"));

		if ( m_strMarcGroup+m_strFormCode == szGroupName )
		{
			INT nCount = pStringArray->GetSize();
			for (INT idx = 0; idx < nCount; idx++)
			{
				m_MarcGridCtrl.AddUserAlias(pStringArray->GetAt(idx));
			}
		}
	}

	Display();
	
	return;

	EFS_END
	return;
}
