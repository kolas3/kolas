#include "stdafx.h"
#include "BO_LOC_3180.h"
#include "BO_LOC_3182.h"
#include "BO_LOC_3183.h"
#include "..\\BL_LOC_LOAN\\BO_LOC_3170.h"	
#include "..\\BL_LOC_LOAN\\BL_LOC_LOAN_PROC.h"	
#include "..\\..\\..\\.\\공통\\MARC_CONVERT\\MarcConvertApi.h"
#include "SPF_Work.h"
#include <direct.h>
#include <stdio.h>

#include "BO_LOC_3000Api.h"
#include "..\LOC_CommonAPI\LocCommonAPI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CBO_LOC_3180::CBO_LOC_3180(CESL_Mgr* pParent /*=NULL*/, CLoanShareManager* pLoanShareManager, CLonaManageValue *pLona)
	: CESL_Mgr(IDD, pParent)
{
	m_pLona = pLona;
	m_pCheckLoan = new CCheckLoanBook( pParent , m_pLona);

	m_pSpfWork = new CSPF_Work(this); 

	m_pLoanShareManager = pLoanShareManager;

	m_pParent = pParent;
	
	m_bAppendCheck = FALSE;
		
	m_sDelayMoney = _T("");
	m_sShareUserRecKey = _T("");
	m_sShareLoanRecKey = _T("");
	m_sReservedLoanKey = _T("");
	m_sFailDesc = _T("");
	m_sFailCode = _T("");
	m_sSEQ_NO = _T("");
	m_sTime = _T("");
	m_bIsReservedBookByMe = FALSE;	
	m_nReturnType = 0;
	m_nReturnBookType = 0;
	m_nLoanType = 0;
	m_nLoanBookType = 0;
	m_nFailCnt = 0;
	m_nSuccessCnt = 0;
	m_nSelectCnt = 0;
	m_sTYPE = _T("");
	m_sREG_NO = _T("");
	m_sUSER_NO = _T("");
	m_nCurrentIndex = 0;
}

CBO_LOC_3180::~CBO_LOC_3180()
{
	if( m_pCheckLoan != NULL ) 
		delete m_pCheckLoan;

	if( m_pSpfWork != NULL ) 
		delete m_pSpfWork;
}

VOID CBO_LOC_3180::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3180)	
	DDX_Check(pDX, IDC_cAppendCheck, m_bAppendCheck);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_LOC_3180, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3180)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_btnFileLoad, OnbtnFileLoad)
	ON_BN_CLICKED(IDC_bSort, OnbSort)
	ON_BN_CLICKED(IDC_bSave, OnbSave)
	ON_BN_CLICKED(IDC_bModify, OnbModify)
	ON_BN_CLICKED(IDC_b3100_RELOAD, OnbRELOAD)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CBO_LOC_3180::OnInitDialog() 
{
	CDialog::OnInitDialog();

	EnableThemeDialogTexture(GetSafeHwnd()); 
	
	if(InitESL_Mgr(_T("SM_BO_LOC_3180")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	InitMainGrid();
	
	return TRUE;  
	              
}

INT CBO_LOC_3180::InitMainGrid()
{
	EFS_BEGIN
	INT ids=0;

	CBL_LOC_LOAN_PROC *pLoanProc=NULL;
	pLoanProc = new CBL_LOC_LOAN_PROC(pParentMgr , m_pLoanShareManager  , m_pLona);
	
	::PostMessage( m_pParent->m_hWnd, WM_KEYDOWN, VK_ESCAPE, 1 );

	if( pLoanProc != NULL ) delete pLoanProc;

	return 0;
	EFS_END
	return -1;
}

VOID CBO_LOC_3180::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);	
}

BOOL CBO_LOC_3180::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{		
	return CDialog::Create(IDD, pParentWnd);
}

VOID CBO_LOC_3180::OnbtnFileLoad() 
{
	EFS_BEGIN
	INT ids=0;
	ids = FileLoad();
	if(ids < 0){
		CString tmpstr;
		tmpstr.Format(_T("ERROR CBO_LOC_3180::OnbtnFileLoad() := FileLoad() [ids=%d]"), ids);
		AfxMessageBox(tmpstr);
	}

	return;
	EFS_END
	return;
}

INT CBO_LOC_3180::FileLoad()
{
EFS_BEGIN

	INT i=0, ids=0;
	CString sFileName;
	TCHAR szCurrentDir[1024];

	_tgetcwd(szCurrentDir, 1024);
	CFileDialog fd(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT);
	if (fd.DoModal() != IDOK) return 100;
	_tchdir(szCurrentDir);
		
	sFileName = fd.GetPathName();

	
	CESL_DataMgr *pDM = FindDM( _T("DM_BO_LOC_3180") );
	if( pDM == NULL ){
		return -1;
	}

	CESL_ControlMgr *pCM = FindCM( _T("CM_BO_LOC_3180") );
	if( pCM == NULL ) {
		return -3;
	}

	CStdioFile cFile;
	CFileException eF;
	
	if( cFile.Open( sFileName , CFile::modeRead | CFile::shareDenyNone | CFile::typeBinary , &eF ) == 0 )
	{
		return -5;
	}

	const INT fieldCnt = 16;
	CString fieldName[ fieldCnt ] = {
		_T("REC_KEY"), _T("WORKTYPE"), _T("USER_NO"), _T("REG_NO"), _T("PROC_DATE"),
		_T("RETURN_DATE"), _T("LOAN_DATE"), _T("RETURN_PLAN_DATE"), _T("LOAN_STOP_DATE"), _T("BOOK_STATUS"),
		_T("BOOK_STATUS_DESC"), _T("ARREAR"), _T("REPARATION"), _T("WORK_START_TIME"), _T("WORKER"),
		_T("SEQ_NO")
	};

	CString fieldType[ fieldCnt ] = {
		_T("NUMERIC"), _T("STRING"), _T("STRING"), _T("STRING"), _T("NUMERIC"),
		_T("NUMERIC"), _T("NUMERIC"), _T("NUMERIC"), _T("NUMERIC"), _T("STRING"),
		_T("STRING"), _T("STRING"), _T("STRING"), _T("NUMERIC"), _T("STRING"),
		_T("NUMERIC")
	};

	CString fieldData[ fieldCnt ];

	CString sLine;
	CStringArray sData;
	CString pk;

	pDM->MakeRecKey(m_sSEQ_NO);

	pDM->GetOneValueQuery(_T("SELECT TO_CHAR(SYSDATE, 'YYYY/MM/DD:HH24-MI-SS') FROM DUAL"), m_sTime);
	if(m_sTime.IsEmpty()){
		return -7;
	}

	INT nLineCnt=0;
	INT nSuccessCnt=0;
	INT nFailCnt=0;
	INT	nCheck;
	INT	nPos;
	CString tmpREG_NO;
	CString sQuery;
	CString tmpREC_KEY;
	
	TCHAR cUni;
	cFile.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		cFile.SeekToBegin();
	}		
	BOOL bIsEnd = FALSE;

	TCHAR lpBuffer[4096];
	DWORD dwPos = cFile.GetPosition();
	while(TRUE)
	{
		memset(lpBuffer, '\0', 4096);
		dwPos = cFile.GetPosition();
		INT nCount = cFile.Read(lpBuffer, 4096);
		if(!nCount)
		{
			break;
		}

		CharConvert cv;
		TCHAR*       szTmp = NULL;
		INT         len   = 0;
		
		cv.MultiByteToUnicode((char*)lpBuffer, &szTmp, &len);		
		if( 0 < len)
		{
			sLine.Format( _T("%s"), szTmp );
			delete [] szTmp;
		}
		INT nFindPos = sLine.Find(_T("\n"));		
		if(-1 == nFindPos)
		{
			if(FALSE == bIsEnd)
			{
				sLine = sLine + _T("\r\n");
				nFindPos = sLine.Find(_T("\n"));

				bIsEnd = TRUE;
			}
			else break; 
		}

		sLine = sLine.Left(nFindPos);
		cFile.Seek(dwPos + (DWORD)nFindPos+1, CFile::begin);	
		
		sLine.Replace(_T("(null)"), _T(""));

 		sData.RemoveAll();
 		tmpREG_NO.Empty();
 		sQuery.Empty();	

		nCheck = 0;
		nPos = 0;
		while ( nPos >= 0 )
		{			
			nPos = sLine.Find(_T(";"), nPos+1);
			if ( nPos >= 0 ) nCheck++;
			if ( nCheck > 10 ) break;
		}
		if ( nCheck != 10 ) 
		{
			
			AfxMessageBox(_T("형식에 맞지 않는 파일입니다."));
			return 0;
		}

		MakeArray(sLine, _T(";"), sData);
		for(i=0;i<fieldCnt;i++){
			fieldData[i].Empty();
		}
		
		nLineCnt++;
		
		tmpREG_NO = sData.GetAt(2);
		MakeRegNoSpaceToZero(tmpREG_NO);
	
		if(sData.GetAt(0).Compare(_T("0"))==0){
			sQuery.Format(_T("SELECT REC_KEY FROM CO_OFFLINE_LOAN_TBL WHERE USER_NO='%s' and REG_NO='%s' AND MANAGE_CODE = UDF_MANAGE_CODE and WORKTYPE='대출' AND TO_CHAR(LOAN_DATE,'YYYY/MM/DD:HH24-MI-SS')='%s'"), sData.GetAt(1), tmpREG_NO, sData.GetAt(3));
		}else if(sData.GetAt(0).Compare(_T("1"))==0){
			sQuery.Format(_T("SELECT REC_KEY FROM CO_OFFLINE_LOAN_TBL WHERE USER_NO='%s' and REG_NO='%s' AND MANAGE_CODE = UDF_MANAGE_CODE and WORKTYPE='반납' AND TO_CHAR(RETURN_DATE,'YYYY/MM/DD:HH24-MI-SS')='%s'"), sData.GetAt(1), tmpREG_NO, sData.GetAt(6));
		}else if(sData.GetAt(0).Compare(_T("2"))==0){
			sQuery.Format(_T("SELECT REC_KEY FROM CO_OFFLINE_LOAN_TBL WHERE USER_NO='%s' and REG_NO='%s' AND MANAGE_CODE = UDF_MANAGE_CODE and WORKTYPE='무인대출' AND TO_CHAR(LOAN_DATE,'YYYY/MM/DD:HH24-MI-SS')='%s'"), sData.GetAt(1), tmpREG_NO, sData.GetAt(3));
		}else if(sData.GetAt(0).Compare(_T("3"))==0){
			sQuery.Format(_T("SELECT REC_KEY FROM CO_OFFLINE_LOAN_TBL WHERE USER_NO='%s' and REG_NO='%s' AND MANAGE_CODE = UDF_MANAGE_CODE and WORKTYPE='무인반납' AND TO_CHAR(RETURN_DATE,'YYYY/MM/DD:HH24-MI-SS')='%s'"), sData.GetAt(1), tmpREG_NO, sData.GetAt(6));
		}
		pDM->GetOneValueQuery(sQuery, tmpREC_KEY);
		if(!tmpREC_KEY.IsEmpty()){
			nFailCnt++;
			continue;
		}

		pDM->StartFrame();
		pDM->MakeRecKey(pk);
		pDM->InitDBFieldData();

		for(i=0;i<fieldCnt;i++){
			if(fieldName[i].Compare(_T("REC_KEY"))==0){
				fieldData[i] = pk;
			}else if(fieldName[i].Compare(_T("WORKTYPE"))==0){
				if(sData.GetAt(0).Compare(_T("0"))==0){
					fieldData[i] = _T("대출");
				}else if(sData.GetAt(0).Compare(_T("1"))==0){
					fieldData[i] = _T("반납");
				}else if(sData.GetAt(0).Compare(_T("2"))==0){
					fieldData[i] = _T("무인대출");
				}else if(sData.GetAt(0).Compare(_T("3"))==0){
					fieldData[i] = _T("무인반납");
				}
			}else if(fieldName[i].Compare(_T("USER_NO"))==0){
				fieldData[i] = sData.GetAt(1);
				if(fieldData[i].IsEmpty()){
					fieldData[i] = _T("NULL");
				}
			}else if(fieldName[i].Compare(_T("REG_NO"))==0){
				fieldData[i] = tmpREG_NO;
			}else if(fieldName[i].Compare(_T("LOAN_DATE"))==0){
				if(!sData.GetAt(3).IsEmpty()) fieldData[i].Format(_T("to_date('%s','YYYY/MM/DD:HH24-MI-SS')"), sData.GetAt(3));
			}else if(fieldName[i].Compare(_T("RETURN_PLAN_DATE"))==0){
				if(!sData.GetAt(4).IsEmpty()) fieldData[i].Format(_T("to_date('%s','YYYY/MM/DD')"), sData.GetAt(4));
			}else if(fieldName[i].Compare(_T("BOOK_STATUS"))==0){
				fieldData[i] = sData.GetAt(5);
			}else if(fieldName[i].Compare(_T("BOOK_STATUS_DESC"))==0){
				fieldData[i] = BookStatusCodeToDesc(sData.GetAt(5));
			}else if(fieldName[i].Compare(_T("RETURN_DATE"))==0){
				if(!sData.GetAt(6).IsEmpty()) fieldData[i].Format(_T("to_date('%s','YYYY/MM/DD:HH24-MI-SS')"), sData.GetAt(6));
			}else if(fieldName[i].Compare(_T("ARREAR"))==0){
				fieldData[i] = sData.GetAt(7);
			}else if(fieldName[i].Compare(_T("REPARATION"))==0){
				fieldData[i] = sData.GetAt(8);
			}else if(fieldName[i].Compare(_T("LOAN_STOP_DATE"))==0){
				if(!sData.GetAt(9).IsEmpty()) fieldData[i].Format(_T("to_date('%s','YYYY/MM/DD')"), sData.GetAt(9));
			}else if(fieldName[i].Compare(_T("WORK_START_TIME"))==0){
				fieldData[i].Format(_T("to_date('%s','YYYY/MM/DD:HH24-MI-SS')"), m_sTime);
			}else if(fieldName[i].Compare(_T("WORKER"))==0){
				fieldData[i] = GetUserID();
			}else if(fieldName[i].Compare(_T("SEQ_NO"))==0){
				fieldData[i] = m_sSEQ_NO;
			}else if(fieldName[i].Compare(_T("PROC_DATE"))==0){
				if(sData.GetAt(0).Compare(_T("0"))==0 || sData.GetAt(0).Compare(_T("2"))==0){
					fieldData[i].Format(_T("to_date('%s','YYYY/MM/DD:HH24-MI-SS')"), sData.GetAt(3));
				}else if(sData.GetAt(0).Compare(_T("1"))==0 || sData.GetAt(0).Compare(_T("3"))==0){
					fieldData[i].Format(_T("to_date('%s','YYYY/MM/DD:HH24-MI-SS')"), sData.GetAt(6));
				}
			}

			pDM->AddDBFieldData( fieldName[ i ], fieldType[ i ], fieldData[ i ] );
		}
		pDM->AddDBFieldData( _T("MANAGE_CODE"), _T("STRING"), _T("UDF_MANAGE_CODE") );

		pDM->MakeInsertFrame(_T("CO_OFFLINE_LOAN_TBL"));
		ids = pDM->SendFrame();
		if(ids){
			nFailCnt++;
		}else{
			nSuccessCnt++;
		}
		pDM->EndFrame();		
	}	

	cFile.Close();
	
	CString sWhere;
	sWhere.Format(_T(" to_char(WORK_START_TIME , 'YYYY/MM/DD:HH24-MI-SS')='%s' and SEQ_NO = %s "),
		m_sTime, m_sSEQ_NO);
	ids = ShowMainGrid(sWhere);
	if(ids){
		
	}

	if(nSuccessCnt != nLineCnt){
		sWhere.Format(_T("파일에 있는 %d건중 %d건만 불러오기에 성공하였습니다.\n실패한 %d건은 이미 시도했거나 수행된 작업입니다.\n지난 작업내역을 보려면 '지난작업 불러오기'를 눌러주십시요."),
			nLineCnt, nSuccessCnt, nFailCnt);
		AfxMessageBox(sWhere);
	}

	return 0;
