// SePurchaseMail.cpp : implementation file
//

#include "stdafx.h"
#include "SePurchaseMail.h"
#include "..\SE_API\SePurchaseNoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSePurchaseMail dialog


CSePurchaseMail::CSePurchaseMail(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSePurchaseMail)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDM = NULL;
	m_pDM_SEQ_NO = NULL;
	m_pCM = NULL;
	m_pGrid = NULL;
	m_pDM_ALL = NULL;
	
	m_bAutoMode = FALSE;
	m_strSEQ_NO1 = _T("");
	m_strSEQ_NO2 = _T("");
	m_strOrderSeqYear = _T("");
}

CSePurchaseMail::~CSePurchaseMail()
{
}

VOID CSePurchaseMail::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSePurchaseMail)
	DDX_Control(pDX, IDC_cmbPUR_MAIL_PROVIDER, m_ctrlProviderCombo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSePurchaseMail, CDialog)
	//{{AFX_MSG_MAP(CSePurchaseMail)
	ON_EN_KILLFOCUS(IDC_edtPUR_MAIL_SEQ_NO, OnKillfocusedtPURMAILSEQNO)
	ON_BN_CLICKED(IDC_btnPUR_MAIL_SEQ_NO, OnbtnPURMAILSEQNO)
	ON_CBN_SELCHANGE(IDC_cmbPUR_MAIL_PROVIDER, OnSelchangecmbPURMAILPROVIDER)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSePurchaseMail message handlers

BOOL CSePurchaseMail::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(InitESL_Mgr( _T("연속_수서_구입자료관리_메일") )<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : 연속_수서_구입자료관리_메일") );
		return FALSE;
	}
	
	m_pDM = FindDM(_T("DM_연속_구입자료관리_메일"));
	m_pDM_ALL = FindDM(_T("DM_연속_구입자료관리_메일_전체"));
	m_pDM_SEQ_NO = FindDM(_T("DM_연속_구입자료관리_메일_차수번호")); 
	m_pCM = FindCM(_T("CM_연속_구입자료관리_메일"));
	if(m_pCM) m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("그리드"));
	

	if(!m_pDM || !m_pCM || !m_pDM_SEQ_NO || !m_pGrid || !m_pDM_ALL) {
		AfxMessageBox( _T("DM,CM Error : 연속_수서_구입자료관리_메일") );
		return FALSE;
	}
	
	if(m_bAutoMode) 
	{
		if(!IsUseAutoMailOption()) {
			EndDialog(0);
			return FALSE;
		}

		if(m_strOrderSeqYear.IsEmpty() || m_strSEQ_NO1.IsEmpty()) 
		{ 
			AfxMessageBox(_T("차수년도 또는 주문차수가 입력되지 않았습니다"));
			return FALSE;
		}

		InitAutoMode();
		ShowAutoModeData();
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CSePurchaseMail::IsUseAutoMailOption()
{
	CString strQuery;
	CString strResult;
	strQuery = _T("SELECT VALUE_1 FROM ESL_MANAGE_TBL ") 
			   _T("WHERE GROUP_1 = 'L' AND GROUP_2 = '수서' AND GROUP_3 = '연속' ") 
			   _T("AND CLASS_ALIAS = '주문메일자동발송'")
			   _T("AND MANAGE_CODE = UDF_MANAGE_CODE");
	
	m_pDM->GetOneValueQuery(strQuery,strResult);
	
	if(_T("Y") == strResult) return TRUE;

	return FALSE;
}

INT CSePurchaseMail::InitAutoMode()
{
	CString strSeqNo;
	if(!m_strSEQ_NO2.IsEmpty())
	{
		strSeqNo.Format(_T("%s-%s"),m_strSEQ_NO1,m_strSEQ_NO2);
	}
	else 
	{

		strSeqNo = m_strSEQ_NO1;
	}

	m_pCM->SetControlMgrData(_T("차수년도"),m_strOrderSeqYear);
	m_pCM->SetControlMgrData(_T("차수번호"),strSeqNo);
	

	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_edtPUR_MAIL_SEQ_NO);
	if(pEdit) pEdit->SetReadOnly();
	GetDlgItem(IDC_btnPUR_MAIL_SEQ_NO)->ShowWindow(FALSE);

	return 0;
}

