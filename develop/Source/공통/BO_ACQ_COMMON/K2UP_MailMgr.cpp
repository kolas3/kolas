// K2UP_MailMgr.cpp: implementation of the CK2UP_MailMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "K2UP_MailMgr.h"
#include "OrderMailDataInfo.h"
#include "../BO_ACQ_API/BO_ACQ_Business_Api.h"
#include "../COMMON_CONVERSION_MGR/PriceInfoConversionMgr.h"
#include "MailAccountMgr.h"

#include "BO_LOC_3000Api.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#define WORK_LOG _T("메일관리"), _T(__FILE__), __LINE__
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CK2UP_MailMgr::CK2UP_MailMgr(CESL_Mgr * pMgr):CSPF_Work(pMgr)
{
	CLibInfoMgr mgr(pMgr);
	mgr.GetLibInfo(pMgr->GetUserID(), &m_cLibInfo);
	m_pDataProcessingDlg = NULL;
	m_strSelectSpeciesKey = _T("");
	m_pMgr = pMgr;

	m_strQuery = _T("");
	m_nOrderOpType = -1;	
}

CK2UP_MailMgr::~CK2UP_MailMgr()
{
	if(m_pDataProcessingDlg)
	{
		delete m_pDataProcessingDlg;
		m_pDataProcessingDlg = NULL;
	}
}

INT CK2UP_MailMgr::SPFExecutePath(CString strPath)
{
	if(strPath.IsEmpty()) return -1;

	if(strPath == _T("책KEYDM생성"))
		return MakeDMByKeyList();
	else if(strPath == _T("그룹DM생성"))
		return MakeDMByGroupInfo();
	else if(strPath == _T("구입처추출"))
		return ExtractProvider();
	else if(strPath == _T("주문메일발송"))
		return SendOrderMail();
	else if(strPath == _T("비치희망메일검색"))
		return SearchRequestMailData();
	else if(strPath == _T("비치희망메일발송"))
		return SendRequestMail();

	return -1;
}

INT CK2UP_MailMgr::MakeDMByKeyList()
{
	CString strProvider, strReckeyList;
	CESL_DataMgr * pDM_BOOK = (CESL_DataMgr*)GetTempPtr(_T("DM_BOOK"));
	CESL_DataMgr * pDM_VOL = (CESL_DataMgr*)GetTempPtr(_T("DM_VOL"));
	GetTempData(_T("PROVIDER"), strProvider);
	GetTempData(_T("KEY_LIST"), strReckeyList);
	CString strFindRecKeyQuery;
	strFindRecKeyQuery = strReckeyList;
	ConvertRecKeyToSubQuery(strFindRecKeyQuery, _T("BO_BOOK_TBL"), _T("REC_KEY"));
	
	CString strQuery, strTmpData;
	strQuery.Format(_T("I.REC_KEY = S.REC_KEY AND I.REC_KEY = P.SPECIES_KEY AND P.REC_KEY = B.ACQ_KEY AND B.BOOK_APPENDIX_FLAG='B' AND B.REC_KEY IN (%s)"), strFindRecKeyQuery);
	if(!strProvider.IsEmpty()) strTmpData.Format(_T(" AND P.PROVIDER_CODE = '%s'"), strProvider);
	strQuery += strTmpData;
	ShowDataProcessingDlg();
	m_pDataProcessingDlg->SetMessage(_T("메일발송자료를 구축중입니다."));
	INT ids = pDM_BOOK->RefreshDataManager(strQuery, m_pDataProcessingDlg->GetProgressCtrl());
	if(ids < 0) 
	{
		DeleteDataProcessingDlg();
		return ids;
	}
	
	SetTempData(_T("QUERY"), strQuery.GetBuffer(0));
	pDM_VOL->FreeData();
	
	CBO_ACQ_Business_Api mgr(m_pParentMgr);
	mgr.InitTempData();
	mgr.SetTempPtr(_T("책DM"), (DWORD*)pDM_BOOK);
	mgr.SetTempPtr(_T("권DM"), (DWORD*)pDM_VOL);
	mgr.SetTempPtr(_T("ProgressCtrl"), (DWORD*)m_pDataProcessingDlg->GetProgressCtrl());
	ids = mgr.SPFExecutePath(_T("책To권"));
	DeleteDataProcessingDlg();
	return ids;
}

INT CK2UP_MailMgr::MakeDMByGroupInfo()
{
	CString strProvider;	
	CESL_DataMgr * pDM_BOOK = (CESL_DataMgr*)GetTempPtr(_T("DM_BOOK"));
	CESL_DataMgr * pDM_VOL = (CESL_DataMgr*)GetTempPtr(_T("DM_VOL"));
	GetTempData(_T("PROVIDER"), strProvider);
	CGroupInfo * pGroup = (CGroupInfo*)GetTempPtr(_T("GROUP_INFO"));
	
	CString strQuery, strTmpData, strAcqYear, strGroupInfo, strWorkCode;
	strAcqYear = pGroup->GetAcqYear();
	strGroupInfo = pGroup->GetGroupInfo();
	strWorkCode = pGroup->GetWorkCode();

	if(strWorkCode.IsEmpty())
	{
		if( m_strSelectSpeciesKey.IsEmpty() )
		{
			strQuery.Format(_T("I.REC_KEY = S.REC_KEY AND I.REC_KEY = P.SPECIES_KEY AND P.REC_KEY = B.ACQ_KEY AND B.BOOK_APPENDIX_FLAG='B' AND P.ORDER_YEAR='%s' AND P.ORDER_SEQ_NO=%s"),
				strAcqYear, strGroupInfo);
		}
		else
		{
			CString strFindRecKeyQuery;
			strFindRecKeyQuery = m_strSelectSpeciesKey;
			ConvertRecKeyToSubQuery(strFindRecKeyQuery, _T("BO_PURCHASEINFO_TBL"), _T("SPECIES_KEY"));
			strQuery.Format(_T("I.REC_KEY = S.REC_KEY AND I.REC_KEY = P.SPECIES_KEY AND P.REC_KEY = B.ACQ_KEY AND B.BOOK_APPENDIX_FLAG='B' AND P.ORDER_YEAR='%s' AND P.ORDER_SEQ_NO=%s AND P.SPECIES_KEY IN(%s)"),
				strAcqYear, strGroupInfo, strFindRecKeyQuery);			
		}
	}
	else 
	{
		if( m_strSelectSpeciesKey.IsEmpty() )
		{
			strQuery.Format(_T("I.REC_KEY = S.REC_KEY AND I.REC_KEY = P.SPECIES_KEY AND P.REC_KEY = B.ACQ_KEY AND B.BOOK_APPENDIX_FLAG='B' AND P.ORDER_YEAR='%s' AND P.ORDER_SEQ_NO=%s AND I.WORK_CODE='%s'"),
				strAcqYear, strGroupInfo, strWorkCode);
		}
		else
		{
			CString strFindRecKeyQuery;
			strFindRecKeyQuery = m_strSelectSpeciesKey;
			ConvertRecKeyToSubQuery(strFindRecKeyQuery, _T("BO_PURCHASEINFO_TBL"), _T("SPECIES_KEY"));
			strQuery.Format(_T("I.REC_KEY = S.REC_KEY AND I.REC_KEY = P.SPECIES_KEY AND P.REC_KEY = B.ACQ_KEY AND B.BOOK_APPENDIX_FLAG='B' AND P.ORDER_YEAR='%s' AND P.ORDER_SEQ_NO=%s AND I.WORK_CODE='%s' AND P.SPECIES_KEY IN(%s)"),
				strAcqYear, strGroupInfo, strWorkCode, strFindRecKeyQuery);		
		}
	}

	if(!strProvider.IsEmpty()) strTmpData.Format(_T(" AND P.PROVIDER_CODE = '%s'"), strProvider);
	strQuery += strTmpData;
	ShowDataProcessingDlg();
	m_pDataProcessingDlg->SetMessage(_T("메일발송자료를 구축중입니다."));
	INT ids = pDM_BOOK->RefreshDataManager(strQuery, m_pDataProcessingDlg->GetProgressCtrl());
	if(ids < 0)
	{
		DeleteDataProcessingDlg();
		return ids;
	}
	m_strQuery = strQuery;
	
	pDM_VOL->FreeData();
	SetTempData(_T("QUERY"), m_strQuery.GetBuffer(0));
	
	CBO_ACQ_Business_Api mgr(m_pParentMgr);
	mgr.InitTempData();
	mgr.SetTempPtr(_T("책DM"), (DWORD*)pDM_BOOK);
	mgr.SetTempPtr(_T("권DM"), (DWORD*)pDM_VOL);
	mgr.SetTempPtr(_T("ProgressCtrl"), (DWORD*)m_pDataProcessingDlg->GetProgressCtrl());
	ids = mgr.SPFExecutePath(_T("책To권"));
	DeleteDataProcessingDlg();
	return ids;
}

