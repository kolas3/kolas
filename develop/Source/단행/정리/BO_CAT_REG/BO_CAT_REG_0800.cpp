// BO_CAT_REG_0800.cpp : implementation file
//

#include "stdafx.h"
//#include _T("bo_cat_reg.h")
#include "BO_CAT_REG_0800.h"
#include "..\..\..\ESL\SIReportMgr\SIReportManager.h"
#include "..\..\..\공통\MarcAdjustApi\CatApi_BO.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_REG_0800 dialog


CBO_CAT_REG_0800::CBO_CAT_REG_0800(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{

}

CBO_CAT_REG_0800::~CBO_CAT_REG_0800()
{
}     


BOOL CBO_CAT_REG_0800::Create(CWnd* pParentWnd)
{
EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

VOID CBO_CAT_REG_0800::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_REG_0800)
	DDX_Check(pDX, IDC_chkAPPENDIX_ADD, m_chkAppendixAdd);
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_REG_0800, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_REG_0800)
	ON_BN_CLICKED(IDC_rB4, OnrB4)
	ON_BN_CLICKED(IDC_rA4, OnrA4)
	ON_BN_CLICKED(IDC_rA4_RANDSCAPE, OnrA4RANDSCAPE)
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_REG_0800 message handlers

BOOL CBO_CAT_REG_0800::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("BO_CAT_REG_0800")) < 0)	
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	
	
	//++ 2006.12.06 KBS Update - 관리값으로 설정
	GetManageValue( _T("기타") , _T("공통") , _T("육사") , _T("육사") , m_str64 );
	GetManageValue( _T("기타") , _T("공통") , _T("국방대") , _T("국방대") , m_str90 );
	GetManageValue( _T("기타") , _T("공통") , _T("5163") , _T("5163") , m_str5163 );
	GetManageValue( _T("수서") , _T("단행") , _T("원부출력_권차표시설정") , _T("") , m_strVolValue );
	//-- 2006.12.06 KBS 
	
	CButton *pPart, *pA4;

	pPart = (CButton *)GetDlgItem(IDC_rSHELF_PRINT);
	pPart->SetCheck(1);
	if( _T("Y") == m_str90 )
	{
		pA4 = (CButton *)GetDlgItem(IDC_rB4);
		pA4->SetCheck(1);
	}
	else
	{
		pA4 = (CButton *)GetDlgItem(IDC_rA4);
		pA4->SetCheck(1);
	}
	
	//5163만 적용... 장서원부 출력	
	if( _T("Y") != m_str5163 )
	{
		GetDlgItem(IDC_rJANGSEO)->ShowWindow(SW_HIDE);
	}
	

	//===================================================================
	//2007.06.29 ADD PJW : 등록구분코드 콤보박스 초기값 세팅
	//20080612 UPDATE BY PJW : 등록구분값이 디폴트값을 가지도록 수정
	//CComboBox* pCombBOX = NULL;
	//pCombBOX =(CComboBox*) GetDlgItem(IDC_eACCESSION_CODE);
	//pCombBOX->SetCurSel(25);
	CESL_ControlMultiComboBox *pCmb = (CESL_ControlMultiComboBox *)GetDlgItem(IDC_eACCESSION_CODE);
	CString strRegCode;
	m_pInfo->pCodeMgr->GetDefaultCode(_T("등록구분"), strRegCode);
	
	pCmb->SetWindowText(strRegCode);
	//===================================================================
	

	// KOL.RED.2018.220 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//등록구분|M|12049|DROP_LIST||!등록구분.코드,!등록구분.설명|,|||
	//등록번호까지|E|1120|||||||
	//등록번호부터|E|1121|||||||
	{
		CESL_ControlEdit* pCtrl = (CESL_ControlEdit*)FindControl(_T("CM_BO_CAT_REG_0800"), _T("등록번호부터"));
		ASSERT(pCtrl);

		pCtrl->SetKLRegnoMode(TRUE);
		pCtrl->SetRegCodeCombo((CWnd*)FindControl(_T("CM_BO_CAT_REG_0800"), _T("등록구분")), 1);

		pCtrl->SetRegCodeLength(m_pInfo->m_nRegCodeLength);
	}

	{
		CESL_ControlEdit* pCtrl = (CESL_ControlEdit*)FindControl(_T("CM_BO_CAT_REG_0800"), _T("등록번호까지"));
		ASSERT(pCtrl);

		pCtrl->SetKLRegnoMode(TRUE);
		pCtrl->SetRegCodeCombo((CWnd*)FindControl(_T("CM_BO_CAT_REG_0800"), _T("등록구분")), 1);

		pCtrl->SetRegCodeLength(m_pInfo->m_nRegCodeLength);
	}
	// KOL.RED.2018.220 ---------------------------------------------------------------------------	

	
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	return TRUE;



EFS_END
return FALSE;

}

