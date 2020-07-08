// MARC_IMPORT.cpp : implementation file
//

#include "stdafx.h"
#include "MARC_IMPORT.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#include "MARC_VIEW.h"
/////////////////////////////////////////////////////////////////////////////
// CMARC_IMPORT dialog


CMARC_IMPORT::CMARC_IMPORT(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMARC_IMPORT)
	m_nRadBasicInfo = 1;
	//}}AFX_DATA_INIT
	m_nCurrentGridRow = -1;
	m_strTitle = _T("");
	m_strAuthor = _T("");
	m_strPublisher = _T("");
	m_strPubYear = _T("");
}

CMARC_IMPORT::~CMARC_IMPORT()
{
}

VOID CMARC_IMPORT::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMARC_IMPORT)
	DDX_Radio(pDX, IDC_radMARC_BASIC_INFO, m_nRadBasicInfo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMARC_IMPORT, CDialog)
	//{{AFX_MSG_MAP(CMARC_IMPORT)
	ON_BN_CLICKED(IDC_bSEARCH, OnbSEARCH)
	ON_BN_CLICKED(IDC_bMARC, OnbMARC)
	ON_BN_CLICKED(IDC_bCLEAR, OnbCLEAR)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMARC_IMPORT message handlers

INT CMARC_IMPORT::InitReference()
{
	if(InitESL_Mgr(_T("단행_마크반입")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return -1;
	}	 

	//관련 DM, CM, GRID init;
 	m_pDM = FindDM(_T("DM_단행_마크반입"));
    if (!m_pDM) {
		AfxMessageBox(_T("[마크반입] DM을 찾을 수 없습니다."));
		return -2;
    }	
 
	m_pCM = FindCM(_T("CM_단행_마크반입"));
	if (!m_pCM) {
		AfxMessageBox(_T("[마크반입] CM을 찾을 수 없습니다."));
		return -3;
	}
	  
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("마크반입그리드"));
    if (!m_pGrid) {
		AfxMessageBox(_T("해당 GRID를 찾을 수 없습니다."));
		return -4;
	} 
	m_pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;

	return 0;
}

BOOL CMARC_IMPORT::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if(InitReference()<0)  return FALSE;
	SetControlAlias();
	DisplayControls();
	( (CEdit*)GetDlgItem(IDC_edtMARC_TITLE) ) -> SetWindowText( m_strTitle );
	( (CEdit*)GetDlgItem(IDC_edtMARC_AUTHOR) ) -> SetWindowText( m_strAuthor );
	( (CEdit*)GetDlgItem(IDC_edtMARC_PUBLISHER) ) -> SetWindowText( m_strPublisher );
	( (CEdit*)GetDlgItem(IDC_edtMARC_PUBLICATE_YEAR_START) ) -> SetWindowText( m_strPubYear );

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CMARC_IMPORT::SetControlAlias()
{
   
	m_nControlAlias = 0;
	m_strControlAlias[m_nControlAlias++] = _T("IBS_본표제");
  	m_strControlAlias[m_nControlAlias++] = _T("IBS_저작자");
	m_strControlAlias[m_nControlAlias++] = _T("IBS_발행자");
	m_strControlAlias[m_nControlAlias++] = _T("IBS_키워드");
	
	//현재  [4] 부터는 코드   
	m_strControlAlias[m_nControlAlias++] = _T("IBS_자료구분_코드");
    m_strControlAlias[m_nControlAlias++] = _T("IBS_형식구분_코드");
    m_strControlAlias[m_nControlAlias++] = _T("IBS_매체구분_코드");

	
	INT nCount = 0;
	m_strFieldName[nCount++] = _T("IDX_TITLE");
	m_strFieldName[nCount++] = _T("IDX_AUTHOR");
	m_strFieldName[nCount++] = _T("IDX_PUBLISHER");
	m_strFieldName[nCount++] = _T("IDX_KEYWORD");	
	
	
	m_strFieldName[nCount++] = _T("MAT_CODE");
	m_strFieldName[nCount++] = _T("FORM_CODE");
	m_strFieldName[nCount++] = _T("MEDIA_CODE");

	
    return 0;
}