INT CK2UP_MailMgr::ExtractProvider()
{
	CStringArray * pArrProvider = (CStringArray*)GetTempPtr(_T("PROVIDER_ARRAY"));
	CStringArray * pArrProviderDesc = (CStringArray*)GetTempPtr(_T("PROVIDER_DESC_ARRAY"));
	GetTempData(_T("QUERY"), m_strQuery);
	CESL_DataMgr tmpDM;
	tmpDM.SetCONNECTION_INFO(m_pParentMgr->CONNECTION_INFO);
	
	CString strQuery;
	strQuery.Format(
		_T("SELECT DISTINCT(P.PROVIDER_CODE) FROM IDX_BO_TBL I, BO_SPECIES_TBL S, BO_PURCHASEINFO_TBL P, BO_BOOK_TBL B WHERE I.MANAGE_CODE = UDF_MANAGE_CODE AND %s ORDER BY P.PROVIDER_CODE"),
		m_strQuery);
	INT ids = tmpDM.RestructDataManager(strQuery);
	if(ids < 0) return ids;

	pArrProvider->RemoveAll();		pArrProviderDesc->RemoveAll();
	pArrProvider->Add(_T(""));			pArrProviderDesc->Add(_T("전체자료"));

	INT cnt = tmpDM.GetRowCount();
	CString strTmpData;
	for(INT i=0 ; i<cnt ; i++)
	{
		strTmpData = tmpDM.GetCellData(i, 0);
		strTmpData.TrimLeft();		strTmpData.TrimRight();
		if(strTmpData.IsEmpty()) continue;
		pArrProvider->Add(strTmpData);
		m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("구입처관리"), strTmpData, strTmpData);
		pArrProviderDesc->Add(strTmpData);
	}
	
	return 0;
}

INT CK2UP_MailMgr::SendOrderMail()
{
	CESL_DataMgr * pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	CESL_Grid * pGrid = (CESL_Grid*)GetTempPtr(_T("GRID"));
	CString strCopyDMAlias, strProvider;
	GetTempData(_T("COPYDM_ALIAS"), strCopyDMAlias);
	GetTempData(_T("PROVIDER"), strProvider);
	INT nOpType = -1;
	GetTempData(_T("OPERATION_TYPE"), m_nOrderOpType);
	CStringArray * pArrProvider = (CStringArray*)GetTempPtr(_T("PROVIDER_ARRAY"));
	CESL_DataMgr * pCopyDM = m_pParentMgr->FindDM(strCopyDMAlias);

	INT ids = MakeSelectedOrderDM(pDM, pCopyDM, pGrid);
	if(ids < 0) return ids;

	CSendMailMgr mailMgr(m_pParentMgr);
	CArray <COrderMailRecordInfo *, COrderMailRecordInfo *> mailRecordInfo;
	ids = SetOrderMailRecord(pCopyDM, &mailMgr, &mailRecordInfo);
	if(ids < 0) return -1;

	INT nRecordCnt = mailMgr.GetCountMailRecord();
	if(nRecordCnt == 0) return 1000;

	CString strMailType;
	CString strMailServer;
	CString strAccountYN;
	CString strAccount;
	CString strID;
	CString strPassword;
	CString strSenderName;
	CString strMailAddress;
	
	CMailAccountMgr MailAccountMgr;
	MailAccountMgr.GetMailAccount(_T("수서"), strAccount, strID, strPassword, strSenderName, strMailAddress);
	MailAccountMgr.GetMailServer(strMailServer, strAccountYN);		
	
	if ( strMailAddress != _T("") )
	{
		ids = mailMgr.DoModal(MAIL_SERVER_USER, strMailServer, strMailAddress, strAccountYN, strID, strPassword);
		if(ids < 0) return ids;
	}
	else
	{
		ids = mailMgr.DoModal(MAIL_SERVER_CFGFILE);
		if(ids < 0) return ids;
	}	
	

	pDM->StartFrame();
	INT nResult = -1;
	for(INT i=0 ; i<nRecordCnt ; i++)
	{
		nResult = mailMgr.GetMailRecordSendState(i);
		if(nResult == 0)
		{
			mailRecordInfo.GetAt(i)->UpdateMailSendDate(pDM, m_pParentMgr->GetWorkLogMsg(WORK_LOG), GetTodayDate(), m_nOrderOpType);
		}
	}
	
	ids = pDM->SendFrame();
	if(ids < 0) return -1;
	ids = pDM->EndFrame();
	if(ids < 0) return -1;

	INT cnt = mailRecordInfo.GetSize();
	for(i=0 ; i<cnt ; i++)
	{
		delete mailRecordInfo.GetAt(i);
	}
	mailRecordInfo.RemoveAll();

	return ids;
}

