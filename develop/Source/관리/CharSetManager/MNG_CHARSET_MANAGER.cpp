// MNG_CHARSET_MANAGER.cpp : implementation file
//

#include "stdafx.h"
#include "MNG_CHARSET_MANAGER.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "ESL_DataMgr.h"
#include "ESL_CharSetMgr.h"
#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////
// CMNG_CHARSET_MANAGER dialog


CMNG_CHARSET_MANAGER::CMNG_CHARSET_MANAGER(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_CHARSET_MANAGER)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	// KOL.UDF.022 시큐어코딩 보완
	nBlockSize = 0;
	nHalfBlockSize = 0;
}

CMNG_CHARSET_MANAGER::~CMNG_CHARSET_MANAGER()
{
	
}
BOOL CMNG_CHARSET_MANAGER::Create(CWnd* pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CMNG_CHARSET_MANAGER::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_CHARSET_MANAGER)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_CHARSET_MANAGER, CDialog)
	//{{AFX_MSG_MAP(CMNG_CHARSET_MANAGER)
	ON_BN_CLICKED(IDC_bIMPORT1, OnbIMPORT1)
	ON_BN_CLICKED(IDC_bTEST1, OnbTEST1)
	ON_BN_CLICKED(IDC_bEXPORT1, OnbEXPORT1)
	ON_BN_CLICKED(IDC_bIMPORT2, OnbIMPORT2)
	ON_BN_CLICKED(IDC_bEXPORT2, OnbEXPORT2)
	ON_BN_CLICKED(IDC_bTEST2, OnbTEST2)
	ON_BN_CLICKED(IDC_bIMPORT3, OnbIMPORT3)
	ON_BN_CLICKED(IDC_bIMPORT4, OnbIMPORT4)
	ON_BN_CLICKED(IDC_bIMPORT5, OnbIMPORT5)
	ON_BN_CLICKED(IDC_bEXPORT3, OnbEXPORT3)
	ON_BN_CLICKED(IDC_bEXPORT4, OnbEXPORT4)
	ON_BN_CLICKED(IDC_bEXPORT5, OnbEXPORT5)
	ON_BN_CLICKED(IDC_bTEST3, OnbTEST3)
	ON_BN_CLICKED(IDC_bTEST4, OnbTEST4)
	ON_BN_CLICKED(IDC_bTEST5, OnbTEST5)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_CHARSET_MANAGER message handlers

VOID CMNG_CHARSET_MANAGER::OnbIMPORT1() 
{
	if (AfxMessageBox(_T("Chinese(TCHAR) -> Korean(TCHAR)를 반입하시겠습니까?"), MB_YESNO) != IDYES) return;
	CString strFileName = _T("SINGLE_CHI_TO_KOR_TBL.txt");

	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);


	CString strLine;
	CString strC, strP;
	CString strMsg;

	CStdioFile fd;
	if (fd.Open(strFileName, CFile::modeRead | CFile::shareDenyWrite | CFile::typeBinary) != TRUE) return;
	
	TCHAR cUni;
	fd.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fd.SeekToBegin();
	}
	
	TCHAR pBuf[2049];
	INT pos;
	INT nTemp, nSuccess=0, nError=0, nTotal=0;

	CString strQuery;
	dm.StartFrame();
	INT nCount = 0;
	INT ids;
	while(TRUE) 
	{
		if (fd.ReadString(pBuf, 2048) == NULL) break;
		strLine.Format(_T("%s"), pBuf);
		pos = strLine.Find(_T("|"));
		if (pos == -1) continue;
		strC = strLine.Left(pos);
		strP = strLine.Mid(pos+1);
		strC.TrimLeft();			strC.TrimRight();
		strP.TrimLeft();			strP.TrimRight();


		strQuery.Format(_T("INSERT INTO ESL_CHAR_SET_TBL (REC_KEY, FromSet, ToSet, FromChar, ToChar) VALUES (%s.NEXTVAL, 'CHINESE_CHAR', 'KOREAN_CHAR', '%s', '%s');"), dm.GetDefaultSEQName(), strC, strP);

		dm.AddFrame(strQuery);
	
		if (nCount % 20 == 0) {
			nTemp = dm.frame.GetCount();
			ids = dm.SendFrame();
				if (ids == 0) nSuccess += nTemp;
					else nError += nTemp;
				nTotal += nTemp;
			dm.EndFrame();
		}
		nCount++;
		strMsg.Format(_T("Success : %d - Error : %d - Total : %d"), nSuccess, nError, nTotal);
		GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(strMsg);
	}
	nTemp = dm.frame.GetCount();
	ids = dm.SendFrame();
		if (ids == 0) nSuccess += nTemp;
			else nError += nTemp;
		nTotal += nTemp;
	dm.EndFrame();
	strMsg.Format(_T("Success : %d - Error : %d - Total : %d"), nSuccess, nError, nTotal);
	GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(strMsg);

	AfxMessageBox(_T("완료되었습니다."));	
}