VOID CBO_CAT_REG_0800::OnOK() 
{
EFS_BEGIN

	INT ids = -1;	
	CString strLibName;
	CCatApi_BO CatApi(this);

	/****************************************************
	 ****************************************************/
	//20080502 UPDATE BY PJW : 관리값에 있는 도서관명을 받아온다.
	CatApi.GetLibInfo(&CString(_T("LIB_NAME")), &strLibName, 1);
	/****************************************************
	 ****************************************************/
	strLibName += _T("의 도서관자료로 아래와 같이 등록함.");

	//5163용 장서원부.. 따로 만들어서.. 보내야지 ㅠㅠ;;
	CButton* pJangSeo = (CButton *)GetDlgItem(IDC_rJANGSEO);
	if( pJangSeo->GetCheck() == 1 ) 
	{
		Print_JangSeo();
		return;
	}

	// 원부 번호로 원부 출력
	CButton* pPart = (CButton*)GetDlgItem(IDC_rSHELF_PRINT);
	if( pPart->GetCheck() == 1 ) 
	{
		CSIReportManager SIReportManager(this);
		SIReportManager.m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
		SIReportManager.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
		SIReportManager.DeleteTempPageFiles();
		
		//A4,B4 선택
		CButton * pB4;
		pB4 = (CButton *)GetDlgItem(IDC_rB4);
		if( pB4->GetCheck() == 1 )
		{
			CButton * pB4Serial;
			pB4Serial = (CButton *)GetDlgItem(IDC_ChkSerial);
			if( pB4Serial->GetCheck() == 1 )
			{
				SIReportManager.m_nUserPaperSize = -9999;
				SIReportManager.m_nOrientation = DMORIENT_PORTRAIT;
				SIReportManager.m_nResolution = 1728;
			}
			if		( _T("Y") == m_str64 )	ids = SIReportManager.GetSIReportFileFromDB(_T("원부출력B4_육사"));
			else if	( _T("Y") == m_str90 )	ids = SIReportManager.GetSIReportFileFromDB(_T("원부출력B4_국방대"));
			else							ids = SIReportManager.GetSIReportFileFromDB(_T("원부출력B4"));
		}
		else if (static_cast<CButton*>(GetDlgItem(IDC_rA4_RANDSCAPE))->GetCheck() == 1 ) 
		{
			if ( _T("Y") == m_str64 )	ids = SIReportManager.GetSIReportFileFromDB(_T("원부출력_가로_육사"));
			else						ids = SIReportManager.GetSIReportFileFromDB(_T("원부출력_가로"));
		}
		else
		{
			if ( _T("Y") == m_str64 )	ids = SIReportManager.GetSIReportFileFromDB(_T("원부출력_육사"));
			else						ids = SIReportManager.GetSIReportFileFromDB(_T("원부출력"));
		}
		
		if( ids < 0 )
		{
			AfxMessageBox(_T("DB로 부터 SI Report 정보를 가져오지 못하였습니다."));
			return;
		}
		
		CString sYear, sAccessionNo;
		GetControlData(_T("CM_BO_CAT_REG_0800"), _T("작성년도"), sYear );
		if( sYear == _T("") )
		{
			AfxMessageBox( _T("작성년도를 입력하세요.") );
			return;
		}
		GetControlData( _T("CM_BO_CAT_REG_0800"), _T("원부번호"), sAccessionNo );
		if( sAccessionNo == _T("") )
		{
			AfxMessageBox( _T("원부번호를 입력하세요.") );
			return;
		}

		//원부정보DM
		CESL_DataMgr *pDmAccession;
		pDmAccession = FindDM( _T("DM_BO_CAT_REG_0800_원부") );
		if( pDmAccession == NULL )
		{
			AfxMessageBox( _T("DM_BO_CAT_REG_0800_원부 error") );
			return;
		}
		CString sAccQuery;
		sAccQuery.Format( _T("ACCESSION_REC_NO = '%s' AND ACCESSION_REC_MAKE_YEAR = '%s'"), sAccessionNo, sYear );
		if( pDmAccession->RefreshDataManager( sAccQuery ) < 0 )		return;
		m_pInfo->pCodeMgr->ConvertCodeToDescInDM(pDmAccession, _T("입수구분"), _T("원부입수구분"));	
		INT iAccCount = pDmAccession->GetRowCount();


		CESL_DataMgr *pDM_RegCode = FindDM( _T("DM_BO_CAT_REG_0800_01") );
		if( pDM_RegCode == NULL )
		{
			AfxMessageBox( _T("DM_BO_CAT_PROC_0800_01") );
			return;
		}

		CString strQuery;
		//rQuery.Format(_T("ACCESSION_REC_KEY IN(SELECT REC_KEY FROM CO_ACCESSION_REC_TBL WHERE ACCESSION_REC_NO =%s)")
		//		, sAccessionNo );
		strQuery.Format(_T("ACCESSION_REC_KEY IN(SELECT REC_KEY FROM CO_ACCESSION_REC_TBL WHERE ACCESSION_REC_NO =%s AND ACCESSION_REC_MAKE_YEAR ='%s')")
						, sAccessionNo, sYear );

		CString strExtraCondition = pDM_RegCode->EXTRA_CONDITION;
		pDM_RegCode->EXTRA_CONDITION = _T("GROUP BY REG_CODE ORDER BY REG_CODE");
		if ( pDM_RegCode->RefreshDataManager( strQuery ) < 0 ) return;

		pDM_RegCode->EXTRA_CONDITION = strExtraCondition;

		INT nRegCodeCount = pDM_RegCode->GetRowCount();
		if (nRegCodeCount < 1) 
		{
			AfxMessageBox( _T("검색된 데이터가 없습니다.") );
			return;
		}
		
		// 2005-11-10 황정영: 국방대 본관/분관별 종수/책수 
		CString sQuery, strSMA, strBMA, strSBR, strBBR, strSTOTAL, strBTOTAL;
		if( m_str90 == _T("Y") )
		{
//			INT nStotal, nBtotal;
			//본관 종수
			sQuery.Format(_T("SELECT COUNT(DISTINCT(SPECIES_KEY)) FROM BO_BOOK_TBL WHERE "));
			sQuery += strQuery;
			sQuery += _T(" AND MANAGE_CODE = 'MA'");
			pDM_RegCode->GetOneValueQuery(sQuery, strSMA);
			
			//본관 책수
			sQuery.Format(_T("SELECT COUNT(REG_NO) FROM BO_BOOK_TBL WHERE "));
			sQuery += strQuery;
			sQuery += _T(" AND MANAGE_CODE = 'MA'");
			pDM_RegCode->GetOneValueQuery(sQuery, strBMA);
			
			//분관 종수
			sQuery.Format(_T("SELECT COUNT(DISTINCT(SPECIES_KEY)) FROM BO_BOOK_TBL WHERE "));
			sQuery += strQuery;
			sQuery += _T(" AND MANAGE_CODE = 'BR'");
			pDM_RegCode->GetOneValueQuery(sQuery, strSBR);

			//분관 책수
			sQuery.Format(_T("SELECT COUNT(REG_NO) FROM BO_BOOK_TBL WHERE "));
			sQuery += strQuery;
			sQuery += _T(" AND MANAGE_CODE = 'BR'");
			pDM_RegCode->GetOneValueQuery(sQuery, strBBR);
			
			//합계 종수
			sQuery.Format(_T("SELECT COUNT(DISTINCT(SPECIES_KEY)) FROM BO_BOOK_TBL WHERE "));
			sQuery += strQuery;
			sQuery += _T(" AND MANAGE_CODE IN ('BR','MA') ");
			pDM_RegCode->GetOneValueQuery(sQuery, strSTOTAL);
			
			//합계 책수
			sQuery.Format(_T("SELECT COUNT(DISTINCT(REG_NO)) FROM BO_BOOK_TBL WHERE "));
			sQuery += strQuery;
			sQuery += _T(" AND MANAGE_CODE IN ('BR','MA') ");
			pDM_RegCode->GetOneValueQuery(sQuery, strBTOTAL);

			if(strSMA.IsEmpty())
				strSMA = _T("0");
			if(strBMA.IsEmpty())
				strBMA = _T("0");
			if(strSBR.IsEmpty())
				strSBR = _T("0");
			if(strBBR.IsEmpty())
				strBBR = _T("0");
			if(strSTOTAL.IsEmpty())
				strSTOTAL = _T("0");
			if(strBTOTAL.IsEmpty())
				strBTOTAL = _T("0");
			
/*			//합계
			nStotal = _ttoi(strSMA) + _ttoi(strSBR);
			nBtotal = _ttoi(strBMA) + _ttoi(strBBR);
			
			strSTOTAL.Format(_T("%d"), nStotal);
			strBTOTAL.Format(_T("%d"), nBtotal);*/
		}

		CESL_DataMgr *pDM_0800_02 = FindDM( _T("DM_BO_CAT_REG_0800_02") );
		if( pDM_0800_02 == NULL )
		{
			AfxMessageBox( _T("DM_BO_CAT_REG_0800_01") );
			return;
		}
		
		INT idx = -1;
		INT nPriceSum = 0;
		INT nRowCount = 0;
		INT iSwReg = 0;
		CString sMinRegNo;
		CString sMaxRegNo;
		// 등록구분 별로 출력합니다.
		for (INT idxRegCode = 0; idxRegCode < nRegCodeCount; idxRegCode++)
		{
			CString strRegCode = pDM_RegCode->GetCellData(_T("BB_등록구분"), idxRegCode);

			//등록구분 상관없이
			//strQuery.Format(_T("I.REC_KEY = S.REC_KEY AND S.REC_KEY = B.SPECIES_KEY AND B.REG_CODE = '%s' ")
			//				_T("AND ACCESSION_REC_KEY IN(SELECT REC_KEY FROM CO_ACCESSION_REC_TBL WHERE ACCESSION_REC_NO =%s) ")
			//				_T("ORDER BY B.REG_CODE, B.REG_NO"), strRegCode, sAccessionNo );
			
			//===================================================================
			//2008.07.03 UPDATE BY PJW : 원부출력시 부록자료 포함 유무에 따라 쿼리가 달라지도록 수정
//			strQuery.Format(_T("I.REC_KEY = S.REC_KEY AND S.REC_KEY = B.SPECIES_KEY AND B.REG_CODE = '%s' ")
//							_T("AND ACCESSION_REC_KEY IN(SELECT REC_KEY FROM CO_ACCESSION_REC_TBL WHERE ACCESSION_REC_NO =%s AND ACCESSION_REC_MAKE_YEAR ='%s') ")
//							_T("ORDER BY B.REG_CODE, B.REG_NO"), strRegCode, sAccessionNo, sYear );

			CButton* pBAppendixAdd;
			pBAppendixAdd = (CButton *)GetDlgItem(IDC_chkAPPENDIX_ADD);
			if( pBAppendixAdd->GetCheck() == 1 )
			{
				strQuery.Format(_T("I.REC_KEY = S.REC_KEY AND S.REC_KEY = B.SPECIES_KEY AND B.REG_CODE = '%s' ")
								_T("AND ACCESSION_REC_KEY IN(SELECT REC_KEY FROM CO_ACCESSION_REC_TBL WHERE ACCESSION_REC_NO =%s AND ACCESSION_REC_MAKE_YEAR ='%s') ")
								_T("ORDER BY B.REG_CODE, B.REG_NO"), strRegCode, sAccessionNo, sYear );
			}
			else
			{
				strQuery.Format(_T("I.REC_KEY = S.REC_KEY AND S.REC_KEY = B.SPECIES_KEY AND B.REG_CODE = '%s' ")
								_T("AND ACCESSION_REC_KEY IN(SELECT REC_KEY FROM CO_ACCESSION_REC_TBL WHERE ACCESSION_REC_NO =%s AND ACCESSION_REC_MAKE_YEAR ='%s') ")
								_T("AND BOOK_APPENDIX_FLAG = 'B' ")
								_T("ORDER BY B.REG_CODE, B.REG_NO"), strRegCode, sAccessionNo, sYear );
			}
			//===================================================================

			if ( pDM_0800_02->RefreshDataManager( strQuery ) < 0 ) return;

			// 등록구분 설명으로 바꾸자
			CString strRegCodeDesc;
			m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("등록구분"), strRegCode, strRegCodeDesc);

			nRowCount = pDM_0800_02->GetRowCount();
			if (nRowCount < 1) continue;

			nPriceSum = 0;
			iSwReg = 0;
			for (idx = 0; idx < nRowCount; idx++)
			{
				//형태사항 만들기
				CString sPage = pDM_0800_02->GetCellData( _T("BB_면장수"), idx );
				CString sPhysical = pDM_0800_02->GetCellData( _T("BB_물리적특성"), idx );
				CString sBookSize = pDM_0800_02->GetCellData( _T("BB_크기"), idx );
				CString sAccompMat = pDM_0800_02->GetCellData( _T("BB_딸림자료"), idx );// 소정 추가 2004.10.01 
				CString sForm = sPage;
				if( sPhysical != _T("") )
				{
					if ( sForm != _T("") )
					{
						sForm = sForm + _T(" : ") + sPhysical;
					}
					else
					{
						sForm = sPhysical;
					}
				}
				if( sBookSize != _T("") )
				{
					if( sForm != _T("") )
					{
						sForm = sForm + _T(" ; ") + sBookSize;
					}
					else
					{
						sForm = sBookSize;
					}				
				}
				// 소정 추가 2004.10.01 ++++++++++++++++++++++++++++++++++++
				// 형식구분에 딸림자료 나오게 추가
				if ( sAccompMat != _T("") )
				{
					if( sForm != _T("") )
					{
						sForm = sForm + _T(" + ") + sAccompMat;
					}
					else 
					{
						sForm = sAccompMat;
					}
				}
				// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				pDM_0800_02->SetCellData( _T("형태사항"), sForm, idx );
				//청구기호 만들기
				CString sShelfCode = pDM_0800_02->GetCellData( _T("BB_별치기호_코드"), idx );
				CString sClassNo = pDM_0800_02->GetCellData( _T("BB_분류기호"), idx );
				CString sBookCode = pDM_0800_02->GetCellData( _T("BB_도서기호"), idx );
				CString sVolCode = pDM_0800_02->GetCellData( _T("BB_편권차"), idx );
				CString sDupCode = pDM_0800_02->GetCellData( _T("BB_복본기호"), idx );
				
				// 별치기호 설명으로 바꾸기
				if (!sShelfCode.IsEmpty())
					m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("별치기호"), sShelfCode, sShelfCode);
				CString sCallNo = sShelfCode;
				
				if ( sClassNo != _T("") ) {
					if ( sCallNo != _T("") ) {
						sCallNo = sCallNo + _T(" ") + sClassNo;
					}
					else {
						sCallNo = sClassNo;
					}
				}
				if ( sBookCode != _T("") ) {
					if ( sBookCode != _T("") ) {
						sCallNo = sCallNo + _T("-") + sBookCode;
					}
					else {
						sCallNo = sBookCode;
					}
				}
				if ( sVolCode != _T("") ) {
					if ( sVolCode != _T("") ) {
						sCallNo = sCallNo + _T("-") + sVolCode;
					}
					else {
						sCallNo = sVolCode;
					}
				}
				if ( sDupCode != _T("") ) {
					if ( sDupCode != _T("") ) {
						sCallNo = sCallNo + _T("=") + sDupCode;
					}
					else {
						sCallNo = sDupCode;
					}
				}
				pDM_0800_02->SetCellData( _T("청구기호"), sCallNo, idx );
				//등록번호
				CString sRegNo = pDM_0800_02->GetCellData( _T("BB_등록번호"), idx );
				if ( iSwReg == 0 && sRegNo != _T("") ) {
					sMinRegNo = sRegNo;
					sMaxRegNo = sRegNo;
					iSwReg = 1;
				}
				else {
					if ( sRegNo < sMinRegNo ) {
						sMinRegNo = sRegNo;
					}
					else if ( sRegNo > sMaxRegNo ) {
						sMaxRegNo = sRegNo;
					}
				}
				//마크에서 표제저작사항 만들기
				CString sStreamMarc;			
				pDM_0800_02->GetCellData( _T("BS_MARC"), idx, sStreamMarc );
				CMarc marc;
				m_pInfo->pMarcMgr->Decoding( sStreamMarc, &marc );
				CString sTitleInfo;		//표제저작사항
				CString s245a;
				m_pInfo->pMarcMgr->GetItem( &marc, _T("245$a"), s245a );
				sTitleInfo = s245a;

				CString s245h;
				m_pInfo->pMarcMgr->GetItem( &marc, _T("245$h"), s245h );
				if ( s245h != _T("") ) {
					if ( sTitleInfo != _T("") ) {
						sTitleInfo = sTitleInfo + s245h;
					}
					else {
						sTitleInfo = s245h;
					}
				}
				
				CString s245b;
				m_pInfo->pMarcMgr->GetItem( &marc, _T("245$b"), s245b );
				if ( s245b != _T("") ) {
					if ( sTitleInfo != _T("") ) {
						sTitleInfo = sTitleInfo + _T(" : ") + s245b;
					}
					else {
						sTitleInfo = s245b;
					}
				}

				//m_pInfo->pMarcMgr->GetItem( &marc, _T("245$n"), s245n );
				//s245n = pDM_0800_02->GetCellData( _T("BB_권책기호"), idx );
				CString s245n = _T("");
				if		( _T("0") == m_strVolValue )	m_pInfo->pMarcMgr->GetItem( &marc, _T("245$n"), s245n );
				else if	( _T("1") == m_strVolValue )	s245n = pDM_0800_02->GetCellData( _T("BB_권책기호"), idx );
				else									s245n = _T("");
				if( FALSE == s245n.IsEmpty() )
				{
					sTitleInfo = sTitleInfo + _T(" . ") + s245n;
				}

				CString s245p;
				m_pInfo->pMarcMgr->GetItem( &marc, _T("245$p"), s245p );
				s245p = pDM_0800_02->GetCellData( _T("BB_편제"), idx );
				if ( s245p != _T("") ) {
					sTitleInfo = sTitleInfo + _T(" , ") + s245p;
				}
				CString s245d;
				m_pInfo->pMarcMgr->GetItem( &marc, _T("245$d"), s245d );
				s245d = pDM_0800_02->GetCellData( _T("BB_편저자"), idx );
				if ( s245d != _T("") ) {
					sTitleInfo = sTitleInfo + _T(" / ") + s245d;
				}
				/*
				//++++++++++++++++++++++++++++++++++++++++++++++++++
                //2003.08.28 이학중 추가 
				//원부출력시 표제저작사항에 245$e 추가 되도록 수정 
                CString s245e;
				CArray<CString,CString&> arr245e ;
				m_pInfo->pMarcMgr->GetItem( &marc, _T("245$e"), s245e ,&arr245e);
				if ( s245e != _T("") ) {
					for(INT i = 0 ; i< arr245e.GetSize ();i++) 
					{
					   sTitleInfo = sTitleInfo + _T(" ; ") +  arr245e.GetAt (i);

					}
				}                
				//+++++++++++++++++++++++++++++++++++++++++++++++++++
				*/

				CString s250a;
				m_pInfo->pMarcMgr->GetItem( &marc, _T("250$a"), s250a );
				if ( s250a != _T("") ) {
					sTitleInfo = sTitleInfo + _T(" .- ") + s250a;
				}
/*				//총서표제,총서편차
				CString s490a;
				m_pInfo->pMarcMgr->GetItem( &marc, _T("490$a"), s490a );
				if ( s490a != _T("") ) {
					sTitleInfo = sTitleInfo + _T(" .- ( ") + s490a + _T(" )");
				}
				else {
					CString s440;
					CString s440a;
					m_pInfo->pMarcMgr->GetItem( &marc, _T("440$a"), s440a );
					if ( s440a != _T("") ) {
						CString s440v;
						m_pInfo->pMarcMgr->GetItem( &marc, _T("440$v"), s440v );
						s440 = _T("( ") + s440a + _T(" ; ") + s440v + _T(" )");
					}
					if ( s440 != _T("") ) {
						sTitleInfo = sTitleInfo + _T(" .- ") + s440;
					}
				}		
				pDM_0800_02->SetCellData( _T("IBS_표제사항"), sTitleInfo, idx );
*/				
				//2006-01-24 hjy  수정 : 440 우선
				//총서표제,총서편차
				
				CString s440;
				CString s440a;
				m_pInfo->pMarcMgr->GetItem( &marc, _T("440$a"), s440a );
				if ( s440a != _T("") )
				{
					CString s440v;
					m_pInfo->pMarcMgr->GetItem( &marc, _T("440$v"), s440v );
					s440 = _T("( ") + s440a + _T(" ; ") + s440v + _T(" )");
				}
				if ( s440 != _T("") )
				{
					sTitleInfo = sTitleInfo + _T(" .- ") + s440;
				}
				else
				{
					CString s490a;
					m_pInfo->pMarcMgr->GetItem( &marc, _T("490$a"), s490a );
					if ( s490a != _T("") ) 
					{
						sTitleInfo = sTitleInfo + _T(" .- ( ") + s490a + _T(" )");
					}
				}
				
				pDM_0800_02->SetCellData( _T("IBS_표제사항"), sTitleInfo, idx );
				//발행사항 만들기
				CString sPublishInfo;		//발행사항
				CString s260a;
				m_pInfo->pMarcMgr->GetItem( &marc, _T("260$a"), s260a );
				sPublishInfo = s260a;
				
				CString s260b;
				m_pInfo->pMarcMgr->GetItem( &marc, _T("260$b"), s260b );
				if ( s260b != _T("") )
				{
					if ( sPublishInfo != _T("") )
					{
						sPublishInfo = sPublishInfo + _T(" : ") + s260b;
					}
					else
					{
						sPublishInfo = s260b;
					}
				}
				CString s260c;
				m_pInfo->pMarcMgr->GetItem( &marc, _T("260$c"), s260c );
				if ( s260c != _T("") ) {
					if ( sPublishInfo != _T("") ) {
						sPublishInfo = sPublishInfo + _T(", ") + s260c;
					}
					else {
						sPublishInfo = s260c;
					}
				}
				pDM_0800_02->SetCellData( _T("IBS_발행자사항"), sPublishInfo, idx );
				
				//가격
				CString sPrice;
				pDM_0800_02->GetCellData( _T("BB_가격"), idx, sPrice );

				CString sPriceCharacter;
				pDM_0800_02->GetCellData( _T("BB_가격특성"), idx, sPriceCharacter );
				
				if ( sPrice == _T("0") || sPrice == _T(""))
				{
					pDM_0800_02->SetCellData( _T("BB_가격"), sPriceCharacter, idx );

				}
				else 
				{
					nPriceSum += _ttoi(sPrice);
					// 2005.10.07 REM BY PDJ					
					//현금 표시 포멧으로 만들기					
					//NUMBERFMT nFmt = { 0, 0, 3, _T("."), _T(","), 1 };
					//TCHAR szOUT[20];
					//::GetNumberFormat(NULL, NULL, sPrice, &nFmt, szOUT, 20);
					//sPrice = _T("\\") + sPrice;
					/*
					sPrice.Format(_T("\\%s"), szOUT);
					pDM_0800_02->SetCellData( _T("BB_가격"), sPrice, idx );
					*/
					CString sItem, sPriceCode;
					m_pInfo->pMarcMgr->GetItem( &marc, _T("950$b"), sItem );
					for ( INT i = 0 ; i < sItem.GetLength() ; i++ ) {
						CString sOneWord = sItem.Mid( i, 1 );
						if ( sOneWord >= _T("0") && sOneWord <= _T("9") ) {
							break;
						}
						sPriceCode = sPriceCode + sOneWord;
					}
					
					// 2005.10.07 REM BY PDJ
					// \일 경우 소수점 절사
					CString strTempPrice;
					strTempPrice = sPrice;
					INT nPoint;
					if ( sPriceCode == _T("\\") || sPriceCode == _T(""))
					{						
						nPoint= strTempPrice.Find(_T("."), 0 );
						if ( nPoint > 0 ) sPrice.Format(_T("\\%s"), strTempPrice.Left(nPoint));
						else sPrice.Format(_T("\\%s"), strTempPrice );
					}
					else
					{
						sPrice.Format(_T("%s%s"), sPriceCode, strTempPrice);
					}

					/*
					if ( sPriceCode != _T("") ) {
						sPrice.Format(_T("%s%s"), sPriceCode, szOUT);
					}
					else {
						sPrice.Format(_T("\\%s"), szOUT);
					}
					*/
					pDM_0800_02->SetCellData( _T("BB_가격"), sPrice, idx );
				}			


/* 수정 사항 : 원부출력시 같은 종이 나오면 ' _T(" '로 출력하게 수정 +++
 * 수정 날짜 : 2003.09.03
 * 수정 사람 : 김홍수 
 */
/*
			CString strSKeyPre = pDM_0800_02->GetCellData( _T("BB_종키"), idx-1 );
			CString strSKeyNow = pDM_0800_02->GetCellData( _T("BB_종키"), idx );

				if ( strSKeyPre == strSKeyNow ) 
				{	
					pDM_0800_02->SetCellData( _T("IBS_표제사항"), _T("                '' "), idx );
					pDM_0800_02->SetCellData( _T("IBS_발행자사항"), _T("           '' "), idx );
					pDM_0800_02->SetCellData( _T("형태사항"), _T("            '' "), idx );
					pDM_0800_02->SetCellData( _T("BB_가격"), _T("   ''     "), idx );
					//pDM_0800_02->SetCellData( _T("청구기호"), _T("     '' "), idx );
				}
*/
// 수정 사항 : 원부출력시 같은 종이 나오면 ' _T(" '로 출력하게 수정 +++
				
				// 2005-11-10 황정영 : 국방대 관리구분 설명값, 기증자 정보
				if( m_str90 == _T("Y"))
				{
					CString strManage;
					pDM_0800_02->GetCellData(_T("BB_관리구분"),idx,strManage);
					if(strManage == _T("MA"))
						pDM_0800_02->SetCellData(_T("관리구분"),_T("본관"),idx);
					else if(strManage == _T("BR"))
						pDM_0800_02->SetCellData(_T("관리구분"),_T("분관"),idx);
					else
						pDM_0800_02->SetCellData(_T("관리구분"),_T(""),idx);
					
					//기증자
					CString strDonator, strTmp;
					pDM_0800_02->GetCellData(_T("BB_종키"),idx,strTmp);
					sQuery.Format(_T("SELECT D.DONATOR_NAME ")
								  _T("FROM BO_DONINFO_TBL D, BO_BOOK_TBL B ")
								  _T("WHERE B.SPECIES_KEY = D.SPECIES_KEY ")
								  _T("AND B.ACQ_CODE = '2' ")
								  _T("AND B.SPECIES_KEY = '%s'")
								  _T("AND B.MANAGE_CODE = UDF_MANAGE_CODE")
								  , strTmp);
					pDM_0800_02->GetOneValueQuery(sQuery,strDonator);
					pDM_0800_02->SetCellData(_T("기증자"),strDonator,idx);

					pDM_0800_02->SetCellData(_T("BB_등록일자"), pDmAccession->GetCellData( _T("원부등록일자"), 0 ), idx);
				}
			}
			//출력하기
			CString sAccAcqCode = pDmAccession->GetCellData( _T("원부입수구분"), 0 );
			CString sPrice;
			sPrice.Format(_T("%d"), nPriceSum);

			CString sRegNo = sMinRegNo + _T(" 부터  ") + sMaxRegNo + _T(" 까지");
			//책수 구하기		
			CString sBookCount;
			sBookCount.Format(_T("%d"), nRowCount);
			
			CString sRegDate = pDmAccession->GetCellData( _T("원부등록일자"), 0 );
			SIReportManager.SetTempVariable(_T("수입구분"), sAccAcqCode);
			SIReportManager.SetTempVariable(_T("수입년도"), sYear);
			//가격
			//현금 표시 포멧으로 만들기
			NUMBERFMT nFmt = { 0, 0, 3, _T("."), _T(","), 1 };
			TCHAR szOUT[20];
			::GetNumberFormat(NULL, NULL, sPrice, &nFmt, szOUT, 20);
			CString sPriceFont;		
			sPriceFont.Format( _T("%15s"), szOUT );
			SIReportManager.SetTempVariable(_T("가격"), sPriceFont);
			SIReportManager.SetTempVariable(_T("등록구분"), strRegCodeDesc);
			SIReportManager.SetTempVariable(_T("원부번호"), sAccessionNo);
			SIReportManager.SetTempVariable(_T("등록번호"), sRegNo);

            //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
            //2003.08.23 이학중 수정 
			//원부출력물에 직명,성명 나오게 설정.
			CString sClassName,sRegistrant ;
            strQuery.Format (_T("select class_name FROM CO_ACCESSION_REC_TBL WHERE  ACCESSION_REC_NO = %s AND ACCESSION_REC_MAKE_YEAR = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE"), sAccessionNo, sYear);
            pDmAccession->GetOneValueQuery (strQuery,sClassName);
            strQuery.Format (_T("select REGISTRANT FROM CO_ACCESSION_REC_TBL WHERE  ACCESSION_REC_NO = %s AND ACCESSION_REC_MAKE_YEAR = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE"),sAccessionNo, sYear );
			pDmAccession->GetOneValueQuery (strQuery,sRegistrant); 
            
			SIReportManager.SetTempVariable(_T("직명"), sClassName);
			SIReportManager.SetTempVariable(_T("성명"), sRegistrant);
			//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			
			//2005-11-10 황정영 : 갑지/을지 수정
			if( m_str90 == _T("Y"))
			{
				SIReportManager.SetTempVariable(_T("본관종수"), strSMA);
				SIReportManager.SetTempVariable(_T("본관책수"), strBMA);
				SIReportManager.SetTempVariable(_T("분관종수"), strSBR);
				SIReportManager.SetTempVariable(_T("분관책수"), strBBR);
				SIReportManager.SetTempVariable(_T("종합계"), strSTOTAL);
				SIReportManager.SetTempVariable(_T("책합계"), strBTOTAL);
			}

			//책수 줄 맞추기
			CString sBookFont;
			sBookFont.Format( _T("%15s"), sBookCount );
			SIReportManager.SetTempVariable(_T("책수"), sBookFont);		
			SIReportManager.SetTempVariable(_T("등록일자"), sRegDate);

			// 2005.06.27 ADD BY PDJ
			// 비고란 추가됨
			CString sRemark = pDmAccession->GetCellData( _T("비고"), 0 );
			SIReportManager.SetTempVariable(_T("비고"), sRemark);
			
			SIReportManager.SetTempVariable(_T("LIB_NAME"), strLibName);
			
			SIReportManager.SetDataMgr(0, pDM_0800_02);
			//ids = SIReportManager.MakeXMLPageFiles();
			ids = SIReportManager.MAKEPAGE_DrawWorkForm(-1);		// 워크폼 Manual 반복 모드
			// 마지막 수평선을 그림
			Print_SetLastPageLine();			
		}
		ids = SIReportManager.Print2();
	}

	//등록번호범위 출력
	//=====================================================
	//2008.11.04 ADD BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
	INT nRegCodeLength;
	nRegCodeLength = m_pInfo->nREG_CODE_LENGTH;
	//=====================================================
	CButton * pACCESSIONNO_PRINT;
	pACCESSIONNO_PRINT = (CButton *)GetDlgItem(IDC_rACCESSIONNO_PRINT);	
	if ( pACCESSIONNO_PRINT->GetCheck() == 1 )
	{
		CString sRegNoStart, sRegNoEnd;
		GetControlData( _T("CM_BO_CAT_REG_0800"), _T("등록번호부터"), sRegNoStart );
		GetControlData( _T("CM_BO_CAT_REG_0800"), _T("등록번호까지"), sRegNoEnd );
		sRegNoStart.Remove(' ');
		sRegNoEnd.Remove(' ');
		/*if ( sRegNoStart == _T("") || sRegNoEnd == _T("") ) {
			AfxMessageBox( _T("등록번호 범위를 입력하세요.") );
			return;
		}*/
		//2007.06.29 ADD PJW : 등록번호구분코드를 콤보박스로 변경
		//=====================================================
		//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 		if ( sRegNoStart == _T("") || sRegNoStart.GetLength() > 10) 
// 		{
// 			AfxMessageBox( _T("시작 검색 등록번호를 자리 수 내에서 입력하세요.") );
// 			SetDlgItemText( IDC_eACCESSION_NO_START, _T("") );
// 			return;
// 		}
// 
// 		if (  sRegNoEnd == _T("") || sRegNoEnd.GetLength() > 10) 
// 		{
// 			AfxMessageBox( _T("마지막 검색 등록번호를 자리 수 내에서 입력하세요.") );
// 			SetDlgItemText(IDC_eACCESSION_NO_END,_T(""));
// 			return;
// 		}
// 		CString sRegNoCode;
// 		GetDlgItemText(IDC_eACCESSION_CODE,sRegNoCode);
// 		sRegNoCode.Remove(' ');
// 
// 		if ( sRegNoCode == _T("") || sRegNoCode.GetLength() !=2 ) 
// 		{
// 			AfxMessageBox( _T("등록구분 코드를 정확히 입력하세요.") );
// 			//pCombBOX->SetCurSel(0);
// 			return;
// 		}
// 		
// 		if ( sRegNoStart != _T("") && sRegNoStart.GetLength() <= 10 ) {		
// 			if ( sRegNoStart.GetLength() > 0 ) {
// 				CString sRight;
// 				sRight.Format( _T("%010s") , sRegNoStart );
// 				sRegNoStart = sRegNoCode + sRight;
// 			}
// 		}
// 		
// 
// 		if ( sRegNoEnd != _T("") && sRegNoEnd.GetLength() <= 10 ) {
// 			if ( sRegNoEnd.GetLength() > 0 ) {
// 				CString sRight;
// 				sRight.Format( _T("%010s") , sRegNoEnd );
// 				sRegNoEnd = sRegNoCode + sRight;
// 			}
// 		}

		if ( sRegNoStart == _T("") || sRegNoStart.GetLength() > 12-nRegCodeLength) 
		{
			AfxMessageBox( _T("시작 검색 등록번호를 자리 수 내에서 입력하세요.") );
			SetDlgItemText( IDC_eACCESSION_NO_START, _T("") );
			return;
		}

		if (  sRegNoEnd == _T("") || sRegNoEnd.GetLength() > 12-nRegCodeLength) 
		{
			AfxMessageBox( _T("마지막 검색 등록번호를 자리 수 내에서 입력하세요.") );
			SetDlgItemText(IDC_eACCESSION_NO_END,_T(""));
			return;
		}
		CString sRegNoCode;
		GetDlgItemText(IDC_eACCESSION_CODE,sRegNoCode);
		sRegNoCode.Remove(' ');

		if ( sRegNoCode == _T("") || sRegNoCode.GetLength() !=nRegCodeLength ) 
		{
			AfxMessageBox( _T("등록구분 코드를 정확히 입력하세요.") );
			//pCombBOX->SetCurSel(0);
			return;
		}
		
		if ( sRegNoStart != _T("") && sRegNoStart.GetLength() <= 12-nRegCodeLength ) {		
			if ( sRegNoStart.GetLength() > 0 ) {
				sRegNoStart = m_pInfo->MakeRegNo(sRegNoCode,sRegNoStart);
			}
		}
		

		if ( sRegNoEnd != _T("") && sRegNoEnd.GetLength() <= 12-nRegCodeLength ) {
			if ( sRegNoEnd.GetLength() > 0 ) {
				sRegNoEnd = m_pInfo->MakeRegNo(sRegNoCode,sRegNoEnd);
			}
		}
		//=====================================================
		

		
		

		//2007.06.29 DELETE PJW : 등록번호구분코드를 콤보박스로 받으므로 코드 비교 부분 삭제
		/*
		if ( sRegNoStart != _T("") && sRegNoStart.GetLength() < 12 ) {		
			if ( sRegNoStart.GetLength() > 2 ) {
				CString sRight;
				sRight.Format( _T("%010s") , sRegNoStart.Mid(2) );
				sRegNoStart = sRegNoStart.Left(2) + sRight;
			}
		}
		if ( sRegNoEnd != _T("") && sRegNoEnd.GetLength() < 12 ) {
			if ( sRegNoEnd.GetLength() > 2 ) {
				CString sRight;
				sRight.Format( _T("%010s") , sRegNoEnd.Mid(2) );
				sRegNoEnd = sRegNoEnd.Left(2) + sRight;
			}
		}

		if ( sRegNoStart.Left(2) != sRegNoEnd.Left(2) ) {
			AfxMessageBox( _T("등록구분이 같아야 합니다.") );
			return;
		}
		*/

		CESL_DataMgr *pDM;
		pDM = FindDM( _T("DM_BO_CAT_REG_0800_01") );
		if ( pDM == NULL ) {
			AfxMessageBox( _T("DM_BO_CAT_PROC_0800_01") );
			return;
		}

		CString strQuery;
		//=====================================================
		//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 		strQuery.Format( _T("REG_NO >= '%s' AND REG_NO <= '%s' AND REG_CODE = '%s'"), sRegNoStart, sRegNoEnd, sRegNoStart.Left(2) );

		strQuery.Format( _T("REG_NO >= '%s' AND REG_NO <= '%s' AND REG_CODE = '%s'"), sRegNoStart, sRegNoEnd, sRegNoStart.Left(nRegCodeLength) );
		//=====================================================
		
		if ( pDM->RefreshDataManager( strQuery ) < 0 ) return;

		INT nRowCount = pDM->GetRowCount();
		if ( nRowCount < 1 ) {
			AfxMessageBox( _T("검색된 데이터가 없습니다.") );
			return;
		}
		
		//2005-11-10 황정영 추가
		CString sQuery, strSMA, strBMA, strSBR, strBBR, strSTOTAL, strBTOTAL;
		if( m_str90 == _T("Y"))
		{
//			INT nStotal, nBtotal;

			//본관 종수
			sQuery.Format(_T("SELECT COUNT(DISTINCT(SPECIES_KEY)) FROM BO_BOOK_TBL WHERE "));
			sQuery += strQuery;
			sQuery += _T(" AND MANAGE_CODE = 'MA'");
			pDM->GetOneValueQuery(sQuery, strSMA);
			
			//본관 책수
			sQuery.Format(_T("SELECT COUNT(REG_NO) FROM BO_BOOK_TBL WHERE "));
			sQuery += strQuery;
			sQuery += _T(" AND MANAGE_CODE = 'MA'");
			pDM->GetOneValueQuery(sQuery, strBMA);
			
			//분관 종수
			sQuery.Format(_T("SELECT COUNT(DISTINCT(SPECIES_KEY)) FROM BO_BOOK_TBL WHERE "));
			sQuery += strQuery;
			sQuery += _T(" AND MANAGE_CODE = 'BR'");
			pDM->GetOneValueQuery(sQuery, strSBR);

			//분관 책수
			sQuery.Format(_T("SELECT COUNT(REG_NO) FROM BO_BOOK_TBL WHERE "));
			sQuery += strQuery;
			sQuery += _T(" AND MANAGE_CODE = 'BR'");
			pDM->GetOneValueQuery(sQuery, strBBR);

			//합계 종수
			sQuery.Format(_T("SELECT COUNT(DISTINCT(SPECIES_KEY)) FROM BO_BOOK_TBL WHERE "));
			sQuery += strQuery;
			sQuery += _T(" AND MANAGE_CODE IN ('BR','MA') ");
			pDM->GetOneValueQuery(sQuery, strSTOTAL);
			
			//합계 책수
			sQuery.Format(_T("SELECT COUNT(DISTINCT(REG_NO)) FROM BO_BOOK_TBL WHERE "));
			sQuery += strQuery;
			sQuery += _T(" AND MANAGE_CODE IN ('BR','MA') ");
			pDM->GetOneValueQuery(sQuery, strBTOTAL);

			if(strSMA.IsEmpty())
				strSMA = _T("0");
			if(strBMA.IsEmpty())
				strBMA = _T("0");
			if(strSBR.IsEmpty())
				strSBR = _T("0");
			if(strBBR.IsEmpty())
				strBBR = _T("0");
			if(strSTOTAL.IsEmpty())
				strSTOTAL = _T("0");
			if(strBTOTAL.IsEmpty())
				strBTOTAL = _T("0");
			
/*			//합계
			nStotal = _ttoi(strSMA) + _ttoi(strSBR);
			nBtotal = _ttoi(strBMA) + _ttoi(strBBR);
			
			strSTOTAL.Format(_T("%d"), nStotal);
			strBTOTAL.Format(_T("%d"), nBtotal);*/
		}

		CESL_DataMgr *pDM_0800_02 = FindDM( _T("DM_BO_CAT_REG_0800_02") );
		if ( pDM_0800_02 == NULL ) {
			AfxMessageBox( _T("DM_BO_CAT_REG_0800_01") );
			return;
		}

		CString sRegCode;
		CString sMinRegNo, sMaxRegNo;
		CString sRegCodeDes;
		//책수
		INT iBookCount = 0;
		for ( INT i = 1 ; i <= nRowCount ; i++ ) 
		{
			sRegCode = pDM->GetCellData( _T("BB_등록구분"), i-1 );
			strQuery.Format(_T("I.REC_KEY = S.REC_KEY AND S.REC_KEY = B.SPECIES_KEY ")
							_T("AND B.REG_NO >= '%s' AND B.REG_NO <= '%s' ")
							_T("AND B.REG_CODE = '%s' AND B.WORKING_STATUS > 'BOC' ORDER BY B.REG_NO")
							, sRegNoStart, sRegNoEnd, sRegCode );
			//===================================================================
			//2008.07.03 UPDATE BY PJW : 원부출력시 부록자료 포함 유무에 따라 쿼리가 달라지도록 수정
//			strQuery.Format(_T("I.REC_KEY = S.REC_KEY AND S.REC_KEY = B.SPECIES_KEY ")
//							_T("AND B.REG_NO >= '%s' AND B.REG_NO <= '%s' ")
//							_T("AND B.REG_CODE = '%s' AND B.WORKING_STATUS > 'BOC' ORDER BY B.REG_NO")
//							, sRegNoStart, sRegNoEnd, sRegCode );
			CButton* pBAppendixAdd;
			pBAppendixAdd = (CButton *)GetDlgItem(IDC_chkAPPENDIX_ADD);
			if( pBAppendixAdd->GetCheck() == 1 )
			{
				strQuery.Format(_T("I.REC_KEY = S.REC_KEY AND S.REC_KEY = B.SPECIES_KEY ")
								_T("AND B.REG_NO >= '%s' AND B.REG_NO <= '%s' ")
								_T("AND B.REG_CODE = '%s' AND B.WORKING_STATUS > 'BOC' ORDER BY B.REG_NO")
								, sRegNoStart, sRegNoEnd, sRegCode );
			}
			else
			{
				strQuery.Format(_T("I.REC_KEY = S.REC_KEY AND S.REC_KEY = B.SPECIES_KEY ")
								_T("AND B.REG_NO >= '%s' AND B.REG_NO <= '%s' ")
								_T("AND BOOK_APPENDIX_FLAG = 'B' ")
								_T("AND B.REG_CODE = '%s' AND B.WORKING_STATUS > 'BOC' ORDER BY B.REG_NO")
								, sRegNoStart, sRegNoEnd, sRegCode );
			}
			//===================================================================


			if ( pDM_0800_02->RefreshDataManager( strQuery ) < 0 ) return;
			sRegCodeDes = pDM->GetCellData( _T("BB_등록구분"), i-1 );
			INT nRowCount = pDM_0800_02->GetRowCount();
			//책수
			iBookCount = nRowCount;
			INT iSwReg = 0;
			for ( INT j = 0 ; j < nRowCount ; j ++ ) 
			{
				//형태사항 만들기
				CString sPage = pDM_0800_02->GetCellData( _T("BB_면장수"), j );
				CString sPhysical = pDM_0800_02->GetCellData( _T("BB_물리적특성"), j );
				CString sBookSize = pDM_0800_02->GetCellData( _T("BB_크기"), j );
				CString sAccompMat = pDM_0800_02->GetCellData( _T("BB_딸림자료"), j );// 소정 추가 2004.10.01
				CString sForm = sPage;
				if ( sPhysical != _T("") ) {
					if ( sForm != _T("") ) {
						sForm = sForm + _T(" : ") + sPhysical;
					}
					else {
						sForm = sPhysical;
					}
				}
				if ( sBookSize != _T("") ) {
					if ( sForm != _T("") ) {
						sForm = sForm + _T(" ; ") + sBookSize;
					}
					else {
						sForm = sBookSize;
					}				
				}
				// 소정 추가 2004.10.01 ++++++++++++++++++++++++++++++++++++
				// 형식구분에 딸림자료 나오게 추가
				if ( sAccompMat != _T("") )
				{
					if ( sForm != _T("") )
						sForm = sForm + _T(" + ") + sAccompMat;
					else 
						sForm = sAccompMat;
				}
				// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				pDM_0800_02->SetCellData( _T("형태사항"), sForm, j );
				//청구기호 만들기
				CString sShelfCode = pDM_0800_02->GetCellData( _T("BB_별치기호_코드"), j );
				CString sClassNo = pDM_0800_02->GetCellData( _T("BB_분류기호"), j );
				CString sBookCode = pDM_0800_02->GetCellData( _T("BB_도서기호"), j );
				CString sVolCode = pDM_0800_02->GetCellData( _T("BB_편권차"), j );
				CString sDupCode = pDM_0800_02->GetCellData( _T("BB_복본기호"), j );
				// 별치기호 설명으로 바꾸기
				if (!sShelfCode.IsEmpty())
					m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("별치기호"), sShelfCode, sShelfCode);
				CString sCallNo = sShelfCode;
				
				if ( sClassNo != _T("") ) {
					if ( sCallNo != _T("") ) {
						sCallNo = sCallNo + _T(" ") + sClassNo;
					}
					else {
						sCallNo = sClassNo;
					}
				}
				if ( sBookCode != _T("") ) {
					if ( sBookCode != _T("") ) {
						sCallNo = sCallNo + _T("-") + sBookCode;
					}
					else {
						sCallNo = sBookCode;
					}
				}
				if ( sVolCode != _T("") ) {
					if ( sVolCode != _T("") ) {
						sCallNo = sCallNo + _T("-") + sVolCode;
					}
					else {
						sCallNo = sVolCode;
					}
				}
				if ( sDupCode != _T("") ) {
					if ( sDupCode != _T("") ) {
						sCallNo = sCallNo + _T("=") + sDupCode;
					}
					else {
						sCallNo = sDupCode;
					}
				}

				pDM_0800_02->SetCellData( _T("청구기호"), sCallNo, j );

				//등록번호
				CString sRegNo = pDM_0800_02->GetCellData( _T("BB_등록번호"), j );
				if ( iSwReg == 0 && sRegNo != _T("") ) {
					sMinRegNo = sRegNo;
					sMaxRegNo = sRegNo;
					iSwReg = 1;
				}
				else {
					if ( sRegNo < sMinRegNo ) {
						sMinRegNo = sRegNo;
					}
					else if ( sRegNo > sMaxRegNo ) {
						sMaxRegNo = sRegNo;
					}
				}

				//마크에서 표제저작사항 만들기
				CString sStreamMarc;			
				pDM_0800_02->GetCellData( _T("BS_MARC"), j, sStreamMarc );
				CMarc marc;
				m_pInfo->pMarcMgr->Decoding( sStreamMarc, &marc );
				CString sTitleInfo;		//표제저작사항
				CString s245a;
				m_pInfo->pMarcMgr->GetItem( &marc, _T("245$a"), s245a );
				sTitleInfo = s245a;

				CString s245h;
				m_pInfo->pMarcMgr->GetItem( &marc, _T("245$h"), s245h );
				if ( s245h != _T("") ) {
					if ( sTitleInfo != _T("") ) {
						sTitleInfo = sTitleInfo + s245h;
					}
					else {
						sTitleInfo = s245h;
					}
				}

				CString s245b;
				m_pInfo->pMarcMgr->GetItem( &marc, _T("245$b"), s245b );
				if ( s245b != _T("") ) {
					if ( sTitleInfo != _T("") ) {
						sTitleInfo = sTitleInfo + _T(" : ") + s245b;
					}
					else {
						sTitleInfo = s245b;
					}
				}
				
				//m_pInfo->pMarcMgr->GetItem( &marc, _T("245$n"), s245n );
				//s245n = pDM_0800_02->GetCellData( _T("BB_권책기호"), j );
				CString s245n = _T("");
				if		( _T("0") == m_strVolValue )	m_pInfo->pMarcMgr->GetItem( &marc, _T("245$n"), s245n );
				else if	( _T("1") == m_strVolValue )	s245n = pDM_0800_02->GetCellData( _T("BB_권책기호"), j );
				else									s245n = _T("");
				if( FALSE == s245n.IsEmpty() )
				{
					sTitleInfo = sTitleInfo + _T(" . ") + s245n;
				}

				CString s245p;
				m_pInfo->pMarcMgr->GetItem( &marc, _T("245$p"), s245p );
				s245p = pDM_0800_02->GetCellData( _T("BB_편제"), j );
				if ( s245p != _T("") ) {
					sTitleInfo = sTitleInfo + _T(" , ") + s245p;
				}
				CString s245d;
				m_pInfo->pMarcMgr->GetItem( &marc, _T("245$d"), s245d );
				s245d = pDM_0800_02->GetCellData( _T("BB_편저자"), j );
				if ( s245d != _T("") ) {
					sTitleInfo = sTitleInfo + _T(" / ") + s245d;
				}			
				CString s250a;
				m_pInfo->pMarcMgr->GetItem( &marc, _T("250$a"), s250a );
				if ( s250a != _T("") ) {
					sTitleInfo = sTitleInfo + _T(" .- ") + s250a;
				}
/*				//총서표제,총서편차
				CString s490a;
				m_pInfo->pMarcMgr->GetItem( &marc, _T("490$a"), s490a );
				if ( s490a != _T("") ) {
					sTitleInfo = sTitleInfo + _T(" .- ( ") + s490a + _T(" )");
				}
				else {
					CString s440;
					CString s440a;
					m_pInfo->pMarcMgr->GetItem( &marc, _T("440$a"), s440a );
					if ( s440a != _T("") ) {
						CString s440v;
						m_pInfo->pMarcMgr->GetItem( &marc, _T("440$v"), s440v );
						s440 = _T("( ") + s440a + _T(" ; ") + s440v + _T(" )");
					}
					if ( s440 != _T("") ) {
						sTitleInfo = sTitleInfo + _T(" .- ") + s440;
					}
				}		
				pDM_0800_02->SetCellData( _T("IBS_표제사항"), sTitleInfo, j );
*/
				//2006-01-24 hjy  수정 : 440 우선
				//총서표제,총서편차
				
				CString s440;
				CString s440a;
				m_pInfo->pMarcMgr->GetItem( &marc, _T("440$a"), s440a );
				if ( s440a != _T("") ) {
					CString s440v;
					m_pInfo->pMarcMgr->GetItem( &marc, _T("440$v"), s440v );
					s440 = _T("( ") + s440a + _T(" ; ") + s440v + _T(" )");
				}
				if ( s440 != _T("") ) {
					sTitleInfo = sTitleInfo + _T(" .- ") + s440;
				}
				else
				{
					CString s490a;
					m_pInfo->pMarcMgr->GetItem( &marc, _T("490$a"), s490a );
					if ( s490a != _T("") ) 
					{
						sTitleInfo = sTitleInfo + _T(" .- ( ") + s490a + _T(" )");
					}
				}

				pDM_0800_02->SetCellData( _T("IBS_표제사항"), sTitleInfo, j );
				//발행사항 만들기
				CString sPublishInfo;		//발행사항
				CString s260a;
				m_pInfo->pMarcMgr->GetItem( &marc, _T("260$a"), s260a );
				sPublishInfo = s260a;

				CString s260b;
				m_pInfo->pMarcMgr->GetItem( &marc, _T("260$b"), s260b );
				if ( s260b != _T("") ) {
					if ( sPublishInfo != _T("") ) {
						sPublishInfo = sPublishInfo + _T(" : ") + s260b;
					}
					else {
						sPublishInfo = s260b;
					}
				}
				CString s260c;
				m_pInfo->pMarcMgr->GetItem( &marc, _T("260$c"), s260c );
				if ( s260c != _T("") ) {
					if ( sPublishInfo != _T("") ) {
						sPublishInfo = sPublishInfo + _T(", ") + s260c;
					}
					else {
						sPublishInfo = s260c;
					}
				}
				pDM_0800_02->SetCellData( _T("IBS_발행자사항"), sPublishInfo, j );
				
				//가격
				CString sPrice;
				pDM_0800_02->GetCellData( _T("BB_가격"), j, sPrice );

				CString sPriceCharacter;
				pDM_0800_02->GetCellData( _T("BB_가격특성"), j, sPriceCharacter );
				
				if ( sPrice == _T("0") || sPrice == _T(""))
				{
					pDM_0800_02->SetCellData( _T("BB_가격"), sPriceCharacter, j );

				}
				else 
				{
					//===================================================
					//2010.04.30 UPDATE BY PJW : 외국 화폐일경우 그대로 넣도록 수정함
//					//현금 표시 포멧으로 만들기
//					TCHAR szOUT[20];
//					//sPrice = _T("\\") + sPrice;
//					/*
//					sPrice.Format(_T("\\%s"), szOUT);
//					pDM_0800_02->SetCellData( _T("BB_가격"), sPrice, j );
//					*/
//					CString sItem, sPriceCode;
//					m_pInfo->pMarcMgr->GetItem( &marc, _T("950$b"), sItem );
//					for ( INT i = 0 ; i < sItem.GetLength() ; i++ ) {
//						CString sOneWord = sItem.Mid( i, 1 );
//						if ( sOneWord >= _T("0") && sOneWord <= _T("9") ) {
//							break;
//						}
//						sPriceCode = sPriceCode + sOneWord;
//					}
//
//					if ( sPriceCode == _T("\\") )
//					{
//						NUMBERFMT nFmt = { 0, 0, 3, _T("."), _T(","), 1 };
//						::GetNumberFormat(NULL, NULL, sPrice, &nFmt, szOUT, 20);
//					}
//					else 
//					{
//						NUMBERFMT nFmt = { 0, 0, 20, _T("."), _T("."), 1 };
//						::GetNumberFormat(NULL, NULL, sPrice, &nFmt, szOUT, 20);
//					}
//
//
//					if ( sPriceCode != _T("") ) {
//						sPrice.Format(_T("%s%s"), sPriceCode, szOUT);
//					}
//					else {
//						sPrice.Format(_T("\\%s"), szOUT);
//					}
//					pDM_0800_02->SetCellData( _T("BB_가격"), sPrice, j );
					CString sItem, sPriceCode;
					m_pInfo->pMarcMgr->GetItem( &marc, _T("950$b"), sItem );
					for ( INT i = 0 ; i < sItem.GetLength() ; i++ ) {
						CString sOneWord = sItem.Mid( i, 1 );
						if ( sOneWord >= _T("0") && sOneWord <= _T("9") ) {
							break;
						}
						sPriceCode = sPriceCode + sOneWord;
					}

					// \일 경우 소수점 절사
					CString strTempPrice;
					strTempPrice = sPrice;
					INT nPoint;
					if ( sPriceCode == _T("\\") || sPriceCode == _T(""))
					{						
						nPoint= strTempPrice.Find(_T("."), 0 );
						if ( nPoint > 0 ) sPrice.Format(_T("\\%s"), strTempPrice.Left(nPoint));
						else sPrice.Format(_T("\\%s"), strTempPrice );
					}
					else
					{
						sPrice.Format(_T("%s%s"), sPriceCode, strTempPrice);
					}

					pDM_0800_02->SetCellData( _T("BB_가격"), sPrice, j );
					//===================================================
				}

				//비고 950$c값을 넣어달라 하셨음
				CString s950c;
				m_pInfo->pMarcMgr->GetItem( &marc, _T("950$c"), s950c );
				pDM_0800_02->SetCellData( _T("BB_비고"), s950c, j );
				
				//2005-11-10 황정영 : 국방대 관리구분, 기증자 값
				CString sQuery;
				if(m_str90 == _T("Y"))
				{
					CString strManage;
					pDM_0800_02->GetCellData(_T("BB_관리구분"),j,strManage);
					if(strManage == _T("MA"))
						pDM_0800_02->SetCellData(_T("관리구분"),_T("본관"),j);
					else if(strManage == _T("BR"))
						pDM_0800_02->SetCellData(_T("관리구분"),_T("분관"),j);
					else
						pDM_0800_02->SetCellData(_T("관리구분"),_T(""),j);
					
					//기증자
					CString strDonator, strTmp;
					pDM_0800_02->GetCellData(_T("BB_종키"),j,strTmp);
					sQuery.Format(_T("SELECT D.DONATOR_NAME ")
								  _T("FROM BO_DONINFO_TBL D, BO_BOOK_TBL B ")
								  _T("WHERE B.SPECIES_KEY = D.SPECIES_KEY ")
								  _T("AND B.ACQ_CODE = '2' ")
								  _T("AND B.SPECIES_KEY = '%s' ")
								  _T("AND B.MANAGE_CODE = UDF_MANAGE_COD E")
								  , strTmp);
					pDM_0800_02->GetOneValueQuery(sQuery,strDonator);
					pDM_0800_02->SetCellData(_T("기증자"),strDonator,j);
				}

			}			
		}
		m_pInfo->pCodeMgr->ConvertCodeToDescInDM(pDM, _T("등록구분"), _T("BB_등록구분"));
		sRegCodeDes = pDM->GetCellData( _T("BB_등록구분"), 0 );
		
		//출력하기
		CSIReportManager *pSIReportManager = new CSIReportManager(this);
		pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
		if (!pSIReportManager) {
			AfxMessageBox(_T("Report Manager를 생성하지 못하였습니다."));
			return;
		}
		
		pSIReportManager->SetCONNECTION_INFO(pDM->CONNECTION_INFO);
		//A4,B4 선택
		CButton * pB4;
		pB4 = (CButton *)GetDlgItem(IDC_rB4);
		if( pB4->GetCheck() == 1 )
		{
			CButton* pB4Serial;
			pB4Serial = (CButton *)GetDlgItem(IDC_ChkSerial);
			if( pB4Serial->GetCheck() == 1 )
			{
				pSIReportManager->m_nUserPaperSize = -9999;
				pSIReportManager->m_nOrientation = DMORIENT_PORTRAIT;
				pSIReportManager->m_nResolution = 1728;
			}
			if		( m_str64 == _T("Y") )		ids = pSIReportManager->GetSIReportFileFromDB(_T("원부출력B4_육사"));
			else if ( m_str90 == _T("Y") )		ids = pSIReportManager->GetSIReportFileFromDB(_T("원부출력B4_국방대"));
			else							ids = pSIReportManager->GetSIReportFileFromDB(_T("원부출력B4"));
		}
		else if (static_cast<CButton*>(GetDlgItem(IDC_rA4_RANDSCAPE))->GetCheck() == 1 ) 
		{
			if ( m_str64 == _T("Y") ) ids = pSIReportManager->GetSIReportFileFromDB(_T("원부출력_가로_육사"));
			else ids = pSIReportManager->GetSIReportFileFromDB(_T("원부출력_가로"));
		}
		else
		{
			if ( m_str64 == _T("Y") ) ids = pSIReportManager->GetSIReportFileFromDB(_T("원부출력_육사"));
			else ids = pSIReportManager->GetSIReportFileFromDB(_T("원부출력"));
		}
		
		if (ids < 0) {
			AfxMessageBox(_T("DB로 부터 SI Report 정보를 가져오지 못하였습니다."));
			return;
		}
		
		CString sPrice = pDM->GetCellData( _T("가격"), 0 );
		CString sRegNo = sMinRegNo + _T(" 부터  ") + sMaxRegNo + _T(" 까지");
		CString sBookCount;
		sBookCount.Format( _T("%d"), iBookCount );
		
		pSIReportManager->SetTempVariable(_T("LIB_NAME"), strLibName);
		//가격
		//현금 표시 포멧으로 만들기
		NUMBERFMT nFmt = { 0, 0, 3, _T("."), _T(","), 1 };
		TCHAR szOUT[20];
		::GetNumberFormat(NULL, NULL, sPrice, &nFmt, szOUT, 20);
		CString sPriceFont;		
		sPriceFont.Format( _T("%15s"), szOUT );
		pSIReportManager->SetTempVariable(_T("가격"), sPriceFont);
		pSIReportManager->SetTempVariable(_T("등록구분"), sRegCodeDes);
		pSIReportManager->SetTempVariable(_T("등록번호"), sRegNo);
		
		//2005-11-10 황정영 : 갑지/을지 수정
		if(m_str90 == _T("Y"))
		{
			pSIReportManager->SetTempVariable(_T("본관종수"), strSMA);
			pSIReportManager->SetTempVariable(_T("본관책수"), strBMA);
			pSIReportManager->SetTempVariable(_T("분관종수"), strSBR);
			pSIReportManager->SetTempVariable(_T("분관책수"), strBBR);
			pSIReportManager->SetTempVariable(_T("종합계"), strSTOTAL);
			pSIReportManager->SetTempVariable(_T("책합계"), strBTOTAL);
		}

		//책수 줄 맞추기
		CString sBookFont;
		sBookFont.Format( _T("%15s"), sBookCount );
		pSIReportManager->SetTempVariable(_T("책수"), sBookFont);
		pSIReportManager->SetDataMgr(0, pDM_0800_02);
		ids = pSIReportManager->MakeXMLPageFiles();
		// 마지막 수평선을 그림
		Print_SetLastPageLine();
		ids = pSIReportManager->Print2();
		if (pSIReportManager) {
			delete pSIReportManager;
			pSIReportManager = NULL;
		}
	}