INT CK2UP_MailMgr::SetOrderMailRecord(CESL_DataMgr *pCopyDM, CSendMailMgr *pMailMgr, CArray <COrderMailRecordInfo *, COrderMailRecordInfo *> * pMailRecordInfo)
{
	CString strMailInfoKey;
	CString strMailClass;	
	{
		CString strAlias;
		if(m_nOrderOpType > 0) strAlias = _T("주문메일");
		else strAlias = _T("주문취소메일");

		CESL_DataMgr TmpDM;
		TmpDM.SetCONNECTION_INFO(_T(""));
		CString strSQL;
		strSQL.Format(
			_T("SELECT MM.REC_KEY, MM.MAIL_CLASS ")
			_T("FROM MN_MAIL_TBL MM, MN_MAIL_GROUP_TBL MG ")
			_T("WHERE MG.REC_KEY = MM.GROUP_KEY AND MG.M_TYPE = '수서' AND MG.MNG_GROUP = '%s' AND MG.MANAGE_CODE=UDF_MANAGE_CODE"),
			strAlias
			);
		TmpDM.RestructDataManager(strSQL);

		strMailInfoKey = TmpDM.GetCellData(0, 0);
		strMailClass = TmpDM.GetCellData(0, 1);	
	}

	COrderMailRecordInfo * pRecordInfo = NULL;
	SendMailRecord MailRecord;
	CProviderInfo ProviderInfo;
	INT nRowCnt = pCopyDM->GetRowCount();
	INT nStartIdx, nLastIdx, ids;
	nStartIdx = nLastIdx = ids = 0;
	CString strProvider = pCopyDM->GetCellData(_T("BP_구입처부호"), 0);
	CString strTmpData;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strTmpData = pCopyDM->GetCellData(_T("BP_구입처부호"), i);
		CString strExchangeRate;
		CString strTempPrice;
		DOUBLE dPriceExchangeRate;
		DOUBLE dPrice;
		TCHAR* pEndPtr = NULL;
		CPriceInfoConversionMgr PriceConvertMgr;
		
		pCopyDM->GetCellData(_T("BP_환율"), i, strExchangeRate);
		dPriceExchangeRate = _tcstod(strExchangeRate.GetBuffer(0), &pEndPtr);
		
		pCopyDM->GetCellData(_T("UDF_변환가격"), i, strTempPrice);
		strTempPrice.Replace(_T(","),_T(""));
		dPrice = _tcstod(strTempPrice.GetBuffer(0), &pEndPtr);
		dPrice = dPrice * dPriceExchangeRate;
		strTempPrice.Format(_T("%.0f"), dPrice);
		PriceConvertMgr.ConversionPriceInfoToDesc(strTempPrice, strTempPrice);;
		pCopyDM->SetCellData( _T("UDF_변환가격"), strTempPrice, i );

		pCopyDM->GetCellData(_T("UDF_변환할인가격"), i, strTempPrice);
		strTempPrice.Replace(_T(","),_T(""));
		dPrice = _tcstod(strTempPrice.GetBuffer(0), &pEndPtr);
		dPrice = dPrice * dPriceExchangeRate;
		strTempPrice.Format(_T("%.0f"), dPrice);
		PriceConvertMgr.ConversionPriceInfoToDesc(strTempPrice, strTempPrice);;
		pCopyDM->SetCellData( _T("UDF_변환할인가격"), strTempPrice, i );
		
		strTempPrice.ReleaseBuffer();
		strExchangeRate.ReleaseBuffer();

		if(strProvider == strTmpData)
		{
			nLastIdx = i;
		}
		else
		{
			pRecordInfo = new COrderMailRecordInfo();
			ProviderInfo.SetProviderInfo(pCopyDM, pCopyDM->GetCellData(_T("BP_구입처부호"), nStartIdx));
			MailRecord.m_nFileAttachCnt = 0;
			MailRecord.m_pFileAttach = _T("");
			MailRecord.m_sEncoding = _T("text/html");
			MailRecord.m_sRecipientAddress = ProviderInfo.GetProviderEMail();
			MailRecord.m_sRecipientName = ProviderInfo.GetProviderName();
			MailRecord.m_sCCName = _T("");
			MailRecord.m_sCCAddress = _T("");	
			MailRecord.m_sMailClass = strMailClass;
			MailRecord.m_sMailInfoKey = strMailInfoKey;

			if(m_nOrderOpType > 0) MailRecord.m_sSubject = _T("주문메일");
			else MailRecord.m_sSubject = _T("주문취소메일");
			ids = MakeOrderMailRecord(pCopyDM, nStartIdx, nLastIdx, &MailRecord, pRecordInfo, m_cLibInfo.GetLibName(), &ProviderInfo);
			if(ids < 0) return ids;
			pMailMgr->AddMailRecord(&MailRecord);
			pMailRecordInfo->Add(pRecordInfo);
			strProvider = strTmpData;
			nStartIdx = i;
			nLastIdx = i;
		}
	}
	pRecordInfo = new COrderMailRecordInfo();
	ProviderInfo.SetProviderInfo(pCopyDM, pCopyDM->GetCellData(_T("BP_구입처부호"), nStartIdx));
	MailRecord.m_nFileAttachCnt = 0;
	MailRecord.m_pFileAttach = _T("");
	MailRecord.m_sEncoding = _T("text/html");
	MailRecord.m_sRecipientAddress = ProviderInfo.GetProviderEMail();
	MailRecord.m_sRecipientName = ProviderInfo.GetProviderName();
	MailRecord.m_sCCName = _T("");
	MailRecord.m_sCCAddress = _T("");	
	MailRecord.m_sMailClass = strMailClass;
	MailRecord.m_sMailInfoKey = strMailInfoKey;

	if(m_nOrderOpType > 0) MailRecord.m_sSubject = _T("주문메일");
	else MailRecord.m_sSubject = _T("주문취소메일");
	ids = MakeOrderMailRecord(pCopyDM, nStartIdx, nRowCnt-1, &MailRecord, pRecordInfo, m_cLibInfo.GetLibName(), &ProviderInfo);
	if(ids < 0) return ids;

	pMailMgr->AddMailRecord(&MailRecord);
	pMailRecordInfo->Add(pRecordInfo);
	return 0;
}

