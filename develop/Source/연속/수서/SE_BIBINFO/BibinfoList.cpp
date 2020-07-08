// BibinfoList.cpp : implementation file
//

#include "stdafx.h"
#include "BibinfoList.h"
#include "..\..\..\ESL\Marc\MarcEdit\MarcEditCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBibinfoList dialog


CBibinfoList::CBibinfoList(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBibinfoList)
	m_bAcqExpInfo = TRUE;
	m_bAcqVolInfo = TRUE;
	m_bAppenExpInfo = TRUE;
	m_bBasicInfo = TRUE;
	m_bCode = TRUE;
	m_bManageInfo = TRUE;
	m_bMarc = TRUE;
	m_bMissVolInfo = TRUE;
	m_bPubExpInfo = TRUE;
	m_bSubInfo = TRUE;
	m_bVolExpInfo = TRUE;
	//}}AFX_DATA_INIT
	
	m_nACQ_TYPE = 1; //default
	m_pParentDM = NULL;
	m_pParentGrid = NULL;
	m_pDM = NULL;
	m_pDM_SPECIES = NULL;

}
CBibinfoList::~CBibinfoList()
{
}


VOID CBibinfoList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBibinfoList)
	DDX_Check(pDX, IDC_chkBIB_LIST_ACQ_EXP_INFO, m_bAcqExpInfo);
	DDX_Check(pDX, IDC_chkBIB_LIST_ACQ_VOL_INFO, m_bAcqVolInfo);
	DDX_Check(pDX, IDC_chkBIB_LIST_APPEN_EXP_INFO, m_bAppenExpInfo);
	DDX_Check(pDX, IDC_chkBIB_LIST_BASIC_INFO, m_bBasicInfo);
	DDX_Check(pDX, IDC_chkBIB_LIST_CODE, m_bCode);
	DDX_Check(pDX, IDC_chkBIB_LIST_MANAGE_INFO, m_bManageInfo);
	DDX_Check(pDX, IDC_chkBIB_LIST_MARC, m_bMarc);
	DDX_Check(pDX, IDC_chkBIB_LIST_MISS_VOL_INFO, m_bMissVolInfo);
	DDX_Check(pDX, IDC_chkBIB_LIST_PUB_EXP_INFO, m_bPubExpInfo);
	DDX_Check(pDX, IDC_chkBIB_LIST_SUB_INFO, m_bSubInfo);
	DDX_Check(pDX, IDC_chkBIB_LIST_VOL_EXP_INFO, m_bVolExpInfo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBibinfoList, CDialog)
	//{{AFX_MSG_MAP(CBibinfoList)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBibinfoList message handlers

BOOL CBibinfoList::OnInitDialog() 
{
	CDialog::OnInitDialog();


	// TODO: Add extra initialization here
	
	
	if(InitESL_Mgr( _T("연속_수서_서지정보관리_교열지") )<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : 연속_수서_서지정보관리_교열지") );
		return false;
	}
	

	m_pDM = FindDM(_T("DM_연속_교열지_출력"));
	m_pDM_SPECIES = FindDM(_T("DM_연속_교열지_종"));

	if(!m_pDM || !m_pDM_SPECIES) {
		AfxMessageBox( _T("DM Error : 연속_수서_서지정보관리_교열지") );
		return false;
	}
	


	if(2 == m_nACQ_TYPE) //기증일 경우 구독이력정보 선택은 배제한다
	{
		GetDlgItem(IDC_chkBIB_LIST_SUB_INFO)->ShowWindow(FALSE);
		m_bSubInfo = FALSE;
		UpdateData(FALSE);
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBibinfoList::OnOK() 
{
	// TODO: Add extra validation here
	
	UpdateData(TRUE);
	
	m_pInfo->pMarcMgr->SetMarcGroup(_T("통합마크"));

	//print
	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	

	INT ids;
	pSIReportManager->SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	ids = pSIReportManager->GetSIReportFileFromDB(_T("연속수서교열지"));
	
	if (ids < 0) {
		ESLAfxMessageBox(_T("DB로 부터 SI Report 정보를 가져오지 못하였습니다."));

		if (pSIReportManager) {
			delete pSIReportManager;
			pSIReportManager = NULL;
		}

		return ;
	}
	
	pSIReportManager->m_pWorkForm->paper = PAPER_A4_PORTRAIT;
	pSIReportManager->SetDataMgr(0, m_pDM, 0);
	pSIReportManager->DeleteTempPageFiles();

	m_pParentGrid->SelectMakeList();
	INT nIdx = m_pParentGrid->SelectGetHeadPosition();
	CString strSpeceisKey;
	INT nRowCnt;


	while(nIdx>=0)
	{
		m_pDM->FreeData();
		strSpeceisKey = _T("");
		strSpeceisKey = m_pParentDM->GetCellData(_T("ISS_종KEY"),nIdx);
		
		if(!strSpeceisKey.IsEmpty()) MakeList(strSpeceisKey,nIdx);

		nRowCnt = m_pDM->GetRowCount();
		if(nRowCnt>0) pSIReportManager->MAKEPAGE_DrawWorkForm(-1);
		nIdx = m_pParentGrid->SelectGetNext();

	}
	

	ids = pSIReportManager->Print2();
	
	//PrintData();
	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}

	CDialog::OnOK();
}

VOID CBibinfoList::MakeList(CString strSpeceisKey,INT nIdx)
{
	INT ids;
	m_Marc.Init();

	CString strQuery;
	CString strSteamMarc;
	strQuery.Format(_T("REC_KEY = %s"),strSpeceisKey);
	m_pDM_SPECIES->RefreshDataManager(strQuery);
		
	INT nRow = m_pDM_SPECIES->GetRowCount();
	if(nRow<1) return;
	
	strSteamMarc = m_pDM_SPECIES->GetCellData(_T("MARC"),0);
	m_pInfo->pMarcMgr->Decoding(strSteamMarc,&m_Marc);
		
	
	CString tmp;
	CString strLine;

	//제어 번호 
	tmp = m_pDM_SPECIES->GetCellData(_T("SS_제어번호"),0);
	strLine.Format(_T("제어번호 : %s"),tmp);
	InsertLineToDM(strLine);
	tmp = _T("");

	//표제 
	tmp = m_pParentDM->GetCellData(_T("ISS_표제사항"),nIdx);
	strLine.Format(_T("표    제 : %s"),tmp);
	InsertLineToDM(strLine);
	tmp = _T("");

	//발행자
	tmp = m_pParentDM->GetCellData(_T("ISS_발행자사항"),nIdx);
	strLine.Format(_T("발 행 자 : %s"),tmp);
	InsertLineToDM(strLine);
	tmp = _T("");

	//1: 기본서지사항
	if(m_bBasicInfo) 
	{			
		ids = MakeBasicInfoList(nIdx);
		if(ids<0) return;
	}

	//2: 구분코드 
	if(m_bCode) 
	{			
		ids = MakeCodeList();
		if(ids<0) return;
	}

	//3: 마크 
	if(m_bMarc) 
	{			
		ids = MakeMarcList();
		if(ids<0) return;
	}

	//4:관리정보 
	if(m_bManageInfo)
	{
		ids = MakeManageInfoList(strSpeceisKey);
		if(ids<0) return;
	}

	//5:구독이력정보
	if(m_bSubInfo)
	{
		ids = MakeSubInfoList(strSpeceisKey);
		if(ids<0) return;
	}


	//6: 입수권호정보 
	if(m_bAcqVolInfo)
	{
		ids = MakeAcqVolInfoList(strSpeceisKey);
		if(ids<0) return;
	}

	
	//7: 결호권호정보
	if(m_bMissVolInfo)
	{
		ids = MakeMissVolInfoList(strSpeceisKey);
		if(ids<0) return;
	}
	
	//8: 권호예측정보
	if(m_bVolExpInfo)
	{
		ids = MakeVolExpInfoList(strSpeceisKey);
		if(ids<0) return;
	}

	//9: 발행일예측정보 
	if(m_bPubExpInfo)
	{
		ids = MakePubExpInfoList(strSpeceisKey);
		if(ids<0) return;
	}

	//10: 입수일예측정보
	if(m_bAcqExpInfo)
	{
		ids = MakeAcqExpInfoList(strSpeceisKey);
		if(ids<0) return;
	}
	
	//11: 부록예측정보
	if(m_bAppenExpInfo)
	{
		ids = MakeAppenExpInfoList(strSpeceisKey);
		if(ids<0) return;
	}

}

INT CBibinfoList::MakeBasicInfoList(INT nIdx)
{
	CString strLine;
	CString strDesc;
	CString tmp;
	
	DrawEmptyLine();
	strLine = _T("<기본서지정보>");
	InsertLineToDM(strLine);
	DrawSeperatorLine();

	//ISSN 
	m_pInfo->pMarcMgr->GetItem(&m_Marc,_T("022$a"),tmp);
	strLine.Format(_T("I S S N  : %s"),tmp);                  
	InsertLineToDM(strLine);
	tmp = _T("");

	//CODEN
	m_pInfo->pMarcMgr->GetItem(&m_Marc,_T("030$a"),tmp);
	strLine.Format(_T("CODEN    : %s"),tmp);                  
	InsertLineToDM(strLine);
	tmp = _T("");


	//간행물등록번호 
	tmp = m_pDM_SPECIES->GetCellData(_T("SS_간행물등록번호"),0);
	strLine.Format(_T("간행물등록번호 : %s"),tmp);
	InsertLineToDM(strLine);
	tmp = _T("");
	
	
	//발행년유형(간행상태)
	CString strStatus;
	m_pInfo->pMarcMgr->GetItem(&m_Marc,_T("008@6"),tmp);
	strStatus = tmp;
	m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("간행상태코드"),tmp,strDesc);
	strLine.Format(_T("간행상태 : %s [ %s ]"),tmp,strDesc);
	InsertLineToDM(strLine);
	tmp = _T("");
	strDesc = _T("");


	//간행빈도
	m_pInfo->pMarcMgr->GetItem(&m_Marc,_T("008@18"),tmp);
	m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("간행빈도코드"),tmp,strDesc);
	strLine.Format(_T("간행빈도 : %s [ %s ]"),tmp,strDesc);
	InsertLineToDM(strLine);
	tmp = _T("");
	strDesc = _T("");


	//창간년 
	m_pInfo->pMarcMgr->GetItem(&m_Marc,_T("008@7-10"),tmp);
	strLine.Format(_T("창간년   : %s"),tmp);
	InsertLineToDM(strLine);
	tmp = _T("");

	//폐간년 
	if(strStatus == _T("d")) //폐간된 경우만 폐간년 적용  
	{
		m_pInfo->pMarcMgr->GetItem(&m_Marc,_T("008@11-14"),tmp);
		strLine.Format(_T("폐간년   : %s"),tmp);
		InsertLineToDM(strLine);
		tmp = _T("");
	}


	//정규성
	m_pInfo->pMarcMgr->GetItem(&m_Marc,_T("008@19"),tmp);
	m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("정규성코드"),tmp,strDesc);
	strLine.Format(_T("정규성   : %s [ %s ]"),tmp,strDesc);
	InsertLineToDM(strLine);
	tmp = _T("");
	strDesc = _T("");

	//가격
	CString tmp2;
	tmp = m_pDM_SPECIES->GetCellData(_T("SS_가격"),0);
	tmp2 = m_pDM_SPECIES->GetCellData(_T("SS_화폐구분_코드"),0);
	
	if(!tmp.IsEmpty())
	{
		strDesc.Format(_T("%s %s"),tmp2,tmp);
	}
	
	strLine.Format(_T("가격     : %s"),strDesc);
	InsertLineToDM(strLine);
	tmp = _T("");
	
	//자료실
	tmp = m_pDM_SPECIES->GetCellData(_T("SS_배가위치_코드"),0);
	m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("자료실구분"),tmp,strDesc);
	strLine.Format(_T("자료실   : %s [ %s ]"),tmp,strDesc);
	InsertLineToDM(strLine);
	tmp = _T("");
	strDesc = _T("");
	

	//제본여부 
	tmp = m_pDM_SPECIES->GetCellData(_T("SS_제본여부"),0);
	strLine.Format(_T("제본여부 : %s"),tmp);
	InsertLineToDM(strLine);
	tmp = _T("");
		
	DrawSeperatorLine();

	return 0;
}