EFS_END
	return -1;
}

INT CBO_LOC_3180::MakeArray(CString sLine, CString sDel, CStringArray &saArray)
{
EFS_BEGIN
	INT nIndex;
	INT nDelLength = sDel.GetLength();
	BOOL bStop = FALSE;

	while(TRUE)
	{
		nIndex = sLine.Find(sDel);
		if( nIndex < 0 )
		{
			bStop = TRUE;

			if( !sLine.IsEmpty() )
			{
				saArray.Add(sLine);
			}
		}

		if( bStop ) break;

		saArray.Add(sLine.Mid(0,nIndex));

		sLine = sLine.Mid(nIndex+nDelLength);
	}

	return 0;
EFS_END
	return -1;
}

VOID CBO_LOC_3180::OnbSort() 
{
EFS_BEGIN
	CESL_DataMgr *pDM = FindDM( _T("DM_BO_LOC_3180") );
	if( pDM == NULL ){
		AfxMessageBox(_T("ERROR FindDM"));
		return ;
	}

	if(pDM->GetRowCount() < 1){
		return;
	}

	pDM->SORT(_T("PROC_DATE"));

	CESL_ControlMgr *pCM = FindCM( _T("CM_BO_LOC_3180") );
	if( pCM == NULL ) {
		AfxMessageBox(_T("ERROR FindCM"));
		return ;
	}
	
	pCM->AllControlDisplay();

	return;
EFS_END
	return ;
}

VOID CBO_LOC_3180::OnbSave() 
{
	EFS_BEGIN

		INT i=0, ids=0;

		CESL_ControlMgr *pCM = FindCM(_T("CM_BO_LOC_3180"));
		if(pCM == NULL){
			AfxMessageBox(_T("ERROR CBO_LOC_3180::OnbSave() := FindCM(CM_BO_LOC_3180)"));
			return;
		}

		CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl(_T("MainGrid")));
		if(pGrid == NULL){
			AfxMessageBox(_T("ERROR CBO_LOC_3180::OnbSave() := FindControl(MainGrid)"));
			return;
		}

		CESL_DataMgr *pDM = FindDM(_T("DM_BO_LOC_3180"));
		if(pDM == NULL){
			AfxMessageBox(_T("ERROR CBO_LOC_3180::OnbSave() := FindDM(DM_BO_LOC_3180)"));
			return;
		}

		pGrid->SelectMakeList();
		INT nSelectCount = pGrid->SelectGetCount();

		INT nGridRowCount = pDM->GetRowCount();

		if(nSelectCount < 1 || nGridRowCount < 1){
			AfxMessageBox(_T("선정된 자료가 없습니다!"));
			return;
		}

		if(nSelectCount != nGridRowCount){
			if(MessageBox( _T("모든자료를 선정하지 않으셨습니다.\n계속하시겠습니까?") , _T("") , MB_YESNO|MB_ICONQUESTION ) == IDNO ){
				return;
			}
		}
		
		CProgressCtrl *pProgressCtrl;
		pProgressCtrl = (CProgressCtrl*)GetDlgItem(IDC_prgSTATE);
		if(pProgressCtrl != NULL){
			pProgressCtrl->SetRange(0,nSelectCount);
		}
		pProgressCtrl->SetPos(i);

		pGrid->SelectGetHeadPosition();
		INT nTmpIndex;
		INT nSuccessCnt=0;
		INT nFailCnt=0;
		INT nRetryCnt=0;
		CString sTmp;
		CString sWorkingStatus;
		CString sTmpUserNo;
		for( i = 0 ; i < nSelectCount ; i++ )
		{
			pProgressCtrl->SetPos(i);

			nTmpIndex = pGrid->SelectGetIdx();
			m_nCurrentIndex = nTmpIndex;
			pDM->GetCellData(_T("WORKTYPE"), nTmpIndex, m_sTYPE);		
			
			m_sShareLoanRecKey.Empty();
			m_sShareUserRecKey.Empty();
			
			m_sDelayMoney.Empty();
			
			m_sFailCode.Empty();
			m_sFailDesc.Empty();
		
			m_bIsReservedBookByMe = FALSE;
			
			m_sReservedLoanKey.Empty();

			if(m_sTYPE.Compare(_T("대출"))==0){
				m_nLoanType = 4;
			}else if(m_sTYPE.Compare(_T("반납"))==0){
				m_nReturnType = 4;
			}else if(m_sTYPE.Compare(_T("무인대출"))==0){
				m_nLoanType = 5;
			}else if(m_sTYPE.Compare(_T("무인반납"))==0){
				m_nReturnType = 5;
			}
			pDM->GetCellData(_T("REG_NO"), nTmpIndex, m_sREG_NO);
			pDM->GetCellData(_T("USER_NO"), nTmpIndex, m_sUSER_NO);
			
			pDM->GetCellData(_T("SUCCESS_YN"), m_nCurrentIndex, sTmp);
			if(sTmp.Compare(_T("Y"))==0 ){
				nRetryCnt++;
				pGrid->SelectGetNext();
				continue;
			}

			if(
				m_sUSER_NO.Compare(_T("NULL"))==0 
				|| m_sUSER_NO.IsEmpty() 
				|| m_sTYPE.Compare(_T("반납"))==0 
				|| m_sTYPE.Compare(_T("무인반납"))==0
				){
				sTmp.Format(_T("select trim(U.user_no) from co_loan_user_tbl U, LS_WORK_T01 L where L.user_key = U.rec_key and L.status in ('0','2') and L.reg_no = '%s'"), m_sREG_NO);
				pDM->GetOneValueQuery(sTmp, sTmpUserNo);
				if(sTmpUserNo.IsEmpty()){
					nFailCnt++;
					
					UpdateResult(pDM, _T("N"), _T("10"), _T("존재하지 않는 대출자번호"), pDM->GetCellData(_T("REC_KEY"), m_nCurrentIndex));				
					sTmp = _T("대출자번호[") + m_sUSER_NO + _T("]가 존재하지 않습니다.\n계속 진행하시겠습니까?");
					if(MessageBox(sTmp, _T("대출자번호 확인"), MB_YESNO|MB_ICONQUESTION ) == IDNO ){
						break;
					}else{
						pGrid->SelectGetNext();
						continue;
					}
				}else{
					m_sUSER_NO = sTmpUserNo;
					pDM->GetCellData(_T("REC_KEY"), m_nCurrentIndex, sTmp);
					pDM->SetCellData(_T("USER_NO"), m_sUSER_NO, m_nCurrentIndex);
					pGrid->SetTextMatrix(m_nCurrentIndex+1, 3, m_sUSER_NO);
					UpdateOneColumn(pDM, _T("CO_OFFLINE_LOAN_TBL"), _T("USER_NO"), _T("STRING"), sTmp, m_sUSER_NO);
				}
			}

			
			ids = SaveUserinfoDM();
			if(ids){
				nFailCnt++;
				
				UpdateResult(pDM, _T("N"), _T("10"), _T("존재하지 않는 대출자번호"), pDM->GetCellData(_T("REC_KEY"), m_nCurrentIndex));				
				sTmp = _T("대출자번호[") + m_sUSER_NO + _T("]가 존재하지 않습니다.\n계속 진행하시겠습니까?");
				if(MessageBox(sTmp, _T("대출자번호 확인"), MB_YESNO|MB_ICONQUESTION ) == IDNO ){
					break;
				}else{
					pGrid->SelectGetNext();
					continue;
				}
			}
			
			ids = SaveBookinfoDM();
			if(ids){
				nFailCnt++;
				
				UpdateResult(pDM, _T("N"), _T("20"), _T("존재하지 않는 책정보"), pDM->GetCellData(_T("REC_KEY"), m_nCurrentIndex));				
				sTmp = _T("등록번호[") + m_sREG_NO + _T("]가 존재하지 않습니다.\n계속 진행하시겠습니까?");
				if(MessageBox(sTmp, _T("등록번호 확인"), MB_YESNO|MB_ICONQUESTION ) == IDNO ){
					break;
				}else{
					pGrid->SelectGetNext();
					continue;
				}
			}
			
			ids = CheckDuplicate();
			if(ids){
				
				nFailCnt++;
				UpdateResult(pDM, _T("N"), _T("30"), _T("이미처리된 대출/반납 정보"), pDM->GetCellData(_T("REC_KEY"), m_nCurrentIndex));				
				sTmp = _T("등록번호[") + m_sREG_NO + _T("]는 이미 처리된 상태입니다.\n계속 진행하시겠습니까?");
				if(MessageBox(sTmp, _T("중복확인"), MB_YESNO|MB_ICONQUESTION ) == IDNO ){
					break;
				}else{
					pGrid->SelectGetNext();
					continue;
				}
			}

			if(m_sTYPE.Compare(_T("대출"))==0 || m_sTYPE.Compare(_T("무인대출"))==0){
				
				ids = this->GetDataMgrData(_T("DM_EXP_LOAN_BOOK_INFO"),_T("상태"),sWorkingStatus, 0);
				if(ids){
					nFailCnt++;
					UpdateResult(pDM, _T("N"), _T("30"), _T("대출처리시 오류"), pDM->GetCellData(_T("REC_KEY"), m_nCurrentIndex));
					sTmp = _T("등록번호[") + m_sREG_NO + _T("] 처리중 오류가 발생하였습니다.\n계속 진행하시겠습니까?");
					if(MessageBox(sTmp, _T("확인"), MB_YESNO|MB_ICONQUESTION ) == IDNO ){
						break;
					}else{
						pGrid->SelectGetNext();
						continue;
					}
				}
				ids = 0;
				if(m_nLoanBookType==1){
					if(sWorkingStatus.Compare(_T("BOL112N"))!=0){
						ids = 1;
					}
				}else if(m_nLoanBookType==2){
					if(sWorkingStatus.Compare(_T("SEL112N"))!=0 && sWorkingStatus.Compare(_T("SEL212N"))!=0){
						ids = 2;
					}
				}else if(m_nLoanBookType==3){
					if(sWorkingStatus.Compare(_T("BOL112N"))!=0){
						ids = 3;
					}
				}
				if(ids){
					
					nFailCnt++;
					UpdateResult(pDM, _T("N"), _T("60"), _T("배가상태가 아님"), pDM->GetCellData(_T("REC_KEY"), m_nCurrentIndex));				
					sTmp = _T("등록번호[") + m_sREG_NO + _T("]는 대출이 불가능한 상태입니다.\n계속 진행하시겠습니까?");
					if(MessageBox(sTmp, _T("확인"), MB_YESNO|MB_ICONQUESTION ) == IDNO ){
						break;
					}else{
						pGrid->SelectGetNext();
						continue;
					}
				}
				
				ids = LoanProc();
				if(ids){
					nFailCnt++;
					UpdateResult(pDM, _T("N"), _T("30"), _T("대출처리시 오류"), pDM->GetCellData(_T("REC_KEY"), m_nCurrentIndex));
					sTmp = _T("등록번호[") + m_sREG_NO + _T("] 처리중 오류가 발생하였습니다.\n계속 진행하시겠습니까?");
					if(MessageBox(sTmp, _T("확인"), MB_YESNO|MB_ICONQUESTION ) == IDNO ){
						break;
					}else{
						pGrid->SelectGetNext();
						continue;
					}
				}else{
					nSuccessCnt++;
					UpdateResult(pDM, _T("Y"), m_sFailCode, m_sFailDesc, pDM->GetCellData(_T("REC_KEY"), m_nCurrentIndex));
				}
			}else if(m_sTYPE.Compare(_T("반납"))==0 || m_sTYPE.Compare(_T("무인반납"))==0){
				
				ids = ReturnCheckBook();
				if(ids<0){
					nFailCnt++;
					UpdateResult(pDM, _T("N"), _T("40"), _T("반납처리시 오류"), pDM->GetCellData(_T("REC_KEY"), m_nCurrentIndex));
					sTmp = _T("등록번호[") + m_sREG_NO + _T("] 처리중 오류가 발생하였습니다.\n계속 진행하시겠습니까?");
					if(MessageBox(sTmp, _T("확인"), MB_YESNO|MB_ICONQUESTION ) == IDNO ){
						break;
					}else{
						pGrid->SelectGetNext();
						continue;
					}
				}
				if(ids>0){
					
					nFailCnt++;
					UpdateResult(pDM, _T("N"), _T("70"), _T("대출상태가 아님"), pDM->GetCellData(_T("REC_KEY"), m_nCurrentIndex));				
					sTmp = _T("등록번호[") + m_sREG_NO + _T("]는 반납이 불가능한 상태입니다.\n계속 진행하시겠습니까?");
					if(MessageBox(sTmp, _T("확인"), MB_YESNO|MB_ICONQUESTION ) == IDNO ){
						break;
					}else{
						pGrid->SelectGetNext();
						continue;
					}
				}				
				ids = ReturnProc();
				if(ids){
					nFailCnt++;
					UpdateResult(pDM, _T("N"), _T("40"), _T("반납처리시 오류"), pDM->GetCellData(_T("REC_KEY"), m_nCurrentIndex));
					sTmp = _T("등록번호[") + m_sREG_NO + _T("] 처리중 오류가 발생하였습니다.\n계속 진행하시겠습니까?");
					if(MessageBox(sTmp, _T("확인"), MB_YESNO|MB_ICONQUESTION ) == IDNO ){
						break;
					}else{
						pGrid->SelectGetNext();
						continue;
					}
				}else{
					nSuccessCnt++;
					UpdateResult(pDM, _T("Y"), m_sFailCode, m_sFailDesc, pDM->GetCellData(_T("REC_KEY"), m_nCurrentIndex));

					
					ids = RecordReserveExpireDate();
					if(ids){
					}
				}
			}

			pGrid->SelectGetNext();
		}
		pProgressCtrl->SetPos(i);
		
		InitExpDM();
		
		CString sWhere;
		sWhere.Format(_T(" to_char(WORK_START_TIME , 'YYYY/MM/DD:HH24-MI-SS')='%s' and SEQ_NO = %s "),
		m_sTime, m_sSEQ_NO);
		ShowMainGrid(sWhere);

		sTmp.Empty();
		sTmp.Format(_T("%d건 중 %d건을 처리하였습니다!\n[성공:%d건]\n[실패:%d건]\n[재시도:%d건]"), nSelectCount, nSuccessCnt, nSuccessCnt, nFailCnt, nRetryCnt);
		AfxMessageBox(sTmp);

		return;
	EFS_END
		return;
}

VOID CBO_LOC_3180::OnOK() 
{
	CDialog::OnOK();
}

VOID CBO_LOC_3180::OnbModify() 
{
	EFS_BEGIN

	CESL_DataMgr *pDM = FindDM( _T("DM_BO_LOC_3180") );
	if( pDM == NULL ){
		AfxMessageBox(_T("ERROR FindDM"));
		return ;
	}

	if(pDM->GetRowCount() < 1){
		return;
	}

	CBO_LOC_3182 Dlg(this);

	Dlg.DoModal();

		return;
	EFS_END
		return;
}