INT CK2UP_MailMgr::MakeOrderMailRecord(CESL_DataMgr *pDM, 
									   INT nStartIdx, 
									   INT nLastIdx,
									   SendMailRecord *pMailRecord,
									   COrderMailRecordInfo * pRecordInfo, 
									   CString strLibName,
									   CProviderInfo * pProviderInfo)
{
	CString body, header, footer;
	if(m_nOrderOpType > 0)
	{
		header = _T("주문메일");
		footer.Format(_T("다음의 자료들을 %s 요청합니다."), _T("주문"));
	}
	else
	{
		header = _T("주문취소메일");
		footer.Format(_T("다음의 자료들을 %s 요청합니다."), _T("주문취소"));
	}

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
			_T("<table border=\"0\" cellpadding=\"0\" cellspacing=\"0\" width=800 align=\"center\">\r\n")
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
			_T("		<div align=\"center\">주문책수</div>\r\n")
			_T("	  </td>\r\n")
			_T("	  <td width=\"86\"> \r\n");
	tmpstr.Format(_T("		<div align=\"center\">%d 책</div>\r\n"), GetBookCount(pDM, nStartIdx, nLastIdx));
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
			_T("<table width=\"800\" border=\"1\" cellspacing=\"0\" cellpadding=\"1\" bordercolor=\"#666666\" align=\"center\">\r\n")
			_T("<tr> \r\n")
			_T("<td width=\"17\" bgcolor=\"#eeeeee\"> \r\n")
			_T("  <div align=\"center\">번호</div>\r\n")
			_T("</td>\r\n")
			_T("<td width=\"217\" bgcolor=\"#eeeeee\" height=\"35\"> \r\n")
			_T("  <div align=\"center\">도서명</div>\r\n")
			_T("</td>\r\n")
			_T("<td width=\"91\" bgcolor=\"#eeeeee\" height=\"35\"> \r\n")
			_T("  <div align=\"center\">저자</div>\r\n")
			_T("</td>\r\n")
			_T("<td width=\"90\" bgcolor=\"#eeeeee\" height=\"35\"> \r\n")
			_T("  <div align=\"center\">출판사</div>\r\n")
			_T("</td>\r\n")
			_T("<td width=\"134\" bgcolor=\"#eeeeee\" height=\"35\"> \r\n")
			_T("  <div align=\"center\">권정보</div>\r\n")
			_T("</td>\r\n")
			_T("<td width=\"50\" bgcolor=\"#eeeeee\" height=\"35\"> \r\n")
			_T("  <div align=\"center\">책수</div>\r\n")
			_T("</td>\r\n")
			_T("<td width=\"61\" bgcolor=\"#eeeeee\" height=\"35\"> \r\n")
			_T("  <div align=\"center\">정가</div>\r\n")
			_T("</td>\r\n")
			_T("<td width=\"45\" bgcolor=\"#eeeeee\" height=\"35\"> \r\n")
			_T("  <div align=\"center\">할인율</div>\r\n")
			_T("</td>\r\n")
			_T("<td width=\"65\" bgcolor=\"#eeeeee\" height=\"35\"> \r\n")
			_T("  <div align=\"center\">할인가</div>\r\n")
			_T("</td>\r\n")
			_T("</tr>\r\n");
	CString tmpstr1, tmpstr2;
	for(INT i=nStartIdx ; i<=nLastIdx ; i++)
	{
		body +=	_T("<tr>\r\n")
				_T("<td width=\"17\" height=\"25\"> \r\n");
		tmpstr.Format(_T("<div align=\"center\">%d</div>\r\n"), i+1);
		body += tmpstr;
		body +=	_T("</td>\r\n");
		pDM->GetCellData(_T("IBS_본표제"), i, tmpstr1);
		tmpstr.Format(_T("<td width=\"217\">&nbsp;%s</td>\r\n"), tmpstr1);
		body += tmpstr;
		body += _T("<td width=\"91\"> \r\n");
		pDM->GetCellData(_T("BB_저작자"), i, tmpstr1);
		tmpstr.Format(_T("<div align=\"center\">&nbsp;%s</div>\r\n"), tmpstr1);
		body += tmpstr;
		body +=	_T("</td>\r\n")
				_T("<td width=\"90\"> \r\n");
		pDM->GetCellData(_T("IBS_발행자"), i, tmpstr1);
		tmpstr.Format(_T("<div align=\"center\">&nbsp;%s</div>\r\n"), tmpstr1);
		body += tmpstr;
		body +=	_T("</td>\r\n")
				_T("<td width=\"134\"> \r\n");
		pDM->GetCellData(_T("BB_편권차"), i, tmpstr);
		pDM->GetCellData(_T("BB_편제"), i, tmpstr2);
		if(tmpstr.IsEmpty() && tmpstr2.IsEmpty()) tmpstr1.Empty();
		else tmpstr1.Format(_T("%s : %s"), tmpstr, tmpstr2);
		tmpstr.Format(_T("<div>&nbsp;%s</div>\r\n"), tmpstr1);
		body += tmpstr;
		body +=	_T("</td>\r\n")
				_T("<td width=\"50\"> \r\n");
		pDM->GetCellData(_T("UDF_책수"), i, tmpstr1);
		tmpstr.Format(_T("<div align=\"center\">&nbsp;%s 책</div>\r\n"), tmpstr1);
		body += tmpstr;
		body += _T("</td>\r\n")
				_T("<td width=\"61\"> \r\n");
		pDM->GetCellData(_T("UDF_변환가격"), i, tmpstr1);
		tmpstr.Format(_T("<div align=\"center\">&nbsp;%s 원</div>\r\n"), tmpstr1);
		body += tmpstr;
		body += _T("</td>\r\n")
				_T("<td width=\"45\"> \r\n");
		pDM->GetCellData(_T("BP_할인율"), i, tmpstr1);
		tmpstr.Format(_T("<div align=\"center\">&nbsp;%s %%</div>\r\n"), tmpstr1);
		body += tmpstr;
		body += _T("</td>\r\n")
				_T("<td width=\"65\"> \r\n");
		pDM->GetCellData(_T("UDF_변환할인가격"), i, tmpstr1);
		tmpstr.Format(_T("<div align=\"center\">&nbsp;%s 원</div>\r\n"), tmpstr1);
		body += tmpstr;
		body += _T("</td>\r\n")
				_T("</tr>\r\n");
	}
	body +=	_T("<tr> \r\n")
			_T("<td colspan=\"9\">\r\n")
			_T("  <table width=\"100%%\" border=\"1\" cellspacing=\"0\" cellpadding=\"0\" bordercolor=\"#666666\">\r\n")
			_T("	<tr> \r\n")
			_T("	  <td width=\"610\" height=\"30\" bgcolor=\"#eeeeee\"> \r\n")
			_T("		<div align=\"center\">합 계</div>\r\n")
			_T("	  </td>\r\n")
			_T("	  <td width=\"64\" bgcolor=\"#FFFFFF\"> \r\n");
	CString tmpprice, tmpDiscprice;
	GetTotalPrice(pDM, nStartIdx, nLastIdx, tmpprice, tmpDiscprice);
	tmpstr.Format(_T("		<div align=\"center\">%s 원</div>\r\n"), tmpprice);
	body += tmpstr;
	body +=	_T("	  </td>\r\n")
			_T("	  <td width=\"46\" bgcolor=\"#eeeeee\"> \r\n")
			_T("		<div align=\"center\">&nbsp;</div>\r\n")
			_T("	  </td>\r\n")
			_T("	  <td width=\"64\" bgcolor=\"#FFFFFF\"> \r\n");
	tmpstr.Format(_T("		<div align=\"center\">%s 원</div>\r\n"), tmpDiscprice);
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

INT CK2UP_MailMgr::MakeSelectedOrderDM(CESL_DataMgr *pDM, CESL_DataMgr *pCopyDM, CESL_Grid *pGrid)
{
	INT ids = pGrid->SelectMakeList();
	if(ids < 0) return ids;
	INT i = pGrid->SelectGetHeadPosition();
	if(i < 0) return i;

	pCopyDM->FreeData();
	INT nTargetRow = -1;
	while(i >= 0)
	{
		pCopyDM->InsertRow(-1);
		nTargetRow = pCopyDM->GetRowCount()-1;
		ids = CopyDMRowToDMByAlias(pDM, i, pCopyDM, nTargetRow);
		if(ids < 0) return 0;
		i = pGrid->SelectGetNext();
	}

	pCopyDM->SORT(_T("BP_구입처부호"));
	return 0;
}

INT CK2UP_MailMgr::CopyDMRowToDMByAlias(CESL_DataMgr *pDM_SOURCE, INT SourceRowIdx, CESL_DataMgr *pDM_TARGET, INT TargetRowIdx)
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

CString CK2UP_MailMgr::GetTodayDate(VOID)
{
	CString result;
	CTime t = CTime::GetCurrentTime();
	result.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
	return result;
}

CString CK2UP_MailMgr::GetTodayYear(VOID)
{
	CString result;
	CTime t = CTime::GetCurrentTime();
	result.Format(_T("%04d"), t.GetYear());
	return result;
}

INT CK2UP_MailMgr::GetBookCount(CESL_DataMgr * pDM, INT nStartIdx, INT nLastIdx)
{
	CString strTmpData;
	INT nTotalCnt = 0;
	for(INT i=nStartIdx ; i<=nLastIdx ; i++)
	{
		strTmpData = pDM->GetCellData(_T("UDF_책수"), i);
		nTotalCnt += _ttoi(strTmpData.GetBuffer(0));
	}
	return nTotalCnt;
}

VOID CK2UP_MailMgr::GetTotalPrice(CESL_DataMgr *pDM, INT nStartIdx, INT nLastIdx, CString &strTotalPrice, CString &strDiscTotalPrice)
{
	CString strTmpData;
	DOUBLE dTotalPrice, dPrice, dDiscRate, dDiscTotalPrice;
	dTotalPrice = dDiscTotalPrice = dPrice = dDiscRate = 0.0;
	INT nBookCnt = 0;
	for(INT i=nStartIdx ; i<=nLastIdx ; i++)
	{
		strTmpData = pDM->GetCellData(_T("BB_가격"), i);
		strTmpData.Replace(_T(","), _T(""));
		TCHAR* pEndPtr = NULL;
		dPrice = _tcstod(strTmpData.GetBuffer(0), &pEndPtr);

		strTmpData = pDM->GetCellData(_T("UDF_책수"), i);
		nBookCnt = _ttoi(strTmpData.GetBuffer(0));
		strTmpData = pDM->GetCellData(_T("BP_할인율"), i);
		dDiscRate = _tcstod(strTmpData.GetBuffer(0), &pEndPtr);

		CString strExchangeRate;
		DOUBLE dPriceExchangeRate;
		pDM->GetCellData(_T("BP_환율"), i, strExchangeRate);
		dPriceExchangeRate = _tcstod(strExchangeRate.GetBuffer(0), &pEndPtr);
		dTotalPrice += dPrice * nBookCnt * dPriceExchangeRate;
		dDiscTotalPrice += (dPrice - (dPrice*(dDiscRate/100))) * nBookCnt * dPriceExchangeRate;
		strExchangeRate.ReleaseBuffer();
	}
	CPriceInfoConversionMgr PriceConvertMgr;
	strTotalPrice.Format(_T("%.0f"), dTotalPrice);
	PriceConvertMgr.ConversionPriceInfoToDesc(strTotalPrice, strTotalPrice);
	strDiscTotalPrice.Format(_T("%.0f"), dDiscTotalPrice);
	PriceConvertMgr.ConversionPriceInfoToDesc(strDiscTotalPrice, strDiscTotalPrice);
}

INT CK2UP_MailMgr::SearchRequestMailData()
{
	CESL_DataMgr * pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	CESL_ControlMgr * pCM = (CESL_ControlMgr*)GetTempPtr(_T("CM"));
	CESL_Grid * pGrid = (CESL_Grid*)GetTempPtr(_T("Grid"));

	CString requestDate1, requestDate2, requestor, status, strManageCode;
	pCM->GetControlMgrData(_T("신청일1"), requestDate1);
	pCM->GetControlMgrData(_T("신청일2"), requestDate2);
	pCM->GetControlMgrData(_T("신청자"), requestor);
	pCM->GetControlMgrData(_T("비치상태"), status);
	pCM->GetControlMgrData(_T("관리구분"), strManageCode);

	requestDate1.TrimLeft();	requestDate1.TrimRight();
	requestDate2.TrimLeft();	requestDate2.TrimRight();
	requestor.TrimLeft();		requestor.TrimRight();
	status.TrimLeft();			status.TrimRight();
	strManageCode.TrimLeft();	strManageCode.TrimRight();

	CString strQuery;
	CString strOption = _T("");

	if( !requestDate1.IsEmpty() && !requestDate2.IsEmpty() )
	{
		strOption.Format(_T(" APPLICANT_DATE BETWEEN to_date('%s','YYYY/MM/DD') AND to_date('%s', 'YYYY/MM/DD')+0.99999"), requestDate1, requestDate2);
	
	}
	else if(!requestDate1.IsEmpty())
	{
		strOption.Format(_T(" APPLICANT_DATE BETWEEN to_date('%s','YYYY/MM/DD') AND to_date('%s', 'YYYY/MM/DD')+0.99999"), requestDate1, requestDate1);		
	}
	else if( !requestDate2.IsEmpty() )
	{
		m_pParentMgr->ESLAfxMessageBox(_T("검색 시작일을 입력해 주세요."));
		return 1000;
	}

	strQuery += strOption;
	strOption = _T("");

	if(!strManageCode.IsEmpty())
	{
		if(strQuery.IsEmpty())
			strOption.Format(_T(" MANAGE_CODE = '%s'"), strManageCode);
		else
			strOption.Format(_T(" AND MANAGE_CODE = '%s'"), strManageCode);
	}
	strQuery += strOption;
	strOption.Empty();

	if(!requestor.IsEmpty() && !strQuery.IsEmpty())
		strOption.Format(_T(" AND APPLICANT_NAME LIKE '%s%%'"), requestor);
	else if(!requestor.IsEmpty() && strQuery.IsEmpty())
		strOption.Format(_T(" APPLICANT_NAME LIKE '%s%%'"), requestor);
	strQuery += strOption;
	strOption = _T("");

	if(!status.IsEmpty() && !strQuery.IsEmpty())
		strOption.Format(_T(" AND FURNISH_STATUS = '%s'"), status);
	else if(!status.IsEmpty() && strQuery.IsEmpty())
		strOption.Format(_T(" FURNISH_STATUS = '%s'"), status);
	strQuery += strOption;

	INT ids = pDM->RefreshDataManager(strQuery);
	if(ids < 0) return -1001;
	
	if(TRUE == m_pMgr->m_pInfo->m_bLibSecuUse && 0 < pDM->GetRowCount())
	{
		CString strGetData;
		CStringArray asDes1;
		for(INT i = 0; i < pDM->GetRowCount(); i++)
		{
			pDM->GetCellData(_T("EMAIL"), i, strGetData);
			asDes1.Add(strGetData);
		}
		CBO_LOC_3000Api *p3000Api = new CBO_LOC_3000Api(m_pMgr);
		if(NULL != p3000Api)
		{
			if(TRUE == p3000Api->ConnectSocket()) 
			{
				p3000Api->UserInfoDecrytion(_T("BO_FURNISH_TBL"), _T("EMAIL"), asDes1);
				for(i = 0; i < pDM->GetRowCount(); i++)
				{
					pDM->SetCellData(_T("EMAIL"), asDes1.GetAt(i), i);
				}
				p3000Api->CloseSocket();	
			}
			delete p3000Api;
			p3000Api = NULL;
		}
		asDes1.RemoveAll(); asDes1.FreeExtra();
	}

	if(pDM->GetRowCount() == 0) return 2000;

	return 0;
}

INT CK2UP_MailMgr::SendRequestMail()
{	
	CESL_DataMgr * pParentDM = (CESL_DataMgr*)GetTempPtr(_T("PARENT_DM"));
	CESL_DataMgr * pDM = (CESL_DataMgr*)GetTempPtr(_T("DM"));
	CESL_Grid * pGrid = (CESL_Grid*)GetTempPtr(_T("Grid"));
	CESL_DataMgr * pComplexDM = (CESL_DataMgr*)GetTempPtr(_T("COMPLEX_DM"));  // 2005-03-16
	
	INT ids = pGrid->SelectMakeList();
	if(ids < 0) return -1001;
	INT i = pGrid->SelectGetHeadPosition();
	if(i < 0) return -1001;

	CSendMailMgr MailMgr(m_pParentMgr);
	CRequestMailRecordInfo RecordInfo;
	SendMailRecord MailRecord;
	CString tmpstr;

	CString strIs5163, strQuery, strReckey, strTmp, strIs64;
	CStringArray astrRecKeys;
	strReckey.Empty();

	while(i >= 0)
	{
		strTmp = pDM->GetCellData(_T("비치희망KEY"), i);
		astrRecKeys.Add(strTmp);
		if( strReckey.IsEmpty() == TRUE )	strReckey += strTmp;
		else								strReckey += _T(", ") + strTmp;
		
		i = pGrid->SelectGetNext();
	}

	strIs5163.Empty();
	strIs64.Empty();
	strQuery.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND GROUP_2 = '기타'AND GROUP_3 = '공통' AND CLASS_ALIAS = '5163'"), strIs5163);
	pDM->GetOneValueQuery(strQuery, strIs5163);
	strQuery.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND GROUP_2 = '기타'AND GROUP_3 = '공통' AND CLASS_ALIAS = '육사'"), strIs64);
	pDM->GetOneValueQuery(strQuery, strIs64);
	if( _T("Y") == strIs5163 && _T("Y") != strIs64)
		strQuery.Format(_T("REC_KEY IN (") + strReckey + _T(") ORDER BY APPLICANT_NAME"));
	else
	{
		strQuery.Format(_T("REC_KEY IN (") + strReckey + _T(") ORDER BY EMAIL"));
	}
	ids = pComplexDM->RefreshDataManager(strQuery);
	if(ids <0) return -1000;
	INT nRowCount = -1;

	BOOL bIs5163 = FALSE;
	CStdioFile File;
	if( NULL != File.Open(_T("../cfg/MailSystem.cfg"), CFile::modeRead | CFile::typeBinary) )
	{
		TCHAR cUni;
		File.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			File.SeekToBegin();
		}
		
		CString strLine, strParam;
		while(File.ReadString(strLine))
		{
			strLine.TrimLeft(); strLine.TrimRight();
			strParam = strLine.Left(1);
			if(strParam == _T("["))
			{
				strLine.TrimLeft(_T("[5163 :"));
				strLine.TrimRight(_T("]"));
				strLine.TrimLeft(); strLine.TrimRight();
				if(strLine == _T("TRUE")) 
				{
					bIs5163 = TRUE;
					break;
				}
				bIs5163 = FALSE;
				break;
			}
		}
	}
	File.Close();

	UserMailForm MailForm;
	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(pDM->CONNECTION_INFO);

	CString strTitle, strContents_header, strContents, strContents_tail, email, requestor;
	strQuery.Format(_T("SELECT M.TITLE, M.CONTENTS_HEADER, M.CONTENTS, M.CONTENTS_TAIL, M.REC_KEY, M.MAIL_CLASS ")
		            _T("FROM MN_MAIL_GROUP_TBL G, MN_MAIL_TBL M ")
					_T("WHERE M.GROUP_KEY = G.REC_KEY AND G.M_TYPE = '수서' AND G.MNG_GROUP = '비치희망안내' AND G.MANAGE_CODE=UDF_MANAGE_CODE"));
	TmpDM.RestructDataManager(strQuery);
	MailForm.m_sTitle = TmpDM.GetCellData(0, 0);
	MailForm.m_sContents_Header = TmpDM.GetCellData(0, 1);
	MailForm.m_sContents = TmpDM.GetCellData(0, 2);
	MailForm.m_sContents_Tail = TmpDM.GetCellData(0, 3);

	MailRecord.m_sMailInfoKey = TmpDM.GetCellData(0, 4);
	MailRecord.m_sMailClass = TmpDM.GetCellData(0, 5);

	CStringArray arrApplicantName, arrApplicantEMAIL;
	nRowCount = pComplexDM->GetRowCount();

	if(TRUE == m_pMgr->m_pInfo->m_bLibSecuUse && 0 < pComplexDM->GetRowCount())
	{
		CString strGetData;
		CStringArray asDes1;
		for(INT i = 0; i < pComplexDM->GetRowCount(); i++)
		{
			pComplexDM->GetCellData(_T("EMAIL"), i, strGetData);
			asDes1.Add(strGetData);
		}
		CBO_LOC_3000Api *p3000Api = new CBO_LOC_3000Api(m_pMgr);
		if(NULL != p3000Api)
		{
			if(TRUE == p3000Api->ConnectSocket()) 
			{
				p3000Api->UserInfoDecrytion(_T("BO_FURNISH_TBL"), _T("EMAIL"), asDes1);
				for(i = 0; i < pComplexDM->GetRowCount(); i++)
				{
					pComplexDM->SetCellData(_T("EMAIL"), asDes1.GetAt(i), i);
				}
				p3000Api->CloseSocket();	
			}
			delete p3000Api;
			p3000Api = NULL;
		}
		asDes1.RemoveAll(); asDes1.FreeExtra();
	}

	for(INT row=0; row < nRowCount; row++)
	{
		MailRecord.m_nFileAttachCnt = 0;
		MailRecord.m_pFileAttach = _T("");
		MailRecord.m_sEncoding = _T("text/html");
		if( _T("Y") == strIs5163 && _T("Y") != strIs64 )
		{
			email.Empty();
		}
		else
		{
			pComplexDM->GetCellData(_T("EMAIL"), row, email);
			if(email.IsEmpty()) continue;
		}
		MailRecord.m_sRecipientAddress = email; 

		ids = SetRequestMailRecord(pComplexDM, bIs5163, row, MailForm, MailRecord);
		if(ids < 0) 
			continue;

		MailMgr.AddMailRecord(&MailRecord);
		arrApplicantName.Add(MailRecord.TO_EMP_CODE);
		arrApplicantEMAIL.Add(email);

	}

	INT recordCnt = MailMgr.GetCountMailRecord();
	if(recordCnt == 0) return 1000;

	CString strMailType;
	CString strMailServer;
	CString strAccountYN;
	CString strAccount;
	CString strID;
	CString strPassword;
	CString strSenderName;
	CString strMailAddress;
	
	CMailAccountMgr MailAccountMgr;
	MailAccountMgr.GetMailAccount(_T("수서"), strAccount, strID, strPassword, strSenderName, strMailAddress);
	MailAccountMgr.GetMailServer(strMailServer, strAccountYN);		
	
	if ( strMailAddress != _T("") )
	{
		ids = MailMgr.DoModal(MAIL_SERVER_USER, strMailServer, strMailAddress, strAccountYN, strID, strPassword);
		if(ids < 0) return ids;
	}
	else
	{
		ids = MailMgr.DoModal(MAIL_SERVER_CFGFILE);
		if(ids < 0) return ids;
	}
	INT result = -1;
	pDM->StartFrame();
	for(i=recordCnt-1 ; i>=0 ; i--)
	{
		result = MailMgr.GetMailRecordSendState(i);
		if(result == 0)
		{
			for(INT idx=0; idx < pDM->GetRowCount(); idx++)
			{
				for( INT j=0; j<astrRecKeys.GetSize(); j++ )
				{
					if( pDM->GetCellData(_T("비치희망KEY"), idx) == astrRecKeys.GetAt(j) )
					{
						if(_T("Y") ==strIs5163 && _T("Y") != strIs64)
						{
							if(arrApplicantName.GetAt(i) == pDM->GetCellData(_T("신청자"), idx))
							{
								RecordInfo.SetRecKey(pDM->GetCellData(_T("비치희망KEY"), idx));
								RecordInfo.UpdateMailSendDate(pDM, m_pParentMgr->GetWorkLogMsg(WORK_LOG), GetTodayDate());
								RecordInfo.SetMailSendDateInfo(pDM, GetTodayDate());
								RecordInfo.SetMailSendDateInfo(pParentDM, GetTodayDate());
							}
							else
								continue;
							
						}
						else
						{
							if(arrApplicantEMAIL.GetAt(i) == pDM->GetCellData(_T("EMAIL"), idx)) 
							{
								RecordInfo.SetRecKey(pDM->GetCellData(_T("비치희망KEY"), idx));
								RecordInfo.UpdateMailSendDate(pDM, m_pParentMgr->GetWorkLogMsg(WORK_LOG), GetTodayDate());
								RecordInfo.SetMailSendDateInfo(pDM, GetTodayDate());
								RecordInfo.SetMailSendDateInfo(pParentDM, GetTodayDate());
							}
							else
								continue;
						}
					}
				}
			}
		}
		else
			continue;

	}

	ids = pDM->SendFrame();
	if(ids < 0) return -1;
	ids = pDM->EndFrame();
	if(ids < 0) return -1;

	return ids;
}