INT CBibinfoList::MakeCodeList()
{
	CString strLine;
	CString strTmpLine;
	CString strDesc;
	CString tmp;
	INT nWide = 50;
	INT nLength;
	INT nGap;

	DrawEmptyLine();
	strLine = _T("<구분코드>");
	InsertLineToDM(strLine);
	DrawSeperatorLine();

	//등록구분, 보조등록구분
	tmp = m_pDM_SPECIES->GetCellData(_T("SS_등록구분_코드"),0);
	m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("등록구분"),tmp,strDesc);
	
	strLine = _T("");
	strTmpLine.Format(_T("등록구분    : %s [ %s ]"),tmp,strDesc);
	strLine += strTmpLine;
	
	nLength = strLine.GetLength();
	
	if(nLength<nWide)
	{
		nGap = nWide-nLength;
	
		for(INT i=0;i<nGap;i++)
		{
			strLine += _T(" ");
		}
	}

	tmp = m_pDM_SPECIES->GetCellData(_T("SS_보조등록구분_코드"),0);
	m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("보조등록구분"),tmp,strDesc);
	strTmpLine.Format(_T("보조등록구분: %s [ %s ]"),tmp,strDesc);
	strLine += strTmpLine;

	InsertLineToDM(strLine);
	tmp = _T("");
	strDesc = _T("");

	
	//자료구분, 형식구분
	tmp = m_pDM_SPECIES->GetCellData(_T("SS_자료구분_코드"),0);
	m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("자료구분"),tmp,strDesc);
	
	strLine = _T("");
	strTmpLine.Format(_T("자료구분    : %s [ %s ]"),tmp,strDesc);
	strLine += strTmpLine;
	
	nLength = strLine.GetLength();
	
	if(nLength<nWide)
	{
		nGap = nWide-nLength;
	
		for(INT i=0;i<nGap;i++)
		{
			strLine += _T(" ");
		}
	}

	tmp = m_pDM_SPECIES->GetCellData(_T("SS_형식구분_코드"),0);
	m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("형식구분"),tmp,strDesc);
	strTmpLine.Format(_T("형식구분    : %s [ %s ]"),tmp,strDesc);
	strLine += strTmpLine;

	InsertLineToDM(strLine);
	tmp = _T("");
	strDesc = _T("");


	//매체구분, 관리구분
	tmp = m_pDM_SPECIES->GetCellData(_T("SS_매체구분_코드"),0);
	m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("매체구분"),tmp,strDesc);
	
	strLine = _T("");
	strTmpLine.Format(_T("매체구분    : %s [ %s ]"),tmp,strDesc);
	strLine += strTmpLine;
	
	nLength = strLine.GetLength();
	
	if(nLength<nWide)
	{
		nGap = nWide-nLength;
	
		for(INT i=0;i<nGap;i++)
		{
			strLine += _T(" ");
		}
	}

	tmp = m_pDM_SPECIES->GetCellData(_T("SS_관리구분_코드"),0);
	m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("관리구분"),tmp,strDesc);
	strTmpLine.Format(_T("관리구분    : %s [ %s ]"),tmp,strDesc);
	strLine += strTmpLine;

	InsertLineToDM(strLine);
	tmp = _T("");
	strDesc = _T("");
	
	
	//이용대상구분, 이용제한구분
	tmp = m_pDM_SPECIES->GetCellData(_T("SS_이용대상구분_코드"),0);
	m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("이용대상구분"),tmp,strDesc);
	
	strLine = _T("");
	strTmpLine.Format(_T("이용대상구분: %s [ %s ]"),tmp,strDesc);
	strLine += strTmpLine;
	
	nLength = strLine.GetLength();
	
	if(nLength<nWide)
	{
		nGap = nWide-nLength;
	
		for(INT i=0;i<nGap;i++)
		{
			strLine += _T(" ");
		}
	}

	tmp = m_pDM_SPECIES->GetCellData(_T("SS_이용제한구분_코드"),0);
	m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("이용제한구분"),tmp,strDesc);
	strTmpLine.Format(_T("이용제한구분: %s [ %s ]"),tmp,strDesc);
	strLine += strTmpLine;

	InsertLineToDM(strLine);
	tmp = _T("");
	strDesc = _T("");

	DrawSeperatorLine();

	return 0;
}


