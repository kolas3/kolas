// BO_CAT_REG_MAIN.cpp : implementation file
//

#include "stdafx.h"
#include "BO_CAT_REG_MAIN.h"
#include "BO_CAT_REG_0400.h"
#include "..\BO_CAT_REGNO_GIVE\BO_CAT_REGNO_GIVE1.h"
#include "../BO_CAT_REGNO_CANCEL/BO_CAT_RegNoCancel.h"
#include "../../../공통/BO_ACQ_SPECIES_FORM/Species_Form.h"
#include "../../../공통/FileImport/FileImportDlg.h"
#include "../../../공통\MarcAdjustApi\CatApi_BO.h"
#include "../../../공통/BO_DATA_CHANGE_MANAGER/CodeChange.h"
#include "../../../공통/BO_DATA_CHANGE_MANAGER/AcqInfoChange.h"
#include "../../../공통/BO_DATA_CHANGE_MANAGER/DonateInfoChange.h"

#include "../../../공통/CMarcChanger/MARC_CHANGER_01.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_REG_MAIN dialog


CBO_CAT_REG_MAIN::CBO_CAT_REG_MAIN(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_REG_MAIN)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	//+++++++++++++++++++++++++++++++++++++++++
	//2003.10.28 이학중 추가 
    m_Bo_ProgressDlg = NULL ;
	m_nSortDirection = 0;
	m_pSearchDlg = NULL;
	m_pSortItemSet = NULL;
	m_nRow = -1;
	//+++++++++++++++++++++++++++++++++++++++++
}
CBO_CAT_REG_MAIN::~CBO_CAT_REG_MAIN()
{
	if(m_pSearchDlg)
	{
		delete m_pSearchDlg;
		m_pSearchDlg = NULL;
	}
	if(m_pSortItemSet)
	{
		delete m_pSortItemSet;
		m_pSortItemSet = NULL;
	}
}

VOID CBO_CAT_REG_MAIN::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_REG_MAIN)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_CAT_REG_MAIN, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_REG_MAIN)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_REG_MAIN message handlers

BOOL CBO_CAT_REG_MAIN::Create(CWnd* pParentWnd)
{

	return CDialog::Create(IDD, pParentWnd);

	
}
BOOL CBO_CAT_REG_MAIN::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("SM_단행_정리_등록_메인")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	CESL_Grid * pGrid = (CESL_Grid*)FindControl(_T("CM_단행_정리_등록_메인"), _T("MAIN_GRID"));
	if(pGrid) pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;	
	
	// UPDATE BY PDJ 2005.10.07
	// 기본정렬을 다시사용.. 별도 정렬기능이 제대로 동작안됨
	// 2005-12-29 황정영 수정 : 기본정렬 사용하면 복본순서 대로 정렬이 안되어 
	// 등록번호 부여시 순서가 불분명해짐
	pGrid->SetSort(FALSE);
	//pGrid->SetSort(TRUE);

	if (pGrid == NULL) return FALSE;
/*
	pGrid->InitPopupMenuItem();
	pGrid->AddPopupMenuItem(static_cast<CView*>(pMain), this->VIEW_STYLE);
*/
	m_MarcConvertMgr.SetMarcMgr(m_pInfo->pMarcMgr);

	m_pSearchDlg = new CBO_CAT_SEARCH(this);
	CESL_DataMgr *pDM = FindDM(_T("DM_단행_정리_등록_메인"));
	m_pSearchDlg->SetCONNECTION_INFO(pDM->CONNECTION_INFO);
	InitSortItem();
	InitPopupMenuItem();

	//========================================================
	//20080613 ADD BY PJW
	CString strMsg;
	strMsg = _T("등록대상 자료를 관리합니다.");
	static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_USER+2004, NULL, (long)strMsg.GetBuffer(0));
	strMsg.ReleaseBuffer();
	//========================================================


	
	EnableThemeDialogTexture(GetSafeHwnd());     
	return TRUE;
}

VOID CBO_CAT_REG_MAIN::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_MSHFLEXGRID);
	if (pGrid->GetSafeHwnd() == NULL) return;

	pGrid->MoveWindow(-2, 35, cx + 4, cy-35);	
}

INT CBO_CAT_REG_MAIN::CreateRegNO()
{
	//=======================================================================
	// 변수정의
	//=======================================================================
	INT idx, nAllCnt, nRegCnt;
	CString strTmp, strAcqCode;	
	CStringArray astrErrRegNo, astrErrAcq, astrErrData;
	CESL_Grid* pGrid	= NULL;
	CESL_DataMgr* pDM	= NULL;

	//=======================================================================
	// 1. Grid/DM을 설정한다.
	//=======================================================================
	pGrid = (CESL_Grid*)FindControl(_T("CM_단행_정리_등록_메인"), _T("MAIN_GRID"));	
	
	SelectMakeList(_T("CM_단행_정리_등록_메인"), _T("MAIN_GRID"));
	idx = SelectGetHeadPosition(_T("CM_단행_정리_등록_메인"), _T("MAIN_GRID"));
	if( idx < 0 )
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));	// 에러
		return FALSE;			
	}

	pDM = FindDM(_T("DM_단행_정리_등록_메인"));
	if( NULL == pDM )
	{
		ESLAfxMessageBox(_T("DM_BO_CAT_REG error"));
		return FALSE;
	}

	//=======================================================================
	// 2. 같은 수입구분에 대해서만 변경하여야 한다.
	// 2005-12-19 강봉수 수정
	//=======================================================================
	strAcqCode = pDM->GetCellData(_T("BB_수입구분"),idx);

	astrErrRegNo.RemoveAll();
	astrErrAcq.RemoveAll();
	astrErrData.RemoveAll();

	//=======================================================================
	// 3. 선정된 자료들의 유효성을 검사한다.
	//=======================================================================	
	nAllCnt = pGrid->SelectGetCount();
	nRegCnt = 0;
	while( idx != -1 )
	{

		CString sCodeTmp	= pDM->GetCellData(_T("BB_자료상태_코드"),	idx );
		CString sRecKeyTmp	= pDM->GetCellData(_T("BB_등록번호"),		idx );
		CString sAcqCode	= pDM->GetCellData(_T("BB_수입구분"),		idx );

		if( sCodeTmp != _T("BOR111N") && sRecKeyTmp.IsEmpty() == FALSE )
		{
			strTmp.Empty();
			strTmp.Format(_T("%d"), idx+1);
			astrErrRegNo.Add(strTmp);
			pGrid->SetAt(idx, 0, _T(""));
		}
		else if( (sCodeTmp != _T("BOR111N") && sRecKeyTmp.IsEmpty() == TRUE ) || 
				 (sCodeTmp == _T("BOR111N") && sRecKeyTmp.IsEmpty() == FALSE ) )
		{
			strTmp.Empty();
			strTmp.Format(_T("%d"), idx+1);
			astrErrData.Add(strTmp);
			pGrid->SetAt(idx, 0, _T(""));
		}
		/*******************************************************
		 *******************************************************/
		// 수입구분이 다르다면 변경자료에서 제외 한다.
		// 20080122 UPDATE PJW : 수입구분이 다를경우 다른 데이터 수만큼 메세지박스 뜨는걸 방지
		//					  수입구분이 드르면 메세지박스 띄우고 선정을 지운다음 리턴한다.
		/*
		else if( strAcqCode != sAcqCode )
		{
			strTmp.Empty();
			strTmp.Format(_T("%d"), idx+1);
			astrErrAcq.Add(strTmp);
			pGrid->SetAt(idx, 0, _T(""));
		}
		else	nRegCnt++;
		*/
		else if( strAcqCode != sAcqCode )
		{
			INT nAllCnt = 0;
			CString strChk;
			nAllCnt = pGrid->GetRows();
			ESLAfxMessageBox(_T("수입구분 동일한 자료들로 선택 주세요."));
			for( INT i = 1 ; i < nAllCnt-1; i++ )
			{
				pGrid->GetAt(i, 0, strChk);
				if( _T("V") == strChk )
				{
					pGrid->SetAt(i, 0, _T(""));
				}
			}
			return 0;
		}
		else	nRegCnt++;
		/*******************************************************
		 *******************************************************/

		idx = SelectGetNext(_T("CM_단행_정리_등록_메인"), _T("MAIN_GRID"));
	}

	//=======================================================================
	// 4. 등록번호 부여창을 열어 해당자료에 대해 등록번호를 부여한다.
	//=======================================================================
	BO_CAT_REGNO_GIVE dlg(this, _T(""));
	dlg.m_strParentCMAlias = _T("CM_단행_정리_등록_메인");
	dlg.m_strParentDMAlias = _T("DM_단행_정리_등록_메인");
	dlg.m_nOpenMode = 1;
	if ( dlg.DoModal() != IDOK ) return 0;
	

	//=======================================================================
	// 5. 변경되지 않는 자료에 대한 처리를 한다.
	//=======================================================================	
	if( nAllCnt != nRegCnt )
	{
		CTime time = CTime::GetCurrentTime();
		CString strTime;
		FILE* fp = _tfopen(_T("..\\log\\CreateRegNo.txt"),_T("wb"));
		if( !fp ) 
		{
			ESLAfxMessageBox(_T("에러 로그파일을 생성할 수 없습니다."));
			return 0;
	}
	
		/*UNCHANGE*/fputc(0xFF, fp);
		/*UNCHANGE*/fputc(0xFE, fp);

		strTime.Empty();
		strTime.Format(_T("\r\n\r\n\r\n[%04d-%02d-%02d %02d:%02d:%02d]"), time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
		_fputts(strTime+_T("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"),fp);

		strTmp.Empty();
		strTmp.Format(_T("\r\n[ 선정자료 ] :\t%d 건"), nAllCnt);
		_fputts(strTmp, fp);
		strTmp.Empty();
		strTmp.Format(_T("\r\n[ 성\t공 ] :\t%d 건"), nRegCnt);
		_fputts(strTmp, fp);
		strTmp.Empty();
		strTmp.Format(_T("\r\n[ 실\t패 ] :\t%d 건"), nAllCnt-nRegCnt);
		_fputts(strTmp, fp);

		if( astrErrRegNo.GetSize() > 0 )
		{
			_fputts(_T("\r\n\r\n\r\n▶ 다음자료는 등록번호가 이미 부여되어 있습니다.\r\n"), fp);
			strTmp.Empty();
			strTmp = astrErrRegNo[0];
			for( idx=1 ; idx<astrErrRegNo.GetSize(); idx++ )
		{
				strTmp += _T(", ") + astrErrRegNo[idx];
		}

			_fputts(strTmp, fp);
		}

		if( astrErrAcq.GetSize() > 0 )
		{
			_fputts(_T("\r\n\r\n\r\n▶ 다음자료는 수입구분이 첫번째 선정자료와 다릅니다.(※수입구분이 같은 자료들로 처리하십시오.)\r\n"), fp);
			strTmp.Empty();
			strTmp = astrErrAcq[0];
			for( idx=1 ; idx<astrErrAcq.GetSize(); idx++ )
		{
				strTmp += _T(", ") + astrErrAcq[idx];
			}

			_fputts(strTmp, fp);
		}

		if( astrErrData.GetSize() > 0 )
		{
			_fputts(_T("\r\n\r\n\r\n▶ 다음자료는 데이터의 내용이 잘못되었습니다.(※등록번호 취소후 다시 처리하십시오.)\r\n"), fp);
			strTmp.Empty();
			strTmp = astrErrData[0];
			for( idx=1 ; idx<astrErrData.GetSize(); idx++ )
			{
				strTmp += _T(", ") + astrErrData[idx];
		}

			_fputts(strTmp, fp);
	}
		_fputts(strTime+_T("----------------------------------------------------------"),fp);
		fclose(fp);

		strTmp.Empty();
		strTmp.Format(_T("%d건의 자료에 대해 등록번호가 부여되지 않았습니다.\n에러 로그를 보시겠습니까?"), nAllCnt-nRegCnt);
		if( IDYES == ESLAfxMessageBox(strTmp, MB_ICONQUESTION | MB_YESNO) )
		{
			ShellExecute(NULL,_T("OPEN"), _T("..\\log\\CreateRegNo.txt"),NULL,NULL,SW_SHOW);
		}
	}
	

	return 0;


}