INT CK2UP_MailMgr::SetRequestMailRecord(CESL_DataMgr * pDM, BOOL bAnotherMailSystem, INT idx, UserMailForm &MailForm, SendMailRecord &MailRecord)
{
	BOOL bAddRecord = FALSE;
	if(idx == (pDM->GetRowCount()-1) )
	{
		bAddRecord = TRUE;
		pDM->SetCellData(_T("UDF_신청자변경"), _T("TRUE"), idx);
	}
	else
	{
		if(bAnotherMailSystem)
		{
			if( pDM->GetCellData(_T("신청자"), idx) == pDM->GetCellData(_T("신청자"), idx+1) )
			{
				bAddRecord = FALSE;
				pDM->SetCellData(_T("UDF_신청자변경"), _T("FALSE"), idx);
			}
			else
			{
				bAddRecord = TRUE;
				pDM->SetCellData(_T("UDF_신청자변경"), _T("TRUE"), idx);
			}
		}
		else
		{
			if( pDM->GetCellData(_T("EMAIL"), idx) == pDM->GetCellData(_T("EMAIL"), idx+1) )
			{
				bAddRecord = FALSE;
				pDM->SetCellData(_T("UDF_신청자변경"), _T("FALSE"), idx);
			}
			else
			{
				bAddRecord = TRUE;
				pDM->SetCellData(_T("UDF_신청자변경"), _T("TRUE"), idx);
			}
		}
	}

	CString body, strTitle, strContents, strContents_total, strContents_header, strContents_tail;
	CString tmpstr, libname, title, author, publisher, pub_year,status, applicant_date, applicant_name, cancel_reason;
	libname = m_cLibInfo.GetLibName();

	CString strIs5163,strIs64;
	CString strQuery;
	strIs5163.Empty();
	strIs64.Empty();
	strQuery.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND GROUP_2 = '기타'AND GROUP_3 = '공통' AND CLASS_ALIAS = '5163'"), strIs5163);
	pDM->GetOneValueQuery(strQuery, strIs5163);
	strQuery.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND GROUP_2 = '기타'AND GROUP_3 = '공통' AND CLASS_ALIAS = '육사'"), strIs64);
	pDM->GetOneValueQuery(strQuery, strIs64);


	pDM->GetCellData(_T("본표제"), idx, title);
	pDM->GetCellData(_T("저작자"), idx, author);
	pDM->GetCellData(_T("발행자"), idx, publisher);
	pDM->GetCellData(_T("발행년"), idx, pub_year);
	pDM->GetCellData(_T("신청일자"), idx, applicant_date);
	
	CESL_DataMgr dm5163;
	dm5163.SetCONNECTION_INFO(pDM->CONNECTION_INFO);

	if( _T("Y") == strIs5163 && _T("Y") != strIs64)
	{
		CString strUserName;
		strQuery.Format(_T("SELECT NAME, USER_POSITION_CODE FROM CO_LOAN_USER_TBL WHERE USER_NO = '%s' "), pDM->GetCellData(_T("신청자"), idx) );
		strUserName.Empty();
		dm5163.RestructDataManager(strQuery);		
		if ( !dm5163.GetCellData(0, 0).IsEmpty() ) 
			applicant_name = dm5163.GetCellData(0, 0);
		else
		{
			pDM->GetCellData(_T("신청자"), idx, applicant_name);
			dm5163.GetCellData(0, 0);
		}
	}
	else
	{
		pDM->GetCellData(_T("신청자"), idx, applicant_name);
	}	

	pDM->GetCellData(_T("취소사유"), idx, cancel_reason);
	pDM->GetCellData(_T("비치상태"), idx, status);
	if(status == _T("1"))
		status = _T("신청중");
	else if(status == _T("2"))
		status = _T("처리중");
	else if(status == _T("3"))
		status = _T("소장중");
	else if(status == _T("4"))
		status = _T("취소됨");

	if(idx == 0)
		strContents_total.Empty();
	else 
	{
		if( _T("TRUE") == pDM->GetCellData(_T("UDF_신청자변경"), idx-1) )
			strContents_total.Empty();
		else
			pDM->GetCellData(_T("UDF_메일중간내용"), idx-1, strContents_total);
	}

	strContents = ConvertDataFromMailDB(MailForm.m_sContents, libname, applicant_date, applicant_name, title, author, publisher, pub_year, status, cancel_reason);
	strContents = strContents_total + strContents + _T("<br>");
	strContents += _T("<br>");
	pDM->SetCellData(_T("UDF_메일중간내용"), strContents, idx);

	if( TRUE == bAnotherMailSystem )
	{
		strTitle = ConvertDataFromMailDB(MailForm.m_sTitle, libname, applicant_date, applicant_name, title, author, publisher, pub_year, status, cancel_reason);
		body += _T("<br>");

		strContents_header = ConvertDataFromMailDB(MailForm.m_sContents_Header, libname, applicant_date, applicant_name, title, author, publisher, pub_year, status, cancel_reason);

		if ( strIs5163 == _T("Y") && _T("Y") != strIs64)
		{
			CString strTmpData;
			strTmpData = dm5163.GetCellData(0,1);

			if ( !strTmpData.IsEmpty() )
				m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("대출소속정보"), strTmpData, strTmpData);

			strContents_header.Replace( _T("%대출자소속정보%"), strTmpData );
		}

		body += strContents_header + _T("<br>");

		body += _T("<br>");

		body += strContents + _T("<br>");

		strContents_tail = ConvertDataFromMailDB(MailForm.m_sContents_Tail, libname, applicant_date, applicant_name, title, author, publisher, pub_year, status, cancel_reason);
		body += strContents_tail + _T("<br>");
		body += _T("<br>");

	}
	else 
	{
		body += _T("<html>\r\n");
		body += _T("<head>\r\n");
		body += _T("	<META HTTP-EQUIV=\"Content-Type\" CONTENT=\"text/html; charset=EUC-KR\">\r\n");

		strTitle = ConvertDataFromMailDB(MailForm.m_sTitle, libname, applicant_date, applicant_name, title, author, publisher, pub_year, status, cancel_reason);
		body +=	   _T("		<title>") + strTitle + _T("\r\n");

		body += _T("</title>\r\n");
		body += (_T("<style TYPE=\"text/css\">\r\n"));
		body += (_T("<!--\r\n"));
		body += (_T("	td {font-size:10pt; font-face:굴림}\r\n"));
		body += (_T("	font {font-size:11pt; font-face:굴림}\r\n"));
		body += (_T("	.mid {font:12pt 굴림; color=\"#003399\"}\r\n"));
		body += (_T("-->\r\n"));
		body += (_T("</style>\r\n"));
		body += _T("</head>\r\n");
		body += _T("<body>\r\n");

		body += _T("<body leftmargin=0 topmargin=0 marginwidth=0 marginheight=0 text=\"#000000\" bgcolor=#ffffff link=\"#006699\" alink=\"#996600\" vlink=\"#996600\">\r\n");
		body += _T("<br>\r\n");

		strContents_header = ConvertDataFromMailDB(MailForm.m_sContents_Header, libname, applicant_date, applicant_name, title, author, publisher, pub_year, status, cancel_reason);
		body += _T("<font> ") + strContents_header + _T("</font><br>\r\n");
		body += _T("<br>\r\n");

		body += _T("<font> ") + strContents + _T("</font><br>\r\n");

		strContents_tail = ConvertDataFromMailDB(MailForm.m_sContents_Tail, libname, applicant_date, applicant_name, title, author, publisher, pub_year, status, cancel_reason);
		body += _T("<font> ") + strContents_tail + _T("</font><br>\r\n");
		body += _T("<br>\r\n");
		
		body += _T("</body>\r\n");
		body += _T("</html>\r\n");
	}

	if(!bAddRecord) 
		return -1;

	MailRecord.m_sRecipientName = pDM->GetCellData(_T("신청자"), idx); 
	MailRecord.m_sSubject = strTitle;
	MailRecord.m_sCCName = _T("");
	MailRecord.m_sCCAddress = _T("");
	MailRecord.m_sBody = body;
	MailRecord.TO_EMP_CODE = pDM->GetCellData(_T("신청자"), idx);

	return 0;	
}

