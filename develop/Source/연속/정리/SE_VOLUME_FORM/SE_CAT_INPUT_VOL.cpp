// SE_CAT_INPUT_VOL.cpp : implementation file
//

#include "stdafx.h"
#include "SE_CAT_INPUT_VOL.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_CAT_INPUT_VOL dialog


CSE_CAT_INPUT_VOL::CSE_CAT_INPUT_VOL(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	m_pParent = NULL;
	m_pParent = pParent;

	m_pCM = NULL;
	m_pDM_Volume = NULL;
	m_pDM_Species = NULL;
	m_pDM_Index = NULL;

//	m_pCatApi = NULL;
}

CSE_CAT_INPUT_VOL::~CSE_CAT_INPUT_VOL()
{
}

VOID CSE_CAT_INPUT_VOL::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSE_CAT_INPUT_VOL, CDialog)
	//{{AFX_MSG_MAP(CSE_CAT_INPUT_VOL)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_CAT_INPUT_VOL message handlers

BOOL CSE_CAT_INPUT_VOL::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();

	if (InitESL_Mgr(_T("SM_연속_정리_권입력")) < 0)
	{
		AfxMessageBox ( _T("Init SM_연속_정리_권입력 Error") );
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_연속_정리_권"));
	if ( m_pCM == NULL ) 
	{
		AfxMessageBox ( _T("Init CM_연속_정리_권 Error") );
		return FALSE;
	}

	m_pDM_Volume = FindDM(_T("DM_연속_정리DM_권정보"));
	if ( m_pDM_Volume == NULL ) 
	{
		AfxMessageBox ( _T("Init DM_연속_정리DM_권정보 Error") );
		return FALSE;
	}

	m_pDM_Species = FindDM(_T("DM_연속_정리DM_종정보"));
	if ( m_pDM_Species == NULL ) 
	{
		AfxMessageBox ( _T("Init DM_연속_정리DM_종정보 Error") );
		return FALSE;
	}

	m_pDM_Index = FindDM(_T("DM_연속_정리DM_색인정보"));
	if ( m_pDM_Index == NULL ) 
	{
		AfxMessageBox ( _T("Init DM_연속_정리DM_색인정보 Error") );
		return FALSE;
	}

	// 값 뿌리기
	//m_pCM->AllControlDisplay(0);

	// 초기값 설정
	INT ids = InitData();

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;

EFS_END
	return FALSE;
}