INT CBO_LOC_3180::LoanProc()
{
	EFS_BEGIN
	INT ids=0;
	
	ids = LoanCheckBook();
	if(ids){		
		if(ids < 0){
			return -1;
		}else if(ids == 1020){
			m_sFailCode = _T("1020");
			m_sFailDesc = _T("배가상태가 아님");
		}else if(ids == 1030){
			m_sFailCode = _T("1030");
			m_sFailDesc = _T("부록자료 대출불가");
		}else if(ids == 1040){
			m_sFailCode = _T("1040");
			m_sFailDesc = _T("별치코드 대출불가");
		}else if(ids == 1050){
			m_sFailCode = _T("1050");
			m_sFailDesc = _T("자료실 대출불가");
		}else if(ids == 1060){
			m_sFailCode = _T("1060");
			m_sFailDesc = _T("이용제한구분 대출불가");
		}else if(ids == 1070){
			m_sFailCode = _T("1070");
			m_sFailDesc = _T("등록구분 대출불가");
		}else if(ids == 1080){
			m_sFailCode = _T("1080");
			m_sFailDesc = _T("이용대상구분 대출불가");
		}else if(ids == 1090){
			m_sFailCode = _T("1090");
			m_sFailDesc = _T("다른사람이 예약한 자료");
		}else{
			return -3;
		}
	}

	ids = LoanCheckUser();
	if(ids){
		if(ids < 0){
			return -5;
		}else if(ids == 2010){
			m_sFailCode = _T("2010");
			m_sFailDesc = _T("정상회원이 아님");
		}else if(ids == 2020){
			m_sFailCode = _T("2020");
			m_sFailDesc = _T("대출정지일이 지나지 않음");
		}else if(ids == 2030){
			m_sFailCode = _T("2030");
			m_sFailDesc = _T("개인재대출");
		}else if(ids == 2040){
			m_sFailCode = _T("2040");
			m_sFailDesc = _T("가족재대출");
		}else if(ids == 2050){
			m_sFailCode = _T("2050");
			m_sFailDesc = _T("대출책수 초과");
		}else if(ids == 2060){
			m_sFailCode = _T("2060");
			m_sFailDesc = _T("관리구분 대출불가");
		}else{
			return -7;
		}
	}

	if(m_bIsReservedBookByMe==TRUE){
		ids = LoanDBProc_UpdateLoan();
		if(ids){
			return -9;
		}
	}else{
		ids = LoanDBProc_InsertLoan();
		if(ids){
			return -11;
		}
	}
	
	ids = LoanDBProc_UpdateBook();
	if(ids){
		return -13;
	}
	
	ids = LoanDBProc_UpdateUser();
	if(ids){
		return -15;
	}
	
	CESL_DataMgr *pDM = FindDM( _T("DM_BO_LOC_3180") );
	if(pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE2( -17 , _T("CBO_LOC_3180::LoanProc() := FindDM()"));

	ids = pDM->SendFrame();
	if(ids) ERROR_MSG_RETURN_ERROR_CODE2( -19 , _T("CBO_LOC_3180::LoanProc() := SendFrame()"));

	ids = pDM->EndFrame();
	if(ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -21 , _T("CBO_LOC_3180::LoanProc() := EndFrame()"));

	if(m_bIsReservedBookByMe==TRUE){
		m_sShareLoanRecKey = m_sReservedLoanKey;
		m_pSpfWork->SetTempData( SHARE_CO_LOAN_TBL_UPDATE_REC_KEY , CComVariant(m_sShareLoanRecKey) );
		m_pSpfWork->SetTempData( SHARE_CO_LOAN_TBL_UPDATE_FIELDS , CComVariant(_T("LOAN_TYPE_CODE,RETURN_PLAN_DATE,STATUS,LOAN_DATE")) );

		ids = this->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("대출자KEY"), m_sShareUserRecKey, 0);
		m_pSpfWork->SetTempData( SHARE_CO_LOAN_USER_TBL_UPDATE_REC_KEY , CComVariant(m_sShareUserRecKey) );
		m_pSpfWork->SetTempData( SHARE_CO_LOAN_USER_TBL_UPDATE_FIELDS , CComVariant(_T("LOAN_COUNT,RESERVATION_COUNT")) );
		ShareInfoCenterUpload( _T("U") , _T("U") );
	}else{
		m_pSpfWork->SetTempData( SHARE_CO_LOAN_TBL_INSERT_REC_KEY , CComVariant(m_sShareLoanRecKey) );

		ids = this->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("대출자KEY"), m_sShareUserRecKey, 0);
		m_pSpfWork->SetTempData( SHARE_CO_LOAN_USER_TBL_UPDATE_REC_KEY , CComVariant(m_sShareUserRecKey) );
		m_pSpfWork->SetTempData( SHARE_CO_LOAN_USER_TBL_UPDATE_FIELDS , CComVariant(_T("LOAN_COUNT,LAST_LOAN_DATE")) );
		ShareInfoCenterUpload( _T("I") , _T("U") );
	}

	return 0;
EFS_END
	return -1;
}

INT CBO_LOC_3180::ReturnProc()
{
	EFS_BEGIN

	INT ids = 0 , i = 0;

	ids = ReturnDBProc_UpdateLoan();
	if(ids){
		return -7;
	}
	
	ids = ReturnDBProc_UpdateBook();
	if(ids){
		return -9;
	}
	
	ids = ReturnDBProc_UpdateUser();
	if(ids){
		return -11;
	}
	
	CESL_DataMgr *pDM = FindDM( _T("DM_BO_LOC_3180") );
	if(pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE2( -13 , _T("CBO_LOC_3180::ReturnProc() := FindDM()"));

	ids = pDM->SendFrame();
	if(ids) ERROR_MSG_RETURN_ERROR_CODE2( -15 , _T("CBO_LOC_3180::ReturnProc() := SendFrame()"));

	ids = pDM->EndFrame();
	if(ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -17 , _T("CBO_LOC_3180::ReturnProc() := EndFrame()"));
	
	ids = this->GetDataMgrData(_T("DM_BO_LOC_3180_LOAN"), _T("KEY"), m_sShareLoanRecKey, 0);
	m_pSpfWork->SetTempData( SHARE_CO_LOAN_TBL_UPDATE_REC_KEY , CComVariant(m_sShareLoanRecKey) );
	m_pSpfWork->SetTempData( SHARE_CO_LOAN_TBL_UPDATE_FIELDS , CComVariant(_T("STATUS,RETURN_TYPE_CODE,REPARATION,ARREAR,RETURN_DATE")) );
	ids = this->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("대출자KEY"), m_sShareUserRecKey, 0);
	m_pSpfWork->SetTempData( SHARE_CO_LOAN_USER_TBL_UPDATE_REC_KEY , CComVariant(m_sShareUserRecKey) );
	m_pSpfWork->SetTempData( SHARE_CO_LOAN_USER_TBL_UPDATE_FIELDS , CComVariant(_T("LOAN_COUNT,LOAN_STOP_DATE,DELAY_COUNT")) );
	ShareInfoCenterUpload( _T("U") , _T("U") );

	return 0;
	EFS_END
	return -1;
}

INT CBO_LOC_3180::LoanCheckBook()
{
	EFS_BEGIN

	INT ids=0;
	
	CESL_DataMgr* pDMmember = FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	if(pDMmember == NULL) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_3180::LoanCheckBook()") );

	CESL_DataMgr* pDMbook = FindDM(_T("DM_EXP_LOAN_BOOK_INFO"));
	if(pDMbook == NULL) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CBO_LOC_3180::LoanCheckBook()") );

	CString sTempData;
	
	ids = pDMbook->GetCellData(_T("부록여부"), 0, sTempData);
	if(ids) ERROR_MSG_RETURN_ERROR_CODE2( -9 , _T("CBO_LOC_3180::LoanCheckBook()") );
	if(sTempData.Compare(_T("A"))==0){
		
		if(m_pCheckLoan->IsAppendixLoan()){
			return 1030;
		}
	}
	
	ids = pDMbook->GetCellData(_T("별치기호"), 0, sTempData);
	if(ids) ERROR_MSG_RETURN_ERROR_CODE2( -11 , _T("CBO_LOC_3180::LoanCheckBook()") );
	if(!sTempData.IsEmpty()){
		if(m_pCheckLoan->IsLoan(_T("별치기호"), sTempData)){
			return 1040;
		}		
	}
	
	ids = pDMbook->GetCellData(_T("자료실구분"), 0, sTempData);
	if(ids) ERROR_MSG_RETURN_ERROR_CODE2( -13 , _T("CBO_LOC_3180::LoanCheckBook()") );
	if(!sTempData.IsEmpty()){
		if(m_pCheckLoan->IsLoan(_T("자료실구분"), sTempData)){
			return 1050;
		}		
	}
	
	ids = pDMbook->GetCellData(_T("이용제한구분"), 0, sTempData);
	if(ids) ERROR_MSG_RETURN_ERROR_CODE2( -15 , _T("CBO_LOC_3180::LoanCheckBook()") );
	if(!sTempData.IsEmpty()){
		if(m_pCheckLoan->IsLoan(_T("이용제한구분"), sTempData)){
			return 1060;
		}		
	}
	
	ids = pDMbook->GetCellData(_T("등록구분"), 0, sTempData);
	if(ids) ERROR_MSG_RETURN_ERROR_CODE2( -17 , _T("CBO_LOC_3180::LoanCheckBook()") );
	if(!sTempData.IsEmpty()){
		if(m_pCheckLoan->IsLoan(_T("등록구분"), sTempData)){
			return 1070;
		}		
	}
	
	ids = m_pCheckLoan->IsUseObject( sTempData );
	if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2( -19 , _T("CBO_LOC_3180::LoanCheckBook()") );
	if(ids>0){
		return 1080;
	}
	
	CString query, user_key, rec_key;
	ids = pDMbook->GetCellData(_T("책KEY"), 0, sTempData);
	if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2( -21 , _T("CBO_LOC_3180::LoanCheckBook()") );

	ids = pDMmember->GetCellData(_T("대출자KEY"), 0, user_key);
	if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2( -23 , _T("CBO_LOC_3180::LoanCheckBook()") );
	
	query.Format( _T("SELECT rec_key FROM LS_WORK_T01 WHERE USER_KEY = %s and BOOK_KEY = %s and status = '3'"),user_key, sTempData );
	pDMmember->GetOneValueQuery( query, rec_key );

	if(!rec_key.IsEmpty()){
		m_sReservedLoanKey = rec_key;
		m_bIsReservedBookByMe = TRUE;		
		return 0;
	}else{
		m_bIsReservedBookByMe = FALSE;
	}
	
	query.Empty();
	rec_key.Empty();
	query.Format( _T("SELECT rec_key FROM LS_WORK_T01 WHERE BOOK_KEY = %s and status = '3'"), sTempData );
	pDMmember->GetOneValueQuery( query, rec_key );

	if(!rec_key.IsEmpty()){
		return 1090;
	}

	return 0;
	EFS_END
	return -1;
}

INT CBO_LOC_3180::LoanCheckUser()
{
EFS_BEGIN

	INT ids=0;
	
	CESL_DataMgr* pDMmember = FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	if(pDMmember == NULL) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_3180::LoanCheckUser() := FindDM()") );

	CESL_DataMgr* pDMbook = FindDM(_T("DM_EXP_LOAN_BOOK_INFO"));
	if(pDMbook == NULL) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CBO_LOC_3180::LoanCheckBook()") );

	CString sTempData;
	
	ids = pDMmember->GetCellData( _T("회원상태"), 0 , sTempData);
	if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CBO_LOC_3180::LoanCheckUser() := GetCellData()") );
	if(sTempData.Compare(_T("0")) != 0){
		return 2010;
	}
	
	ids = pDMmember->GetCellData( _T("대출정지일"), 0 , sTempData);
	if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("CBO_LOC_3180::LoanCheckUser() := GetCellData()") );
	if(!sTempData.IsEmpty()){
		COleDateTime tLoanStopDate( _ttoi(sTempData.Mid(0,4)) , _ttoi(sTempData.Mid(5,2)) , _ttoi(sTempData.Mid(8,2)) , 0 , 0 , 0 );
		
		CString tmpdate;
		pDMmember->GetOneValueQuery(_T("select to_char(sysdate, 'YYYY/MM/DD') from dual"), tmpdate);
		COleDateTime tCurrent( _ttoi(tmpdate.Mid(0,4)) , _ttoi(tmpdate.Mid(5,2)) , _ttoi(tmpdate.Mid(8,2)) , 0 , 0 , 0 );
		
		if( tCurrent <= tLoanStopDate ) return 2020;
	}
	
	ids = m_pCheckLoan->IsReLoanBook();
	if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2( -9 , _T("CBO_LOC_3180::LoanCheckUser() := IsReLoanBook()") );
	if(ids==11){
		return 2030;
	}else if(ids==22){
		return 2040;
	}
	
	ids = pDMmember->GetCellData( _T("대출가능책수"), 0 , sTempData);
	if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2( -11 , _T("CBO_LOC_3180::LoanCheckUser() := GetCellData()") );
	if( _ttoi( sTempData ) < 1 ){
		CString sIsAppendix;
		ids = pDMbook->GetCellData(_T("부록여부"), 0, sIsAppendix);
		if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2( -13 , _T("CBO_LOC_3180::LoanCheckUser() := GetCellData()") );
		if( m_pCheckLoan->IsAppendixIncludeLoanCnt() != 0 && sIsAppendix.Compare(_T("A")) == 0 ){
		}else{
			return 2050;
		}
	}
	
	ids = m_pCheckLoan->IsUseManageCode();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -15 , _T("CBO_LOC_3180::LoanCheckUser() := IsUseManageCode()") );
	
	if( ids == 1 ) 
	{
		ids = m_pCheckLoan->IsLoanManageCode();
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -17 , _T("CBO_LOC_3180::LoanCheckUser() := IsLoanManageCode()") );
		if( ids == 0 ) 
		{
			return 2060;
		}
	}

	return 0;
EFS_END
	return -1;
}

INT CBO_LOC_3180::MakeRegNoSpaceToZero(CString &sRegNo)
{
	EFS_BEGIN

	CString sRegCode;
	CString sRegNumber;
	CString sRegSpace;		
	
	sRegNo.TrimLeft();
	sRegNo.TrimRight();
	sRegNo.Replace( _T(" "), _T("") );

	CLocCommonAPI::MakeRegNoSpaceToZero(this, sRegNo, NULL);

	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_3180::GetBookType()
{
	EFS_BEGIN

	INT nRowCnt = 0, ids=0;
	
	CESL_DataMgr* pDM_BO = FindDM(_T("DM_EXP_LOAN_BO_BOOK_INFO"));
	if(pDM_BO == NULL) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_3180::GetBookType()") );

	CString sWhere;
	sWhere.Format(_T(" BB.REG_NO = '%s' AND BB.MANAGE_CODE = '%s'"), m_sREG_NO, m_pInfo->MANAGE_CODE);

	pDM_BO->FreeData();
	ids = pDM_BO->RefreshDataManager(sWhere);
	if(ids) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CBO_LOC_3180::GetBookType()") );

	nRowCnt = pDM_BO->GetRowCount();
	if(nRowCnt > 0){
		m_nLoanBookType = 1;
		m_nReturnBookType = 1;
		return nRowCnt;
	}
	
	CESL_DataMgr* pDM_SE = FindDM(_T("DM_EXP_LOAN_SE_BOOK_INFO"));
	if(pDM_SE == NULL) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("CBO_LOC_3180::GetBookType()") );

	sWhere.Format(_T(" BB.REG_NO = '%s' AND BB.MANAGE_CODE = '%s'"), m_sREG_NO, m_pInfo->MANAGE_CODE);
	pDM_SE->FreeData();
	ids = pDM_SE->RefreshDataManager(sWhere);
	if(ids) ERROR_MSG_RETURN_ERROR_CODE2( -11 , _T("CBO_LOC_3180::GetBookType()") );

	nRowCnt = pDM_SE->GetRowCount();
	if(nRowCnt > 0){
		m_nLoanBookType = 2;
		m_nReturnBookType = 2;
		return nRowCnt;
	}
	
	CESL_DataMgr* pDM_NON = FindDM(_T("DM_EXP_LOAN_NON_DB_BOOK_INFO"));
	if(pDM_NON == NULL)  ERROR_MSG_RETURN_ERROR_CODE2( -13 , _T("CBO_LOC_3180::GetBookType()") );
	
	sWhere.Format(_T(" NB.REG_NO = '%s' "), m_sREG_NO);
	sWhere.Format(_T(" NB.REG_NO = '%s' AND NB.MANAGE_CODE = '%s'"), m_sREG_NO, m_pInfo->MANAGE_CODE);

	pDM_NON->FreeData();
	ids = pDM_NON->RefreshDataManager(sWhere);
	if(ids) ERROR_MSG_RETURN_ERROR_CODE2( -15 , _T("CBO_LOC_3180::GetBookType()") );

	nRowCnt = pDM_NON->GetRowCount();
	if(nRowCnt > 0){
		m_nLoanBookType = 3;
		m_nReturnBookType = 3;
		return nRowCnt;
	}

	return nRowCnt;

	EFS_END
	return -1;
}