INT CK2UP_MailMgr::SetRequestMailBody(CESL_DataMgr *pDM, INT idx, CString &body, CString libname)
{
	BOOL bIs5163 = FALSE;

	CStdioFile File;
	
	if( NULL != File.Open(_T("../cfg/MailSystem.cfg"), CFile::modeRead | CFile::typeBinary) )
	{
		TCHAR cUni;
		File.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			File.SeekToBegin();
		}
		
		CString strLine, strParam;
		while(File.ReadString(strLine))
		{
			strLine.TrimLeft(); strLine.TrimRight();
			strParam = strLine.Left(1);
			if(strParam == _T("["))
			{
				strLine.TrimLeft(_T("[5163 :"));
				strLine.TrimRight(_T("]"));
				strLine.TrimLeft(); strLine.TrimRight();
				if(strLine == _T("TRUE")) 
				{
					bIs5163 = TRUE;
					break;
				}
				File.Close();
				bIs5163 = FALSE;
				break;
			}

			bIs5163 = FALSE;
		}
	}

	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(pDM->CONNECTION_INFO);

	CString strQuery, strTitle, strContents_header, strContents, strContents_tail;
	strQuery.Format(_T("SELECT M.TITLE, M.CONTENTS_HEADER, M.CONTENTS, M.CONTENTS_TAIL ")
		            _T("FROM MN_MAIL_GROUP_TBL G, MN_MAIL_TBL M ")					
					_T("WHERE M.GROUP_KEY = G.REC_KEY AND G.M_TYPE = '수서' AND G.MNG_GROUP = '비치희망안내' AND G.MANAGE_CODE=UDF_MANAGE_CODE"));
	TmpDM.RestructDataManager(strQuery);
	strTitle = TmpDM.GetCellData(0, 0);
	strContents_header = TmpDM.GetCellData(0, 1);
	strContents = TmpDM.GetCellData(0, 2);
	strContents_tail = TmpDM.GetCellData(0, 3);
		
	CString tmpstr, title, author, publisher, pub_year,status, applicant_date, applicant_name, cancel_reason;

	if( TRUE == bIs5163 )
	{
		pDM->GetCellData(_T("본표제"), idx, title);
		pDM->GetCellData(_T("저작자"), idx, author);
		pDM->GetCellData(_T("발행자"), idx, publisher);
		pDM->GetCellData(_T("발행년"), idx, pub_year);
		pDM->GetCellData(_T("비치상태"), idx, status);
		pDM->GetCellData(_T("신청일자"), idx, applicant_date);  // 2005-03-16
		pDM->GetCellData(_T("신청자"), idx, applicant_name);
		pDM->GetCellData(_T("취소사유"), idx, cancel_reason);
		if(status == _T("1"))
			status = _T("신청중");
		else if(status == _T("2"))
			status = _T("처리중");
		else if(status == _T("3"))
			status = _T("소장중");
		else if(status == _T("4"))
			status = _T("취소됨");

		body += _T("안녕하세요 ") + libname + _T("입니다.\r\n");
		body += pDM->GetCellData(_T("신청자"), idx) + _T("님이 ") + pDM->GetCellData(_T("신청일자"), idx) + _T("일 신청하신 자료\r\n");
		body += _T("\r\n");
		body += _T("도서명 : ") + title + _T("\r\n");
		body += _T("저  자 : ") + author + _T("\r\n");
		body += _T("출판사 : ") + publisher + _T("\r\n");
		body += _T("발행년 : ") + pub_year + _T("\r\n");
		body += _T("\r\n");
		body += _T("\r\n");
		body += _T("현재 ") + status + _T(" 상태임을 알려드립니다.\r\n");

		if(!cancel_reason.IsEmpty()) 
		{
			body += _T("\r\n");
			body += _T("비  고 : ") + cancel_reason + _T(" \r\n");
			body += _T("이용해 주셔서 감사드립니다.");
			body += _T("\r\n");
			body += _T("\r\n");
		}
	}
	else
	{
		pDM->GetCellData(_T("본표제"), idx, title);
		pDM->GetCellData(_T("저작자"), idx, author);
		pDM->GetCellData(_T("발행자"), idx, publisher);
		pDM->GetCellData(_T("발행년"), idx, pub_year);
		pDM->GetCellData(_T("비치상태"), idx, status);
		pDM->GetCellData(_T("신청일자"), idx, applicant_date);
		pDM->GetCellData(_T("신청자"), idx, applicant_name);
		pDM->GetCellData(_T("취소사유"), idx, cancel_reason);
		if(status == _T("1"))
			status = _T("신청중");
		else if(status == _T("2"))
			status = _T("처리중");
		else if(status == _T("3"))
			status = _T("소장중");
		else if(status == _T("4"))
			status = _T("취소됨");

		body += _T("<html>\r\n");
		body += _T("<head>\r\n");
		body += _T("	<META HTTP-EQUIV=\"Content-Type\" CONTENT=\"text/html; charset=EUC-KR\">\r\n");
		strTitle = ConvertDataFromMailDB(strTitle, libname, applicant_date, applicant_name, title, author, publisher, pub_year, status, cancel_reason);
		body +=	   _T("		<title>") + strTitle + _T("\r\n");

		body += _T("</title>\r\n");
		body += (_T("<style TYPE=\"text/css\">\r\n"));
		body += (_T("<!--\r\n"));
		body += (_T("	td {font-size:9pt; font-face:굴림}\r\n"));
		body += (_T("	font {font-size:11pt; font-face:굴림}\r\n"));
		body += (_T("	.mid {font:12pt 굴림; color=\"#003399\"}\r\n"));
		body += (_T("-->\r\n"));
		body += (_T("</style>\r\n"));
		body += _T("</head>\r\n");
		body += _T("<body>\r\n<br><br>");
		body += _T("<body leftmargin=0 topmargin=0 marginwidth=0 marginheight=0 text=\"#000000\" bgcolor=#ffffff link=\"#006699\" alink=\"#996600\" vlink=\"#996600\">\r\n");
		body += _T("<br>\r\n");		
		strContents_header = ConvertDataFromMailDB(strContents_header, libname, applicant_date, applicant_name, title, author, publisher, pub_year, status, cancel_reason);
		body += _T("<font> ") + strContents_header + _T("</font><br>\r\n");
		body += _T("<br>\r\n");
		strContents = ConvertDataFromMailDB(strContents, libname, applicant_date, applicant_name, title, author, publisher, pub_year, status, cancel_reason);
		body += _T("<font> ") + strContents + _T("</font><br>\r\n");
		body += _T("<br>\r\n");
		strContents_tail = ConvertDataFromMailDB(strContents_tail, libname, applicant_date, applicant_name, title, author, publisher, pub_year, status, cancel_reason);
		body += _T("<font> ") + strContents_tail + _T("</font><br>\r\n");
		body += _T("<br>\r\n");

		
		body += _T("</body>\r\n");
		body += _T("</html>\r\n");
	}

	return 0;
}

