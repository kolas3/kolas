// CheckinVolESL.cpp : implementation file
//

#include "stdafx.h"
#include "CheckinVolESL.h"
#include "..\SE_API\SeExpectMgr.h"
#include "..\SE_API\SeApi.h"
#include "..\SE_EXPECT\SeExpectForm.h"
#include "..\SE_VOL\SeVolDlg.h"
#include "CheckinVolMoreExp.h"
#include "CheckinVolSetMoreExp.h"
#include "..\..\..\관리\목차초록관리\ContentsMgr_Editor\CONTENTMGR_SE_EDIT.h"
#include "..\SE_API\CheckinReceipt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCheckinVolESL dialog


CCheckinVolESL::CCheckinVolESL(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CCheckinVolESL)
	m_bIsAllDataShow = FALSE;
	//}}AFX_DATA_INIT
	
	//실제 VIEW에게 보내는 메시지를 차단한다.
	pMain = NULL;

	m_pCM = NULL;
	m_pDM_IDX = NULL;
	m_pParentGrid = NULL;
	m_nACQ_TYPE = 1; //default
	m_nCurrentParentIdx = 0;
	m_nCurrentIdx = -1;
	m_pGrid = NULL;
	m_pDM_EXP = NULL;
	m_pDM_DB = NULL;
	m_pDM_WORK = NULL;
	m_pDM_ACQ = NULL;
	
	m_strSPECIES_KEY = _T("");
	m_strPUB_FREQ = _T("");
	m_strACQ_YEAR = _T("");
	m_strRECEIPT_NO = _T("");

	m_pProgressDlg = NULL;
	m_pResourceMgr = NULL;

	//color ref init
	CR_exp = RGB(255,255,255);
	CR_acq = RGB(255,255,255);
	CR_lack = RGB(255,255,255);
	CR_miss = RGB(255,255,255);
	CR_delay = RGB(255,255,255);
	CR_nonpub = RGB(255,255,255);

	CR_BLACK = RGB(0,0,0);

	m_bIsLightVersion = FALSE;

	m_bIsNeedReceiptNo = FALSE;

	m_bIsDepartMngUse = FALSE;
	m_pDM_DEPART = NULL;
}

CCheckinVolESL::~CCheckinVolESL()
{
	//FreeChild();
}

VOID CCheckinVolESL::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCheckinVolESL)
	DDX_Control(pDX, IDC_tabCHECKIN_VOL, m_tabCTRL);
	DDX_Check(pDX, IDC_chkCHECKIN_VOL_ALLSHOW, m_bIsAllDataShow);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCheckinVolESL, CDialog)
	//{{AFX_MSG_MAP(CCheckinVolESL)
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_tabCHECKIN_VOL, OnSelchangetabCHECKINVOL)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_btnCHECKIN_VOL_PREV, OnbtnCHECKINVOLPREV)
	ON_BN_CLICKED(IDC_btnCHECKIN_VOL_NEXT, OnbtnCHECKINVOLNEXT)
	ON_BN_CLICKED(IDC_chkCHECKIN_VOL_ALLSHOW, OnchkCHECKINVOLALLSHOW)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCheckinVolESL message handlers

BOOL CCheckinVolESL::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}


INT CCheckinVolESL::SetParentInfo(CESL_DataMgr *pParentDM, 
									   CESL_Grid *pParentGrid,
									   INT nACQ_TYPE,
									   CString strACQ_YEAR,
									   CString strRECEIPT_NO,
									   BOOL bIsLightVersion)
{
	m_pDM_IDX = pParentDM;
	m_pParentGrid = pParentGrid;
	m_nACQ_TYPE = nACQ_TYPE;
	m_strACQ_YEAR = strACQ_YEAR;
	m_strRECEIPT_NO = strRECEIPT_NO;
	m_bIsLightVersion = bIsLightVersion;

	return 0;
}


BOOL CCheckinVolESL::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(InitESL_Mgr(_T("연속_수서_체크인_권단위"))<0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error : 연속_수서_체크인_권단위"));
		return FALSE;
	}
	
	m_tabCTRL.InsertItem(0,_T("예정/결호"));
	m_tabCTRL.InsertItem(1,_T("입수"));
	m_tabCTRL.InsertItem(2,_T("입수/결호"));	
	m_tabCTRL.InsertItem(3,_T("결호"));	
	
	
	m_pCM = FindCM(_T("CM_연속_체크인_권단위"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("그리드"));

	
	m_pDM_EXP = FindDM(_T("DM_체크인_예측"));
	m_pDM_DB = FindDM(_T("DM_체크인_DB"));
	m_pDM_WORK = FindDM(_T("DM_체크인_작업"));

	//수입 구분 정보
	if(1 == m_nACQ_TYPE) 
		m_pDM_ACQ = FindDM(_T("DM_체크인_구입정보"));
	else
		m_pDM_ACQ = FindDM(_T("DM_체크인_기증정보"));
	

	if(!m_pCM || !m_pGrid || !m_pDM_EXP || !m_pDM_DB || !m_pDM_WORK || !m_pDM_ACQ)
	{
		AfxMessageBox(_T("CM,DM,Grid Error : 체크인_권단위"));
		return FALSE;
	}
	
	//그리드의 자동 setreverse 기능 제거
	m_pGrid->m_bFroceReverse = FALSE; 

	//그리드의 자동 sort 기능 제거
//	m_pGrid->SetSort(FALSE);
		

	//부서지정 사용 여부를 가져온다
	CSeApi api(this);
	m_bIsDepartMngUse = api.IsDepartMngUse(m_pDM_WORK);
	
	if(1 == m_nACQ_TYPE && m_bIsDepartMngUse)
	{
		m_pDM_DEPART = FindDM(_T("DM_체크인_부서지정자료"));
		if(!m_pDM_DEPART)
		{
			AfxMessageBox(_T("DM Error : DM_체크인_부서지정자료"));
		}
	}


	//표지정보를 사용하지 않는 경우는 타이머 설정을 통해 해당 버튼을 감춘다
	if(!api.IsCoverImageUse(m_pDM_WORK))
	{
		SetTimer(0,100,NULL);
	}
	

	//데이터 초기화 
	if(InitAllReference()<0)
	{
		return FALSE;
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CCheckinVolESL::FreeChild()
{
	if(m_pProgressDlg)	delete m_pProgressDlg;
	m_pProgressDlg = NULL;

}

VOID CCheckinVolESL::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridCHECKIN_VOL);
	if(pGrid->GetSafeHwnd() == NULL) return;
		
	/*
	CRect rect;
	GetClientRect(rect);
	rect.top = 25;
	rect.bottom -= 5;
	rect.left += 5;
	rect.right -= 5;
	
	pGrid->MoveWindow(rect);
	
	CWnd* pWnd = NULL;
	pWnd = GetDlgItem(IDC_tabCHECKIN_VOL);
	if(pWnd->GetSafeHwnd() == NULL) return;
	pWnd->MoveWindow(0, 0 , cx, cy);
	*/

	CRect rect;
	GetClientRect(rect);
	rect.top = 60;
	rect.bottom -= 5;
	rect.left += 5;
	rect.right -= 5;
	
	pGrid->MoveWindow(rect);
	
	CWnd* pWnd = NULL;
	pWnd = GetDlgItem(IDC_tabCHECKIN_VOL);
	if(pWnd->GetSafeHwnd() == NULL) return;
	pWnd->MoveWindow(0, 35 , cx+2, cy);
	pGrid->MoveWindow(-2, 55, cx+2, cy-55);


}

INT CCheckinVolESL::InitAllReference()
{
	
	//부모 그리드에서 선정정보를 얻어온다
	if( !m_pParentGrid )
	{
		AfxMessageBox(_T("부모 그리드를 찾을 수 없습니다!"));
		return -1;
	}

	if( !m_pDM_IDX )
	{
		AfxMessageBox(_T("부모 DM을 찾을 수 없습니다!"));
		return -2;
	}


	m_pParentGrid->SelectMakeList();
	//===================================================
	//2010.01.11 ADD BY PJW : 현재 index를 보여준다.
//	m_nCurrentParentIdx = m_pParentGrid->SelectGetHeadPosition();
//	if( m_nCurrentParentIdx < 0 )
//	{
//		m_nCurrentParentIdx = m_pParentGrid->GetIdx();
//		//AfxMessageBox(_T("선정된 정보가 없습니다!"));
//		//return -3;
//	}
	m_nCurrentParentIdx = m_pParentGrid->GetIdx();
	//===================================================
	

	//설정 파일에서 그리드에 칠할 정보를 가지고 온다 
	SetColorRefFromFile();

	RefreshAll();

	return 0;
}

INT CCheckinVolESL::SetColorRefFromFile()
{
	
	LPCTSTR lpszFileName = CFG_FILE_PATH_COLOR;
	CStdioFile file;
	INT nSuccess = file.Open(lpszFileName, CFile::modeRead | CFile::typeBinary);
	
	if(nSuccess)
	{
		TCHAR cUni;
		file.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			file.SeekToBegin();
		}
		
		CString strLine;
		CString tmp;
		CString strColor;
		INT nPos;
		INT nReadCnt = 0;
		
		while(file.ReadString(strLine))
		{
			
			strLine.TrimLeft();
			strLine.TrimRight();
			
			nPos = strLine.Find(_T(":"));

			if(nPos>0)
			{
				tmp = strLine.Left(nPos);
				tmp.TrimLeft();
				tmp.TrimRight();
				
				if(_T("예정") == tmp)
				{
					strColor = strLine.Mid(nPos+1);
					strColor.TrimLeft();
					strColor.TrimRight();
					if(!strColor.IsEmpty()) SetColorRefByString(CR_exp,strColor);
				}
				else if(_T("입수") == tmp)
				{
					strColor = strLine.Mid(nPos+1);
					strColor.TrimLeft();
					strColor.TrimRight();
					if(!strColor.IsEmpty()) SetColorRefByString(CR_acq,strColor);
				}
				else if(_T("미달") == tmp)
				{
					strColor = strLine.Mid(nPos+1);
					strColor.TrimLeft();
					strColor.TrimRight();
					if(!strColor.IsEmpty()) SetColorRefByString(CR_lack,strColor);
				}
				else if(_T("결호") == tmp)
				{
					strColor = strLine.Mid(nPos+1);
					strColor.TrimLeft();
					strColor.TrimRight();
					if(!strColor.IsEmpty()) SetColorRefByString(CR_miss,strColor);

				}
				else if(_T("지연") == tmp)
				{
					strColor = strLine.Mid(nPos+1);
					strColor.TrimLeft();
					strColor.TrimRight();
					if(!strColor.IsEmpty()) SetColorRefByString(CR_delay,strColor);

				}
				else if(_T("미발행") == tmp)
				{
					strColor = strLine.Mid(nPos+1);
					strColor.TrimLeft();
					strColor.TrimRight();
					if(!strColor.IsEmpty()) SetColorRefByString(CR_nonpub,strColor);
				}		
			}
		}

		file.Close();
	}


	return 0;
}

INT CCheckinVolESL::WriteColorRefOnFile()
{
	
	LPCTSTR lpszFileName = CFG_FILE_PATH_COLOR;
	CStdioFile file;
	INT nSuccess = file.Open(lpszFileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);
	
	if(nSuccess<0) return -1;
	
	if( 0 == file.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		file.Write( &cUni, sizeof(TCHAR));
	}
		
	CString strLine;

	strLine = GetStringByColorRef(_T("예정"), CR_exp);
	file.WriteString(strLine);

	strLine = GetStringByColorRef(_T("입수"), CR_acq);
	file.WriteString(strLine);

	strLine = GetStringByColorRef(_T("미달"), CR_lack);
	file.WriteString(strLine);
	
	strLine = GetStringByColorRef(_T("결호"), CR_miss);
	file.WriteString(strLine);
	
	strLine = GetStringByColorRef(_T("지연"), CR_delay);
	file.WriteString(strLine);
	
	strLine = GetStringByColorRef(_T("미발행"), CR_nonpub);
	file.WriteString(strLine);

	file.Close();
	
	return 0;
}

CString CCheckinVolESL::GetStringByColorRef(CString strTitle, COLORREF& colorref)
{
	CString strResult;
	
	INT red,green,blue;
	
	red = GetRValue(colorref);
	green = GetGValue(colorref);
	blue = GetBValue(colorref);
	
	strResult.Format(_T("%s : %d/%d/%d\r\n"),strTitle,red,green,blue);

	return strResult;
}

VOID CCheckinVolESL::SetColorRefByString(COLORREF& colorref, CString strColor)
{
	INT red,green,blue;
	red = green = blue = 255;
	ExtractStringColorToRGB(strColor,&red,&green,&blue);
	colorref = RGB(red,green,blue);
}

VOID CCheckinVolESL::ExtractStringColorToRGB(CString strColor, INT *red, INT *green, INT *blue)
{
	if(strColor.GetLength() == 0) return ;
	INT idx, startidx, lastidx;
	idx = startidx = 0;
	lastidx = strColor.GetLength();
	idx = strColor.Find(_T("/"), startidx);
	if(idx > 0)
		*red = _ttoi((strColor.Left(idx)).GetBuffer(0));
	startidx = idx + 1;
	idx = strColor.Find(_T("/"), startidx);
	if(idx > 0)
		*green = _ttoi((strColor.Mid(startidx, idx - 1)).GetBuffer(0));
	startidx = idx + 1;
	*blue = _ttoi((strColor.Right(lastidx - startidx)).GetBuffer(0));
	return ;
}