CString CMARC_IMPORT::MakeRangeQuery(CString strFieldName, CString strData1, CString strData2)
{
	CString tmpquery, strRegCode;	
	INT nOption, nRegNo;
	
	if(!strData1.IsEmpty() && strFieldName == _T("REG_NO") )
	{
		if(strData1.GetLength() < 3) strData1.Empty();
		else
		{
			//===================================================================
			//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 			strRegCode = strData1.Left(2);
// 			strData1.Replace(strRegCode, _T(""));
// 			nRegNo = _ttoi(strData1.GetBuffer(0));
// 			strData1.Format(_T("%s%010d"), strRegCode, nRegNo);
			INT nRegCodeLength;
			nRegCodeLength = m_pInfo->nREG_CODE_LENGTH;

			strRegCode = strData1.Left(nRegCodeLength);
			strData1.Replace(strRegCode, _T(""));
			strData1 = m_pInfo->MakeRegNo(strRegCode, strData1);
			//===================================================================
		}
	}
	if(!strData2.IsEmpty() && strFieldName == _T("REG_NO"))
	{
		if(strData2.GetLength() < 3) strData2.Empty();
		else
		{
			strRegCode = strData2.Left(2);
			strData2.Replace(strRegCode, _T(""));
			nRegNo = _ttoi(strData2.GetBuffer(0));
			strData2.Format(_T("%s%010d"), strRegCode, nRegNo);
		}
	}


	if(!strData1.IsEmpty() && strData2.IsEmpty()) nOption = 1;	
	else if(!strData1.IsEmpty() && !strData2.IsEmpty()) nOption = 2;
	else if(strData1.IsEmpty() && !strData2.IsEmpty())
	{
		ESLAfxMessageBox(_T("시작정보가 올바르지 않습니다."));
		return _T("-1");
	}

	if(nOption != 0)
	{
		if(nOption == 1) tmpquery.Format(_T("%s = '%s'"),strFieldName,strData1);
		if(nOption == 2) tmpquery.Format(_T("%s BETWEEN '%s' AND '%s'"),strFieldName,strData1,strData2);
	}

	return tmpquery;
}

INT CMARC_IMPORT::GetECOSEARCHQuery(CStringArray * pArrECOSearch, CString &strECOSEARCHQuery)
{
	CESL_DataMgr TmpDM;
	
	INT nCnt = pArrECOSearch->GetSize();
	INT ids;
	CString strTmpData;
	strECOSEARCHQuery.Empty();
	for(INT i=0 ; i<nCnt ; i++)
	{
		strTmpData = pArrECOSearch->GetAt(i);
		strTmpData.MakeUpper();
		strTmpData = strTmpData.Mid(4);
		strTmpData.Replace(_T("REC_KEY"), _T(""));
		strTmpData.TrimLeft();	strTmpData.TrimRight();
		ids = TmpDM.GetECOSEARCHReplace(strTmpData);
		if(strECOSEARCHQuery.IsEmpty()) 
			strECOSEARCHQuery = strTmpData;
		else
			strECOSEARCHQuery += _T(" INTERSECT ") + strTmpData;
	}
	return 0;
}