INT CBO_LOC_3180::LoanDBProc_InsertLoan()
{
	EFS_BEGIN
	
	INT i=0, ids=0;

	CESL_DataMgr *pDM = FindDM( _T("DM_BO_LOC_3180") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_3180::LoanDBProc_InsertLoan() := FindDM()") );

	pDM->StartFrame();
	pDM->InitDBFieldData();
	
	CString sSrcDMAlias = _T("DM_EXP_LOAN_BOOK_INFO");
	const INT nSrcFieldCnt = 13;
	TCHAR *sSrcFieldAlias[nSrcFieldCnt] = 
	{
		_T("책KEY") , _T("자료실구분") , _T("단행/연속구분") , _T("분류기호구분") , _T("분류기호") , 
		_T("등록구분") , _T("종정보키") , _T("등록번호") , _T("별치기호") , 
		_T("본표제") , _T("저작자") , _T("발행자") , _T("청구기호")
	};
	TCHAR *sDstFieldAlias[nSrcFieldCnt] = 
	{
		_T("BOOK_KEY") , _T("SHELF_LOC_CODE") , _T("PUBLISH_FORM_CODE") , _T("CLASS_NO_TYPE") , _T("CLASS_NO") , 
		_T("REG_CODE") , _T("SPECIES_KEY")  , _T("REG_NO") , _T("SEPARATE_SHELF_CODE") , 
		_T("TITLE") , _T("AUTHOR") , _T("PUBLISHER") , _T("CALL_NO")
	};
	TCHAR *sDstFieldTypeAlias[nSrcFieldCnt] = 
	{
		_T("NUMERIC") , _T("STRING") , _T("STRING") , _T("STRING") , _T("STRING") , 
		_T("STRING") , _T("NUMERIC") , _T("STRING") , _T("STRING") ,
		_T("STRING") , _T("STRING")  , _T("STRING") , _T("STRING")
	};

	CString sGetData;
	for( i= 0 ; i < nSrcFieldCnt ;i++ )
	{
		ids = this->GetDataMgrData( sSrcDMAlias , sSrcFieldAlias[i] , sGetData , 0 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CBO_LOC_3180::LoanDBProc_InsertLoan() := GetDataMgrData()") );

		if( CString(_T("NUMERIC")).Compare(sDstFieldTypeAlias[i])==0 )
			if( sGetData.IsEmpty() ) sGetData = _T("NULL");

		ids = pDM->AddDBFieldData( sDstFieldAlias[i] , sDstFieldTypeAlias[i] , sGetData , TRUE );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("CBO_LOC_3180::LoanDBProc_InsertLoan() := AddDBFieldData()") );

	}

	CString sSrcUserDMAlias = _T("DM_BO_LOC_3100_MEMBER_INFO");
	const INT nSrcUserFieldCnt = 4;
	TCHAR *sSrcUserFieldAlias[nSrcUserFieldCnt] = 
	{
		_T("대출자KEY") , _T("대출자소속") , _T("대출자직급") , _T("회원구분") 
	};
	TCHAR *sDstUserFieldAlias[nSrcUserFieldCnt] = 
	{
		_T("USER_KEY") , _T("USER_POSITION_CODE") , _T("USER_CLASS_CODE") , _T("MEMBER_CLASS") 
	};
	TCHAR *sDstUserFieldTypeAlias[nSrcUserFieldCnt] = 
	{
		_T("NUMERIC") , _T("STRING") , _T("STRING") , _T("STRING") 
	};

	for( i= 0 ; i < nSrcUserFieldCnt ;i++ )
	{
		ids = this->GetDataMgrData( sSrcUserDMAlias , sSrcUserFieldAlias[i] , sGetData , 0 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -9 , _T("CBO_LOC_3180::LoanDBProc_InsertLoan() := GetDataMgrData()") );

		ids = pDM->AddDBFieldData( sDstUserFieldAlias[i] , sDstUserFieldTypeAlias[i] , sGetData , TRUE );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -11 , _T("CBO_LOC_3180::LoanDBProc_InsertLoan() := AddDBFieldData()") );

	}	
	
	if(m_nLoanType==4){
		ids = pDM->AddDBFieldData( _T("LOAN_TYPE_CODE") , _T("STRING") , _T("4") , TRUE );
	}else if(m_nLoanType==5){
		ids = pDM->AddDBFieldData( _T("LOAN_TYPE_CODE") , _T("STRING") , _T("5") , TRUE );
	}else{
		ids = pDM->AddDBFieldData( _T("LOAN_TYPE_CODE") , _T("STRING") , _T("0") , TRUE );
	}
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -13 , _T("CBO_LOC_3180::LoanDBProc_InsertLoan() := AddDBFieldData()") );
	
	CString sTmpDate;
	sTmpDate += _T("to_date('");
	sTmpDate += pDM->GetCellData(_T("RETURN_PLAN_DATE"), m_nCurrentIndex);
	sTmpDate += _T("', 'YYYY/MM/DD')");
	ids = pDM->AddDBFieldData( _T("RETURN_PLAN_DATE") , _T("NUMERIC") , sTmpDate , TRUE);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -17 , _T("CBO_LOC_3180::LoanDBProc_InsertLoan() := AddDBFieldData()") );
	
	ids = pDM->AddDBFieldData( _T("STATUS") , _T("STRING") , _T("0") , TRUE );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -19 , _T("CBO_LOC_3180::LoanDBProc_InsertLoan() := AddDBFieldData()") );
	
	sTmpDate.Empty();
	sTmpDate += _T("to_date('");
	sTmpDate += pDM->GetCellData(_T("LOAN_DATE"), m_nCurrentIndex);
	sTmpDate += _T("', 'YYYY/MM/DD:HH24-MI-SS')");
	ids = pDM->AddDBFieldData( _T("LOAN_DATE") , _T("NUMERIC") , sTmpDate , TRUE);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -21 , _T("CBO_LOC_3180::LoanDBProc_InsertLoan() := AddDBFieldData()") );
	
	CString sIsAppendix;
	ids = this->GetDataMgrData( _T("DM_EXP_LOAN_BOOK_INFO") , _T("부록여부") , sIsAppendix , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -21 , _T("CBO_LOC_3180::LoanDBProc_InsertLoan() := GetDataMgrData()") );

	if( sIsAppendix.Compare( _T("A") ) == 0 )
	{
		if( m_pCheckLoan->IsAppendixIncludeLoanCnt() != 0 )
		{
			pDM->AddDBFieldData( _T("LOAN_KEY") , _T("NUMERIC") , _T("1") );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -23 , _T("CBO_LOC_3180::LoanDBProc_InsertLoan() := AddDBFieldData()") );
		}
	}

	CString sLog;
	sLog.Format( _T("%s") , this->GetWorkLogMsg( _T("오프라인대출"), __WFILE__, __LINE__ ) );
	ids = pDM->AddDBFieldData( _T("FIRST_WORK") , _T("STRING") , sLog , TRUE );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -25 , _T("CBO_LOC_3180::LoanDBProc_InsertLoan() := AddDBFieldData()") );
	
	CString sRecKey;
	ids = pDM->MakeRecKey(sRecKey);
	ids = pDM->AddDBFieldData( _T("REC_KEY") , _T("NUMERIC") , sRecKey , TRUE );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -29 , _T("CBO_LOC_3180::LoanDBProc_InsertLoan() := AddDBFieldData()") );
	
	ids = pDM->AddDBFieldData( _T("L_DEVICE"), _T("STRING"), _T("KOLASIII"), TRUE );
	if(ids < 0) ERROR_MSG_RETURN_ERROR_CODE2( -13 , _T("CBO_LOC_3180::LoanDBProc_UpdateBook() := AddDBFieldData()") );	
	
	ids = pDM->AddDBFieldData( _T("MANAGE_CODE"), _T("STRING"), _T("UDF_MANAGE_CODE"), TRUE );
	if(ids < 0) ERROR_MSG_RETURN_ERROR_CODE2( -13 , _T("CBO_LOC_3180::LoanDBProc_UpdateBook() := AddDBFieldData()") );
		
	m_sShareLoanRecKey = sRecKey;
		
	ids = pDM->MakeInsertFrame(_T("LS_WORK_T01"));
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -31 , _T("CBO_LOC_3180::LoanDBProc_InsertLoan() := MakeInsertFrame()") );

	pDM->InitDBFieldData();

	return 0;
	EFS_END
	return -1;
}

INT CBO_LOC_3180::LoanDBProc_UpdateLoan()
{
	EFS_BEGIN

	INT i=0, ids=0;

	CESL_DataMgr *pDM = FindDM( _T("DM_BO_LOC_3180") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_3180::LoanDBProc_UpdateLoan() := FindDM()") );


	pDM->StartFrame();
	pDM->InitDBFieldData();
	
	if(m_nLoanType==4){
		ids = pDM->AddDBFieldData( _T("LOAN_TYPE_CODE") , _T("STRING") , _T("4") , TRUE );
	}else if(m_nLoanType==5){
		ids = pDM->AddDBFieldData( _T("LOAN_TYPE_CODE") , _T("STRING") , _T("5") , TRUE );
	}
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CBO_LOC_3180::LoanDBProc_UpdateLoan() := AddDBFieldData()") );
	
	CString sTmpDate;
	sTmpDate += _T("to_date('");
	sTmpDate += pDM->GetCellData(_T("RETURN_PLAN_DATE"), m_nCurrentIndex);
	sTmpDate += _T("', 'YYYY/MM/DD')");
	ids = pDM->AddDBFieldData( _T("RETURN_PLAN_DATE") , _T("NUMERIC") , sTmpDate , TRUE);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("CBO_LOC_3180::LoanDBProc_UpdateLoan() := AddDBFieldData()") );
	
	ids = pDM->AddDBFieldData( _T("STATUS") , _T("STRING") , _T("0") , TRUE );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -9 , _T("CBO_LOC_3180::LoanDBProc_UpdateLoan() := AddDBFieldData()") );
	
	sTmpDate.Empty();
	sTmpDate += _T("to_date('");
	sTmpDate += pDM->GetCellData(_T("LOAN_DATE"), m_nCurrentIndex);
	sTmpDate += _T("', 'YYYY/MM/DD:HH24-MI-SS')");
	ids = pDM->AddDBFieldData( _T("LOAN_DATE") , _T("NUMERIC") , sTmpDate , TRUE);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -11 , _T("CBO_LOC_3180::LoanDBProc_UpdateLoan() := AddDBFieldData()") );
	
	pDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), this->GetWorkLogMsg( _T("오프라인예약대출"), __WFILE__, __LINE__ ), TRUE );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -13 , _T("CBO_LOC_3180::LoanDBProc_UpdateLoan() := AddDBFieldData()") );
	
	ids = pDM->AddDBFieldData( _T("L_DEVICE"), _T("STRING"), _T("KOLASIII"), TRUE );
	if(ids < 0) ERROR_MSG_RETURN_ERROR_CODE2( -13 , _T("CBO_LOC_3180::LoanDBProc_UpdateBook() := AddDBFieldData()") );

	ids = pDM->AddDBFieldData( _T("MANAGE_CODE"), _T("STRING"), _T("UDF_MANAGE_CODE"), TRUE );
	if(ids < 0) ERROR_MSG_RETURN_ERROR_CODE2( -13 , _T("CBO_LOC_3180::LoanDBProc_UpdateBook() := AddDBFieldData()") );

	ids = pDM->MakeUpdateFrame( _T("LS_WORK_T01") , _T("REC_KEY") , _T("NUMERIC") , m_sReservedLoanKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -15 , _T("CBO_LOC_3180::LoanDBProc_UpdateLoan() := MakeUpdateFrame()") );

	pDM->InitDBFieldData();

	return 0;
	EFS_END
	return -1;
}

INT CBO_LOC_3180::LoanDBProc_UpdateBook()
{
	EFS_BEGIN

	INT i=0, ids=0;

	CESL_DataMgr *pDM = FindDM( _T("DM_BO_LOC_3180") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_3180::LoanDBProc_UpdateBook() := FindDM()") );

	pDM->InitDBFieldData();
	
	CString sRecKey;
	ids = this->GetDataMgrData(_T("DM_EXP_LOAN_BOOK_INFO"), _T("책KEY") , sRecKey , 0 );
	if(ids < 0) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CBO_LOC_3180::LoanDBProc_UpdateBook() := GetDataMgrData()") );

	CString sPreWorkingStatus;
	ids = this->GetDataMgrData(_T("DM_EXP_LOAN_BOOK_INFO"), _T("상태") , sPreWorkingStatus , 0 );
	if(ids < 0) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("CBO_LOC_3180::LoanDBProc_UpdateBook() := GetDataMgrData()") );
	ids = pDM->AddDBFieldData( _T("PREV_WORKING_STATUS") , _T("STRING") , sPreWorkingStatus , TRUE );
	if(ids < 0) ERROR_MSG_RETURN_ERROR_CODE2( -9 , _T("CBO_LOC_3180::LoanDBProc_UpdateBook() := AddDBFieldData()") );
	
	CString sWorkingStatus;
	CString sTable;
	if(m_nLoanBookType == 1){ 
		sWorkingStatus = _T("BOL211O");
		sTable = _T("BO_BOOK_TBL");
	}else if(m_nLoanBookType == 2){ 
		sWorkingStatus = _T("SEL311O");
		sTable = _T("SE_BOOK_TBL");
	}else if(m_nLoanBookType == 3){ 
		sWorkingStatus = _T("BOL214O");
		sTable = _T("CO_NON_DB_BOOK_TBL");
	}else{
		sWorkingStatus = _T("BOL211O");
		sTable = _T("BO_BOOK_TBL");
	}
	ids = pDM->AddDBFieldData( _T("WORKING_STATUS") , _T("STRING") , sWorkingStatus , TRUE );
	if(ids < 0) ERROR_MSG_RETURN_ERROR_CODE2( -11 , _T("CBO_LOC_3180::LoanDBProc_UpdateBook() := AddDBFieldData()") );
	
	ids = pDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), this->GetWorkLogMsg( _T("오프라인대출"), __WFILE__, __LINE__ ), TRUE );
	if(ids < 0) ERROR_MSG_RETURN_ERROR_CODE2( -13 , _T("CBO_LOC_3180::LoanDBProc_UpdateBook() := AddDBFieldData()") );
	
	AddIdxQueryFrame( pDM, m_nLoanBookType, sRecKey );
	
	ids = pDM->MakeUpdateFrame(sTable,  _T("REC_KEY") , _T("NUMERIC") , sRecKey );
	if(ids < 0) ERROR_MSG_RETURN_ERROR_CODE2( -15 , _T("CBO_LOC_3180::LoanDBProc_UpdateBook() := MakeUpdateFrame()") );

	pDM->InitDBFieldData();

	return 0;
	EFS_END
	return -1;
}