INT CCheckinVolESL::RefreshAll(INT nMoreExp)
{
	//기본 추가 예측 기능
	if( 0 == nMoreExp )
	{
		nMoreExp = GetDefaultMoreExpCnt();
	}
	
	m_strSPECIES_KEY = m_pDM_IDX->GetCellData(_T("ISS_종KEY"),m_nCurrentParentIdx);
	m_strPUB_FREQ = m_pDM_IDX->GetCellData(_T("ISS_간행빈도"),m_nCurrentParentIdx);
	
	if( m_strPUB_FREQ.IsEmpty() )
	{
		AfxMessageBox(_T("해당 종의 간행빈도가 없어 예측정보를 구성할 수 없습니다!"));
		return -1;
	}

	if( m_strSPECIES_KEY.IsEmpty() )
	{
		AfxMessageBox(_T("해당 종KEY를 찾을 수 없습니다!"));
		return -2;
	}
	
	m_pDM_EXP->FreeData(); 
	m_pDM_DB->FreeData(); 
	m_pDM_ACQ->FreeData();


	CString strWhere;
	strWhere.Format(_T("SPECIES_KEY = %s"),m_strSPECIES_KEY);
	m_pDM_ACQ->RefreshDataManager(strWhere);
	if( 0 == m_pDM_ACQ->GetRowCount() )
	{
		AfxMessageBox(_T("수입구분정보를 가져올 수 없습니다"));
		return -3;
	}
	
	CString strACQ_KEY = _T("");
	if(1 == m_nACQ_TYPE) strACQ_KEY = m_pDM_ACQ->GetCellData(_T("SP_구입관리KEY"),0);
	else strACQ_KEY = m_pDM_ACQ->GetCellData(_T("SD_기증관리KEY"),0);
	
	
	m_pProgressDlg->SetProGressRange(100);
	//m_pProgressDlg->SetMessage(_T("예측정보를 구성하는 중..."));
	m_pProgressDlg->BringWindowToTop();
	m_pProgressDlg->ShowWindow(SW_SHOW);

	CProgressCtrl* pProgress = m_pProgressDlg->GetProgressCtrl();


	//예측정보를 구성한다 
	//종의 간행상태가 폐간, 혹은 휴간이면 예측 하지 않는다 
	m_arrayVolExpKeyList.RemoveAll();
	m_arrayOtherVolExpKeyList.RemoveAll();
	
	CString strPUB_STATUS;
	strPUB_STATUS = m_pDM_IDX->GetCellData(_T("ISS_간행상태"),m_nCurrentParentIdx);

	if(_T("d") != strPUB_STATUS && _T("s") !=  strPUB_STATUS)
	{	

		CSeExpectMgr mgr(this);
		mgr.InitMgr(m_pDM_EXP,
					m_pDM_ACQ, 
					m_strPUB_FREQ,
					m_nACQ_TYPE,
					m_arrayVolExpKeyList,
					m_arrayOtherVolExpKeyList,
					nMoreExp,
					pProgress);
	}
	else
	{
		if(_T("d") == strPUB_STATUS)
		{
			AfxMessageBox(_T("현 자료는 폐간되었으므로 예측정보가 구성되지 않습니다!"));
		}
		else if(_T("s") == strPUB_STATUS)
		{
			AfxMessageBox(_T("현 자료는 휴간중이므로 예측정보가 구성되지 않습니다!"));
		}

	}

	/*
	//DB에서 해당정보를 가져온다.
	strWhere.Format(_T("A.VOL_KEY = V.REC_KEY AND V.VOL_TYPE = 'V' AND A.ACQ_KEY = %s"),strACQ_KEY); 
	m_pDM_DB->RefreshDataManager(strWhere);
	*/

	//modified by loveisverb
	//2004-12-10
	//발행년도를 기준으로 제한검색
	UpdateData(TRUE);
	CString strPUBLISH_DATE;
	m_pCM->GetControlMgrData(_T("발행년도"),strPUBLISH_DATE);

	if(m_bIsAllDataShow || strPUBLISH_DATE.IsEmpty())
	{
		strWhere.Format(_T("A.VOL_KEY = V.REC_KEY AND V.VOL_TYPE = 'V' AND A.ACQ_KEY = %s"),strACQ_KEY); 
		m_pDM_DB->RefreshDataManager(strWhere);
	}
	else
	{
		
		CString strStartDate;
		CString strEndDate;
		strStartDate.Format(_T("%s/01/01"),strPUBLISH_DATE);
		strEndDate.Format(_T("%s/12/31"),strPUBLISH_DATE);
	
		strWhere.Format(_T("A.VOL_KEY = V.REC_KEY AND V.VOL_TYPE = 'V' AND A.ACQ_KEY = %s")
						_T(" AND V.PUBLISH_DATE BETWEEN '%s' AND '%s'"),
						strACQ_KEY,strStartDate,strEndDate); 
		m_pDM_DB->RefreshDataManager(strWhere);
	}
	
	m_pProgressDlg->SetProGressPos(100);
	m_pProgressDlg->ShowWindow(SW_HIDE);


	//현재 정보를 조합해서 보여준다
	MakeWorkDM();
	
	//부모 그리드 반전
	m_pParentGrid->SetReverse(m_nCurrentParentIdx);
	
	//종정보 보여주기 
	if(m_pResourceMgr) m_pResourceMgr->m_dlgSpeciesInfo.ShowSpeciesInfo(m_nCurrentParentIdx);
	
	//변수 초기화 
	m_bIsNeedReceiptNo = FALSE;

	//부서지정정보
	if(1 == m_nACQ_TYPE && m_bIsDepartMngUse)
	{
		strWhere.Format(_T("PURCHASE_MANAGE_KEY = %s"),strACQ_KEY);
		m_pDM_DEPART->RefreshDataManager(strWhere);
		
	}

	return 0;
}
	
INT CCheckinVolESL::CopyExpToWork()
{
	INT nRow_Exp = m_pDM_EXP->GetRowCount();
	INT nRow_Work;
	CString strVoltitle;
	CSeApi api(this);
	CString strBeforeRow;

	for(INT i=0;i<nRow_Exp;i++)
	{
		m_pDM_WORK->InsertRow(-1);
		nRow_Work = m_pDM_WORK->GetRowCount()-1;
		CopyDMToDMByAlias(m_pDM_EXP,i,m_pDM_WORK,nRow_Work);
		

		//예측권호를 권호명에 보여준다
		strVoltitle = _T("");
		strVoltitle = m_pDM_EXP->GetCellData(_T("SA_입수예측권호명"),i);
		m_pDM_WORK->SetCellData(_T("SV_권호명"),strVoltitle,nRow_Work);
		m_pDM_WORK->SetCellData(_T("UDF_입수상태"),_T("예정"),nRow_Work);
		m_pDM_WORK->SetCellData(_T("UDF_상태"),_T("E"),nRow_Work);
		m_pDM_WORK->SetCellData(_T("SV_권호구분"),_T("1"),nRow_Work);
		m_pDM_WORK->SetCellData(_T("UDF_권호구분"),_T("일반"),nRow_Work);
	
		strBeforeRow.Format(_T("%d"),i);
		m_pDM_WORK->SetCellData(_T("UDF_행"),strBeforeRow,nRow_Work);

		//종정보, 수입구분정보를 토대로 권정보와 수입구분별 권 정보를 만들어 낸다.
		api.MAKE_EXP_WORK_by_IDX_ACQ(m_pDM_IDX,m_nCurrentParentIdx,m_pDM_WORK,nRow_Work,m_pDM_ACQ,m_nACQ_TYPE);
		

	}
	
	return 0;
}

INT CCheckinVolESL::CopyMissToWork()
{
	INT nRow_DB = m_pDM_DB->GetRowCount();
	INT nRow_Work;
	CString strCHECKIN_STATUS;
	CString strVOLUME_CLASS;
	CString strBeforeRow;
	CSeApi api(this);

	CString strLackBookCnt;
	INT nLackBook = 0;

	for(INT i=0;i<nRow_DB;i++)
	{
		strCHECKIN_STATUS = _T("");
		strCHECKIN_STATUS = m_pDM_DB->GetCellData(_T("SA_입수상태"),nRow_DB-i-1);
		
		if(_T("1") == strCHECKIN_STATUS) continue;
			
		m_pDM_WORK->InsertRow(-1);
		nRow_Work = m_pDM_WORK->GetRowCount()-1;
		CopyDMToDMByAlias(m_pDM_DB,nRow_DB-i-1,m_pDM_WORK,nRow_Work);

		if(_T("2") == strCHECKIN_STATUS)
			m_pDM_WORK->SetCellData(_T("UDF_입수상태"),_T("결호"),nRow_Work);
		else if(_T("3") == strCHECKIN_STATUS)
			m_pDM_WORK->SetCellData(_T("UDF_입수상태"),_T("미달"),nRow_Work);
		

		strVOLUME_CLASS = _T("");
		strVOLUME_CLASS = m_pDM_WORK->GetCellData(_T("SV_권호구분"),nRow_Work);
		

		if(_T("1") == strVOLUME_CLASS)
			m_pDM_WORK->SetCellData(_T("UDF_권호구분"),_T("일반"),nRow_Work);
		else if(_T("2") == strVOLUME_CLASS)
			m_pDM_WORK->SetCellData(_T("UDF_권호구분"),_T("합병호"),nRow_Work);
		else if(_T("3") == strVOLUME_CLASS)
			m_pDM_WORK->SetCellData(_T("UDF_권호구분"),_T("미발행"),nRow_Work);


		m_pDM_WORK->SetCellData(_T("UDF_상태"),_T("D"),nRow_Work); //D: DB
		
		strBeforeRow.Format(_T("%d"),i);
		m_pDM_WORK->SetCellData(_T("UDF_행"),strBeforeRow,nRow_Work);
		
		//종정보, 수입구분정보를 추가한다
		api.MAKE_DB_WORK_by_IDX_ACQ(m_pDM_IDX,m_nCurrentParentIdx,m_pDM_WORK,nRow_Work,m_pDM_ACQ,m_nACQ_TYPE);
		

		//지연 처리
		nLackBook = 0;
		strLackBookCnt = m_pDM_WORK->GetCellData(_T("UDF_지연수"),nRow_Work);
		if(!strLackBookCnt.IsEmpty()) nLackBook = _ttoi(strLackBookCnt.GetBuffer(0));
		
		if(nLackBook>0){
			m_pDM_WORK->SetCellData(_T("UDF_입수상태"),_T("지연"),nRow_Work);
			//SetGridRowUniqueColor(INT nRowIdx)
		}
		

		
	}

	return 0;
}

INT CCheckinVolESL::CopyAcqToWork()
{
	INT nRow_DB = m_pDM_DB->GetRowCount();
	INT nRow_Work;
	CString strCHECKIN_STATUS;
	CString strVOLUME_CLASS;
	CString strBeforeRow;
	CSeApi api(this);

	for(INT i=0;i<nRow_DB;i++)
	{
		
		strCHECKIN_STATUS = _T("");
		strCHECKIN_STATUS = m_pDM_DB->GetCellData(_T("SA_입수상태"),nRow_DB-i-1);
		
		if(_T("2") == strCHECKIN_STATUS) continue;
		
		m_pDM_WORK->InsertRow(-1);
		nRow_Work = m_pDM_WORK->GetRowCount()-1;
		CopyDMToDMByAlias(m_pDM_DB,nRow_DB-i-1,m_pDM_WORK,nRow_Work);

		if(_T("1") == strCHECKIN_STATUS)
			m_pDM_WORK->SetCellData(_T("UDF_입수상태"),_T("입수"),nRow_Work);
		else if(_T("3") == strCHECKIN_STATUS)
			m_pDM_WORK->SetCellData(_T("UDF_입수상태"),_T("미달"),nRow_Work);

		
		strVOLUME_CLASS = _T("");
		strVOLUME_CLASS = m_pDM_WORK->GetCellData(_T("SV_권호구분"),nRow_Work);
		

		if(_T("1") == strVOLUME_CLASS)
			m_pDM_WORK->SetCellData(_T("UDF_권호구분"),_T("일반"),nRow_Work);
		else if(_T("2") == strVOLUME_CLASS)
			m_pDM_WORK->SetCellData(_T("UDF_권호구분"),_T("합병호"),nRow_Work);
		else if(_T("3") == strVOLUME_CLASS)
			m_pDM_WORK->SetCellData(_T("UDF_권호구분"),_T("미발행"),nRow_Work);
	

		m_pDM_WORK->SetCellData(_T("UDF_상태"),_T("D"),nRow_Work); //D: DB

		strBeforeRow.Format(_T("%d"),i);
		m_pDM_WORK->SetCellData(_T("UDF_행"),strBeforeRow,nRow_Work);

		//종정보, 수입구분정보를 추가한다
		api.MAKE_DB_WORK_by_IDX_ACQ(m_pDM_IDX,m_nCurrentParentIdx,m_pDM_WORK,nRow_Work,m_pDM_ACQ,m_nACQ_TYPE);
		
	}

	return 0;
}

