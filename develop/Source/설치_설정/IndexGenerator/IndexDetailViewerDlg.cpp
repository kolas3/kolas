// IndexDetailViewerDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "indexgenerator.h"
#include "IndexDetailViewerDlg.h"
#include "SearchMono.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIndexDetailViewerDlg dialog


CIndexDetailViewerDlg::CIndexDetailViewerDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CIndexDetailViewerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIndexDetailViewerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParentMgr = pParent;
	m_Create = FALSE;
	m_bRecycleData	= FALSE;
	m_pkCnt = 0;
}

CIndexDetailViewerDlg::~CIndexDetailViewerDlg()
{
	
}

void CIndexDetailViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIndexDetailViewerDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIndexDetailViewerDlg, CDialog)
	//{{AFX_MSG_MAP(CIndexDetailViewerDlg)
	ON_BN_CLICKED(IDC_bBEFORE, OnbBEFORE)
	ON_BN_CLICKED(IDC_bNEXT, OnbNEXT)
	ON_BN_CLICKED(IDC_INDEX_START, OnIndexStart)
	ON_WM_SHOWWINDOW()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIndexDetailViewerDlg message handlers

BOOL CIndexDetailViewerDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	switch( m_nDANorYON )
	{
	case DAN:
		{
			if( InitESL_Mgr(_T("SM_IndexGeneratorInfoView_Mono")) < 0 )
	{
			ESLAfxMessageBox(_T("InitESL_Mgr Error"));
			return FALSE;
		}
		m_pIndexViewDM = FindDM(_T("DM_IndexGeneratorInfoView_Mono"));
		m_pIndexViewCM = FindCM(_T("CM_IndexGeneratorInfoView_Mono"));;
		m_pBookViewDM = FindDM(_T("DM_IndexGeneratorInfoView_Book_Mono"));
		m_pBookGrid = (CESL_Grid*)m_pIndexViewCM->FindControl(_T("그리드"));
	}
		break;
	case YON:
		{
			if( InitESL_Mgr(_T("SM_IndexGeneratorInfoView_Serial")) < 0 )
	{
			ESLAfxMessageBox(_T("InitESL_Mgr Error"));
			return FALSE;
		}
		m_pIndexViewDM = FindDM(_T("DM_IndexGeneratorInfoView_Serial"));
		m_pIndexViewCM = FindCM(_T("CM_IndexGeneratorInfoView_Serial"));;
		m_pBookViewDM = FindDM(_T("DM_IndexGeneratorInfoView_Book_Serial"));
		m_pBookGrid = (CESL_Grid*)m_pIndexViewCM->FindControl(_T("연속_그리드"));
	}
		break;
	default:	return FALSE;
		break;
	}


	if( Init_IndexViewerCtrl() < 0 )	return FALSE;
	if( Init_MarcViewerCtrl()  < 0 )	return FALSE;

	
	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CIndexDetailViewerDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// 처음 마크 정보 Set
	SetMarcInfo();
	// 처음 색인 정보 Set
	SetIndexInfo();
	// 처음 책 정보 Set
	SetBookInfo();

	m_IndexInfoControl.Display();
	m_MarcInfoControl.Display();
	m_pBookGrid->Display();

	switch( m_nDANorYON )
	{
	case DAN:		GetDlgItem(IDC_BOOK_GRID)->ShowWindow(SW_HIDE);
		break;
	case YON:		GetDlgItem(IDC_BOOK_GRID_MONO)->ShowWindow(SW_HIDE);
		break;
	default:
		break;
	}

	ShowHideButton();
}

HBRUSH CIndexDetailViewerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

void CIndexDetailViewerDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

VOID CIndexDetailViewerDlg::OnbBEFORE() 
{
	// TODO: Add your control notification handler code here

	m_idx--;
	POSITION pos = m_pkList.FindIndex( m_idx );
	m_pk = m_pkList.GetPrev( pos );
	
	m_eslGrid->SetReverse( m_selectedGridRow.GetAt( m_idx ) );

	SetMarcInfo();
	SetIndexInfo();
	SetBookInfo();

	m_IndexInfoControl.Display();
	m_MarcInfoControl.Display();
	m_pBookGrid->Display();

	ShowHideButton();

}