INT CBibinfoList::MakeMarcList()
{
	CString strLine;
	CString strDesc;
	CString tmp;

	DrawEmptyLine();
	strLine = _T("<마크>");
	InsertLineToDM(strLine);
	DrawSeperatorLine();
	DrawSeperatorLine();

	// Marc Editor에 넣었다가 뺀다.
	CString strEditMarc;
	CMarcEditCtrl MarcEditCtrl;
	MarcEditCtrl.Create(NULL,NULL,WS_CHILD | ES_MULTILINE, CRect(0,0,0,0), this, 11202);
	//MarcEditCtrl.Create(_T(""),_T(""),WS_CHILD | ES_MULTILINE, CRect(0,0,0,0), this, 1079);

	MarcEditCtrl.Init(m_pInfo->pMarcMgr, &m_Marc);
	MarcEditCtrl.Display();
	
	if(IsWindow(MarcEditCtrl.GetSafeHwnd()))
		MarcEditCtrl.GetWindowText( strEditMarc );

	CString str;
	str.Format(_T("%c"), SUBFIELD_CODE);
	strEditMarc.Replace(str, _T("▼"));

	str.Format(_T("%c"), FIELD_TERMINATOR);
	strEditMarc.Replace(str, _T("▲"));

	CString sLeader;
	m_Marc.GetLeader(sLeader);
	strEditMarc = sLeader + _T("▲") + strEditMarc;
		
	//마크 짜르기
	INT nRowCnt = m_pDM->GetRowCount()-1;
	//INT iCount = m_pDM->GetRowCount()-1;
	INT iCount = nRowCnt;
	INT iStart = 0;
	
	while ( TRUE ) {
			INT iPos = strEditMarc.Find(_T("▲"));
			if ( iPos == 0 ) {
				iPos = strEditMarc.Find(_T("\r\n"));
			}

			CString sData = strEditMarc.Mid( iStart, iPos+2 );			
			strEditMarc = strEditMarc.Mid( iPos+2 );
			strEditMarc.TrimLeft();
			INT iLineWordCount = 300;
			
			
			while ( sData.GetLength() > iLineWordCount ) {
				CString sDataSub = sData.Left( iLineWordCount );
				BOOL bCh = IsCanCut( sDataSub, --iLineWordCount );
				if ( bCh == FALSE ) {
					iLineWordCount;
					sDataSub = sData.Left( iLineWordCount );
				}
				m_pDM->InsertRow(-1);
				m_pDM->SetCellData(_T("UDF_라인"), sDataSub, iCount );
				
				if(iCount == nRowCnt) {
					DrawEmptyLine();
					iCount++;
				}

				iCount++;
				sData = sData.Mid(iLineWordCount);
			}
			

			iStart = 0;
			m_pDM->InsertRow(-1);
			m_pDM->SetCellData( _T("UDF_라인"), sData, iCount );
		
			if(iCount == nRowCnt) {
				DrawEmptyLine();
				iCount++;
			}

			iCount++;
			if ( iPos == -1 ) {
				break;
			}
		}

		m_pDM->InsertRow(-1);
		m_pDM->SetCellData(_T("UDF_라인"),_T(""), iCount );
		if(iCount == nRowCnt) DrawEmptyLine();
		iCount++;
	
	
	DrawSeperatorLine();

	return 0;
}


INT CBibinfoList::MakeManageInfoList(CString strSpeceisKey)
{
	
	if(1 == m_nACQ_TYPE) return MakePurchaseManageInfoList(strSpeceisKey);
	else if(2 == m_nACQ_TYPE) return MakeDonateManageInfoList(strSpeceisKey);
	
	return 0;
}

INT CBibinfoList::MakePurchaseManageInfoList(CString strSpeceisKey)
{

	CESL_DataMgr* pDM = NULL;
	pDM = FindDM(_T("DM_연속_교열지_구입정보"));
	if(!pDM) return -1;
	
	CString strWhere;
	strWhere.Format(_T("SPECIES_KEY = %s"),strSpeceisKey);
	pDM->RefreshDataManager(strWhere);

	if(0 == pDM->GetRowCount()) return 0;

	CString strLine;
	CString strDesc;
	CString tmp;
	CString strTmpLine;
	INT nWide = 50;
	INT nLength;
	INT nGap;

	DrawEmptyLine();
	strLine = _T("<구입관리정보>");
	InsertLineToDM(strLine);
	DrawSeperatorLine();
	

	//구입처, 구독기간 
	tmp = pDM->GetCellData(_T("SP_구입처_코드"),0);
	m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("구입처관리"),tmp,strDesc);
	
	strLine = _T("");
	strTmpLine.Format(_T("구입처      : %s [ %s ]"),tmp,strDesc);
	strLine += strTmpLine;
	
	nLength = strLine.GetLength();
	
	if(nLength<nWide)
	{
		nGap = nWide-nLength;
	
		for(INT i=0;i<nGap;i++)
		{
			strLine += _T(" ");
		}
	}
	
	CString strStartDate,strEndDate;
	strStartDate = pDM->GetCellData(_T("SP_구독시작일"),0);
	strEndDate = pDM->GetCellData(_T("SP_구독만기일"),0);

	strTmpLine = _T("구독기간    : ");

	if(!strStartDate.IsEmpty() || !strEndDate.IsEmpty())
	{
		CString strTmpLin2;
		strTmpLin2.Format(_T("%s ~ %s"),strStartDate,strEndDate);
		strTmpLine += strTmpLin2;
	}

	strLine += strTmpLine;
	InsertLineToDM(strLine);
	tmp = _T("");
	strDesc = _T("");


	//구독가격,입수책수
	tmp = pDM->GetCellData(_T("SP_구독가격"),0);
	strLine.Format(_T("구독가격    : %s"),tmp);
	
	nLength = strLine.GetLength();
	
	if(nLength<nWide)
	{
		nGap = nWide-nLength;
	
		for(INT i=0;i<nGap;i++)
		{
			strLine += _T(" ");
		}
	}
	
	tmp = pDM->GetCellData(_T("SP_입수책수"),0);
	strTmpLine.Format(_T("입수책수    : %s"),tmp);
	strLine += strTmpLine;
	InsertLineToDM(strLine);
	tmp = _T("");


	//등록책수, 열람우선여부
	tmp = pDM->GetCellData(_T("SP_등록책수"),0);
	strLine.Format(_T("등록책수    : %s"),tmp);
	
	nLength = strLine.GetLength();
	
	if(nLength<nWide)
	{
		nGap = nWide-nLength;
	
		for(INT i=0;i<nGap;i++)
		{
			strLine += _T(" ");
		}
	}
	
	tmp = pDM->GetCellData(_T("SP_열람우선여부"),0);
	strTmpLine.Format(_T("열람우선여부: %s"),tmp);
	strLine += strTmpLine;
	InsertLineToDM(strLine);
	tmp = _T("");
	
	DrawSeperatorLine();

	return 0;
}