EFS_END
}

INT CBO_CAT_REG_0800::SIReportEventHandler( CSIReportEventInfo * EventInfo )
{
EFS_BEGIN

	CSIReportManager *pSIReportManager = (CSIReportManager *)EventInfo->m_pSIReportManager;
	if (!pSIReportManager) return -1;
	return 0;

EFS_END
return -1;

}

VOID CBO_CAT_REG_0800::Print_SetLastPageLine(VOID)
{
EFS_BEGIN

	INT ids;

	// XML 과 WorkForm객체를 생성한다.
	CSIReportXMLIOMgr *pXMLIOMgr = new CSIReportXMLIOMgr(MASTER_IS_MANAGER);
	CSIReportWorkForm *pPageWorkForm = new CSIReportWorkForm(MASTER_IS_MANAGER);
	
	if (pXMLIOMgr == NULL) {
		AfxMessageBox(_T("XML 처리기 생성에 실패하였습니다."));
		return;
	}
	if (pPageWorkForm == NULL) {
		AfxMessageBox(_T("중간 XML 처리기 생성에 실패하였습니다."));
		return;
	}

	// 현재 마지막 페이지 번호를 가져온다.
	INT nMaxTmpPageNum = pXMLIOMgr->GetMaxTmpPageNum();
	if (nMaxTmpPageNum <= 0) {
		AfxMessageBox(_T("페이지 정보를 찾을 수 없습니다."));
		return;
	}
	CString strFileName;
	strFileName.Format(_T("rpt_tmp\\page%04d.xml"), nMaxTmpPageNum);
	// 현재 마지막 페이지 파일을 불러온다.
	ids = pPageWorkForm->LoadWorkForm(strFileName);
	if (ids != 0) {
		AfxMessageBox(_T("페이지를 로드 하지 못하였습니다."));
		return;
	}
	// 불러온 WorkForm중에서 First Sheet를 가져온다.(중간 페이지 파일에는 Sheet가 한개뿐이다.)
	CSIReportWorkSheet *pSheet = pPageWorkForm->FindSheet(0);
	if (pSheet == NULL) {
		AfxMessageBox(_T("페이지를 찾을 수 없습니다."));
		return;
	}
	// 마지막 수평줄 처리가 이미 되어 있는지 본다. 이미 처리되어 있다면 두번 할 필요 없다.
	CSIReportObject * pObject = pSheet->FindObject(_T("추가된마지막수평줄"));
	if (pObject) return;
	// 외부테이블 과 반복 테이블을 가져온다.(테이블 이름은 상황에 따라 변경할 것)
	CSIReportObject_Table * pIn = (CSIReportObject_Table*)pSheet->FindObject(_T("테이블_17"));
	CSIReportObject_Table * pOut = (CSIReportObject_Table*)pSheet->FindObject(_T("테이블_43"));
	if (pIn == NULL) return;
	if (pOut == NULL) return;

	CRect InLeft, InRight, OutLeft, OutRight, NewRect;

	CSIReportObject_Table_Cell *pCell;

	// 반복테이블의 마지막 라인의 왼쪽 처음과 오른쪽 마지막셀의 Rect를 구하고
	pCell = NULL;
	pCell = pIn->GetCell(pIn->row_count-1, 0);
	if (pCell) InLeft = pCell->GetRect();
	
	pCell = NULL;
	pCell = pIn->GetCell(pIn->row_count-1, pIn->col_count-1);
	if (pCell) InRight = pCell->GetRect();

	// 외부테이블의 마지막 라인의 왼쪽 처음 셀의 Rect를 구한다.
	pCell = NULL;
	pCell = pOut->GetCell(pOut->row_count-1, 0);
	if (pCell) OutLeft = pCell->GetRect();

	// 외부테이블보다 반복테이블이 작으면 마지막 수평줄을 추가해준다.
	if (InLeft.bottom < OutLeft.bottom) {
		// 마지막 수평줄의 Rect를 계산한다.
		NewRect.top = InLeft.bottom;
		NewRect.bottom = InLeft.bottom;
		NewRect.left = InLeft.left;
		NewRect.right = InRight.right;

		// 마지막 수평줄을 중간 페이지 파일에 추가한다.
		CSIReportObject_Line *pLine = (CSIReportObject_Line *)pSheet->AddNewObject(OBJECT_LINE);
		if (pLine) {
			pLine->SetName(_T("추가된마지막수평줄"));
			pLine->SetRect(NewRect);
		}

		// 변경된 중간페이지 파일을 저장한다.
		ids = pXMLIOMgr->SaveToXML(pPageWorkForm, strFileName);
	}

	// 생성된 Object를 해제
	if (pXMLIOMgr) {
		delete pXMLIOMgr;
		pXMLIOMgr = NULL;
	}
	if (pPageWorkForm) {
		delete pPageWorkForm;
		pPageWorkForm = NULL;
	}
	return;

EFS_END
}