VOID CIndexDetailViewerDlg::OnbNEXT() 
{
	// TODO: Add your control notification handler code here

	if( m_idx == (m_pkCnt - 1) ) 
		return;
	m_idx++;
	POSITION pos = m_pkList.FindIndex( m_idx );
	m_pk = m_pkList.GetNext( pos );

	m_eslGrid->SetReverse( m_selectedGridRow.GetAt( m_idx ) );

	SetMarcInfo();
	SetIndexInfo();
	SetBookInfo();

	m_IndexInfoControl.Display();
	m_MarcInfoControl.Display();
	m_pBookGrid->Display();

	ShowHideButton();
	
}

VOID CIndexDetailViewerDlg::OnIndexStart() 
{
	INT Ids;
//	CSearchMono* m_pParentMgr;
	if( m_nDANorYON == DAN )		
	{
		Ids = ((CSearchMono*)m_pParentMgr)->CurrentIndexReproduce();
	}
//	else if( m_nDANorYON == YON )	Ids = ((CIndexReproduceMgr_Serial*)m_pParentMgr)->CurrentIndexReproduce();

	if(Ids == 0 )	ESLAfxMessageBox(_T("색인재생성 완료!"));	

	OnbNEXT();
}

//BOOL CIndexDetailViewerDlg::OnInitDialog() 
//{
//	CDialog::OnInitDialog();
//	
//	// 단행인지 연속인지 비교하여 SM/DM/CM/Grid를 설정한다.
//	
//
//	return TRUE;
//}

INT CIndexDetailViewerDlg::SetDialogInfo( CStringList &pkList, CESL_Grid *grid, CArray< INT, INT > &gridRow, INT idx )
{
	m_pkList.RemoveAll();
	
	POSITION pos = pkList.GetHeadPosition();
	while( pos )	m_pkList.AddTail( pkList.GetNext( pos ) );		

	m_pkCnt = m_pkList.GetCount();

	m_idx = idx;

	pos = m_pkList.FindIndex(m_idx);
	m_pk = m_pkList.GetNext( pos );

	m_eslGrid = grid;

	for( INT i = 0; i < gridRow.GetSize(); i++ )	m_selectedGridRow.Add( gridRow.GetAt( i ) );

	return 0;
}

INT CIndexDetailViewerDlg::Init_IndexViewerCtrl()
{
	m_IndexInfoControl.InitViewer( IDC_INDEX_VIEWER , this );
	m_IndexInfoControl.SetViewerWidth( 30 , 67 );

	return 0;
}

INT CIndexDetailViewerDlg::Init_MarcViewerCtrl()
{
	m_MarcInfoControl.InitViewer( IDC_MARC_VIEWER , this );
	m_MarcInfoControl.SetViewerWidth( 30 , 67 );

	return 0;
}

INT CIndexDetailViewerDlg::SetMarcInfo()
{
	switch( m_nDANorYON )
	{
	case DAN:		SetMarcInfo_Mono();
		break;
	case YON:		SetMarcInfo_Serial();
		break;
	default:		return -1;
		break;
	}
	
	return 0;
}


INT CIndexDetailViewerDlg::SetMarcInfo_Mono()
{
	INT Ids, i;
	CString strQuery, strMarc, strMarcAlias, strMarcData, strTbl;

	if( m_bRecycleData == TRUE )	strTbl = _T("BO_DELETE_SPECIES_TBL");
	else							strTbl = _T("BO_SPECIES_TBL");
	
	strQuery.Format(_T("SELECT MARC FROM %s WHERE REC_KEY = %s"), strTbl, m_pk);
	Ids = m_pIndexViewDM->GetOneValueQuery(strQuery, strMarc);
	if( Ids < 0 )	return -1;

	CIndexDataMgr m_IndexDataMgr(this);
	if( m_IndexDataMgr.SetDataMgr() == -1 )		return -1;

	m_IndexArray.RemoveAll();
	m_IndexDataMgr.ExtractIndexInMarc(strMarc, m_IndexArray);
	//INT nMarcDataCnt = m_IndexDataMgr.m_MarcAliasArray.GetSize();
	for( i=0; i<m_IndexArray.GetSize(); i++ )
	{
		//strMarcAlias = m_IndexDataMgr.m_MarcAliasArray.GetAt(i);
		strMarcAlias = m_IndexArray.GetAt(i);
		strMarcData = m_IndexDataMgr.m_MarcDataArray.GetAt(i);
		//strMarcData = m_IndexArray.GetAt(i);
		m_MarcInfoControl.SetControlData( strMarcAlias , strMarcData );
	}

	return 0;
}