INT CBibinfoList::MakeDonateManageInfoList(CString strSpeceisKey)
{
	CESL_DataMgr* pDM = NULL;
	pDM = FindDM(_T("DM_연속_교열지_기증정보"));
	if(!pDM) return -1;

	CString strWhere;
	strWhere.Format(_T("SPECIES_KEY = %s"),strSpeceisKey);
	pDM->RefreshDataManager(strWhere);

	if(0 == pDM->GetRowCount()) return 0;

	CString strLine;
	CString strDesc;
	CString tmp;
	CString strTmpLine;
	INT nWide = 50;
	INT nLength;
	INT nGap;

	DrawEmptyLine();
	strLine = _T("<기증관리정보>");
	InsertLineToDM(strLine);
	DrawSeperatorLine();
	

	//기증자, 기증시작일 
	tmp = pDM->GetCellData(_T("SD_기증자명"),0);
	
	strLine = _T("");
	strLine.Format(_T("기증자     : %s"),tmp);
	strLine += strTmpLine;
	
	nLength = strLine.GetLength();
	
	if(nLength<nWide)
	{
		nGap = nWide-nLength;
	
		for(INT i=0;i<nGap;i++)
		{
			strLine += _T(" ");
		}
	}
	
	CString strStartDate;
	strStartDate = pDM->GetCellData(_T("SD_기증시작일"),0);

	strTmpLine = _T("기증시작일   : ");

	if(!strStartDate.IsEmpty())
	{
		CString strTmpLin2;
		strTmpLin2.Format(_T("%s ~ "),strStartDate);
		strTmpLine += strTmpLin2;
	}

	strLine += strTmpLine;
	InsertLineToDM(strLine);
	tmp = _T("");
	strDesc = _T("");


	//입수책수,등록책수
	tmp = pDM->GetCellData(_T("SD_입수책수"),0);
	strLine.Format(_T("입수책수    : %s"),tmp);
	
	nLength = strLine.GetLength();
	
	if(nLength<nWide)
	{
		nGap = nWide-nLength;
	
		for(INT i=0;i<nGap;i++)
		{
			strLine += _T(" ");
		}
	}
	
	tmp = pDM->GetCellData(_T("SD_등록책수"),0);
	strTmpLine.Format(_T("등록책수    : %s"),tmp);
	strLine += strTmpLine;
	InsertLineToDM(strLine);
	tmp = _T("");


	//열람우선여부
	tmp = pDM->GetCellData(_T("SD_열람우선여부"),0);
	strLine.Format(_T("열람우선여부: %s"),tmp);
	InsertLineToDM(strLine);
	tmp = _T("");
	
	DrawSeperatorLine();

	return 0;
}

INT CBibinfoList::MakeSubInfoList(CString strSpeceisKey)
{
	
	CESL_DataMgr* pDM = NULL;
	pDM = FindDM(_T("DM_연속_교열지_구독이력정보"));
	if(!pDM) return -1;
	
	CString strManageKey;
	CString strQuery;
	strQuery.Format(_T("SELECT REC_KEY FROM SE_PURCHASE_MANAGE_TBL WHERE SPECIES_KEY = %s"),strSpeceisKey);
	pDM->GetOneValueQuery(strQuery,strManageKey);
	
	if(strManageKey.IsEmpty()) return 0;
	
	CString strWhere;
	strWhere.Format(_T("PURCHASE_MANAGE_KEY = %s"),strManageKey);
	pDM->RefreshDataManager(strWhere);
	
	INT nRowCnt = pDM->GetRowCount();
	if(0 == nRowCnt) return 0;


	CString strLine;
	CString strDesc;
	CString tmp;
	CString strTmpLine;
	INT nWide = 40;
	INT nLength;
	INT nGap;

	DrawEmptyLine();
	strLine = _T("<구입관리정보>");
	InsertLineToDM(strLine);
	DrawSeperatorLine();
	
	
	//header
	strLine = _T("");
	strLine.Format(_T("코드  구입처명"));
	
	nLength = strLine.GetLength();
	
	if(nLength<nWide)
	{
		nGap = nWide-nLength;
	
		for(INT i=0;i<nGap;i++)
		{
			strLine += _T(" ");
		}
	}

	strLine += _T("   구독시작일   구독만기일   구독가격");
	InsertLineToDM(strLine);
	DrawSeperatorLine();
	
	CString strStartDate,strEndDate,strPrice;

	for(INT i=0;i<nRowCnt;i++)
	{
		//구입처코드,설명
		tmp = pDM->GetCellData(_T("구입처_코드"),i);
		m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("구입처관리"),tmp,strDesc);
		
		strLine = _T("");
		strTmpLine.Format(_T("%s  [ %s ]"),tmp,strDesc);
		strLine += strTmpLine;
		
		nLength = strLine.GetLength();
		
		if(nLength<nWide)
		{
			nGap = nWide-nLength;
		
			for(INT i=0;i<nGap;i++)
			{
				strLine += _T(" ");
			}
		}
		
		//구독시작일, 구독만기일, 구독가격
		strStartDate = pDM->GetCellData(_T("구독시작일"),i);
		strEndDate = pDM->GetCellData(_T("구독만기일"),i);
		strPrice = pDM->GetCellData(_T("구독가격"),i);
	
		strTmpLine.Format(_T("   %s    %s    %s"),strStartDate,strEndDate,strPrice);
		strLine += strTmpLine;
		InsertLineToDM(strLine);
	}

	DrawSeperatorLine();

	return 0;
}

INT CBibinfoList::MakeAcqVolInfoList(CString strSpeceisKey)
{
	CESL_DataMgr* pDM = NULL;
	pDM = FindDM(_T("DM_연속_교열지_권호정보"));
	if(!pDM) return -1;
	
	CString strWhere;
	strWhere.Format(_T("A.VOL_KEY = V.REC_KEY AND A.CHECKIN_STATUS != '3' AND V.VOL_TYPE = 'V' AND V.SPECIES_KEY = %s"),strSpeceisKey); 

	pDM->RefreshDataManager(strWhere);
	
	INT nRowCnt = pDM->GetRowCount();

	CString strLine;
	CString strDesc;
	CString tmp;
	CString strTmpLine;
	INT nWide = 50;
	INT nLength;
	INT nGap;

	DrawEmptyLine();
	strLine = _T("<입수권호정보>");
	InsertLineToDM(strLine);
	DrawSeperatorLine();
		
	//header
	strLine = _T("");
	strLine.Format(_T("권호명"));
	
	nLength = strLine.GetLength();
	
	if(nLength<nWide)
	{
		nGap = nWide-nLength;
	
		for(INT i=0;i<nGap;i++)
		{
			strLine += _T(" ");
		}
	}

	strLine += _T("   발행일       입수일      입수책수");
	InsertLineToDM(strLine);
	DrawSeperatorLine();
	
	CString strPubDate,strAcqDate,strAcqBookCnt;

	for(INT i=0;i<nRowCnt;i++)
	{
		//권호명
		tmp = pDM->GetCellData(_T("SV_권호명"),i);
		
		strLine = _T("");
		strTmpLine.Format(_T("%s"),tmp);
		strLine += strTmpLine;
		
		nLength = strLine.GetLength();
		
		if(nLength<nWide)
		{
			nGap = nWide-nLength;
		
			for(INT i=0;i<nGap;i++)
			{
				strLine += _T(" ");
			}
		}
		
		//발행일, 입수일, 입수책수
		strPubDate = pDM->GetCellData(_T("SV_발행일"),i);
		strAcqDate = pDM->GetCellData(_T("UDF_입수일"),i);
		strAcqBookCnt = pDM->GetCellData(_T("SA_입수책수"),i);
	
		strTmpLine.Format(_T("   %s    %s    %s"),strPubDate,strAcqDate,strAcqBookCnt);
		strLine += strTmpLine;
		InsertLineToDM(strLine);
	}

	DrawSeperatorLine();

	return 0;
}