INT CCheckinVolESL::CopyAllDBToWork()
{
	INT nRow_DB = m_pDM_DB->GetRowCount();
	INT nRow_Work;
	CString strCHECKIN_STATUS;
	CString strBeforeRow;
	CSeApi api(this);
	CString strVOLUME_CLASS;
	
	CString strLackBookCnt;
	INT nLackBook = 0;

	for(INT i=0;i<nRow_DB;i++)
	{
		m_pDM_WORK->InsertRow(-1);
		nRow_Work = m_pDM_WORK->GetRowCount()-1;
		
		strCHECKIN_STATUS = _T("");
		strCHECKIN_STATUS = m_pDM_DB->GetCellData(_T("SA_입수상태"),nRow_DB-i-1);
		CopyDMToDMByAlias(m_pDM_DB,nRow_DB-i-1,m_pDM_WORK,nRow_Work);
		
		if(_T("1") == strCHECKIN_STATUS)
			m_pDM_WORK->SetCellData(_T("UDF_입수상태"),_T("입수"),nRow_Work);
		else if(_T("2") == strCHECKIN_STATUS)
			m_pDM_WORK->SetCellData(_T("UDF_입수상태"),_T("결호"),nRow_Work);
		else if(_T("3") == strCHECKIN_STATUS)
			m_pDM_WORK->SetCellData(_T("UDF_입수상태"),_T("미달"),nRow_Work);

		
		strVOLUME_CLASS = _T("");
		strVOLUME_CLASS = m_pDM_WORK->GetCellData(_T("SV_권호구분"),nRow_Work);
		

		if(_T("1") == strVOLUME_CLASS)
			m_pDM_WORK->SetCellData(_T("UDF_권호구분"),_T("일반"),nRow_Work);
		else if(_T("2") == strVOLUME_CLASS)
			m_pDM_WORK->SetCellData(_T("UDF_권호구분"),_T("합병호"),nRow_Work);
		else if(_T("3") == strVOLUME_CLASS)
			m_pDM_WORK->SetCellData(_T("UDF_권호구분"),_T("미발행"),nRow_Work);



		m_pDM_WORK->SetCellData(_T("UDF_상태"),_T("D"),nRow_Work); //D: DB
	
		strBeforeRow.Format(_T("%d"),i);
		m_pDM_WORK->SetCellData(_T("UDF_행"),strBeforeRow,nRow_Work);

		//종정보, 수입구분정보를 추가한다
		api.MAKE_DB_WORK_by_IDX_ACQ(m_pDM_IDX,m_nCurrentParentIdx,m_pDM_WORK,nRow_Work,m_pDM_ACQ,m_nACQ_TYPE);
	

		if(_T("2") == strCHECKIN_STATUS) //결호이면 
		{
			//지연 처리
			nLackBook = 0;
			strLackBookCnt = m_pDM_WORK->GetCellData(_T("UDF_지연수"),nRow_Work);
			if(!strLackBookCnt.IsEmpty()) nLackBook = _ttoi(strLackBookCnt.GetBuffer(0));
			
			if(nLackBook>0){
				m_pDM_WORK->SetCellData(_T("UDF_입수상태"),_T("지연"),nRow_Work);
			}
		}

	}

	return 0;
}

	
INT CCheckinVolESL::CopyDMToDMByAlias(CESL_DataMgr *pDM_SOURCE, INT SourceRowIdx, CESL_DataMgr *pDM_TARGET, INT TargetRowIdx)
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

VOID CCheckinVolESL::OnSelchangetabCHECKINVOL(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	MakeWorkDM();
	
	//test
	//m_nCurrentIdx = -1;

	*pResult = 0;
}

VOID CCheckinVolESL::MakeWorkDM()
{
	//작업 DM의 data를 삭제한다 
	m_pDM_WORK->FreeData(); 
	
	//test
	//DB DM을 권호일련번호에 따라 sort한다
	m_pDM_DB->SORT(_T("SV_권호일련번호"));

	//텝 상태에 따라 각 정보를 적절히 조합한다
	INT nCurSel = m_tabCTRL.GetCurSel();
	
	switch(nCurSel)
	{
	case 0:
		CopyExpToWork();
		CopyMissToWork();	
		break;
	case 1:
		CopyAcqToWork();
		break;
	case 2:
		CopyAllDBToWork();
		break;
	case 3:
		CopyMissToWork();
		break;
	}
	
	m_pGrid->Display();
	
	//color setting 
	SetAllGridColor();

}


VOID CCheckinVolESL::SetAllGridColor()
{
	//color setting 

	CString strUDF;
	CString strCHECKIN_STATUS;
	CString strLackBookCnt;
	CString strVOLUME_CLASS;
	INT nLackBook = 0;
	
	

	m_pGrid->ShowWindow(FALSE);
	for(INT i=0;i<m_pDM_WORK->GetRowCount();i++)
	{
		strUDF = _T("");
		strUDF = m_pDM_WORK->GetCellData(_T("UDF_상태"),i); 
		
		if(_T("E") == strUDF) //예정자료
		{
			SetGridRowUniqueColor(CR_exp,i);
		}
		else if(_T("D") ==strUDF) //DB 자료
		{
			strCHECKIN_STATUS = _T("");
			strCHECKIN_STATUS = m_pDM_WORK->GetCellData(_T("SA_입수상태"),i); 
			
			if(_T("1") == strCHECKIN_STATUS) //입수
			{
				strVOLUME_CLASS = _T("");
				strVOLUME_CLASS = m_pDM_WORK->GetCellData(_T("SV_권호구분"),i); 
				
				if(_T("3") == strVOLUME_CLASS)
					SetGridRowUniqueColor(CR_nonpub,i); //미발행 
				else
					SetGridRowUniqueColor(CR_acq,i); //일반,합병호

			}
			else if(_T("2") == strCHECKIN_STATUS) //결호 
			{
				nLackBook = 0;		
				strLackBookCnt = m_pDM_WORK->GetCellData(_T("UDF_지연수"),i);
				if(!strLackBookCnt.IsEmpty()) nLackBook = _ttoi(strLackBookCnt.GetBuffer(0));
					
				if(nLackBook>0){
					SetGridRowUniqueColor(CR_delay,i); //지연
				}
				else
				{
					SetGridRowUniqueColor(CR_miss,i); //결호 
				}
				
			}
			else if(_T("3") == strCHECKIN_STATUS) //미달 
			{
				
				SetGridRowUniqueColor(CR_lack,i);

			}
		}

	}
	m_pGrid->ShowWindow(TRUE);


}

INT CCheckinVolESL::CMD_CHECKIN()
{
	//----------------------------------------------------------------------
	// 2005-10-11 입수처리 버튼을 누르면 저장될 수 있도록 수정
	//----------------------------------------------------------------------
	//결호자료와 예정자료에 대해서만 입수처리 하는 함수이다.
	if( m_pGrid->SelectMakeList() < 0 )
		return 0;
	if( m_pGrid->SelectGetCount() < 1 )
	{ 
		AfxMessageBox(_T("선정된 정보가 없습니다."));
		return 0;
	}
	INT nIdx = m_pGrid->SelectGetHeadPosition();
	CString strUDF;
	CString strStatus;

	while(nIdx>=0)
	{
		strUDF = _T("");
		strUDF = m_pDM_WORK->GetCellData(_T("UDF_상태"),nIdx);
		strStatus = m_pDM_WORK->GetCellData(_T("UDF_입수상태"),nIdx);
		if(_T("E") == strUDF || _T("M") == strUDF) CheckinForUser(nIdx);
		if(_T("D") == strUDF || _T("U") == strUDF) 
		{
			if( strStatus != _T("미달") )
				CheckinDBForUser(nIdx);
			m_bIsNeedReceiptNo = TRUE;
		}
	
		nIdx = m_pGrid->SelectGetNext();
	}

	CMD_SAVE();

	return 0;
}

INT CCheckinVolESL::CMD_MISS()
{
	//----------------------------------------------------------------------
	// 2005-10-11 결호처리 버튼을 누르면 저장될 수 있도록 수정
	//----------------------------------------------------------------------
	//입수자료와 예정자료에 대해서만 결호처리 하는 함수이다.
	if( m_pGrid->SelectMakeList() < 0 )
		return 0;
	if( m_pGrid->SelectGetCount() < 1 )
	{ 
		AfxMessageBox(_T("선정된 정보가 없습니다."));
		return 0;
	}
	INT nIdx = m_pGrid->SelectGetHeadPosition();
	CString strUDF;

	while(nIdx>=0)
	{
		strUDF = _T("");
		strUDF = m_pDM_WORK->GetCellData(_T("UDF_상태"),nIdx);
		if(_T("E") == strUDF || _T("C") == strUDF) MissbookForUser(nIdx); 
		if(_T("D") == strUDF || _T("U") == strUDF) MissbookDBForUser(nIdx);
		
		nIdx = m_pGrid->SelectGetNext();
	}
	CMD_SAVE();


	return 0;
}