INT CBO_LOC_3180::LoanDBProc_UpdateUser()
{
	EFS_BEGIN

	INT i=0, ids=0;

	CESL_DataMgr *pDM = FindDM( _T("DM_BO_LOC_3180") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_3180::LoanDBProc_UpdateUser() := FindDM()") );

	pDM->InitDBFieldData();
	
	CString sRecKey;
	CString sLoanBookCnt;
	CString sReservationCnt;
	CString sDMUserInfoAlias = _T("DM_BO_LOC_3100_MEMBER_INFO");
	ids = this->GetDataMgrData( sDMUserInfoAlias , _T("대출자KEY") , sRecKey , 0 ); 
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CBO_LOC_3180::LoanDBProc_UpdateUser() := GetDataMgrData()") );
	
	ids = this->GetDataMgrData( sDMUserInfoAlias , _T("총대출책수") , sLoanBookCnt , 0 ); 
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("CBO_LOC_3180::LoanDBProc_UpdateUser() := GetDataMgrData()") );

	ids = this->GetDataMgrData( sDMUserInfoAlias , _T("총예약책수") , sReservationCnt , 0 ); 
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -8 , _T("CBO_LOC_3180::LoanDBProc_UpdateUser() := GetDataMgrData()") );

	sLoanBookCnt.Format( _T("%d") , _ttoi(sLoanBookCnt) + 1 );

	CString sIsAppendix;
	ids = this->GetDataMgrData( _T("DM_EXP_LOAN_BOOK_INFO") , _T("부록여부") , sIsAppendix , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -9 , _T("CBO_LOC_3180::LoanDBProc_UpdateUser() := GetDataMgrData()") );
	if( sIsAppendix.Compare( _T("A") ) == 0 )
	{
		if( m_pCheckLoan->IsAppendixIncludeLoanCnt() != 0 )
		{
			sLoanBookCnt.Format( _T("%d") , _ttoi(sLoanBookCnt) - 1 );
		}
	}
		
	ids = pDM->AddDBFieldData( _T("LOAN_COUNT") , _T("NUMERIC") , sLoanBookCnt , TRUE );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -11 , _T("CBO_LOC_3180::LoanDBProc_UpdateUser() := AddDBFieldData()") );
	
	CString sLastLoanDate;
	sLastLoanDate += _T("to_date('");
	sLastLoanDate += pDM->GetCellData(_T("LOAN_DATE"), m_nCurrentIndex);
	sLastLoanDate += _T("', 'YYYY/MM/DD:HH24-MI-SS')");
	ids = pDM->AddDBFieldData( _T("LAST_LOAN_DATE ") , _T("NUMERIC") , sLastLoanDate , TRUE );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -13 , _T("CBO_LOC_3180::LoanDBProc_UpdateUser() := AddDBFieldData()") );
	
	if(m_bIsReservedBookByMe==TRUE){
		sReservationCnt.Format( _T("%d") , _ttoi(sReservationCnt) - 1 );
		ids = pDM->AddDBFieldData( _T("RESERVATION_COUNT") , _T("NUMERIC") , sReservationCnt , TRUE );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -14 , _T("CBO_LOC_3180::LoanDBProc_UpdateUser() := AddDBFieldData()") );
	}
	
	pDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), this->GetWorkLogMsg( _T("오프라인대출"), __WFILE__, __LINE__ ), TRUE );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -15 , _T("CBO_LOC_3180::LoanDBProc_UpdateUser() := AddDBFieldData()") );

	ids = pDM->MakeUpdateFrame( _T("CO_LOAN_USER_TBL") , _T("REC_KEY") , _T("NUMERIC") , sRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -17 , _T("CBO_LOC_3180::LoanDBProc_UpdateUser() := MakeUpdateFrame()") );

	pDM->InitDBFieldData();

	return 0;
	EFS_END
	return -1;
}

INT CBO_LOC_3180::SaveUserinfoDM()
{
	EFS_BEGIN

	INT ids=0;

	CESL_DataMgr* pDM = FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	if(pDM == NULL){
		ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_3180::SaveUserinfoDM()") );
	}

	CESL_DataMgr* pDMMain = FindDM(_T("DM_BO_LOC_3180"));
	if(pDMMain == NULL){
		ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CBO_LOC_3180::SaveUserinfoDM()") );
	}

	pDM->FreeData();

	CString sWhere;
	CButton * pBtn = (CButton*)GetDlgItem(IDC_chk3180_USER_MANAGE_CODE);
	if(NULL != pBtn)
	{
		if(TRUE == pBtn->GetCheck())
		{
			sWhere.Format(_T(" CLU.USER_NO = '%s' AND CLU.MANAGE_CODE = '%s'"), m_sUSER_NO, m_pInfo->MANAGE_CODE );
		}
	}
	if(TRUE == sWhere.IsEmpty())
	{	
		sWhere.Format(_T(" CLU.USER_NO = '%s' "), m_sUSER_NO );
	}

	ids = pDM->RefreshDataManager(sWhere);
	if(ids) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("CBO_LOC_3180::SaveUserinfoDM()") );

	if(pDM->GetRowCount() != 1){
		return -1;
	}
	
	if(TRUE == m_pInfo->m_bLibSecuUse && 0 < pDM->GetRowCount())
	{
		CStringArray asDes1,asDes2,asDes3,asDes4,asDes5,asDes6,asDes7;
		for(INT i = 0; i < pDM->GetRowCount(); i++)
		{
			CString strGetData;
			pDM->GetCellData(_T("주민등록번호"), i, strGetData);
			asDes1.Add(strGetData);
			pDM->GetCellData(_T("이메일"), i, strGetData);
			asDes2.Add(strGetData);
			pDM->GetCellData(_T("휴대폰"), i, strGetData);
			asDes3.Add(strGetData);
			pDM->GetCellData(_T("대출자주소"), i, strGetData);
			asDes4.Add(strGetData);
			pDM->GetCellData(_T("자택전화"), i, strGetData);
			asDes5.Add(strGetData);
			pDM->GetCellData(_T("근무지전화"), i, strGetData);
			asDes6.Add(strGetData);
			pDM->GetCellData(_T("회원증비밀번호"), i, strGetData);
			asDes7.Add(strGetData);
		}
		CBO_LOC_3000Api* pApi = new CBO_LOC_3000Api(this);
		if(NULL != pApi)
		{
			if(TRUE == pApi->CheckConnection())
			{
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("CIVIL_NO"), asDes1);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("E_MAIL"), asDes2);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("HANDPHONE"), asDes3);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("H_ADDR1"), asDes4);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("H_PHONE"), asDes5);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("W_PHONE"), asDes6);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("CARD_PASSWORD"), asDes7);
					pApi->CloseSocket();	
				}
				for(i = 0; i < pDM->GetRowCount(); i++)
				{
					pDM->SetCellData(_T("주민등록번호"), asDes1.GetAt(i), i);
					pDM->SetCellData(_T("이메일"), asDes2.GetAt(i), i);
					pDM->SetCellData(_T("휴대폰"), asDes3.GetAt(i), i);
					pDM->SetCellData(_T("대출자주소"), asDes4.GetAt(i), i);
					pDM->SetCellData(_T("자택전화"), asDes5.GetAt(i), i);
					pDM->SetCellData(_T("근무지전화"), asDes6.GetAt(i), i);
					pDM->SetCellData(_T("회원증비밀번호"), asDes7.GetAt(i), i);
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
		asDes6.RemoveAll(); asDes6.FreeExtra();
		asDes7.RemoveAll(); asDes7.FreeExtra();
	}

	ids = AddMemberInfo();
	if(ids) ERROR_MSG_RETURN_ERROR_CODE2( -9 , _T("CBO_LOC_3180::SaveUserinfoDM()") );


	return 0;

	EFS_END
	return -1;
}

INT CBO_LOC_3180::AddExpLoanInfo()
{
	EFS_BEGIN

	INT ids, i;

	CESL_DataMgr *pDstDM = FindDM( _T("DM_EXP_LOAN_BOOK_INFO") );
	if( pDstDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_3180::AddExpLoanInfo()") );

	CESL_DataMgr *pSrcDM = NULL;
	if(m_nLoanBookType == 1 ){
		pSrcDM = FindDM( _T("DM_EXP_LOAN_BO_BOOK_INFO") );
	}else if(m_nLoanBookType == 2 ){
		pSrcDM = FindDM( _T("DM_EXP_LOAN_SE_BOOK_INFO") );
	}else if(m_nLoanBookType == 3 ){
		pSrcDM = FindDM( _T("DM_EXP_LOAN_NON_DB_BOOK_INFO") );
	}
	if( pSrcDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CBO_LOC_3180::AddExpLoanInfo()") );	

	const INT nFieldCnt = 36;
	TCHAR *sDstDMFieldAlias[nFieldCnt] = 
	{
		_T("책KEY") , _T("상태") , _T("자료실구분") , _T("단행/연속구분") , _T("분류기호구분") , 
		_T("분류기호") , _T("관리구분") , _T("등록구분") , _T("종정보키") , _T("등록번호") , 
		_T("MARC") , _T("별치기호") , _T("이용제한구분") , 
		_T("IBS_청구_별치기호_코드") ,	
		_T("IBS_청구_분류기호") ,		
		_T("IBS_청구_도서기호") ,		
		_T("IBS_청구_권책기호") ,		
		_T("IBS_청구_복본기호")	,	
		_T("책값") , _T("형식구분") , _T("이용대상구분") , _T("본표제") , _T("저작자") , _T("발행자") ,
		_T("권호명") , 
		_T("관리구분") ,
		_T("부록여부") , 
		_T("이전자료상태") , 
		_T("대출장소") ,
		_T("대출자KEY") , 
		_T("무인반납중여부") ,
		_T("예약건수") ,
		_T("부록책수") ,
		_T("RFID시리얼") ,
		_T("대출형태") ,
		_T("매체구분")

	};

	CString sGetData;

	pDstDM->FreeData();
	ids = pDstDM->InsertRow(-1);
	if(ids) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("CBO_LOC_3180::AddExpLoanInfo()") );
	for(i=0;i<nFieldCnt;i++){
		sGetData.Empty();
		ids = pSrcDM->GetCellData( sDstDMFieldAlias[i] , 0 , sGetData );
		if( ids < 0 && (ids !=-4012) && (ids!=-111) )
			ERROR_MSG_RETURN_ERROR_CODE2( -11 , _T("CBO_LOC_3180::AddExpLoanInfo()") );

		if( (ids == -111) || (ids == -4012) ) sGetData = _T("");
		ids = pDstDM->SetCellData( sDstDMFieldAlias[i] , sGetData , pDstDM->GetRowCount()-1 ); 
	}
	
	CString sShelfCode;
	MakeShelfCode(this, pDstDM, 0, sShelfCode);

	return 0;

	EFS_END
	return -1;
}

VOID CBO_LOC_3180::OnbRELOAD() 
{
	EFS_BEGIN

	INT ids = 0;

	ids = Reload();
	if(ids){
	}

	return;

	EFS_END
	return;
}

INT CBO_LOC_3180::Reload()
{
	EFS_BEGIN

	CBO_LOC_3183 Dlg(this);

	Dlg.DoModal();

	CESL_DataMgr *pDM = FindDM( _T("DM_BO_LOC_3180") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_3180::Reload()") );

	pDM->GetCellData(_T("WORK_START_TIME"), 0, m_sTime);
	pDM->GetCellData(_T("SEQ_NO"), 0, m_sSEQ_NO);

	return 0;
	EFS_END
	return -1;
}

CString CBO_LOC_3180::BookStatusCodeToDesc(CString &code)
{
	EFS_BEGIN

	if(code.Compare(_T("1"))==0){
		return _T("정상반납");
	}else if(code.Compare(_T("4"))==0){
		return _T("연체반납");
	}else if(code.Compare(_T("5"))==0){
		return _T("파손반납");
	}else if(code.Compare(_T("6"))==0){
		return _T("분실반납");
	}else if(code.Compare(_T("7"))==0){
		return _T("연체파손반납");
	}else if(code.Compare(_T("8"))==0){
		return _T("연체분실반납");
	}else{
		return _T("");
	}
	EFS_END
	return _T("");
}

INT CBO_LOC_3180::ShowMainGrid(CString &sWhere)
{
	EFS_BEGIN

	INT ids=0;
	CESL_DataMgr *pDM = FindDM( _T("DM_BO_LOC_3180") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_3180::ShowMainGrid()") );

	CESL_ControlMgr *pCM = FindCM( _T("CM_BO_LOC_3180") );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CBO_LOC_3180::ShowMainGrid()") );

	pDM->FreeData();
	ids = pDM->RefreshDataManager(sWhere);
	if(ids) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("CBO_LOC_3180::ShowMainGrid()") );

	ids = pCM->AllControlDisplay();
	if(ids) ERROR_MSG_RETURN_ERROR_CODE2( -9 , _T("CBO_LOC_3180::ShowMainGrid()") );

	return 0;
	EFS_END
	return -1;
}

INT CBO_LOC_3180::UpdateOneColumn(CESL_DataMgr *pDM, CString table, CString column, CString type, CString rec_key, CString data)
{
	EFS_BEGIN

	INT ids=0;

	pDM->StartFrame();
	pDM->InitDBFieldData();
	pDM->AddDBFieldData( column, type, data );

	pDM->MakeUpdateFrame( table , _T("REC_KEY") , _T("NUMERIC") , rec_key );

	ids = pDM->SendFrame();
	if(ids){
		pDM->EndFrame();
		return -1;
	}

	pDM->EndFrame();
	return 0;

	EFS_END
	return -1;
}

INT CBO_LOC_3180::UpdateResult(CESL_DataMgr *pDM, CString success_yn, CString fail_code, CString fail_desc, CString &rec_key)
{
	EFS_BEGIN

	INT ids=0;

	pDM->StartFrame();
	pDM->InitDBFieldData();
	pDM->AddDBFieldData( _T("SUCCESS_YN"), _T("STRING"), success_yn );
	pDM->AddDBFieldData( _T("FAIL_CODE"), _T("STRING"), fail_code );
	pDM->AddDBFieldData( _T("FAIL_DESC"), _T("STRING"), fail_desc );

	pDM->MakeUpdateFrame( _T("CO_OFFLINE_LOAN_TBL") , _T("REC_KEY") , _T("NUMERIC") , rec_key );

	ids = pDM->SendFrame();
	if(ids){
		pDM->EndFrame();
		return -1;
	}

	pDM->EndFrame();
	return 0;

	EFS_END
	return -1;
}

CString CBO_LOC_3180::GetSpeciesKey(CESL_DataMgr *pDM, CString recKey, INT type)
{
	CString tblName;
	switch( type )
	{
		case 1 :
			tblName = _T("BO_BOOK_TBL");
			break;
		case 2 :
			tblName = _T("SE_BOOK_TBL");
			break;
		default :
			return _T("");
			break;
	}

	CString query, speciesKey;
	query.Format( _T("SELECT SPECIES_KEY FROM %s WHERE REC_KEY = %s"), tblName, recKey );
	pDM->GetOneValueQuery( query, speciesKey );
	return speciesKey;
}

INT CBO_LOC_3180::ReturnCheckBook()
{
	EFS_BEGIN

	INT ids=0, i=0;

	CESL_DataMgr *pDM = FindDM( _T("DM_BO_LOC_3180_LOAN") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_3180::ReturnCheckBook() := FindDM()") );
	pDM->FreeData();
	
	CString sUserKey;
	ids = this->GetDataMgrData( _T("DM_BO_LOC_3100_MEMBER_INFO") , _T("대출자KEY") , sUserKey , 0 );
	if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CBO_LOC_3180::ReturnCheckBook() := GetDataMgrData()") );

	CString sBookKey;
	if(m_nReturnBookType==1){
		ids = this->GetDataMgrData( _T("DM_EXP_LOAN_BO_BOOK_INFO") , _T("책KEY") , sBookKey , 0 );
	}else if(m_nReturnBookType==2){
		ids = this->GetDataMgrData( _T("DM_EXP_LOAN_SE_BOOK_INFO") , _T("책KEY") , sBookKey , 0 );
	}else if(m_nReturnBookType==3){
		ids = this->GetDataMgrData( _T("DM_EXP_LOAN_NON_DB_BOOK_INFO") , _T("책KEY") , sBookKey , 0 );
	}
	if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("CBO_LOC_3180::ReturnCheckBook() := GetDataMgrData()") );

	CString sWhere;
	sWhere.Format(_T("USER_KEY = %s AND BOOK_KEY = %s AND STATUS IN ('0','2')"), sUserKey, sBookKey);

	ids = pDM->RefreshDataManager(sWhere);
	if(ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -9 , _T("CBO_LOC_3180::ReturnCheckBook() := GetDataMgrData()") );


	if(pDM->GetRowCount() != 1){
		return 3010;
	}	

	return 0;
	EFS_END
	return -1;
}

INT CBO_LOC_3180::ReturnCheckUser()
{
	EFS_BEGIN

	return 0;
	EFS_END
	return -1;
}

INT CBO_LOC_3180::ReturnDBProc_UpdateLoan()
{
	EFS_BEGIN

	INT i=0, ids=0;

	CESL_DataMgr *pDM = FindDM( _T("DM_BO_LOC_3180") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_3180::ReturnDBProc_UpdateLoan() := FindDM()") );

	pDM->StartFrame();
	pDM->InitDBFieldData();
	
	ids = pDM->AddDBFieldData( _T("STATUS") , _T("STRING") , _T("1") , TRUE );
	if(ids < 0) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CBO_LOC_3180::ReturnDBProc_UpdateLoan() := AddDBFieldData()") );
	
	if(m_nReturnType==4){
		ids = pDM->AddDBFieldData( _T("RETURN_TYPE_CODE") , _T("STRING") , _T("4") , TRUE );
	}else if(m_nReturnType==5){
		ids = pDM->AddDBFieldData( _T("RETURN_TYPE_CODE") , _T("STRING") , _T("5") , TRUE );
	}else{
		ids = pDM->AddDBFieldData( _T("RETURN_TYPE_CODE") , _T("STRING") , _T("0") , TRUE );
	}
	if(ids < 0) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("CBO_LOC_3180::ReturnDBProc_UpdateLoan() := AddDBFieldData()") );	

	CString sReparation;
	pDM->GetCellData(_T("REPARATION"), m_nCurrentIndex, sReparation);
	if(!sReparation.IsEmpty()){
		ids = pDM->AddDBFieldData( _T("REPARATION") , _T("STRING") , sReparation , TRUE );
		if(ids < 0) ERROR_MSG_RETURN_ERROR_CODE2( -9 , _T("CBO_LOC_3180::ReturnDBProc_UpdateLoan() := AddDBFieldData()") );
	}

	CString sArrear;
	pDM->GetCellData(_T("ARREAR"), m_nCurrentIndex, sArrear);
	if(sArrear.IsEmpty()){
		ids = GetDelayMoney();
		if(ids < 0) ERROR_MSG_RETURN_ERROR_CODE2( -10 , _T("CBO_LOC_3180::ReturnDBProc_UpdateLoan() := GetDelayMoney()") );

		if(!m_sDelayMoney.IsEmpty()){
			ids = pDM->AddDBFieldData( _T("ARREAR") , _T("STRING") , m_sDelayMoney , TRUE );
			if(ids < 0) ERROR_MSG_RETURN_ERROR_CODE2( -11 , _T("CBO_LOC_3180::ReturnDBProc_UpdateLoan() := AddDBFieldData()") );
		}
	}else{
		ids = pDM->AddDBFieldData( _T("ARREAR") , _T("STRING") , sArrear , TRUE );
		if(ids < 0) ERROR_MSG_RETURN_ERROR_CODE2( -11 , _T("CBO_LOC_3180::ReturnDBProc_UpdateLoan() := AddDBFieldData()") );
	}
	
	CString sTmpDate;
	sTmpDate += _T("to_date('");
	sTmpDate += pDM->GetCellData(_T("RETURN_DATE"), m_nCurrentIndex);
	sTmpDate += _T("', 'YYYY/MM/DD:HH24-MI-SS')");
	ids = pDM->AddDBFieldData( _T("RETURN_DATE") , _T("NUMERIC") , sTmpDate , TRUE);
	if(ids < 0) ERROR_MSG_RETURN_ERROR_CODE2( -13 , _T("CBO_LOC_3180::ReturnDBProc_UpdateLoan() := AddDBFieldData()") );

	pDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), GetWorkLogMsg( _T("오프라인반납"), __WFILE__, __LINE__ ), TRUE );
	if(ids < 0) ERROR_MSG_RETURN_ERROR_CODE2( -15 , _T("CBO_LOC_3180::ReturnDBProc_UpdateLoan() := AddDBFieldData()") );
	
	ids = pDM->AddDBFieldData( _T("R_DEVICE"), _T("STRING"), _T("KOLASIII"), TRUE );
	if(ids < 0) ERROR_MSG_RETURN_ERROR_CODE2( -13 , _T("CBO_LOC_3180::LoanDBProc_UpdateBook() := AddDBFieldData()") );
		
	ids = pDM->AddDBFieldData( _T("MANAGE_CODE"), _T("STRING"), _T("UDF_MANAGE_CODE"), TRUE );
	if(ids < 0) ERROR_MSG_RETURN_ERROR_CODE2( -13 , _T("CBO_LOC_3180::LoanDBProc_UpdateBook() := AddDBFieldData()") );
	
	CString sRecKey;
	ids = this->GetDataMgrData(_T("DM_BO_LOC_3180_LOAN"), _T("KEY"), sRecKey, 0);
	if(ids < 0) ERROR_MSG_RETURN_ERROR_CODE2( -17 , _T("CBO_LOC_3180::ReturnDBProc_UpdateLoan() := GetDataMgrData()") );
	ids = pDM->MakeUpdateFrame( _T("LS_WORK_T01") , _T("REC_KEY") , _T("NUMERIC") , sRecKey );
	if(ids < 0) ERROR_MSG_RETURN_ERROR_CODE2( -17 , _T("CBO_LOC_3180::ReturnDBProc_UpdateLoan() := MakeUpdateFrame()") );

	pDM->InitDBFieldData();

	return 0;
	EFS_END
	return -1;
}

