// BO_ACQ_FileExportMgr.cpp: implementation of the CBO_ACQ_FileExportMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BO_ACQ_FileExportMgr.h"
#include "Request_DataExportDlg.h"
#include <direct.h>
#include "../../../공통/INCLUDE/BO_ACQ.h"
// 2011.09.08 ADD : 3000Api
#include "BO_LOC_3000Api.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBO_ACQ_FileExportMgr::CBO_ACQ_FileExportMgr(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CBO_ACQ_FileExportMgr::IDD, pParent)
{
	m_pParentMgr = pParent;
}

CBO_ACQ_FileExportMgr::~CBO_ACQ_FileExportMgr()
{

}

INT CBO_ACQ_FileExportMgr::FileExportProc()
{

	INT ids;

	// 파일을 저장하기 위해 사용자가 원하는 위치를 얻어온다.
	ids = GetFilePath();
	if(ids < 0) return -1;

	BOOL bCreate;
	// 지정 경로에 IMPORT_EXPORT 폴더 생성
	m_strExportFilePath += _T("\\IMPORT_EXPORT");
	bCreate = CreateDirectory(m_strExportFilePath, NULL);
	if(!bCreate) {
		AfxMessageBox(_T("지정 위치에 같은 이름의 EXPORT 폴더가 존재합니다.\r\n")
			          _T("기존 폴더를 다른 이름으로 변경한 후 계속하시기 바랍니다."));
		return -1;
	}

	// 2005-01-06 순서 바꿈
	// 반출 테이블 수 만큼 루프 돌면서 검색 결과를 DM에 저장하고 DM의 내용을 파일에 옮긴다.
	// SM 불러오기.
	if (InitESL_Mgr(_T("K2UPGRADE_단행_자료반출")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return -1;
	}			

	// 마스터 파일 만들기 
	ids = SetMasterFileInfo();
	if(ids < 0) {
		AfxMessageBox(_T("반출정보파일 생성 실패!"));
		return -1;
	}

	BOOL bFileExist;
	CString strSourceFile, strDestFile;

	for(INT Idx=0; Idx < m_nTableCount; Idx++)
	{

		// 1.테이블명.ctl 파일 생성 및 내용 저장
		// 미리 cfg에 저장해 놓은 각 테이블의 ctl 파일을 복사한다.
		strSourceFile.Empty();	strDestFile.Empty();
		strSourceFile.Format(_T("..\\cfg\\%s.ctl"), m_strTableNameArray[Idx]);
		strDestFile = m_strExportFilePath + _T("\\") + m_strTableNameArray[Idx] + _T(".ctl");
		bFileExist = CopyFile(strSourceFile, strDestFile, FALSE);
		if(!bFileExist) return -1;

		if((SPECIES_TYPE)m_nSpeciesType == BO_ORDER)
		{
			ids = CopyFromDMToFile_ORDER(Idx);
			if(ids < 0) return -1;
		}
		else if((SPECIES_TYPE)m_nSpeciesType == BO_REQUEST)
		{
			ids = CopyFromDMToFile_REQUEST(Idx);
			if(ids < 0) return -1;
		}
	}

	// 2005-01-16
	// 검수자료 파일반출의 경우 비치희망정보도 반출하도록 수정.
	// DM_단행_파일반출_구입정보에서 rec_key를 읽어서 그 rec_key를 acq_key로 갖는 비치희망정보를 읽어 파일에 저장한다.
	if((SPECIES_TYPE)m_nSpeciesType == BO_ORDER)
	{
		strSourceFile.Empty();	strDestFile.Empty();
		strSourceFile.Format(_T("..\\cfg\\BO_FURNISH_TBL.ctl"));
		strDestFile = m_strExportFilePath + _T("\\BO_FURNISH_TBL.ctl");
		bFileExist = CopyFile(strSourceFile, strDestFile, FALSE);
		if(!bFileExist) return -1;

		ids = CopyFromPurchaseToFurnish_ORDER();
		if(ids < 0)
			return -1;
	}

	return 0;
}

INT CBO_ACQ_FileExportMgr::GetFilePath()
{
	//사용자가 선택한 폴더 경로 얻어오는 소스.
	ITEMIDLIST        *pidlBrowse;
    TCHAR            pszPathname[MAX_PATH];
 	TCHAR    szCurrentDir[512];
	_tgetcwd(szCurrentDir, 512);

	BROWSEINFO BrInfo;
	// BrInfo.hwndOwner = GetSafeHwnd();
    BrInfo.pidlRoot = NULL;

    memset( &BrInfo, 0, sizeof(BrInfo) );
    BrInfo.pszDisplayName = pszPathname;
    BrInfo.lpszTitle = _T("파일을 저장할 위치를 선택하세요");
    BrInfo.ulFlags = BIF_RETURNONLYFSDIRS ;

    // 다이얼로그를 띄우기
    pidlBrowse = ::SHBrowseForFolder(&BrInfo);    

    if( pidlBrowse != NULL)
    {
        // 패스를 얻어옴
        ::SHGetPathFromIDList(pidlBrowse, pszPathname);    
    }
	else return -1;

	_tchdir(szCurrentDir);   

	m_strExportFilePath = pszPathname;

	return 0;

}


/************************************************
 err -1000; //파일 열기 실패.
 err -1001; //종타입에 해당하는 항목이 없음.
*************************************************/
INT CBO_ACQ_FileExportMgr::SetMasterFileInfo()
{
	BOOL bError;
	INT i;
	CString strFileName, strWrite;

	CStdioFile Masterfile;
	strFileName.Format(_T("%s\\ExportFileInfo.txt"), m_strExportFilePath);

	// 2005-01-16
	CString strQuery, strResult;
	strQuery.Empty();	strResult.Empty();
	CESL_DataMgr * TmpDM;
	TmpDM = FindDM(_T("DM_단행_자료반출_비치희망정보"));
	if(TmpDM == NULL) return -1002;

	bError = Masterfile.Open(strFileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);
	if(!bError) return -1000;
	
	if( 0 == Masterfile.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		Masterfile.Write( &cUni, sizeof(TCHAR));
	}
		
	switch((SPECIES_TYPE)m_nSpeciesType)
	{
		case BO_REQUEST:
			// 반출할 테이블을 배열에 넣는다.
			m_nTableCount = 0;
			m_strTableNameArray[0] = _T("BO_FURNISH_TBL");	
			m_strDMArray[0] = _T("DM_단행_자료반출_비치희망정보");	
			m_nTableCount++;
			
			// 반출정보입력
			Masterfile.WriteString(_T("#반입종타입=BO_REQUEST\r\n"));
			// 이부분에 검색쿼리항목들을 넣어 준다.
			for(i = 0; i< m_pExportColumn.GetSize(); i++)
			{
				strWrite.Format(_T("#반출항목=%s\r\n"), m_pExportColumn.GetAt(i));
				Masterfile.WriteString(strWrite);
			}
			Masterfile.WriteString(_T("BO_FURNISH_TBL\r\n"));
			break;
		case BO_ORDER:
			// 반출할 테이블을 배열에 넣는다.
			m_nTableCount = 0;
			m_strTableNameArray[0] = _T("BO_PURCHASE_SEQ_NO_TBL");	m_strDMArray[0] = _T("DM_단행_자료반출_차수정보");		
			m_strTableNameArray[1] = _T("BO_PURCHASE_WORK_NO_TBL");	m_strDMArray[1] = _T("DM_단행_자료반출_작업번호정보");	
			m_strTableNameArray[2] = _T("BO_SPECIES_TBL");			m_strDMArray[2] = _T("DM_단행_자료반출_종정보");		
			m_strTableNameArray[3] = _T("BO_BOOK_TBL");				m_strDMArray[3] = _T("DM_단행_자료반출_책정보");		
			m_strTableNameArray[4] = _T("BO_EBOOK_TBL");			m_strDMArray[4] = _T("DM_단행_자료반출_EBOOK정보");		
			m_strTableNameArray[5] = _T("BO_PURCHASEINFO_TBL");		m_strDMArray[5] = _T("DM_단행_자료반출_구입정보");		
			m_strTableNameArray[6] = _T("IDX_BO_TBL");				m_strDMArray[6] = _T("DM_단행_자료반출_색인정보");	
			// 2005-01-16
			// m_strTableNameArray[7] = _T("IDX_TMP_BO_TBL");			m_strDMArray[7] = _T("DM_단행_자료반출_색인임시정보");	

			m_strSelectQueryArray[0].Format(_T("Q.ACQ_YEAR = %s AND Q.SEQ_NO = %s AND Q.MANAGE_CODE = UDF_MANAGE_CODE"), 
											m_strAcqYear, m_strGroupInfo);  m_nTableCount++;
			m_strSelectQueryArray[1].Format(_T("Q.ACQ_YEAR = %s AND Q.SEQ_NO = %s AND Q.MANAGE_CODE = UDF_MANAGE_CODE"), 
											m_strAcqYear, m_strGroupInfo); m_nTableCount++;
			m_strSelectQueryArray[2].Format(_T("P.ORDER_YEAR = '%s' AND I.WORK_CODE = '%s' AND P.ORDER_SEQ_NO = %s AND ")
											_T("P.REC_KEY IN ( SELECT ACQ_KEY FROM BO_BOOK_TBL WHERE ACQ_CODE = '1') AND I.MANAGE_CODE = UDF_MANAGE_CODE"),   //ACQ_CODE = '1':구입
											m_strAcqYear, m_strWorkCode, m_strGroupInfo);	m_nTableCount++;
			m_strSelectQueryArray[3].Format(_T("P.ORDER_YEAR = '%s' AND I.WORK_CODE = '%s' AND P.ORDER_SEQ_NO = %s AND B.ACQ_CODE = '1' AND I.MANAGE_CODE = UDF_MANAGE_CODE"),
											m_strAcqYear, m_strWorkCode, m_strGroupInfo);	m_nTableCount++;
			m_strSelectQueryArray[4].Format(_T("P.ORDER_YEAR = '%s' AND I.WORK_CODE = '%s' AND P.ORDER_SEQ_NO = %s AND B.ACQ_CODE = '1' AND I.MANAGE_CODE = UDF_MANAGE_CODE"),
											m_strAcqYear, m_strWorkCode, m_strGroupInfo);	m_nTableCount++;
			m_strSelectQueryArray[5].Format(_T("P.ORDER_YEAR = '%s' AND I.WORK_CODE = '%s' AND P.ORDER_SEQ_NO = %s AND I.MANAGE_CODE = UDF_MANAGE_CODE"),
											m_strAcqYear, m_strWorkCode, m_strGroupInfo);	m_nTableCount++;
			m_strSelectQueryArray[6].Format(_T("P.ORDER_YEAR = '%s' AND I.WORK_CODE = '%s' AND P.ORDER_SEQ_NO = %s AND I.MANAGE_CODE = UDF_MANAGE_CODE"),
											m_strAcqYear, m_strWorkCode, m_strGroupInfo);	m_nTableCount++;
			// m_strSelectQueryArray[7].Format(_T("P.ORDER_YEAR = '%s' AND I.WORK_CODE = '%s' AND P.ORDER_SEQ_NO = %s"),
			//								m_strAcqYear, m_strWorkCode, m_strGroupInfo);	m_nTableCount++;

			// 반출정보입력
			Masterfile.WriteString(_T("#반입종타입=BO_ORDER\r\n"));
			strWrite.Format(_T("#수입년도=%s\r\n"), m_strAcqYear);
			Masterfile.WriteString(strWrite);
			strWrite.Format(_T("#차수번호=%s\r\n"), m_strGroupInfo);
			Masterfile.WriteString(strWrite);
			strWrite.Format(_T("#업무구분=%s\r\n"), m_strWorkCode);
			Masterfile.WriteString(strWrite);

			for(i=0; i < m_nTableCount; i++){
				strWrite.Format(_T("%s\r\n"), m_strTableNameArray[i]);
				Masterfile.WriteString(strWrite);
			}
			break;
		default: 
			Masterfile.Close();
			return -1001; 
			break;
	}

	Masterfile.Close();
	return 0;
}

INT CBO_ACQ_FileExportMgr::CopyFromDMToFile_ORDER(INT Idx)
{
	INT ids;
	INT nRowCount = 0;   // 검색 결과 Row 수
	CString strQuery, strFile, strRowData, strTempData;
	CESL_DataMgr  *pDM;
	CStdioFile tablefile;

	// 2. DM에 쿼리 날려서 결과 받아오기.
	pDM = FindDM(m_strDMArray[Idx]);
	ids = pDM->RefreshDataManager(m_strSelectQueryArray[Idx]);
	if(ids < 0) return -1;
	nRowCount = pDM->GetRowCount();
	
	// 3. 테이블명.dat 파일 생성 및 내용 저장.
	strFile.Empty(); 
	strFile = m_strExportFilePath + _T("\\") + m_strTableNameArray[Idx] + _T(".dat");
	tablefile.Open(strFile, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary);

	if( 0 == tablefile.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		tablefile.Write( &cUni, sizeof(TCHAR));
	}
		
	// 4. 파일에 검색 결과 DM의 RowCount만큼 루프 돌면서 각 컬럼의 내용을 저장한다.
	strRowData.Empty();
	for(INT i=0; i< nRowCount; i++)
	{
		for(INT j=0; j < pDM->GetColCount(); j++)
		{
			strTempData.Empty();
			strTempData = pDM->GetCellData(i, j);
			// 2005-01-12 
			// 데이터에 엔터가 있는 경우는 엔터를 없애고 반출한다.
			strTempData.Replace(_T("\r\n"), _T(""));
			strRowData += _T("<start>") + strTempData + _T("<end>");
		}
		strRowData += _T("|\r\n");
		tablefile.WriteString(strRowData);
		strRowData.Empty();
	}

	tablefile.Close();
	pDM = NULL;
	
	return 0;
}

INT CBO_ACQ_FileExportMgr::CopyFromDMToFile_REQUEST(INT Idx)
{
	INT ids;
	INT nRowCount = 0;   // 검색 결과 Row 수
	CString strFile, strQuery, strRec_key, strRowData, strTempData;
	CESL_DataMgr  *pDM;
	CStdioFile tablefile;

	// 2. DM에 쿼리 날려서 결과 받아오기.
	pDM = FindDM(m_strDMArray[Idx]);
	
	// 3. 테이블명.dat 파일 생성 및 내용 저장.
	strFile.Empty();
	strFile = m_strExportFilePath + _T("\\") + m_strTableNameArray[Idx] + _T(".dat");
	tablefile.Open(strFile, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary);

	if( 0 == tablefile.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		tablefile.Write( &cUni, sizeof(TCHAR));
	}
		
	// 4. 파일에 검색 결과 DM의 RowCount만큼 루프 돌면서 각 컬럼의 내용을 저장한다.
	strRowData.Empty();

	nRowCount = m_SelectedRowIdxArray->GetSize();

	for(INT i=0; i< nRowCount; i++)
	{
		strRec_key = m_SelectedRowIdxArray->GetAt(i);
		strQuery.Format(_T("REC_KEY = %s"), strRec_key);
		ids = pDM->RefreshDataManager(strQuery);
		if(ids < 0) return -1;
		if(pDM->GetRowCount() != 1 ) return -1;

		for(INT j=0; j < pDM->GetColCount(); j++)
		{
			strTempData.Empty();
			strTempData = pDM->GetCellData(0, j);
			// 2005-01-12 
			// 데이터에 엔터가 있는 경우는 엔터를 없애고 반출한다.
			strTempData.Replace(_T("\r\n"), _T(""));
			strRowData += _T("<start>") + strTempData + _T("<end>");
		}
		strRowData += _T("|\r\n");
		tablefile.WriteString(strRowData);
		strRowData.Empty();
	}

	tablefile.Close();
	pDM = NULL;

	return 0;
}

INT CBO_ACQ_FileExportMgr::SetExportColumn()
{

	CRequest_DataExportDlg Dlg(m_pParentMgr);
	if(Dlg.DoModal() == IDCANCEL) return -1;

	for(INT i=0; i < Dlg.m_strCheckArray.GetSize(); i++)
	{
		m_pExportColumn.Add(Dlg.m_strCheckArray.GetAt(i));
	}

	return 0;
}

INT CBO_ACQ_FileExportMgr::CopyFromPurchaseToFurnish_ORDER()
{
	// 2005-01-16
	// 비치희망자료인 경우 DM을 비치희망자료_파일반출과 같이 쓰기 때문에 
	// 위와 같이 하지 못하고, 차수정보TBL에서 acq_key를 얻어와서 그 acq_key를 갖는 비치희망자료를 검색하여 반출한다.

	INT ids;
	INT nPurchaseRowCount = 0;   // 검색 결과 Row 수
	INT nFurnishCount = 0;		 // acq_key를 만족하는 데이터갯수
	INT nColCount = 0;			 // 비치희망정보의 컬럼갯수
	CString strFile, strAcq_key, strQuery, strTempData, strRowData;
	CESL_DataMgr * pPurchaseDM;
	CESL_DataMgr * pFurnishDM;
	CStdioFile tablefile;

	pPurchaseDM = FindDM(_T("DM_단행_자료반출_구입정보"));
	pFurnishDM = FindDM(_T("DM_단행_자료반출_비치희망정보"));
	if(pPurchaseDM == NULL || pFurnishDM== NULL) return -1001;

	// 3. 테이블명.dat 파일 생성 및 내용 저장.
	strFile.Empty();
	strFile = m_strExportFilePath + _T("\\BO_FURNISH_TBL.dat");
	tablefile.Open(strFile, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary);

	if( 0 == tablefile.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		tablefile.Write( &cUni, sizeof(TCHAR));
	}
		
	// 4. 파일에 검색 결과 구입정보DM의 RowCount만큼 루프 돌면서 각 레코드의 REC_Key를 가져온다.
	nPurchaseRowCount = pPurchaseDM->GetRowCount();

	for(INT i=0; i< nPurchaseRowCount; i++)
	{
		strAcq_key = pPurchaseDM->GetCellData(_T("BP_REC_KEY"), i);
		if(strAcq_key.IsEmpty()) return -1;
		strQuery.Format(_T("ACQ_KEY = %s"), strAcq_key);

		ids = pFurnishDM->RefreshDataManager(strQuery);
		if(ids < 0) return -1;

		nFurnishCount = pFurnishDM->GetRowCount();
		if(nFurnishCount < 1 ) continue;
		
		// 2011.09.08 ADD : 3000Api
		if(TRUE == m_pInfo->m_bLibSecuUse)
		{
			CStringArray asDes1,asDes2,asDes3,asDes4,asDes5;
			CString strGetData;
			for(INT i = 0; i < pFurnishDM->GetRowCount(); i++)
			{
				pFurnishDM->GetCellData(_T("주민등록번호"), i, strGetData);
				asDes1.Add(strGetData);
				pFurnishDM->GetCellData(_T("주소"), i, strGetData);
				asDes2.Add(strGetData);
				pFurnishDM->GetCellData(_T("전화번호"), i, strGetData);
				asDes3.Add(strGetData);
				pFurnishDM->GetCellData(_T("핸드폰"), i, strGetData);
				asDes4.Add(strGetData);
				pFurnishDM->GetCellData(_T("전자우편"), i, strGetData);
				asDes5.Add(strGetData);
			}
			CBO_LOC_3000Api *pApi = new CBO_LOC_3000Api(this);
			if(NULL != pApi)
			{
				if(TRUE == pApi->CheckConnection())
				{
					if(TRUE == pApi->ConnectSocket()) 
					{
						pApi->UserInfoDecrytion(_T("BO_FURNISH_TBL"), _T("CIVIL_NO"), asDes1);
						pApi->CloseSocket();
					}
					if(TRUE == pApi->ConnectSocket()) 
					{
						pApi->UserInfoDecrytion(_T("BO_FURNISH_TBL"), _T("ADDRESS"), asDes2);
						pApi->CloseSocket();
					}
					if(TRUE == pApi->ConnectSocket()) 
					{
						pApi->UserInfoDecrytion(_T("BO_FURNISH_TBL"), _T("TEL"), asDes3);
						pApi->CloseSocket();
					}
					if(TRUE == pApi->ConnectSocket()) 
					{
						pApi->UserInfoDecrytion(_T("BO_FURNISH_TBL"), _T("HANDPHONE"), asDes4);
						pApi->CloseSocket();
					}
					if(TRUE == pApi->ConnectSocket()) 
					{
						pApi->UserInfoDecrytion(_T("BO_FURNISH_TBL"), _T("EMAIL"), asDes5);
						pApi->CloseSocket();
					}
					for(i = 0; i < pFurnishDM->GetRowCount(); i++)
					{
						pFurnishDM->SetCellData(_T("주민등록번호"), asDes1.GetAt(i), i);
						pFurnishDM->SetCellData(_T("주소"), asDes2.GetAt(i), i);
						pFurnishDM->SetCellData(_T("전화번호"), asDes3.GetAt(i), i);
						pFurnishDM->SetCellData(_T("핸드폰"), asDes4.GetAt(i), i);
						pFurnishDM->SetCellData(_T("전자우편"), asDes5.GetAt(i), i);
					}
				}
				delete pApi;
				pApi = NULL;
			}
			asDes1.RemoveAll(); asDes1.FreeExtra();
			asDes2.RemoveAll(); asDes2.FreeExtra();
			asDes3.RemoveAll(); asDes3.FreeExtra();
			asDes4.RemoveAll(); asDes4.FreeExtra();
			asDes5.RemoveAll(); asDes5.FreeExtra();
		}

		nColCount = pFurnishDM->GetColCount();
		for(INT j=0; j < nFurnishCount; j++)
		{
			for(INT k=0; k < nColCount; k++)
			{
				strTempData.Empty();
				strTempData = pFurnishDM->GetCellData(j, k);
				// 2005-01-12 
				// 데이터에 엔터가 있는 경우는 엔터를 없애고 반출한다.
				strTempData.Replace(_T("\r\n"), _T(""));
				strRowData += _T("<start>") + strTempData + _T("<end>");
			}

			strRowData += _T("|\r\n");
			tablefile.WriteString(strRowData);
			strRowData.Empty();
		}
	}

	tablefile.Close();

	// MasterFile에 _T("BO_FURNISH_TBL") 추가.
	BOOL bError;
	CString strFileName;
	CStdioFile Masterfile;
	strFileName.Format(_T("%s\\ExportFileInfo.txt"), m_strExportFilePath);
	bError = Masterfile.Open(strFileName, CFile::modeWrite | CFile::typeBinary);
	if(!bError) return -1000;

	if( 0 == Masterfile.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		Masterfile.Write( &cUni, sizeof(TCHAR));
	}
		
	Masterfile.SeekToEnd();
	Masterfile.WriteString(_T("BO_FURNISH_TBL"));
	Masterfile.Close();

	return 0;
}