INT CBibinfoList::MakeMissVolInfoList(CString strSpeceisKey)
{
	CESL_DataMgr* pDM = NULL;
	pDM = FindDM(_T("DM_연속_교열지_권호정보"));
	if(!pDM) return -1;
	
	CString strWhere;
	strWhere.Format(_T("A.VOL_KEY = V.REC_KEY AND A.CHECKIN_STATUS = '2' AND V.VOL_TYPE = 'V' AND V.SPECIES_KEY = %s"),strSpeceisKey); 

	pDM->RefreshDataManager(strWhere);
	
	INT nRowCnt = pDM->GetRowCount();

	CString strLine;
	CString strDesc;
	CString tmp;
	CString strTmpLine;
	INT nWide = 50;
	INT nLength;
	INT nGap;

	DrawEmptyLine();
	strLine = _T("<결호권호정보>");
	InsertLineToDM(strLine);
	DrawSeperatorLine();
		
	//header
	strLine = _T("");
	strLine.Format(_T("권호명"));
	
	nLength = strLine.GetLength();
	
	if(nLength<nWide)
	{
		nGap = nWide-nLength;
	
		for(INT i=0;i<nGap;i++)
		{
			strLine += _T(" ");
		}
	}


	strLine += _T("   입수예정일   독촉일      독촉차수");
	InsertLineToDM(strLine);
	DrawSeperatorLine();
	
	CString strAcqExpDate,strClaimDate,strClaimCnt;

	for(INT i=0;i<nRowCnt;i++)
	{
		//권호명
		tmp = pDM->GetCellData(_T("SV_권호명"),i);
		
		strLine = _T("");
		strTmpLine.Format(_T("%s"),tmp);
		strLine += strTmpLine;
		
		nLength = strLine.GetLength();
		
		if(nLength<nWide)
		{
			nGap = nWide-nLength;
		
			for(INT i=0;i<nGap;i++)
			{
				strLine += _T(" ");
			}
		}
		
		//독촉일, 독촉차수
		strAcqExpDate = pDM->GetCellData(_T("SA_입수예정일"),i);
		strClaimDate = pDM->GetCellData(_T("UDF_독촉일"),i);
		strClaimCnt = pDM->GetCellData(_T("UDF_독촉차수"),i);
	
		strTmpLine.Format(_T("   %s    %s    %s"),strAcqExpDate,strClaimDate,strClaimCnt);
		strLine += strTmpLine;
		InsertLineToDM(strLine);
	}

	DrawSeperatorLine();

	return 0;
}


INT CBibinfoList::MakeVolExpInfoList(CString strSpeceisKey)
{
	CESL_DataMgr* pDM = NULL;
	pDM = FindDM(_T("DM_연속_교열지_권호예측"));
	if(!pDM) return -1;
	
	CString strManageKey;
	CString strQuery;
	
	if(1 == m_nACQ_TYPE)
		strQuery.Format(_T("SELECT REC_KEY FROM SE_PURCHASE_MANAGE_TBL WHERE SPECIES_KEY = %s"),strSpeceisKey);
	else if(2 == m_nACQ_TYPE)
		strQuery.Format(_T("SELECT REC_KEY FROM SE_DONATE_MANAGE_TBL WHERE SPECIES_KEY = %s"),strSpeceisKey);
	
	
	pDM->GetOneValueQuery(strQuery,strManageKey);
	
	if(strManageKey.IsEmpty()) return 0;
	
	CString strWhere;
	strWhere.Format(_T("ACQ_KEY = %s AND FORMAT_TYPE = '1'"),strManageKey);
	pDM->RefreshDataManager(strWhere);
	
	INT nRowCnt = pDM->GetRowCount();

	CString strLine;
	CString strDesc;
	CString tmp;
	CString strTmpLine;
	INT nWide = 50;
	INT nLength;
	INT nGap;

	DrawEmptyLine();
	strLine = _T("<권호예측정보>");
	InsertLineToDM(strLine);
	DrawSeperatorLine();
		
	//header
	strLine = _T("");
	strLine.Format(_T("단계명   단계식별명"));
	
	nLength = strLine.GetLength();
	
	if(nLength<nWide)
	{
		nGap = nWide-nLength;
	
		for(INT i=0;i<nGap;i++)
		{
			strLine += _T(" ");
		}
	}

	strLine += _T("시작수 끝수 단계증가값 증가단위 예측시작수");
	InsertLineToDM(strLine);
	DrawSeperatorLine();
	
	CString strLevelName,strLevelSign,strStartNo,strEndNo,strIncLimit,strIncUnit,strAcqExpNo;

	for(INT i=0;i<nRowCnt;i++)
	{
		//단계명,단계식별명
		strLevelName = pDM->GetCellData(_T("단계명"),i);
		strLevelSign = pDM->GetCellData(_T("단계식별명"),i);

		strLine = _T("");
		strTmpLine.Format(_T("%s  %s"),strLevelName,strLevelSign);
		strLine += strTmpLine;
		
		nLength = strLine.GetLength();
		
		if(nLength<nWide)
		{
			nGap = nWide-nLength;
		
			for(INT i=0;i<nGap;i++)
			{
				strLine += _T(" ");
			}
		}
		
		//시작수 끝수 단계증가값 증가단위 예측시작수
		strStartNo = pDM->GetCellData(_T("시작수"),i);
		strEndNo = pDM->GetCellData(_T("끝수"),i);
		strIncLimit = pDM->GetCellData(_T("단계증가값"),i);
		strIncUnit = pDM->GetCellData(_T("증가단위"),i);
		strAcqExpNo = pDM->GetCellData(_T("예측시작수"),i);
		
		if(strStartNo.IsEmpty()) strStartNo = _T("없음");
		if(strEndNo.IsEmpty()) strEndNo = _T("없음");
		if(strIncLimit.IsEmpty()) strIncLimit = _T("  없음  ");
		if(strIncUnit.IsEmpty()) strIncUnit = _T("없음");
		

		strTmpLine.Format(_T(" %s   %s    %s    %s        %s"),strStartNo,strEndNo,strIncLimit,strIncUnit,strAcqExpNo);
		strLine += strTmpLine;
		InsertLineToDM(strLine);
	}


	DrawSeperatorLine();
	


	///////////////////////////부차적권호
	strWhere.Format(_T("ACQ_KEY = %s AND FORMAT_TYPE = '2'"),strManageKey);
	pDM->RefreshDataManager(strWhere);
	
	nRowCnt = pDM->GetRowCount();

	DrawEmptyLine();
	strLine = _T("<부차적권호예측정보>");
	InsertLineToDM(strLine);
	DrawSeperatorLine();
		
	//header
	strLine = _T("");
	strLine.Format(_T("단계명   단계식별명"));
	
	nLength = strLine.GetLength();
	
	if(nLength<nWide)
	{
		nGap = nWide-nLength;
	
		for(INT i=0;i<nGap;i++)
		{
			strLine += _T(" ");
		}
	}

	strLine += _T("시작수 끝수 단계증가값 증가단위 예측시작수");
	InsertLineToDM(strLine);
	DrawSeperatorLine();
	
	for(i=0;i<nRowCnt;i++)
	{
		//단계명,단계식별명
		strLevelName = pDM->GetCellData(_T("단계명"),i);
		strLevelSign = pDM->GetCellData(_T("단계식별명"),i);

		strLine = _T("");
		strTmpLine.Format(_T("%s  %s"),strLevelName,strLevelSign);
		strLine += strTmpLine;
		
		nLength = strLine.GetLength();
		
		if(nLength<nWide)
		{
			nGap = nWide-nLength;
		
			for(INT i=0;i<nGap;i++)
			{
				strLine += _T(" ");
			}
		}
		
		//시작수 끝수 단계증가값 증가단위 예측시작수
		strStartNo = pDM->GetCellData(_T("시작수"),i);
		strEndNo = pDM->GetCellData(_T("끝수"),i);
		strIncLimit = pDM->GetCellData(_T("단계증가값"),i);
		strIncUnit = pDM->GetCellData(_T("증가단위"),i);
		strAcqExpNo = pDM->GetCellData(_T("예측시작수"),i);
		
		if(strIncLimit.IsEmpty()) strIncLimit = _T("       ");

		strTmpLine.Format(_T(" %s   %s    %s    %s        %s"),strStartNo,strEndNo,strIncLimit,strIncUnit,strAcqExpNo);
		strLine += strTmpLine;
		InsertLineToDM(strLine);
	}


	DrawSeperatorLine();

	return 0;
}