INT CSePurchaseMail::ShowAutoModeData()
{
	
	CString strQuery;
	CString strTempQuery;
	
	strTempQuery.Format(_T("ORDER_YEAR = '%s'"),m_strOrderSeqYear);
	strQuery += strTempQuery;

	if(m_strSEQ_NO2.IsEmpty())
	{
		strTempQuery.Format(_T("AND ORDER_SEQ_NO = %s"),m_strSEQ_NO1);
	}
	else
	{
		strTempQuery.Format(_T("AND ORDER_SEQ_NO BETWEEN %s AND %s"),m_strSEQ_NO1,m_strSEQ_NO2);
	}

	strQuery += strTempQuery;

	m_pDM_ALL->RefreshDataManager(strQuery);

	INT nRowCnt = m_pDM_ALL->GetRowCount();

	if(0 == nRowCnt)
	{
		AfxMessageBox(_T("해당차수내에 속하는 종정보가 없습니다!"));
		return 0;
	}
	
	//본서명,발행자,저작자를 마크에서 추출해 넣어 준다
	CMarc Marc;
	CString strStreamMarc = _T("");
	CString strTitle,strPublisher,strAuthor,strProvider;
	m_pInfo->pMarcMgr->SetMarcGroup(_T("통합마크"));
	

	for(INT i=0;i<nRowCnt;i++)
	{
		strStreamMarc = m_pDM_ALL->GetCellData(_T("MARC"),i);
		
		m_pInfo->pMarcMgr->Decoding(strStreamMarc,&Marc);
		
		strTitle = _T("");
		strPublisher = _T("");
		strAuthor = _T("");

		m_pInfo->pMarcMgr->GetItem(&Marc,_T("245$a"),strTitle);
		m_pInfo->pMarcMgr->GetItem(&Marc,_T("260$b"),strPublisher);
		m_pInfo->pMarcMgr->GetItem(&Marc,_T("245$d"),strAuthor);

		m_pDM_ALL->SetCellData(_T("UDF_본서명"),strTitle,i);
		m_pDM_ALL->SetCellData(_T("UDF_발행자"),strPublisher,i);
		m_pDM_ALL->SetCellData(_T("UDF_저작자"),strAuthor,i);
		
		strProvider = _T("");
		strProvider = m_pDM_ALL->GetCellData(_T("구입처_코드"),i);
		m_pDM_ALL->SetCellData(_T("UDF_구입처"),strProvider,i);

	}
	
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM_ALL,_T("구입처관리"),_T("UDF_구입처"));
	
	m_pDM->FreeData();
	INT nIdx = -1;

	for(i=0;i<nRowCnt;i++)
	{
		m_pDM->InsertRow(-1);
		nIdx = m_pDM->GetRowCount()-1;
		CopyDMToDMByAlias(m_pDM_ALL,i,m_pDM,nIdx);
	}


	SetProviderCombo();
	SetCountPriceInfo();
	m_pGrid->Display();


	return 0;
}

VOID CSePurchaseMail::OnKillfocusedtPURMAILSEQNO() 
{
	// TODO: Add your control notification handler code here
		
	if(m_bAutoMode) return;

	//차수번호를 입력하는 경우 검색 결과를 그리드에 보여준다.
	ShowGridData();
	
}