INT CSE_CAT_INPUT_VOL::InitData()
{
	CString strItemData;
	INT ids;

	// Combo 초기화 ********************************************
	// 책입력상태
	CComboBox * pCombo;
	pCombo = ( CComboBox * )GetDlgItem( IDC_INPUT_BOOK_STATUS );
	if(pCombo)
	{
		pCombo->InsertString( 0, _T("정리대상자료") );
		pCombo->InsertString( 1, _T("배가자료(소장자료)") );
		pCombo->SetCurSel( 0 );
	}

	// 권호구분
	pCombo = NULL;
	pCombo = ( CComboBox * )GetDlgItem( IDC_cmbVOL_VOLUME_CLASS );
	if(pCombo)
	{
		pCombo->InsertString( 0, _T("일반") );
		pCombo->InsertString( 1, _T("합병호") );
		pCombo->SetCurSel( 0 );
	}

	// 가격성격
	pCombo = NULL;
	pCombo = ( CComboBox * )GetDlgItem( IDC_cmbVOL_PRICE_CHARACTER );
	if(pCombo)
	{
		pCombo->InsertString( 0, _T("정가") );
		pCombo->InsertString( 1, _T("비매품") );
		pCombo->InsertString( 2, _T("가격불명") );
		strItemData.Format(_T(""));
		m_pDM_Species->GetCellData(_T("SS_가격성격"), 0, strItemData);
		if ( !strItemData.Compare(_T("정가")) ) pCombo->SetCurSel( 0 );
		else if ( !strItemData.Compare(_T("비매품")) ) pCombo->SetCurSel( 1 );
		else if ( !strItemData.Compare(_T("가격불명")) ) pCombo->SetCurSel( 2 );
	}
	// ********************************************************

	// 마크 가져오기
	CMarc marc;
	CString strStreamMarc;
	ids = m_pDM_Species->GetCellData( _T("SS_MARC"), 0, strStreamMarc);
	ids = this->m_pInfo->pMarcMgr->Decoding(strStreamMarc, &marc);

	// 본서명
	strItemData.Format(_T(""));
	this->m_pInfo->pMarcMgr->GetItem(&marc,_T("245$a"),strItemData);
	m_pCM->SetControlMgrData(_T("SV_본서명"), strItemData );

	// 형식구분
	m_pCM->SetControlMgrData(_T("SV_형식구분_코드"), m_pDM_Species->GetCellData(_T("SS_형식구분_코드"), 0) );

	// 매체구분
	m_pCM->SetControlMgrData(_T("SV_매체구분_코드"), m_pDM_Species->GetCellData(_T("SS_매체구분_코드"), 0) );

	// 권호일련번호
	strItemData = m_pDM_Species->GetCellData(_T("SS_최종권호일련번호"), 0);
	strItemData.Format(_T("%d"), _ttoi(strItemData)+10 );
	m_pCM->SetControlMgrData(_T("SV_권호일련번호"), strItemData );

	// 화폐구분
	m_pCM->SetControlMgrData(_T("SS_화폐구분_코드"), m_pDM_Species->GetCellData(_T("SS_화폐구분_코드"), 0) );

	//가격
	m_pCM->SetControlMgrData(_T("SS_가격"), m_pDM_Species->GetCellData(_T("SS_가격"), 0) );

	//가격성격
	m_pCM->SetControlMgrData(_T("SV_가격성격"), m_pDM_Species->GetCellData(_T("SS_가격성격"), 0) );

	// 면장수
	strItemData.Format(_T(""));
	this->m_pInfo->pMarcMgr->GetItem(&marc,_T("300$a"),strItemData);
	m_pCM->SetControlMgrData(_T("SV_면장수"), strItemData );

	// 물리적특성
	strItemData.Format(_T(""));
	this->m_pInfo->pMarcMgr->GetItem(&marc,_T("300$b"),strItemData);
	m_pCM->SetControlMgrData(_T("SV_물리적특성"), strItemData );

	// 책크기
	strItemData.Format(_T(""));
	this->m_pInfo->pMarcMgr->GetItem(&marc,_T("300$c"),strItemData);
	m_pCM->SetControlMgrData(_T("SV_책크기"), strItemData );

	// 딸림자료
	strItemData.Format(_T(""));
	this->m_pInfo->pMarcMgr->GetItem(&marc,_T("300$d"),strItemData);
	m_pCM->SetControlMgrData(_T("SV_딸림자료"), strItemData );

	return 0;
}

VOID CSE_CAT_INPUT_VOL::OnOK() 
{
	// 권정보 입력
	INT ids = SaveDataToVolDM();

	// 책정보 입력

	CDialog::OnOK();
}