INT CBibinfoList::MakePubExpInfoList(CString strSpeceisKey)
{
	CESL_DataMgr* pDM = NULL;
	pDM = FindDM(_T("DM_연속_교열지_권호예측"));
	if(!pDM) return -1;
	
	CString strQuery;
	CString strResult[2];

	if(1 == m_nACQ_TYPE)
		strQuery.Format(_T("SELECT PUB_EXP_KEY, PUB_EXP_TYPE FROM SE_PURCHASE_MANAGE_TBL WHERE SPECIES_KEY = %s"),strSpeceisKey);
	else if(2 == m_nACQ_TYPE)
		strQuery.Format(_T("SELECT PUB_EXP_KEY, PUB_EXP_TYPE FROM SE_DONATE_MANAGE_TBL WHERE SPECIES_KEY = %s"),strSpeceisKey);
	
	pDM->GetValuesQuery(strQuery,strResult,2);
	
	//예측 type 이 없으면 return
	if(strResult[1].IsEmpty()) return 0;
	

	CString strLastDate;
	CString strTblName;
	strTblName = GetExpTable(strResult[1]);

	strQuery.Format(_T("SELECT EXP_START_DATE FROM %s WHERE REC_KEY = %s"),strTblName,strResult[0]);
	pDM->GetOneValueQuery(strQuery,strLastDate);
	

	CString strLine;
	DrawEmptyLine();
	CString strType = GetExpTypeDesc(strResult[1]);

	strLine.Format(_T("<발행일 예측정보 [%s]   예측시작일: %s>"),strType,strLastDate);
	InsertLineToDM(strLine);
	
	switch(strResult[1].GetAt(0))
	{
	case 'M':
		return MakeMonthTypeList(strResult[0]);
		break;
	case 'W':
		return MakeWeekTypeList(strResult[0]);
		break;
	case 'D':
		return MakeDayTypeList(strResult[0]);
		break;
	case 'E':
		return MakeEtcTypeList(strResult[0]);
		break;
	default:
		break;
	}

	return 0;
}

INT CBibinfoList::MakeAcqExpInfoList(CString strSpeceisKey)
{
	CESL_DataMgr* pDM = NULL;
	pDM = FindDM(_T("DM_연속_교열지_권호예측"));
	if(!pDM) return -1;
	
	CString strQuery;
	CString strResult[2];

	if(1 == m_nACQ_TYPE)
		strQuery.Format(_T("SELECT ACQ_EXP_KEY, ACQ_EXP_TYPE FROM SE_PURCHASE_MANAGE_TBL WHERE SPECIES_KEY = %s"),strSpeceisKey);
	else if(2 == m_nACQ_TYPE)
		strQuery.Format(_T("SELECT ACQ_EXP_KEY, ACQ_EXP_TYPE FROM SE_DONATE_MANAGE_TBL WHERE SPECIES_KEY = %s"),strSpeceisKey);
	
	pDM->GetValuesQuery(strQuery,strResult,2);
	

	//예측 type 이 없으면 return
	if(strResult[1].IsEmpty()) return 0;
	
	CString strLastDate;
	CString strTblName;
	strTblName = GetExpTable(strResult[1]);

	strQuery.Format(_T("SELECT EXP_START_DATE FROM %s WHERE REC_KEY = %s"),strTblName,strResult[0]);
	pDM->GetOneValueQuery(strQuery,strLastDate);

	CString strLine;
	DrawEmptyLine();
	CString strType = GetExpTypeDesc(strResult[1]);
	
	strLine.Format(_T("<입수일 예측정보 [%s]   예측시작일: %s>"),strType,strLastDate);
	InsertLineToDM(strLine);
	
	switch(strResult[1].GetAt(0))
	{
	case 'M':
		return MakeMonthTypeList(strResult[0]);
		break;
	case 'W':
		return MakeWeekTypeList(strResult[0]);
		break;
	case 'D':
		return MakeDayTypeList(strResult[0]);
		break;
	case 'E':
		return MakeEtcTypeList(strResult[0]);
		break;
	default:
		break;
	}


	return 0;
}

INT CBibinfoList::MakeMonthTypeList(CString strRecKey)
{
	CESL_DataMgr* pDM = NULL;
	pDM = FindDM(_T("DM_연속_교열지_월간예측"));
	if(!pDM) return -1;
	
	CString strWhere;
	strWhere.Format(_T("REC_KEY = %s"),strRecKey);
	pDM->RefreshDataManager(strWhere);
	
	INT nRowCnt = pDM->GetRowCount();
	if(0 == nRowCnt) return 0;
	
	DrawSeperatorLine();

	CString strLine;
	CString strDesc;
	CString tmp;
	CString strTmpLine;
	INT nWide = 5;
	INT nLength;
	INT nGap;
	
	const INT cnt = 24;
	CString alias[ cnt ];
	CString data[ cnt ];
	GetMonthAlias(alias);
	pDM->GetCellData(alias,24,data,0);

	//header
	strLine = _T("");
	strLine = _T(" 월  :     1    2    3    4    5    6    7    8    9   10   11   12");
	InsertLineToDM(strLine);
	DrawSeperatorLine();
	
	strLine = _T("일자 : ");
	
	for(INT i=0;i<12;i++)
	{
		nLength = data[i].GetLength();
	
		if(nLength<nWide)
		{
			nGap = nWide-nLength;
	
			for(INT i=0;i<nGap;i++)
			{
				strLine += _T(" ");
			}
		}
		
		strLine += data[i];
	}
	
	InsertLineToDM(strLine);
	
	strLine = _T("부수 : ");
	
	for(i=12;i<24;i++)
	{
		nLength = data[i].GetLength();
	
		if(nLength<nWide)
		{
			nGap = nWide-nLength;
	
			for(INT i=0;i<nGap;i++)
			{
				strLine += _T(" ");
			}
		}
		
		strLine += data[i];
	}
	
	InsertLineToDM(strLine);
	DrawSeperatorLine();


	return 0;
}