INT CBO_LOC_3180::ReturnDBProc_UpdateBook()
{
	EFS_BEGIN

	INT i=0, ids=0;

	CESL_DataMgr *pDM = FindDM( _T("DM_BO_LOC_3180") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_3180::ReturnDBProc_UpdateBook() := FindDM()") );

	pDM->InitDBFieldData();
	
	CString sTable;
	CString sPreWorkingStatus;
	CString sRecKey;
	if(m_nReturnBookType == 1){ 
		sTable = _T("BO_BOOK_TBL");
		ids = this->GetDataMgrData(_T("DM_EXP_LOAN_BO_BOOK_INFO"), _T("상태") , sPreWorkingStatus , 0 );
		ids = this->GetDataMgrData(_T("DM_EXP_LOAN_BO_BOOK_INFO"), _T("책KEY") , sRecKey , 0 );
	}else if(m_nReturnBookType == 2){ 
		sTable = _T("SE_BOOK_TBL");
		ids = this->GetDataMgrData(_T("DM_EXP_LOAN_SE_BOOK_INFO"), _T("상태") , sPreWorkingStatus , 0 );
		ids = this->GetDataMgrData(_T("DM_EXP_LOAN_SE_BOOK_INFO"), _T("책KEY") , sRecKey , 0 );
	}else if(m_nReturnBookType == 3){ 
		sTable = _T("CO_NON_DB_BOOK_TBL");
		ids = this->GetDataMgrData(_T("DM_EXP_LOAN_NON_DB_BOOK_INFO"), _T("상태") , sPreWorkingStatus , 0 );
		ids = this->GetDataMgrData(_T("DM_EXP_LOAN_NON_DB_BOOK_INFO"), _T("책KEY") , sRecKey , 0 );
	}
	if(ids < 0) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CBO_LOC_3180::ReturnDBProc_UpdateBook() := GetDataMgrData()") );

	ids = pDM->AddDBFieldData( _T("PREV_WORKING_STATUS") , _T("STRING") , sPreWorkingStatus , TRUE );
	if(ids < 0) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("CBO_LOC_3180::ReturnDBProc_UpdateBook() := AddDBFieldData()") );
	
	CString sWorkingStatus;
	sWorkingStatus = GetWorkingStatus();
	ids = pDM->AddDBFieldData( _T("WORKING_STATUS") , _T("STRING") , sWorkingStatus , TRUE );
	if(ids < 0) ERROR_MSG_RETURN_ERROR_CODE2( -11 , _T("CBO_LOC_3180::ReturnDBProc_UpdateBook() := AddDBFieldData()") );

	if(sWorkingStatus.Compare(_T("BOL252N"))==0 || sWorkingStatus.Compare(_T("SEL216O"))==0){
		CString sShelfStatusChangeDate;
		sShelfStatusChangeDate += _T("to_date('");
		sShelfStatusChangeDate += pDM->GetCellData(_T("RETURN_DATE"), m_nCurrentIndex);
		sShelfStatusChangeDate += _T("', 'YYYY/MM/DD:HH24-MI-SS')");
		ids = pDM->AddDBFieldData( _T("LAST_SHELF_STATUS_CHANGE_DATE") , _T("NUMERIC") , sShelfStatusChangeDate , TRUE );
		if(ids < 0) ERROR_MSG_RETURN_ERROR_CODE2( -13 , _T("CBO_LOC_3180::ReturnDBProc_UpdateBook() := AddDBFieldData()") );
	}
	
	pDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), GetWorkLogMsg( _T("오프라인반납"), __WFILE__, __LINE__ ), TRUE );
	if(ids < 0) ERROR_MSG_RETURN_ERROR_CODE2( -15 , _T("CBO_LOC_3180::ReturnDBProc_UpdateBook() := AddDBFieldData()") );
	
	AddIdxQueryFrame( pDM, m_nReturnBookType, sRecKey );
	
	ids = pDM->MakeUpdateFrame(sTable,  _T("REC_KEY") , _T("NUMERIC") , sRecKey );
	if(ids < 0) ERROR_MSG_RETURN_ERROR_CODE2( -17 , _T("CBO_LOC_3180::ReturnDBProc_UpdateBook() := MakeUpdateFrame()") );

	pDM->InitDBFieldData();

	return 0;
	EFS_END
	return -1;
}

INT CBO_LOC_3180::ReturnDBProc_UpdateUser()
{
	EFS_BEGIN

	INT ids=0;

	CESL_DataMgr *pDM = FindDM( _T("DM_BO_LOC_3180") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_3180::ReturnDBProc_UpdateUser() := FindDM()") );

	pDM->InitDBFieldData();
	
	CString sBookCnt;
	ids = this->GetDataMgrData( _T("DM_BO_LOC_3100_MEMBER_INFO") , _T("총대출책수") , sBookCnt , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -5 , _T("CBO_LOC_3180::ReturnDBProc_UpdateUser() := GetDataMgrData()") );
	
	CString sIsAppendix;
	if(m_nReturnBookType==1){
		ids = this->GetDataMgrData( _T("DM_EXP_LOAN_BO_BOOK_INFO") , _T("부록여부") , sIsAppendix , 0 );
	}else if(m_nReturnBookType==2){
		ids = this->GetDataMgrData( _T("DM_EXP_LOAN_SE_BOOK_INFO") , _T("부록여부") , sIsAppendix , 0 );
	}
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -7 , _T("CBO_LOC_3180::ReturnDBProc_UpdateUser() := GetDataMgrData()") );
	if( sIsAppendix.Compare(_T("A")) == 0  )
	{
		if( m_pCheckLoan->IsAppendixIncludeLoanCnt() != 0 )
		{
			sBookCnt.Format( _T("%d") , _ttoi(sBookCnt)+1 );
		}
	}

	sBookCnt.Format( _T("%d") , _ttoi(sBookCnt) - 1);
	ids = pDM->AddDBFieldData( _T("LOAN_COUNT") , _T("NUMERIC") , sBookCnt , TRUE );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -9 , _T("CBO_LOC_3180::ReturnDBProc_UpdateUser() := AddDBFieldData()") );
	
	CString sInLoanStopDate;
	ids = pDM->GetCellData(_T("LOAN_STOP_DATE"), m_nCurrentIndex, sInLoanStopDate);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -11 , _T("CBO_LOC_3180::ReturnDBProc_UpdateUser() := GetCellData()") );

	CString sLoanStopDate;
	CString delayDays;
	CString sLoanStopSetDate;
	ids = GetLoanStopDate( sLoanStopDate, delayDays );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -13 , _T("CBO_LOC_3180::ReturnDBProc_UpdateUser() := GetLoanStopDate()") );
	if(!sLoanStopDate.IsEmpty()){
		if( sIsAppendix.Compare(_T("A")) == 0  )
		{
			if( m_pCheckLoan->IsAppendixIncludeLoanCnt() != 0 )
			{
				sLoanStopDate = _T("");
				delayDays.Empty();
			}
		}

		if( !sLoanStopDate.IsEmpty() )
		{
			if(!sInLoanStopDate.IsEmpty()){
				sLoanStopDate = sInLoanStopDate;
			}
			
			ids = pDM->AddDBFieldData( _T("LOAN_STOP_DATE") , _T("STRING") , sLoanStopDate , TRUE );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -15 , _T("CBO_LOC_3180::ReturnDBProc_UpdateUser() := AddDBFieldData()") );

			
			sLoanStopSetDate.Empty();
			sLoanStopSetDate += _T("to_date('");
			sLoanStopSetDate += pDM->GetCellData(_T("RETURN_DATE"), m_nCurrentIndex);
			sLoanStopSetDate += _T("', 'YYYY/MM/DD:HH24-MI-SS')");
			ids = pDM->AddDBFieldData( _T("LOAN_STOP_SET_DATE") , _T("NUMERIC") , sLoanStopSetDate , TRUE );
			if(ids < 0) ERROR_MSG_RETURN_ERROR_CODE2( -17 , _T("CBO_LOC_3180::ReturnDBProc_UpdateBook() := AddDBFieldData()") );
		}
	}
	
	if(_ttoi(delayDays) > 0){
		INT nTotalDelayCnt=0;
		ids = GetTotalDelayCnt( nTotalDelayCnt );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -19 , _T("CBO_LOC_3180::ReturnDBProc_UpdateBook() := GetTotalDelayCnt()") );

		if( nTotalDelayCnt > 0 )
		{
			
			CString sTotalDelayCnt;
			sTotalDelayCnt.Format(_T("%d") , nTotalDelayCnt );
			pDM->AddDBFieldData( _T("DELAY_COUNT") , _T("NUMERIC") , sTotalDelayCnt , TRUE );
			if(ids < 0) ERROR_MSG_RETURN_ERROR_CODE2( -21 , _T("CBO_LOC_3180::ReturnDBProc_UpdateBook() := AddDBFieldData()") );
		}
			
		CString sDelaydayCnt;
		sDelaydayCnt.Format( _T("DELAYDAY_COUNT + %s"), delayDays );
		pDM->AddDBFieldData( _T("DELAYDAY_COUNT"), _T("NUMERIC"), sDelaydayCnt, TRUE );
		if(ids < 0) ERROR_MSG_RETURN_ERROR_CODE2( -23 , _T("CBO_LOC_3180::ReturnDBProc_UpdateBook() := AddDBFieldData()") );
	}
	
	pDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), GetWorkLogMsg( _T("오프라인반납"), __WFILE__, __LINE__ ), TRUE );
	if(ids < 0) ERROR_MSG_RETURN_ERROR_CODE2( -17 , _T("CBO_LOC_3180::ReturnDBProc_UpdateBook() := AddDBFieldData()") );

	CString sRecKey;
	ids = this->GetDataMgrData( _T("DM_BO_LOC_3100_MEMBER_INFO") , _T("대출자KEY") , sRecKey , 0 );
	if(ids < 0) ERROR_MSG_RETURN_ERROR_CODE2( -19 , _T("CBO_LOC_3180::ReturnDBProc_UpdateBook() := GetDataMgrData()") );
	
	ids = pDM->MakeUpdateFrame( _T("CO_LOAN_USER_TBL") , _T("REC_KEY") , _T("NUMERIC") , sRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -21 , _T("CBO_LOC_3180::ReturnDBProc_UpdateBook() := MakeUpdateFrame()") );

	pDM->InitDBFieldData();

	return 0;
	EFS_END
	return -1;
}