INT CSE_CAT_INPUT_VOL::SaveDataToVolDM()
{
	CComboBox * pCombo;
	CString strItemData;
	INT ids;
	INT nCurSel = 0;

	// 1/ 권입력 ++++++++++++++++++++++++++++++++++++++++++++++++
	m_pDM_Volume->InsertRow(-1);
	INT nCurtVolIdx = m_pDM_Volume->GetRowCount() -1;

	const INT cnt  = 17;
	CString alias[cnt] = {
			_T("SV_가격"),
			_T("SV_가격성격"),
			_T("SV_권호구분"),
			_T("SV_권호명"),
			_T("SV_권호별서명"),
			_T("SV_권호일련번호"),
			_T("SV_본서명"),
			_T("SV_딸림자료"),
			_T("SV_매체구분_코드"),
			_T("SV_면장수"),
			_T("SV_물리적특성"),
			_T("SV_발행일자"),
			_T("SV_부차적권호명"),
			_T("SV_비고"),
			_T("SV_책크기"),
			_T("SV_형식구분_코드"),
			_T("SV_화폐구분")
			};

	CString tmp;
	for(INT i=0;i<cnt;i++)
	{
		tmp = _T("");
		m_pCM->GetControlMgrData(alias[i],tmp);
		m_pDM_Volume->SetCellData(alias[i],tmp,nCurtVolIdx);
		// 권호일련번호 종에 세팅
		if (i == 5)	m_pDM_Species->SetCellData(_T("SS_최종권호일련번호"), tmp, 0);
	}

	// SV_종KEY
	m_pDM_Volume->SetCellData(_T("SV_종KEY"),m_pDM_Species->GetCellData(_T("SS_종KEY"), 0),nCurtVolIdx);

	// SV_권호유형
	m_pDM_Volume->SetCellData(_T("SV_권호유형"),_T("V"),nCurtVolIdx);

	// SV_색인권호명 -> 저장시 입력하도록

	// SV_가격 성격 
	strItemData = _T("");
	pCombo = ( CComboBox * )GetDlgItem( IDC_cmbVOL_PRICE_CHARACTER );
	if(pCombo) pCombo->GetWindowText(strItemData);
	m_pDM_Volume->SetCellData(_T("SV_가격성격"),strItemData,nCurtVolIdx);
	

	// SV_권호구분
	pCombo = ( CComboBox * )GetDlgItem( IDC_cmbVOL_VOLUME_CLASS );
	if(pCombo) 
	{
		nCurSel = pCombo->GetCurSel();
		
		if(0 == nCurSel) //일반
			m_pDM_Volume->SetCellData(_T("SV_권호구분"),_T("1"),nCurtVolIdx);
		else if(1 == nCurSel) //합병호
			m_pDM_Volume->SetCellData(_T("SV_권호구분"),_T("2"),nCurtVolIdx);
	}

	// SV_DB_FLAG
	m_pDM_Volume->SetCellData(_T("SV_DB_FLAG"),_T("I"),nCurtVolIdx);

	// 최초, 마지막작업일시 들어가는지 확인 요

	// 권키 tmp : 책과의 Sync를 맞추어주어야 한다.
	m_pDM_Volume->MakeRecKey(strItemData);
	m_pDM_Volume->SetCellData(_T("SV_권KEY"),strItemData,nCurtVolIdx);

	// 2/ 책입력 ++++++++++++++++++++++++++++++++++++++++++++++++
	// 저장상태
	pCombo = ( CComboBox * )GetDlgItem( IDC_INPUT_BOOK_STATUS );
	if(pCombo) nCurSel = pCombo->GetCurSel();

	CESL_DataMgr* pDM_Book;
	CString strWorkingStatus;
	if (nCurSel == 0) {
		strWorkingStatus.Format(_T("SEC111N"));	// 정리중
		pDM_Book = FindDM(_T("DM_연속_정리DM_책정보"));
		if ( pDM_Book == NULL ) 
		{
			AfxMessageBox ( _T("Init DM_연속_정리DM_책정보 Error") );
			return -1;
		}
	}
	if (nCurSel == 1) {
		strWorkingStatus.Format(_T("SEL212N"));	// 소장종
		pDM_Book = FindDM(_T("DM_연속_정리DM_소장책정보"));
		if ( pDM_Book == NULL ) 
		{
			AfxMessageBox ( _T("Init DM_연속_정리DM_소장책정보 Error") );
			return -1;
		}
	}

	// 입력책수만큼 넣어주기
	tmp = _T("");
	m_pCM->GetControlMgrData(_T("UDF_입력책수"),tmp);
	INT nDupNumber = 0;

	for(i=0; i<_ttoi(tmp); i++)
	{
		if (i != 0) nDupNumber = i+1;
		ids = ((CCatApi_SE*)m_pCatApi)->InsertBookToDM(m_pDM_Species, m_pDM_Volume, pDM_Book
								, nCurtVolIdx, strWorkingStatus, nDupNumber, _T(""), _T(""), _T(""), _T(""));

		if (ids<0) 
		{
			AfxMessageBox(_T("책 입력을 정상적으로 완료하지 못하였습니다."));
			return -1;
		}
	}
	
	// 총책수
	m_pDM_Volume->SetCellData(_T("SV_총자료수"), tmp, nCurtVolIdx);


	return 0;
}
HBRUSH CSE_CAT_INPUT_VOL::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