INT CBibinfoList::MakeWeekTypeList(CString strRecKey)
{
	CESL_DataMgr* pDM = NULL;
	pDM = FindDM(_T("DM_연속_교열지_주간예측"));
	if(!pDM) return -1;
	
	CString strWhere;
	strWhere.Format(_T("REC_KEY = %s"),strRecKey);
	pDM->RefreshDataManager(strWhere);
	
	INT nRowCnt = pDM->GetRowCount();
	if(0 == nRowCnt) return 0;
	
	DrawSeperatorLine();

	CString strLine;
	CString strDesc;
	CString tmp;
	CString strTmpLine;
	INT nWide = 6;
	INT nLength;
	INT nGap;
	
	const INT cnt = 7;
	CString alias[ cnt ];
	CString data[ cnt ];
	GetWeekAlias(alias);
	
	pDM->GetCellData(alias,7,data,0);

	//header
	strLine = _T("");
	strLine = _T(" 요일  :     월    화    수    목    금    토    일");
	InsertLineToDM(strLine);
	DrawSeperatorLine();
	
	strLine = _T(" 부수  : ");
	
	for(INT i=0;i<cnt;i++)
	{
		nLength = data[i].GetLength();
	
		if(nLength<nWide)
		{
			nGap = nWide-nLength;
	
			for(INT i=0;i<nGap;i++)
			{
				strLine += _T(" ");
			}
		}
		
		strLine += data[i];
	}
	
	InsertLineToDM(strLine);
	DrawSeperatorLine();


	return 0;
}

INT CBibinfoList::MakeDayTypeList(CString strRecKey)
{
	CESL_DataMgr* pDM = NULL;
	pDM = FindDM(_T("DM_연속_교열지_일간예측"));
	if(!pDM) return -1;
	
	CString strWhere;
	strWhere.Format(_T("REC_KEY = %s"),strRecKey);
	pDM->RefreshDataManager(strWhere);
	
	INT nRowCnt = pDM->GetRowCount();
	if(0 == nRowCnt) return 0;
	
	DrawSeperatorLine();

	CString strLine;
	CString strDesc;
	CString tmp;
	CString strTmpLine;
	INT nWide = 5;
	INT nLength;
	INT nGap;
	
	const INT cnt = 12;
	CString alias[ cnt ];
	CString data[ cnt ];
	GetDayAlias(alias);
	pDM->GetCellData(alias,12,data,0);

	//header
	strLine = _T("");
	strLine = _T(" 일자 :     1    2    3    4    5    6");
	InsertLineToDM(strLine);
	DrawSeperatorLine();
	
	strLine = _T(" 일자 : ");
	
	for(INT i=0;i<6;i++)
	{
		nLength = data[i].GetLength();
	
		if(nLength<nWide)
		{
			nGap = nWide-nLength;
	
			for(INT i=0;i<nGap;i++)
			{
				strLine += _T(" ");
			}
		}
		
		strLine += data[i];
	}
	
	InsertLineToDM(strLine);
	
	strLine = _T(" 부수 : ");
	
	for(i=6;i<12;i++)
	{
		nLength = data[i].GetLength();
	
		if(nLength<nWide)
		{
			nGap = nWide-nLength;
	
			for(INT i=0;i<nGap;i++)
			{
				strLine += _T(" ");
			}
		}
		
		strLine += data[i];
	}
	
	InsertLineToDM(strLine);
	DrawSeperatorLine();

	return 0;
}

INT CBibinfoList::MakeEtcTypeList(CString strRecKey)
{
	CESL_DataMgr* pDM = NULL;
	pDM = FindDM(_T("DM_연속_교열지_기타예측"));
	if(!pDM) return -1;
	
	CString strWhere;
	strWhere.Format(_T("REC_KEY = %s"),strRecKey);
	pDM->RefreshDataManager(strWhere);
	
	INT nRowCnt = pDM->GetRowCount();
	if(0 == nRowCnt) return 0;
	
	DrawSeperatorLine();
	
	const INT cnt = 4;
	CString alias[ cnt ];
	CString data[ cnt ];
	GetEtcAlias(alias);
	pDM->GetCellData(alias,4,data,0);
	
	CString strLine;
	strLine.Format(_T("예측 간격 : %s 년 %s 월 %s 일     부수: %s 부"),data[0],data[1],data[2],data[3]);
	InsertLineToDM(strLine);
	DrawSeperatorLine();

	return 0;
}

INT CBibinfoList::MakeAppenExpInfoList(CString strSpeceisKey)
{
	
	CESL_DataMgr* pDM = NULL;
	pDM = FindDM(_T("DM_연속_교열지_부록예측"));
	if(!pDM) return -1;
	
	CString strManageKey;
	CString strQuery;
	
	if(1 == m_nACQ_TYPE)
		strQuery.Format(_T("SELECT REC_KEY FROM SE_PURCHASE_MANAGE_TBL WHERE SPECIES_KEY = %s"),strSpeceisKey);
	else if(2 == m_nACQ_TYPE)
		strQuery.Format(_T("SELECT REC_KEY FROM SE_DONATE_MANAGE_TBL WHERE SPECIES_KEY = %s"),strSpeceisKey);
	
	pDM->GetOneValueQuery(strQuery,strManageKey);
	
	if(strManageKey.IsEmpty()) return 0;
	
	CString strWhere;
	strWhere.Format(_T("ACQ_KEY = %s"),strManageKey);
	pDM->RefreshDataManager(strWhere);
	
	INT nRowCnt = pDM->GetRowCount();

	CString strLine;
	CString strDesc;
	CString tmp;
	CString strTmpLine;
	INT nWide = 50;
	INT nLength;
	INT nGap;

	DrawEmptyLine();
	strLine = _T("<부록예측정보>");
	InsertLineToDM(strLine);
	DrawSeperatorLine();
	

	//header
	strLine = _T("");
	strLine.Format(_T("부록명"));
	
	nLength = strLine.GetLength();
	
	if(nLength<nWide)
	{
		nGap = nWide-nLength;
	
		for(INT i=0;i<nGap;i++)
		{
			strLine += _T(" ");
		}
	}

	strLine += _T("형식구분  매체구분  폐기여부");
	InsertLineToDM(strLine);
	DrawSeperatorLine();
	
	CString strAppenName,strFormCode,strMediaCode,strDiscardYn;

	for(INT i=0;i<nRowCnt;i++)
	{
		//부록명
		strAppenName = pDM->GetCellData(_T("부록명"),i);

		strLine = _T("");
		strTmpLine.Format(_T("%s"),strAppenName);
		strLine += strTmpLine;
		
		nLength = strLine.GetLength();
		
		if(nLength<nWide)
		{
			nGap = nWide-nLength;
		
			for(INT i=0;i<nGap;i++)
			{
				strLine += _T(" ");
			}
		}
		
		//형식구분 매체구분 폐기여부
		strFormCode = pDM->GetCellData(_T("형식구분"),i);
		strMediaCode = pDM->GetCellData(_T("매체구분"),i);
		strDiscardYn = pDM->GetCellData(_T("폐기여부"),i);
	
		strTmpLine.Format(_T("    %s       %s       %s"),strFormCode,strMediaCode,strDiscardYn);
		strLine += strTmpLine;
		InsertLineToDM(strLine);
	}

	DrawSeperatorLine();

	return 0;
}

CString CBibinfoList::GetExpTypeDesc(CString strType)
{
	CString strDesc = _T("");

	switch(strType.GetAt(0))
	{
	case 'M':
		strDesc = _T("월단위");
		break;
	case 'W':
		strDesc = _T("주단위");
		break;
	case 'D':
		strDesc = _T("일단위");
		break;
	case 'E':
		strDesc = _T("기타단위");
		break;
	default:
		break;
	}

	return strDesc;
}