VOID CBO_CAT_REG_MAIN::CancelGivenRegNo()
{
	INT idx,ids;
	SelectMakeList(_T("CM_단행_정리_등록_메인"), _T("MAIN_GRID"));
	idx = SelectGetHeadPosition(_T("CM_단행_정리_등록_메인"), _T("MAIN_GRID"));

	if(idx < 0)
	{
		//에러처리
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return;
	}	

	CESL_DataMgr *pDM = FindDM(_T("DM_단행_정리_등록_메인"));
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_단행_정리_등록_메인") );
		return ;
	}

	CString BookKeyList,sRecKeyTmp,sRegNo,sTmpRegNo;
	sRegNo = pDM->GetCellData( _T("BB_등록번호") , idx );

	if (sRegNo != _T(""))
	{
		BookKeyList = pDM->GetCellData( _T("BB_책KEY") , idx );
	}

	idx = SelectGetNext(_T("CM_단행_정리_등록_메인"), _T("MAIN_GRID"));

	while ( idx != -1 ) {
		sTmpRegNo = pDM->GetCellData( _T("BB_등록번호") , idx );

		if (sTmpRegNo != _T(""))
		{
			sRecKeyTmp = pDM->GetCellData( _T("BB_책KEY") , idx );
			BookKeyList = BookKeyList + _T(", ") + sRecKeyTmp;
		}

		idx = SelectGetNext(_T("CM_단행_정리_등록_메인"), _T("MAIN_GRID"));
	}

	if(BookKeyList.GetLength() == 0)
	{
		ESLAfxMessageBox(_T("등록번호가 부여된 자료가 없습니다."));
		return;
	}

	if(IDCANCEL == ESLAfxMessageBox(_T("선정하신 자료에 부여된 등록번호를 해제하시겠습니까?"), MB_OKCANCEL))
		return;

	CBO_CAT_RegNoCancel dlg(this);
	ids = dlg.RegNoCancel(BookKeyList);

	if(ids < 0)
		ESLAfxMessageBox(_T("등록번호를 해제할 수 없습니다."));
	else
		ESLAfxMessageBox(_T("등록번호를 해제하였습니다."));

//	CString GridAlias = GetGridCtrlAlias(m_tabACQ.GetCurSel(), m_IsSpecies);
//	ids = m_pPurchaseMgr->UpdateDMbyRegNoRelativeField(_T("CM_LIGHT_단행_수서"), GridAlias, m_tabACQ.GetCurSel(), m_IsSpecies);
//	if(ids < 0) return ;

	DisplayGrid(_T("CM_단행_정리_등록_메인"), _T("MAIN_GRID"));
	return;	
}

VOID CBO_CAT_REG_MAIN::DisplayGrid(CString cmAlias, CString gridAlias)
{
	
	CESL_Grid * pGrid = (CESL_Grid*)FindControl(cmAlias, gridAlias);
	if(!pGrid) return;
	pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;
	pGrid->Display();
}

INT CBO_CAT_REG_MAIN::GridDisplayEventProc(CString &strGridAlias, INT nRow, 
INT nCol, CString &strDMFieldAlias, CString &str)
{
EFS_BEGIN
	CString strAcqType, strAcqYear, strGroupNo, strTmpData;
	INT nGroupNo, ids;
	CESL_DataMgr *pDM = FindDM(_T("DM_단행_정리_등록_메인"));

	if(m_nRow != nRow)
	{
		CMarc marc;
		strTmpData = pDM->GetCellData(_T("BS_MARC"), nRow);
		ids = m_pInfo->pMarcMgr->Decoding(strTmpData, &marc);
		if(ids >= 0)
		{
			strTmpData.Empty();	ids = m_MarcConvertMgr.GetSeriesStatement(&marc, strTmpData);
			if(ids >= 0) pDM->SetCellData(_T("UDF_총서사항"), strTmpData, nRow);
			strTmpData.Empty();	ids = m_MarcConvertMgr.GetEditionStatement(&marc, strTmpData);
			if(ids >= 0) pDM->SetCellData(_T("IBS_판사항"), strTmpData, nRow);
			strTmpData.Empty();	 strTmpData = pDM->GetCellData(_T("IBS_본표제"), nRow);
			ids = m_MarcConvertMgr.GetTitleStatement(&marc, strTmpData);
			if(ids >= 0) pDM->SetCellData(_T("UDF_표제사항"), strTmpData, nRow);
			strTmpData.Empty();	ids = m_MarcConvertMgr.GetPublishPlace(&marc, strTmpData);
			if(ids >= 0) pDM->SetCellData(_T("UDF_발행지"), strTmpData, nRow);
			strTmpData.Empty();	ids = m_MarcConvertMgr.GetCurrencyCode(&marc, strTmpData);
			// 2006.01.09 강봉수 - 화폐구분을 추가한다.
			if(ids >= 0) pDM->SetCellData(_T("UDF_화폐구분"), strTmpData, nRow);
			m_nRow = nRow;
			if(m_nRow == pDM->GetRowCount()-1) m_nRow = -1;
		}
	}
	
    if (strGridAlias == _T("MAIN_GRID"))
	{
		// 2006.01.09 강봉수 - 한국화폐일경우 소수점을 적용하지 않는다.
		if		( strDMFieldAlias == _T("BB_가격") )
		{
			strTmpData.Empty();
			strTmpData = pDM->GetCellData(_T("UDF_화폐구분"), nRow);
			if( strTmpData == _T("\\") )
			{
				INT nPos = str.Find(_T("."));
				if( nPos > 0 )	str = str.Left(nPos);
				pDM->SetCellData(strDMFieldAlias, str, nRow);
			}
		}
        else if ( strDMFieldAlias == _T("BB_자료상태_코드") )
		{
            if		( str == _T("BOR111N") )	str = _T("등록인계자료");
			else if ( str == _T("BOR112N") )	str = _T("등록자료");
			else if ( str == _T("BOR113O") )	str = _T("가원부자료");

			if( pDM )	pDM->SetCellData(_T("BB_자료상태_설명"), str, nRow);
        }
		else if( strDMFieldAlias == _T("BB_수입구분") )
		{
			m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("입수구분"), str, str);
			pDM->SetCellData(_T("BP_수서정보KEY"), pDM->GetCellData(_T("BB_수서정보KEY"), nRow), nRow);
		}
		else if( strDMFieldAlias == _T("UDF_그룹번호") )
		{
			pDM->SetCellData(_T("BP_수입구분"), pDM->GetCellData(_T("BB_수입구분"), nRow), nRow);
			pDM->SetCellData(_T("BP_수입년도"), pDM->GetCellData(_T("BB_수입년도"), nRow), nRow);
			pDM->SetCellData(_T("BP_그룹번호"), pDM->GetCellData(_T("BB_그룹번호"), nRow), nRow);
			strAcqType = pDM->GetCellData(_T("BP_수입구분"), nRow);
			strAcqYear = pDM->GetCellData(_T("BP_수입년도"), nRow);
			strGroupNo = pDM->GetCellData(_T("BP_그룹번호"), nRow);
			if(!strAcqType.IsEmpty() && strAcqType != _T("0"))
			{
				if(!strAcqYear.IsEmpty() && !strGroupNo.IsEmpty())
				{
					nGroupNo = _ttoi(strGroupNo.GetBuffer(0));
					if(nGroupNo < 0) strTmpData.Format(_T("%s-적용안함"), strAcqYear);
					else strTmpData.Format(_T("%s-%04d"), strAcqYear, nGroupNo);
					str = strTmpData;
					pDM->SetCellData(strDMFieldAlias, str, nRow);
				}
			}
		}
		else if( strDMFieldAlias == _T("BS_주제구분") || strDMFieldAlias == _T("BP_주제구분") )
		{
			strTmpData.Empty();
			strTmpData = pDM->GetCellData(_T("IBS_분류표구분"), nRow);
			
			if		( strTmpData == _T("1") )
			{
				m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("KDC대주제부호관리"), str, str);
			}
			else if	( strTmpData == _T("2") )
			{
				m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("DDC대주제부호관리"), str, str);
			}
			else if ( strTmpData == _T("3") || strTmpData == _T("7") )
			{
				m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("LC주제부호"), str, str);
			}
			else /*if( strTmpData.IsEmpty() == TRUE )*/ 
			{
				strTmpData = _T("");
			}

			pDM->SetCellData(_T("UDF_주제구분설명"), str, nRow);
		}
    }
	return 0;