VOID CSePurchaseMail::ShowGridData()
{
	
	CString strYear;
	CString strNo;

	m_pCM->GetControlMgrData(_T("차수년도"),strYear);
	m_pCM->GetControlMgrData(_T("차수번호"),strNo);

	if(strNo.IsEmpty()) return;
	
	CString strQuery;
	strQuery.Format(_T("ORDER_YEAR = '%s' AND ORDER_SEQ_NO = %s"),strYear,strNo);
	
	m_pDM_ALL->RefreshDataManager(strQuery);

	INT nRowCnt = m_pDM_ALL->GetRowCount();

	if(0 == nRowCnt)
	{
		AfxMessageBox(_T("해당차수내에 속하는 종정보가 없습니다!"));
		return;
	}
	
	//본서명,발행자,저작자를 마크에서 추출해 넣어 준다
	CMarc Marc;
	CString strStreamMarc = _T("");
	CString strTitle,strPublisher,strAuthor,strProvider;
	m_pInfo->pMarcMgr->SetMarcGroup(_T("통합마크"));
	

	for(INT i=0;i<nRowCnt;i++)
	{
		strStreamMarc = m_pDM_ALL->GetCellData(_T("MARC"),i);
		
		m_pInfo->pMarcMgr->Decoding(strStreamMarc,&Marc);
		
		strTitle = _T("");
		strPublisher = _T("");
		strAuthor = _T("");

		m_pInfo->pMarcMgr->GetItem(&Marc,_T("245$a"),strTitle);
		m_pInfo->pMarcMgr->GetItem(&Marc,_T("260$b"),strPublisher);
		m_pInfo->pMarcMgr->GetItem(&Marc,_T("245$d"),strAuthor);

		m_pDM_ALL->SetCellData(_T("UDF_본서명"),strTitle,i);
		m_pDM_ALL->SetCellData(_T("UDF_발행자"),strPublisher,i);
		m_pDM_ALL->SetCellData(_T("UDF_저작자"),strAuthor,i);
		
		strProvider = _T("");
		strProvider = m_pDM_ALL->GetCellData(_T("구입처_코드"),i);
		m_pDM_ALL->SetCellData(_T("UDF_구입처"),strProvider,i);

	}
	
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM_ALL,_T("구입처관리"),_T("UDF_구입처"));
	
	m_pDM->FreeData();
	INT nIdx = -1;

	for(i=0;i<nRowCnt;i++)
	{
		m_pDM->InsertRow(-1);
		nIdx = m_pDM->GetRowCount()-1;
		CopyDMToDMByAlias(m_pDM_ALL,i,m_pDM,nIdx);
	}


	SetProviderCombo();
	SetCountPriceInfo();
	m_pGrid->Display();
	
}

VOID CSePurchaseMail::SetCountPriceInfo()
{
	
	INT nRowCnt = m_pDM->GetRowCount();
	CString strCount;
	strCount.Format(_T("%d"),nRowCnt);
	m_pCM->SetControlMgrData(_T("종수"),strCount);
	
	CString strPrice;
	INT nPrice = 0;
	INT nTotalPrice = 0;

	for(INT i=0;i<nRowCnt;i++)
	{
		strPrice = _T("");
		strPrice = m_pDM->GetCellData(_T("구독가격"),i);
		
		if(strPrice.IsEmpty()) continue;
		
		nPrice = _ttoi(strPrice.GetBuffer(0));
		nTotalPrice += nPrice;
	}

	
	strPrice.Format(_T("%d"),nTotalPrice);
	m_pCM->SetControlMgrData(_T("가격합산"),strPrice);

}

VOID CSePurchaseMail::OnbtnPURMAILSEQNO() 
{
	// TODO: Add your control notification handler code here
	
	CSePurchaseNoDlg dlg(this);
	dlg.m_nOpenMode = -1;
	if(dlg.DoModal() != IDOK) return;
	
	m_pCM->SetControlMgrData(_T("차수년도"),dlg.m_strYear);
	m_pCM->SetControlMgrData(_T("차수번호"),dlg.m_strPurchaseNo);
	ShowGridData();

}