VOID CBO_CAT_REG_0800::OnrB4() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_ChkSerial)->EnableWindow(true);
}

VOID CBO_CAT_REG_0800::OnrA4() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_ChkSerial)->EnableWindow(false);
}

VOID CBO_CAT_REG_0800::OnrA4RANDSCAPE() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_ChkSerial)->EnableWindow(false);
}

BOOL CBO_CAT_REG_0800::Print_JangSeo()
{

	CESL_DataMgr *pDM;
	pDM = FindDM ( _T("DM_장서원부") );
	if ( pDM == NULL ) 
	{
		AfxMessageBox( _T("장서원부 DM 초기화 에러") );
		return FALSE;
	}

	//데이터를 넣은것을 SIREPORT에 출력한다.
	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	if (!pSIReportManager) {
		AfxMessageBox(_T("Report Manager를 생성하지 못하였습니다."));
		return 1;
	}
	pSIReportManager->SetCONNECTION_INFO(pDM->CONNECTION_INFO);
	pSIReportManager->DeleteTempPageFiles();

	INT ids = pSIReportManager->GetSIReportFileFromDB(_T("장서원부"));
	if (ids < 0) {
		AfxMessageBox(_T("DB로 부터 SI Report 정보를 가져오지 못하였습니다."));
		return 1;
	}

	//5163일 경우 장서원부 출력로직
	CESL_DataMgr dm;
	CButton * pAccNo = (CButton *)GetDlgItem(IDC_rSHELF_PRINT);
	if ( pAccNo->GetCheck() == 1 ) 
	{
		//원부번호로 검색
		CString sYear, sAccessionNo;
		GetControlData( _T("CM_BO_CAT_REG_0800"), _T("작성년도"), sYear );
		if ( sYear == _T("") ) {
			AfxMessageBox( _T("작성년도를 입력하세요.") );
			return FALSE;
		}
		GetControlData( _T("CM_BO_CAT_REG_0800"), _T("원부번호"), sAccessionNo );
		if ( sAccessionNo == _T("") ) {
			AfxMessageBox( _T("원부번호를 입력하세요.") );
			return FALSE;
		}

		CESL_DataMgr *pDmAccession;
		pDmAccession = FindDM( _T("DM_BO_CAT_REG_0800_원부") );
		if ( pDmAccession == NULL ) {
			AfxMessageBox( _T("DM_BO_CAT_REG_0800_원부 error") );
			return FALSE;
		}
		CString sAccQuery;
		sAccQuery.Format( _T("ACCESSION_REC_NO = '%s' AND ACCESSION_REC_MAKE_YEAR = '%s'"), sAccessionNo, sYear );
		if ( pDmAccession->RefreshDataManager( sAccQuery ) < 0 ) return FALSE;
		m_pInfo->pCodeMgr->ConvertCodeToDescInDM(pDmAccession, _T("입수구분"), _T("원부입수구분"));	
		INT iAccCount = pDmAccession->GetRowCount();

		CString strQuery;
		//strQuery.Format(_T("SELECT B.INPUT_DATE, B.REG_NO, S.MARC, B.PRICE, B.CLASS_NO, B.BOOK_CODE, B.VOL_CODE, B.COPY_CODE, B.SEPARATE_SHELF_CODE FROM BO_BOOK_TBL B, BO_SPECIES_TBL S WHERE B.ACCESSION_REC_KEY IN(SELECT REC_KEY FROM CO_ACCESSION_REC_TBL WHERE S.REC_KEY = B.SPECIES_KEY AND ACCESSION_REC_NO =%s AND ACCESSION_REC_MAKE_YEAR ='%s')")
		//				, sAccessionNo, sYear );
		strQuery.Format(_T("SELECT B.INPUT_DATE, B.REG_NO, S.MARC, B.PRICE, ")
						_T("B.CLASS_NO, B.BOOK_CODE, B.VOL_CODE, B.COPY_CODE, ")
						_T("B.SEPARATE_SHELF_CODE, B.BOOK_APPENDIX_FLAG, ")
						_T("B.VOL_TITLE, B.AUTHOR, B.VOL, B.PUBLISH_YEAR ")
						_T("FROM BO_BOOK_TBL B, BO_SPECIES_TBL S ")
						_T("WHERE B.ACCESSION_REC_KEY IN(SELECT REC_KEY FROM CO_ACCESSION_REC_TBL WHERE S.REC_KEY = B.SPECIES_KEY AND ACCESSION_REC_NO =%s AND ACCESSION_REC_MAKE_YEAR ='%s') ")
						_T("AND B.MANAGE_CODE = UDF_MANAGE_CODE ")
						_T("ORDER BY B.REG_NO")
						, sAccessionNo, sYear );

		dm.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
		dm.RestructDataManager( strQuery );

		//SIREPORT 변수 
		CString strTmp;
		strTmp.Format(_T(" 원부번호 : %s 년도 %s 원부"), sYear, sAccessionNo );
		pSIReportManager->SetTempVariable(_T("검색조건"), strTmp);
	}


	//=====================================================
	//2008.11.04 ADD BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
	INT nRegCodeLength;
	nRegCodeLength = m_pInfo->nREG_CODE_LENGTH;
	//=====================================================

	CButton * pRegNo = (CButton *)GetDlgItem(IDC_rACCESSIONNO_PRINT);	
	if ( pRegNo->GetCheck() == 1 )
	{
		//등록번호로 검색
		CString sRegNoStart, sRegNoEnd;
		GetControlData( _T("CM_BO_CAT_REG_0800"), _T("등록번호부터"), sRegNoStart );
		GetControlData( _T("CM_BO_CAT_REG_0800"), _T("등록번호까지"), sRegNoEnd );	

		if ( sRegNoStart == _T("") || sRegNoEnd == _T("") ) {
			AfxMessageBox( _T("등록번호 범위를 입력하세요.") );
			return FALSE;
		}
		//=====================================================
		//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 		if ( sRegNoStart != _T("") && sRegNoStart.GetLength() < 12 ) {		
// 			if ( sRegNoStart.GetLength() > 2 ) {
// 				CString sRight;
// 				sRight.Format( _T("%010s") , sRegNoStart.Mid(2) );
// 				sRegNoStart = sRegNoStart.Left(2) + sRight;
// 			}
// 		}
// 		if ( sRegNoEnd != _T("") && sRegNoEnd.GetLength() < 12 ) {
// 			if ( sRegNoEnd.GetLength() > 2 ) {
// 				CString sRight;
// 				sRight.Format( _T("%010s") , sRegNoEnd.Mid(2) );
// 				sRegNoEnd = sRegNoEnd.Left(2) + sRight;
// 			}
// 		}

		if ( sRegNoStart != _T("") && sRegNoStart.GetLength() < 12 ) {		
			if ( sRegNoStart.GetLength() > nRegCodeLength ) {
				sRegNoStart = m_pInfo->MakeRegNo(sRegNoStart.Left(nRegCodeLength),sRegNoStart.Mid(nRegCodeLength));
			}
		}
		if ( sRegNoEnd != _T("") && sRegNoEnd.GetLength() < 12 ) {
			if ( sRegNoEnd.GetLength() > nRegCodeLength ) {
				sRegNoEnd = m_pInfo->MakeRegNo(sRegNoEnd.Left(nRegCodeLength),sRegNoEnd.Mid(nRegCodeLength));
			}
		}
		//=====================================================
		

		if ( sRegNoStart.Left(2) != sRegNoEnd.Left(2) ) {
			AfxMessageBox( _T("등록구분이 같아야 합니다.") );
			return FALSE;
		}

		CString strQuery;
		//strQuery.Format( _T("SELECT B.INPUT_DATE, B.REG_NO, S.MARC, B.PRICE, B.CLASS_NO, B.BOOK_CODE, B.VOL_CODE, B.COPY_CODE, B.SEPARATE_SHELF_CODE FROM BO_BOOK_TBL B, BO_SPECIES_TBL S")
		//	_T(" WHERE B.SPECIES_KEY = S.REC_KEY AND B.REG_NO >= '%s' AND B.REG_NO <= '%s' "), sRegNoStart, sRegNoEnd );

		strQuery.Format( _T("SELECT B.INPUT_DATE, B.REG_NO, S.MARC, B.PRICE, ")
							_T("B.CLASS_NO, B.BOOK_CODE, B.VOL_CODE, B.COPY_CODE, ")
							_T("B.SEPARATE_SHELF_CODE, B.BOOK_APPENDIX_FLAG, ")
							_T("B.VOL_TITLE, B.AUTHOR, B.VOL, B.PUBLISH_YEAR ")
							_T("FROM BO_BOOK_TBL B, BO_SPECIES_TBL S")
							_T(" WHERE B.SPECIES_KEY = S.REC_KEY AND B.REG_NO >= '%s' AND B.REG_NO <= '%s' ")
							_T("AND B.MANAGE_CODE = UDF_MANAGE_COD E")
							_T("ORDER BY B.REG_NO "), sRegNoStart, sRegNoEnd );

		//검색 조건에 맞는 데이터를 가져온다.
		dm.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
		dm.RestructDataManager( strQuery );

		//SIREPORT 변수 
		CString strTmp;
		strTmp.Format(_T("등록번호 :  %s - %s "), sRegNoStart, sRegNoEnd );
		pSIReportManager->SetTempVariable(_T("검색조건"), strTmp);

	}


	//출력용 DM에 저장
	pDM->FreeData();

	CString strTmpItem;
	CString strInputDate;
	CString strRegNo;
	CString strStreamMarc;
	CString strPrice;
	CString strClassNo;
	CString strBookCode;
	CString strVolCode;
	CString strCopyCode;
	CString strSeparateShelfCode;
	CString strVolTitle;
	CString strAuthor;
	CString strVol;
	CString strPublishYear;
	CString strBookAppendixFlag;


	INT nPageNum = 1;
	INT nSumPrice = 0;
	INT nPrintPerCount = 0;

	for ( INT idx = 0; idx < dm.GetRowCount() ; idx++ ) 
	{
		nPrintPerCount++;
		//0 입수일자
		//1 등록번호
		//2 마크
		//3 가격
		strInputDate		= dm.GetCellData( idx, 0 );
		strRegNo			= dm.GetCellData( idx, 1 );
		strStreamMarc		= dm.GetCellData( idx, 2 );
		strPrice			= dm.GetCellData( idx, 3 );
		strClassNo			= dm.GetCellData( idx, 4 );
		strBookCode			= dm.GetCellData( idx, 5 );
		strVolCode			= dm.GetCellData( idx, 6 );
		strCopyCode			= dm.GetCellData( idx, 7 );
		strSeparateShelfCode= dm.GetCellData( idx, 8 );
		strBookAppendixFlag = dm.GetCellData( idx, 9 );
		strVolTitle			= dm.GetCellData( idx, 10 );
		strAuthor			= dm.GetCellData( idx, 11 );
		strVol				= dm.GetCellData( idx, 12 );
		strPublishYear		= dm.GetCellData( idx, 13 );
		
		ConversionPriceInfoToDesc ( strPrice, strPrice );

		pDM->InsertRow(-1);
		//입수일자 및 등록번호
		strTmpItem = strInputDate + _T("\r\n") + strRegNo ;
		pDM->SetCellData(_T("입수일자등록번호"), strTmpItem , idx );

		// 서지사항
		CMarc marc;
		CString strMarc245Field;
		CString strMarc260Field;
		CString strMarc300Field;
		m_pInfo->pMarcMgr->Decoding( strStreamMarc, &marc );
		m_pInfo->pMarcMgr->GetField( &marc, _T("245"), strMarc245Field );
		m_pInfo->pMarcMgr->GetField( &marc, _T("260"), strMarc260Field );
		m_pInfo->pMarcMgr->GetField( &marc, _T("300"), strMarc300Field );

		strMarc245Field.Replace(SUBFIELD_CODE, '$');
		strMarc260Field.Replace(SUBFIELD_CODE, '$');
		strMarc300Field.Replace(SUBFIELD_CODE, '$');

		if ( strBookAppendixFlag==_T("A") )		
		{
			strTmpItem.Empty();
			// 2005.08.01 부록명 출력시 서지사항을 부록정보로 구성
			if ( !strVolTitle.IsEmpty() )
			{
				strTmpItem+=_T("[부록] ")+strVolTitle;
			}
			if ( !strAuthor.IsEmpty() )
			{
				strTmpItem+=_T(" /")+strAuthor;
			}
			if ( !strVol.IsEmpty() )
			{
				strTmpItem+=_T(" .")+strVol;
			}
			if ( !strPublishYear.IsEmpty() )
			{
				strTmpItem+=_T(" ")+strPublishYear;
			}						
		}
		else
		{
			if ( !strMarc245Field.IsEmpty() )
			{
				strMarc245Field = strMarc245Field.Mid(2);
				strMarc245Field.Replace(_T("$a"), _T(" "));
				strMarc245Field.Replace(_T("$d"), _T(" "));
				strMarc245Field.Replace(_T("$e"), _T(" "));
				strMarc245Field.Replace(_T("$b"), _T(" "));
				strMarc245Field.Replace(_T("$n"), _T(" "));
				strMarc245Field.Replace(_T("$p"), _T(" "));
				strMarc245Field.Replace(_T("$x"), _T(" "));
				strMarc245Field.Replace(_T("$c"), _T(" "));
				strMarc245Field.Replace(_T("$h"), _T(" "));
				strMarc245Field.Replace(_T("$6"), _T(" "));
			}
			
			if ( !strMarc260Field.IsEmpty() )
			{
				strMarc260Field = strMarc260Field.Mid(2);
				strMarc260Field.Replace(_T("$a"), _T(" "));
				strMarc260Field.Replace(_T("$b"), _T(" "));
				strMarc260Field.Replace(_T("$c"), _T(" "));
				strMarc260Field.Replace(_T("$d"), _T(" "));
				strMarc260Field.Replace(_T("$e"), _T(" "));
				strMarc260Field.Replace(_T("$f"), _T(" "));
				strMarc260Field.Replace(_T("$g"), _T(" "));
			}
			if ( !strMarc300Field.IsEmpty() )
			{
				strMarc300Field = strMarc300Field.Mid(2);
				strMarc300Field.Replace(_T("$a"), _T(" "));
				strMarc300Field.Replace(_T("$b"), _T(" "));
				strMarc300Field.Replace(_T("$c"), _T(" "));
				strMarc300Field.Replace(_T("$d"), _T(" "));
				strMarc300Field.Replace(_T("$e"), _T(" "));
			}
			
			strMarc245Field.Replace( FIELD_TERMINATOR , ' ');
			strMarc260Field.Replace( FIELD_TERMINATOR , ' ');
			strMarc300Field.Replace( FIELD_TERMINATOR , ' ');
			
			strTmpItem.Empty();
			strTmpItem = strMarc245Field + _T(" ") + strMarc260Field + _T(" ") + strMarc300Field;
		}

		pDM->SetCellData(_T("서지사항"), strTmpItem , idx );

		//가격
		pDM->SetCellData(_T("가격"), strPrice , idx );

		//청구기호 (분류기호한줄, 090$b한줄)
		strTmpItem.Empty();

		//별치는 설명으로
		if ( !strSeparateShelfCode.IsEmpty() ) 
			m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("별치기호"), strSeparateShelfCode, strSeparateShelfCode);

		strTmpItem = strTmpItem + strSeparateShelfCode ;	
		
		strTmpItem = strTmpItem + _T("\r\n") + strClassNo ;
		strTmpItem = strTmpItem + _T("\r\n") + strBookCode ;

		// 2005.04.11 ADD BY PDJ
		// 청구기호 출력시 권책기호 부분 추가함.
		if ( strVolCode.IsEmpty() )
		{
			if ( !strCopyCode.IsEmpty() ) 
			strTmpItem = strTmpItem + _T("\r\nc.") + strCopyCode ;			
		}
		else
		{
			strTmpItem = strTmpItem + _T("\r\n") + strVolCode ;
			if ( !strCopyCode.IsEmpty() ) 
			strTmpItem = strTmpItem + _T("=c.") + strCopyCode ;
		}
		
		

		pDM->SetCellData(_T("청구기호"), strTmpItem , idx );

		strPrice.Replace( _T(","), _T(""));
		nSumPrice = nSumPrice + _ttoi(strPrice);

		//페이지당 가격 합산 추가
		if ( nPrintPerCount == 15 )
		{
			CString strTmpData, strTmpSumPrice;
			strTmpData.Format(_T("계%d"), nPageNum);
			strTmpSumPrice.Format(_T("%d"), nSumPrice);
			ConversionPriceInfoToDesc(strTmpSumPrice, strTmpSumPrice);
			pSIReportManager->SetTempVariable(strTmpData , strTmpSumPrice );
			nSumPrice = 0;
			nPageNum++;
			nPrintPerCount = 0;
		}

	}

	//최종으로 가격 추가
	if ( nPrintPerCount != 1 ) 
	{
		CString strTmpData, strTmpSumPrice;
		strTmpData.Format(_T("계%d"), nPageNum);
		strTmpSumPrice.Format(_T("%d"), nSumPrice);
		ConversionPriceInfoToDesc(strTmpSumPrice, strTmpSumPrice);
		pSIReportManager->SetTempVariable(strTmpData , strTmpSumPrice );
	} 

	
	pSIReportManager->SetDataMgr(0, pDM );
	pSIReportManager->MAKEPAGE_DrawWorkForm(-1);		// 워크폼 Manual 반복 모드

	Print_SetLastPageLine();

	ids = pSIReportManager->Print2();
	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}	

	return TRUE;
}