VOID CMNG_CHARSET_MANAGER::OnbTEST1() 
{
	CArray <INT, INT> arr;

	arr.Add(1);

	arr.Add(3);


	arr.GetSize();
	

	CESL_CharSetMgr m;
	m.InitData(_T("CHINESE_CHAR"), _T("KOREAN_CHAR"));
	
	INT ids;
	CString strTo;
	DWORD dSTART, dFINISH, dELA;
	dSTART = GetTickCount();
	ids = m.Find(_T("詰"), strTo);
	dFINISH = GetTickCount();
	dELA = dFINISH-dSTART;
	CString strMsg;
	strMsg.Format(_T("(%s) Max %d.%d sec"), strTo, dELA/1000, dELA%1000);
	AfxMessageBox(strMsg);
}

VOID CMNG_CHARSET_MANAGER::OnbEXPORT1() 
{
	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	
	CString strQuery;
	strQuery.Format(_T("SELECT FromChar, ToChar FROM ESL_CHAR_SET_TBL WHERE FromSet='CHINESE_CHAR' AND ToSet='KOREAN_CHAR' ORDER BY FromChar"));

	INT ids, i;
	ids = dm.RestructDataManager(strQuery);

	INT nRow = dm.GetRowCount();
	INT nSize;
	TCHAR * pData;
	nSize = (nRow * nBlockSize);
	pData = (TCHAR*)malloc(nSize+sizeof(TCHAR));
	if (!pData) {
		return;
	}
	pData[nSize] = '\0';
	TCHAR tTemp[256];
	for (i = 0; i < nBlockSize; i++) tTemp[i] = ' ';
	for (i = 0; i < nSize; i+=80) {
		wmemcpy(pData+i, tTemp, nBlockSize);
	}

	CString strFileName = GetDataFileName(_T("CHINESE_CHAR"), _T("KOREAN_CHAR"));

	CString strC, strP;
	INT nPos;
	for (i = 0; i < nRow; i++) {
		strC = dm.GetCellData(i, 0);
		strP = dm.GetCellData(i, 1);
		nPos = i * nBlockSize;
		wmemcpy(pData+nPos, strC.GetBuffer(0), strC.GetLength());
		wmemcpy(pData+nPos+nHalfBlockSize, strP.GetBuffer(0), strP.GetLength());
	}
	CFile fd;
	if (fd.Open(strFileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary) != TRUE) {
		free(pData);
		return;
	}
	
	if( 0 == fd.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		fd.Write( &cUni, sizeof(TCHAR));
	}
	
	fd.WriteHuge(pData, nRow*nBlockSize);

	fd.Close();
	free(pData);
	AfxMessageBox(_T("Export 완료!"));
}

BOOL CMNG_CHARSET_MANAGER::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	nBlockSize = 80;
	nHalfBlockSize = nBlockSize/2;

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

CString CMNG_CHARSET_MANAGER::GetDataFileName(CString strFromSet, CString strToSet)
{
	if (strFromSet == _T("CHINESE_CHAR") && strToSet == _T("KOREAN_CHAR")) return _T("..\\CFG\\ESL_CHINESE_CHAR_TO_KOREAN_CHAR.CFG");
	if (strFromSet == _T("KOREAN_WORD") && strToSet == _T("CHINESE_WORD")) return _T("..\\CFG\\ESL_KOREAN_WORD_TO_CHINESE_WORD.CFG");
	if (strFromSet == _T("KOREAN_CHAR") && strToSet == _T("HIRA_CHAR")) return _T("..\\CFG\\ESL_KOREAN_CHAR_TO_HIRA_CHAR.CFG");
	if (strFromSet == _T("KOREAN_CHAR") && strToSet == _T("KATA_CHAR")) return _T("..\\CFG\\ESL_KOREAN_CHAR_TO_KATA_CHAR.CFG");
	if (strFromSet == _T("KOREAN_CHAR") && strToSet == _T("ROMA_CHAR")) return _T("..\\CFG\\ESL_KOREAN_CHAR_TO_ROMA_CHAR.CFG");

	return _T("");
}