VOID CSePurchaseMail::SetProviderCombo()
{
	m_ctrlProviderCombo.ResetContent();
	INT nRowCnt = m_pDM_ALL->GetRowCount();
	
	CString strProvider;
	CArray<CString,CString> arrayProvider;

	for(INT i=0;i<nRowCnt;i++)
	{
		strProvider = _T("");
		strProvider = m_pDM->GetCellData(_T("UDF_구입처"),i);
		if(_T("적용안함") == strProvider) continue;

		if(!IsExistString(arrayProvider,strProvider))
		{
			arrayProvider.Add(strProvider);
		}
	}
	
	m_ctrlProviderCombo.InsertString(0,_T("전체자료"));
	
	
	INT nSize = arrayProvider.GetSize();

	for(i=0;i<nSize;i++)
	{
		m_ctrlProviderCombo.InsertString(i+1,arrayProvider.GetAt(i));
	}	
	
	m_ctrlProviderCombo.SetCurSel(0);
}

BOOL CSePurchaseMail::IsExistString(CArray<CString,CString>& arrayString, CString strCompare)
{
	INT nSize = arrayString.GetSize();
	CString tmp;

	for(INT i=0;i<nSize;i++)
	{
		tmp = arrayString.GetAt(i);
		if(tmp == strCompare) return TRUE;
	}

	return FALSE;
}

VOID CSePurchaseMail::OnSelchangecmbPURMAILPROVIDER() 
{
	// TODO: Add your control notification handler code here
	
	ShowGridDataByProviderType();
	m_pGrid->Display();
	
}

VOID CSePurchaseMail::ShowGridDataByProviderType()
{
	CString strProvider;
	m_ctrlProviderCombo.GetWindowText(strProvider);
	
	m_pDM->FreeData();
	
	INT nIdx = 0;
	INT nRowCnt = m_pDM_ALL->GetRowCount();

	CString strUdfProvider;

	for(INT i=0;i<nRowCnt;i++)
	{
		if(_T("전체자료") != strProvider)
		{
			strUdfProvider = _T("");
			strUdfProvider = m_pDM_ALL->GetCellData(_T("UDF_구입처"),i);
			if(strUdfProvider != strProvider) continue;
		}

		m_pDM->InsertRow(-1);
		nIdx = m_pDM->GetRowCount()-1;
		CopyDMToDMByAlias(m_pDM_ALL,i,m_pDM,nIdx);
	}
}



INT CSePurchaseMail::CopyDMToDMByAlias(CESL_DataMgr *pDM_SOURCE, INT SourceRowIdx, CESL_DataMgr *pDM_TARGET, INT TargetRowIdx)
{
	if (!pDM_SOURCE || !pDM_TARGET) return -1;

	INT nSourceColumnCount = pDM_SOURCE->GetRefColumnCount();
	CString field_alias, field_name, field_type, data_type, strData;
	INT ids;
	for (INT col = 0; col < nSourceColumnCount; col++) {
		strData = _T("");
		ids = pDM_SOURCE->FindColumnFieldName(col, field_alias, field_name, field_type, data_type, SourceRowIdx, strData);
		ids = pDM_TARGET->SetCellData(field_alias, strData, TargetRowIdx);
	}
	
	return 0;
}

VOID CSePurchaseMail::OnOK() 
{
	// TODO: Add extra validation here
	//메일보내기 함수 

	if(SendMail()<0)
	{
		AfxMessageBox(_T("메일 형식 작성에 실패하였습니다!"));
	}
	

	return;
	CDialog::OnOK();
}

CString CSePurchaseMail::GetLIBInfo(CESL_DataMgr * pDM)
{
	CString strLibName;
	pDM->GetOneValueQuery(_T("SELECT MAX(VALUE) FROM MN_LIBINFO_TBL WHERE VALUE_NAME = '도서관명'"), strLibName);
	return strLibName;
}