EFS_END
return -1;

}
VOID CBO_CAT_REG_MAIN::InputBook()
{
	CESL_DataMgr *pDM = FindDM(_T("DM_단행_정리_등록_메인"));
	CESL_Grid *pGrid = ( CESL_Grid * )FindControl(_T("CM_단행_정리_등록_메인"), _T("MAIN_GRID"));

	//수입구분 기본값을 가져와야쥐
	CString strAcqCode;
	SPECIES_TYPE nSpeciesType = SPECIES_TYPE_INVALID; 
	ACQ_TYPE nAcqType = ACQ_TYPE_INVALID;
	if(!m_pSearchDlg->GetCreate())
	{
		m_pInfo->pCodeMgr->GetDefaultCode(_T("입수구분"), strAcqCode);
		INT nAcqCode = _ttoi(strAcqCode.GetBuffer(0));
		nAcqType = (ACQ_TYPE)nAcqCode;
		if(nAcqType == PURCHASE) 
		{
			nSpeciesType = BO_REG_PURCHASE;
			m_pSearchDlg->GetGroupCollection()->SetAcqType(nAcqType);
		}
		else if(nAcqType == DONATE)
		{
			nSpeciesType = BO_REG_DONATE;
			m_pSearchDlg->GetGroupCollection()->SetAcqType(nAcqType);
		}
		else if(nAcqType == ACQ_TYPE_INVALID)
		{
			nSpeciesType = BO_REG_INVALID;
			m_pSearchDlg->GetGroupCollection()->SetAcqType(nAcqType);
		}
		else 
		{
			nSpeciesType = BO_REG_INVALID;
			nAcqType = OTHER_ACQ_TYPE;
			m_pSearchDlg->GetGroupCollection()->SetAcqType(nAcqType);
			m_pSearchDlg->GetGroupCollection()->SetAcqCode(strAcqCode);
		}	
	}
	else 
	{
		if(m_pSearchDlg->GetGroupCollection()->GetAcqType() == PURCHASE) 
			nSpeciesType = BO_REG_PURCHASE;
		else if(m_pSearchDlg->GetGroupCollection()->GetAcqType() == DONATE)
			nSpeciesType = BO_REG_DONATE;
		else if(m_pSearchDlg->GetGroupCollection()->GetAcqType() == ACQ_TYPE_INVALID)
			nSpeciesType = BO_REG_INVALID;
		else 
			nSpeciesType = BO_REG_INVALID;
	}
	
	CSpecies_Form dlg(this);

	dlg.SetAcqType(m_pSearchDlg->GetGroupCollection()->GetAcqType());
	dlg.SetSpeciesType(nSpeciesType);	
	dlg.SetOpType(SINSERT);
	dlg.SetSpeciesKeyAlias(_T("IBS_종KEY"));
	dlg.SetAccuKeyAlias(_T("BB_수서정보KEY"));
	dlg.SetGroupCollection(m_pSearchDlg->GetGroupCollection());
	dlg.SetParentType(BOOK);
	dlg.SetParentDM(pDM);
	dlg.SetParentBookDM(pDM);	
	dlg.SetParentGrid(pGrid);
	dlg.DoModal();

	pGrid->DisplayNumber();

	SetRegDataInfo();
}
INT CBO_CAT_REG_MAIN::DeleteBook()
{
	CESL_DataMgr *pDM = FindDM(_T("DM_단행_정리_등록_메인"));
	CESL_ControlMgr *pCM = FindCM(_T("CM_단행_정리_등록_메인"));
	CESL_Grid * pGrid = (CESL_Grid*)pCM->FindControl(_T("MAIN_GRID"));

	// 2005-01-17 송승민
	// 그리드에 아무 데이터도 없거나 선정된 데이터가 없을때 삭제 버튼 누른경우 그냥 리턴한다.
	// BEGIN:
	if(pDM->GetRowCount() == 0) 
	{
		ESLAfxMessageBox(_T("검색된 자료가 없습니다."));
		return 0;
	}
	if( (pGrid->SelectMakeList() < 0) || (pGrid->SelectGetHeadPosition() < 0) )
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return 0;
	}
	// END:
	
	CBO_ACQ_Business_Api api(this);
	api.InitTempData();		api.InitTempPtr();
	api.SetTempPtr(_T("DM"), (DWORD*)pDM);
	api.SetTempPtr(_T("Grid"), (DWORD*)pGrid);
	api.SetTempData(_T("SPECIES_TYPE"), (INT)BO_REG_INVALID);
	api.SetTempData(_T("ACQ_TYPE"), (INT)ACQ_TYPE_INVALID);
	api.SetTempData(_T("삭제타입"), (INT)BOOK_DELETE);
	api.SetTempData(_T("책수알리아스"), _T("BB_등록번호"));
	api.SetTempData(_T("가격알리아스"), _T("원부번호"));
	api.SetTempData(_T("삭제종류"), _T(""));
	api.SetTempPtr(_T("종키리스트"), NULL);
	api.SetTempPtr(_T("수서정보키리스트"), NULL);
	api.SetTempPtr(_T("권일련번호리스트"), NULL);
	INT ids = api.SPFExecutePath(_T("삭제"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정한 자료를 삭제할 수 없습니다."));
		return 0;
	}

	pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;

	pGrid->Display();
	SetRegDataInfo();
	return 0;

}


VOID CBO_CAT_REG_MAIN::DoClassify() //분류대상처리
{
	INT idx, ids;

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //2003.10.28 이학중 추가 
	m_Bo_ProgressDlg = new CBO_CAT_REG_PROGRESS_DLG(this);
	m_Bo_ProgressDlg->Create (this);
    m_Bo_ProgressDlg->CenterWindow () ;
    m_Bo_ProgressDlg->ShowWindow (SW_SHOW);
	m_Bo_ProgressDlg->UpdateWindow() ;
	
	m_Bo_ProgressDlg->SetTitle (_T("분류대상처리를 하고 있습니다."));
    m_Bo_ProgressDlg->m_ProgressCtrl->SetStep(1);
    m_Bo_ProgressDlg->m_ProgressCtrl->SetPos(0) ;
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	SelectMakeList(_T("CM_단행_정리_등록_메인"), _T("MAIN_GRID"));
	idx = SelectGetHeadPosition(_T("CM_단행_정리_등록_메인"), _T("MAIN_GRID"));

	CESL_DataMgr *p049DM = FindDM(_T("DM_단행_정리_등록_049"));
	p049DM->FreeData();

	if(idx < 0)
	{
		//에러처리
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	    // 2003.10.28 이학중 추가 
		if(m_Bo_ProgressDlg != NULL)
		{
			delete m_Bo_ProgressDlg ;
            m_Bo_ProgressDlg = NULL ;
			
		}
        //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		return;
	}	

	CESL_DataMgr *pDM = FindDM(_T("DM_단행_정리_등록_메인"));
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_단행_정리_등록_메인") );
		return ;
	}

	CString BookKey,strStatus,sRec_Key,strRegNo,strCopyCode,strVolTitle;
	strStatus = pDM->GetCellData( _T("BB_자료상태_코드") , idx );

	if (strStatus != _T("BOR112N")) //등록자료 
	{
		//에러처리
		ESLAfxMessageBox(_T("등록 자료만 처리 할 수 있습니다."));
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// 2003.10.28 이학중 추가 
		if(m_Bo_ProgressDlg != NULL)
		{
			delete m_Bo_ProgressDlg ;
			m_Bo_ProgressDlg = NULL ;
			
		}
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		return;
	}

	idx = SelectGetNext(_T("CM_단행_정리_등록_메인"), _T("MAIN_GRID"));

	while ( idx != -1 ) {
		strStatus = pDM->GetCellData( _T("BB_자료상태_코드") , idx );

		if (strStatus != _T("BOR112N")) //등록자료 
		{
			//에러처리
			ESLAfxMessageBox(_T("등록 자료만 처리 할 수 있습니다."));
			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			// 2003.10.28 이학중 추가 
			if(m_Bo_ProgressDlg != NULL)
			{
				delete m_Bo_ProgressDlg ;
				m_Bo_ProgressDlg = NULL ;
				
			}
			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			return;
		}

		idx = SelectGetNext(_T("CM_단행_정리_등록_메인"), _T("MAIN_GRID"));
	}

	if(IDCANCEL == ESLAfxMessageBox(_T("분류대상처리 작업을 하시겠습니까?"), MB_OKCANCEL))
	{
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// 2003.10.28 이학중 추가 
		if(m_Bo_ProgressDlg != NULL)
		{
			delete m_Bo_ProgressDlg ;
			m_Bo_ProgressDlg = NULL ;
			
		}
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		return;
	}

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_단행_정리_등록_메인"));
	if ( pCM == NULL ) {
		AfxMessageBox( _T("CM_단행_정리_등록_메인 error") );
		return ;
	}

	//대상 책 키 가져오기
	SelectMakeList(_T("CM_단행_정리_등록_메인"), _T("MAIN_GRID"));
	idx = SelectGetHeadPosition(_T("CM_단행_정리_등록_메인"), _T("MAIN_GRID"));

	BookKey = pDM->GetCellData( _T("BB_책KEY") , idx );

	// 변경 spl 실행
	INT Dm_index=0;
	pDM->StartFrame();
	pDM->InitDBFieldData();
	pDM->AddDBFieldData(_T("WORKING_STATUS"), _T("STRING"), _T("BOC112O") );
	pDM->AddDBFieldData(_T("PREV_WORKING_STATUS"), _T("STRING"), _T("BOR112N") );
	pDM->MakeUpdateFrame(_T("BO_BOOK_TBL"), _T("REC_KEY"), _T("NUMERIC"), BookKey, idx);	

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// INDEX TABLE 에 WORKING_STATUS 필드 UPDATE , BOOK TABLE 과 같은 데이터로 UPDATE
    sRec_Key = pDM->GetCellData( _T("IBS_종KEY") , idx );

	pDM->InitDBFieldData ();
	pDM->AddDBFieldData(_T("WORKING_STATUS"), _T("STRING"), _T("BOC112O") );
	pDM->MakeUpdateFrame(_T("IDX_BO_TBL"), _T("REC_KEY"), _T("NUMERIC"), sRec_Key, idx);
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	// 040226_HSS IDX_TMP_BO_TBL 추가 ++++++++++++++
	//=====================================================
	//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL 사용안함 으로 주석처리함
// 	ids = InsertTMPtbl(pDM, sRec_Key);
// 	if ( 0 > ids ) {
// 		AfxMessageBox( _T("InsertTMPtbl() Error") );
// 		return;
// 	}
	//=====================================================
	
	// +++++++++++++++++++++++++++++++++++++++++

	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// 2005-12-01 황정영 : 부록정보 유무확인 후 부록자료상태를 분류인계로 UPDATE한다.
	CString sQuery, sAppen;
	CESL_DataMgr * pTmpDM = new CESL_DataMgr;
	pTmpDM->SetCONNECTION_INFO(pDM->CONNECTION_INFO);
	sQuery.Format(_T("SELECT WORKING_STATUS, REC_KEY FROM BO_BOOK_TBL WHERE BOOK_APPENDIX_FLAG = 'A' AND SPECIES_KEY = %s"),sRec_Key);
	pTmpDM->RestructDataManager(sQuery);
	INT nRow = pTmpDM->GetRowCount();
	if( 0 < nRow)
	{
		pTmpDM->StartFrame();
		for( INT i = 0; i < nRow ; i++)
		{
			BookKey = pTmpDM->GetCellData(i,1);
			pTmpDM->InitDBFieldData();
			pTmpDM->AddDBFieldData(_T("WORKING_STATUS"), _T("STRING"), _T("BOC112O") );
			pTmpDM->MakeUpdateFrame(_T("BO_BOOK_TBL"), _T("REC_KEY"), _T("NUMERIC"), BookKey, i);
		}
		pTmpDM->SendFrame();
		pTmpDM->EndFrame();
	}

	//049 tag 생성
	sRec_Key = pDM->GetCellData( _T("IBS_종KEY") , idx );
	strRegNo = pDM->GetCellData( _T("BB_등록번호") , idx );
	strCopyCode = pDM->GetCellData( _T("BB_복본기호") , idx );  
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// 2003.08.21 이학중 수정 
	// 049 Tag $v 생성시 VOL_TITLE 에서 VOL 로 수정 
	strVolTitle = pDM->GetCellData( _T("BB_편권차") , idx );

	// 기존 소스 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//strVolTitle = pDM->GetCellData( _T("BB_편제명부록명") , idx );
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //2003.10.28 이학중 수정
    //정리 인계시 마크에 별치기호 입력 하도록 
    
	CString strSHELF_CODE ;
    strSHELF_CODE = pDM->GetCellData (_T("BB_별치기호_코드"),idx) ; 
    //수정전
	//GetMARCTag(sRec_Key,strRegNo,strCopyCode,strVolTitle);
    GetMARCTag(sRec_Key,strRegNo,strCopyCode,strVolTitle,strSHELF_CODE);
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	
	pDM->DeleteRow(idx);
	Dm_index++;
	

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// 2003.10.28 이학중 추가 
    m_Bo_ProgressDlg->m_ProgressCtrl->SetRange (0,SelectGetCount(_T("CM_단행_정리_등록_메인"), _T("MAIN_GRID"))) ;   
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


	idx = SelectGetNext(_T("CM_단행_정리_등록_메인"), _T("MAIN_GRID"));

	while ( idx != -1 ) {
		BookKey = pDM->GetCellData( _T("BB_책KEY") , (idx - Dm_index) );

		pDM->InitDBFieldData();
		pDM->AddDBFieldData(_T("WORKING_STATUS"), _T("STRING"), _T("BOC112O") );
		pDM->AddDBFieldData(_T("PREV_WORKING_STATUS"), _T("STRING"), _T("BOR112N") );
		pDM->MakeUpdateFrame(_T("BO_BOOK_TBL"), _T("REC_KEY"), _T("NUMERIC"), BookKey, (idx - Dm_index));

		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// INDEX TABLE 에 WORKING_STATUS 필드 UPDATE , BOOK TABLE 과 같은 데이터로 UPDATE
		sRec_Key = pDM->GetCellData( _T("IBS_종KEY") , (idx - Dm_index) );

		pDM->InitDBFieldData ();
		pDM->AddDBFieldData(_T("WORKING_STATUS"), _T("STRING"), _T("BOC112O") );
		pDM->MakeUpdateFrame(_T("IDX_BO_TBL"), _T("REC_KEY"), _T("NUMERIC"), sRec_Key, (idx - Dm_index));
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		// 040226_HSS IDX_TMP_BO_TBL 추가 ++++++++++++++
		//=====================================================
		//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL 사용안함 으로 주석처리함
// 		ids = InsertTMPtbl(pDM, sRec_Key);
// 		if ( 0 > ids ) {
// 			AfxMessageBox( _T("InsertTMPtbl() Error") );
// 			return;
// 		}
		//=====================================================
		
		
		// +++++++++++++++++++++++++++++++++++++++++

		//049 tag 생성
		sRec_Key = pDM->GetCellData( _T("IBS_종KEY") , (idx - Dm_index) );
		strRegNo = pDM->GetCellData( _T("BB_등록번호") , (idx - Dm_index) );
		strCopyCode = pDM->GetCellData( _T("BB_복본기호") , (idx - Dm_index) );  
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// 2003.08.21 이학중 수정 
		// 049 Tag $v 생성시 VOL_TITLE 에서 VOL 로 수정 
		strVolTitle = pDM->GetCellData( _T("BB_편권차") , (idx - Dm_index) );
		
		// 기존 소스 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//strVolTitle = pDM->GetCellData( _T("BB_편제명부록명") , idx );
        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//2003.10.28 이학중 수정
		//정리 인계시 마크에 별치기호 입력 하도록 

		CString strSHELF_CODE ;
		strSHELF_CODE = pDM->GetCellData (_T("BB_별치기호_코드"),(idx - Dm_index)) ; 
		//수정전
		//GetMARCTag(sRec_Key,strRegNo,strCopyCode,strVolTitle);
		GetMARCTag(sRec_Key,strRegNo,strCopyCode,strVolTitle,strSHELF_CODE);

        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		pDM->DeleteRow(idx - Dm_index);
		Dm_index++;
        
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        //2003.10.28 이학중 추가
		m_Bo_ProgressDlg->m_ProgressCtrl->StepIt () ;
		
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		idx = SelectGetNext(_T("CM_단행_정리_등록_메인"), _T("MAIN_GRID"));
	}