INT CBO_LOC_3180::SaveBookinfoDM()
{
	EFS_BEGIN

	INT ids=0;
	
	ids = GetBookType();
	if(ids<0){
		return -1;
	}else if(ids==0){
		return -3;
	}else if(ids>1){
		return -5;
	}

	if(m_sTYPE.Compare(_T("대출"))==0 || m_sTYPE.Compare(_T("무인대출"))==0){
		ids = AddExpLoanInfo();
		if(ids){
		}
	}

	return 0;
	EFS_END
	return -1;
}

CString CBO_LOC_3180::GetWorkingStatus()
{
	INT ids=0;
	CString sWorkingStatus=_T("");
	CString sBookStatusDesc;

	CESL_DataMgr *pDM = FindDM( _T("DM_BO_LOC_3180") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_3180::GetWorkingStatus() := FindDM()") );

	pDM->GetCellData(_T("BOOK_STATUS_DESC"), m_nCurrentIndex, sBookStatusDesc);

	if(m_nReturnBookType == 1 || m_nReturnBookType == 3){
		if(sBookStatusDesc.Compare(_T("정상반납"))==0){
			sWorkingStatus = _T("BOL112N");
		}else if(sBookStatusDesc.Compare(_T("연체반납"))==0){
			sWorkingStatus = _T("BOL112N");
		}else if(sBookStatusDesc.Compare(_T("파손반납"))==0){
			sWorkingStatus = _T("BOL114O");
		}else if(sBookStatusDesc.Compare(_T("분실반납"))==0){
			sWorkingStatus = _T("BOL252N");
		}else if(sBookStatusDesc.Compare(_T("연체파손반납"))==0){
			sWorkingStatus = _T("BOL114O");
		}else if(sBookStatusDesc.Compare(_T("연체분실반납"))==0){
			sWorkingStatus = _T("BOL252N");
		}
	}else if(m_nReturnBookType == 2){
		if(sBookStatusDesc.Compare(_T("정상반납"))==0){
			this->GetDataMgrData(_T("DM_EXP_LOAN_SE_BOOK_INFO") , _T("이전자료상태") , sWorkingStatus , 0 );
		}else if(sBookStatusDesc.Compare(_T("연체반납"))==0){
			this->GetDataMgrData(_T("DM_EXP_LOAN_SE_BOOK_INFO") , _T("이전자료상태") , sWorkingStatus , 0 );
		}else if(sBookStatusDesc.Compare(_T("파손반납"))==0){
			sWorkingStatus = _T("SEL214N");
		}else if(sBookStatusDesc.Compare(_T("분실반납"))==0){
			sWorkingStatus = _T("SEL216O");
		}else if(sBookStatusDesc.Compare(_T("연체파손반납"))==0){
			sWorkingStatus = _T("SEL214N");
		}else if(sBookStatusDesc.Compare(_T("연체분실반납"))==0){
			sWorkingStatus = _T("SEL216O");
		}
	}

	return sWorkingStatus;
}

INT CBO_LOC_3180::AddMemberInfo()
{
	EFS_BEGIN

	INT ids;
	CString sDMAlias = _T("DM_BO_LOC_3100_MEMBER_INFO");

	INT nMaxLoanBookCnt;
	ids = m_pCheckLoan->GetLoanBookCnt( nMaxLoanBookCnt );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_3180::AddMemberInfo() := GetLoanBookCnt()") );
	
	CString sTotalLoanBookCnt;
	ids = this->GetDataMgrData( sDMAlias , _T("총대출책수") , sTotalLoanBookCnt , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CBO_LOC_3180::AddMemberInfo() := GetDataMgrData()") );

	CString sViewLoanBookCnt;
	sViewLoanBookCnt.Format( _T("%s/%d") , sTotalLoanBookCnt,nMaxLoanBookCnt );
	CString sIsLoanBookCnt;
	sIsLoanBookCnt.Format( _T("%d") , nMaxLoanBookCnt-_ttoi(sTotalLoanBookCnt) );

	ids = this->SetDataMgrData( sDMAlias , _T("대출책수상태") , sViewLoanBookCnt , 0);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("CBO_LOC_3180::AddMemberInfo() := SetDataMgrData()") );
	ids = this->SetDataMgrData( sDMAlias , _T("대출가능책수") , sIsLoanBookCnt , 0);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -9 , _T("CBO_LOC_3180::AddMemberInfo() := SetDataMgrData()") );
	
	INT nMaxReservationBookCnt;
	ids = m_pCheckLoan->GetReserveBookCnt( nMaxReservationBookCnt );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -11 , _T("CBO_LOC_3180::AddMemberInfo() := GetReserveBookCnt()") );
	
	CString sTotalReservationBookCnt;
	ids = this->GetDataMgrData( sDMAlias , _T("총예약책수") , sTotalReservationBookCnt , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -13 , _T("CBO_LOC_3180::AddMemberInfo() := GetDataMgrData()") );

	if( sTotalReservationBookCnt.IsEmpty() ) sTotalReservationBookCnt = _T("0");

	CString sViewReservationBookCnt;
	sViewReservationBookCnt.Format( _T("%s/%d") , sTotalReservationBookCnt,nMaxReservationBookCnt );
	CString sIsReservationBookCnt;
	sIsReservationBookCnt.Format( _T("%d") , nMaxReservationBookCnt-_ttoi(sTotalReservationBookCnt) );

	ids = this->SetDataMgrData( sDMAlias , _T("예약책수상태") , sViewReservationBookCnt , 0);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -15 , _T("CBO_LOC_3180::AddMemberInfo() := SetDataMgrData()") );
	ids = this->SetDataMgrData( sDMAlias , _T("예약가능책수") , sIsReservationBookCnt , 0);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -17 , _T("CBO_LOC_3180::AddMemberInfo() := SetDataMgrData()") );
	
	CString sMemberState;
	ids = this->GetDataMgrData( sDMAlias , _T("회원상태") , sMemberState , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -21 , _T("CBO_LOC_3180::AddMemberInfo() := GetDataMgrData()") );
	
	if( sMemberState.Compare( _T("0") ) == 0 )
		sMemberState = _T("정상");
	else if( sMemberState.Compare( _T("1") ) == 0 )
		sMemberState = _T("대출정지");
	else if( sMemberState.Compare( _T("2") ) == 0 )
		sMemberState = _T("제적");
	
	ids = this->SetDataMgrData( sDMAlias , _T("회원상태설명") , sMemberState , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -23 , _T("CBO_LOC_3180::AddMemberInfo() := SetDataMgrData()") );
	
	return 0;

	EFS_END
	return -1;
}

INT CBO_LOC_3180::CheckDuplicate()
{
	EFS_BEGIN

	INT ids=0, i=0;

	CESL_DataMgr *pDM = FindDM( _T("DM_BO_LOC_3180_LOAN") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_3180::CheckDuplicate() := FindDM()") );
	pDM->FreeData();


	CString sUserKey;
	ids = this->GetDataMgrData( _T("DM_BO_LOC_3100_MEMBER_INFO") , _T("대출자KEY") , sUserKey , 0 );
	if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CBO_LOC_3180::CheckDuplicate() := GetDataMgrData()") );


	CString sBookKey;
	if(m_nReturnBookType==1){
		ids = this->GetDataMgrData( _T("DM_EXP_LOAN_BO_BOOK_INFO") , _T("책KEY") , sBookKey , 0 );
	}else if(m_nReturnBookType==2){
		ids = this->GetDataMgrData( _T("DM_EXP_LOAN_SE_BOOK_INFO") , _T("책KEY") , sBookKey , 0 );
	}else if(m_nReturnBookType==3){
		ids = this->GetDataMgrData( _T("DM_EXP_LOAN_NON_DB_BOOK_INFO") , _T("책KEY") , sBookKey , 0 );
	}
	if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("CBO_LOC_3180::CheckDuplicate() := GetDataMgrData()") );


	CString sWhere;
	CString sTmpDate;
	if(m_sTYPE.Compare(_T("대출"))==0 || m_sTYPE.Compare(_T("무인대출"))==0){
		ids = this->GetDataMgrData(_T("DM_BO_LOC_3180"), _T("LOAN_DATE"), sTmpDate, m_nCurrentIndex);
		if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2( -9 , _T("CBO_LOC_3180::CheckDuplicate() := GetDataMgrData()") );
		sWhere.Format(_T("USER_KEY = %s AND BOOK_KEY = %s AND TO_CHAR(LOAN_DATE, 'YYYY/MM/DD:HH24-MI-SS')='%s'"), sUserKey, sBookKey, sTmpDate);
	}else if(m_sTYPE.Compare(_T("반납"))==0 || m_sTYPE.Compare(_T("무인반납"))==0){
		ids = this->GetDataMgrData(_T("DM_BO_LOC_3180"), _T("RETURN_DATE"), sTmpDate, m_nCurrentIndex);
		if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2( -11 , _T("CBO_LOC_3180::CheckDuplicate() := GetDataMgrData()") );
		sWhere.Format(_T("USER_KEY = %s AND BOOK_KEY = %s AND TO_CHAR(RETURN_DATE, 'YYYY/MM/DD:HH24-MI-SS')='%s'"), sUserKey, sBookKey, sTmpDate);
	}

	ids = pDM->RefreshDataManager(sWhere);
	if(ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -13 , _T("CBO_LOC_3180::CheckDuplicate() := RefreshDataManager()") );


	if(pDM->GetRowCount() > 0){
		return -15;
	}

	return 0;
	EFS_END
	return -1;
}

INT CBO_LOC_3180::MakeShelfCode(CESL_Mgr *pEslMgr, CESL_DataMgr *pDM, INT nIndex, CString &sShelfCode)
{
	EFS_BEGIN

	INT ids;

	sShelfCode.Empty();
	CString sTmpCode;
	
	const INT nCnt = 5;
	TCHAR *sFieldAlias[nCnt] =
	{
		_T("IBS_청구_별치기호_코드") ,	
		_T("IBS_청구_분류기호") ,		
		_T("IBS_청구_도서기호") ,		
		_T("IBS_청구_권책기호") ,		
		_T("IBS_청구_복본기호") ,		
	};
	

	for( INT j = 0 ; j < nCnt ; j++ )
	{
		ids = pDM->GetCellData( sFieldAlias[j] , nIndex , sTmpCode );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(-2 , _T("CBO_LOC_3180::MakeShelfCode( CString sDMAlias )") );
		sTmpCode.TrimLeft(); sTmpCode.TrimRight();
		
		if( j == 0 && !sTmpCode.IsEmpty() )
		{
			pEslMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("별치기호") , sTmpCode , sTmpCode );
		}
		
		if( !sTmpCode.IsEmpty() ) 
		{
			if( sShelfCode.GetLength() > 1 )
			{
				
				if( j == 1 )
					sShelfCode += _T(" ");
				else if( j == 4 )
					sShelfCode += _T("=");
				else
					sShelfCode += _T("-");
			}
			sShelfCode += sTmpCode;
		}
	}
	
	
	ids = pDM->SetCellData( _T("청구기호") , sShelfCode , nIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_3180::MakeShelfCode( CString sDMAlias )") );
	
	return 0;

	EFS_END

	return -1;
}

INT CBO_LOC_3180::RecordReserveExpireDate()
{
	EFS_BEGIN

	INT ids=0;

	CESL_DataMgr *pDM = FindDM( _T("DM_BO_LOC_3180") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_3180::RecordReserveExpireDate() := FindDM()") );

	CString sBookKey;
	ids = this->GetDataMgrData( _T("DM_BO_LOC_3180_LOAN") , _T("책KEY") , sBookKey , 0 );
	if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CBO_LOC_3180::RecordReserveExpireDate() := GetDataMgrData()") );

	CString sSQL;
	sSQL.Format( _T("SELECT REC_KEY FROM LS_WORK_T01 WHERE BOOK_KEY = %s AND STATUS = '3' AND RESERVATION_DATE = ")
		_T("(SELECT MIN(RESERVATION_DATE) FROM LS_WORK_T01 WHERE BOOK_KEY = %s AND STATUS = '3')") , sBookKey , sBookKey );

	CString sLoanKey;
	ids = pDM->GetOneValueQuery( sSQL , sLoanKey );

	
	if( sLoanKey.IsEmpty() )
		return 0;

	
	CString sTmp;
	sTmp = _T("등록번호[") + m_sREG_NO + _T("]는 예약된 자료입니다. 예약 처리를 하시겠습니까?");
	if(MessageBox(sTmp, _T("예약 처리"), MB_YESNO|MB_ICONQUESTION ) == IDNO ){
		return 0;	
	}
	
	CESL_DataMgr *pDMLoanInfo = FindDM( _T("DM_BO_LOC_3100_LOAN_INFO") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -11 , _T("CBO_LOC_3180::RecordReserveExpireDate() := FindDM()") );

	pDMLoanInfo->FreeData();
	pDMLoanInfo->InsertRow(-1);
	
	pDMLoanInfo->SetCellData(_T("책KEY"), sBookKey, 0);

	CString sPublishFormCode;
	if(m_nReturnBookType==1){
		sPublishFormCode = _T("MO");
	}else if(m_nReturnBookType==2){
		sPublishFormCode = _T("SE");
	}
	pDMLoanInfo->SetCellData(_T("단행/연속구분"), sPublishFormCode, 0);


	CBO_LOC_3170 Dlg(this , m_pLoanShareManager , m_pLona , 0 );
	Dlg.DoModal();

	pDMLoanInfo->FreeData();

	return 0;
	EFS_END
	return -1;
}

INT CBO_LOC_3180::InitExpDM()
{
	EFS_BEGIN

	const INT nDMCount = 6;
	TCHAR *sDMAlias[nDMCount] = 
	{
		_T("DM_EXP_LOAN_BO_BOOK_INFO") , _T("DM_EXP_LOAN_SE_BOOK_INFO") , _T("DM_EXP_LOAN_NON_DB_BOOK_INFO") ,
		_T("DM_EXP_LOAN_BOOK_INFO"),
		_T("DM_BO_LOC_3100_MEMBER_INFO"),
		_T("DM_BO_LOC_3180_LOAN")
	};

	CESL_DataMgr *pDM;
	for( INT i = 0 ; i < nDMCount ; i++ )
	{
		pDM = FindDM( sDMAlias[i] );
		if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("CBO_LOC_3180::InitExpDM()") );

		pDM->FreeData();
	}

	return 0;

	EFS_END
	return -1;
}