INT CCheckinVolESL::CMD_SAVE()
{
	//현재 그리드의 모든 정보를 처리하는 함수 
	//DB 정보의 경우에는 수정 되었을 때만 처리하고 
	//예측정보를 처리할 때에는 자동 결호처리를 고려한다. 
	//작업 처리후 데이터를 해당 DM으로 보낸다 
	
	//수입년도와 접수번호가 입력되어 있는지를 확인한다.
	m_strRECEIPT_NO.TrimLeft();
	m_strRECEIPT_NO.TrimRight();
	if(m_strRECEIPT_NO.IsEmpty())
	{
		//modified by loveisverb 2004-09-15
		//접수번호 자동 부여 방식으로 
		if(IsNeedReceipNo())
		{
			CString strMsg;
			strMsg.Format(_T("접수번호가 입력되지 않았습니다!\n새 접수번호로 저장하시겠습니까?"));
			if(AfxMessageBox(strMsg,MB_YESNO) == IDYES)
			{
				if(GetNewReceiptNo()<0)
				{
					AfxMessageBox(_T("접수번호 자동 부여에 실패 하였습니다!"));
					return 0;
				}
			}
			else
			{
				//변경된 그리드를 복구한다. Modified by hjy.
				RefreshAll();
				return 0;
			}
		}

	}
	

	CESL_DataMgr* pDM_VOL		  = FindDM(_T("DM_체크인_권"));
	CESL_DataMgr* pDM_ACQ_VOL	  = FindDM(_T("DM_체크인_수입구분별권"));
	CESL_DataMgr* pDM_BOOK		  = FindDM(_T("DM_체크인_책"));
	CESL_DataMgr* pDM_BOOK_INSERT = FindDM(_T("DM_체크인_책_입력"));
	CESL_DataMgr* pDM_APPENDIX	  = FindDM(_T("DM_체크인_부록"));

	CSeApi api(this);
	
	CString strUDF;
	CString msg;
	INT nRow = m_pDM_WORK->GetRowCount();
	INT ids;

	m_pGrid->UpdateWindow();
	
	INT nProcCnt = 0;
	CString strUDFforVerify;
	//상태를 검사한다.
	BOOL bIsAlert = FALSE;
	for(INT i=0;i<nRow;i++)
	{
		strUDF = _T("");
		strUDF = m_pDM_WORK->GetCellData(_T("UDF_상태"),nRow-i-1);	
		
		if(_T("E") == strUDF) bIsAlert = TRUE;
		if(_T("C") == strUDF || _T("M") == strUDF || _T("X") == strUDF)
		{
			
			nProcCnt++;
			if(bIsAlert) { //처리되지 않은 기존 예측 정보 존재 
				
				if(CheckUnprocessedData()<0) return 0;
			}
		}
		
		if(_T("U") == strUDF || _T("Z") == strUDF) nProcCnt++;
			
	}
	

	if(nProcCnt>0) {
		m_pProgressDlg->SetProGressRange(nProcCnt);
		m_pProgressDlg->ShowWindow(SW_SHOW);
		m_pProgressDlg->BringWindowToTop();
		m_pProgressDlg->CenterWindow();
		m_pProgressDlg->SetProGressPos(0);
	}
	
	CString strProgress;
	INT nProcCount = 1;

	for(i=0;i<nRow;i++)
	{
		strUDF = _T("");
		strUDF = m_pDM_WORK->GetCellData(_T("UDF_상태"),nRow-i-1);	
		if(_T("E") == strUDF) break;
		if(_T("D") == strUDF) continue;
		
		strProgress.Format(_T("[ %d ] 건의 자료를 처리하는 중입니다"),nProcCount);
		m_pProgressDlg->SetMessage(strProgress);
		nProcCount++;

		if(_T("C") == strUDF || _T("M") == strUDF) //insert
		{

			ids = api.CHECK_IN(m_pDM_IDX,
							   m_nCurrentParentIdx,
							   m_pDM_WORK,nRow-i-1,
						       m_pDM_ACQ,
						       m_nACQ_TYPE,
						       pDM_VOL,
						       pDM_ACQ_VOL,
						       pDM_BOOK,
						       m_arrayVolExpKeyList,
						       m_arrayOtherVolExpKeyList,
						       pDM_BOOK_INSERT,
						       pDM_APPENDIX,
							   m_strACQ_YEAR,
							   m_strRECEIPT_NO,
							   m_bIsDepartMngUse,
							   m_pDM_DEPART);
		
		}
		else if(_T("U") == strUDF) //update
		{
			ids = api.VOL_UPDATE(m_pDM_IDX,
							     m_nCurrentParentIdx,
							     m_pDM_WORK,nRow-i-1,
							     m_pDM_ACQ,
						         m_nACQ_TYPE,
						         pDM_VOL,
						         pDM_ACQ_VOL,
						         pDM_BOOK,
								 pDM_BOOK_INSERT,
								 m_strACQ_YEAR,
								 m_strRECEIPT_NO,
								 m_bIsLightVersion,
								 m_bIsDepartMngUse,
								 m_pDM_DEPART);
		}
		else if(_T("X") == strUDF) //Exp delete
		{
			
			ids = api.VOL_DELETE_EXP(m_pDM_WORK,
									 nRow-i-1,
									 m_pDM_ACQ,
									 m_nACQ_TYPE,
									 m_arrayVolExpKeyList,
									 m_arrayOtherVolExpKeyList);

		}
		else if(_T("Z") == strUDF) //DB delete 
		{

			ids = api.VOL_DELETE(m_pDM_WORK,
								 nRow-i-1,
								 m_nACQ_TYPE);

		}


		m_pProgressDlg->SetProGressPos(i+1);
		
		
		if(ids<0) //에러처리 
		{
			
			if(_T("Z") == strUDF)
			{
				if(1002 == ids) AfxMessageBox(_T("해당 권호에 삭제할 수 없는 책이 있습니다!\n 확인해 주십시오."));

			}


			m_pProgressDlg->ShowWindow(SW_HIDE);
			msg.Format(_T("[ %d ] 번째 행 작업에서 문제가 생겨 작업을 중단합니다"),nRow-i);
			AfxMessageBox(msg);
			return -1;

		}
		else //삭제 후 정보 상태 전이 처리 
		{
			if(_T("C") == strUDF || _T("M") == strUDF) //예측정보는 EXP --> DB로 MOVE
			{
				
				CString strUDFRow = m_pDM_WORK->GetCellData(_T("UDF_행"),nRow-i-1);
				INT nUDFRow = _ttoi(strUDFRow.GetBuffer(0));
				if(nUDFRow>=0) 
				{
					INT nBeforeRow = m_pDM_EXP->GetRowCount()-1;
					
					//삭제 가능 데이터인지 검증
					CString strACQ_EXP_DATE = _T("");
					strACQ_EXP_DATE = m_pDM_EXP->GetCellData(_T("SA_입수예정일"),nBeforeRow);
					if(!strACQ_EXP_DATE.IsEmpty()) m_pDM_EXP->DeleteRow(nBeforeRow);
				}


				INT nDBrow;
				m_pDM_DB->InsertRow(-1);
				nDBrow = m_pDM_DB->GetRowCount()-1;
				CopyDMToDMByAlias(m_pDM_WORK,nRow-i-1,m_pDM_DB,nDBrow);
				
			}
			else if(_T("U") == strUDF) //update는 refresh(copy)
			{
				
				CString strACQ_VOL_KEY_WORK;
				CString strACQ_VOL_KEY_DB;
				CString strWORKrow;
				strWORKrow = m_pDM_WORK->GetCellData(_T("UDF_행"),nRow-i-1);
				strACQ_VOL_KEY_WORK = m_pDM_WORK->GetCellData(_T("SA_수입구분별권KEY"),nRow-i-1);
				

				//일단은 검색 방식으로 
				for(INT j=0;j<m_pDM_DB->GetRowCount();j++)
				{
					strACQ_VOL_KEY_DB = _T("");
					strACQ_VOL_KEY_DB = m_pDM_DB->GetCellData(_T("SA_수입구분별권KEY"),j);
					
					if(strACQ_VOL_KEY_WORK == strACQ_VOL_KEY_DB)
						CopyDMToDMByAlias(m_pDM_WORK,nRow-i-1,m_pDM_DB,j);

				}

			}
			else if(_T("X") == strUDF) //Exp delete Exp DM을 삭제 
			{
				
				//기존의 예측 정보를 찾아서 삭제(테스트) 
				CString strBeforeRow;
				INT nBeforeRow;
				strBeforeRow = m_pDM_WORK->GetCellData(_T("UDF_행"),nRow-i-1);
				nBeforeRow = _ttoi(strBeforeRow.GetBuffer(0));

				if(nBeforeRow>=0) 
				{
					//삭제 가능 데이터인지 검증
					CString strACQ_EXP_DATE = _T("");
					strACQ_EXP_DATE = m_pDM_EXP->GetCellData(_T("SA_입수예정일"),nBeforeRow);
					if(!strACQ_EXP_DATE.IsEmpty()) m_pDM_EXP->DeleteRow(nBeforeRow);
				}
			
			}
			else if(_T("Z") == strUDF) //DB delete DB DM을 삭제 
			{
				CString strACQ_VOL_KEY_WORK;
				CString strACQ_VOL_KEY_DB;
				CString strWORKrow;
				strWORKrow = m_pDM_WORK->GetCellData(_T("UDF_행"),nRow-i-1);
				strACQ_VOL_KEY_WORK = m_pDM_WORK->GetCellData(_T("SA_수입구분별권KEY"),nRow-i-1);
				

				//일단은 검색 방식으로 
				for(INT j=0;j<m_pDM_DB->GetRowCount();j++)
				{
					strACQ_VOL_KEY_DB = _T("");
					strACQ_VOL_KEY_DB = m_pDM_DB->GetCellData(_T("SA_수입구분별권KEY"),j);
					
					if(strACQ_VOL_KEY_WORK == strACQ_VOL_KEY_DB) m_pDM_DB->DeleteRow(j);
				}

			}

		}

	
	}	
	
	m_pProgressDlg->ShowWindow(SW_HIDE);
	AfxMessageBox(_T("저장하였습니다!"));
	m_nCurrentIdx = -1;
	MakeWorkDM();

	return 0;
}

INT CCheckinVolESL::CMD_PRINT()
{

	INT nRowCnt = m_pDM_WORK->GetRowCount();
	if(0 == nRowCnt)
	{
		AfxMessageBox(_T("출력할 자료가 없습니다!"));
		return 0;
	}

	//print
	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	

	INT ids;
	pSIReportManager->SetCONNECTION_INFO(m_pDM_WORK->CONNECTION_INFO);
	ids = pSIReportManager->GetSIReportFileFromDB(_T("연속체크인권별목록"));
	
	if (ids < 0) {
		ESLAfxMessageBox(_T("DB로 부터 SI Report 정보를 가져오지 못하였습니다."));
		return 0;
	}
	
	pSIReportManager->m_pWorkForm->paper = PAPER_A4_LANDSCAPE;
	pSIReportManager->SetDataMgr(0, m_pDM_WORK, 0);
	
	
	CString strtitle = _T("");
	strtitle = m_pDM_IDX->GetCellData(_T("ISS_표제사항"),m_nCurrentParentIdx);
	CString strPublisher = _T("");
	strPublisher = m_pDM_IDX->GetCellData(_T("ISS_발행자사항"),m_nCurrentParentIdx);

	
	pSIReportManager->SetTempVariable(_T("서명"),strtitle);
	pSIReportManager->SetTempVariable(_T("발행자"),strPublisher);
	
	INT nCurSel = m_tabCTRL.GetCurSel();
	CString strTabName = _T("");
	
	switch(nCurSel)
	{
	case 0:
		strTabName = _T("예정/결호자료목록 ");
		break;
	case 1:
		strTabName = _T("입수자료목록");
		break;
	case 2:
		strTabName = _T("입수/결호자료목록");
		break;
	case 3:
		strTabName = _T("결호자료목록");
		break;
	default:
		break;
	}

	
	pSIReportManager->SetTempVariable(_T("탭이름"),strTabName);


	ids = pSIReportManager->Print();

	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}

	return 0;
}

INT	CCheckinVolESL::CMD_VOL_INSERT()
{

	INT nCurSel = m_tabCTRL.GetCurSel();
	if(0 != nCurSel)
	{
		AfxMessageBox(_T("권 입력은 [예정/결호] 자료 에서만 가능합니다!"));
		return 0;	
	}

	//종정보, 수입구분정보를 토대로 권정보와 수입구분별 권 정보를 만들어 낸다.
	m_pDM_WORK->InsertRow(-1);
	INT nRow_Work = m_pDM_WORK->GetRowCount()-1;
	
	CSeApi api(this);
	api.MAKE_EXP_WORK_by_IDX_ACQ(m_pDM_IDX,m_nCurrentParentIdx,m_pDM_WORK,nRow_Work,m_pDM_ACQ,m_nACQ_TYPE);
	m_pDM_WORK->SetCellData(_T("UDF_입수상태"),_T("예정"),nRow_Work);
	m_pDM_WORK->SetCellData(_T("UDF_상태"),_T("E"),nRow_Work);
	m_pDM_WORK->SetCellData(_T("UDF_행"),_T("-1"),nRow_Work); 
	m_pDM_WORK->SetCellData(_T("UDF_권호구분"),_T("일반"),nRow_Work);
	m_pDM_WORK->SetCellData(_T("SV_권호구분"),_T("1"),nRow_Work); //일반

	CSeVolDlg dlg(this);
	dlg.m_pParentGrid = m_pGrid;
	dlg.m_nOpenMode = 1; //입력
	dlg.m_nACQ_TYPE = m_nACQ_TYPE;
	dlg.m_bIsLightVersion = m_bIsLightVersion;

	if(dlg.DoModal() == IDOK) 
	{
	//----------------------------------------------------------------------
	// 2005-10-11 권호입력 버튼을 누르면 저장될 수 있도록 수정
	//----------------------------------------------------------------------
/*		AfxMessageBox(_T("입력된 권호는 자동 입수처리 저장 됩니다."));

		CString strUDF = _T("");
		strUDF = m_pDM_WORK->GetCellData(_T("UDF_상태"),nRow_Work);
		if(_T("E") == strUDF || _T("M") == strUDF) CheckinForUser(nRow_Work);
		if(_T("D") == strUDF || _T("U") == strUDF) 
		{
			CheckinDBForUser(nRow_Work);
			m_bIsNeedReceiptNo = TRUE;
		}
		CMD_SAVE();*/
	}
	else
	{
		m_pDM_WORK->DeleteRow(nRow_Work);
	}

	return 0;	
}

INT	CCheckinVolESL::CMD_VOL_MODIFY()
{
	INT nCurSel = m_tabCTRL.GetCurSel();
/*	if( 0 == nCurSel || 3 == nCurSel )
	{
		AfxMessageBox(_T("권수정은 [입수] 또는 [입수/결호] 자료 에서만 가능합니다!"));
		return 0;	
	}
*/
	if(m_nCurrentIdx<0)
	{
		AfxMessageBox(_T("선정된 정보가 없습니다!"));
		return 0;
	}

	if(m_nCurrentIdx>=0)
	{
		SetGridRowBoldFont(m_nCurrentIdx, FALSE);
	}
	
	CString strTitleInfo,strPubInfo,strAuthorInfo;
	strTitleInfo = m_pDM_IDX->GetCellData(_T("ISS_표제사항"),m_nCurrentParentIdx);
	strPubInfo = m_pDM_IDX->GetCellData(_T("ISS_발행자사항"),m_nCurrentParentIdx);
	strAuthorInfo = m_pDM_IDX->GetCellData(_T("ISS_저작자사항"),m_nCurrentParentIdx);

	CSeVolDlg dlg(this);
	
	m_pGrid->SelectMakeList();
	INT cnt = m_pGrid->SelectGetCount();

	if(cnt>0) dlg.m_nMoveOption = 2;
	else dlg.m_nMoveOption = 1;
	
	dlg.m_nCurrentParentIdx = m_nCurrentIdx;
	dlg.m_pParentGrid = m_pGrid;
	dlg.m_nOpenMode = -1; //수정
	dlg.m_nACQ_TYPE = m_nACQ_TYPE;
	dlg.m_strTITLE_INFO = strTitleInfo;
	dlg.m_strAUTHOR_INFO = strAuthorInfo;
	dlg.m_strPUB_INFO = strPubInfo;
	dlg.m_bIsLightVersion = m_bIsLightVersion;

	//----------------------------------------------------------------------
	// 2005-10-11 권호수정 버튼을 누르면 저장될 수 있도록 수정
	//----------------------------------------------------------------------
/*	INT ids = dlg.DoModal();
	
	if(dlg.m_nCurrentParentIdx>=0)
	{
		SetGridRowBoldFont(dlg.m_nCurrentParentIdx, FALSE);
	}

	//책삭제 operation 수행
	if(11 == ids)
	{
		RefreshAll();
	}
	
	//책수가 증가 된 작업이 진행되었는지를 파악해 
	//접수번호 필요 유무를 확인한다.
	m_bIsNeedReceiptNo = dlg.m_bIsBookCntIncreased;
*/
	INT ids = dlg.DoModal();
	if( IDOK == ids )
	{
		if(dlg.m_nCurrentParentIdx>=0)
		{
			SetGridRowBoldFont(dlg.m_nCurrentParentIdx, FALSE);
		}
		m_bIsNeedReceiptNo = dlg.m_bIsBookCntIncreased;
		
		if(nCurSel == 2 || nCurSel == 1)
			CMD_SAVE();
	}
	else if(11 == ids)
	{
		RefreshAll();
	}
	else
	{
		;
	}
	return 0;	
}