/*
	if( pDM->SendFrame() >= 0 )
	{
		DBto049();
	}
	*/
	DBto049(pDM);
	ids = pDM->SendFrame();
	if ( 0 > ids ) {
		AfxMessageBox( _T("SendFrame()확인") );
		return;
	}
	ids = pDM->EndFrame();
	if ( 0 > ids ) {
		AfxMessageBox( _T("EndFrame()확인") );
		return;
	}

	CESL_Grid *pGrid_MainGrid_All = ( CESL_Grid * )FindControl(_T("CM_단행_정리_등록_메인"), _T("MAIN_GRID"));
	pGrid_MainGrid_All->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;

	pGrid_MainGrid_All->Display();

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// 2003.10.28 이학중 추가 
	if(m_Bo_ProgressDlg != NULL)
	{
		delete m_Bo_ProgressDlg ;
		m_Bo_ProgressDlg = NULL ;
	}
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	AfxMessageBox(_T("변경 되었습니다."));

	return;
}

// 040226_HSS IDX_TMP_BO_TBL 추가 ++++++++++++++
INT CBO_CAT_REG_MAIN::InsertTMPtbl(CESL_DataMgr *pDM, CString sRec_Key)
{
	//=====================================================
	//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL 사용안함 으로 주석처리함
// 	CString  sQuery;
// 	INT ids;
// 
// 	sQuery.Format(_T("INSERT INTO IDX_TMP_BO_TBL (REC_KEY, SPECIES_KEY, IDX_WORK_FLAG, INPUT_DATE) ")
// 			_T("VALUES (ESL_SEQ.NEXTVAL, %s, 'I', sysdate);"),sRec_Key);
// 	ids = pDM->AddFrame( sQuery );
// 	if ( 0 > ids ) return -1;
	//=====================================================
	
	
	return 0;
}

VOID CBO_CAT_REG_MAIN::DoCatalog() //정리 대상처리
{
	INT idx, ids;
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //2003.10.28 이학중 추가 
	m_Bo_ProgressDlg = new CBO_CAT_REG_PROGRESS_DLG(this);
	m_Bo_ProgressDlg->Create (this);
    m_Bo_ProgressDlg->CenterWindow () ;
    m_Bo_ProgressDlg->ShowWindow (SW_SHOW);
	m_Bo_ProgressDlg->UpdateWindow() ;
	
	m_Bo_ProgressDlg->SetTitle (_T("정리대상처리를 하고 있습니다."));
    m_Bo_ProgressDlg->m_ProgressCtrl->SetStep(1);
    m_Bo_ProgressDlg->m_ProgressCtrl->SetPos(0) ;
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	SelectMakeList(_T("CM_단행_정리_등록_메인"), _T("MAIN_GRID"));
	idx = SelectGetHeadPosition(_T("CM_단행_정리_등록_메인"), _T("MAIN_GRID"));

	CESL_DataMgr *p049DM = FindDM(_T("DM_단행_정리_등록_049"));
	p049DM->FreeData();

	if(idx < 0)
	{
		//에러처리
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	    // 2003.10.28 이학중 추가 
		if(m_Bo_ProgressDlg != NULL)
		{
			delete m_Bo_ProgressDlg ;
            m_Bo_ProgressDlg = NULL ;
			
		}
        //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		return;
	}	

	CESL_DataMgr *pDM = FindDM(_T("DM_단행_정리_등록_메인"));
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_단행_정리_등록_메인") );
		return ;
	}

	CString BookKey,strStatus,sRec_Key,strRegNo,strCopyCode,strVolTitle;
	strStatus = pDM->GetCellData( _T("BB_자료상태_코드") , idx );

	if (strStatus != _T("BOR112N")) //등록자료 
	{
		//에러처리
		ESLAfxMessageBox(_T("등록 자료만 처리 할 수 있습니다."));
		
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	    // 2003.10.28 이학중 추가 
		if(m_Bo_ProgressDlg != NULL)
		{
			delete m_Bo_ProgressDlg ;
            m_Bo_ProgressDlg = NULL ;
			
		}
        //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		return;
		
	}

	idx = SelectGetNext(_T("CM_단행_정리_등록_메인"), _T("MAIN_GRID"));

	while ( idx != -1 ) {
		strStatus = pDM->GetCellData( _T("BB_자료상태_코드") , idx );

		if (strStatus != _T("BOR112N")) //등록자료 
		{
			//에러처리
			ESLAfxMessageBox(_T("등록 자료만 처리 할 수 있습니다."));
			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			// 2003.10.28 이학중 추가 
			if(m_Bo_ProgressDlg != NULL)
			{
				delete m_Bo_ProgressDlg ;
				m_Bo_ProgressDlg = NULL ;
				
			}
			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			return;
		}

		idx = SelectGetNext(_T("CM_단행_정리_등록_메인"), _T("MAIN_GRID"));
	}



	if(IDCANCEL == ESLAfxMessageBox(_T("정리대상처리 작업을 하시겠습니까?"), MB_OKCANCEL))
	{
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// 2003.10.28 이학중 추가 
		if(m_Bo_ProgressDlg != NULL)
		{
			delete m_Bo_ProgressDlg ;
			m_Bo_ProgressDlg = NULL ;
			
		}
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		return;
	}


	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_단행_정리_등록_메인"));
	if ( pCM == NULL ) {
		AfxMessageBox( _T("CM_단행_정리_등록_메인 error") );
		return ;
	}


	//대상 책 키 가져오기
	SelectMakeList(_T("CM_단행_정리_등록_메인"), _T("MAIN_GRID"));
	idx = SelectGetHeadPosition(_T("CM_단행_정리_등록_메인"), _T("MAIN_GRID"));

	BookKey = pDM->GetCellData( _T("BB_책KEY") , idx );

	// 변경 spl 실행
	INT Dm_index=0;
	pDM->StartFrame();
	pDM->InitDBFieldData();
	pDM->AddDBFieldData(_T("WORKING_STATUS"), _T("STRING"), _T("BOC113N") );
	pDM->AddDBFieldData(_T("PREV_WORKING_STATUS"), _T("STRING"), _T("BOR112N") );
	pDM->MakeUpdateFrame(_T("BO_BOOK_TBL"), _T("REC_KEY"), _T("NUMERIC"), BookKey, idx);	
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// 2003.08.24 이학중 추가 
	// INDEX TABLE 에 WORKING_STATUS 필드 UPDATE , BOOK TABLE 과 같은 데이터로 UPDATE
    sRec_Key = pDM->GetCellData( _T("IBS_종KEY") , idx );

	pDM->InitDBFieldData ();
	pDM->AddDBFieldData(_T("WORKING_STATUS"), _T("STRING"), _T("BOC113N") );
	pDM->MakeUpdateFrame(_T("IDX_BO_TBL"), _T("REC_KEY"), _T("NUMERIC"), sRec_Key, idx);
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	// 040226_HSS IDX_TMP_BO_TBL 추가 ++++++++++++++
	//=====================================================
	//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL 사용안함 으로 주석처리함