VOID CMNG_CHARSET_MANAGER::OnbIMPORT2() 
{
	if (AfxMessageBox(_T("Korean(word) -> Chinese(word)를 반입하시겠습니까?"), MB_YESNO) != IDYES) return;
	CString strFileName = _T("HANJA_WORD_TBL.txt");

	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);


	CString strLine;
	CString strC, strP;
	CString strMsg;

	CStdioFile fd;
	if (fd.Open(strFileName, CFile::modeRead | CFile::shareDenyWrite | CFile::typeBinary) != TRUE) return;
	
	TCHAR cUni;
	fd.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fd.SeekToBegin();
	}
	
	TCHAR pBuf[2049];
	INT pos;
	INT nTemp, nSuccess=0, nError=0, nTotal=0;

	CString strQuery;
	dm.StartFrame();
	INT nCount = 0;
	INT ids;
	while(TRUE) 
	{
		if (fd.ReadString(pBuf, 2048) == NULL) break;
		strLine.Format(_T("%s"), pBuf);
		pos = strLine.Find(_T("|"));
		if (pos == -1) continue;
		strC = strLine.Left(pos);
		strP = strLine.Mid(pos+1);
		strC.TrimLeft();			strC.TrimRight();
		strP.TrimLeft();			strP.TrimRight();

		strQuery.Format(_T("INSERT INTO ESL_CHAR_SET_TBL (REC_KEY, FromSet, ToSet, FromChar, ToChar) VALUES (%s.NEXTVAL, 'KOREAN_WORD', 'CHINESE_WORD', '%s', '%s');"), dm.GetDefaultSEQName(), strC, strP);

		dm.AddFrame(strQuery);
	
		if (nCount % 20 == 0) {
			nTemp = dm.frame.GetCount();
			ids = dm.SendFrame();
				if (ids == 0) nSuccess += nTemp;
					else nError += nTemp;
				nTotal += nTemp;
			dm.EndFrame();
		}
		nCount++;
		strMsg.Format(_T("Success : %d - Error : %d - Total : %d"), nSuccess, nError, nTotal);
		GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(strMsg);
	}
	nTemp = dm.frame.GetCount();
	ids = dm.SendFrame();
		if (ids == 0) nSuccess += nTemp;
			else nError += nTemp;
		nTotal += nTemp;
	dm.EndFrame();
	strMsg.Format(_T("Success : %d - Error : %d - Total : %d"), nSuccess, nError, nTotal);
	GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(strMsg);

	AfxMessageBox(_T("완료되었습니다."));		
}

VOID CMNG_CHARSET_MANAGER::OnbEXPORT2() 
{
	ExportToDataFile(_T("KOREAN_WORD"), _T("CHINESE_WORD"));
}