VOID CMARC_IMPORT::OnbSEARCH() 
{
	// TODO: Add your control notification handler code here

	// 2004.09.15 검색항목체크 : 아무것도 입력하지 않고 검색하면 시간이 너무 오래걸림. 
	CString sTitle, sKeyword, sAuthor, sPublisher, sClassCode1, sClassCode2;
	CString sRegNo1, sRegNo2, sPublicateYearStart, sPublicateYearEnd, sSubject;

	GetDlgItem(IDC_edtMARC_TITLE)->GetWindowText(sTitle);
	GetDlgItem(IDC_edtMARC_KEYWORD)->GetWindowText(sKeyword);
	GetDlgItem(IDC_edtMARC_AUTHOR)->GetWindowText(sAuthor);
	GetDlgItem(IDC_edtMARC_PUBLISHER)->GetWindowText(sPublisher);
	GetDlgItem(IDC_edtMARC_CLASS_CODE1)->GetWindowText(sClassCode1);
	GetDlgItem(IDC_edtMARC_CLASS_CODE2)->GetWindowText(sClassCode2);
	GetDlgItem(IDC_edt_MARC_REG_NO1)->GetWindowText(sRegNo1);
	GetDlgItem(IDC_edt_MARC_REG_NO2)->GetWindowText(sRegNo2);
	GetDlgItem(IDC_edtMARC_PUBLICATE_YEAR_START)->GetWindowText(sPublicateYearStart);
	GetDlgItem(IDC_edtMARC_PUBLICATE_YEAR_END)->GetWindowText(sPublicateYearEnd);
	GetDlgItem(IDC_eSUBJECT)->GetWindowText(sSubject);
	
	
	if ( sTitle == _T("") && sKeyword == _T("") && sAuthor == _T("") && sPublisher == _T("") ) {
		if ( sClassCode1 == _T("") && sClassCode2 == _T("") && sRegNo1 == _T("") && sRegNo2 == _T("")) {
			if ( sPublicateYearStart == _T("") && sPublicateYearEnd ==_T("") && sSubject == _T("") ) {
				AfxMessageBox( _T("검색 조건이 부족합니다.") );
				return;
			}
		}
	}

	// 각 콘트롤 항목들에 맞는 query condition을 만든다 
	CString query,tmpquery;
    CString tmpdata;
    INT nCondition= 1;
	
	query = _T("SPECIES_CLASS IN ('0', '1')");
	//현재는 [4]까지만 CATSEARCH사용 
	CStringArray ArrECOSearch;	ArrECOSearch.RemoveAll();

	for(INT i=0;i<4;i++) 
	{
		tmpdata.Empty();
		tmpquery.Empty();
		
		m_pCM->GetControlMgrData(m_strControlAlias[i],tmpdata);
		((CIndexTool*)m_pInfo->pIndexTool)->GetIndex()->MakeIndex(tmpdata);	
		((CIndexTool*)m_pInfo->pIndexTool)->GetIndex()->ConvertHanjaToHangul(tmpdata);
		if(!tmpdata.IsEmpty()) 
		{
			if(m_pInfo->GetSearchIndexType() == ORA_CATSEARCH)
			{
				tmpquery.Format(_T(" AND CATSEARCH(%s,'%s*',NULL)>0 "),m_strFieldName[i],tmpdata);
				query += tmpquery;
			}
			else if(m_pInfo->GetSearchIndexType() == ECO_SEARCH)
			{
				tmpquery.Format(_T(" AND &ECOSEARCH(REC_KEY, %s, '%s*') "), _T("BO_") + m_strFieldName[i], tmpdata);		
				ArrECOSearch.Add(tmpquery);
			}
					 
			nCondition++;
		}
	}

	if(m_pInfo->GetSearchIndexType() == ECO_SEARCH)
	{
		GetECOSEARCHQuery(&ArrECOSearch, tmpdata);
		if(!tmpdata.IsEmpty())
			query += _T(" AND REC_KEY IN ( ") + tmpdata + _T(")");
	}

	//여러 코드 값들의 SEARCH
	for(i=4;i<m_nControlAlias; i++) {
		tmpdata.Empty();
		tmpquery.Empty();

		m_pCM->GetControlMgrData(m_strControlAlias[i],tmpdata);
        
		if(!tmpdata.IsEmpty()) {

		if(nCondition>0) query+= _T(" AND "); 
		tmpquery.Format(_T("%s = '%s'"),m_strFieldName[i],tmpdata);
        query += tmpquery;
		         
		nCondition++;
		}
	}
	
	//발행년도 검색 
	tmpdata.Empty();
	tmpquery.Empty();
	CString tmpdata2;
	m_pCM->GetControlMgrData(_T("IBS_발행년도1"),tmpdata);
	m_pCM->GetControlMgrData(_T("IBS_발행년도2"),tmpdata2);
		
	tmpquery = MakeRangeQuery(_T("IDX_IPUB_YEAR"),tmpdata,tmpdata2);
	
	if(!tmpquery.IsEmpty() && tmpquery != _T("-1")) 
	{
		if(nCondition>0) query+= _T(" AND ");
		query += tmpquery;
		nCondition++;
	}

	//주제어 검색..
	CString strSubject;
	m_pCM->GetControlMgrData(_T("IBS_주제어"),strSubject);
	if ( !strSubject.IsEmpty() )
	{
		CString sWhereTitle;
		if ( m_pInfo->GetSearchIndexType() == ORA_CATSEARCH ) {
			sWhereTitle.Format( _T("CATSEARCH(IDX_SUBJECT,'%s*',NULL)>0") , strSubject );
		}
		else if ( m_pInfo->GetSearchIndexType() == ECO_SEARCH ) {
			sWhereTitle.Format( _T("&ECOSEARCH(REC_KEY, BO_IDX_SUBJECT, '%s*')"), strSubject );
		}
		else if ( m_pInfo->GetSearchIndexType() == MNG_INVALID ) {
			AfxMessageBox( _T("SearchIndexType이 설정되지 않았습니다.") );
			return;
		}
		query = query + _T("AND ") + sWhereTitle ;
	}


	//분류기호 검색 
	CString strCLASS_CODE;
	m_pCM->GetControlMgrData(_T("IBS_분류표구분_코드"),strCLASS_CODE);
	tmpdata.Empty();
	tmpdata2.Empty();
	tmpquery.Empty();
	m_pCM->GetControlMgrData(_T("IBS_분류기호1"),tmpdata);
	m_pCM->GetControlMgrData(_T("IBS_분류기호2"),tmpdata2);

	if(_T("1") == strCLASS_CODE)
	{
		tmpquery = MakeRangeQuery(_T("KDC_CLASS"),tmpdata,tmpdata2);
		if(!tmpquery.IsEmpty() && tmpquery != _T("-1")) 
		{
			if(nCondition>0) query+= _T(" AND ");
			query += tmpquery;
			nCondition++;
		}
	} 
	else if(_T("2") == strCLASS_CODE)
	{
		tmpquery = MakeRangeQuery(_T("DDC_CLASS"),tmpdata,tmpdata2);
		if(!tmpquery.IsEmpty() && tmpquery != _T("-1")) 
		{
			if(nCondition>0) query+= _T(" AND ");
			query += tmpquery;
			nCondition++;
		}
	}
	
	//등록번호검색
	tmpdata.Empty();
	tmpdata2.Empty();
	tmpquery.Empty();
	m_pCM->GetControlMgrData(_T("IBS_등록번호1"),tmpdata);
	m_pCM->GetControlMgrData(_T("IBS_등록번호2"),tmpdata2);

	CString strINquery = _T("");
	tmpquery = MakeRangeQuery(_T("REG_NO"),tmpdata,tmpdata2);
	if(!tmpquery.IsEmpty() && tmpquery != _T("-1"))
	{
		if(nCondition>0) query+= _T(" AND ");
		strINquery.Format(_T("REC_KEY IN (SELECT SPECIES_KEY FROM BO_BOOK_TBL WHERE %s)"),tmpquery);
		query += strINquery;
		nCondition++;
	}

	
    CProgressCtrl* pProgressCtrl;
	pProgressCtrl = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS);
	INT ids = m_pDM->RefreshDataManager(query,pProgressCtrl);
	pProgressCtrl->SetPos(100);

    if(m_pDM->GetRowCount()>0) m_nCurrentGridRow = 1;
	m_pGrid->Display();

}