INT CIndexDetailViewerDlg::SetMarcInfo_Serial()
{
//	INT Ids;
//	CString strQuery, strMarc, strMarcAlias, strMarcData;
//
//	strQuery.Format(_T("SELECT MARC FROM SE_SPECIES_TBL WHERE REC_KEY = %s"), m_pk);
//
//	Ids = m_pIndexViewDM->GetOneValueQuery(strQuery, strMarc);
//	if(Ids < 0) return -1;
//
//	CIndexDataMgr_Serial m_IndexDataMgr_Serial(this);
//	if(m_IndexDataMgr_Serial.SetDataMgr() == -1) return -1;
//
//	m_IndexArray.RemoveAll();
//	m_IndexDataMgr_Serial.ExtractIndexInMarc(strMarc, m_IndexArray);
//	INT nMarcDataCnt = m_IndexDataMgr_Serial.m_MarcAliasArray.GetSize();
//	for(INT i=0; i < nMarcDataCnt; i++)
//	{
//		strMarcAlias = m_IndexDataMgr_Serial.m_MarcAliasArray.GetAt(i);
//		strMarcData = m_IndexDataMgr_Serial.m_MarcDataArray.GetAt(i);
//		if(strMarcAlias == _T("간행상태")) strMarcData = GetPublicationStatus(strMarcData);
//		else if(strMarcAlias == _T("간행빈도")) strMarcData = GetPublicationFreq(strMarcData);
//
//		m_MarcInfoControl.SetControlData( strMarcAlias , strMarcData );
//	}
	
	return 0;
}

INT CIndexDetailViewerDlg::SetIndexInfo()
{
	INT Ids, nPos, i;	
	CString strQuery, strIdxTitle, strIdxAuthor, strIdxPublisher, strIdxIPubYear, strIdxKeyword, strAllItem;
	CString strIdxIPublisher, strIdxITitle, strIdxVolTitle, strIndexAlias, strData, strParam, strAlias;

	switch( m_nDANorYON )
	{
	case DAN:		strQuery.Format(_T("REC_KEY = %s"), m_pk);
		break;
	case YON:		strQuery.Format(_T("I.REC_KEY = %s"), m_pk);
		break;
	default:		return -1;
		break;
	}

	Ids = m_pIndexViewDM->RefreshDataManager(strQuery);
	if( Ids < 0 )	return -1;

	//중복 데이터가 존재하는 경우 또는 해당하는 데이터가 없는 경우 리턴.
	Ids = m_pIndexViewDM->GetRowCount();
	if( Ids != 1 ) return -1;  
	
	for( i=0; i<m_IndexArray.GetSize(); i++ )
	{
		strIndexAlias = m_IndexArray.GetAt(i);
		m_pIndexViewDM->GetCellData(strIndexAlias, 0, strData);

		if		( m_nDANorYON == DAN )		strParam = _T("IBS_");
		else if	( m_nDANorYON == YON )		strParam = _T("ISS_");

		nPos = strIndexAlias.Find(strParam, 0);
		strAlias = strIndexAlias.Mid(nPos+_tcsclen(strParam));
		if( m_nDANorYON == YON )
		{
			if		( strAlias == _T("간행상태") )	strData = GetPublicationStatus(strData);
			else if	( strAlias == _T("간행빈도") )	strData = GetPublicationFreq(strData);
		}

		m_IndexInfoControl.SetControlData(strAlias , strData );
	}
	// 권서명
	if( m_nDANorYON == DAN )
	{
		m_pIndexViewDM->GetCellData(_T("IBS_검색용권서명"), 0, strData);
		m_IndexInfoControl.SetControlData(_T("검색용권서명") , strData );
	}

	return 0;
}