VOID CK2UP_MailMgr::ShowDataProcessingDlg()
{
	if(m_pDataProcessingDlg == NULL) m_pDataProcessingDlg = new CDataProcessingDlg(m_pParentMgr);

	if(!m_pDataProcessingDlg->GetCreate())
	{
		if(!m_pDataProcessingDlg->Create(m_pParentMgr))
		{
			delete m_pDataProcessingDlg;
			m_pDataProcessingDlg = NULL;
			return;
		}
	}
	m_pDataProcessingDlg->CenterWindow();
	m_pDataProcessingDlg->ShowWindow(SW_SHOW);
	m_pDataProcessingDlg->UpdateWindow();
	m_pDataProcessingDlg->ShowHideCancelButton(FALSE);
	m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
}

VOID CK2UP_MailMgr::DeleteDataProcessingDlg()
{
	if(m_pDataProcessingDlg)
	{
		m_pDataProcessingDlg->ShowWindow(SW_HIDE);
		delete m_pDataProcessingDlg;
		m_pDataProcessingDlg = NULL;
	}
}

CString CK2UP_MailMgr::ConvertDataFromMailDB(CString strSource, CString libname, CString applicant_date, 
											 CString applicant_name, CString title, CString author, 
											 CString publisher, CString pub_year, 
											 CString status, CString strCancelReason)
{
	strSource.Replace(_T("%도서관명%"), libname);
	strSource.Replace(_T("%신청일%"), applicant_date);
	strSource.Replace(_T("%이용자명%"), applicant_name);
	strSource.Replace(_T("%서명%"), title);
	strSource.Replace(_T("%저자%"), author);
	strSource.Replace(_T("%출판사%"), publisher);
	strSource.Replace(_T("%발행년%"), pub_year);
	strSource.Replace(_T("%비치상태%"), status);
	strSource.Replace(_T("%비고%"), strCancelReason);
	strSource.Replace(_T("\r\n"), _T("<br>"));
	strSource.Replace(_T("\n"), _T("<br>"));

	return strSource;

}