// 	ids = InsertTMPtbl(pDM, sRec_Key);
// 	if ( 0 > ids ) {
// 		AfxMessageBox( _T("InsertTMPtbl() Error") );
// 		return;
// 	}
	//=====================================================
	
	
	// +++++++++++++++++++++++++++++++++++++++++

	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// 2005-12-01 황정영 : 부록정보 유무확인 후 부록자료상태를 정리인계로 UPDATE한다.
	CString sQuery, sAppen;
	CESL_DataMgr * pTmpDM = new CESL_DataMgr;
	pTmpDM->SetCONNECTION_INFO(pDM->CONNECTION_INFO);
	sQuery.Format(_T("SELECT WORKING_STATUS, REC_KEY FROM BO_BOOK_TBL WHERE BOOK_APPENDIX_FLAG = 'A' AND SPECIES_KEY = %s"),sRec_Key);
	pTmpDM->RestructDataManager(sQuery);
	INT nRow = pTmpDM->GetRowCount();
	if( 0 < nRow)
	{
		pTmpDM->StartFrame();
		for( INT i = 0; i < nRow ; i++)
		{
			BookKey = pTmpDM->GetCellData(i,1);
			pTmpDM->InitDBFieldData();
			pTmpDM->AddDBFieldData(_T("WORKING_STATUS"), _T("STRING"), _T("BOC113N") );
			pTmpDM->MakeUpdateFrame(_T("BO_BOOK_TBL"), _T("REC_KEY"), _T("NUMERIC"), BookKey, i);
		}
		pTmpDM->SendFrame();
		pTmpDM->EndFrame();
	}
	
	//049 tag 생성 ++++++++++++++++++++++++++++++
	sRec_Key = pDM->GetCellData( _T("IBS_종KEY") , idx );
	strRegNo = pDM->GetCellData( _T("BB_등록번호") , idx );
	strCopyCode = pDM->GetCellData( _T("BB_복본기호") , idx );  
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// 2003.08.21 이학중 수정 
	// 049 Tag $v 생성시 VOL_TITLE 에서 VOL 로 수정 
	strVolTitle = pDM->GetCellData( _T("BB_편권차") , idx );

	// 기존 소스 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//strVolTitle = pDM->GetCellData( _T("BB_편제명부록명") , idx );
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //2003.10.28 이학중 수정
    //정리 인계시 마크에 별치기호 입력 하도록 
    
	CString strSHELF_CODE ;
    strSHELF_CODE = pDM->GetCellData (_T("BB_별치기호_코드"),idx) ; 
    //수정전
	//GetMARCTag(sRec_Key,strRegNo,strCopyCode,strVolTitle);
	GetMARCTag(sRec_Key,strRegNo,strCopyCode,strVolTitle,strSHELF_CODE );
    
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	//작업 로우 지우기 	
	pDM->DeleteRow(idx);
	Dm_index++;
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// 2003.10.28 이학중 추가 
    m_Bo_ProgressDlg->m_ProgressCtrl->SetRange (0,SelectGetCount(_T("CM_단행_정리_등록_메인"), _T("MAIN_GRID"))) ;   
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	idx = SelectGetNext(_T("CM_단행_정리_등록_메인"), _T("MAIN_GRID"));

	while ( idx != -1 ) {
		BookKey = pDM->GetCellData( _T("BB_책KEY") , (idx - Dm_index) );

		pDM->InitDBFieldData();
		pDM->AddDBFieldData(_T("WORKING_STATUS"), _T("STRING"), _T("BOC113N") );
		pDM->AddDBFieldData(_T("PREV_WORKING_STATUS"), _T("STRING"), _T("BOR112N") );
		pDM->MakeUpdateFrame(_T("BO_BOOK_TBL"), _T("REC_KEY"), _T("NUMERIC"), BookKey, (idx - Dm_index));

		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// 2003.08.24 이학중 추가 
		// INDEX TABLE 에 WORKING_STATUS 필드 UPDATE , BOOK TABLE 과 같은 데이터로 UPDATE
		sRec_Key = pDM->GetCellData( _T("IBS_종KEY") , (idx - Dm_index) );

		pDM->InitDBFieldData ();
		pDM->AddDBFieldData(_T("WORKING_STATUS"), _T("STRING"), _T("BOC113N") );
		pDM->MakeUpdateFrame(_T("IDX_BO_TBL"), _T("REC_KEY"), _T("NUMERIC"), sRec_Key, (idx - Dm_index));
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		// 040226_HSS IDX_TMP_BO_TBL 추가 ++++++++++++++
		//=====================================================
		//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL 사용안함 으로 주석처리함
// 		ids = InsertTMPtbl(pDM, sRec_Key);
// 		if ( 0 > ids ) {
// 			AfxMessageBox( _T("InsertTMPtbl() Error") );
// 			return;
// 		}
		//=====================================================
		
		// +++++++++++++++++++++++++++++++++++++++++

		//049 tag 생성
		sRec_Key = pDM->GetCellData( _T("IBS_종KEY") , (idx - Dm_index));
		strRegNo = pDM->GetCellData( _T("BB_등록번호") , (idx - Dm_index));
		strCopyCode = pDM->GetCellData( _T("BB_복본기호") , (idx - Dm_index));  
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// 2003.08.21 이학중 수정 
		// 049 Tag $v 생성시 VOL_TITLE 에서 VOL 로 수정 
		strVolTitle = pDM->GetCellData( _T("BB_편권차") , (idx - Dm_index) );
		
		// 기존 소스 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//strVolTitle = pDM->GetCellData( _T("BB_편제명부록명") , idx - Dm_index );
        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//2003.10.28 이학중 수정
		//정리 인계시 마크에 별치기호 입력 하도록 

		CString strSHELF_CODE ;
		strSHELF_CODE = pDM->GetCellData (_T("BB_별치기호_코드"),(idx - Dm_index)) ; 
		//수정전
		//GetMARCTag(sRec_Key,strRegNo,strCopyCode,strVolTitle);
		
		GetMARCTag(sRec_Key,strRegNo,strCopyCode,strVolTitle,strSHELF_CODE);
        //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


		//작업 로우 지우기 	
		pDM->DeleteRow(idx - Dm_index);
		Dm_index++;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        //2003.10.28 이학중 추가
		m_Bo_ProgressDlg->m_ProgressCtrl->StepIt () ;
		
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		idx = SelectGetNext(_T("CM_단행_정리_등록_메인"), _T("MAIN_GRID"));
	}
/*
	if( pDM->SendFrame() >= 0 )
	{
		DBto049();
	}
*/
	DBto049(pDM);
	ids = pDM->SendFrame();
	if ( 0 > ids ) {
		AfxMessageBox( _T("SendFrame()확인") );
		return;
	}
	ids = pDM->EndFrame();
	if ( 0 > ids ) {
		AfxMessageBox( _T("EndFrame()확인") );
		return;
	}

	CESL_Grid *pGrid_MainGrid_All = ( CESL_Grid * )FindControl(_T("CM_단행_정리_등록_메인"), _T("MAIN_GRID"));
	pGrid_MainGrid_All->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;

	pGrid_MainGrid_All->Display();
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// 2003.10.28 이학중 추가 
	if(m_Bo_ProgressDlg != NULL)
	{
		delete m_Bo_ProgressDlg ;
		m_Bo_ProgressDlg = NULL ;
	}
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	AfxMessageBox(_T("변경 되었습니다."));

	return;

}

VOID CBO_CAT_REG_MAIN::DoFindCopy()
{
	INT idx, ids;
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //2003.10.28 이학중 추가 
	m_Bo_ProgressDlg = new CBO_CAT_REG_PROGRESS_DLG(this);
	m_Bo_ProgressDlg->Create (this);
    m_Bo_ProgressDlg->CenterWindow () ;
    m_Bo_ProgressDlg->ShowWindow (SW_SHOW);
	m_Bo_ProgressDlg->UpdateWindow() ;
	
	m_Bo_ProgressDlg->SetTitle (_T("복본대상처리를 하고 있습니다."));
    m_Bo_ProgressDlg->m_ProgressCtrl->SetStep(1);
    m_Bo_ProgressDlg->m_ProgressCtrl->SetPos(0) ;
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	SelectMakeList(_T("CM_단행_정리_등록_메인"), _T("MAIN_GRID"));
	idx = SelectGetHeadPosition(_T("CM_단행_정리_등록_메인"), _T("MAIN_GRID"));

	CESL_DataMgr *p049DM = FindDM(_T("DM_단행_정리_등록_049"));
	p049DM->FreeData();

	if(idx < 0)
	{
		//에러처리
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	    // 2003.10.28 이학중 추가 
		if(m_Bo_ProgressDlg != NULL)
		{
			delete m_Bo_ProgressDlg ;
            m_Bo_ProgressDlg = NULL ;
			
		}
        //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		return;
	}	

	CESL_DataMgr *pDM = FindDM(_T("DM_단행_정리_등록_메인"));
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_단행_정리_등록_메인") );
		return ;
	}

	CString BookKey,strStatus,sRec_Key,strRegNo,strCopyCode,strVolTitle;
	strStatus = pDM->GetCellData( _T("BB_자료상태_코드") , idx );

	if (strStatus != _T("BOR112N")) //등록자료 
	{
		//에러처리
		ESLAfxMessageBox(_T("등록 자료만 처리 할 수 있습니다."));
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// 2003.10.28 이학중 추가 
		if(m_Bo_ProgressDlg != NULL)
		{
			delete m_Bo_ProgressDlg ;
			m_Bo_ProgressDlg = NULL ;
			
		}
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		return;
	}

	idx = SelectGetNext(_T("CM_단행_정리_등록_메인"), _T("MAIN_GRID"));

	while ( idx != -1 ) {
		strStatus = pDM->GetCellData( _T("BB_자료상태_코드") , idx );

		if (strStatus != _T("BOR112N")) //등록자료 
		{
			//에러처리
			ESLAfxMessageBox(_T("등록 자료만 처리 할 수 있습니다."));
			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			// 2003.10.28 이학중 추가 
			if(m_Bo_ProgressDlg != NULL)
			{
				delete m_Bo_ProgressDlg ;
				m_Bo_ProgressDlg = NULL ;
				
			}
			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			return;
		}

		idx = SelectGetNext(_T("CM_단행_정리_등록_메인"), _T("MAIN_GRID"));
	}



	if(IDCANCEL == ESLAfxMessageBox(_T("복본대상처리 작업을 하시겠습니까?"), MB_OKCANCEL))
	{
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// 2003.10.28 이학중 추가 
		if(m_Bo_ProgressDlg != NULL)
		{
			delete m_Bo_ProgressDlg ;
			m_Bo_ProgressDlg = NULL ;
			
		}
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		return;
	}
 

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_단행_정리_등록_메인"));
	if ( pCM == NULL ) {
		AfxMessageBox( _T("CM_단행_정리_등록_메인 error") );
		return ;
	}


	//대상 책 키 가져오기
	SelectMakeList(_T("CM_단행_정리_등록_메인"), _T("MAIN_GRID"));
	idx = SelectGetHeadPosition(_T("CM_단행_정리_등록_메인"), _T("MAIN_GRID"));

	BookKey = pDM->GetCellData( _T("BB_책KEY") , idx );

	// 변경 spl 실행
	INT Dm_index=0;
	pDM->StartFrame();
	pDM->InitDBFieldData();
	pDM->AddDBFieldData(_T("WORKING_STATUS"), _T("STRING"), _T("BOC111O") );
	pDM->AddDBFieldData(_T("PREV_WORKING_STATUS"), _T("STRING"), _T("BOR112N") );
	pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), GetWorkLogMsg(_T("원부등록"),WORK_LOG) );

	pDM->MakeUpdateFrame(_T("BO_BOOK_TBL"), _T("REC_KEY"), _T("NUMERIC"), BookKey, idx);
	

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// 2003.08.20 이학중 추가 
	// INDEX TABLE 에 WORKING_STATUS 필드 UPDATE , BOOK TABLE 과 같은 데이터로 UPDATE
    sRec_Key = pDM->GetCellData( _T("IBS_종KEY") , idx );

	pDM->InitDBFieldData ();
	pDM->AddDBFieldData(_T("WORKING_STATUS"), _T("STRING"), _T("BOC111O") );
	pDM->MakeUpdateFrame(_T("IDX_BO_TBL"), _T("REC_KEY"), _T("NUMERIC"), sRec_Key, idx);
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	// 040226_HSS IDX_TMP_BO_TBL 추가 ++++++++++++++
	//=====================================================
	//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL 사용안함 으로 주석처리함