INT CIndexDetailViewerDlg::SetBookInfo()
{
	if		( m_nDANorYON == DAN )	SetBookInfo_Mono();
	else if	( m_nDANorYON == YON )	SetBookInfo_Serial();
	else return -1;

	return 0;
}

INT CIndexDetailViewerDlg::SetBookInfo_Mono()
{
	INT Ids, i;
	CString strQuery, strCode, strValue, strTitle, strAllTitle = _T("");
	CStringArray TargetArray;
	

	strQuery.Format(_T("S.REC_KEY = B.SPECIES_KEY AND S.REC_KEY = %s"), m_pk);

	if( m_bRecycleData == TRUE )	m_pBookViewDM->TBL_NAME = _T("BO_DELETE_BOOK_TBL B, BO_DELETE_SPECIES_TBL S");
	else							m_pBookViewDM->TBL_NAME = _T("BO_BOOK_TBL B, BO_SPECIES_TBL S");

	Ids = m_pBookViewDM->RefreshDataManager(strQuery);
	if( Ids < 0 )	return -1;

	Ids = m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pBookViewDM, _T("자료실구분"), _T("BB_자료실구분"));
	Ids = m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pBookViewDM, _T("등록구분"), _T("BB_등록구분"));
	Ids = m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pBookViewDM, _T("보조등록구분"), _T("BS_보조등록구분"));
	Ids = m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pBookViewDM, _T("이용제한구분"), _T("BB_이용제한구분"));
	Ids = m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pBookViewDM, _T("관리구분"), _T("BB_관리구분"));
	Ids = m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pBookViewDM, _T("별치기호"), _T("BB_별치기호"));
	Ids = m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pBookViewDM, _T("형식구분"), _T("BS_형식구분"));
	Ids = m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pBookViewDM, _T("자료구분"), _T("BS_자료구분"));
	Ids = m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pBookViewDM, _T("매체구분"), _T("BS_매체구분"));
	Ids = m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pBookViewDM, _T("업무구분"), _T("BS_업무구분"));
	Ids = m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pBookViewDM, _T("분류표구분"), _T("BB_분류표구분"));

	for( i=0; i<m_pBookViewDM->GetRowCount(); i++ )
	{
		m_pBookViewDM->GetCellData(_T("BB_자료상태"), i, strCode);
		strValue = GetWorkingStatusDesc_Mono(strCode);
		m_pBookViewDM->SetCellData(_T("UDF_자료상태"), strValue, i);

		m_pBookViewDM->GetCellData(_T("BB_편제색인"), i, strTitle);
		strAllTitle += strTitle + _T(" ");
	}

	TargetArray.RemoveAll();
	ParsingString(_T(" "), strAllTitle, &TargetArray);
	strAllTitle.Empty();
	for( i=0 ; i<TargetArray.GetSize(); i++ )
	{
		if( strAllTitle.IsEmpty() == TRUE )		strAllTitle = TargetArray.GetAt(i);
		else strAllTitle += _T(" ") + TargetArray.GetAt(i);
	}

	m_MarcInfoControl.SetControlData(_T("권서명") , strAllTitle );

	return 0;
}

INT CIndexDetailViewerDlg::SetBookInfo_Serial()
{
	CString strQuery;
	strQuery.Format(_T("S.REC_KEY = V.SPECIES_KEY AND V.REC_KEY = B.VOL_KEY AND S.REC_KEY = %s"), m_pk);
	INT Ids;
	Ids = m_pBookViewDM->RefreshDataManager(strQuery);
	if(Ids < 0) return -1;

	// 자료실 값 받아오기.
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pBookViewDM, _T("자료실구분"), _T("SB_자료실구분"));
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pBookViewDM, _T("등록구분"), _T("SB_등록구분"));
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pBookViewDM, _T("보조등록구분"), _T("SS_보조등록구분"));
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pBookViewDM, _T("이용제한구분"), _T("SB_이용제한구분"));
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pBookViewDM, _T("관리구분"), _T("SB_관리구분"));
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pBookViewDM, _T("별치기호"), _T("SB_별치기호"));
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pBookViewDM, _T("형식구분"), _T("SS_형식구분"));
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pBookViewDM, _T("자료구분"), _T("SS_자료구분"));
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pBookViewDM, _T("매체구분"), _T("SS_매체구분"));
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pBookViewDM, _T("분류표구분"), _T("SB_분류표구분"));

	// 자료상태 값 받아오기.
	CString strCode, strValue;
	INT nRowCount;
	nRowCount = m_pBookViewDM->GetRowCount();
	for(INT i=0; i < nRowCount; i++)
	{
		m_pBookViewDM->GetCellData(_T("SB_자료상태"), i, strCode);
		strValue = GetWorkingStatusDesc_Serial(strCode);
		m_pBookViewDM->SetCellData(_T("UDF_자료상태"), strValue, i);
	}

	return 0;
}