VOID CMNG_CHARSET_MANAGER::ExportToDataFile(CString strFromSet, CString strToSet)
{
	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	
	CString strQuery;
	strQuery.Format(_T("SELECT FromChar, ToChar FROM ESL_CHAR_SET_TBL WHERE FromSet='%s' AND ToSet='%s' ORDER BY FromChar"), strFromSet, strToSet);

	INT ids, i;
	ids = dm.RestructDataManager(strQuery);

	INT nRow = dm.GetRowCount();
	INT nSize;
	TCHAR * pData;
	nSize = (nRow * nBlockSize);
	pData = (TCHAR*)malloc(nSize+sizeof(TCHAR));
	if (!pData) {
		return;
	}
	pData[nSize] = '\0';
	TCHAR tTemp[256];
	for (i = 0; i < nBlockSize; i++) tTemp[i] = ' ';
	for (i = 0; i < nSize; i+=80) {
		wmemcpy(pData+i, tTemp, nBlockSize);
	}

	CString strFileName = GetDataFileName(strFromSet, strToSet);

	CString strC, strP;
	CString strTmp;
	INT nPos;
	POSITION pos = dm.data.GetHeadPosition();
	DWORD *row;
	for (i = 0; i < nRow; i++) {
		row = NULL;
		row = (DWORD*)dm.data.GetNext(pos);
		strC.Format(_T("%s"), row[0]);
		strP.Format(_T("%s"), row[1]);
		//strC = dm.GetCellData(i, 0);
		//strP = dm.GetCellData(i, 1);
		nPos = i * nBlockSize;
		wmemcpy(pData+nPos, strC.GetBuffer(0), strC.GetLength());
		wmemcpy(pData+nPos+nHalfBlockSize, strP.GetBuffer(0), strP.GetLength());
		strTmp.Format(_T("%d"), i);
		GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(strTmp);
	}
	CFile fd;
	if (fd.Open(strFileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary) != TRUE) {
		free(pData);
		return;
	}
	
	if( 0 == fd.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		fd.Write( &cUni, sizeof(TCHAR));
	}
	
	fd.WriteHuge(pData, nRow*nBlockSize);

	fd.Close();
	free(pData);
	AfxMessageBox(_T("Export 완료!"));	
}

VOID CMNG_CHARSET_MANAGER::OnbTEST2() 
{
	CESL_CharSetMgr m;
	m.InitData(_T("KOREAN_WORD"), _T("CHINESE_WORD"));
	
	INT ids, nStartRow = 0;
	DWORD dSTART, dFINISH, dELA;
	dSTART = GetTickCount();
	CString strTo[128];
	INT nStrTo = 128;
	ids = m.FindKoreanWordToChineseWord(_T("학교"),strTo, nStrTo);
	dFINISH = GetTickCount();
	dELA = dFINISH-dSTART;
	CString strMsg;
	strMsg.Format(_T("Max %d.%d sec"), dELA/1000, dELA%1000);
	AfxMessageBox(strMsg);	
}

VOID CMNG_CHARSET_MANAGER::OnbIMPORT3() 
{
	if (AfxMessageBox(_T("Korean(TCHAR) -> Hira(TCHAR)를 반입하시겠습니까?"), MB_YESNO) != IDYES) return;
	CString strFileName = _T("KOR_TO_JPN_ROMA_TBL.txt");

	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);


	CString strLine;
	CString strC, strP;
	CString strMsg;

	CStdioFile fd;
	if (fd.Open(strFileName, CFile::modeRead | CFile::shareDenyWrite | CFile::typeBinary) != TRUE) return;
	
	TCHAR cUni;
	fd.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fd.SeekToBegin();
	}
	
	TCHAR pBuf[2049];
	INT pos;
	INT nTemp, nSuccess=0, nError=0, nTotal=0;

	CString strQuery;
	dm.StartFrame();
	INT nCount = 0;
	INT ids;
	while(TRUE) 
	{
		if (fd.ReadString(pBuf, 2048) == NULL) break;
		strLine.Format(_T("%s"), pBuf);
		pos = strLine.Find(_T("|"));
		if (pos == -1) continue;
		strC = strLine.Left(pos);
		strP = strLine.Mid(pos+1);
		strC.TrimLeft();			strC.TrimRight();
		strP.TrimLeft();			strP.TrimRight();
		strP = strP.Left(30);
		strP.TrimLeft();			strP.TrimRight();

		strQuery.Format(_T("INSERT INTO ESL_CHAR_SET_TBL (REC_KEY, FromSet, ToSet, FromChar, ToChar) VALUES (%s.NEXTVAL, 'KOREAN_CHAR', 'HIRA_CHAR', '%s', '%s');"), dm.GetDefaultSEQName(), strC, strP);

		dm.AddFrame(strQuery);
	
		if (nCount % 20 == 0) {
			nTemp = dm.frame.GetCount();
			ids = dm.SendFrame();
				if (ids == 0) nSuccess += nTemp;
					else nError += nTemp;
				nTotal += nTemp;
			dm.EndFrame();
		}
		nCount++;
		strMsg.Format(_T("Success : %d - Error : %d - Total : %d"), nSuccess, nError, nTotal);
		GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(strMsg);
	}
	nTemp = dm.frame.GetCount();
	ids = dm.SendFrame();
		if (ids == 0) nSuccess += nTemp;
			else nError += nTemp;
		nTotal += nTemp;
	dm.EndFrame();
	strMsg.Format(_T("Success : %d - Error : %d - Total : %d"), nSuccess, nError, nTotal);
	GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(strMsg);

	AfxMessageBox(_T("완료되었습니다."));		
}

