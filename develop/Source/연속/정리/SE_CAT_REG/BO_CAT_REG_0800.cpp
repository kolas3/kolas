// BO_CAT_REG_0800.cpp : implementation file
//

#include "stdafx.h"
//#include _T("bo_cat_reg.h")
#include "BO_CAT_REG_0800.h"
#include "..\..\..\ESL\SIReportMgr\SIReportManager.h"

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
	//{{AFX_DATA_INIT(CBO_CAT_REG_0800)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
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
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_REG_0800, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_REG_0800)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_REG_0800 message handlers

BOOL CBO_CAT_REG_0800::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("SE_CAT_REG_0800")) < 0)	
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	CButton * pPart;
	pPart = (CButton *)GetDlgItem(IDC_rSHELF_PRINT);
	pPart->SetCheck(1);

	//2005-11-11 황정영 추가
	CString str90;
	GetManageValue(_T("기타"), _T("공통"), _T("국방대"), _T("국방대"), str90);
	CButton * pA4;
	if( str90 == _T("Y"))
	{
		pA4 = (CButton *)GetDlgItem(IDC_rA4_RANDSCAPE);
		pA4->SetCheck(1);
	}
	else
	{
		pA4 = (CButton *)GetDlgItem(IDC_rA4);
		pA4->SetCheck(1);
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

VOID CBO_CAT_REG_0800::OnOK() 
{
EFS_BEGIN

	CButton * pPart;
	pPart = (CButton *)GetDlgItem(IDC_rSHELF_PRINT);
	CString strIS64, str90;
	GetManageValue( _T("기타") , _T("공통") , _T("육사") , _T("육사") , strIS64 );
	GetManageValue( _T("기타"), _T("공통"), _T("국방대"), _T("국방대"), str90 );

	CString sYear, sAccessionNo;	
	if ( pPart->GetCheck() == 1 ) {		
		GetControlData( _T("CM_SE_CAT_REG_0800"), _T("작성년도"), sYear );
		if ( sYear == _T("") ) {
			AfxMessageBox( _T("작성년도를 입력하세요.") );
			return;
		}
		GetControlData( _T("CM_SE_CAT_REG_0800"), _T("원부번호"), sAccessionNo );
		if ( sAccessionNo == _T("") ) {
			AfxMessageBox( _T("원부번호를 입력하세요.") );
			return;
		}
	

		//원부정보DM
		CESL_DataMgr *pDmAccession;
		pDmAccession = FindDM( _T("DM_SE_CAT_REG_0800_원부") );
		if ( pDmAccession == NULL ) {
			AfxMessageBox( _T("DM_BO_CAT_REG_0800_원부 error") );
			return;
		}
		CString sAccQuery;
		sAccQuery.Format( _T("ACCESSION_REC_NO = '%s' AND ACCESSION_REC_MAKE_YEAR = '%s'"), sAccessionNo, sYear );
		if ( pDmAccession->RefreshDataManager( sAccQuery ) < 0 ) return;
		m_pInfo->pCodeMgr->ConvertCodeToDescInDM(pDmAccession, _T("입수구분"), _T("원부입수구분"));	
		INT iAccCount = pDmAccession->GetRowCount();


		CESL_DataMgr *pDM;
		pDM = FindDM( _T("DM_SE_CAT_REG_0800_01") );
		if ( pDM == NULL ) {
			AfxMessageBox( _T("DM_BO_CAT_PROC_0800_01") );
			return;
		}

		CString sQuery;
		//sQuery.Format( _T("REG_NO >= '%s' AND REG_NO <= '%s'"), sRegNoStart, sRegNoEnd );
		sQuery.Format( _T("B.ACCESSION_REC_KEY IN(SELECT REC_KEY FROM CO_ACCESSION_REC_TBL WHERE ACCESSION_REC_NO =%s AND ACCESSION_REC_MAKE_YEAR='%s')"), sAccessionNo, sYear );
		if ( pDM->RefreshDataManager( sQuery ) < 0 ) return;

		INT iRowCount = pDM->GetRowCount();
		if ( iRowCount < 1 ) {
			AfxMessageBox( _T("검색된 데이터가 없습니다.") );
			return;
		}
		
		// 2005-11-10 황정영: 국방대 본관/분관별 종수/책수 
		CString strQuery, strSMA, strBMA, strSBR, strBBR, strSTOTAL, strBTOTAL;
		if (str90 == _T("Y"))
		{
//			INT nStotal, nBtotal;
			//본관 종수
			strQuery.Format(_T("SELECT COUNT(DISTINCT(SPECIES_KEY)) FROM SE_BOOK_TBL B WHERE "));
			strQuery += sQuery;
			strQuery += _T(" AND MANAGE_CODE = 'MA'");
			pDM->GetOneValueQuery(strQuery, strSMA);
			
			//본관 책수
			strQuery.Format(_T("SELECT COUNT(DISTINCT(REG_NO)) FROM SE_BOOK_TBL B WHERE "));
			strQuery += sQuery;
			strQuery += _T(" AND MANAGE_CODE = 'MA'");
			pDM->GetOneValueQuery(strQuery, strBMA);
			
			//분관 종수
			strQuery.Format(_T("SELECT COUNT(DISTINCT(SPECIES_KEY)) FROM SE_BOOK_TBL B WHERE "));
			strQuery += sQuery;
			strQuery += _T(" AND MANAGE_CODE = 'BR'");
			pDM->GetOneValueQuery(strQuery, strSBR);

			//분관 책수
			strQuery.Format(_T("SELECT COUNT(DISTINCT(REG_NO)) FROM SE_BOOK_TBL B WHERE "));
			strQuery += sQuery;
			strQuery += _T(" AND MANAGE_CODE = 'BR'");
			pDM->GetOneValueQuery(strQuery, strBBR);

			//합계 종수
			strQuery.Format(_T("SELECT COUNT(DISTINCT(SPECIES_KEY)) FROM SE_BOOK_TBL B WHERE "));
			strQuery += sQuery;
			strQuery += _T(" AND MANAGE_CODE IN ('BR','MA') ");
			pDM->GetOneValueQuery(strQuery, strSTOTAL);
			
			//합계 책수
			strQuery.Format(_T("SELECT COUNT(DISTINCT(REG_NO)) FROM SE_BOOK_TBL B WHERE "));
			strQuery += sQuery;
			strQuery += _T(" AND MANAGE_CODE IN ('BR','MA') ");
			pDM->GetOneValueQuery(strQuery, strBTOTAL);

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
			
			//합계
/*			nStotal = _ttoi(strSMA) + _ttoi(strSBR);
			nBtotal = _ttoi(strBMA) + _ttoi(strBBR);
			
			strSTOTAL.Format(_T("%d"), nStotal);
			strBTOTAL.Format(_T("%d"), nBtotal);*/
		}

		CESL_DataMgr *pDM_0800_02;
		pDM_0800_02 = FindDM( _T("DM_SE_CAT_REG_0800_02") );
		if ( pDM_0800_02 == NULL ) {
			AfxMessageBox( _T("DM_BO_CAT_REG_0800_01") );
			return;
		}

		CString sRegCode, sRegCodeAll;
		CString sMinRegNo, sMaxRegNo;
		CString sRegCodeDes;

		//등록구분 합치기
		for ( INT i = 1 ; i <= iRowCount ; i++ ) {
			sRegCode = pDM->GetCellData( _T("BB_등록구분"), i-1 );
			if ( sRegCode != _T("") ) {
				if ( sRegCodeAll != _T("") ) {
					sRegCodeAll = sRegCodeAll + _T(", '") + sRegCode + _T("'");
				}
				else {
					sRegCodeAll = _T("'") + sRegCode + _T("'");
				}
			}
			/*
			//설명
			m_pInfo->pCodeMgr->ConvertCodeToDescInDM(pDM, _T("등록구분"), _T("BB_등록구분"));
			CString sRegCodeDesTmp = pDM->GetCellData( _T("BB_등록구분"), i-1 );
			if ( sRegCodeDes != _T("") ) {
				sRegCodeDes = sRegCodeDes + _T(", ") + sRegCodeDesTmp;
			}
			else {
				sRegCodeDes = sRegCodeDesTmp;
			}
			*/
		}		
		for ( i = 1 ; i <= iRowCount ; i++ ) {						
			//설명
			m_pInfo->pCodeMgr->ConvertCodeToDescInDM(pDM, _T("등록구분"), _T("BB_등록구분"));
			CString sRegCodeDesTmp = pDM->GetCellData( _T("BB_등록구분"), i-1 );
			if ( sRegCodeDes != _T("") ) {
				sRegCodeDes = sRegCodeDes + _T(", ") + sRegCodeDesTmp;
			}
			else {
				sRegCodeDes = sRegCodeDesTmp;
			}			
		}
		/*
		//등록구분별로
		for ( INT i = 1 ; i <= iRowCount ; i++ ) {			
			sRegCode = pDM->GetCellData( _T("BB_등록구분"), i-1 );
			sQuery.Format( _T("I.REC_KEY = B.SPECIES_KEY AND B.REG_CODE = '%s' AND ACCESSION_REC_KEY IN(SELECT REC_KEY FROM CO_ACCESSION_REC_TBL WHERE ACCESSION_REC_NO =%s)"), sRegCode, sAccessionNo );
			if ( pDM_0800_02->RefreshDataManager( sQuery ) < 0 ) return;
			m_pInfo->pCodeMgr->ConvertCodeToDescInDM(pDM, _T("등록구분"), _T("BB_등록구분"));
			CString sRegCodeDesTmp = pDM->GetCellData( _T("BB_등록구분"), i-1 );
			if ( sRegCodeDes != _T("") ) {
				sRegCodeDes = sRegCodeDes + _T(", ") + sRegCodeDesTmp;
			}
			else {
				sRegCodeDes = sRegCodeDesTmp;
			}
			INT iRowCount_02 = pDM_0800_02->GetRowCount();		
			INT iSwReg = 0;
			for ( INT j = 0 ; j < iRowCount_02 ; j ++ ) {
				//형태사항 만들기
				CString sPage = pDM_0800_02->GetCellData( _T("BB_면장수"), j );
				CString sPhysical = pDM_0800_02->GetCellData( _T("BB_물리적특성"), j );
				CString sBookSize = pDM_0800_02->GetCellData( _T("BB_크기"), j );
				CString sForm = sPage + _T(" : ") + sPhysical + _T(" ; ") + sBookSize;
				pDM_0800_02->SetCellData( _T("형태사항"), sForm, j );
				//청구기호 만들기
				CString sShelfCode = pDM_0800_02->GetCellData( _T("IBS_청구_별치기호_코드"), j );
				CString sClassNo = pDM_0800_02->GetCellData( _T("IBS_청구_분류기호"), j );
				CString sBookCode = pDM_0800_02->GetCellData( _T("IBS_청구_도서기호"), j );
				CString sVolCode = pDM_0800_02->GetCellData( _T("IBS_청구_권책기호"), j );
				CString sCallNo = sShelfCode + _T(" ") + sClassNo + _T("-") + sBookCode + _T(" ") + sVolCode;
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
			}
		}
		*/
		//등록구분 상관없이
		sQuery.Format( _T("I.REC_KEY = S.REC_KEY AND I.REC_KEY = V.SPECIES_KEY AND V.REC_KEY = B.VOL_KEY AND B.BINDED_BOOK_YN = 'N' AND B.REG_CODE IN (%s) AND B.ACCESSION_REC_KEY IN(SELECT REC_KEY FROM CO_ACCESSION_REC_TBL WHERE ACCESSION_REC_NO =%s AND ACCESSION_REC_MAKE_YEAR='%s') ORDER BY B.REG_CODE, B.REG_NO"), sRegCodeAll, sAccessionNo, sYear );
		if ( pDM_0800_02->RefreshDataManager( sQuery ) < 0 ) return;
		/*
		m_pInfo->pCodeMgr->ConvertCodeToDescInDM(pDM, _T("등록구분"), _T("BB_등록구분"));
		CString sRegCodeDesTmp = pDM->GetCellData( _T("BB_등록구분"), i-1 );
		if ( sRegCodeDes != _T("") ) {
			sRegCodeDes = sRegCodeDes + _T(", ") + sRegCodeDesTmp;
		}
		else {
			sRegCodeDes = sRegCodeDesTmp;
		}
		*/		
		INT iRowCount_02 = pDM_0800_02->GetRowCount();		
		INT iSwReg = 0;
		for ( INT j = 0 ; j < iRowCount_02 ; j ++ ) {
			//형태사항 만들기
			CString sPage = pDM_0800_02->GetCellData( _T("BB_면장수"), j );
			CString sPhysical = pDM_0800_02->GetCellData( _T("BB_물리적특성"), j );
			CString sBookSize = pDM_0800_02->GetCellData( _T("BB_크기"), j );
			//CString sForm = sPage + _T(" : ") + sPhysical + _T(" ; ") + sBookSize;
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
			pDM_0800_02->SetCellData( _T("형태사항"), sForm, j );
			//청구기호 만들기
			CString sShelfCode = pDM_0800_02->GetCellData( _T("IBS_청구_별치기호_코드"), j );
			CString sClassNo = pDM_0800_02->GetCellData( _T("IBS_청구_분류기호"), j );
			CString sBookCode = pDM_0800_02->GetCellData( _T("IBS_청구_도서기호"), j );
			CString sVolCode = pDM_0800_02->GetCellData( _T("IBS_청구_권책기호"), j );
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
			CString s245n;
			m_pInfo->pMarcMgr->GetItem( &marc, _T("245$n"), s245n );
			if ( s245n != _T("") ) {
				sTitleInfo = sTitleInfo + _T(" . ") + s245n;
			}
			CString s245p;
			m_pInfo->pMarcMgr->GetItem( &marc, _T("245$p"), s245p );
			if ( s245p != _T("") ) {
				sTitleInfo = sTitleInfo + _T(" , ") + s245p;
			}
			CString s245d;
			m_pInfo->pMarcMgr->GetItem( &marc, _T("245$d"), s245d );
			if ( s245d != _T("") ) {
				sTitleInfo = sTitleInfo + _T(" / ") + s245d;
			}			
			CString s250a;
			m_pInfo->pMarcMgr->GetItem( &marc, _T("250$a"), s250a );
			if ( s250a != _T("") ) {
				sTitleInfo = sTitleInfo + _T(" .- ") + s250a;
			}
			//2006.01.24 hjy 수정 : 440 우선
/*			//총서표제,총서편차
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
			}*/
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

			//2005-11-10 황정영 : 국방대는 권호책기호를 뒤에 넣어준다.
			if( str90 == _T("Y"))
			{
				CString strVolCode, strOtherVolTitle;
				pDM_0800_02->GetCellData( _T("VB_권호명"), j, strVolCode );
				pDM_0800_02->GetCellData( _T("VB_부차적권호명"), j, strOtherVolTitle );
				if(!strVolCode.IsEmpty())
				{
					sTitleInfo += _T(" [");
					sTitleInfo += strVolCode;
					sTitleInfo += _T(" : ");
					sTitleInfo += strOtherVolTitle;
					sTitleInfo += _T(" ]");
				}

				//2005-12-01 황정영 : 제어번호추가
				CString strControlNo;
				strControlNo.Empty();
				m_pInfo->pMarcMgr->GetItem( &marc, _T("001*"), strControlNo);
				pDM_0800_02->SetCellData( _T("IBS_제어번호"),strControlNo, j);
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
			//현금 표시 포멧으로 만들기
			NUMBERFMT nFmt = { 0, 0, 3, _T("."), _T(","), 1 };
			TCHAR szOUT[20];
			::GetNumberFormat(NULL, NULL, sPrice, &nFmt, szOUT, 20);
			sPrice = _T("\\") + sPrice;
			pDM_0800_02->SetCellData( _T("BB_가격"), sPrice, j );

			// 2005-11-10 황정영 : 국방대 관리구분 설명값, 기증자 정보
			if(str90 == _T("Y"))
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
				strQuery.Format(_T("SELECT D.DONATOR_NAME ")
							  _T("FROM SE_DONATE_MANAGE_TBL D, SE_BOOK_TBL B ")
							  _T("WHERE B.MANAGE_CODE = UDF_MANAGE_CODE AND B.SPECIES_KEY = D.SPECIES_KEY ")
							  _T("AND B.SPECIES_KEY = '%s'")
							  , strTmp);
				pDM_0800_02->GetOneValueQuery(strQuery,strDonator);
				pDM_0800_02->SetCellData(_T("기증자"),strDonator,j);
			}

		}

		//출력하기
		CSIReportManager *pSIReportManager = new CSIReportManager(this);
		pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
		if (!pSIReportManager) {
			AfxMessageBox(_T("Report Manager를 생성하지 못하였습니다."));
			return;
		}
		INT ids;
		pSIReportManager->SetCONNECTION_INFO(pDM->CONNECTION_INFO);

		//A4,B4 선택
		CButton * pB4;
		pB4 = (CButton *)GetDlgItem(IDC_rB4);
		if ( pB4->GetCheck() == 1 ) {	
			if ( strIS64 == _T("Y") ) ids = pSIReportManager->GetSIReportFileFromDB(_T("SE원부출력B4_육사"));
			else if( str90 == _T("Y") ) ids = pSIReportManager->GetSIReportFileFromDB(_T("SE원부출력B4_국방대"));
			else ids = pSIReportManager->GetSIReportFileFromDB(_T("SE원부출력B4"));
		}
		else if (static_cast<CButton*>(GetDlgItem(IDC_rA4_RANDSCAPE))->GetCheck() == 1 ) 
		{
			if ( strIS64 == _T("Y") ) ids = pSIReportManager->GetSIReportFileFromDB(_T("SE원부출력_가로_육사"));
			else if( str90 == _T("Y") ) ids = pSIReportManager->GetSIReportFileFromDB(_T("SE원부출력_가로_국방대"));
			else ids = pSIReportManager->GetSIReportFileFromDB(_T("SE원부출력_가로"));
		}
		else {
			if ( strIS64 == _T("Y") ) ids = pSIReportManager->GetSIReportFileFromDB(_T("SE원부출력_육사"));
			else ids = pSIReportManager->GetSIReportFileFromDB(_T("SE원부출력"));
		}
		
		if (ids < 0) {
			AfxMessageBox(_T("DB로 부터 SI Report 정보를 가져오지 못하였습니다."));
			return;
		}
		
		//pSIReportManager->SetTempVariable(_T("%BARCODE바코드제목%"), _T("강남(이씨오)도서관"));
		CString sAccAcqCode = pDmAccession->GetCellData( _T("원부입수구분"), 0 );
		CString sPrice = pDM->GetCellData( _T("가격"), 0 );
		CString sRegNo = sMinRegNo + _T(" 부터  ") + sMaxRegNo + _T(" 까지");
		//책수 구하기		
		CString sBookCount = pDM->GetCellData( _T("책수"), 0 );
		INT iBookCount = pDM->GetRowCount();
		INT iTotBookCount = 0;
		for ( i = 0 ; i < iBookCount ; i ++ ) {
			sBookCount = pDM->GetCellData( _T("책수"), i );
			INT iBookCount = _ttoi( sBookCount );
			iTotBookCount = iTotBookCount + iBookCount;
		}
		sBookCount.Format( _T("%d"), iTotBookCount );

		CString sRegDate = pDmAccession->GetCellData( _T("원부등록일자"), 0 );
		pSIReportManager->SetTempVariable(_T("수입구분"), sAccAcqCode);
		pSIReportManager->SetTempVariable(_T("수입년도"), sYear);
		//가격
		//현금 표시 포멧으로 만들기
		NUMBERFMT nFmt = { 0, 0, 3, _T("."), _T(","), 1 };
		TCHAR szOUT[20];
		::GetNumberFormat(NULL, NULL, sPrice, &nFmt, szOUT, 20);
		CString sPriceFont;		
		sPriceFont.Format( _T("%15s"), szOUT );
		pSIReportManager->SetTempVariable(_T("가격"), sPriceFont);
		pSIReportManager->SetTempVariable(_T("등록구분"), sRegCodeDes);
		pSIReportManager->SetTempVariable(_T("원부번호"), sAccessionNo);
		pSIReportManager->SetTempVariable(_T("등록번호"), sRegNo);
		
		//2005-11-10 황정영 : 갑지/을지 수정
		if(str90 == _T("Y"))
		{
			pSIReportManager->SetTempVariable(_T("종수"), strSMA);
			pSIReportManager->SetTempVariable(_T("책수"), strBMA);
			pSIReportManager->SetTempVariable(_T("분관종수"), strSBR);
			pSIReportManager->SetTempVariable(_T("분관책수"), strBBR);
			pSIReportManager->SetTempVariable(_T("종합계"), strSTOTAL);
			pSIReportManager->SetTempVariable(_T("책합계"), strBTOTAL);
		}

		//2006-01-10 황정영 수정 
		//원부출력물에 직명,성명 나오게 설정.
		CString sClassName,sRegistrant ;
        strQuery.Format (_T("SELECT CLASS_NAME FROM CO_ACCESSION_REC_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND  ACCESSION_REC_NO = %s AND ACCESSION_REC_MAKE_YEAR = '%s'"), sAccessionNo, sYear);
        pDmAccession->GetOneValueQuery (strQuery,sClassName);
        strQuery.Format (_T("SELECT REGISTRANT FROM CO_ACCESSION_REC_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND  ACCESSION_REC_NO = %s AND ACCESSION_REC_MAKE_YEAR = '%s'"),sAccessionNo, sYear );
		pDmAccession->GetOneValueQuery (strQuery,sRegistrant); 
        
		pSIReportManager->SetTempVariable(_T("직명"), sClassName);
		pSIReportManager->SetTempVariable(_T("성명"), sRegistrant);

		//책수 줄 맞추기
		CString sBookFont;
		sBookFont.Format( _T("%15s"), sBookCount );
		pSIReportManager->SetTempVariable(_T("책수"), sBookFont);		
		pSIReportManager->SetTempVariable(_T("등록일자"), sRegDate);

		// 2005.08.01 ADD BY PDJ
		// 비고란 추가됨
		CString sRemark = pDmAccession->GetCellData( _T("비고"), 0 );
		pSIReportManager->SetTempVariable(_T("비고"), sRemark);
		//도서관명 가져오기
		/*
		CString sPath = _T("..\\CFG\\LocalLibraryInfo.cfg");
		FILE *fp;
		fp = _tfopen( sPath , _T("rb") );
		//fp = _tfopen( sFilePath , _T("rb") );
		
		if ( fp == NULL ) {
			AfxMessageBox( _T("..\\CFG\\LocalLibraryInfo.cfg 파일이 없습니다.") );
			return;
		}
		TCHAR cLine[256];
		CString sLibraryName;
		while( _fgetts( cLine, 256, fp ) ) {
			CString sLine;
			sLine.Format( _T("%s"), cLine );
			if ( sLine != _T("") ) {
				CString sTmp = sLine.Left(10);
				if ( sLine.Left(10) == _T("도서관이름") ) {
					INT iPos = sLine.Find(_T("="));
					sLibraryName = sLine.Mid( iPos+2 );
					sLibraryName = sLibraryName.Left( sLibraryName.GetLength() - 2 );
				}
			}
		}
		*/
		CString sLibraryName;
		//===================================================
		//2010.09.03 UPDATE BY PJW : MANAGE_CODE로 도서관명을 가져온다.
		//sQuery = _T("SELECT VALUE FROM MN_LIBINFO_TBL WHERE VALUE_NAME ='도서관명'");
		sQuery = _T("SELECT LIB_NAME FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE");
		//===================================================
		
		pDM_0800_02->GetOneValueQuery( sQuery , sLibraryName );
		//SetControlData( _T("CM_BO_CAT_PROC_0203_01"), _T("도서관명"), sLibraryName );
		sLibraryName = sLibraryName + _T("의 도서관자료로 아래와 같이 등록함.");

		pSIReportManager->SetTempVariable(_T("LIB_NAME"), sLibraryName);

	/*	
		m_pDM->InitDataMgrRef(1);
		m_pDM->SetDataMgrRef(0, _T("BB_등록번호"), _T("REG_NO"), _T("str"), _T(""), _T(""));
		m_pDM->TBL_NAME = _T("BO_BOOK_TBL");

		ids = m_pDM->RefreshDataManager(_T("rownum <= 50"));
		CString str;
		m_pDM->GetCellData(_T("BB_등록번호"), 0, str);
		INT nRowCount = m_pDM->GetRowCount();
	*/
		/*
		pSIReportManager->SetDataMgr(0, pDM_0800_02);
		ids = pSIReportManager->Print();
		if (pSIReportManager) {
			delete pSIReportManager;
			pSIReportManager = NULL;
		}
		*/

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

	//등록번호범위 출력
	CButton * pACCESSIONNO_PRINT;
	//=====================================================
	//2008.11.04 ADD BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
	INT nRegCodeLength;
	nRegCodeLength = pParentMgr->m_pInfo->nREG_CODE_LENGTH;
	//=====================================================
	pACCESSIONNO_PRINT = (CButton *)GetDlgItem(IDC_rACCESSIONNO_PRINT);	
	if ( pACCESSIONNO_PRINT->GetCheck() == 1 ) {
		CString sRegNoStart, sRegNoEnd;		
		GetControlData( _T("CM_SE_CAT_REG_0800"), _T("등록번호부터"), sRegNoStart );
		GetControlData( _T("CM_SE_CAT_REG_0800"), _T("등록번호까지"), sRegNoEnd );	

		if ( sRegNoStart == _T("") || sRegNoEnd == _T("") ) {
			AfxMessageBox( _T("등록번호 범위를 입력하세요.") );
			return;
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
// 
// 		if ( sRegNoStart.Left(2) != sRegNoEnd.Left(2) ) {
// 			AfxMessageBox( _T("등록구분이 같아야 합니다.") );
// 			return;
// 		}
		//nRegCodeLength = m_pParent->m_pInfo->nREG_CODE_LENGTH;
		if ( sRegNoStart != _T("") && sRegNoStart.GetLength() < 12 ) {		
			if ( sRegNoStart.GetLength() > nRegCodeLength ) {
				sRegNoStart = pParentMgr->m_pInfo->MakeRegNo(sRegNoStart.Left(nRegCodeLength),sRegNoStart.Mid(nRegCodeLength));
			}
		}
		if ( sRegNoEnd != _T("") && sRegNoEnd.GetLength() < 12 ) {
			if ( sRegNoEnd.GetLength() > nRegCodeLength ) {
				sRegNoEnd = pParentMgr->m_pInfo->MakeRegNo(sRegNoEnd.Left(nRegCodeLength),sRegNoEnd.Mid(nRegCodeLength));
			}
		}

		if ( sRegNoStart.Left(nRegCodeLength) != sRegNoEnd.Left(nRegCodeLength) ) {
			AfxMessageBox( _T("등록구분이 같아야 합니다.") );
			return;
		}
		//=====================================================

		

		CESL_DataMgr *pDM;
		pDM = FindDM( _T("DM_SE_CAT_REG_0800_01") );
		if ( pDM == NULL ) {
			AfxMessageBox( _T("DM_SE_CAT_PROC_0800_01") );
			return;
		}

		CString sQuery;
		//=====================================================
		//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 		sQuery.Format( _T("REG_NO >= '%s' AND REG_NO <= '%s' AND REG_CODE = '%s'"), sRegNoStart, sRegNoEnd, sRegNoStart.Left(2) );

		sQuery.Format( _T("REG_NO >= '%s' AND REG_NO <= '%s' AND REG_CODE = '%s'"), sRegNoStart, sRegNoEnd, sRegNoStart.Left(nRegCodeLength) );
		//=====================================================
		
		if ( pDM->RefreshDataManager( sQuery ) < 0 ) return;

		INT iRowCount = pDM->GetRowCount();
		if ( iRowCount < 1 ) {
			AfxMessageBox( _T("검색된 데이터가 없습니다.") );
			return;
		}
		
		//2005-11-10 황정영 추가
		CString strQuery, strSMA, strBMA, strSBR, strBBR, strSTOTAL, strBTOTAL;
		if(str90 == _T("Y"))
		{
//			INT nStotal, nBtotal;

			//본관 종수
			strQuery.Format(_T("SELECT COUNT(DISTINCT(SPECIES_KEY)) FROM SE_BOOK_TBL WHERE "));
			strQuery += sQuery;
			strQuery += _T(" AND MANAGE_CODE = 'MA'");
			pDM->GetOneValueQuery(strQuery, strSMA);
			
			//본관 책수
			strQuery.Format(_T("SELECT COUNT(REG_NO) FROM SE_BOOK_TBL WHERE "));
			strQuery += sQuery;
			strQuery += _T(" AND MANAGE_CODE = 'MA'");
			pDM->GetOneValueQuery(strQuery, strBMA);
			
			//분관 종수
			strQuery.Format(_T("SELECT COUNT(DISTINCT(SPECIES_KEY)) FROM SE_BOOK_TBL WHERE "));
			strQuery += sQuery;
			strQuery += _T(" AND MANAGE_CODE = 'BR'");
			pDM->GetOneValueQuery(strQuery, strSBR);

			//분관 책수
			strQuery.Format(_T("SELECT COUNT(REG_NO) FROM SE_BOOK_TBL WHERE "));
			strQuery += sQuery;
			strQuery += _T(" AND MANAGE_CODE = 'BR'");
			pDM->GetOneValueQuery(strQuery, strBBR);

			//합계 종수
			strQuery.Format(_T("SELECT COUNT(DISTINCT(SPECIES_KEY)) FROM SE_BOOK_TBL WHERE "));
			strQuery += sQuery;
			strQuery += _T(" AND MANAGE_CODE IN ('BR','MA') ");
			pDM->GetOneValueQuery(strQuery, strSTOTAL);
			
			//합계 책수
			strQuery.Format(_T("SELECT COUNT(DISTINCT(REG_NO)) FROM SE_BOOK_TBL WHERE "));
			strQuery += sQuery;
			strQuery += _T(" AND MANAGE_CODE IN ('BR','MA') ");
			pDM->GetOneValueQuery(strQuery, strBTOTAL);

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
			
			//합계
/*			nStotal = _ttoi(strSMA) + _ttoi(strSBR);
			nBtotal = _ttoi(strBMA) + _ttoi(strBBR);
			
			strSTOTAL.Format(_T("%d"), nStotal);
			strBTOTAL.Format(_T("%d"), nBtotal);*/
		}

		CESL_DataMgr *pDM_0800_02;
		pDM_0800_02 = FindDM( _T("DM_SE_CAT_REG_0800_02") );
		if ( pDM_0800_02 == NULL ) {
			AfxMessageBox( _T("DM_SE_CAT_REG_0800_02") );
			return;
		}

		CString sRegCode;
		CString sMinRegNo, sMaxRegNo;
		CString sRegCodeDes;
		INT iBookCount;	//책수
		for ( INT i = 1 ; i <= iRowCount ; i++ ) {
			//CString sRegCode = pDM->GetCellData( _T("BB_등록구분"), i-1 );
			sRegCode = pDM->GetCellData( _T("BB_등록구분"), i-1 );
			sQuery.Format( _T("I.REC_KEY = S.REC_KEY AND I.REC_KEY = V.SPECIES_KEY AND V.REC_KEY = B.VOL_KEY AND B.BINDED_BOOK_YN = 'N' AND B.REG_NO >= '%s' AND B.REG_NO <= '%s' AND B.REG_CODE = '%s' AND B.WORKING_STATUS > 'SEC' ORDER BY B.REG_CODE, B.REG_NO"), sRegNoStart, sRegNoEnd, sRegCode );
			//sQuery.Format( _T("I.REC_KEY = B.SPECIES_KEY AND B.REG_CODE = '%s' AND ACCESSION_REC_KEY IN(SELECT REC_KEY FROM CO_ACCESSION_REC_TBL WHERE ACCESSION_REC_NO =%s)"), sRegCode, sAccessionNo );
			if ( pDM_0800_02->RefreshDataManager( sQuery ) < 0 ) return;
			//m_pInfo->pCodeMgr->ConvertCodeToDescInDM(pDM, _T("등록구분"), _T("BB_등록구분"));
			sRegCodeDes = pDM->GetCellData( _T("BB_등록구분"), i-1 );
			INT iRowCount_02 = pDM_0800_02->GetRowCount();
			//책수
			iBookCount = iRowCount_02;
			INT iSwReg = 0;
			for ( INT j = 0 ; j < iRowCount_02 ; j ++ ) {
				//형태사항 만들기
				CString sPage = pDM_0800_02->GetCellData( _T("BB_면장수"), j );
				CString sPhysical = pDM_0800_02->GetCellData( _T("BB_물리적특성"), j );
				CString sBookSize = pDM_0800_02->GetCellData( _T("BB_크기"), j );
				//CString sForm = sPage + _T(" : ") + sPhysical + _T(" ; ") + sBookSize;
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
				pDM_0800_02->SetCellData( _T("형태사항"), sForm, j );
				//청구기호 만들기
				CString sShelfCode = pDM_0800_02->GetCellData( _T("IBS_청구_별치기호_코드"), j );
				CString sClassNo = pDM_0800_02->GetCellData( _T("IBS_청구_분류기호"), j );
				CString sBookCode = pDM_0800_02->GetCellData( _T("IBS_청구_도서기호"), j );
				CString sVolCode = pDM_0800_02->GetCellData( _T("IBS_청구_권책기호"), j );
				CString sDupCode = pDM_0800_02->GetCellData( _T("BB_복본기호"), j );
				//CString sCallNo = sShelfCode + _T(" ") + sClassNo + _T("-") + sBookCode + _T("=") + sVolCode;
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
				CString s245n;
				m_pInfo->pMarcMgr->GetItem( &marc, _T("245$n"), s245n );
				if ( s245n != _T("") ) {
					sTitleInfo = sTitleInfo + _T(" . ") + s245n;
				}
				CString s245p;
				m_pInfo->pMarcMgr->GetItem( &marc, _T("245$p"), s245p );
				if ( s245p != _T("") ) {
					sTitleInfo = sTitleInfo + _T(" , ") + s245p;
				}
				CString s245d;
				m_pInfo->pMarcMgr->GetItem( &marc, _T("245$d"), s245d );
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
				}*/
				//2006.01.24 hjy 수정 : 440 우선
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
				//2005-11-10 황정영 : 국방대는 권호책기호를 뒤에 넣어준다.
				if( str90 == _T("Y"))
				{
					CString strVolCode, strOtherVolTitle;
					pDM_0800_02->GetCellData( _T("VB_권호명"), j, strVolCode );
					pDM_0800_02->GetCellData( _T("VB_부차적권호명"), j, strOtherVolTitle );
					if(!strVolCode.IsEmpty())
					{
						sTitleInfo += _T(" [");
						sTitleInfo += strVolCode;
						sTitleInfo += _T(" : ");
						sTitleInfo += strOtherVolTitle;
						sTitleInfo += _T(" ]");
					}

					//2005-12-01 황정영 : 제어번호추가
					CString strControlNo;
					strControlNo.Empty();
					m_pInfo->pMarcMgr->GetItem( &marc, _T("001*"), strControlNo);
					pDM_0800_02->SetCellData( _T("IBS_제어번호"),strControlNo, j);
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
				//현금 표시 포멧으로 만들기
				NUMBERFMT nFmt = { 0, 0, 3, _T("."), _T(","), 1 };
				TCHAR szOUT[20];
				::GetNumberFormat(NULL, NULL, sPrice, &nFmt, szOUT, 20);
				sPrice = _T("\\") + sPrice;
				pDM_0800_02->SetCellData( _T("BB_가격"), sPrice, j );

				//2005-11-10 황정영 : 국방대 관리구분, 기증자 값
				CString strQuery;
				if(str90 == _T("Y"))
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
					strQuery.Format(_T("SELECT D.DONATOR_NAME ")
								  _T("FROM SE_DONATE_MANAGE_TBL D, SE_BOOK_TBL B ")
								  _T("WHERE B.SPECIES_KEY = D.SPECIES_KEY ")
								  _T("AND B.SPECIES_KEY = '%s' ")
								  _T("AND B.MANAGE_CODE = UDF_MANAGE_CODE")			
								  , strTmp);
					pDM_0800_02->GetOneValueQuery(strQuery,strDonator);
					pDM_0800_02->SetCellData(_T("기증자"),strDonator,j);
				}
			}			
		}
		m_pInfo->pCodeMgr->ConvertCodeToDescInDM(pDM, _T("등록구분"), _T("BB_등록구분"));
		//m_pInfo->pCodeMgr->ConvertCodeToDescInDM(pDM, _T("등록구분"), sRegCodeDes);
		sRegCodeDes = pDM->GetCellData( _T("BB_등록구분"), 0 );
		

		//출력하기
		CSIReportManager *pSIReportManager = new CSIReportManager(this);
		pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
		if (!pSIReportManager) {
			AfxMessageBox(_T("Report Manager를 생성하지 못하였습니다."));
			return;
		}
		INT ids;
		
		pSIReportManager->SetCONNECTION_INFO(pDM->CONNECTION_INFO);
		//ids = pSIReportManager->GetSIReportFileFromDB(_T("원부출력"));
		//A4,B4 선택
		CButton * pB4;
		pB4 = (CButton *)GetDlgItem(IDC_rB4);
		if ( pB4->GetCheck() == 1 ) {		
			if ( strIS64 == _T("Y") ) ids = pSIReportManager->GetSIReportFileFromDB(_T("SE원부출력B4_육사"));
			else if( str90 == _T("Y") ) ids = pSIReportManager->GetSIReportFileFromDB(_T("SE원부출력B4_국방대"));
			else ids = pSIReportManager->GetSIReportFileFromDB(_T("SE원부출력B4"));
		}
		else if (static_cast<CButton*>(GetDlgItem(IDC_rA4_RANDSCAPE))->GetCheck() == 1 ) 
		{
			if ( strIS64 == _T("Y") ) ids = pSIReportManager->GetSIReportFileFromDB(_T("SE원부출력_가로_육사"));
			else if( str90 == _T("Y") ) ids = pSIReportManager->GetSIReportFileFromDB(_T("SE원부출력_가로_국방대"));
			else ids = pSIReportManager->GetSIReportFileFromDB(_T("SE원부출력_가로"));
		}
		else {
			if ( strIS64 == _T("Y") ) ids = pSIReportManager->GetSIReportFileFromDB(_T("SE원부출력_육사"));
			else ids = pSIReportManager->GetSIReportFileFromDB(_T("SE원부출력"));
		}
		
		if (ids < 0) {
			AfxMessageBox(_T("DB로 부터 SI Report 정보를 가져오지 못하였습니다."));
			return;
		}
		
		//pSIReportManager->SetTempVariable(_T("%BARCODE바코드제목%"), _T("강남(이씨오)도서관"));
		//CString sAccAcqCode = pDmAccession->GetCellData( _T("원부입수구분"), 0 );
		CString sPrice = pDM->GetCellData( _T("가격"), 0 );
		CString sRegNo = sMinRegNo + _T(" 부터  ") + sMaxRegNo + _T(" 까지");
		//CString sBookCount = pDM->GetCellData( _T("책수"), 0 );
		CString sBookCount;
		sBookCount.Format( _T("%d"), iBookCount );
		//CString sRegDate = pDmAccession->GetCellData( _T("원부등록일자"), 0 );
		//pSIReportManager->SetTempVariable(_T("수입구분"), sAccAcqCode);
		//pSIReportManager->SetTempVariable(_T("수입년도"), sYear);

		//도서관명 가져오기
		CString sPath = _T("..\\CFG\\LocalLibraryInfo.cfg");
		FILE *fp;
		fp = _tfopen( sPath , _T("rb") );
		//fp = _tfopen( sFilePath , _T("rb") );
		
		if ( fp == NULL ) {
			AfxMessageBox( _T("..\\CFG\\LocalLibraryInfo.cfg 파일이 없습니다.") );
			return;
		}
		
		//++++++++++++++++++++++++++++++++++++++++++
		//Unicode일땐 2byte건너띄기
		FILE* fpUni = fp;
	
		if( 0xFF == fgetc(fpUni) ) 
		{
			if( 0xFE != fgetc(fpUni) )
			{
				fseek(fpUni, 0, SEEK_SET);
			}
		}
		else
		{
			fseek(fpUni, 0, SEEK_SET);
		}
		//------------------------------------------
		
		TCHAR cLine[256];
		CString sLibraryName;
		while( _fgetts( cLine, 256, fp ) ) {
			CString sLine;
			sLine.Format( _T("%s"), cLine );
			if ( sLine != _T("") ) {
				CString sTmp = sLine.Left(10);
				if ( sLine.Left(10) == _T("도서관이름") ) {
					INT iPos = sLine.Find(_T("="));
					sLibraryName = sLine.Mid( iPos+2 );
					sLibraryName = sLibraryName.Left( sLibraryName.GetLength() - 2 );
				}
			}
		}
		fclose(fp);
		
		//SetControlData( _T("CM_BO_CAT_PROC_0203_01"), _T("도서관명"), sLibraryName );
		sLibraryName = sLibraryName + _T("의 도서관자료로 아래와 같이 등록함.");

		pSIReportManager->SetTempVariable(_T("LIB_NAME"), sLibraryName);
		//가격
		//현금 표시 포멧으로 만들기
		NUMBERFMT nFmt = { 0, 0, 3, _T("."), _T(","), 1 };
		TCHAR szOUT[20];
		::GetNumberFormat(NULL, NULL, sPrice, &nFmt, szOUT, 20);
		CString sPriceFont;		
		sPriceFont.Format( _T("%15s"), szOUT );
		pSIReportManager->SetTempVariable(_T("가격"), sPriceFont);
		//pSIReportManager->SetTempVariable(_T("가격"), sPrice);
		pSIReportManager->SetTempVariable(_T("등록구분"), sRegCodeDes);
		//pSIReportManager->SetTempVariable(_T("원부번호"), sAccessionNo);
		pSIReportManager->SetTempVariable(_T("등록번호"), sRegNo);
		
		//2005-11-10 황정영 : 갑지/을지 수정
		if(str90 == _T("Y"))
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
		//pSIReportManager->SetTempVariable(_T("등록일자"), sRegDate);
		
		// 2005.08.01 ADD BY PDJ
		// 비고란 추가됨
		CString sRemark = pDM->GetCellData( _T("비고"), 0 );
		pSIReportManager->SetTempVariable(_T("비고"), sRemark);
	/*	
		m_pDM->InitDataMgrRef(1);
		m_pDM->SetDataMgrRef(0, _T("BB_등록번호"), _T("REG_NO"), _T("str"), _T(""), _T(""));
		m_pDM->TBL_NAME = _T("BO_BOOK_TBL");

		ids = m_pDM->RefreshDataManager(_T("rownum <= 50"));
		CString str;
		m_pDM->GetCellData(_T("BB_등록번호"), 0, str);
		INT nRowCount = m_pDM->GetRowCount();
	*/
		/*
		pSIReportManager->SetDataMgr(0, pDM_0800_02);
		ids = pSIReportManager->Print();
		if (pSIReportManager) {
			delete pSIReportManager;
			pSIReportManager = NULL;
		}
		*/

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
	
	//CDialog::OnOK();

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

HBRUSH CBO_CAT_REG_0800::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