CString CIndexDetailViewerDlg::MakeALLITEM(CString strIdxTitle, CString strIdxAuthor, 
										   CString strIdxPublisher, CString strIdxKeyword)
{
	//IDX_ALL_ITEM필드를 채우기 위해서 검색용본표제, 저작자, 발행자, 키워드 를 추출한다.
	CString strIdxAllItem =_T("");
	strIdxAllItem = strIdxTitle + _T(" ") + strIdxAuthor + _T(" ") + strIdxPublisher + _T(" ") + strIdxKeyword;
	CStringArray TargetArray;
	TargetArray.RemoveAll();
	ParsingString(_T(" "), strIdxAllItem, &TargetArray);
	INT cnt = TargetArray.GetSize();
	strIdxAllItem.Empty();
	for(INT i=0 ; i<cnt ; i++)
	{
		if(strIdxAllItem.IsEmpty()) strIdxAllItem = TargetArray.GetAt(i);
		else strIdxAllItem += _T(" ") + TargetArray.GetAt(i);
	}
	
	return strIdxAllItem;
}

INT CIndexDetailViewerDlg::ParsingString(CString DelimeterStr, CString strSource, 
										 CStringArray *pTargetArray, BOOL bIndexing/*=TRUE*/)
{
	if(strSource.GetLength() == 0) return 0;
	INT pos = strSource.Find(DelimeterStr, 0);
	CString strTmpData;
	while(pos >= 0)
	{
		strTmpData = strSource.Mid(0, pos);
		strSource = strSource.Mid(pos+DelimeterStr.GetLength());
		if(bIndexing) ((CIndexTool*)pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strTmpData);
		if(!bIndexing || !CheckIsHave(pTargetArray, strTmpData))
			pTargetArray->Add(strTmpData);
		pos = strSource.Find(DelimeterStr, 0);
	}
	if(bIndexing) ((CIndexTool*)pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strSource);
	if(!bIndexing || !CheckIsHave(pTargetArray, strSource))
		pTargetArray->Add(strSource);
	return 0;
}

BOOL CIndexDetailViewerDlg::CheckIsHave(CStringArray *codeValue, CString code)
{
	INT cnt = codeValue->GetSize();
	if(cnt == 0) return FALSE;
	for(INT i=0 ; i<cnt ; i++)
		if(codeValue->GetAt(i) == code) return TRUE;
	return FALSE;
}