INT	CCheckinVolESL::CMD_VOL_DELETE()
{
	//삭제 함수로 예정자료에대한 삭제와 기존 자료에 대한 삭제를 처리한다.
	m_pGrid->SelectMakeList();
	INT nIdx = m_pGrid->SelectGetHeadPosition();
	CString strUDF;
	CString strVOL_TITLE;
	CString msg;
	
	INT nCnt = m_pGrid->SelectGetCount();
	if(0 == nCnt)
	{
		AfxMessageBox(_T("선정된 자료가 없습니다!"));
		return 0;
	}
	
	msg.Format(_T("[ %d ] 개의 권호를 삭제 하시겠습니까?"),nCnt);
	if(AfxMessageBox(msg,MB_YESNO) != IDYES) return 0;
	
	CSeApi api(this);
	INT ids = 0;

	while(nIdx>=0)
	{
		strUDF = _T("");
		strUDF = m_pDM_WORK->GetCellData(_T("UDF_상태"),nIdx);
		strVOL_TITLE = m_pDM_WORK->GetCellData(_T("SV_권호명"),nIdx);
		
	
		if(_T("E") == strUDF || _T("M") == strUDF || _T("C") == strUDF) DeleteForUser(nIdx); 
		if(_T("D") == strUDF || _T("U") == strUDF) {
			
			//DB 데이터는 삭제 가능한지 조사한다.
			ids = api.VOL_DELETE_VERIFY(m_pDM_WORK,nIdx);
			if(ids<0)
			{
				
				if(-1001 == ids) //미확인 삭제 불가책 존재
				{
				
					if(m_bIsLightVersion)
					{
						
						msg.Format(_T("[ %s ] 권호는 이미 인계된 책 또는 부록이 존재합니다.\n")
								   _T("이러한 권호는 책정보 혹은 부록 화면에서 확인후 책단위로 삭제\n")
								   _T("할 수 있습니다. 확인해 주십시요"),strVOL_TITLE);

					}
					else
					{
						msg.Format(_T("[ %s ] 권호는 \n")
								   _T("<실시간인계자료,제본인계자료,등록인계자료>\n")
								   _T("이외의 책 또는 부록이 존재합니다.\n")
								   _T("이러한 권호는 책정보 화면에서 확인후 책단위로 삭제\n")
								   _T("할 수 있습니다. 확인해 주십시요"),strVOL_TITLE);
					}

					AfxMessageBox(msg);

				}
				else if(-1002 == ids) //기사 정보 존재 
				{
						
						msg.Format(_T("[ %s ] 권호는 기사정보가 있어 \n")
								   _T("해당 권호 삭제시 기사까지 삭제됩니다!\n")
							       _T("그래도 삭제 하시겠습니까?"),strVOL_TITLE);
						
						if(AfxMessageBox(msg,MB_YESNO) == IDYES) DeleteDBForUser(nIdx);
	
								   
						
				}
				else if(-1003 == ids) //부록 정보 존재 
				{

										
						msg.Format(_T("[ %s ] 권호는 부록이 있어 \n")
							       _T("해당 권호 삭제시 부록까지 삭제될 수 있습니다!\n")
							       _T("그래도 삭제 하시겠습니까?"),strVOL_TITLE);
						
						if(AfxMessageBox(msg,MB_YESNO) == IDYES) DeleteDBForUser(nIdx);

				}
				else if(-1004 == ids) //기사 및 부록 정보 존재 
				{
						
						msg.Format(_T("[ %s ] 권호는 기사와 부록 있어\n")
							       _T("해당 권호 삭제시 함께 삭제될 수 있습니다!\n")
							       _T("그래도 삭제 하시겠습니까?"),strVOL_TITLE);
						
						if(AfxMessageBox(msg,MB_YESNO) == IDYES) DeleteDBForUser(nIdx);				
				}
			

			}
			else 
			{
				DeleteDBForUser(nIdx);
			}
		}

		nIdx = m_pGrid->SelectGetNext();
	}	
	//----------------------------------------------------------------------
	// 2005-10-11 권호 삭제 버튼을 누르면 저장될 수 있도록 수정
	//----------------------------------------------------------------------
	CMD_SAVE();

	return 0;	
}

INT	CCheckinVolESL::CMD_VOL_MERGE()
{
	//합병호 함수 
	//예정 tab에 대해서만 가능함
	INT nCurSel = m_tabCTRL.GetCurSel();
	if(0 != nCurSel)
	{
		AfxMessageBox(_T("합병호 처리는 [예정/결호] 자료 에서만 가능합니다!"));
		return 0;	
	}
	
	INT nSelected = 0;
	m_pGrid->SelectMakeList();
	nSelected = m_pGrid->SelectGetCount();

	if(nSelected<2) 
	{
		AfxMessageBox(_T("두 개 이상의 항목이 선택되지 않았습니다!"));
		return 0;
	}

	INT idx = m_pGrid->SelectGetHeadPosition();
	INT nGap;
		
	while(idx>=0)
	{
		
		nGap = idx;
	
		idx =  m_pGrid->SelectGetNext();
		
		nGap = idx - nGap;

		if(nGap>1)
		{
			AfxMessageBox(_T("합병호처리는 연결된 권호에대해서만 가능합니다!"));
			return 0;
		}
	}

	CString msg;
	msg.Format(_T("선택된 %d 개의 권호를 합병호 처리 하시겠습니까?"),nSelected);
	if(AfxMessageBox(msg,MB_YESNO) != IDYES) return 0;


	CSeVolDlg dlg(this);
	dlg.m_pParentGrid = m_pGrid;
	dlg.m_nOpenMode = -2; //합병호
	dlg.m_nACQ_TYPE = m_nACQ_TYPE;
	dlg.m_bIsLightVersion = m_bIsLightVersion;

	if(dlg.DoModal() != IDOK) return 0;

	//합병호중 제일 나중 것에는 권호구분을 합병호로 만들고 나머지는 삭제 처리한다.
	m_pGrid->SelectMakeList();
	idx = m_pGrid->SelectGetHeadPosition();
	m_pDM_WORK->SetCellData(_T("UDF_권호구분"),_T("합병호"),idx);
	m_pDM_WORK->SetCellData(_T("SV_권호구분"),_T("2"),idx); //합병호
	m_pGrid->DisplayLine(idx);
	INT idx_Merge = idx;

	
	//삭제될 자료는 선정을 해제한다.
	CString strUDF; 

	while(idx>=0)
	{
		idx =  m_pGrid->SelectGetNext();
		
		strUDF = _T("");
		strUDF = m_pDM_WORK->GetCellData(_T("UDF_상태"),idx);
		
		if(idx>=0)
		{
			if(_T("E") == strUDF || _T("C") == strUDF || _T("M") == strUDF)
			{
				DeleteForUser(idx);
			}
			else
			{
				DeleteDBForUser(idx);
			}
			
			m_pGrid->SetAt(idx, m_pGrid->m_nSelectIdx, _T(""));
		}
	}

	strUDF = _T("");
	strUDF = m_pDM_WORK->GetCellData(_T("UDF_상태"),idx_Merge);
	if(_T("E") == strUDF || _T("M") == strUDF) CheckinForUser(idx_Merge);
	if(_T("D") == strUDF || _T("U") == strUDF) 
	{
		CheckinDBForUser(idx_Merge);
		m_bIsNeedReceiptNo = TRUE;
	}	
	//----------------------------------------------------------------------
	// 2005-10-11 합병호처리 버튼을 누르면 저장될 수 있도록 수정
	//----------------------------------------------------------------------
	CMD_SAVE();

	return 0;
}

INT	CCheckinVolESL::CMD_VOL_NON_PUBLISH()
{
	//미발행 처리
	//모든 권호에 대해서 가능 
	m_pGrid->SelectMakeList();
	INT nSelected = m_pGrid->SelectGetCount();

	if(nSelected<1) 
	{
		AfxMessageBox(_T("선택된 항목이 없습니다!"));
		return 0;
	}

	INT idx = m_pGrid->SelectGetHeadPosition();
	CString strVOLUME_CLASS;
	CString strVOL_TITLE;
	CString strUDF;
	CString msg;
	INT nProcess = 0;


	while(idx>=0)
	{
		strVOLUME_CLASS = _T("");
		strVOLUME_CLASS = m_pDM_WORK->GetCellData(_T("SV_권호구분"),idx);
		if(_T("3") == strVOLUME_CLASS) 
		{
			AfxMessageBox(_T("선택한 정보중에 이미 미발행처리된 자료가 있습니다! \n 확인하여 주십시요."));
			return 0;
		}
		
		strUDF = m_pDM_WORK->GetCellData(_T("UDF_상태"),idx);
		if(_T("X") == strUDF || _T("Z") == strUDF)
		{
			AfxMessageBox(_T("선택한 정보중에 삭제 예정 자료가 있습니다! \n 확인하여 주십시요."));
			return 0;
		}

		idx =  m_pGrid->SelectGetNext();
	}

	
	idx = m_pGrid->SelectGetHeadPosition();

	while(idx>=0)
	{
		strVOL_TITLE = _T("");
		strUDF = _T("");
		strVOL_TITLE = m_pDM_WORK->GetCellData(_T("SV_권호명"),idx);
		strUDF = m_pDM_WORK->GetCellData(_T("UDF_상태"),idx);


		msg.Format(_T("[ %s ] 권호를 미발행 처리 하시겠습니까?"),strVOL_TITLE);
		if(AfxMessageBox(msg,MB_YESNO) == IDYES)
		{

			if(_T("D") == strUDF || _T("U") == strUDF) 
			{
				m_pDM_WORK->SetCellData(_T("UDF_입수상태"),_T("입수"),idx);
				m_pDM_WORK->SetCellData(_T("UDF_상태"),_T("U"),idx);
				m_pDM_WORK->SetCellData(_T("UDF_권호구분"),_T("미발행"),idx);
				m_pDM_WORK->SetCellData(_T("SV_권호구분"),_T("3"),idx); //미발행
				m_pDM_WORK->SetCellData(_T("SA_입수책수"),_T("0"),idx);
				m_pDM_WORK->SetCellData(_T("SA_결호책수"),_T("0"),idx);
				m_pGrid->DisplayLine(idx);
				nProcess++;
			}
			else if(_T("X") == strUDF || _T("E") == strUDF || _T("C") == strUDF || _T("M") == strUDF)
			{

				m_pDM_WORK->SetCellData(_T("UDF_입수상태"),_T("입수(예정)"),idx);
				m_pDM_WORK->SetCellData(_T("UDF_상태"),_T("C"),idx);
				m_pDM_WORK->SetCellData(_T("UDF_권호구분"),_T("미발행"),idx);
				m_pDM_WORK->SetCellData(_T("SV_권호구분"),_T("3"),idx); //미발행
				m_pDM_WORK->SetCellData(_T("SA_입수책수"),_T("0"),idx);
				m_pDM_WORK->SetCellData(_T("SA_결호책수"),_T("0"),idx);
				m_pGrid->DisplayLine(idx);
				nProcess++;

			}

		}

		idx =  m_pGrid->SelectGetNext();
	}
	
	if(nProcess)
	{
	//----------------------------------------------------------------------
	// 2005-10-11 미발행처리 버튼을 누르면 저장될 수 있도록 수정
	//----------------------------------------------------------------------
		//msg.Format(_T(" %d 건의 자료를 미발행 처리 하였습니다!"),nProcess);
		//AfxMessageBox(msg);
		CMD_SAVE();
	}

	return 0;
}

INT	CCheckinVolESL::CMD_EXP_INFO()
{
	CSeExpectForm  dlg(this);
	dlg.m_nOpenMode = -2; //일반 수정
	dlg.m_nACQ_TYPE = m_nACQ_TYPE;
	dlg.m_strSPECIES_KEY = m_strSPECIES_KEY;
	dlg.m_strPUB_FREQ = m_strPUB_FREQ;
	dlg.m_strTITLE = m_pDM_IDX->GetCellData(_T("ISS_본표제"),m_nCurrentParentIdx);	 

	if(dlg.DoModal() == IDOK)
	{
		RefreshAll();
	}
	
	return 0;	
}