INT CBO_CAT_REG_0800::ConversionPriceInfoToDesc(CString strSrcData, CString &strDesData)
{
	CString strTmpData, strData1, strData2;
	CStringArray arrTmpData;	arrTmpData.RemoveAll();
	INT nLength = 0;
	INT nPos = -1;
	// 2005-01-13 소수점 이하는 , 표시를 하지 않도록 하기 위해 소수점 앞자리만 넘겨준다.
	nPos = strSrcData.Find(_T("."));
	if( nPos < 0 ) // _T(".")이 없는 경우
	{
		strData1 = strSrcData;
		strData2.Empty();
	}
	else
	{
		strData1 = strSrcData.Left(nPos);
		strData2 = strSrcData.Mid(nPos);
	}

	strData1.TrimLeft();		strData1.TrimRight();
	while(!strData1.IsEmpty())
	{
		nLength = strData1.GetLength();
		if(nLength >= 0)
			strTmpData = strData1.Right(3);
		else
			strTmpData = strData1;
		arrTmpData.InsertAt(0, strTmpData);
		if(nLength-3 >= 0)
			strData1 = strData1.Mid(0, nLength-3);
		else 
			strData1.Empty();
	}

	strDesData.Empty();
	INT cnt = arrTmpData.GetSize();
	for(INT i=0 ; i<cnt ; i++)
	{
		if(strDesData.IsEmpty()) strDesData = arrTmpData.GetAt(i);
		else strDesData += _T(",") + arrTmpData.GetAt(i);
	}
	strDesData += strData2;

	return 0;
}
HBRUSH CBO_CAT_REG_0800::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}