VOID CMNG_CHARSET_MANAGER::OnbIMPORT4() 
{
	if (AfxMessageBox(_T("Korean(TCHAR) -> Kata(TCHAR)를 반입하시겠습니까?"), MB_YESNO) != IDYES) return;
	CString strFileName = _T("KOR_TO_JPN_ROMA_TBL.txt");

	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);


	CString strLine;
	CString strC, strP;
	CString strMsg;

	CStdioFile fd;
	if (fd.Open(strFileName, CFile::modeRead | CFile::shareDenyWrite | CFile::typeBinary) != TRUE) return;
	
	TCHAR cUni;
	fd.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fd.SeekToBegin();
	}
	
	TCHAR pBuf[2049];
	INT pos;
	INT nTemp, nSuccess=0, nError=0, nTotal=0;

	CString strQuery;
	dm.StartFrame();
	INT nCount = 0;
	INT ids;
	while(TRUE) 
	{
		if (fd.ReadString(pBuf, 2048) == NULL) break;
		strLine.Format(_T("%s"), pBuf);
		pos = strLine.Find(_T("|"));
		if (pos == -1) continue;
		strC = strLine.Left(pos);
		strP = strLine.Mid(pos+1);
		strC.TrimLeft();			strC.TrimRight();
		strP.TrimLeft();			strP.TrimRight();
		pos = strP.Find(_T("|"));
		strP = strP.Mid(pos+1);
		strP.TrimLeft();			strP.TrimRight();
		strP = strP.Left(30);
		strP.TrimLeft();			strP.TrimRight();

		strQuery.Format(_T("INSERT INTO ESL_CHAR_SET_TBL (REC_KEY, FromSet, ToSet, FromChar, ToChar) VALUES (%s.NEXTVAL, 'KOREAN_CHAR', 'KATA_CHAR', '%s', '%s');"), dm.GetDefaultSEQName(), strC, strP);

		dm.AddFrame(strQuery);
	
		if (nCount % 20 == 0) {
			nTemp = dm.frame.GetCount();
			ids = dm.SendFrame();
				if (ids == 0) nSuccess += nTemp;
					else nError += nTemp;
				nTotal += nTemp;
			dm.EndFrame();
		}
		nCount++;
		strMsg.Format(_T("Success : %d - Error : %d - Total : %d"), nSuccess, nError, nTotal);
		GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(strMsg);
	}
	nTemp = dm.frame.GetCount();
	ids = dm.SendFrame();
		if (ids == 0) nSuccess += nTemp;
			else nError += nTemp;
		nTotal += nTemp;
	dm.EndFrame();
	strMsg.Format(_T("Success : %d - Error : %d - Total : %d"), nSuccess, nError, nTotal);
	GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(strMsg);

	AfxMessageBox(_T("완료되었습니다."));			
}