CString CIndexDetailViewerDlg::GetWorkingStatusDesc_Serial(CString strCode)
{
	CString code[64];
	CString desc[64];
	INT cnt = 0;

	code[cnt] = _T("SEA111N");	desc[cnt] = _T("구입입수자료"); cnt++;
	code[cnt] = _T("SEA112N");	desc[cnt] = _T("구입결호자료"); cnt++;
	code[cnt] = _T("SEA211N");	desc[cnt] = _T("기증입수자료"); cnt++;
	code[cnt] = _T("SEA212N");	desc[cnt] = _T("기증결호자료"); cnt++;
	code[cnt] = _T("SEB111N");	desc[cnt] = _T("제본인계자료"); cnt++;
	code[cnt] = _T("SEB112N");	desc[cnt] = _T("제본결정자료"); cnt++;
	code[cnt] = _T("SEB113N");	desc[cnt] = _T("제본발주자료"); cnt++;	
	code[cnt] = _T("SEB114N");	desc[cnt] = _T("제본검수자료"); cnt++;	
	code[cnt] = _T("SEL111O");	desc[cnt] = _T("실시간인계자료"); cnt++;	
	code[cnt] = _T("SEL112N");	desc[cnt] = _T("실시간배가자료"); cnt++;	
	code[cnt] = _T("SEL113O");	desc[cnt] = _T("실시간폐기자료"); cnt++;	
	code[cnt] = _T("SER111N");	desc[cnt] = _T("등록인계자료"); cnt++;
	code[cnt] = _T("SER112N");	desc[cnt] = _T("등록자료"); cnt++;	
	code[cnt] = _T("SER113O");	desc[cnt] = _T("가원부번호부여자료"); cnt++;
	code[cnt] = _T("SEC111N");	desc[cnt] = _T("정리대상자료"); cnt++;
	code[cnt] = _T("SEC112O");	desc[cnt] = _T("열람인계대상자료"); cnt++;
	code[cnt] = _T("SEL211O");	desc[cnt] = _T("열람인계자료"); cnt++;
	code[cnt] = _T("SEL212N");	desc[cnt] = _T("배가자료"); cnt++;
	code[cnt] = _T("SEL213O");	desc[cnt] = _T("수리제본자료"); cnt++;
	code[cnt] = _T("SEL214O");	desc[cnt] = _T("파손자료"); cnt++;
	code[cnt] = _T("SEL215O");	desc[cnt] = _T("제적자료"); cnt++;
	code[cnt] = _T("SEL216O");	desc[cnt] = _T("분실자료"); cnt++;
	code[cnt] = _T("SEL217O");	desc[cnt] = _T("재정리자료"); cnt++;
	code[cnt] = _T("SEL311O");	desc[cnt] = _T("관외대출자료"); cnt++;
	code[cnt] = _T("SEL312O");	desc[cnt] = _T("관내대출자료"); cnt++;
	//=====================================================
	//2008.12.01 UPDATE BY PJW : 단행 상호대차 자료상태 추가
	// 18.09.27 JMJ KOL.RED.2018.008
	code[cnt] = _T("SEL411O");	desc[cnt] = _T("연속책두레업무진행자료"); cnt++;
	code[cnt] = _T("SEL511O");	desc[cnt] = _T("통합도서타관반납"); cnt++;
	//=====================================================
	code[cnt] = _T("SEL313O");	desc[cnt] = _T("특별대출자료"); cnt++;
	code[cnt] = _T("SEL314O");	desc[cnt] = _T("긴급대출자료"); cnt++;
    code[cnt] = _T("SEL315O");  desc[cnt] = _T("배달자료"); cnt++; 


	CString strDesc = _T("");
	for(INT i=0;i<cnt;i++)
	{
		if(code[i] == strCode)
		{
			strDesc = desc[i];
			break;
		}
	
	}

	return strDesc;
}

CString CIndexDetailViewerDlg::GetWorkingStatusDesc_Mono(CString strCode)
{
	strCode.TrimLeft();		strCode.TrimRight();

	CString str;
	if(strCode == _T("BOT111O")) str = _T("기초자료");
	else if(strCode == _T("BOT112O")) str = _T("반입자료");
	else if(strCode == _T("BOA111N")) str = _T("구입대상자료");
	else if(strCode == _T("BOA112N")) str = _T("주문자료");
	else if(strCode == _T("BOA113O")) str = _T("미납품자료");
	else if(strCode == _T("BOA114O")) str = _T("구입인계대상자료");
	else if(strCode == _T("BOA211O")) str = _T("기증접수자료");
	else if(strCode == _T("BOA212O")) str = _T("기증미등록자료");
	else if(strCode == _T("BOR111N")) str = _T("등록인계자료");
	else if(strCode == _T("BOR112N")) str = _T("등록자료");
	else if(strCode == _T("BOR113O")) str = _T("가원부번호부여자료");
	else if(strCode == _T("BOC111O")) str = _T("복본조사대상자료");
	else if(strCode == _T("BOC112O")) str = _T("분류대상자료");
	else if(strCode == _T("BOC113N")) str = _T("정리대상자료");
	else if(strCode == _T("BOC114O")) str = _T("열람인계대상자료");
	else if(strCode == _T("BOL111O")) str = _T("열람인계자료");
	else if(strCode == _T("BOL112N")) str = _T("배가자료");
	else if(strCode == _T("BOL113O")) str = _T("수리제본자료");
	else if(strCode == _T("BOL114O")) str = _T("파손자료");
	else if(strCode == _T("BOL115O")) str = _T("제적자료");
	else if(strCode == _T("BOL211O")) str = _T("관외대출자료");
	else if(strCode == _T("BOL212O")) str = _T("관내대출자료");
	//=====================================================
	//2008.12.01 UPDATE BY PJW : 단행 상호대차 자료상태 추가
	// 18.09.27 JMJ KOL.RED.2018.008
	else if(strCode == _T("BOL411O")) str = _T("단행책두레업무진행자료");
	else if(strCode == _T("BOL511O")) str = _T("통합도서타관반납");
	// 2013.12.01 UPDATE BY BJS : 2013도서관통합서비스확대구축및개선 (통합도서타관대출 상태 추가)
	else if(strCode == _T("BOL611O")) str = _T("통합도서타관대출");
	//=====================================================
	else if(strCode == _T("BOL213O")) str = _T("특별대출자료");
	else if(strCode == _T("BOL214O")) str = _T("긴급대출자료");
	else if(strCode == _T("BOL215O")) str = _T("배달자료");

	return str;
}