INT	CCheckinVolESL::CMD_CONTENTS_INFO()
{
	//목차편집
	//예정 tab에서는 할 수 없음 
	INT nCurSel = m_tabCTRL.GetCurSel();
	if(0 == nCurSel)
	{
		AfxMessageBox(_T("목차편집은 [예정/결호] 에서 할 수 없습니다!"));
		return 0;	
	}
	
	if(m_nCurrentIdx<0)
	{
		AfxMessageBox(_T("선택된 항목이 없습니다!"));
		return 0;
	}

	CCONTENTMGR_SE_EDIT dlg(this);
	dlg.Set_Mgr(_T(""),_T("DM_체크인_작업"),_T(""),_T("SV_권KEY"),m_nCurrentIdx);
	dlg.DoModal();
	
	
	//기사갯수 update
	CString strArticleCnt;
	strArticleCnt.Format(_T("%d"),dlg.m_KisaCount);
	m_pDM_WORK->SetCellData(_T("UDF_기사수"),strArticleCnt,m_nCurrentIdx);

	CString strACQ_VOL_KEY_WORK;
	CString strACQ_VOL_KEY_DB;
	strACQ_VOL_KEY_WORK = m_pDM_WORK->GetCellData(_T("SA_수입구분별권KEY"),m_nCurrentIdx);
	

	//일단은 검색 방식으로 
	for(INT i=0;i<m_pDM_DB->GetRowCount();i++)
	{
		strACQ_VOL_KEY_DB = _T("");
		strACQ_VOL_KEY_DB = m_pDM_DB->GetCellData(_T("SA_수입구분별권KEY"),i);
					
		if(strACQ_VOL_KEY_WORK == strACQ_VOL_KEY_DB) {
			m_pDM_DB->SetCellData(_T("UDF_기사수"),strArticleCnt,i);
		}
	}
	
	//그리드 refresh
	m_pGrid->DisplayLine(m_nCurrentIdx);
	

	return 0;
}

INT	CCheckinVolESL::CMD_REFRESH()
{
	RefreshAll();
	return 0;	
}

INT	CCheckinVolESL::CMD_MORE_EXP()
{
	CString strAutoUseYN = _T("N");
	CString strAutoCount = _T("0");
	
	LPCTSTR lpszFileName = CFG_FILE_PATH_MORE_EXP;

	CStdioFile file;

	INT nSuccess = file.Open(lpszFileName, CFile::modeRead | CFile::typeBinary);
	
	if(nSuccess)
	{
		CString strLine;
		CString tmp;
		INT nPos;
		INT nReadCnt = 0;

		TCHAR cUni;
		file.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			file.SeekToBegin();
		}
				
		while (file.ReadString(strLine))
		{
			strLine.TrimLeft();
			strLine.TrimRight();
			
			nPos = strLine.Find(_T(":"));

			if(nPos>0)
			{
				tmp = strLine.Left(nPos);
				tmp.TrimLeft();
				tmp.TrimRight();
				if(_T("AUTO_USE") == tmp)
				{
					strAutoUseYN = strLine.Mid(nPos+1);
					strAutoUseYN.TrimLeft();
					strAutoUseYN.TrimRight();
				}
				else if(_T("AUTO_COUNT") == tmp)
				{
					strAutoCount = strLine.Mid(nPos+1);
					strAutoCount.TrimLeft();
					strAutoCount.TrimRight();
				}

			}
		}

		file.Close();
	}
	
	INT nMoreExpCnt;

	if(_T("Y") == strAutoUseYN)
	{
		nMoreExpCnt = _ttoi(strAutoCount.GetBuffer(0));
	}
	else if(_T("N") == strAutoUseYN)
	{
		CCheckinVolMoreExp dlg(this);
		if(dlg.DoModal() != IDOK) return 0;
		nMoreExpCnt = _ttoi(dlg.m_strMoreExpCnt.GetBuffer(0));
	}
	
	RefreshAll(nMoreExpCnt);


	return 0;
}

INT	CCheckinVolESL::CMD_MORE_EXP_SETTING()
{
	//권호별 체크인 화면 설정(추가예측, 그리드 색상)

	CString strDefaultUseYN = _T("N");
	CString strDefaultCount = _T("0");
	CString strAutoUseYN = _T("N");
	CString strAutoCount = _T("0");
	
	LPCTSTR lpszFileName = CFG_FILE_PATH_MORE_EXP;

	CStdioFile file;

	INT nSuccess = file.Open(lpszFileName, CFile::modeRead | CFile::typeBinary);
	
	if(nSuccess)
	{
		CString strLine;
		CString tmp;
		INT nPos;
		INT nReadCnt = 0;

		TCHAR cUni;
		file.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			file.SeekToBegin();
		}
				
		while (file.ReadString(strLine))
		{
			strLine.TrimLeft();
			strLine.TrimRight();
			
			nPos = strLine.Find(_T(":"));

			if(nPos>0)
			{
				tmp = strLine.Left(nPos);
				tmp.TrimLeft();
				tmp.TrimRight();

				if(_T("DEFAULT_USE") == tmp)
				{
					strDefaultUseYN = strLine.Mid(nPos+1);
					strDefaultUseYN.TrimLeft();
					strDefaultUseYN.TrimRight();
				}
				else if(_T("DEFAULT_COUNT") == tmp)
				{
					strDefaultCount = strLine.Mid(nPos+1);
					strDefaultCount.TrimLeft();
					strDefaultCount.TrimRight();
				}
				else if(_T("AUTO_USE") == tmp)
				{
					strAutoUseYN = strLine.Mid(nPos+1);
					strAutoUseYN.TrimLeft();
					strAutoUseYN.TrimRight();
				}
				else if(_T("AUTO_COUNT") == tmp)
				{
					strAutoCount = strLine.Mid(nPos+1);
					strAutoCount.TrimLeft();
					strAutoCount.TrimRight();
				}

			}
		}

		file.Close();
	}
	
	CCheckinVolSetMoreExp dlg(this);
	

	//기본추가예측
	if(_T("Y") == strDefaultUseYN)
	{
		dlg.m_bDEFAULT_USE_YN = TRUE;
	}
	else if(_T("N") == strDefaultUseYN)
	{
		dlg.m_bDEFAULT_USE_YN = FALSE;
	}

	dlg.m_strDEFAULT_EXP_NO = strDefaultCount;
	

	//자동추가예측
	if(_T("Y") == strAutoUseYN)
	{
		dlg.m_bAUTO_USE_YN = TRUE;
	}
	else if(_T("N") == strAutoUseYN)
	{
		dlg.m_bAUTO_USE_YN = FALSE;
	}
	
	dlg.m_strMORE_EXP_NO = strAutoCount;

	//그리드 색상
	dlg.CR_exp = CR_exp; //예정 
	dlg.CR_acq = CR_acq; //입수 
	dlg.CR_lack = CR_lack; //미달 
	dlg.CR_miss = CR_miss; //결호 
	dlg.CR_delay = CR_delay; //지연 
	dlg.CR_nonpub = CR_nonpub; //미발행

	if(dlg.DoModal() != IDOK) return 0;
	

	//기본 추가 예측 갯수 설정
	if(dlg.m_bDEFAULT_USE_YN)
		strDefaultUseYN = _T("Y");
	else
		strDefaultUseYN = _T("N");
	

	strDefaultCount = dlg.m_strDEFAULT_EXP_NO;

	
	//추가 예측 갯수 설정
	if(dlg.m_bAUTO_USE_YN)
		strAutoUseYN = _T("Y");
	else
		strAutoUseYN = _T("N");
	
	strAutoCount = dlg.m_strMORE_EXP_NO;
	
	CStdioFile fileWrite;
	CString tmp;
	nSuccess = fileWrite.Open(lpszFileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);

	if(nSuccess)
	{
		if( 0 == fileWrite.GetLength() )
		{			
			TCHAR cUni = 0xFEFF;
			fileWrite.Write( &cUni, sizeof(TCHAR));
		}
		
		tmp.Format(_T("DEFAULT_USE : %s\r\n"),strDefaultUseYN);
		fileWrite.WriteString(tmp);
	
		tmp.Format(_T("DEFAULT_COUNT : %s\r\n"),strDefaultCount);
		fileWrite.WriteString(tmp);

		tmp.Format(_T("AUTO_USE : %s\r\n"),strAutoUseYN);
		fileWrite.WriteString(tmp);
	
		tmp.Format(_T("AUTO_COUNT : %s\r\n"),strAutoCount);
		fileWrite.WriteString(tmp);
	
		fileWrite.Close();
	}

	//그리드 색상 설정
	CR_exp = dlg.CR_exp; //예정 
	CR_acq = dlg.CR_acq; //입수 
	CR_lack = dlg.CR_lack; //미달 
	CR_miss = dlg.CR_miss; //결호 
	CR_delay = dlg.CR_delay; //지연 
	CR_nonpub = dlg.CR_nonpub; //미발행

	//그리드 색상 파일 저장
	WriteColorRefOnFile();

	AfxMessageBox(_T("설정되었습니다!"));

	return 0;
}

INT CCheckinVolESL::CMD_RECEIPT_NO()
{
	//접수번호 설정
	CCheckinReceipt dlg(this);
	dlg.m_strACQ_YEAR = m_strACQ_YEAR;
	dlg.m_nACQ_TYPE = m_nACQ_TYPE;

	if(dlg.DoModal() == IDOK)
	{
		m_strACQ_YEAR = dlg.m_strACQ_YEAR;
		m_strRECEIPT_NO = dlg.m_strRECEIPT_NO;

		//종정보 다이얼로그 refresh 
		if(m_pResourceMgr) {

			m_pResourceMgr->m_dlgSpeciesInfo.SetReceiptNo(m_strACQ_YEAR,m_strRECEIPT_NO);
			m_pResourceMgr->m_dlgSpeciesInfo.ShowSpeciesInfo(m_nCurrentParentIdx);
		
		}	 
	}

	return 0;
}

INT	CCheckinVolESL::CMD_PREV()
{
	//===================================================
	//2010.01.11 UPDATE BY PJW : INDEX이동 수정함
//	INT nPos = m_pParentGrid->SelectGetPrev();
//	if(nPos<0) {
//
//		m_pParentGrid->SelectMakeList();
//		m_nCurrentParentIdx = m_pParentGrid->SelectGetHeadPosition();
//		if(m_nCurrentParentIdx<0) 
//		{	
//			nPos = m_pParentGrid->GetIdx() - 1;
//			if ( nPos == -1 ) nPos = 0;
//		}
//		else
//		{
//
//			m_pParentGrid->SelectGetHeadPosition();
//			AfxMessageBox(_T("맨 처음 정보 입니다!"));
//			return 0;
//		}
//	}
	INT nPos;
	INT nHeadIdx;
	m_pParentGrid->SelectMakeList();
	nHeadIdx = m_pParentGrid->SelectGetHeadPosition();
	nPos = nHeadIdx;
	while( 1 )
	{
		if( nPos == m_nCurrentParentIdx )
		{
			if( m_nCurrentParentIdx == nHeadIdx )
			{
				AfxMessageBox(_T("맨 처음 정보 입니다!"));
				return 0;
			}
			else
			{
				nPos = m_pParentGrid->SelectGetPrev();
			}
			break;
		}
		nPos = m_pParentGrid->SelectGetNext();
	}

	//===================================================
	
	m_nCurrentParentIdx = nPos;
	RefreshAll();

	return 0;
}

INT	CCheckinVolESL::CMD_NEXT()
{
	//===================================================
	//2010.01.11 UPDATE BY PJW : INDEX이동 수정함
//	INT nPos = m_pParentGrid->SelectGetNext();
//	if(nPos<0) {
//
//		m_pParentGrid->SelectMakeList();
//		m_nCurrentParentIdx = m_pParentGrid->SelectGetHeadPosition();
//		if(m_nCurrentParentIdx<0) 
//		{	
//			nPos = m_pParentGrid->GetIdx() + 1;
//		}
//		else
//		{
//			m_pParentGrid->SelectGetTailPosition();
//			AfxMessageBox(_T("맨 마지막 정보 입니다!"));
//			return 0;
//		}
//	}
	INT nPos;
	INT nTailIdx;
	m_pParentGrid->SelectMakeList();
	nTailIdx = m_pParentGrid->SelectGetTailPosition();
	nPos = nTailIdx;
	while( 1 )
	{
		if( nPos == m_nCurrentParentIdx )
		{
			if( m_nCurrentParentIdx == nTailIdx )
			{
				AfxMessageBox(_T("맨 마지막 정보 입니다!"));
				return 0;
			}
			else
			{
				nPos = m_pParentGrid->SelectGetNext();
			}
			break;
		}
		nPos = m_pParentGrid->SelectGetPrev();
	}

	//===================================================
	
	m_nCurrentParentIdx = nPos;
	RefreshAll();
	
	return 0;	
}

INT CCheckinVolESL::CMD_COVER_INFO()
{
	//표지정보 
	//예정 tab에서는 할 수 없음 
	
	INT nCurSel = m_tabCTRL.GetCurSel();
	if(0 == nCurSel)
	{
		AfxMessageBox(_T("표지정보 관리는 [예정/결호] 에서 할 수 없습니다!"));
		return 0;	
	}
	
	if(m_nCurrentIdx<0)
	{
		AfxMessageBox(_T("선택된 항목이 없습니다!"));
		return 0;
	}

	CString strVOL_KEY;
	strVOL_KEY = m_pDM_WORK->GetCellData(_T("SV_권KEY"),m_nCurrentIdx);

	if(strVOL_KEY.IsEmpty())
	{
		AfxMessageBox(_T("해당 권호키를 찾을 수 없습니다!"));
		return 0;
	}

	CCOVER_MAIN dlg(this);
	dlg.SetInsertMode(FALSE);
	dlg.SetPubType(_SERIAL_);
	dlg.SetLocalPath(_T("..\\bmp\\cover_info\\"));
	dlg.SetBasicName(strVOL_KEY.GetBuffer(0));
	dlg.SetSpeciesKey(strVOL_KEY.GetBuffer(0));
	dlg.DoModal();


	return 0;
}