// 	ids = InsertTMPtbl(pDM, sRec_Key);
// 	if ( 0 > ids ) {
// 		AfxMessageBox( _T("InsertTMPtbl() Error") );
// 		return;
// 	}
	//=====================================================
	
	// +++++++++++++++++++++++++++++++++++++++++

	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// 2005-12-01 황정영 : 부록정보 유무확인 후 부록자료상태를 복본대상인계로 UPDATE한다.
	CString sQuery, sAppen;
	CESL_DataMgr * pTmpDM = new CESL_DataMgr;
	pTmpDM->SetCONNECTION_INFO(pDM->CONNECTION_INFO);
	sQuery.Format(_T("SELECT WORKING_STATUS, REC_KEY FROM BO_BOOK_TBL WHERE BOOK_APPENDIX_FLAG = 'A' AND SPECIES_KEY = %s"),sRec_Key);
	pTmpDM->RestructDataManager(sQuery);
	INT nRow = pTmpDM->GetRowCount();
	if( 0 < nRow)
	{
		pTmpDM->StartFrame();
		for( INT i = 0; i < nRow ; i++)
		{
			BookKey = pTmpDM->GetCellData(i,1);
			pTmpDM->InitDBFieldData();
			pTmpDM->AddDBFieldData(_T("WORKING_STATUS"), _T("STRING"), _T("BOC111O") );
			pTmpDM->MakeUpdateFrame(_T("BO_BOOK_TBL"), _T("REC_KEY"), _T("NUMERIC"), BookKey, i);
		}
		pTmpDM->SendFrame();
		pTmpDM->EndFrame();
	}

	//049 tag 생성
	sRec_Key = pDM->GetCellData( _T("IBS_종KEY") , idx );
	strRegNo = pDM->GetCellData( _T("BB_등록번호") , idx );
	strCopyCode = pDM->GetCellData( _T("BB_복본기호") , idx );  
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// 2003.08.21 이학중 수정 
	// 049 Tag $v 생성시 VOL_TITLE 에서 VOL 로 수정 
	strVolTitle = pDM->GetCellData( _T("BB_편권차") , idx );

	// 기존 소스 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//strVolTitle = pDM->GetCellData( _T("BB_편제명부록명") , idx );
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //2003.10.28 이학중 수정
    //정리 인계시 마크에 별치기호 입력 하도록 
    
	CString strSHELF_CODE ;
    strSHELF_CODE = pDM->GetCellData (_T("BB_별치기호_코드"),idx) ; 
    //수정전
	//GetMARCTag(sRec_Key,strRegNo,strCopyCode,strVolTitle);
	GetMARCTag(sRec_Key,strRegNo,strCopyCode,strVolTitle,strSHELF_CODE);
	
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	
	pDM->DeleteRow(idx);
	Dm_index++;
	
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// 2003.10.28 이학중 추가 
    m_Bo_ProgressDlg->m_ProgressCtrl->SetRange (0,SelectGetCount(_T("CM_단행_정리_등록_메인"), _T("MAIN_GRID"))) ;   
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	idx = SelectGetNext(_T("CM_단행_정리_등록_메인"), _T("MAIN_GRID"));

	while ( idx != -1 ) {
		BookKey = pDM->GetCellData( _T("BB_책KEY") , (idx - Dm_index) );

		pDM->InitDBFieldData();
		pDM->AddDBFieldData(_T("WORKING_STATUS"), _T("STRING"), _T("BOC111O") );
		pDM->AddDBFieldData(_T("PREV_WORKING_STATUS"), _T("STRING"), _T("BOR112N") );
		pDM->MakeUpdateFrame(_T("BO_BOOK_TBL"), _T("REC_KEY"), _T("NUMERIC"), BookKey, (idx - Dm_index));

        //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// 2003.08.20 이학중 추가  
		// INDEX TABLE 에 WORKING_STATUS 필드 UPDATE , BOOK TABLE 과 같은 데이터로 UPDATE
		sRec_Key = pDM->GetCellData( _T("IBS_종KEY") , (idx - Dm_index) );

		pDM->InitDBFieldData ();
		pDM->AddDBFieldData(_T("WORKING_STATUS"), _T("STRING"), _T("BOC111O") );
		pDM->MakeUpdateFrame(_T("IDX_BO_TBL"), _T("REC_KEY"), _T("NUMERIC"), sRec_Key, (idx - Dm_index));
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		// 040226_HSS IDX_TMP_BO_TBL 추가 ++++++++++++++
		//=====================================================
		//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL 사용안함 으로 주석처리함
// 		ids = InsertTMPtbl(pDM, sRec_Key);
// 		if ( 0 > ids ) {
// 			AfxMessageBox( _T("InsertTMPtbl() Error") );
// 			return;
// 		}
		//=====================================================
		
		// +++++++++++++++++++++++++++++++++++++++++

		//049 tag 생성
/* 수정 사항 : 73번원부(3개이상)에서 복본대상 처리시 SQL Error 발생 수정   +++
 * 수정 날짜 : 2003.08.20
 * 수정 사람 : 김홍수 
 */
		/*
		sRec_Key = pDM->GetCellData( _T("IBS_종KEY") , idx );
		strRegNo = pDM->GetCellData( _T("BB_등록번호") , idx );
		strCopyCode = pDM->GetCellData( _T("BB_복본기호") , idx );  
		strVolTitle = pDM->GetCellData( _T("BB_편제명부록명") , idx );
		*/

		sRec_Key = pDM->GetCellData( _T("IBS_종KEY") , (idx - Dm_index) );
		strRegNo = pDM->GetCellData( _T("BB_등록번호") , (idx - Dm_index) );
		strCopyCode = pDM->GetCellData( _T("BB_복본기호") , (idx - Dm_index) );  
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// 2003.08.21 이학중 수정 
		// 049 Tag $v 생성시 VOL_TITLE 에서 VOL 로 수정 
		strVolTitle = pDM->GetCellData( _T("BB_편권차") , (idx - Dm_index) );
		
		// 기존 소스 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//strVolTitle = pDM->GetCellData( _T("BB_편제명부록명") , idx );
        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// 수정 사항 : 73번원부(3개이상)에서 복본대상 처리시 SQL Error 발생 수정   ---

		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//2003.10.28 이학중 수정
		//정리 인계시 마크에 별치기호 입력 하도록 

		CString strSHELF_CODE ;
		strSHELF_CODE = pDM->GetCellData (_T("BB_별치기호_코드"),(idx - Dm_index)) ; 
		//수정전
		//GetMARCTag(sRec_Key,strRegNo,strCopyCode,strVolTitle);

		GetMARCTag(sRec_Key,strRegNo,strCopyCode,strVolTitle,strSHELF_CODE);

        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		pDM->DeleteRow(idx - Dm_index);
		Dm_index++;

		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        //2003.10.28 이학중 추가
		m_Bo_ProgressDlg->m_ProgressCtrl->StepIt () ;
		
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


		idx = SelectGetNext(_T("CM_단행_정리_등록_메인"), _T("MAIN_GRID"));

	}