VOID CIndexDetailViewerDlg::DisplayNext()
{
	OnbNEXT();
}


VOID CIndexDetailViewerDlg::ShowHideButton()
{
	CButton *pBtn;
	BOOL bBefore, bNext;

	// 선택된 갯수가 하나일 때.
	if( 1 == m_pkCnt ) {
		bBefore = FALSE;
		bNext = FALSE;
	}
	else {
		// 맨 처음 Row일 때.
		if( !m_idx ){
			bBefore = FALSE;
			bNext = TRUE;
		}
		// 맨 끝 Row일 때.
		else if( m_idx == m_pkCnt - 1 ) {
			bBefore = TRUE;
			bNext = FALSE;
		}
		// 그 외.
		else{
			bBefore = TRUE;
			bNext = TRUE;
		}

	}

	pBtn = ( CButton * )GetDlgItem( IDC_bBEFORE );
	pBtn->EnableWindow(bBefore);

	pBtn = ( CButton * )GetDlgItem( IDC_bNEXT );
	pBtn->EnableWindow(bNext);

}



CString CIndexDetailViewerDlg::GetPublicationStatus(CString strCode)
{
	strCode.TrimLeft();		strCode.TrimRight();

	CString strData;
	strData = _T("");
	if(strCode == _T("c")) 			strData.Format(_T("계속 간행되고 있는 간행물"));
	else if(strCode == _T("d"))		strData.Format(_T("폐간된 간행물"));

	return strData;
}

CString CIndexDetailViewerDlg::GetPublicationFreq(CString strCode)
{
	strCode.TrimLeft();		strCode.TrimRight();

	CString strData;
	strData = _T("");
	if(strCode == _T("a")) 			strData.Format(_T("연간"));
	else if(strCode == _T("b")) 	strData.Format(_T("격월간"));  
	else if(strCode == _T("c")) 	strData.Format(_T("주2회간"));
	else if(strCode == _T("d"))		strData.Format(_T("일간"));
	else if(strCode == _T("e")) 	strData.Format(_T("격주간"));
	else if(strCode == _T("f")) 	strData.Format(_T("반년간"));
	else if(strCode == _T("g")) 	strData.Format(_T("격년간"));
	else if(strCode == _T("h")) 	strData.Format(_T("3년 1회간"));
	else if(strCode == _T("i")) 	strData.Format(_T("주3회간"));
	else if(strCode == _T("j")) 	strData.Format(_T("월3회간"));
	else if(strCode == _T("m")) 	strData.Format(_T("월간"));
	else if(strCode == _T("q")) 	strData.Format(_T("계간"));
	else if(strCode == _T("s")) 	strData.Format(_T("반월간"));
	else if(strCode == _T("t")) 	strData.Format(_T("연3회간"));
	else if(strCode == _T("u")) 	strData.Format(_T("간행빈도 불명"));
	else if(strCode == _T("w")) 	strData.Format(_T("주간"));
	else if(strCode == _T("z")) 	strData.Format(_T("기타 간행빈도"));

	return strData;

}