INT CSePurchaseMail::SendMail()
{
	m_pGrid->SelectMakeList();
	if(0 == m_pGrid->SelectGetCount()) {
		AfxMessageBox(_T("선택된 항목이 없습니다!"));
		return 0;
	}
	
	CSendMailMgr mailMgr(this);


	//선택된 항목을 구입처별로 묶어서 처리한다
	CString strLibName = GetLIBInfo(m_pDM);
	
	CString strProvider;
	CArray<INT,INT> arrayIdx;
	INT nIdx = m_pGrid->SelectGetHeadPosition();
	
	
	while(nIdx>=0)
	{
		strProvider = m_pDM->GetCellData(_T("구입처_코드"),nIdx);
		if(strProvider.IsEmpty()) {
			AfxMessageBox(_T("구입처가 없는 선택항목이 있습니다! \n 확인 해 주십시오"));
			return 0;
		}
		nIdx = m_pGrid->SelectGetNext();
	}
	
	CString strCompare = _T("");
	nIdx = m_pGrid->SelectGetHeadPosition();

	while(nIdx>=0)
	{
		strProvider = m_pDM->GetCellData(_T("구입처_코드"),nIdx);
		
		if(strCompare.IsEmpty()) //초기
		{
			strCompare = strProvider;
			arrayIdx.Add(nIdx);
		}
		else //비교
		{
			if(strCompare == strProvider) //같은 구입처 
			{
				arrayIdx.Add(nIdx);
			}
			else //다른 구입처
			{
				if(SetMailInfo(&mailMgr,strLibName,strCompare,arrayIdx)<0) return -1;
				arrayIdx.RemoveAll();
				strCompare = strProvider;
				arrayIdx.Add(nIdx);
			}
		}

		nIdx = m_pGrid->SelectGetNext();
	}
	
	if(SetMailInfo(&mailMgr,strLibName,strCompare,arrayIdx)<0) return -1;

	INT nRecordCnt = mailMgr.GetCountMailRecord();
	mailMgr.DoModal(MAIL_SERVER_CFGFILE);

	return 0;
}

INT CSePurchaseMail::SetMailInfo(CSendMailMgr* pMailMgr,
								 CString strLibName,
								 CString strProviderCode,
								 CArray<INT,INT>& arrayIdx)
{
	int ids = 0;

	SendMailRecord MailRecord;
	CProviderInfo ProviderInfo;
	
	ProviderInfo.SetProviderInfo(m_pDM, strProviderCode);
	MailRecord.m_nFileAttachCnt = 0;
	MailRecord.m_pFileAttach = _T("");
	MailRecord.m_sEncoding = _T("text/html");
	MailRecord.m_sRecipientAddress = ProviderInfo.GetProviderEMail();
	MailRecord.m_sRecipientName = ProviderInfo.GetProviderName();
	MailRecord.m_sCCName = _T("");
	MailRecord.m_sCCAddress = _T("");
	MailRecord.m_sSubject = _T("주문메일");

	// 2009.03.03 ADD BY CJY : 데몬전송을 위해 메일키와 클래스를 받아온다.
	{
		CString strAlias;

		CESL_DataMgr TmpDM;
		TmpDM.SetCONNECTION_INFO(_T(""));
		CString strSQL;
		strSQL.Format(
			_T("SELECT MM.REC_KEY, MM.MAIL_CLASS ")
			_T("FROM MN_MAIL_TBL MM, MN_MAIL_GROUP_TBL MG ")
			// 2009.11.30 UPDATE BY KSJ : 검색 조건에 관리구분 추가
			//DEL _T("WHERE MG.REC_KEY = MM.GROUP_KEY AND MG.M_TYPE = '수서' AND MG.MNG_GROUP = '주문메일'")
			_T("WHERE MG.REC_KEY = MM.GROUP_KEY AND MG.M_TYPE = '수서' AND MG.MNG_GROUP = '주문메일' AND MG.MANAGE_CODE=UDF_MANAGE_CODE")
			);
		ids = TmpDM.RestructDataManager(strSQL);
		if(0 > ids)
		{
			return -1000;
		}

		MailRecord.m_sMailInfoKey = TmpDM.GetCellData(0, 0);
		MailRecord.m_sMailClass = TmpDM.GetCellData(0, 1);			
	}

	ids = MakeOrderMailRecord(&MailRecord, strLibName, &ProviderInfo, arrayIdx);
	if(ids < 0) return ids;
	
	pMailMgr->AddMailRecord(&MailRecord);

	return 0;
}