VOID CMNG_CHARSET_MANAGER::OnbIMPORT5() 
{
	if (AfxMessageBox(_T("Korean(TCHAR) -> Roma(TCHAR)를 반입하시겠습니까?"), MB_YESNO) != IDYES) return;
	CString strFileName = _T("KOR_TO_JPN_ROMA_TBL.txt");

	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);


	CString strLine;
	CString strC, strP;
	CString strMsg;

	CStdioFile fd;
	if (fd.Open(strFileName, CFile::modeRead | CFile::shareDenyWrite | CFile::typeBinary) != TRUE) return;
	
	TCHAR cUni;
	fd.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fd.SeekToBegin();
	}
	
	TCHAR pBuf[2049];
	INT pos;
	INT nTemp, nSuccess=0, nError=0, nTotal=0;

	CString strQuery;
	dm.StartFrame();
	INT nCount = 0;
	INT ids;
	while(TRUE) 
	{
		if (fd.ReadString(pBuf, 2048) == NULL) break;
		strLine.Format(_T("%s"), pBuf);
		pos = strLine.Find(_T("|"));
		if (pos == -1) continue;
		strC = strLine.Left(pos);
		pos = strLine.ReverseFind('|');
		strP = strLine.Mid(pos+1);
		strP.TrimLeft();			strP.TrimRight();

		strQuery.Format(_T("INSERT INTO ESL_CHAR_SET_TBL (REC_KEY, FromSet, ToSet, FromChar, ToChar) VALUES (%s.NEXTVAL, 'KOREAN_CHAR', 'ROMA_CHAR', '%s', '%s');"), dm.GetDefaultSEQName(), strC, strP);

		dm.AddFrame(strQuery);
	
		if (nCount % 20 == 0) {
			nTemp = dm.frame.GetCount();
			ids = dm.SendFrame();
				if (ids == 0) nSuccess += nTemp;
					else nError += nTemp;
				nTotal += nTemp;
			dm.EndFrame();
		}
		nCount++;
		strMsg.Format(_T("Success : %d - Error : %d - Total : %d"), nSuccess, nError, nTotal);
		GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(strMsg);
	}
	nTemp = dm.frame.GetCount();
	ids = dm.SendFrame();
		if (ids == 0) nSuccess += nTemp;
			else nError += nTemp;
		nTotal += nTemp;
	dm.EndFrame();
	strMsg.Format(_T("Success : %d - Error : %d - Total : %d"), nSuccess, nError, nTotal);
	GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(strMsg);

	AfxMessageBox(_T("완료되었습니다."));			
}

VOID CMNG_CHARSET_MANAGER::OnbEXPORT3() 
{
	ExportToDataFile(_T("KOREAN_CHAR"), _T("HIRA_CHAR"));	
}

VOID CMNG_CHARSET_MANAGER::OnbEXPORT4() 
{
	ExportToDataFile(_T("KOREAN_CHAR"), _T("KATA_CHAR"));	
}

VOID CMNG_CHARSET_MANAGER::OnbEXPORT5() 
{
	ExportToDataFile(_T("KOREAN_CHAR"), _T("ROMA_CHAR"));	
}

VOID CMNG_CHARSET_MANAGER::OnbTEST3() 
{
	CESL_CharSetMgr m;
	m.InitData(_T("KOREAN_CHAR"), _T("HIRA_CHAR"));
	
	INT ids;
	CString strTo;
	DWORD dSTART, dFINISH, dELA;
	dSTART = GetTickCount();
	ids = m.Find(_T("힛"), strTo);
	dFINISH = GetTickCount();
	dELA = dFINISH-dSTART;
	CString strMsg;
	strMsg.Format(_T("(%s) Max %d.%d sec"), strTo, dELA/1000, dELA%1000);
	AfxMessageBox(strMsg);
}

VOID CMNG_CHARSET_MANAGER::OnbTEST4() 
{
	CESL_CharSetMgr m;
	m.InitData(_T("KOREAN_CHAR"), _T("KATA_CHAR"));
	
	INT ids;
	CString strTo;
	DWORD dSTART, dFINISH, dELA;
	dSTART = GetTickCount();
	ids = m.Find(_T("힛"), strTo);
	dFINISH = GetTickCount();
	dELA = dFINISH-dSTART;
	CString strMsg;
	strMsg.Format(_T("(%s) Max %d.%d sec"), strTo, dELA/1000, dELA%1000);
	AfxMessageBox(strMsg);
}

VOID CMNG_CHARSET_MANAGER::OnbTEST5() 
{
	CESL_CharSetMgr m;
	m.InitData(_T("KOREAN_CHAR"), _T("ROMA_CHAR"));
	
	INT ids;
	CString strTo;
	DWORD dSTART, dFINISH, dELA;
	dSTART = GetTickCount();
	ids = m.Find(_T("힛"), strTo);
	dFINISH = GetTickCount();
	dELA = dFINISH-dSTART;
	CString strMsg;
	strMsg.Format(_T("(%s) Max %d.%d sec"), strTo, dELA/1000, dELA%1000);
	AfxMessageBox(strMsg);
}

HBRUSH CMNG_CHARSET_MANAGER::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