/*
	if( pDM->SendFrame() >= 0 )
	{
		DBto049();
	}
*/
	DBto049(pDM);
	ids = pDM->SendFrame();
	if ( 0 > ids ) {
		AfxMessageBox( _T("SendFrame()확인") );
		return;
	}
	ids = pDM->EndFrame();
	if ( 0 > ids ) {
		AfxMessageBox( _T("EndFrame()확인") );
		return;
	}

	CESL_Grid *pGrid_MainGrid_All = ( CESL_Grid * )FindControl(_T("CM_단행_정리_등록_메인"), _T("MAIN_GRID"));
	pGrid_MainGrid_All->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;

	pGrid_MainGrid_All->Display();
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// 2003.10.28 이학중 추가 
	if(m_Bo_ProgressDlg != NULL)
	{
		delete m_Bo_ProgressDlg ;
		m_Bo_ProgressDlg = NULL ;
	}
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	AfxMessageBox(_T("변경 되었습니다."));

	return;

}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//2003.10.28 이학중 수정 
//INT CBO_CAT_REG_MAIN::Make049Tag(INT idx,CString strRegNO,CString strCopyCode, CString strVolTitle)
INT CBO_CAT_REG_MAIN::Make049Tag(INT idx,CString strRegNO,CString strCopyCode, CString strVolTitle,CString strShelfCode /* = _T("") */)
{
	CString strMarc;
	CESL_DataMgr *pDM = FindDM(_T("DM_단행_정리_등록_049"));
	strMarc = pDM->GetCellData( _T("MARC") , idx);

	//strMARC에 strRegNO를 넣으면 됨 
	
	CMarc marc;
	INT ids = m_pInfo->pMarcMgr->Decoding(strMarc, &marc);

	// 등록번호가 MARC에 존재하는지 체크한다.
	CString strItemData;
	CArray<CString, CString&> arrItem;
	m_pInfo->pMarcMgr->GetItem(&marc, _T("049$l"), strItemData, &arrItem);
	CCatApi_BO CatApi(this);
	if (CatApi.IsRegNoInArray(&arrItem, strRegNO)) return 0;

	// 049 찾아오기
	CString strTagCode = _T("049");
	CString str049Field;
	ids = m_pInfo->pMarcMgr->GetField(&marc, strTagCode, str049Field);
	if (str049Field.IsEmpty())
	{
		str049Field = _T("0 ");
	}
	else
	{
		str049Field = str049Field.Mid(0,str049Field.GetLength()-1);

	}

	CString strTemp;
	if( !strRegNO.IsEmpty() )//등록번호 만 있을경우
	{
		strTemp.Format(_T("%cl%s"),SUBFIELD_CODE, strRegNO);
		str049Field+= strTemp;
	}
	if( !strVolTitle.IsEmpty() )//편권차  
	{
		strTemp.Format(_T("%cv%s"),SUBFIELD_CODE, strVolTitle);		
		str049Field+= strTemp;
	}	
	if( !strCopyCode.IsEmpty() )//편권차  
	{
		strTemp.Format(_T("%cc%s"),SUBFIELD_CODE, strCopyCode);		
		str049Field+= strTemp;
	}	

	  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // 2003.10.28 이학중 추가  
    CString strTemp_049f ;
	CArray<CString,CString&> arrTemp ;
	CMap<CString, LPCTSTR, CString, LPCTSTR> mapTempSubData;
	mapTempSubData.RemoveAll();
    ids = m_pInfo->pMarcMgr->GetItem(&marc, _T("049$f"), strTemp_049f,&arrTemp);
	mapTempSubData.SetAt(strShelfCode,strShelfCode);
	for(INT i =0 ;i <arrTemp.GetSize ();i++)
	{
	    mapTempSubData.SetAt(arrTemp.GetAt (i),arrTemp.GetAt (i));	
	}
	if(mapTempSubData.GetCount() > 1)
	{
		//AfxMessageBox(_T("다른 별치 기호가 있습니다. 지시기호를 바꿉니다."));
        if( !strShelfCode .IsEmpty() )
		{
		strTemp.Format(_T("%cf%s"),SUBFIELD_CODE, strShelfCode);
		str049Field.Replace (str049Field.Mid (0,2),_T("1 "));
		str049Field+= strTemp;
		}
	}
	else
	{ 
	    m_pInfo->pMarcMgr->DeleteItem (&marc,_T("049$f"),_T(""));	
		if( !strShelfCode .IsEmpty() )
		{
			strTemp.Format(_T("%cf%s"),SUBFIELD_CODE, strShelfCode);
			CString sGt;
			sGt.Format( _T("%c"), SUBFIELD_CODE );
			CString sFindChar ;
			sFindChar.Format (_T("%sf"),sGt);
			INT nFirst = str049Field.Find (sFindChar);
			if(nFirst !=-1) 
			{
				//===================================================
				//2009.10.19 UPDATE BY PJW : 별치기호가 1자리일수도 있고 2자리 이상일수도 있음으로 코드의 길이를 기준으로 잘라야한다.
//				str049Field.Replace (str049Field.Mid (nFirst,4),_T(""));
				str049Field.Replace (str049Field.Mid (nFirst,strShelfCode.GetLength()+2),_T(""));
				//===================================================
				
				str049Field+= strTemp;
			}
			else
			{
				str049Field+= strTemp;
			}
		}
	}
    
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	CString Tmp;

	m_pInfo->pMarcMgr->GetField(&marc, strTagCode , Tmp);
	if ( Tmp.IsEmpty() )
	{
		m_pInfo->pMarcMgr->DeleteField(&marc, strTagCode);
		m_pInfo->pMarcMgr->InsertField(&marc, strTagCode + str049Field);
	}
	else
	{
		Tmp = Tmp.Mid(0,Tmp.GetLength()-1);
		m_pInfo->pMarcMgr->DeleteField(&marc, strTagCode);
		m_pInfo->pMarcMgr->InsertField(&marc, strTagCode + str049Field);
//		m_pInfo->pMarcMgr->InsertField(&marc, strTagCode + Tmp + str049Field.Mid(2));

	}
//	m_pInfo->pMarcMgr->DeleteField(&marc, strTagCode);

	m_pInfo->pMarcMgr->Encoding(&marc, strMarc);
	

	pDM->SetCellData( _T("MARC"),strMarc, idx);

	
	return 0;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//2003.10.28 이학중 수정 
//INT CBO_CAT_REG_MAIN::GetMARCTag(CString sRec_key,CString strRegNO,CString strCopyCode, CString strVolTitle )
INT CBO_CAT_REG_MAIN::GetMARCTag(CString sRec_key,CString strRegNO,CString strCopyCode, CString strVolTitle , CString strShelfCode /* =_T("")*/ )
{
	INT idx=0;
	CString sKey,strQry,strMARC;
	CESL_DataMgr *pDM = FindDM(_T("DM_단행_정리_등록_049"));

	//데이터 메니저에 마크가 있는지 확인한다.
	INT count = pDM->GetRowCount();

	for( INT i=0 ; i< count ; i++)
	{
		sKey = pDM->GetCellData( _T("IBS_종KEY") , i );
		if( sKey == sRec_key)
		{
			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			//2003.10.28 이학중 수정 
			//Make049Tag(i,strRegNO,strCopyCode,strVolTitle);
            Make049Tag(i,strRegNO,strCopyCode,strVolTitle,strShelfCode);
			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			return 0;
		}

	}

	//데이터 메니저에 마크가 없으면 가져와서 넣는다. 
	pDM->InsertRow(-1);

	strQry.Format(_T("SELECT MARC FROM BO_SPECIES_TBL WHERE REC_KEY = %s"),sRec_key);
	pDM->GetOneValueQuery(strQry,strMARC);

	pDM->SetCellData(_T("IBS_종KEY"), sRec_key , count);
	pDM->SetCellData(_T("MARC"), strMARC , count);

		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//2003.10.28 이학중 수정 
	//Make049Tag(count,strRegNO,strCopyCode,strVolTitle);
    Make049Tag(count,strRegNO,strCopyCode,strVolTitle,strShelfCode); 
	return 0;
}

//INT CBO_CAT_REG_MAIN::DBto049()
INT CBO_CAT_REG_MAIN::DBto049(CESL_DataMgr *pDM_Send)
{
	INT idx=0, ids;
	CString sKey,strQry,strMARC;
	CESL_DataMgr *pDM = FindDM(_T("DM_단행_정리_등록_049"));

	//데이터 메니저에 마크가 있는지 확인한다.
	INT count = pDM->GetRowCount();

	for( INT i=0 ; i< count ; i++)
	{
		sKey = pDM->GetCellData( _T("IBS_종KEY") , i );
		strMARC = pDM->GetCellData( _T("MARC") , i );

		pDM_Send->InitDBFieldData();
		pDM_Send->AddDBFieldData(_T("MARC"), _T("STRING"), strMARC, -1);
		//pDM->ORACLE_OCIMgr_MakeUpdateFrame(_T("BO_SPECIES_TBL"), _T("REC_KEY"), _T("NUMERIC"), sKey);
		ids = pDM_Send->MakeUpdateFrame( _T("BO_SPECIES_TBL"), _T("REC_KEY"), _T("NUMERIC"), sKey );
		if ( 0 > ids ) return -1;
		/*
		pDM->StartFrame();
		pDM->InitDBFieldData();
		pDM->AddDBFieldData(_T("MARC"), _T("STRING"), strMARC, -1);
		//pDM->ORACLE_OCIMgr_MakeUpdateFrame(_T("BO_SPECIES_TBL"), _T("REC_KEY"), _T("NUMERIC"), sKey);
		pDM->MakeUpdateFrame( _T("BO_SPECIES_TBL"), _T("REC_KEY"), _T("NUMERIC"), sKey );
		pDM->SendFrame();
		pDM->EndFrame();
		*/		
	}

	return 0;

}

INT CBO_CAT_REG_MAIN::ModifyBook(OPERATION_TYPE nOpType)
{
	CESL_DataMgr *pDM = FindDM(_T("DM_단행_정리_등록_메인"));

	// 2005-01-17 송승민
	// 그리드에 아무 데이터가 없을 때 수정버튼을 누를경우 리턴한다.
	if(nOpType != SINSERT)
	{
		if(pDM->GetRowCount() == 0)
		{
			ESLAfxMessageBox(_T("검색된 자료가 없습니다."));
			return -1;
		}
	}

	CSpecies_Form dlg(this);
	dlg.SetAcqType(ACQ_TYPE_INVALID);
	dlg.SetSpeciesType(BO_REG_INVALID);
		
	dlg.SetOpType(nOpType);
	dlg.SetSpeciesKeyAlias(_T("IBS_종KEY"));
	dlg.SetAccuKeyAlias(_T("BB_수서정보KEY"));

	CString m_Year;
	// 040304_HSS 수정 LocalTime으로 Select +++++++++++++++
	//CTime mTime = pDM->GetDBTime();
	CTime mTime = CTime::GetCurrentTime();
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++
	m_Year.Format(_T("%d"), mTime.GetYear());

	dlg.SetParentGroupInfo((INT)PURCHASE, m_Year , _T(""), pDM->CONNECTION_INFO);
	dlg.SetParentType(BOOK);
	dlg.SetParentDM(pDM);
	dlg.SetParentBookDM(pDM);	

	CESL_Grid *pGrid_MainGrid_All = ( CESL_Grid * )FindControl(_T("CM_단행_정리_등록_메인"), _T("MAIN_GRID"));
	//===================================================
	//2009.04.07 ADD BY PJW : 그리드 수정을 할경우 선정된자료가 있을경우 현재커서를 선정에 추가한다.
//DEL	if(nOpType == GUPDATE)
//DEL	{
		pGrid_MainGrid_All->SelectMakeList();
		if( pGrid_MainGrid_All->SelectGetCount() > 0 )
		{
			pGrid_MainGrid_All->SetAt( pGrid_MainGrid_All->GetIdx() , 0 , _T("V") );
			pGrid_MainGrid_All->SelectMakeList();
		}
//DEL	}
	//===================================================
	dlg.SetParentGrid(pGrid_MainGrid_All);
	dlg.DoModal();
	pGrid_MainGrid_All->DisplayNumber();	

	return SetRegDataInfo();
}

INT CBO_CAT_REG_MAIN::FileImport()
{
EFS_BEGIN
	CESL_DataMgr *pDM = FindDM(_T("DM_단행_정리_등록_메인"));

	CFileImportDlg dlg(this);
	dlg.SetBeginImportStatus(_T("등록"), _T("BOR111N"));
	dlg.SetOpenMode(4000);

	CString m_Year;
	// 040304_HSS 수정 LocalTime으로 Select +++++++++++++++
	//CTime mTime = pDM->GetDBTime();
	CTime mTime = CTime::GetCurrentTime();
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++
	m_Year.Format(_T("%d"), mTime.GetYear());

	dlg.SetAcqYear(m_Year);
	
	dlg.SetGroupInfo(_T(""));


	dlg.SetParentDM(pDM);

	CESL_Grid *m_pGrid = ( CESL_Grid * )FindControl(_T("CM_단행_정리_등록_메인"), _T("MAIN_GRID"));

	dlg.SetParentGrid(m_pGrid);
	
	dlg.DoModal();
	m_pGrid->DisplayNumber();



	return 0;
EFS_END
return -1;
}

BEGIN_EVENTSINK_MAP(CBO_CAT_REG_MAIN, CDialog)
    //{{AFX_EVENTSINK_MAP(CBO_CAT_REG_MAIN)
	ON_EVENT(CBO_CAT_REG_MAIN, IDC_MSHFLEXGRID, -601 /* DblClick */, OnDblClickMshflexgrid, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CBO_CAT_REG_MAIN::OnDblClickMshflexgrid() 
{
	// UPDATE BY PDJ 2005.10.07
	// 기본정렬을 다시사용.. 별도 정렬기능이 제대로 동작안됨
	// 	
	
	CESL_Grid *m_pGrid = ( CESL_Grid * )FindControl(_T("CM_단행_정리_등록_메인"), _T("MAIN_GRID"));
	if(m_pGrid->GetMouseRow() == 0)
	{
//		SortRegData(m_pGrid);
//		ShowSortDlg();
		return;
	}
	else
		ModifyBook(GUPDATE);
	
}

//정종석추가===================================================================
VOID CBO_CAT_REG_MAIN::RegDataOrder(CESL_ControlMgr * pCM, CESL_DataMgr * pDM)
{
	ShowSortDlg();
}

VOID CBO_CAT_REG_MAIN::CodeChange(CESL_ControlMgr *pCM, CESL_DataMgr *pDM, CESL_Grid *pGrid)
{
	INT ids = pGrid->SelectMakeList();
	if(ids >= 0) ids = pGrid->SelectGetHeadPosition();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return ;
	}

	CCodeChange dlg(this);
	dlg.SetDM(pDM);
	dlg.SetMainGrid(pGrid);
	dlg.SetAcqType(0);
	dlg.SetSpeciesYN(FALSE);

	//20070523 ADD PJW : INT nCurSel = pGrid->GetTopRow();
	//스크롤바 위치 고정 
	INT nCurSel = pGrid->GetTopRow();

	if(IDOK == dlg.DoModal())
	{
		/***************************************************************
		 ***************************************************************/
		//20071101 ADD PJW : 코드변경시 BS_주제구분 도 변경되도록 수정
		INT nRowCnt;
		nRowCnt = pDM->GetRowCount();
		for(INT i = 0 ; i < nRowCnt ; i++)
		{
			CString strSubjectCode;
			CString strChk;

			strChk = pGrid->GetTextMatrix(i+1, 1);
			if( strChk == _T("V") )
			{
				pDM->GetCellData(_T("BP_주제구분"), i, strSubjectCode);
				pDM->SetCellData(_T("BS_주제구분"), strSubjectCode, i);
			}
		}
		/***************************************************************
		 ***************************************************************/
		pGrid->SetProtectSelectColumn(TRUE);
		pGrid->Display();
		//20070523 ADD PJW : pGrid->SetTopRow(nCurSel);
		//스크롤바 위치 고정 
		pGrid->SetTopRow(nCurSel);
		
		pGrid->SetProtectSelectColumn(FALSE);
	}
}
//정종석추가===================================================================

INT CBO_CAT_REG_MAIN::SortRegData(CESL_Grid *pGrid)
{
	CESL_DataMgr *pDM = FindDM(_T("DM_단행_정리_등록_메인"));
	INT nCol = pGrid->GetCol();
	POSITION pos = pGrid->m_arrColumnInfo.FindIndex(nCol-1);
	CESL_GridColumnInfo *pColumnInfo = (CESL_GridColumnInfo*)pGrid->m_arrColumnInfo.GetAt(pos);
	CString strSortAlias = (CString)pColumnInfo->strColumnDMFieldAlias;
	
	if(m_nSortDirection == 0) m_nSortDirection = ESL_DATAMGR_SORT_ASC;
	else if(m_nSortDirection == ESL_DATAMGR_SORT_ASC) m_nSortDirection = ESL_DATAMGR_SORT_DESC;
	else if(m_nSortDirection == ESL_DATAMGR_SORT_DESC) m_nSortDirection = ESL_DATAMGR_SORT_ASC;

	INT ids = m_pSortItemSet->DataSortOfSelectedColumn(pDM, strSortAlias, m_nSortDirection);
	if(ids < 0) return ids;

	pGrid->Display();
	return 0;
}

VOID CBO_CAT_REG_MAIN::ShowGroupMgrFrame()
{
	m_Launcher.SetGroupOpenType(ACQ_GROUP_OPEN);
	m_Launcher.LaunchAcqGroupFrame(GetParentFrame(),this);
}

VOID CBO_CAT_REG_MAIN::ShowSearchDlg()
{
	if( m_pSearchDlg == NULL )		return;
	if( !m_pSearchDlg->GetCreate() )	m_pSearchDlg->Create(this);

	m_pSearchDlg->ShowWindow(SW_SHOW);
	m_pSearchDlg->UpdateWindow();
	m_pSearchDlg->CenterWindow();
}

VOID CBO_CAT_REG_MAIN::ShowSortDlg()
{
	CESL_DataMgr *pDM = FindDM(_T("DM_단행_정리_등록_메인"));

	if(m_pSortItemSet == NULL || pDM == NULL) 
	{
		ESLAfxMessageBox(_T("정렬화면을 초기화 할 수 없습니다."));
		return;
	}
	if(pDM->GetRowCount() == 0)
	{
		ESLAfxMessageBox(_T("정렬할 자료가 없습니다."));
		return ;
	}
	
	CDataSortDlg dlg(this);
	dlg.SetMainDM(pDM);
	dlg.SetSortItemSet(m_pSortItemSet);
	dlg.SetOpenMode(1);
	if(IDOK == dlg.DoModal())
		DisplayGrid(_T("CM_단행_정리_등록_메인"), _T("MAIN_GRID"));
}

BOOL CBO_CAT_REG_MAIN::InitSortItem()
{
	CStringArray * pTmpData = NULL;	
	CESL_Grid * pGrid = (CESL_Grid *)FindControl(_T("CM_단행_정리_등록_메인"), _T("MAIN_GRID"));
	CESL_DataMgr * pDM = FindDM(_T("DM_단행_정리_등록_메인"));

	m_pSortItemSet = new CSortItemSet(this);
	m_pSortItemSet->SetGrid(pGrid);
	m_pSortItemSet->InitDataManager(pDM->CONNECTION_INFO, _T("DM_정렬항목설정"));
	m_pSortItemSet->SetSortCfgFileName(GetSMAlias(), _T("MAIN_GRID"));
	pTmpData = m_pSortItemSet->GetHeadSortList();
	pTmpData->Add(_T("BB_수입구분"));
	pTmpData->Add(_T("BP_수입년도"));
	pTmpData->Add(_T("BP_그룹번호"));
	pTmpData = m_pSortItemSet->GetTailSortList();
	pTmpData->Add(_T("UDF_일련번호"));
	pTmpData->Add(_T("BB_일련번호"));
	
	CMapStringToString * pMap = m_pSortItemSet->GetConvertUDFToRealAliasMap();
	pMap->SetAt(_T("BB_자료상태_설명"), _T("BB_자료상태_코드"));
	
	m_pSortItemSet->ReadSortItem();
	m_pSearchDlg->SetSortItemSet(m_pSortItemSet);
	return TRUE;
}

INT CBO_CAT_REG_MAIN::SetRegDataInfo()
{
	CString strMsg;
	INT nRowCnt, nSpeciesCnt;
	CESL_DataMgr *pDM = FindDM(_T("DM_단행_정리_등록_메인"));
	if(pDM == NULL) return -1;

	nRowCnt = pDM->GetRowCount();
	if(nRowCnt == 0)
	{
		strMsg.Format(_T("검색된 등록인계/등록자료가 없습니다."));
		return 0;
	}

	CMap <CString ,LPCTSTR,CString,LPCTSTR> mapRecKey;
	CString strSpecies_key;
	for(INT i =0 ; i<nRowCnt;i++)
	{
        strSpecies_key=_T("") ;
		pDM->GetCellData (_T("IBS_종KEY"),i,strSpecies_key);
		if(!strSpecies_key.IsEmpty ())
        { 
			mapRecKey.SetAt(strSpecies_key,strSpecies_key);
		}
	}

	nSpeciesCnt = mapRecKey.GetCount();
	strMsg.Format(_T("종수 : %5d\t책수 : %5d"), nSpeciesCnt, nRowCnt);
	
	static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_USER+2004, NULL, (long)strMsg.GetBuffer(0));
	return 0;
}