INT CBO_LOC_3180::ShareInfoCenterUpload(CString sLoanInfoType, CString sUserInfoType)
{
	EFS_BEGIN

	UPLOAD_SHARE_INFO UploadShareInfo;
	UploadShareInfo.nType = 0;

	UploadShareInfo.UploadShareLoanInfo.sCommand = _T("51");
	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableName = _T("LH_STORAGE_V01");
	
	
	UploadShareInfo.UploadShareLoanInfo.sLibCode = m_pParent->m_pInfo->LIB_CODE;

	
	if( sLoanInfoType.Compare(_T("I")) == 0 )
	{
		CString sRecKey;
		m_pSpfWork->GetTempData( SHARE_CO_LOAN_TBL_INSERT_REC_KEY , sRecKey );
		
		
		UploadShareInfo.UploadShareLoanInfo.sType = _T("I");
		
		UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = sRecKey;
		
	}
	else if( sLoanInfoType.Compare(_T("U")) == 0 )
	{
		CString sRecKey;
		CString sUpdateFields;
		m_pSpfWork->GetTempData( SHARE_CO_LOAN_TBL_UPDATE_REC_KEY  , sRecKey );
		m_pSpfWork->GetTempData( SHARE_CO_LOAN_TBL_UPDATE_FIELDS  , sUpdateFields );
		
		UploadShareInfo.UploadShareLoanInfo.sType = _T("U");
		
		UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = sRecKey;
		UploadShareInfo.UploadShareLoanInfo.sUpdatedFields = sUpdateFields;				
	}
	else if( sLoanInfoType.Compare(_T("D")) == 0  )
	{
		CString sRecKey;
		m_pSpfWork->GetTempData( SHARE_CO_LOAN_TBL_DELETE_REC_KEY  , sRecKey );
		
		UploadShareInfo.UploadShareLoanInfo.sType = _T("D");
		
		UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = sRecKey;
		
	}

	if( sLoanInfoType.Compare(_T("N")) != 0 )
		m_pLoanShareManager->SendLoanInfoUpload( UploadShareInfo );
	
	
	UploadShareInfo.UploadShareLoanInfo.sCommand = _T("11");
	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableName = _T("CO_LOAN_USER_TBL");
	UploadShareInfo.UploadShareLoanInfo.sMasterUserInfoChangeYN = _T("Y");
	
	if( sUserInfoType.Compare(_T("I")) == 0 )
	{
		CString sRecKey;
		m_pSpfWork->GetTempData( SHARE_CO_LOAN_USER_TBL_INSERT_REC_KEY , sRecKey );

		UploadShareInfo.UploadShareLoanInfo.sType = _T("I");
		
		UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = sRecKey;
		
	}
	else if( sUserInfoType.Compare(_T("U")) == 0 )
	{
		CString sRecKey;
		CString sUpdateFields;
		m_pSpfWork->GetTempData( SHARE_CO_LOAN_USER_TBL_UPDATE_REC_KEY  , sRecKey );
		m_pSpfWork->GetTempData( SHARE_CO_LOAN_USER_TBL_UPDATE_FIELDS  , sUpdateFields );
				
		UploadShareInfo.UploadShareLoanInfo.sType = _T("U");
		
		UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = sRecKey;
		UploadShareInfo.UploadShareLoanInfo.sUpdatedFields = sUpdateFields;		
	}
	else if( sUserInfoType.Compare(_T("D")) == 0 )
	{
		CString sRecKey;
		m_pSpfWork->GetTempData( SHARE_CO_LOAN_USER_TBL_DELETE_REC_KEY  , sRecKey );
		
		UploadShareInfo.UploadShareLoanInfo.sType = _T("D");
		
		UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = sRecKey;
		
	}
	if( sUserInfoType.Compare(_T("N")) != 0 )
		m_pLoanShareManager->SendLoanInfoUpload( UploadShareInfo );

	return 0;

	EFS_END

	return -1;
}

INT CBO_LOC_3180::GetDelayMoney()
{
	EFS_BEGIN

	INT ids;
	
	ids = m_pCheckLoan->IsSetDelayMoney();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_3180::GetDelayMoney() := m_pCheckLoan->IsSetDelayMoney()") );
	if( ids > 0 ) return 0;
	
	CString sReturnDate;
	ids = this->GetDataMgrData(_T("DM_BO_LOC_3180"), _T("RETURN_DATE"), sReturnDate, m_nCurrentIndex);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CBO_LOC_3180::GetDelayMoney() := GetDataMgrData()") );
	if( sReturnDate.IsEmpty() ) return 0;
	
	CTime tReturnDate( _ttoi(sReturnDate.Mid(0,4)) , _ttoi(sReturnDate.Mid(5,2)) , _ttoi(sReturnDate.Mid(8,2)) , 0 , 0 , 0 );
	
	CString sReturnPlanDate;
	ids = this->GetDataMgrData(_T("DM_BO_LOC_3180_LOAN"), _T("반납예정일"), sReturnPlanDate, 0);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("CBO_LOC_3180::GetDelayMoney() := GetDataMgrData()") );
	if( sReturnPlanDate.IsEmpty() ) return 0;

	CTime tReturnPlanDate( _ttoi(sReturnPlanDate.Mid(0,4)) , _ttoi(sReturnPlanDate.Mid(5,2)) , _ttoi(sReturnPlanDate.Mid(8,2)) , 0 , 0 , 0 );
	
	CTimeSpan tDelayDay(0,0,0,0);
	if( tReturnDate > tReturnPlanDate ){
		tDelayDay = tReturnDate - tReturnPlanDate;
	}else{
		return 0;
	}

	long nDelayDay = tDelayDay.GetDays();

	long nDelayMoneyPerDay;
	ids = m_pCheckLoan->GetDelayMoney( nDelayMoneyPerDay );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -11 , _T("CBO_LOC_3180::GetDelayMoney() := m_pCheckLoan->GetDelayMoney()") );

	long nDelayMoney;
	
	nDelayMoney = nDelayDay*nDelayMoneyPerDay;
	
	
	if(nDelayMoney > 0){
		m_sDelayMoney.Format(_T("%i"), nDelayMoney);
	}

	return 0;

	EFS_END
	return -1;
}

INT CBO_LOC_3180::GetLoanStopDate(CString &sLoanStopDate, CString &delayDays)
{
	EFS_BEGIN

	INT ids;

	sLoanStopDate.Empty();
	delayDays.Empty();
	
	CString sReturnDate;
	ids = this->GetDataMgrData(_T("DM_BO_LOC_3180"), _T("RETURN_DATE"), sReturnDate, m_nCurrentIndex);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CBO_LOC_3180::GetLoanStopDate() := GetDataMgrData()") );
	if( sReturnDate.IsEmpty() ) return 0;
	
	CTime tReturnDate( _ttoi(sReturnDate.Mid(0,4)) , _ttoi(sReturnDate.Mid(5,2)) , _ttoi(sReturnDate.Mid(8,2)) , 0 , 0 , 0 );
	
	CString sReturnPlanDate;
	ids = this->GetDataMgrData(_T("DM_BO_LOC_3180_LOAN"), _T("반납예정일"), sReturnPlanDate, 0);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("CBO_LOC_3180::GetLoanStopDate() := GetDataMgrData()") );
	if( sReturnPlanDate.IsEmpty() ) return 0;
	
	CTime tReturnPlanDate( _ttoi(sReturnPlanDate.Mid(0,4)) , _ttoi(sReturnPlanDate.Mid(5,2)) , _ttoi(sReturnPlanDate.Mid(8,2)) , 0 , 0 , 0 );
	
	CTimeSpan tDelayDay(0,0,0,0);
	INT nDelayDay ;
	if( tReturnDate > tReturnPlanDate ){
		tDelayDay = tReturnDate - tReturnPlanDate;
	}else{
		return 0;
	}
	delayDays.Format( _T("%d"), tDelayDay.GetDays() );
	nDelayDay = tDelayDay.GetDays();
	if(nDelayDay < 1) return 0;
	
	ids = m_pCheckLoan->IsSetLoanStopDate();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -8 , _T("CBO_LOC_3180::GetLoanStopDate() := m_pCheckLoan->IsSetLoanStopDate()") );
	if( ids > 0 ) return 0;
	
	INT nDelayAddDay;
	ids = m_pCheckLoan->GetDelayAddDay( nDelayAddDay );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -11 , _T("CBO_LOC_3180::GetLoanStopDate() := m_pCheckLoan->GetDelayAddDay()") );
	
	INT nDelayDayCount = m_pCheckLoan->GetDelayDayCnt();

	INT nLoanStopDayCount = m_pCheckLoan->GetLoanStopDayCnt();

	nDelayDay = ( nDelayDay + nLoanStopDayCount ) * nDelayAddDay + nDelayDayCount;
	if( nDelayDay < 1 ) return 0;
	
	tDelayDay = CTimeSpan( nDelayDay , 0 , 0 , 0 );
	if( tDelayDay.GetDays() < 1 ) return 0;

	CString sPreLoanStopDate;
	CTime tPreLoanStopDate( 1971 , 1 , 1 , 0 , 0 , 0 );
	ids = this->GetDataMgrData( _T("DM_BO_LOC_3100_MEMBER_INFO") , _T("대출정지일") , sPreLoanStopDate , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -9 , _T("CBO_LOC_3180::GetLoanStopDate() := GetDataMgrData()") );
	
	if( !sPreLoanStopDate.IsEmpty() )
	{
		CTime tTempDate( _ttoi(sPreLoanStopDate.Mid(0,4)) , _ttoi(sPreLoanStopDate.Mid(5,2)) , _ttoi(sPreLoanStopDate.Mid(8,2)) , 0 , 0 , 0 );
		tPreLoanStopDate = tTempDate;
	}
	
	INT nDelayMode;
	ids = m_pCheckLoan->GetLoanStopDateOption( nDelayMode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -15 , _T("CBO_LOC_3180::GetLoanStopDate() := m_pCheckLoan->GetLoanStopDateOption()") );

	CTimeSpan tOneDay( 1, 0 , 0 , 0 );	

	if( nDelayMode == 1 )
	{
		if( tPreLoanStopDate < tReturnDate )
		{
			tDelayDay = tDelayDay - tOneDay;
			tReturnDate += tDelayDay;
			sLoanStopDate = tReturnDate.Format( _T("%Y/%m/%d") );
		}
		else
		{
			tPreLoanStopDate += tDelayDay;
			sLoanStopDate = tPreLoanStopDate.Format( _T("%Y/%m/%d") );

		}
	}
	else
	{
		tDelayDay = tDelayDay - tOneDay;

		CTime tExpLoanStopDate = tReturnDate + tDelayDay;
		if( tPreLoanStopDate < tExpLoanStopDate )
		{
			sLoanStopDate = tExpLoanStopDate.Format(_T("%Y/%m/%d"));
		}
	}

	return 0;

	EFS_END
	return -1;
}

INT CBO_LOC_3180::GetTotalDelayCnt(INT &nTotalDelayCnt)
{
	EFS_BEGIN

	INT ids;

	nTotalDelayCnt = 0;

	CESL_DataMgr *pDM = FindDM( _T("DM_BO_LOC_3100_MEMBER_INFO") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_3180::GetTotalDelayCnt() := FindDM()") );
	
	CString sUserRecKey;
	ids = pDM->GetCellData(_T("대출자KEY"), 0, sUserRecKey);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CBO_LOC_3180::GetTotalDelayCnt() := GetCellData()") ); 
	if(sUserRecKey.IsEmpty()) return 0;
	
	CString sExpLoanStopSetDate;
	ids = this->GetDataMgrData(_T("DM_BO_LOC_3180"), _T("RETURN_DATE"), sExpLoanStopSetDate, m_nCurrentIndex);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("CBO_LOC_3180::GetTotalDelayCnt() := GetDataMgrData()") );
	if(sExpLoanStopSetDate.IsEmpty()) return 0;
	sExpLoanStopSetDate = sExpLoanStopSetDate.Mid(0,10);
	
	CString sSql;
	CString sCnt;
	sSql.Format( _T("SELECT COUNT(*) FROM CO_LOAN_USER_TBL WHERE REC_KEY=%s AND to_char(LOAN_STOP_SET_DATE,'YYYY/MM/DD')='%s' ") 
		, sUserRecKey , sExpLoanStopSetDate );

	ids = pDM->GetOneValueQuery( sSql , sCnt );	
	
	CString sValue;
	m_pCheckLoan->GetMethodDelayCntPlus( sValue );
	
	if( _ttoi(sValue) == 1 ) sCnt = _T("0");

	if( _ttoi(sCnt) > 0 )
	{
		nTotalDelayCnt = 0;
	}
	else
	{
		sSql.Format( _T("SELECT DELAY_COUNT FROM CO_LOAN_USER_TBL WHERE REC_KEY=%s "), sUserRecKey );

		ids = pDM->GetOneValueQuery( sSql , sCnt );

		nTotalDelayCnt = _ttoi( sCnt ) + 1;
	}

	return 0;

	EFS_END
	return -1;
}

HBRUSH CBO_LOC_3180::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}

INT CBO_LOC_3180::FileCodeChange(LPCTSTR lpFilePath)
{
	
	INT		ids, nByte;
	CHAR	*sMultyByte = NULL;
	TCHAR	*sWideByte = NULL;
	CharConvert Convert;
	
	ids = Convert.IsUnicodeFile((TCHAR*)lpFilePath, &nByte);
	if ( ids < 0 ) return -10;
	if ( ISUNICODE != ids ) {
		ids = Convert.ReadAllOfFile((TCHAR*)lpFilePath, &sMultyByte, &nByte);
		if ( ids ) return -20;

		ids = Convert.IsUTF8File((TCHAR*)lpFilePath, &nByte);
		if ( ids < 0 ) {
			free(sMultyByte);
			return -30;
		}
		if ( ISUTF8 != ids ) {
			ids = Convert.MultiByteToUnicode(sMultyByte, &sWideByte, &nByte);
			if ( ids ) {
				free(sMultyByte);
				return -40;
			}
		} else {
			ids = Convert.UTF8ToUnicode(sMultyByte, &sWideByte, &nByte);
			if ( ids ) {
				free(sMultyByte);
				return -50;
			}
		}
		ids = Convert.WriteToFile((TCHAR*)lpFilePath, sWideByte, nByte);
		free(sMultyByte);
		free(sWideByte);
		if ( ids ) return -60;
	}
	return 0;
}

INT CBO_LOC_3180::AddIdxQueryFrame( CESL_DataMgr *pDM, INT type, CString sRecKey )
{
	EFS_BEGIN

	if( (m_pParent->m_pInfo->MODE == 10000) || (m_pParent->m_pInfo->MODE == 30000) )
		return 0;

	CString sIDXTblName, sProcName;
	switch( type )
	{
		case 1 :
			sIDXTblName = _T("IDX_BO_TBL");			
			sProcName = _T("TESTWORKSTATUS");
			break;
		case 2 :
			sIDXTblName = _T("IDX_SE_TBL");			
			sProcName = _T("TESTSEWORKSTATUS");
			break;
		default :
			return -1;
			break;
	}

	CString sSpeciesKey = GetSpeciesKey( pDM, sRecKey, type );

	CString sQuery;
	sQuery.Format( _T("UPDATE %s SET WORKING_STATUS = %s(%s) WHERE REC_KEY = %s;") ,sIDXTblName, sProcName , sSpeciesKey , sSpeciesKey);	
	pDM->AddFrame( sQuery );

	return 0;

	EFS_END

	return -1;
}