INT CCheckinVolESL::CMD_PROCESSPRINT()
{
	INT ids = -1;
	CString strWhere;
	//서명/발행자/구입처/수입구분/간행빈도/구독기간/구독가격/총입수책수/열람우선
	CString strtitle = _T("");
	CString strPublisher = _T("");
	CString strPurPlace = _T("");
	CString strAcqCode = _T("");
	CString strFreq = _T("");
	CString strPurStart = _T("");
	CString strPurEnd = _T("");
	CString strPurPrice = _T("");
	CString strTotalBookCnt = _T("");
	CString strLateBookCnt =_T("");
	CString strMissBookCnt =_T("");
	CString strAppendBookCnt=_T("");	
	CString strIsLocFirst = _T("");
	CString strISSN = _T("");
	

	m_pDM_DB->FreeData();

	CString strACQ_KEY = _T("");
	CString strSpeciesKey = _T("");
	if(1 == m_nACQ_TYPE)
	{
		strACQ_KEY = m_pDM_ACQ->GetCellData(_T("SP_구입관리KEY"),0);
		//strTotalBook = m_pDM_ACQ->GetCellData(_T("SP_입수책수"),0);
		strIsLocFirst = m_pDM_ACQ->GetCellData(_T("SP_열람우선여부"),0);
		strPurStart = m_pDM_ACQ->GetCellData(_T("SP_구독시작일"),0);
		strPurEnd = m_pDM_ACQ->GetCellData(_T("SP_구독만기일"),0);
		strPurPrice = m_pDM_ACQ->GetCellData(_T("SP_구독가격"),0);
	}
	else
	{
		strACQ_KEY = m_pDM_ACQ->GetCellData(_T("SD_기증관리KEY"),0);
		//strTotalBook = m_pDM_ACQ->GetCellData(_T("SD_입수책수"),0);
		strIsLocFirst = m_pDM_ACQ->GetCellData(_T("SD_열람우선여부"),0);
		strPurStart = m_pDM_ACQ->GetCellData(_T("SD_기증시작일"),0);
		//strPurEnd = m_pDM_ACQ->GetCellData(_T("SD_구독만기일"),0);
		//strPurPrice = m_pDM_ACQ->GetCellData(_T("SD_구독가격"),0);
	}

	UpdateData(TRUE);
	CString strPUBLISH_DATE;
	m_pCM->GetControlMgrData(_T("발행년도"),strPUBLISH_DATE);

	strWhere.Format(_T("A.VOL_KEY = V.REC_KEY AND V.VOL_TYPE = 'V' AND A.ACQ_KEY = %s"),strACQ_KEY); 
	//	m_pDM_DB->RefreshDataManager(strWhere);
	
	m_pDM_DB->RefreshDataManager(strWhere);
	
	m_pDM_WORK->FreeData();

	CopyAllDBToWork();

	INT nRowCnt = m_pDM_WORK->GetRowCount();
	if(0 == nRowCnt)
	{
		AfxMessageBox(_T("출력할 자료가 없습니다!"));
		return 0;
	}
	
	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	
	pSIReportManager->SetCONNECTION_INFO(m_pDM_WORK->CONNECTION_INFO);
	ids = pSIReportManager->GetSIReportFileFromDB(_T("연속_체크인_상황표"));
	
	if (ids < 0) {
		ESLAfxMessageBox(_T("DB로 부터 SI Report 정보를 가져오지 못하였습니다."));
		return 0;
	}
	pSIReportManager->m_pWorkForm->paper = PAPER_A4_LANDSCAPE;

	strSpeciesKey = m_pDM_IDX->GetCellData(_T("ISS_종KEY"),m_nCurrentParentIdx);

	strWhere.Format(_T("select count(*) from  se_book_tbl where book_type = 'C' and species_key = %s"),strSpeciesKey);
	m_pDM_WORK->GetOneValueQuery(strWhere,strTotalBookCnt);
	strWhere.Format(_T("select count(*) from  se_book_tbl where book_type = 'M' and species_key = %s"),strSpeciesKey);
	m_pDM_WORK->GetOneValueQuery(strWhere,strMissBookCnt);
	strWhere.Format(_T("select count(*) from  se_book_tbl where book_type = 'L' and species_key = %s"),strSpeciesKey);
	m_pDM_WORK->GetOneValueQuery(strWhere,strLateBookCnt);
	strWhere.Format(_T("select count(*) from  se_book_tbl where book_type = 'A' and species_key = %s"),strSpeciesKey);
	m_pDM_WORK->GetOneValueQuery(strWhere,strAppendBookCnt);
	
	strtitle = m_pDM_IDX->GetCellData(_T("ISS_표제사항"),m_nCurrentParentIdx);
	strPublisher = m_pDM_IDX->GetCellData(_T("ISS_발행자사항"),m_nCurrentParentIdx);
	strISSN = m_pDM_IDX->GetCellData(_T("ISS_ISSN"),m_nCurrentParentIdx);
	strFreq = m_pDM_IDX->GetCellData(_T("UDF_간행빈도"),m_nCurrentParentIdx);
	strAcqCode = m_pDM_IDX->GetCellData(_T("UDF_수입구분"),m_nCurrentParentIdx);

	pSIReportManager->SetTempVariable(_T("서명"),strtitle);
	pSIReportManager->SetTempVariable(_T("발행자"),strPublisher);
	pSIReportManager->SetTempVariable(_T("구입처"),strPurPlace);
	pSIReportManager->SetTempVariable(_T("수입구분"),strAcqCode);
	pSIReportManager->SetTempVariable(_T("간행빈도"),strFreq);
	strPurStart = strPurStart + _T(" ~ ") + strPurEnd;
	pSIReportManager->SetTempVariable(_T("구독기간"),strPurStart);
	pSIReportManager->SetTempVariable(_T("구독가격"),strPurPrice);
	pSIReportManager->SetTempVariable(_T("총입수책수"),strTotalBookCnt);
	pSIReportManager->SetTempVariable(_T("열람우선"),strIsLocFirst);
	pSIReportManager->SetTempVariable(_T("ISSN"),strISSN);
	pSIReportManager->SetTempVariable(_T("결호책수"),strMissBookCnt);
	pSIReportManager->SetTempVariable(_T("지연책수"),strLateBookCnt);
	pSIReportManager->SetTempVariable(_T("부록책수"),strAppendBookCnt);

	pSIReportManager->SetDataMgr(0, m_pDM_WORK);
	ids = pSIReportManager->MakeXMLPageFiles();
	// 마지막 수평선을 그림
	Print_SetLastPageLine();
	ids = pSIReportManager->Print2();
	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}

	//RefreshAll();
	return 0;
}

VOID CCheckinVolESL::Print_SetLastPageLine()
{
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
	CSIReportObject_Table * pIn = (CSIReportObject_Table*)pSheet->FindObject(_T("테이블_1"));
	CSIReportObject_Table * pOut = (CSIReportObject_Table*)pSheet->FindObject(_T("테이블_19"));
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
}


INT CCheckinVolESL::CheckUnprocessedData()
{
	//예측/결호 탭에서의 처리되지 않은 예측 데이터는 자동 결호처리 여부를 묻고 
	//아니면 return -1;
	
	INT nRow = m_pDM_WORK->GetRowCount();
	CString strUDF;
	BOOL bChecked = FALSE;	
	CArray<INT,INT> arrayUnprocessedIdx;
	

	for(INT i=0;i<nRow;i++)
	{
		strUDF = _T("");
		strUDF = m_pDM_WORK->GetCellData(_T("UDF_상태"),i);
		
		if(!bChecked)
		{
			if(_T("C") == strUDF || _T("M") == strUDF || _T("X") == strUDF) bChecked = TRUE;
		}
		else 
		{
			if(_T("E") == strUDF) {
				arrayUnprocessedIdx.Add(i);
			}
		}
	
	}
	
	CString msg;
	INT nSize = arrayUnprocessedIdx.GetSize();
	if( nSize > 0)
	{
		msg.Format(_T("%d 건의 처리되지 않은 이전 예측 자료가 존재 합니다 \n 계속 진행할 경우 이들은 자동 결호 처리 됩니다. 진행하시 겠습니까?"), nSize);
		if(AfxMessageBox(msg,MB_YESNO) != IDYES)
		{
			CMD_REFRESH();
			return -1;
		}
	}

	INT nRowIdx;
	for(i=0;i<nSize;i++)
	{
		nRowIdx = arrayUnprocessedIdx.GetAt(i);
		MissbookForUser(nRowIdx);
	}

	return 0;
}

INT CCheckinVolESL::CheckinForUser(INT nRowIdx)
{
	CString strACQ_EXP_BOOK_CNT;
	strACQ_EXP_BOOK_CNT = m_pDM_WORK->GetCellData(_T("SA_입수예정책수"),nRowIdx);
	if(!strACQ_EXP_BOOK_CNT.IsEmpty()) m_pDM_WORK->SetCellData(_T("SA_입수책수"),strACQ_EXP_BOOK_CNT,nRowIdx);
	m_pDM_WORK->SetCellData(_T("SA_결호책수"),_T("0"),nRowIdx);
	m_pDM_WORK->SetCellData(_T("UDF_입수상태"),_T("입수(예정)"),nRowIdx);
	m_pDM_WORK->SetCellData(_T("UDF_상태"),_T("C"),nRowIdx);
	m_pGrid->DisplayLine(nRowIdx);
	
	return 0;
}

INT CCheckinVolESL::CheckinDBForUser(INT nRowIdx)
{
	//결호 책수를 확인한후 이를 입수책수에 더한후 상태를 U 로 바꾸어 준다. 
	CString strMISS_BOOK_CNT = _T("");
	strMISS_BOOK_CNT =  m_pDM_WORK->GetCellData(_T("SA_결호책수"),nRowIdx);
	INT nMISS_BOOK_CNT = 0;
	nMISS_BOOK_CNT = _ttoi(strMISS_BOOK_CNT.GetBuffer(0));

	if(0 == nMISS_BOOK_CNT) return 0;
	
	CString strACQ_BOOK_CNT = _T("");
	strACQ_BOOK_CNT =  m_pDM_WORK->GetCellData(_T("SA_입수책수"),nRowIdx);
	INT nACQ_BOOK_CNT = _ttoi(strACQ_BOOK_CNT.GetBuffer(0));
	
	CString strNewBookCnt;
	INT nNewBookCnt = nACQ_BOOK_CNT + nMISS_BOOK_CNT;
	strNewBookCnt.Format(_T("%d"),nNewBookCnt);

	m_pDM_WORK->SetCellData(_T("SA_입수책수"),strNewBookCnt,nRowIdx);
	m_pDM_WORK->SetCellData(_T("SA_결호책수"),_T("0"),nRowIdx);
	m_pDM_WORK->SetCellData(_T("UDF_입수상태"),_T("입수"),nRowIdx);
	m_pDM_WORK->SetCellData(_T("UDF_상태"),_T("U"),nRowIdx);
	m_pGrid->DisplayLine(nRowIdx);
	
	return 0;
}


INT CCheckinVolESL::MissbookForUser(INT nRowIdx)
{
	CString strACQ_EXP_BOOK_CNT;
	strACQ_EXP_BOOK_CNT = m_pDM_WORK->GetCellData(_T("SA_입수예정책수"),nRowIdx);
	m_pDM_WORK->SetCellData(_T("SA_입수책수"),_T("0"),nRowIdx);
	if(!strACQ_EXP_BOOK_CNT.IsEmpty()) m_pDM_WORK->SetCellData(_T("SA_결호책수"),strACQ_EXP_BOOK_CNT,nRowIdx);
	m_pDM_WORK->SetCellData(_T("UDF_입수상태"),_T("결호(예정)"),nRowIdx);
	m_pDM_WORK->SetCellData(_T("UDF_상태"),_T("M"),nRowIdx);
	m_pGrid->DisplayLine(nRowIdx);

	return 0;
}

INT CCheckinVolESL::MissbookDBForUser(INT nRowIdx)
{
	
	//입수책수를 결호책수 더한후 상태를 U 로 바꾸어 준다. 
	CString strACQ_BOOK_CNT = _T("");
	strACQ_BOOK_CNT =  m_pDM_WORK->GetCellData(_T("SA_입수책수"),nRowIdx);
	INT nACQ_BOOK_CNT = 0;
	nACQ_BOOK_CNT = _ttoi(strACQ_BOOK_CNT.GetBuffer(0));
	
	if(0 == nACQ_BOOK_CNT) 
	{
		CString strUDF;
		strUDF = m_pDM_WORK->GetCellData(_T("UDF_입수상태"),nRowIdx);
		if(_T("지연") == strUDF) {
			m_pDM_WORK->SetCellData(_T("UDF_입수상태"),_T("결호"),nRowIdx);
			m_pDM_WORK->SetCellData(_T("UDF_상태"),_T("U"),nRowIdx);
			m_pGrid->DisplayLine(nRowIdx);
		}
		
		return 0;
	}

	CString strMISS_BOOK_CNT = _T("");
	strMISS_BOOK_CNT =  m_pDM_WORK->GetCellData(_T("SA_결호책수"),nRowIdx);
	INT nMISS_BOOK_CNT = _ttoi(strMISS_BOOK_CNT.GetBuffer(0));
	
	CString strNewBookCnt;
	INT nNewBookCnt = nACQ_BOOK_CNT + nMISS_BOOK_CNT;
	strNewBookCnt.Format(_T("%d"),nNewBookCnt);

	m_pDM_WORK->SetCellData(_T("SA_결호책수"),strNewBookCnt,nRowIdx);
	m_pDM_WORK->SetCellData(_T("SA_입수책수"),_T("0"),nRowIdx);
	m_pDM_WORK->SetCellData(_T("UDF_입수상태"),_T("결호"),nRowIdx);
	m_pDM_WORK->SetCellData(_T("UDF_상태"),_T("U"),nRowIdx);
	m_pGrid->DisplayLine(nRowIdx);

	return 0;
}