INT CK2UP_MailMgr::ConvertRecKeyToSubQuery(CString &strRecKeyList, CString strTableName, CString strKeyName)
{
	
	CString strTemp;
 	CString strRecKey;
 	CString strBookKeyTempList;
 	CString strQueryWhere;
 	INT nCnt;
 	INT nLocation;
 	INT nStartLocation;
	INT i ; 
	strBookKeyTempList = strRecKeyList;
	strBookKeyTempList.Replace(_T("'"), _T(""));
	nCnt = 0;
	nLocation = 0;
	strQueryWhere.Format(_T(""));
	while( 1 )
	{
		nLocation = strBookKeyTempList.Find(_T(","), nLocation+1);
		++nCnt;
		if( 0 > nLocation )
		{
			break;			
		}
	}
	if ( (nCnt % 1000) == 0 )
	{
		nCnt = (nCnt/1000);
	}
	else
	{
		nCnt = (nCnt/1000)+1;
	}
	nStartLocation = strBookKeyTempList.GetLength()/nCnt;

	for( i = 0 ; i < nCnt ; i++)
	{
		if( i == (nCnt -1) )
		{
			strRecKey = strBookKeyTempList;
		}
		else
		{
			nLocation = strBookKeyTempList.Find(_T(","), nStartLocation);
			strRecKey = strBookKeyTempList.Mid(0, nLocation );
			strTemp.Format(_T("%s,"), strRecKey);
			strBookKeyTempList.Replace( strTemp, _T("") );
		}
	
		if( strQueryWhere.IsEmpty() )
		{
			strQueryWhere.Format(_T("%s IN (%s)"), strKeyName, strRecKey);
		}
		else
		{
			strTemp.Format(_T("%s OR %s IN( %s )"), strQueryWhere,strKeyName, strRecKey );
			strQueryWhere = strTemp;
		}
	}
	strRecKeyList.Format(_T("SELECT %s FROM %s WHERE %s"), strKeyName, strTableName,strQueryWhere);

	return 0;
}