VOID CBO_CAT_REG_MAIN::ChangeAcqInfo()
{
	CESL_DataMgr *pDM = FindDM(_T("DM_단행_정리_등록_메인"));
	CESL_Grid * pGrid = (CESL_Grid *)FindControl(_T("CM_단행_정리_등록_메인"), _T("MAIN_GRID"));
	INT ids = pGrid->SelectMakeList();
	if(ids >= 0) ids = pGrid->SelectGetHeadPosition();
	if(ids < 0 || pDM->GetRowCount() == 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return ;
	}
	
	CAcqInfoChange dlg(this);
	dlg.SetDM(pDM);
	dlg.SetMainGrid(pGrid);
	if(IDOK == dlg.DoModal())
	{
		pGrid->SetProtectSelectColumn(TRUE);
		DisplayGrid(_T("CM_단행_정리_등록_메인"), _T("MAIN_GRID"));
		pGrid->SetProtectSelectColumn(FALSE);
	}	
}

VOID CBO_CAT_REG_MAIN::InitPopupMenuItem()
{
	CESL_Grid * pGrid = (CESL_Grid *)FindControl(_T("CM_단행_정리_등록_메인"), _T("MAIN_GRID"));
	CArray <CString, CString> RemoveListArray;
	RemoveListArray.RemoveAll();
	pGrid->InitPopupMenuItem();
	pGrid->AddPopupMenuItem((CView*)pMain, VIEW_STYLE, &RemoveListArray);	
}

VOID CBO_CAT_REG_MAIN::ShowChangeDonateInfoDlg()
{
	CESL_DataMgr* pDM = FindDM(_T("DM_단행_정리_등록_메인"));
	CESL_Grid* pGrid = (CESL_Grid *)FindControl(_T("CM_단행_정리_등록_메인"), _T("MAIN_GRID"));

	INT ids = pGrid->SelectMakeList();
	if( ids >= 0 )	ids = pGrid->SelectGetHeadPosition();
	if( ids < 0 || pDM->GetRowCount() == 0 )
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return ;
	}

	CDonateInfoChange dlg(this);
	dlg.SetDM(pDM);	
	dlg.SetMainGrid(pGrid);
	if( IDOK == dlg.DoModal() )
	{
		pGrid->SetProtectSelectColumn(TRUE);
		DisplayGrid(_T("CM_단행_정리_등록_메인"), _T("MAIN_GRID"));
		pGrid->SetProtectSelectColumn(FALSE);
	}
}

VOID CBO_CAT_REG_MAIN::ChangeMarc()
{
	//=================================================
	//2008.07.02 UPDATE BY PJW : 대표책의정보도 변경한다.
	CESL_DataMgr *pDM = FindDM(_T("DM_단행_정리_등록_메인"));
	CESL_Grid *pGrid = (CESL_Grid*)FindControl(_T("CM_단행_정리_등록_메인"), _T("MAIN_GRID"));
	INT nSelectCnt;
	INT nSelectIdx;
	INT nDmRowCnt;
	CString strSpeciesKey;
	CString strSelectSpeciesKey;

	pGrid->SelectMakeList();
	nSelectCnt = pGrid->SelectGetCount();
	nSelectIdx = pGrid->SelectGetHeadPosition();

	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		pDM->GetCellData(_T("IBS_종KEY"), nSelectIdx, strSelectSpeciesKey); 
		nDmRowCnt = pDM->GetRowCount();
		for( INT j = 0 ; j < nDmRowCnt ; j++ )
		{
			 pDM->GetCellData(_T("IBS_종KEY"), j, strSpeciesKey); 
			 if( strSelectSpeciesKey == strSpeciesKey )
			 {
				 pGrid->SetTextMatrix(j+1, 1, _T("V"));
			 }

		}
		nSelectIdx = pGrid->SelectGetNext();
	}
	pGrid->SelectMakeList();
	pGrid->SetProtectSelectColumn(TRUE);
	//=======================================================================

	// 종단위, 권단위 처리 필요
	CMARC_CHANGER_01 dlg(this);
	//=================================================
	//2008.07.02 UPDATE BY PJW : 대표책의정보도 변경한다.
	//dlg.SetParentMgr(_T("IBS_종KEY"), _T("CM_단행_정리_등록_메인"), _T("MAIN_GRID"), _T("DM_단행_정리_등록_메인"), _T("BS_MARC"));
	dlg.SetParentMgr(_T("IBS_종KEY"), _T("CM_단행_정리_등록_메인"), _T("MAIN_GRID"), _T("DM_단행_정리_등록_메인"), _T("BS_MARC"),_T("DM_단행_정리_등록_메인"));
	//=================================================
	
	
	//===============================================
	//2008.08.22 UPDATE BY PJW : 그리드를 디스플레이한다.
// 	if ( dlg.DoModal() == IDOK )
//	{
//		AllControlDisplay(_T("CM_단행_정리_등록_메인"));
//	}
	if ( dlg.DoModal() == IDOK )
	{
		AllControlDisplay(_T("CM_단행_정리_등록_메인"));
	}
	pGrid->Display();

	//===============================================
	
	pGrid->SetProtectSelectColumn(FALSE);
}
HBRUSH CBO_CAT_REG_MAIN::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}

VOID CBO_CAT_REG_MAIN::KSearch(CString WorkType, CString strAcqCode, CString strAcqYear, CString strWorkCode, CString strSeqNo)
{
	CString strQuery;
	CESL_DataMgr *pDM = FindDM(_T("DM_단행_정리_등록_메인"));

	strQuery.Format(_T("B.WORKING_STATUS >= 'BOR111N' AND B.WORKING_STATUS <= 'BOR112N' AND B.BOOK_APPENDIX_FLAG = 'B'  AND B.ACQ_YEAR = '%s' AND B.ACQ_KEY IN (SELECT REC_KEY FROM BO_PURCHASEINFO_TBL WHERE ORDER_SEQ_NO IN (%s)) AND I.WORK_CODE = '%s' AND B.ACQ_CODE = '%s' AND I.WORK_CODE = '%s'"),
		strAcqYear, strSeqNo, strWorkCode, strAcqCode, strWorkCode);

	AfxGetMainWnd()->SendMessage(WM_USER+3000,0,0);
	pDM->RefreshDataManager(strQuery);
	AfxGetMainWnd()->SendMessage(WM_USER+3001,pDM->GetRowCount());
	AllControlDisplay(_T("CM_단행_정리_등록_메인"));

	m_pSearchDlg->GetGroupCollection()->RemoveAll();	

	SetControlData(_T("CM_단행_정리_등록_메인"), _T("차수번호")	, strSeqNo);
	SetControlData(_T("CM_단행_정리_등록_메인"), _T("년도")		, strAcqYear);
	m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("입수구분"), strAcqCode, strAcqCode);
	SetControlData(_T("CM_단행_정리_등록_메인"), _T("입수구분")	, strAcqCode);



}