INT CCheckinVolESL::DeleteForUser(INT nRowIdx)
{
	m_pDM_WORK->SetCellData(_T("UDF_입수상태"),_T("삭제(예정)"),nRowIdx);
	m_pDM_WORK->SetCellData(_T("UDF_상태"),_T("X"),nRowIdx);
	m_pGrid->DisplayLine(nRowIdx);

	return 0;
}

INT CCheckinVolESL::DeleteDBForUser(INT nRowIdx)
{

	m_pDM_WORK->SetCellData(_T("UDF_입수상태"),_T("삭제"),nRowIdx);
	m_pDM_WORK->SetCellData(_T("UDF_상태"),_T("Z"),nRowIdx);
	m_pGrid->DisplayLine(nRowIdx);
	
	return 0;
}

BEGIN_EVENTSINK_MAP(CCheckinVolESL, CDialog)
    //{{AFX_EVENTSINK_MAP(CCheckinVolESL)
	ON_EVENT(CCheckinVolESL, IDC_gridCHECKIN_VOL, -600 /* Click */, OnClickgridCHECKINVOL, VTS_NONE)
	ON_EVENT(CCheckinVolESL, IDC_gridCHECKIN_VOL, -601 /* DblClick */, OnDblClickgridCHECKINVOL, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CCheckinVolESL::OnClickgridCHECKINVOL() 
{
	// TODO: Add your control notification handler code here
	INT nDMRow = m_pDM_WORK->GetRowCount();
	if(0 == nDMRow) return;

	INT row = m_pGrid->GetRow();
	if (row == 0) return;
	if (row > 0) {

		if(m_nCurrentIdx>=0) {
			
			if(m_nCurrentIdx<nDMRow)
				SetGridRowBoldFont(m_nCurrentIdx,FALSE);
		}

		m_nCurrentIdx = row-1;
		SetGridRowBoldFont(m_nCurrentIdx,TRUE);
	
	}

}

VOID CCheckinVolESL::SetGridRowBoldFont(INT nRowIdx, BOOL bNewValue)
{
	INT nCol = m_pGrid->GetCol();
	INT nCols = m_pGrid->GetCols(0);
	
	if (nRowIdx >= 0)
	{
		m_pGrid->SetRow(nRowIdx + 1);
		
		for (INT i = 1 ; i < nCols; i++)
		{
			m_pGrid->SetCol(i);
			m_pGrid->SetCellFontBold(bNewValue);
		}

	}
	
	m_pGrid->SetCol(nCol);
}

INT CCheckinVolESL::SetGridRowUniqueColor(COLORREF& colorref, INT nRowIdx)
{

	INT nCol = m_pGrid->GetCol();
	INT nCols = m_pGrid->GetCols(0);
	
	if (nRowIdx >= 0)
	{
		m_pGrid->SetRow(nRowIdx + 1);
		
		for (INT i = 1 ; i < nCols; i++)
		{
			m_pGrid->SetCol(i);
			m_pGrid->SetCellBackColor(colorref);
			m_pGrid->SetCellForeColor(CR_BLACK);
		}
	}
	
	m_pGrid->SetCol(nCol);
	
	return 0;
}

VOID CCheckinVolESL::OnDblClickgridCHECKINVOL() 
{
	INT nRow = m_pGrid->GetMouseRow();
	
	if( nRow > 0)
	{
		CMD_VOL_MODIFY();
	}
	SetAllGridColor();
}


INT CCheckinVolESL::GetDefaultMoreExpCnt()
{

	CString strDefaultUseYN = _T("N");
	CString strDefaultCount = _T("0");
	
	LPCTSTR lpszFileName = CFG_FILE_PATH_MORE_EXP;

	CStdioFile file;

	INT nSuccess = file.Open(lpszFileName, CFile::modeRead | CFile::typeBinary);
	
	if(nSuccess)
	{
		CString strLine;
		CString tmp;
		INT nPos;
		INT nReadCnt = 0;

		TCHAR cUni;
		file.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			file.SeekToBegin();
		}
				
		while (file.ReadString(strLine))
		{
			strLine.TrimLeft();
			strLine.TrimRight();
			
			nPos = strLine.Find(_T(":"));

			if(nPos>0)
			{
				tmp = strLine.Left(nPos);
				tmp.TrimLeft();
				tmp.TrimRight();
				if(_T("DEFAULT_USE") == tmp)
				{
					strDefaultUseYN = strLine.Mid(nPos+1);
					strDefaultUseYN.TrimLeft();
					strDefaultUseYN.TrimRight();
				}
				else if(_T("DEFAULT_COUNT") == tmp)
				{
					strDefaultCount = strLine.Mid(nPos+1);
					strDefaultCount.TrimLeft();
					strDefaultCount.TrimRight();
				}

			}
		}

		file.Close();
	}
	
	INT nMoreExpCnt = 0;

	if(_T("Y") == strDefaultUseYN)
	{
		if(!strDefaultCount.IsEmpty()) nMoreExpCnt = _ttoi(strDefaultCount.GetBuffer(0));
	}
	
	return nMoreExpCnt;
}

INT CCheckinVolESL::GetNewReceiptNo()
{

	CSeApi api(this);
	
	if(m_strACQ_YEAR.IsEmpty())
	{
		m_strACQ_YEAR = api.GetCurrentYear();
	}

	CString strRECEIPT_NO;
	strRECEIPT_NO = api.GetLastReceiptNo(m_pDM_WORK,m_nACQ_TYPE,m_strACQ_YEAR);


	CString strDate;
	strDate = api.GetTodayDate();

	CString strWorkLog;
	strWorkLog = GetWorkLogMsg(WORK_LOG_CHECKIN);

	m_pDM_WORK->StartFrame();
	m_pDM_WORK->InitDBFieldData();
	// 2005-04-11 By SM5ong
	// 마지막 차수번호+1로 Update
	// BEGIN:
	CString strKindCode, strQuery;
  	if(1 == m_nACQ_TYPE)
		strKindCode = _T("CK1_NO"); //구입 접수번호 
	else if(2 == m_nACQ_TYPE)
		strKindCode = _T("CK2_NO"); //기증 접수번호 
  
	if( _T("1") == strRECEIPT_NO )	//검색결과가 없으면 INSERT 
	{
		strQuery.Format(_T("INSERT INTO CO_LAST_NUMBER_TBL (REC_KEY, KIND_CODE, PUBLISH_FORM_CODE, MAKE_YEAR,LAST_NUMBER,CREATE_DATE,CREATE_WORKER,FIRST_WORK,MANAGE_CODE)")
						_T("VALUES	(%s.NEXTVAL,'%s','SE','%s',1,'%s','%s','%s',UDF_MANAGE_CODE);"), 
						DEFAULT_SEQ_NAME, strKindCode, m_strACQ_YEAR, strDate, m_pInfo->USER_ID, strWorkLog);
		m_pDM_WORK->AddFrame(strQuery); 
	}
	else	//있으면 UPDATE
	{
		//===================================================
		//2009.12.01 UPDATE BY PJW : MANAGE_CODE 조건 추가
//		strQuery.Format(_T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER = %s, LAST_WORK = '%s' WHERE MAKE_YEAR = '%s' AND KIND_CODE = '%s';"),
		strQuery.Format(_T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER = %s, LAST_WORK = '%s' WHERE MAKE_YEAR = '%s' AND KIND_CODE = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE;"),
		//===================================================		
			            strRECEIPT_NO, strWorkLog, m_strACQ_YEAR, strKindCode);
		m_pDM_WORK->AddFrame(strQuery);
	}
	// END:
	strQuery.Format(_T("INSERT INTO SE_RECEIPT_TBL ")
					_T("(REC_KEY, ACQ_CODE, ACQ_YEAR, RECEIPT_NO, LAST_SERIAL_NO, ")
					_T("RECEIPT_NO_CREATE_DATE, RECEIPT_NO_CREATE_WORKER, FIRST_WORK, LAST_WORK,MANAGE_CODE) ")
					_T("VALUES (%s.NEXTVAL,'%d','%s','%s',0,'%s','%s','%s','%s',UDF_MANAGE_CODE);")
					,DEFAULT_SEQ_NAME,m_nACQ_TYPE,m_strACQ_YEAR,
					strRECEIPT_NO, strDate, m_pInfo->USER_ID, strWorkLog, strWorkLog);
	m_pDM_WORK->AddFrame(strQuery);
					
	INT ids = m_pDM_WORK->SendFrame();
	m_pDM_WORK->EndFrame();
	
	if(ids<0) return -1;

	m_strRECEIPT_NO = strRECEIPT_NO;

	//종정보 다이얼로그 refresh 
	if(m_pResourceMgr) {

		m_pResourceMgr->m_dlgSpeciesInfo.SetReceiptNo(m_strACQ_YEAR,m_strRECEIPT_NO);
		m_pResourceMgr->m_dlgSpeciesInfo.ShowSpeciesInfo(m_nCurrentParentIdx);
		
	}	 

	return 0;
}

BOOL CCheckinVolESL::IsNeedReceipNo()
{
	//수정시 책수가 증가된 경우나 결호 책이 입수된 경우 
	if(m_bIsNeedReceiptNo) return TRUE;

	INT nRow = m_pDM_WORK->GetRowCount();
	

	//새로 입수처리가 된경우 
	CString strUDF;

	for(INT i=0;i<nRow;i++)
	{
		strUDF = _T("");
		strUDF = m_pDM_WORK->GetCellData(_T("UDF_상태"),nRow-i-1);	

		if(_T("C") == strUDF) 
		{
			return TRUE;
		}
	}

	return FALSE;
}

VOID CCheckinVolESL::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	KillTimer(0);
	if(m_pResourceMgr) m_pResourceMgr->HideToolBarButton(_T("표지정보"));

	CDialog::OnTimer(nIDEvent);
}

VOID CCheckinVolESL::OnbtnCHECKINVOLPREV() 
{
	// TODO: Add your control notification handler code here
	CString strYear;
	m_pCM->GetControlMgrData(_T("발행년도"),strYear);

	if(strYear.IsEmpty()) return;

	INT nYear = 0;
	nYear = _ttoi(strYear.GetBuffer(0));
	nYear--;
	
	strYear.Format(_T("%d"),nYear);
	m_pCM->SetControlMgrData(_T("발행년도"),strYear);
}

VOID CCheckinVolESL::OnbtnCHECKINVOLNEXT() 
{
	// TODO: Add your control notification handler code here
	CString strYear;
	m_pCM->GetControlMgrData(_T("발행년도"),strYear);

	if(strYear.IsEmpty()) return;

	INT nYear = 0;
	nYear = _ttoi(strYear.GetBuffer(0));
	nYear++;
	
	strYear.Format(_T("%d"),nYear);
	m_pCM->SetControlMgrData(_T("발행년도"),strYear);
}

VOID CCheckinVolESL::OnchkCHECKINVOLALLSHOW() 
{
	// TODO: Add your control notification handler code here
	
	//전체 보기가 클릭되면 발행년 콘트롤을 비활성화
	UpdateData(TRUE);
	
	CWnd* pWnd = NULL;


	if(m_bIsAllDataShow)
	{
		pWnd = GetDlgItem(IDC_btnCHECKIN_VOL_PREV);
		if(pWnd) pWnd->EnableWindow(FALSE);

		pWnd = GetDlgItem(IDC_btnCHECKIN_VOL_NEXT);
		if(pWnd) pWnd->EnableWindow(FALSE);

		pWnd = GetDlgItem(IDC_edtCHECKIN_VOL_YEAR);
		if(pWnd) pWnd->EnableWindow(FALSE);

	}
	else
	{
		pWnd = GetDlgItem(IDC_btnCHECKIN_VOL_PREV);
		if(pWnd) pWnd->EnableWindow(TRUE);

		pWnd = GetDlgItem(IDC_btnCHECKIN_VOL_NEXT);
		if(pWnd) pWnd->EnableWindow(TRUE);

		pWnd = GetDlgItem(IDC_edtCHECKIN_VOL_YEAR);
		if(pWnd) pWnd->EnableWindow(TRUE);
	}
	

}

BOOL CCheckinVolESL::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message == WM_KEYDOWN )
	{
		if( pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN )	return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CCheckinVolESL::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