INT CSePurchaseMail::MakeOrderMailRecord(SendMailRecord *pMailRecord,
									     CString strLibName,
									     CProviderInfo* pProviderInfo,
										 CArray<INT,INT>& arrayIdx)
{
	
	
	INT nSize = arrayIdx.GetSize();
	  
	CString body, header, footer;
	
	header = _T("주문메일");
	footer.Format(_T("다음의 자료들을 %s 요청합니다."), _T("주문"));

	CString tmpstr;
	

	body +=	_T("<html>\r\n")
			_T("<head>\r\n")
			_T("<META HTTP-EQUIV=\"Content-Type\" CONTENT=\"text/html; charset=EUC-KR\">\r\n");
	tmpstr.Format(_T("<title>%s</title>\r\n"), header);
	body +=	tmpstr;
	body +=	_T("<style TYPE=\"text/css\">\r\n")
			_T("<!--	td {font-size:9pt; font-face:굴림}\r\n")
			_T("	font {font-face:굴림}\r\n")
			_T("	.mid {font:12pt 굴림; color=\"#003399\"}-->\r\n")
			_T("</style>\r\n")
			_T("</head>\r\n")
			_T("<body leftmargin=\"0\" topmargin=\"0\" marginwidth=\"0\" marginheight=\"0\" text=\"#000000\" bgcolor=ffffff link=\"#006699\" alink=\"#996600\" vlink=\"#996600\">\r\n")
			_T("<br>\r\n")
			_T("<table border=\"1\" cellpadding=\"6\" cellspacing=\"0\" align=\"center\" width=300 bordercolor=\"#666666\">\r\n")
			_T("<tr> \r\n");
	tmpstr.Format(_T("<td align=\"center\" bgcolor=\"#dddddd\"><font size=3>%s</font></td>\r\n"), header);
	body += tmpstr;
	body += _T("</tr>\r\n")
			_T("</table>\r\n")
			_T("<br><br>\r\n")
			_T("<table border=\"0\" cellpadding=\"0\" cellspacing=\"0\" width=750 align=\"center\">\r\n")
			_T("<tr>	\r\n")
			_T("<td align=left valign=bottom> <br>\r\n")
			_T("  <table width=\"365\" border=\"1\" cellspacing=\"0\" cellpadding=\"5\" bordercolor=\"#666666\">\r\n")
			_T("	<tr> \r\n")
			_T("	  <td rowspan=\"3\" width=\"12\">공<br>\r\n")
			_T("		급<br>\r\n")
			_T("		처</td>\r\n")
			_T("	  <td width=\"117\" bgcolor=\"#eeeeee\"> \r\n")
			_T("		<div align=\"center\">상 호</div>\r\n")
			_T("	  </td>\r\n");
	tmpstr.Format(_T("	  <td width=\"198\">&nbsp;%s</td>\r\n"), pProviderInfo->GetProviderName());
	body += tmpstr;
	body += _T("	</tr>\r\n")
			_T("	<tr> \r\n")
			_T("	  <td width=\"117\" bgcolor=\"#eeeeee\"> \r\n")
			_T("		<div align=\"center\">주 소</div>\r\n")
			_T("	  </td>\r\n");
	tmpstr.Format(_T("	  <td width=\"198\">&nbsp;%s</td>\r\n"), pProviderInfo->GetProviderAddr());
	body += tmpstr;
	body +=	_T("	</tr>\r\n")
			_T("	<tr> \r\n")
			_T("	  <td width=\"117\" bgcolor=\"#eeeeee\"> \r\n")
			_T("		<div align=\"center\">전화번호</div>\r\n")
			_T("	  </td>\r\n");
	tmpstr.Format(_T("	  <td width=\"198\">&nbsp;%s</td>\r\n"), pProviderInfo->GetProviderTel());
	body += tmpstr;
	body += _T("	</tr>\r\n")
			_T("  </table>\r\n")
  			_T("</td>\r\n");
	tmpstr.Format(_T("<td align=right valign=bottom>			발송일자 : &nbsp;%s<br><br>\r\n"), GetTodayDate());
	body += tmpstr;
	body += _T("<table width=\"200\" border=\"1\" cellspacing=\"0\" cellpadding=\"5\" bordercolor=\"#666666\">\r\n")
			_T("	<tr> \r\n")
			_T("	  <td rowspan=\"3\" width=\"12\">주문자</td>\r\n")
			_T("	  <td width=\"56\" bgcolor=\"#eeeeee\"> \r\n")
			_T("		<div align=\"center\">성명</div>\r\n")
			_T("	  </td>\r\n")
			_T("	  <td width=\"86\"> \r\n");
	tmpstr.Format(_T("		<div align=\"center\">&nbsp;%s</div>\r\n"), strLibName);
	body += tmpstr;
	body +=	_T("	  </td>\r\n")
			_T("	</tr>\r\n")
			_T("	<tr> \r\n")
			_T("	  <td width=\"56\" bgcolor=\"#eeeeee\" height=\"26\"> \r\n")
			_T("		<div align=\"center\">주문일자</div>\r\n")
			_T("	  </td>\r\n")
			_T("	  <td width=\"86\" height=\"26\"> \r\n");
	tmpstr.Format(_T("		<div align=\"center\">&nbsp;%s</div>\r\n"), GetTodayDate());
	body += tmpstr;
	body +=	_T("	  </td>\r\n")
			_T("	</tr>\r\n")
			_T("	<tr> \r\n")
			_T("	  <td width=\"56\" bgcolor=\"#eeeeee\"> \r\n")
			_T("		<div align=\"center\">주문종수</div>\r\n")
			_T("	  </td>\r\n")
			_T("	  <td width=\"86\"> \r\n");
	tmpstr.Format(_T("		<div align=\"center\">%d 종</div>\r\n"), nSize);
	body += tmpstr;
	body +=	_T("	  </td>\r\n")
			_T("	</tr>\r\n")
			_T("  </table>\r\n")
			_T("</td>\r\n")
			_T("</tr></table><br><center>\r\n")
			_T("<font size=3><br>\r\n");
	tmpstr.Format(_T("%s</font>\r\n"), footer);
	body += tmpstr;
	body +=	_T("</center>\r\n")
			_T("<br>\r\n")
			_T("<table width=\"750\" border=\"1\" cellspacing=\"0\" cellpadding=\"1\" bordercolor=\"#666666\" align=\"center\">\r\n")
			_T("<tr> \r\n")
			_T("<td width=\"17\" bgcolor=\"#eeeeee\"> \r\n")
			_T("  <div align=\"center\">번호</div>\r\n")
			_T("</td>\r\n")
			_T("<td width=\"224\" bgcolor=\"#eeeeee\" height=\"35\"> \r\n")
			_T("  <div align=\"center\">도서명</div>\r\n")
			_T("</td>\r\n")
			_T("<td width=\"224\" bgcolor=\"#eeeeee\" height=\"35\"> \r\n")
			_T("  <div align=\"center\">출판사</div>\r\n")
			_T("</td>\r\n")
			_T("<td width=\"80\" bgcolor=\"#eeeeee\" height=\"35\"> \r\n")
			_T("  <div align=\"center\">구독시작일</div>\r\n")
			_T("</td>\r\n")
			_T("<td width=\"80\" bgcolor=\"#eeeeee\" height=\"35\"> \r\n")
			_T("  <div align=\"center\">구독만기일</div>\r\n")
			_T("</td>\r\n")
			_T("<td width=\"80\" bgcolor=\"#eeeeee\" height=\"35\"> \r\n")
			_T("  <div align=\"center\">구독가격</div>\r\n")
			_T("</td>\r\n")
			_T("</tr>\r\n");

	INT idx = 0;
	CString tmpstr1;
	INT nTotalPrice = 0;
	INT nPrice = 0;

	for(INT i=0;i<nSize;i++)
	{
		idx = arrayIdx.GetAt(i);

		body +=	_T("<tr>\r\n")
				_T("<td width=\"17\" height=\"25\"> \r\n");
		tmpstr.Format(_T("<div align=\"center\">%d</div>\r\n"), i+1);
		body += tmpstr;
		body +=	_T("</td>\r\n");

		m_pDM->GetCellData(_T("UDF_본서명"), idx, tmpstr1);
		tmpstr.Format(_T("<td width=\"224\">&nbsp;%s</td>\r\n"), tmpstr1);
		body += tmpstr;
		
		body += _T("<td width=\"224\"> \r\n");
		m_pDM->GetCellData(_T("UDF_발행자"), idx, tmpstr1);
		tmpstr.Format(_T("<div align=\"center\">&nbsp;%s</div>\r\n"), tmpstr1);
		body += tmpstr;
		body += _T("</td>\r\n");

		
		body += _T("<td width=\"80\"> \r\n");
		m_pDM->GetCellData(_T("구독시작일"), idx, tmpstr1);
		tmpstr.Format(_T("<div align=\"center\">&nbsp;%s</div>\r\n"), tmpstr1);
		body += tmpstr;
		body += _T("</td>\r\n");
			

		body += _T("<td width=\"80\"> \r\n");
		m_pDM->GetCellData(_T("구독만기일"), idx, tmpstr1);
		tmpstr.Format(_T("<div align=\"center\">&nbsp;%s</div>\r\n"), tmpstr1);
		body += tmpstr;
		body += _T("</td>\r\n");



		body += _T("<td width=\"80\"> \r\n");
		m_pDM->GetCellData(_T("구독가격"), idx, tmpstr1);
		
		//구독가격 합계계산
		if(!tmpstr1.IsEmpty()) {
			nPrice = _ttoi(tmpstr1.GetBuffer(0));
			nTotalPrice +=  nPrice;
		}


		tmpstr.Format(_T("<div align=\"center\">&nbsp;%s</div>\r\n"), tmpstr1);
		body += tmpstr;
		body += _T("</td>\r\n");
	}


	body +=	_T("<tr> \r\n")
			_T("<td colspan=\"6\">\r\n")
			_T("  <table width=\"100%%\" border=\"1\" cellspacing=\"0\" cellpadding=\"0\" bordercolor=\"#666666\">\r\n")
			_T("	<tr> \r\n")
			_T("	  <td width=\"640\" height=\"30\" bgcolor=\"#eeeeee\"> \r\n")
			_T("		<div align=\"center\">합 계</div>\r\n")
			_T("	  </td>\r\n")


	_T("	  <td width=\"80\" bgcolor=\"#FFFFFF\"> \r\n");
	tmpstr.Format(_T("		<div align=\"center\">%d 원</div>\r\n"), nTotalPrice);
	body += tmpstr;
	body +=	_T("	  </td>\r\n")
			_T("	</tr>\r\n")
			_T("  </table>\r\n")
			_T("</td>\r\n")
			_T("</tr>\r\n")
			_T("</table>\r\n")
			_T("</body></html>\r\n");


	pMailRecord->m_sBody = body;
	
	return 0;	
}

CString CSePurchaseMail::GetTodayDate()
{
	CString result;
	CTime t = CTime::GetCurrentTime();
	result.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
	return result;
}

CString CSePurchaseMail::GetTodayYear()
{
	CString result;
	CTime t = CTime::GetCurrentTime();
	result.Format(_T("%04d"), t.GetYear());
	return result;
}

HBRUSH CSePurchaseMail::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