VOID CMARC_IMPORT::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	if(m_nCurrentGridRow<0) {
		AfxMessageBox(_T("그리드에 선택된 항목이 없습니다!"));
		return;
	}
	
	m_strSpecies_key = m_pDM->GetCellData(_T("IBS_종KEY"),m_nCurrentGridRow-1);

	CDialog::OnOK();
}

VOID CMARC_IMPORT::OnbMARC() 
{
	//===============================================
	//20080602 UPDATE BY PJW : 그리드에 선정값이 없을경우 리턴한다.
	m_pGrid->SelectMakeList();
	if ( m_pGrid->SelectGetCount() <= 0 )
	{
		AfxMessageBox(_T("그리드에 선택된 항목이 없습니다!"));
		return;
	}
	
	//if(m_nCurrentGridRow<0) {
	//	AfxMessageBox(_T("그리드에 선택된 항목이 없습니다!"));
	//	return;
	//}
	//===============================================
	

	CMARC_VIEW dlg(this);
	dlg.SetParentDM(m_pDM);
	dlg.SetParentGrid(m_pGrid);
    
	dlg.DoModal();

}

BEGIN_EVENTSINK_MAP(CMARC_IMPORT, CDialog)
    //{{AFX_EVENTSINK_MAP(CMARC_IMPORT)
	ON_EVENT(CMARC_IMPORT, IDC_gridMARC_IMPORT, -605 /* MouseDown */, OnMouseDowngridMARCIMPORT, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CMARC_IMPORT::OnMouseDowngridMARCIMPORT(short Button, short Shift, long x, long y) 
{
	// TODO: Add your control notification handler code here
	INT row = m_pGrid->GetRow();
	if (row == 0) return;
	if (row > 0) {
		/// 2008.03.20 rem by pdw
		/// 마크반입 창에서 몇 개 그리드해서 선정처리하면 맨 위에 항목만
        /// 선정처리됨. >> 관련 제거
		//      m_pGrid->SetReverse(row-1);
		m_nCurrentGridRow = row;
	 }
}

VOID CMARC_IMPORT::DisplayControls()
{
	//이거는 도움말버튼을 숨기기위한 거야
	GetDlgItem(IDC_bHELP)->ShowWindow(SW_HIDE);
	
	CButton * pDbDelete;
	pDbDelete=(CButton *)GetDlgItem(IDC_radMARC_ALL_INFO);
	pDbDelete->SetCheck(1);
}

BOOL CMARC_IMPORT::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
 	if (pMsg->message == WM_KEYDOWN && VK_RETURN == pMsg->wParam ) 
	{
		PostMessage(WM_COMMAND, IDC_bSEARCH, BN_CLICKED);
		return TRUE;
	}	
	return CDialog::PreTranslateMessage(pMsg);
}