CString CBibinfoList::GetExpTable(CString strType)
{

	CString strTBL_NAME = _T("");

	switch(strType.GetAt(0))
	{
	case 'M':
		strTBL_NAME = _T("SE_MONTH_EXP_TBL");
		break;
	case 'W':
		strTBL_NAME = _T("SE_WEEK_EXP_TBL");
		break;
	case 'D':
		strTBL_NAME = _T("SE_DAY_EXP_TBL");
		break;
	case 'E':
		strTBL_NAME = _T("SE_ETC_EXP_TBL");
		break;
	default:
		break;
	}

	return strTBL_NAME;
}

VOID CBibinfoList::GetMonthAlias(CString data[])
{
	
	data[0] = _T("EXP_TERM_DAY1");		//	0
	data[1] = _T("EXP_TERM_DAY2");		//  1
	data[2] = _T("EXP_TERM_DAY3");		//  2
	data[3] = _T("EXP_TERM_DAY4");		//	3	  
	data[4] = _T("EXP_TERM_DAY5");		//	4
	data[5] = _T("EXP_TERM_DAY6");		//	5
	data[6] = _T("EXP_TERM_DAY7");		//	6
	data[7] = _T("EXP_TERM_DAY8");		//	7
	data[8] = _T("EXP_TERM_DAY9");		//	8
	data[9] = _T("EXP_TERM_DAY10");		//	9
	data[10] = _T("EXP_TERM_DAY11");	//	10
	data[11] = _T("EXP_TERM_DAY12");	//	11
	data[12] = _T("EXP_BOOK_CNT1");		//	12
	data[13] = _T("EXP_BOOK_CNT2");		//	13
	data[14] = _T("EXP_BOOK_CNT3");		//	14
	data[15] = _T("EXP_BOOK_CNT4");		//	15
	data[16] = _T("EXP_BOOK_CNT5");		//	16
	data[17] = _T("EXP_BOOK_CNT6");		//	17
	data[18] = _T("EXP_BOOK_CNT7");		//	18
	data[19] = _T("EXP_BOOK_CNT8");		//	19
	data[20] = _T("EXP_BOOK_CNT9");		//	20
	data[21] = _T("EXP_BOOK_CNT10");	//	21
	data[22] = _T("EXP_BOOK_CNT11");	//	22
	data[23] = _T("EXP_BOOK_CNT12");	//	23

}

VOID CBibinfoList::GetWeekAlias(CString data[])
{
	data[0] = _T("MON_CNT");				//	0
	data[1]	= _T("TUE_CNT");				//	1
	data[2]	= _T("WED_CNT");				//	2
	data[3]	= _T("THR_CNT");				//	3
	data[4]	= _T("FRI_CNT");				//	4
	data[5]	= _T("SAT_CNT");				//	5
	data[6]	= _T("SUN_CNT");				//  6
	
}

VOID CBibinfoList::GetDayAlias(CString data[])
{
	data[0] = _T("EXP_TERM_DATE1");		//	0
	data[1] = _T("EXP_TERM_DATE2");		//	1
	data[2] = _T("EXP_TERM_DATE3");		//	2
	data[3] = _T("EXP_TERM_DATE4");		//	3
	data[4] = _T("EXP_TERM_DATE5");		//	4
	data[5] = _T("EXP_TERM_DATE6");		//	5
	data[6] = _T("EXP_BOOK_CNT1");		//	6
	data[7] = _T("EXP_BOOK_CNT2");		//	7
	data[8] = _T("EXP_BOOK_CNT3");		//	8
	data[9] = _T("EXP_BOOK_CNT4");		//	9
	data[10] = _T("EXP_BOOK_CNT5");		//	10
	data[11] = _T("EXP_BOOK_CNT6");		//	11

}

VOID CBibinfoList::GetEtcAlias(CString data[])
{
	data[0] = _T("EXP_TERM_YEAR");		//	0
	data[1] = _T("EXP_TERM_MONTH");		//	1
	data[2] = _T("EXP_TERM_DAY");		//	2
	data[3] = _T("EXP_BOOK_CNT");		//	3
}

BOOL CBibinfoList::IsCanCut(CString sStr, INT nPos)
{

	INT i, nLastPos = -1;
	//글자수 초과면 FALSE
	if(nPos > sStr.GetLength())
		return FALSE;
	TCHAR ch = sStr.GetAt(nPos);
	//마지막 글자 || 한글이 아니면 TRUE
	if(nPos + 1 == sStr.GetLength() || (ch & (0x80)) != 128)
		return TRUE;
	//첫글자가 1byte문자면 TRUE 아니면 FALSE
	if(nPos == 0)
		if((ch & (0x80)) != 128)
			return TRUE;
		else
			return FALSE;

	for(i = 0; i < nPos; i++) {
		ch = sStr.GetAt(i);
		//자르는 부분에서 가장 가까운(앞쪽으로)영문의 위치
		if((ch & (0x80)) != 128)
			nLastPos = i;
	}
	if (nLastPos == -1 && nPos % 2 == 1) return FALSE;
	//짝수개 차이나면 TRUE
	return !((nPos - nLastPos) % 2);

}

VOID CBibinfoList::PrintData()
{
	INT nRowCnt = m_pDM->GetRowCount();
	if(0 == nRowCnt)
	{
		AfxMessageBox(_T("출력할 자료가 없습니다!"));
		return;
	}

	//print
	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	

	INT ids;
	pSIReportManager->SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	ids = pSIReportManager->GetSIReportFileFromDB(_T("연속수서교열지"));
	
	if (ids < 0) {
		ESLAfxMessageBox(_T("DB로 부터 SI Report 정보를 가져오지 못하였습니다."));
		return ;
	}
	
	pSIReportManager->m_pWorkForm->paper = PAPER_A4_PORTRAIT;
	pSIReportManager->SetDataMgr(0, m_pDM, 0);
	
	/*
	pSIReportManager->SetTempVariable(_T("형식"),m_strCondition);
	*/

	ids = pSIReportManager->Print();

	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}


}

VOID CBibinfoList::DrawSeperatorLine()
{
	CString strLine;
	CString strSign = _T("-");
	
	for(INT i=0;i<MAX_PRINT_LINE_LENGTH;i++)
	{
		strLine += strSign;
	}

	InsertLineToDM(strLine);
}

VOID CBibinfoList::DrawEmptyLine()
{
	m_pDM->InsertRow(-1);
}

VOID CBibinfoList::InsertLineToDM(CString strLine)
{
	INT nLength = strLine.GetLength();
	if(0 == nLength) return;

	INT nIdx;

	if(nLength<=MAX_PRINT_LINE_LENGTH)
	{
		m_pDM->InsertRow(-1);
		nIdx = m_pDM->GetRowCount()-1; 
		m_pDM->SetCellData(_T("UDF_라인"),strLine,nIdx);

	}
	else
	{
		CString tmp;
		
		while(1)
		{
			tmp = strLine.Left(MAX_PRINT_LINE_LENGTH);
			
			m_pDM->InsertRow(-1);
			nIdx = m_pDM->GetRowCount()-1; 
			m_pDM->SetCellData(_T("UDF_라인"),tmp,nIdx);

			strLine = strLine.Mid(MAX_PRINT_LINE_LENGTH+1);
				
			if(strLine.GetLength()<=MAX_PRINT_LINE_LENGTH)
			{
				m_pDM->InsertRow(-1);
				nIdx = m_pDM->GetRowCount()-1; 
				m_pDM->SetCellData(_T("UDF_라인"),strLine,nIdx);
				break;
			}
		}
	}


}
HBRUSH CBibinfoList::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