VOID CMARC_IMPORT::OnbCLEAR() 
{
	// TODO: Add your control notification handler code here
	m_pCM->AllControlClear();
}

CString CMARC_IMPORT::MakeCallNumber(CESL_DataMgr *pDM, INT nIdx)
{
	CString strCallNo;
	CString strItem;
	pDM->GetCellData(_T("IBS_별치기호"), nIdx, strItem);
	if (!strItem.IsEmpty())
	{
		// 별치기호는 설명으로 바꾼다.
		m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("별치기호"), strItem, strItem);
	}
	strCallNo = strItem;
	
	pDM->GetCellData(_T("IBS_분류기호"), nIdx, strItem);
	if (!strItem.IsEmpty())
	{
		if (strCallNo.IsEmpty())
			strCallNo = strItem;
		else
			strCallNo += _T(" ") + strItem;
	}
	else return _T("");
	
	pDM->GetCellData(_T("IBS_도서기호"), nIdx, strItem);
	if (!strItem.IsEmpty())
	{
		if (strCallNo.IsEmpty())
			strCallNo = strItem;
		else
			strCallNo += _T("-") + strItem;
	}
	
	pDM->GetCellData(_T("IBS_권책기호"), nIdx, strItem);	
	if (!strItem.IsEmpty())
	{
		if (strCallNo.IsEmpty())
			strCallNo = strItem;
		else
			strCallNo += _T("-") + strItem;
	}
	
	// 청구기호
	pDM->SetCellData(_T("UDF_청구기호"), strCallNo, nIdx);
	return strCallNo;
}

INT CMARC_IMPORT::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
	CString strTmpData, strCallNumber, strRegNo, strAcqInfo;
	
	if(strDMFieldAlias == _T("UDF_청구기호"))
	{
		strTmpData = MakeCallNumber(m_pDM, nRow);
		str = strTmpData;
	}
	return 0;
}

INT CMARC_IMPORT::SetSearchCondition(CString strTitle, CString strAuthor, CString strPublisher, CString strPubYear)
{
	m_strTitle		= strTitle;
	m_strAuthor		= strAuthor;
	m_strPublisher	= strPublisher;
	m_strPubYear	= strPubYear;	

	return 0;
}

HBRUSH CMARC_IMPORT::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
