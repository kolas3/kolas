// BO_CAT_PROC_0500_09.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "BO_CAT_PROC_0500_09.h"
#include "BO_CAT_PROC_0510.h"
#include "..\..\..\공통\BO_VOLUME_FORM\BOVolumeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0500_09 dialog


CBO_CAT_PROC_0500_09::CBO_CAT_PROC_0500_09(CESL_Mgr* pParent)// , CString sRecKey/*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	m_bInitDlg = TRUE;
	AfxInitRichEdit();
	m_nDividType = -1;

	//{{AFX_DATA_INIT(CBO_CAT_PROC_0500_09)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBO_CAT_PROC_0500_09::~CBO_CAT_PROC_0500_09()
{
}

VOID CBO_CAT_PROC_0500_09::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_PROC_0500_09)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_PROC_0500_09, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_PROC_0500_09)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_bVOL_INFO_INPUT, OnbVOLINFOINPUT)
	ON_BN_CLICKED(IDC_bPREV, OnbPREV)
	ON_BN_CLICKED(IDC_bNEXT, OnbNEXT)
	ON_BN_CLICKED(IDC_bUPDATE, OnbUPDATE)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0500_09 message handlers

BOOL CBO_CAT_PROC_0500_09::Create(CWnd* pParentWnd)
{
EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

BOOL CBO_CAT_PROC_0500_09::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("BO_CAT_PROC_0500")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	if (m_strParentCMAlias.IsEmpty())
		m_strParentCMAlias = _T("CM_BO_CAT_PROC_0000");
	if (m_strParentDMAlias.IsEmpty())
		m_strParentDMAlias = _T("DM_BO_CAT_PROC_0000");
	if (m_strParentGridAlias.IsEmpty())
		m_strParentGridAlias = _T("ProcGrid");

	INT ids = SelectMakeList(m_strParentCMAlias, m_strParentGridAlias);
	if ( 0 != ids ) {
		AfxMessageBox ( _T("SelectMakeList error") );
	}
	INT idx = SelectGetHeadPosition(m_strParentCMAlias, m_strParentGridAlias);
	if (idx < 0)
	{
		AfxMessageBox(_T("선정된 자료가 없습니다."));
		EndDialog(IDCANCEL);
		m_bInitDlg = FALSE;
		return -1;
	}

	m_SelectIdx = idx;

	m_marcEditor.SubclassDlgItem(IDC_redtMain, -1, this);
	m_marcEditor_obj.SubclassDlgItem(IDC_redtOjbect, -1, this);

	CESL_DataMgr *pDM;
	pDM = FindDM(m_strParentDMAlias);
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_BO_CAT_PROC_0000 error") );
		return FALSE;
	}
	pDM->SetMarcMgr(m_pInfo->pMarcMgr);
	m_pInfo->pMarcMgr->SetMarcGroup(_T("통합마크"));

	m_strStreamMarc = pDM->GetCellData( _T("BS_MARC") , idx );
	m_SelectedSRecKey = pDM->GetCellData( _T("BS_종KEY") , idx );

	CString sRegNo;
	sRegNo = pDM->GetCellData( _T("시작등록번호") , idx );
	if ( sRegNo == _T("") ) {
		AfxMessageBox ( _T("등록번호를 부여해야 합니다.") );
		m_bInitDlg = FALSE;
		return FALSE;
	}

	sRegNo = pDM->GetCellData( _T("BS_복본여부") , idx );
	if ( sRegNo == _T("V") ) {
		AfxMessageBox ( _T("복본자료는 종분리를 할 수 없습니다.") );
		m_bInitDlg = FALSE;
		CDialog::OnCancel();
		return FALSE;
	}
	
	m_strWorkingStatus = pDM->GetCellData( _T("IBS_작업상태INFO"), idx );

	if ( m_strWorkingStatus.GetLength() > 3 ) {
		if ( m_strWorkingStatus == _T("BOC113N") ) {
			m_strWorkingStatus = _T("정리대상자료");
		}
		else if ( m_strWorkingStatus.Left(3) == _T("BOL") ){
			m_strWorkingStatus = _T("소장자료");
		}
	}
	SetControlData( _T("CM_BO_CAT_PROC_0500"), _T("자료상태"), m_strWorkingStatus );

	CString strTagCode, strErrMsg, strHelpMsg;
	
	if(m_pInfo->pMarcMgr->Decoding(m_strStreamMarc, &m_marc))
	{
		POSITION pos;
		pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
		while (pos)
		{
			m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);
			UpdateData(FALSE);
		}	
	}
	m_marcEditor.Init(m_pInfo->pMarcMgr, &m_marc);
	m_marcEditor.Display();	
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;
}

VOID CBO_CAT_PROC_0500_09::OnCancel() 
{
EFS_BEGIN

	EndDialog(IDCANCEL);

EFS_END
}

VOID CBO_CAT_PROC_0500_09::OnbVOLINFOINPUT() 
{
EFS_BEGIN

	CBO_CAT_PROC_0510 dlg( this , m_SelectedSRecKey );
	dlg.m_nDividType = m_nDividType;
	if( IDOK == dlg.DoModal() ) 
	{
		GetDlgItem(IDC_bSEPARATION)->EnableWindow(true);
		m_nDividType = dlg.m_nDividType;
		if ( m_nDividType == 0 ) 
		{
			ExeSEPARATION();
		}
		else
		{
			ExeMultiVolSEPARATION();
		}

	}
	else if ( m_nStart ) 
	{
		CDialog::OnClose();
	}

EFS_END
}

INT CBO_CAT_PROC_0500_09::DmRowCopy(CESL_DataMgr *pSourceDM, INT SourceDM_ROW ,CESL_DataMgr *pTargetDM)
{
EFS_BEGIN

	INT ids;
	CString s_Alias,s_Data,s_fName,s_fType,s_dType;
	INT nCol = pSourceDM->RefList.GetCount();
	
	pTargetDM->InsertRow(-1);

	INT TargetDM_ROW = pTargetDM->GetRowCount()-1;

	for( INT i=0 ; i< nCol ; i++)
	{
		ids =pSourceDM->FindColumnFieldName( i , s_Alias , s_fName ,s_fType,s_dType,SourceDM_ROW,s_Data);
		if( !ids )
		{
			pTargetDM->SetCellData(s_Alias,s_Data,TargetDM_ROW);
			s_Data=_T("");
		}
	}

	return 0;

EFS_END
return -1;

}

VOID CBO_CAT_PROC_0500_09::OnOK() 
{
EFS_BEGIN

	if ( m_nDividType == 1)
	{
		OnVolumnSave();
		return;
	}
	CESL_DataMgr *pDM_IDX;
	CESL_DataMgr *pDM_IDX_COPY;
	CESL_DataMgr *pDM_0510_OBJECT;
	CESL_DataMgr *pDM_0510;

	CESL_DataMgr *pDM_TOC;
	CESL_DataMgr *pDM_TOC_IDX;

	CString sControlNo;
	CString sRecKeyDiv;
	CString s_Alias,s_Data,s_fName,s_fType,s_dType;
	pDM = FindDM(_T("DM_BO_CAT_PROC_0510_SPECIES"));
	if (!pDM) return;

	pDM_TOC = FindDM(_T("DM_목차초록_분리"));
	if( !pDM_TOC )
	{
		AfxMessageBox(_T("DM_목차초록_분리 ERROR"));
		return;
	}
	
	pDM_TOC_IDX = FindDM(_T("DM_목차초록_색인"));
	if( !pDM_TOC_IDX )
	{
		AfxMessageBox(_T("DM_목차초록_색인 ERROR"));
		return;
	}


	CString strWhere = _T("REC_KEY = ") + m_SelectedSRecKey;
	if (pDM->RefreshDataManager(strWhere) < 0)	return;

	
	pDM_COPY = FindDM(_T("DM_BO_CAT_PROC_0510_SPECIES_COPY"));
	if (!pDM) return;
	INT idx = DmRowCopy(pDM, 0 ,pDM_COPY);

	INT ids = pDM->MakeRecKey( sRecKeyDiv );
	if ( 0 > ids ) return;
	ids = pDM->StartFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("StartFrame 를 확인하세요"));
		return;
	}

	pDM->InitDBFieldData();
	pDM->AddDBFieldData(_T("REC_KEY"), _T("STRING"), sRecKeyDiv);

	INT nCol = pDM_COPY->RefList.GetCount();

	for( INT i = 0 ; i< nCol ; i++)
	{
		INT ids =pDM_COPY->FindColumnFieldName( i , s_Alias , s_fName ,s_fType,s_dType,0,s_Data);
		if( !ids )
		{
			s_Data=_T("");
			pDM_COPY->GetCellData(s_Alias,0,s_Data);			
			if ( s_Alias == _T("BS_제어번호") ) {
				CString sControlNo;
				m_pInfo->pMarcMgr->GetItem( &m_marc_obj, _T("001*"), sControlNo );
				s_Data = sControlNo;
			}
			if ( s_Alias != _T("BS_MARC")		&& 
				 s_Alias != _T("BS_종KEY")		&& 
				 s_Alias != _T("BS_SET_ISBN")	&&
				 s_Alias != _T("BS_정리상태")	) 
			{
				pDM->AddDBFieldData(s_fName, _T("STRING"), s_Data);
			}		
			
			
			if ( s_Alias == _T("BS_정리상태") )
			{				
				pDM->AddDBFieldData(s_fName, _T("STRING"), _T("1"));				
			}
		}
	}

	pDM->AddDBFieldData(_T("APPLY_YN"), _T("STRING"), _T("Y"));	
	
	ids = pDM->MakeInsertFrame(_T("BO_SPECIES_TBL"));
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("MakeInsertFrame 를 확인하세요"));
		return;
	}

	m_pInfo->pMarcMgr->GetItem( &m_marc_obj, _T("001*"), sControlNo );
	if ( sControlNo != _T("") ) {
		CString strQuery;
		CString strItem;
		strQuery.Format(_T("SELECT CONTROL_NO FROM BO_SPECIES_TBL WHERE CONTROL_NO = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE"), sControlNo);
		ids = pDM->GetOneValueQuery(strQuery, strItem);
		if ( strItem != _T("") ) {
			AfxMessageBox( _T("제어번호가 중복됩니다. 마지막번호를 수정하고 다시 부여하세요.") );
			return;
		}
		INT iNum = _ttoi ( sControlNo.Right(5) );
		CString strYear = sControlNo.Mid(3,4);
		CString strControlCode = sControlNo.Left(3);
		if ( iNum == 1 ) {
			CString strReckey;
			ids = pDM->MakeRecKey(strReckey);		
			if (ids < 0) return;		
			strQuery.Format(_T("INSERT INTO CO_LAST_NUMBER_TBL (REC_KEY, PUBLISH_FORM_CODE, KIND_CODE, DIVISION_VALUE, MAKE_YEAR, LAST_NUMBER, FIRST_WORK, LAST_WORK, MANAGE_CODE) "
				_T("VALUES ('%s','CO','CON_NO','%s','%s','%s', '%s', '%s', UDF_MANAGE_CODE);"))
				, strReckey, strControlCode, strYear, sControlNo.Right(5), GetWorkLogMsg(_T("종분리"),WORK_LOG), GetWorkLogMsg(_T("종분리"),WORK_LOG));			
		}
		else {
			strQuery.Format(_T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER = %s, LAST_WORK = '%s' WHERE DIVISION_VALUE = '%s' AND MAKE_YEAR = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE;"), 
				sControlNo.Right(5), GetWorkLogMsg(_T("종분리"),WORK_LOG), strControlCode, strYear);			
		}
		ids = pDM->AddFrame(strQuery);
		if ( ids < 0 ) {
			AfxMessageBox( _T("종분리 마지막번호 업데이트 AddFrame 확인") );
			return;
		}
	}

	pDM_IDX = FindDM(_T("DM_BO_CAT_PROC_0510_IDX"));
	if (!pDM_IDX) return;

	if (pDM_IDX->RefreshDataManager(strWhere) < 0)	return;
	
	pDM_IDX_COPY = FindDM(_T("DM_BO_CAT_PROC_0510_IDX_COPY"));
	if (!pDM) return;
	DmRowCopy(pDM_IDX, 0 ,pDM_IDX_COPY);

	pDM_0510 = FindDM(_T("DM_BO_CAT_PROC_0510"));
	pDM_0510_OBJECT = FindDM(_T("DM_BO_CAT_PROC_0510_OBJECT"));
	INT iObjCount = pDM_0510_OBJECT->GetRowCount();

	MakeIndexClassifyCodeConcat(pDM_IDX_COPY, pDM_0510_OBJECT);

	pDM->InitDBFieldData();
	pDM->AddDBFieldData(_T("REC_KEY"), _T("STRING"), sRecKeyDiv);

	nCol = pDM_IDX_COPY->RefList.GetCount();

	for( i=0 ; i< nCol ; i++)
	{
		INT ids =pDM_IDX_COPY->FindColumnFieldName( i , s_Alias , s_fName ,s_fType,s_dType,0,s_Data);
		if( !ids )
		{
			s_Data=_T("");
			pDM_IDX_COPY->GetCellData(s_Alias,0,s_Data);

			if( s_Alias == _T("IBS_제어번호") )
			{
				CString sControlNo;
				m_pInfo->pMarcMgr->GetItem( &m_marc_obj, _T("001*"), sControlNo );
				s_Data = sControlNo;
			}
			if( s_Alias != _T("BS_MARC") && s_Alias != _T("IBS_REC_KEY") )
			{
				pDM->AddDBFieldData(s_fName, _T("STRING"), s_Data);
			}
		}
	}
	
	ids = pDM->MakeInsertFrame(_T("IDX_BO_TBL"));
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("MakeInsertFrame 를 확인하세요"));
		return;
	}
	
	MakeIndexClassifyCodeConcat(pDM_IDX,pDM_0510);
	
	pDM->InitDBFieldData();
	pDM->AddDBFieldData(_T("REC_KEY"), _T("STRING"), m_SelectedSRecKey);

	nCol = pDM_IDX->RefList.GetCount();

	for( i=0 ; i< nCol ; i++)
	{
		INT ids =pDM_IDX->FindColumnFieldName( i , s_Alias , s_fName ,s_fType,s_dType,0,s_Data);
		if( !ids )
		{
			s_Data=_T("");
			pDM_IDX->GetCellData(s_Alias,0,s_Data);
			if( s_Alias == _T("IBS_이용제한구분_코드") || 
				s_Alias == _T("IBS_등록구분_코드") || 
				s_Alias == _T("IBS_관리구분_코드") || 
				s_Alias == _T("IBS_청구_별치기호_코드") ) 
			{
				pDM->AddDBFieldData(s_fName, _T("STRING"), s_Data);
			}
		}
	}
		
	ids = pDM->MakeUpdateFrame( _T("IDX_BO_TBL"), _T("REC_KEY"), _T("NUMERIC"), m_SelectedSRecKey );
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("MakeInsertFrame 를 확인하세요"));
		return;
	}

	INT iCh;

	CString bRepresentYn = _T("N");
	for ( i = 1 ; i <= iObjCount ; i ++ ) 
	{
		CString sItem;
		pDM_0510_OBJECT->GetCellData( _T("BB_대표책여부") , i-1 , sItem );
		if ( sItem == _T("Y") ) 
		{
			bRepresentYn = _T("Y");
			break;
		}
	}

	CString strUseLimitCode;
	for ( i = 1 ; i <= iObjCount ; i ++ ) 
	{
		CString sItem, sStatus, sRepresentBook;
		pDM_0510_OBJECT->GetCellData( _T("BB_책KEY") , i-1 , sItem );
		pDM_0510_OBJECT->GetCellData( _T("BB_자료상태") , i-1 , sStatus );
		pDM_0510_OBJECT->GetCellData( _T("BB_대표책여부") , i-1 , sRepresentBook );
		CString sQuery, sIdxUpdateQuery;
		
		if ( i == 1 && bRepresentYn == _T("N") ) 
		{
			sQuery.Format( _T("UPDATE BO_BOOK_TBL SET REPRESENT_BOOK_YN = 'Y', SPECIES_KEY = %s, LAST_WORK='%s' WHERE REC_KEY = '%s';") , sRecKeyDiv , GetWorkLogMsg(_T("종분리"),WORK_LOG), sItem );
			sIdxUpdateQuery.Format( _T("UPDATE IDX_BO_TBL SET WORKING_STATUS = '%s' WHERE REC_KEY = %s;"),sStatus,sRecKeyDiv);
			 pDM_0510_OBJECT->GetCellData(_T("BB_이용제한구분_코드"), i-1, strUseLimitCode );				
		}
		else 
		{
			if( sRepresentBook == _T("Y") )
			{
				sIdxUpdateQuery.Format( _T("UPDATE IDX_BO_TBL SET WORKING_STATUS = '%s' WHERE REC_KEY = %s;"),sStatus,sRecKeyDiv);				
				strUseLimitCode = pDM_0510_OBJECT->GetCellData(_T("BB_이용제한구분_코드"), i-1 );	
			}
			sQuery.Format( _T("UPDATE BO_BOOK_TBL SET SPECIES_KEY = %s, LAST_WORK='%s' WHERE REC_KEY = '%s';") , sRecKeyDiv , GetWorkLogMsg(_T("종분리"),WORK_LOG), sItem );
		}
		iCh = pDM->AddFrame( sIdxUpdateQuery );
		iCh = pDM->AddFrame( sQuery );
		sQuery.Format(_T("UPDATE LS_WORK_T01 SET SPECIES_KEY = %s WHERE BOOK_KEY = %s;"), sRecKeyDiv, sItem);
		iCh = pDM->AddFrame( sQuery );
		
		if ( 0 > iCh ) return;
		sQuery.Format(_T("UPDATE CO_LOAN_TBL SET SPECIES_KEY = %s WHERE BOOK_KEY = %s;"), sRecKeyDiv, sItem);
		iCh = pDM->AddFrame( sQuery );
		if ( 0 > iCh ) return;

		CString strValue;
		sQuery.Format(_T("SELECT MIN(VALUE_1) FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='책두레시스템' AND VALUE_NAME='책두레사용여부' AND MANAGE_CODE = '%s'"),m_pInfo->MANAGE_CODE);
		iCh = pDM->GetOneValueQuery(sQuery,strValue);
		if ( 0 > iCh ) return;
		if (strValue.Compare(_T("Y"))==0 )
		{
			sQuery.Format(_T("UPDATE ILL_TRANS_TBL SET SPECIES_KEY = %s WHERE BOOK_KEY = %s;"), sRecKeyDiv, sItem);
			iCh = pDM->AddFrame( sQuery );
			if ( 0 > iCh ) return;
		}
	}

	if ( bRepresentYn == _T("Y") ) 
	{
		CESL_DataMgr *pDM_0510;
		pDM_0510 = FindDM(_T("DM_BO_CAT_PROC_0510"));
		CString sMainRecKey = pDM_0510->GetCellData( _T("BB_책KEY") , 0 );
		CString sQuery;
		sQuery.Format( _T("UPDATE BO_BOOK_TBL SET REPRESENT_BOOK_YN = 'Y', LAST_WORK='%s' WHERE REC_KEY = '%s';") ,GetWorkLogMsg(_T("종분리"),WORK_LOG), sMainRecKey );
		iCh = pDM->AddFrame( sQuery );
		if ( 0 > iCh ) return;
	}
	m_pInfo->pMarcMgr->Encoding(&m_marc, m_strStreamMarc);
	m_pInfo->pMarcMgr->Encoding(&m_marc_obj, m_strStreamMarc_obj);

	pDM->InitDBFieldData();
	pDM->AddDBFieldData( _T("MARC"), _T("STRING"), m_strStreamMarc_obj, 1 );
	pDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), GetWorkLogMsg(_T("종분리"), WORK_LOG), 1 );	
	pDM->AddDBFieldData( _T("USE_LIMIT_CODE"), _T("STRING"), strUseLimitCode, 1 );
	INT idsTmp = pDM->MakeUpdateFrame( _T("BO_SPECIES_TBL"), _T("REC_KEY"), _T("NUMERIC"), sRecKeyDiv );
	if ( 0 > idsTmp ) return;

	pDM->InitDBFieldData();
	pDM->AddDBFieldData( _T("MARC"), _T("STRING"), m_strStreamMarc, 1 );
	pDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), GetWorkLogMsg(_T("종분리"), WORK_LOG), 1 );	
	idsTmp = pDM->MakeUpdateFrame( _T("BO_SPECIES_TBL"), _T("REC_KEY"), _T("NUMERIC"), m_SelectedSRecKey );
	if ( 0 > idsTmp ) return;

	if(m_pInfo->GetSearchIndexType() == ECO_SEARCH)
	{
		CString strJJSQuery;
		strJJSQuery.Format(_T("ECO$P_MAKE_BO$I(%s);"), m_SelectedSRecKey);
		iCh = pDM->AddFrame(strJJSQuery);
		if ( 0 > iCh ) return;
		strJJSQuery.Format(_T("ECO$P_MAKE_BO$I(%s);"), sRecKeyDiv);
		iCh = pDM->AddFrame(strJJSQuery);
		if ( 0 > iCh ) return;	
	}
	else
	{
		
	}

	CString sWhereCondition;
	INT nTocCnt = 0;

	sWhereCondition.Format(_T(" toc.SPECIES_KEY = %s "), m_SelectedSRecKey);
	pDM_TOC->EXTRA_CONDITION = _T(" order by toc.vol_sort_no ");
	pDM_TOC->RefreshDataManager(sWhereCondition);
	
	nTocCnt = pDM_TOC->GetRowCount();

	BOOL bTOC_YN = FALSE; 
	BOOL bABS_YN = FALSE;

	if( nTocCnt > 0 )
	{
		CString sControlNoBefore, sControlNoNow;
		CString sTocKey = _T("");
		CString sAbsKey = _T("");
		CString sConKey = _T("");
		CString sTemp = _T("");

		m_pInfo->pMarcMgr->GetItem( &m_marc_obj, _T("001*"), sControlNoNow );
		m_pInfo->pMarcMgr->GetItem( &m_marc, _T("001*"), sControlNoBefore );		
		
		sWhereCondition.Format(_T(" SPECIES_KEY = %s "), m_SelectedSRecKey);
		pDM_TOC_IDX->RefreshDataManager(sWhereCondition);

		CString str90;
		pDM->GetOneValueQuery(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS = '국방대' AND MANAGE_CODE = UDF_MANAGE_CODE"),str90);

		if( str90 == _T("Y") )
		{
			CArray <CString, CString> VolSortListArrayDiv;
			CArray <CString, CString> VolSortListArrayMain;	
			
			for( INT k = 0 ; k< pDM_TOC->GetRowCount() ; k++ )
			{
				CString str, strVal, strKey;
				pDM_TOC->GetCellData(_T("권호일련번호"),k,str);
				pDM_TOC->GetCellData(_T("레코드키"),k,strKey);
				for ( INT j = 0; j< pDM_0510_OBJECT->GetRowCount(); j++ )
				{
					pDM_0510_OBJECT->GetCellData(_T("BB_권일련번호"),j,strVal);
					if( str == strVal )
					{
						INT nIdx;
						for(INT x = 0;  x < pDM_TOC_IDX->GetRowCount(); x++ )
						{
							if( pDM_TOC_IDX->GetCellData(_T("REC_KEY"),x) == strKey )
							{ nIdx = x; break;}
						}
						
						pDM->MakeRecKey(sTocKey);

						if( pDM_TOC_IDX->GetCellData(_T("ABS_YN"),nIdx) == _T("Y") )
						{
							pDM->InitDBFieldData();
							pDM->MakeRecKey(sAbsKey);
							pDM->AddDBFieldData(_T("REC_KEY"), _T("STRING"), sAbsKey);
							pDM->AddDBFieldData(_T("TOC_COMMON_KEY"),_T("STRING"), sTocKey);

							nCol = pDM_TOC->RefList.GetCount();

							for( i=0 ; i< nCol ; i++)
							{
								INT ids =pDM_TOC->FindColumnFieldName( i , s_Alias , s_fName ,s_fType,s_dType,k,s_Data);
								if( !ids )
								{
									s_Data=_T("");
									pDM_TOC->GetCellData(s_Alias,k,s_Data);
									if( s_Alias == _T("초록") || 
										s_Alias == _T("초록생성일") || 
										s_Alias == _T("초록수정일") || 
										s_Alias == _T("초록_이용자구분") ) 
									{
										sTemp = s_fName.Mid(2);
										pDM->AddDBFieldData(sTemp, _T("STRING"), s_Data);
									}
								}
							}
							pDM->MakeInsertFrame(_T("BO_ABSTRACTS_TBL"));
							bABS_YN = TRUE;
						}
						
						if( pDM_TOC_IDX->GetCellData(_T("TOC_YN"),0) == _T("Y") )
						{
							pDM->InitDBFieldData();
							pDM->MakeRecKey(sConKey);
							pDM->AddDBFieldData(_T("REC_KEY"), _T("STRING"), sConKey);
							pDM->AddDBFieldData(_T("TOC_COMMON_KEY"),_T("STRING"), sTocKey);

							nCol = pDM_TOC->RefList.GetCount();

							for( i=0 ; i< nCol ; i++)
							{
								INT ids =pDM_TOC->FindColumnFieldName( i , s_Alias , s_fName ,s_fType,s_dType,k,s_Data);
								if( !ids )
								{
									s_Data=_T("");
									pDM_TOC->GetCellData(s_Alias,k,s_Data);
									if( s_Alias == _T("목차생성일") || 
										s_Alias == _T("목차수정일") || 
										s_Alias == _T("목차_이용자구분") ) 
									{
										sTemp = s_fName.Mid(2);
										pDM->AddDBFieldData(sTemp, _T("STRING"), s_Data);
									}
									if( s_Alias == _T("목차") )
									{
										sTemp = s_fName.Mid(2);
										s_Data.Replace(sControlNoBefore, sControlNoNow);
										pDM->AddDBFieldData(sTemp, _T("STRING"), s_Data);
									}
								}
							}
							pDM->MakeInsertFrame(_T("BO_CONTENTS_TBL"));
							bTOC_YN = TRUE;
						}

						pDM->InitDBFieldData();
						pDM->AddDBFieldData(_T("REC_KEY"), _T("STRING"), sTocKey);
						pDM->AddDBFieldData(_T("ABSTRACTS_KEY"), _T("STRING"), sAbsKey);
						pDM->AddDBFieldData(_T("CONTENTS_KEY"), _T("STRING"), sConKey);
						pDM->AddDBFieldData(_T("SPECIES_KEY"), _T("STRING"), sRecKeyDiv);
						
						nCol = pDM_TOC->RefList.GetCount();

						for( i=0 ; i< nCol ; i++)
						{
							INT ids =pDM_TOC->FindColumnFieldName( i , s_Alias , s_fName ,s_fType,s_dType,k,s_Data);
							if( !ids )
							{
								s_Data=_T("");
								pDM_TOC->GetCellData(s_Alias,k,s_Data);
								if( s_Alias == _T("종구분") || 
									s_Alias == _T("권호일련번호") ) 
								{
									sTemp = s_fName.Mid(4);
									pDM->AddDBFieldData(sTemp, _T("STRING"), s_Data);
								}
								if( s_Alias == _T("제어번호") )
								{
									sTemp = s_fName.Mid(4);
									pDM->AddDBFieldData(sTemp, _T("STRING"), sControlNoNow);
								}
							}
						}
						pDM->AddDBFieldData(_T("MANAGE_CODE"), _T("STRING"), _T("UDF_MANAGE_CODE") );

						pDM->MakeInsertFrame(_T("BO_TOC_COMMON_TBL"));

						pDM->InitDBFieldData();
						pDM->AddDBFieldData(_T("REC_KEY"), _T("STRING"), sTocKey);
						pDM->AddDBFieldData(_T("ABSTRACTS_KEY"), _T("STRING"), sAbsKey);
						pDM->AddDBFieldData(_T("CONTENTS_KEY"), _T("STRING"), sConKey);
						pDM->AddDBFieldData(_T("SPECIES_KEY"), _T("STRING"), sRecKeyDiv);
						
						nCol = pDM_TOC_IDX->RefList.GetCount();

						for( i=0 ; i< nCol ; i++)
						{
							INT ids =pDM_TOC_IDX->FindColumnFieldName( i , s_Alias , s_fName ,s_fType,s_dType,nIdx,s_Data);
							if( !ids )
							{
								s_Data=_T("");
								pDM_TOC_IDX->GetCellData(s_Alias,nIdx,s_Data);
								if( s_Alias == _T("ABS_CREATE_DATE") || 
									s_Alias == _T("ABS_UPDATE_DATE") || 
									s_Alias == _T("ABS_USE_LIMIT_CODE") ||
									s_Alias == _T("ABS_YN") ||
									s_Alias == _T("AUTHOR_INFO") ||
									s_Alias == _T("BOOK_CODE") ||
									s_Alias == _T("CLASS_NO") ||
									s_Alias == _T("IDX_ABS_KEYWORD") ||
									s_Alias == _T("IDX_TOC_KEYWORD") ||
									s_Alias == _T("PLACE_INFO") ||
									s_Alias == _T("PUB_INFO") ||
									s_Alias == _T("PUB_YEAR_INFO") ||
									s_Alias == _T("SE_SHELF_CODE") ||
									s_Alias == _T("SPECIES_CLASS") ||
									s_Alias == _T("TITLE_INFO") ||
									s_Alias == _T("TOC_CREATE_DATE") ||
									s_Alias == _T("TOC_UPDATE_DATE") ||
									s_Alias == _T("TOC_USE_LIMIT_CODE") ||
									s_Alias == _T("TOC_YN") ||
									s_Alias == _T("VOL_CODE_DISP") 		
									) 
								{
									pDM->AddDBFieldData(s_fName, _T("STRING"), s_Data);
								}
							}
						}
						pDM->MakeInsertFrame(_T("IDX_BO_TOC_TBL"));
					}
				}
			}
			
			
		}
		else
		{
			pDM->MakeRecKey(sTocKey);

			if( pDM_TOC_IDX->GetCellData(_T("ABS_YN"),0) == _T("Y") )
			{
				pDM->InitDBFieldData();
				pDM->MakeRecKey(sAbsKey);
				pDM->AddDBFieldData(_T("REC_KEY"), _T("STRING"), sAbsKey);
				pDM->AddDBFieldData(_T("TOC_COMMON_KEY"),_T("STRING"), sTocKey);

				nCol = pDM_TOC->RefList.GetCount();

				for( i=0 ; i< nCol ; i++)
				{
					INT ids =pDM_TOC->FindColumnFieldName( i , s_Alias , s_fName ,s_fType,s_dType,0,s_Data);
					if( !ids )
					{
						s_Data=_T("");
						pDM_TOC->GetCellData(s_Alias,0,s_Data);
						if( s_Alias == _T("초록") || 
							s_Alias == _T("초록생성일") || 
							s_Alias == _T("초록수정일") || 
							s_Alias == _T("초록_이용자구분") ) 
						{
							sTemp = s_fName.Mid(2);
							pDM->AddDBFieldData(sTemp, _T("STRING"), s_Data);
						}
					}
				}
				pDM->MakeInsertFrame(_T("BO_ABSTRACTS_TBL"));
				bABS_YN = TRUE;
			}
			if( pDM_TOC_IDX->GetCellData(_T("TOC_YN"),0) == _T("Y") )
			{
				pDM->InitDBFieldData();
				pDM->MakeRecKey(sConKey);
				pDM->AddDBFieldData(_T("REC_KEY"), _T("STRING"), sConKey);
				pDM->AddDBFieldData(_T("TOC_COMMON_KEY"),_T("STRING"), sTocKey);

				nCol = pDM_TOC->RefList.GetCount();

				for( i=0 ; i< nCol ; i++)
				{
					INT ids =pDM_TOC->FindColumnFieldName( i , s_Alias , s_fName ,s_fType,s_dType,0,s_Data);
					if( !ids )
					{
						s_Data=_T("");
						pDM_TOC->GetCellData(s_Alias,0,s_Data);
						if( s_Alias == _T("목차생성일") || 
							s_Alias == _T("목차수정일") || 
							s_Alias == _T("목차_이용자구분") ) 
						{
							sTemp = s_fName.Mid(2);
							pDM->AddDBFieldData(sTemp, _T("STRING"), s_Data);
						}
						if( s_Alias == _T("목차") )
						{
							sTemp = s_fName.Mid(2);
							s_Data.Replace(sControlNoBefore, sControlNoNow);
							pDM->AddDBFieldData(sTemp, _T("STRING"), s_Data);
						}
					}
				}
				pDM->MakeInsertFrame(_T("BO_CONTENTS_TBL"));
				bTOC_YN = TRUE;
			}

			pDM->InitDBFieldData();
			pDM->AddDBFieldData(_T("REC_KEY"), _T("STRING"), sTocKey);
			pDM->AddDBFieldData(_T("ABSTRACTS_KEY"), _T("STRING"), sAbsKey);
			pDM->AddDBFieldData(_T("CONTENTS_KEY"), _T("STRING"), sConKey);
			pDM->AddDBFieldData(_T("SPECIES_KEY"), _T("STRING"), sRecKeyDiv);
			
			nCol = pDM_TOC->RefList.GetCount();

			for( i=0 ; i< nCol ; i++)
			{
				INT ids =pDM_TOC->FindColumnFieldName( i , s_Alias , s_fName ,s_fType,s_dType,0,s_Data);
				if( !ids )
				{
					s_Data=_T("");
					pDM_TOC->GetCellData(s_Alias,0,s_Data);
					if( s_Alias == _T("종구분") || 
						s_Alias == _T("권호일련번호") ) 
					{
						sTemp = s_fName.Mid(4);
						pDM->AddDBFieldData(sTemp, _T("STRING"), s_Data);
					}
					if( s_Alias == _T("제어번호") )
					{
						sTemp = s_fName.Mid(4);
						pDM->AddDBFieldData(sTemp, _T("STRING"), sControlNoNow);
					}
				}
			}

			pDM->AddDBFieldData(_T("MANAGE_CODE"), _T("STRING"), _T("UDF_MANAGE_CODE") );

			pDM->MakeInsertFrame(_T("BO_TOC_COMMON_TBL"));

			pDM->InitDBFieldData();
			pDM->AddDBFieldData(_T("REC_KEY"), _T("STRING"), sTocKey);
			pDM->AddDBFieldData(_T("ABSTRACTS_KEY"), _T("STRING"), sAbsKey);
			pDM->AddDBFieldData(_T("CONTENTS_KEY"), _T("STRING"), sConKey);
			pDM->AddDBFieldData(_T("SPECIES_KEY"), _T("STRING"), sRecKeyDiv);
			
			nCol = pDM_TOC_IDX->RefList.GetCount();

			for( i=0 ; i< nCol ; i++)
			{
				INT ids =pDM_TOC_IDX->FindColumnFieldName( i , s_Alias , s_fName ,s_fType,s_dType,0,s_Data);
				if( !ids )
				{
					s_Data=_T("");
					pDM_TOC_IDX->GetCellData(s_Alias,0,s_Data);
					if( s_Alias == _T("ABS_CREATE_DATE") || 
						s_Alias == _T("ABS_UPDATE_DATE") || 
						s_Alias == _T("ABS_USE_LIMIT_CODE") ||
						s_Alias == _T("ABS_YN") ||
						s_Alias == _T("AUTHOR_INFO") ||
						s_Alias == _T("BOOK_CODE") ||
						s_Alias == _T("CLASS_NO") ||
						s_Alias == _T("IDX_ABS_KEYWORD") ||
						s_Alias == _T("IDX_TOC_KEYWORD") ||
						s_Alias == _T("PLACE_INFO") ||
						s_Alias == _T("PUB_INFO") ||
						s_Alias == _T("PUB_YEAR_INFO") ||
						s_Alias == _T("SE_SHELF_CODE") ||
						s_Alias == _T("SPECIES_CLASS") ||
						s_Alias == _T("TITLE_INFO") ||
						s_Alias == _T("TOC_CREATE_DATE") ||
						s_Alias == _T("TOC_UPDATE_DATE") ||
						s_Alias == _T("TOC_USE_LIMIT_CODE") ||
						s_Alias == _T("TOC_YN") ||
						s_Alias == _T("VOL_CODE_DISP") 		
						) 
					{
						pDM->AddDBFieldData(s_fName, _T("STRING"), s_Data);
					}
				}
			}
			pDM->MakeInsertFrame(_T("IDX_BO_TOC_TBL"));
		}
	}
	
	if( bABS_YN )
	{
		CString strQuery;
		strQuery.Format(_T("UPDATE IDX_BO_TBL SET ABSTRACTS_YN = 'Y' WHERE REC_KEY = %s;"),sRecKeyDiv);
		pDM->AddFrame(strQuery);
	}

	if( bTOC_YN )
	{
		CString strQuery;
		strQuery.Format(_T("UPDATE IDX_BO_TBL SET CONTENTS_YN = 'Y' WHERE REC_KEY = %s;"),sRecKeyDiv);
		pDM->AddFrame(strQuery);
	}

	iCh = pDM->SendFrame();
	if ( iCh < 0 ) 
	{
		AfxMessageBox( _T("분리저장 확인") );
		return;
	}
	else
	{
		iCh = pDM->EndFrame();
		if ( 0 > iCh ) 
			return;
		else
		{
			pDM->StartFrame();
			pDM->InitDBFieldData();
			
			CString strQuery;
			strQuery = UpdateIndexQuery(m_SelectedSRecKey);
			if( strQuery != _T("") )
				ids = pDM->AddFrame(strQuery);
			strQuery = UpdateIndexQuery(sRecKeyDiv);
			if( strQuery != _T("") )
				ids = pDM->AddFrame(strQuery);
			pDM->SendFrame();
			pDM->EndFrame();
		}
	}

	AfxMessageBox(_T("자료를 분리하였습니다."));

	CESL_DataMgr *pDM_PROC_0000;
	pDM_PROC_0000 = FindDM(m_strParentDMAlias);
	if ( pDM_PROC_0000 == NULL ) {
		AfxMessageBox ( _T("DM_BO_CAT_PROC_0000 error") );
		return;
	}

	CESL_DataMgr *pWhere;
	pWhere = FindDM( _T("DM_BO_CAT_PROC_WHERE") );
	if ( pWhere == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_PROC_WHERE ERROR") );
		return;
	}
	CString sWhere;
	pWhere->GetCellData( _T("조건절"), 0, sWhere );

	if (pDM_PROC_0000->RefreshDataManager(sWhere) < 0)	return;

	pDM_PROC_0000->SORT( _T("시작등록번호"), FALSE );

	CESL_ControlMgr *pCM;
	pCM = FindCM(m_strParentCMAlias);
	if ( pCM == NULL ) {
		AfxMessageBox ( _T("CM_BO_CAT_PROC_0000 error") );
		return;
	}
	for ( i = 0 ; i < pDM_PROC_0000->GetRowCount() ; i++ ) {
		CString sCatalogStatus = pDM_PROC_0000->GetCellData( _T("BS_정리상태"), i );
		if ( sCatalogStatus == _T("0") ) {
			pDM_PROC_0000->SetCellData( _T("BS_정리상태"), _T("정리안됨"), i );
		}
		else if ( sCatalogStatus == _T("1") ) {
			pDM_PROC_0000->SetCellData( _T("BS_정리상태"), _T("정리됨"), i );
		}
	}
	CESL_Grid *pGrid = (CESL_Grid*)FindControl(m_strParentCMAlias , m_strParentGridAlias );

	pGrid->m_bProtectSelectColumn = TRUE;

	pCM->AllControlDisplay();

	pGrid->m_bProtectSelectColumn = FALSE;
	pGrid->SetReverse(pGrid->SelectGetHeadPosition());
	
	for( i = 0; i<pGrid->SelectGetHeadPosition(); i++ )
		pGrid->PostMessage(WM_VSCROLL, SB_LINEDOWN, 0);
	
	CDialog::OnCancel();
	
EFS_END
}

BOOL CBO_CAT_PROC_0500_09::ExeSEPARATION()
{

	INT ids;
	m_strStreamMarc_obj = m_strStreamMarc;

	CString strTagCode, strErrMsg, strHelpMsg;
	if(m_pInfo->pMarcMgr->Decoding(m_strStreamMarc_obj, &m_marc_obj))
	{
		POSITION pos;
		pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
		while (pos)
		{
			m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);
			UpdateData(FALSE);
		}
	}
	
	CESL_DataMgr *pDM_0510_OBJECT;
	pDM_0510_OBJECT = FindDM(_T("DM_BO_CAT_PROC_0510_OBJECT"));
	INT iObjCount = pDM_0510_OBJECT->GetRowCount();

	CArray <CString, CString> RecKeyListArray;
	CString sRec_key;
	for ( INT i = 0 ; i < iObjCount ; i ++ ) 
	{
		CString sItem;
		pDM_0510_OBJECT->GetCellData( _T("BB_책KEY") , i , sItem );
		sRec_key = sRec_key + _T(", ") + sItem;
		RecKeyListArray.Add( sItem );
	}
	sRec_key = sRec_key.Mid(2);

	CESL_DataMgr *pDM = FindDM( _T("DM_BO_CAT_PROC_0500_OBJ책") );
	if ( pDM == NULL ) 
	{
		AfxMessageBox( _T("DM_BO_CAT_PROC_0500_OBJ책 error") );
		return FALSE;
	}

	CString strWhere = _T("BOOK_APPENDIX_FLAG = 'B' AND REC_KEY IN (") + sRec_key + _T(")");

	if ( pDM->RefreshDataManager(strWhere) < 0 ) return FALSE;

	if ( !Set049Tag( &m_marc_obj, pDM, 1 ) ) return FALSE;
	
	CString sPreCotrolNo;
	CString strControlNo;
	m_pInfo->pMarcMgr->GetItem( &m_marc_obj, _T("001*"), sPreCotrolNo );	
	if ( sPreCotrolNo != _T("") ) {		
		CString strControlCode;
		if ( sPreCotrolNo.GetLength() >= 3 ) {
			strControlCode = sPreCotrolNo.Left(3);
		}
		else {
			AfxMessageBox( _T("제어자료구분이 없습니다.") );
			return FALSE;
		}

		CTime time = CTime::GetCurrentTime();
		CString strYear;
		strYear.Format( _T("%04d"), time.GetYear() );
		CString strQuery;
		CString strItem;
		strQuery.Format(_T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE MAKE_YEAR = '%s' AND DIVISION_VALUE = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE"), strYear, strControlCode);
		ids = pDM->GetOneValueQuery(strQuery, strItem);

		CString sControlNo;
		if (ids < 0)
		{
			strControlNo = _T("1");
			sControlNo.Format( _T("%s%s%05s"), strControlCode, strYear, strControlNo );
			strQuery.Format(_T("SELECT CONTROL_NO FROM BO_SPECIES_TBL WHERE CONTROL_NO = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE"), sControlNo);
			ids = pDM->GetOneValueQuery(strQuery, strItem);
			if ( strItem != _T("") ) {
				AfxMessageBox( _T("제어번호가 중복됩니다. 마지막번호를 수정하세요.") );
				return FALSE;
			}

			CString strReckey;
			ids = pDM->MakeRecKey(strReckey);
			if (ids < 0) return FALSE;		
			strQuery.Format(_T("INSERT INTO CO_LAST_NUMBER_TBL (REC_KEY, PUBLISH_FORM_CODE, KIND_CODE, DIVISION_VALUE, MAKE_YEAR, LAST_NUMBER, FIRST_WORK, LAST_WORK, MANAGE_CODE) "
				_T("VALUES ('%s','CO','CON_NO','%s','%s','%s', '%s', '%s', UDF_MANAGE_CODE);"))
				, strReckey, strControlCode, strYear, strControlNo, GetWorkLogMsg(_T("종분리"),WORK_LOG), GetWorkLogMsg(_T("종분리"),WORK_LOG));			
		}
		else
		{
			strControlNo.Format(_T("%d"), _ttoi(strItem) + 1);		
			sControlNo.Format( _T("%s%s%05s"), strControlCode, strYear, strControlNo );			
			strQuery.Format(_T("SELECT CONTROL_NO FROM BO_SPECIES_TBL WHERE CONTROL_NO = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE"), sControlNo);
			ids = pDM->GetOneValueQuery(strQuery, strItem);
			if ( strItem != _T("") ) {
				AfxMessageBox( _T("제어번호가 중복됩니다. 마지막번호를 수정세요.") );
				return FALSE;
			}
			strQuery.Format(_T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER = %s, LAST_WORK = '%s' WHERE DIVISION_VALUE = '%s' AND MAKE_YEAR = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE;"), 
				strControlNo, GetWorkLogMsg(_T("종분리"),WORK_LOG), strControlCode, strYear);			
		}
		m_pInfo->pMarcMgr->DeleteField( &m_marc_obj, _T("001") );
		m_pInfo->pMarcMgr->InsertField( &m_marc_obj, _T("001") + sControlNo );
	}

	CESL_DataMgr *pDmObjVol;
	pDmObjVol = FindDM( _T("DM_BO_CAT_PROC_0500_OBJ권") );
	if ( pDmObjVol == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_PROC_0500_OBJ권 error") );
		return FALSE;
	}
	ids = pDmObjVol->FreeData();
	
	CBO_SPECIES API(this);
	API.CONVERT_BOOK_to_VOLUME( pDM, pDmObjVol );

	if ( pDmObjVol->GetRowCount() > 1 ) {
		if ( !SetVolumnInfo( &m_marc_obj, pDmObjVol, pDM ) ) return FALSE;
	}

	CESL_DataMgr *pDM_0510;
	pDM_0510 = FindDM(_T("DM_BO_CAT_PROC_0510"));
	INT iMainCount = pDM_0510->GetRowCount();

	CString sMainRecKey;
	for ( i = 0 ; i < iMainCount ; i ++ ) {
		CString sItem;
		pDM_0510->GetCellData( _T("BB_책KEY") , i , sItem );
		sMainRecKey = sMainRecKey + _T(", ") + sItem;
	}

	if ( sMainRecKey != _T("") ) {
		sMainRecKey = sMainRecKey.Mid(2);
	}

	CESL_DataMgr *pDmMain = FindDM( _T("DM_BO_CAT_PROC_0300_책") );
	if ( pDmMain == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_PROC_0300_책 error") );
		return FALSE;
	}
	
	CString sMainWhere = _T("REC_KEY IN (") + sMainRecKey + _T(")");
	if ( pDmMain->RefreshDataManager(sMainWhere) < 0 ) return FALSE;

	if ( !Set049Tag ( &m_marc, pDmMain, 0 ) ) return FALSE;

	CESL_DataMgr *pDmVol;
	pDmVol = FindDM( _T("DM_BO_CAT_PROC_0300_권") );
	if ( pDmVol == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_PROC_0300_권 error") );
		return FALSE;	
	}
	
	API.CONVERT_BOOK_to_VOLUME( pDmMain, pDmVol );

	if ( pDmVol->GetRowCount() > 1 ) {
		if ( !SetVolumnInfo( &m_marc, pDmVol, pDmMain ) ) return FALSE;
	}

	pDmObjVol->FreeData();

	m_marcEditor_obj.InitUserAlias();
	m_marcEditor_obj.AddUserAlias(_T("본표제"));
	m_marcEditor_obj.AddUserAlias(_T("저작자"));
	m_marcEditor_obj.AddUserAlias(_T("발행지"));
	m_marcEditor_obj.AddUserAlias(_T("발행자"));
	m_marcEditor_obj.AddUserAlias(_T("발행년"));
	m_marcEditor_obj.AddUserAlias(_T("입력날짜"));
	m_marcEditor_obj.AddUserAlias(_T("발행년1"));
	m_marcEditor_obj.AddUserAlias(_T("유형"));

	m_marcEditor_obj.Init(m_pInfo->pMarcMgr, &m_marc_obj);
	m_marcEditor_obj.Display();
	m_marcEditor.Display();
	
	GetDlgItem(IDOK)->EnableWindow(true);
	
	return TRUE;

}

BOOL CBO_CAT_PROC_0500_09::Set049Tag( CMarc *pMarc, CESL_DataMgr *pDM, INT mode )
{
	if ( pMarc == NULL ) return FALSE;
	INT ids;

	m_pInfo->pMarcMgr->DeleteItem( pMarc, _T("049$l") );
	m_pInfo->pMarcMgr->DeleteItem( pMarc, _T("049$v") );
	m_pInfo->pMarcMgr->DeleteItem( pMarc, _T("049$c") );
	m_pInfo->pMarcMgr->DeleteItem( pMarc, _T("049$f") );

	CString sRegNo;
	CString sVolNoPre = _T("*");
	CString sPreShelfCode;
	INT iCopyCode	 = 1;
	INT iSwRegNo	 = 0;
	INT iSwShelfCode = 0;
	
	
	for ( INT nDMIdx = 0 ; nDMIdx < pDM->GetRowCount() ; nDMIdx ++ ) 
	{
		CString sBookFlag;
		ids = pDM->GetCellData( _T("BB_등록번호"), nDMIdx, sBookFlag );
		if ( sBookFlag == _T("A") ) continue;

		sRegNo = _T("");
		ids = pDM->GetCellData( _T("BB_등록번호"), nDMIdx, sRegNo );		
		if ( sRegNo != _T("") ) {
			m_pInfo->pMarcMgr->DeleteItem(pMarc, _T("049$l"), sRegNo );
			m_pInfo->pMarcMgr->SetItem( pMarc, _T("049$l"), sRegNo );
		}
		CString sItemData = _T("");
		
		ids = pDM->GetCellData( _T("BB_편권차"), nDMIdx, sItemData );
		
		if ( sRegNo != _T("") )
		{	
			if( !sItemData.IsEmpty() )
				ids = m_pInfo->pMarcMgr->SetItem( pMarc, _T("049$v"), sItemData );
			if( m_strWorkingStatus == _T("소장자료") )
			{
				CString sCopyCode;
				sCopyCode = pDM->GetCellData(_T("BB_복본기호"),nDMIdx);
								
				if( !sCopyCode.IsEmpty() )
				m_pInfo->pMarcMgr->SetItem( pMarc, _T("049$c"), sCopyCode );		
			}
			else
			{	
				if ( sVolNoPre == sItemData ) 
				{
					iCopyCode++;
					CString sCopyCode;
					sCopyCode.Format( _T("%d"), iCopyCode );
					
					m_pInfo->pMarcMgr->SetItem( pMarc, _T("049$c"), sCopyCode );
					pDM->SetCellData( _T("BB_복본기호"), sCopyCode, nDMIdx );
				}
				else{
					iCopyCode = 1;
				}
			}
		}
		sVolNoPre = sItemData;

		if ( sRegNo != _T("") ) {
			sRegNo = _T("");
			CString strShelfCode;
			strShelfCode.Empty();
			ids = pDM->GetCellData( _T("BB_별치기호_코드"), nDMIdx, strShelfCode );
			if ( strShelfCode != _T("") ) {
				m_pInfo->pMarcMgr->DeleteItem( pMarc, _T("049$f"), strShelfCode );
				m_pInfo->pMarcMgr->SetItem( pMarc, _T("049$f"), strShelfCode );
			}
			if ( (nDMIdx != 0) && (sPreShelfCode != strShelfCode) && sPreShelfCode != _T("") ) 
			{
				CString s049field;
				m_pInfo->pMarcMgr->GetField( pMarc, _T("049"), s049field );
				s049field = _T("1") + s049field.Mid(1);
				m_pInfo->pMarcMgr->DeleteField( pMarc, _T("049") );
				m_pInfo->pMarcMgr->InsertField( pMarc, _T("049") + s049field );
				iSwShelfCode = 1;
			}
			sPreShelfCode = strShelfCode;
		}
	}

	if ( iSwShelfCode == 0 ) {
		CString s049field;
		m_pInfo->pMarcMgr->GetField( pMarc, _T("049"), s049field );
		if ( s049field != _T("") ) {

			if ( s049field.Left(1) == _T("1") )
			{
				s049field = _T("1") + s049field.Mid(1);
			}
			else
			{
				s049field = _T("0") + s049field.Mid(1);
			}						
			m_pInfo->pMarcMgr->DeleteField( pMarc, _T("049") );
			m_pInfo->pMarcMgr->InsertField( pMarc, _T("049") + s049field );
		}
	}

	return TRUE;
}

BOOL CBO_CAT_PROC_0500_09::SetVolumnInfo( CMarc *pMarc, CESL_DataMgr *pDM, CESL_DataMgr *pDmMain )
{
	CString sVolNo;	
	CString sPriceSum;
	INT iPriceSum = 0;
	INT ids;

	m_pInfo->pMarcMgr->DeleteField( pMarc, _T("020") );
	m_pInfo->pMarcMgr->DeleteItem( pMarc, _T("245$n") );
	ids = m_pInfo->pMarcMgr->DeleteItem( pMarc, _T("245$p") );
	for ( INT i = 0 ; i < pDM->GetRowCount() ; i++ ) 
	{
		CString sItemData = _T("");		

		ids = pDM->GetCellData( _T("BB_편권차"), i, sItemData );
		if ( sItemData != _T("") ) {

			if ( sVolNo == _T("") ) {
				sVolNo = sItemData;
			}
			else {
				sVolNo = sVolNo + _T(",") + sItemData;
			}
		}
		
		ids = pDM->GetCellData( _T("BB_가격"), i, sItemData );
		CString sPrice = sItemData;
		if ( sItemData != _T("") ) {
			INT iPrice = _ttoi ( sItemData.GetBuffer(0) );
			iPriceSum = iPriceSum + iPrice;
		}

		ids = pDM->GetCellData( _T("BB_낱권ISBN"), i, sItemData );
		if ( sItemData != _T("") ) 
		{
			sItemData.Replace( _T("-"), _T("") );
			if ( sPrice != _T("") ) 
			{
				CString sMoneyCode;
				GetControlData( _T("CM_BO_CAT_PROC_0300"), _T("BS_화폐구분"), sMoneyCode );
				sPrice = sMoneyCode + sPrice;	
			}
			if ( sPrice != _T("") ) 
			{
				CString strTmp=_T("");
				strTmp.Format(_T("020  %ca%s%cc%s"), SUBFIELD_CODE, sItemData, SUBFIELD_CODE, sPrice);
				sItemData = strTmp;
			}
			else 
			{
				CString strTmp=_T("");
				strTmp.Format(_T("020  %ca%s"), SUBFIELD_CODE, sItemData);
				sItemData = strTmp;
			}

			CString sEaAdd;
			ids = pDmMain->GetCellData( _T("BB_낱권ISBN부가기호"), i, sEaAdd );
			if ( sEaAdd != _T("") ) 
			{
				CString strTmp=_T("");
				strTmp.Format(_T("%s%cg%s"), sItemData, SUBFIELD_CODE, sEaAdd);
				sItemData = strTmp;
			}
			CString sErrorIsbn = pDmMain->GetCellData( _T("BB_낱권오류ISBN"), i );
			if ( sErrorIsbn != _T("") ) 
			{
				CString strTmp=_T("");
				strTmp.Format(_T("%s%cz%s"), sItemData, SUBFIELD_CODE, sErrorIsbn);
				sItemData = strTmp;
			}
			ids = m_pInfo->pMarcMgr->InsertField( pMarc, sItemData );
		}
	}

	sPriceSum.Format( _T("%d"), iPriceSum );
	if ( sPriceSum != _T("") ) 
	{
		CString sMoneyCode;
		GetControlData( _T("CM_BO_CAT_PROC_0300"), _T("BS_화폐구분"), sMoneyCode );

		CString str950b;
		ids = m_pInfo->pMarcMgr->GetItem( pMarc, _T("950$b"), str950b );
		if( !str950b.IsEmpty() )
		{
			if ( sMoneyCode == _T("") )
			{
				sMoneyCode = str950b;
				if ( sMoneyCode.GetLength() > 2 )
				{
					if ( sMoneyCode.Left(1) < _T("0") || sMoneyCode.Left(1) > _T("9") )
					{
						sMoneyCode = sMoneyCode.Left(1);
					}
				}
			}
			ids = m_pInfo->pMarcMgr->DeleteItem( pMarc, _T("950$b") );
					
			sPriceSum = sMoneyCode + sPriceSum;					
			ids = m_pInfo->pMarcMgr->SetItem( pMarc, _T("950$b"), sPriceSum );
		}
	}
	ids = m_pInfo->pMarcMgr->SetItem( pMarc, _T("245$n"), sVolNo );

	return TRUE;
}

INT CBO_CAT_PROC_0500_09::MakeIndexClassifyCodeConcat(CESL_DataMgr * pDM_Idx, CESL_DataMgr * pDM_Book)
{
	if( pDM_Book == NULL || pDM_Idx == NULL )
		return -1;

	CStringArray * sArrayUserLimitCode = new CStringArray; 
	CStringArray * sArrayRegCode = new CStringArray;
	CStringArray * sArrayManageCode = new CStringArray;
	CStringArray * sArrayShelfCode = new CStringArray;
	CString sUserLimitCode, sRegCode, sManageCode, sShelfCode;

	sArrayUserLimitCode->RemoveAll();
	sArrayRegCode->RemoveAll();
	sArrayManageCode->RemoveAll();
	sArrayShelfCode->RemoveAll();


	INT nBookCnt = pDM_Book->GetRowCount();
	
	for( INT i = 0; i < nBookCnt; i++ )
	{
		pDM_Book->GetCellData(_T("BB_이용제한구분_코드"), i, sUserLimitCode);
		pDM_Book->GetCellData(_T("BB_등록구분_코드"), i, sRegCode);
		pDM_Book->GetCellData(_T("BB_관리구분_코드"), i, sManageCode);
		pDM_Book->GetCellData(_T("BB_별치기호_코드"), i, sShelfCode);

		if(FALSE == IsStringInArray(sArrayUserLimitCode, sUserLimitCode))
			sArrayUserLimitCode->Add(sUserLimitCode);
		if(FALSE == IsStringInArray(sArrayRegCode, sRegCode))
			sArrayRegCode->Add(sRegCode);
		if(FALSE == IsStringInArray(sArrayManageCode, sManageCode))
			sArrayManageCode->Add(sManageCode);
		if(FALSE == IsStringInArray(sArrayShelfCode, sShelfCode))
			sArrayShelfCode->Add(sShelfCode);
	}
	
	sUserLimitCode.Empty();
	sRegCode.Empty();
	sManageCode.Empty();
	sShelfCode.Empty();	

	for( i = 0; i< sArrayUserLimitCode->GetSize() ; i++)
		sUserLimitCode += _T(" ") + sArrayUserLimitCode->GetAt(i);

	for( i = 0; i< sArrayRegCode->GetSize() ; i++)
		sRegCode += _T(" ") + sArrayRegCode->GetAt(i);

	for( i = 0; i< sArrayManageCode->GetSize() ; i++)
		sManageCode += _T(" ") + sArrayManageCode->GetAt(i);

	for( i = 0; i< sArrayShelfCode->GetSize() ; i++)
		sShelfCode += _T(" ") + sArrayShelfCode->GetAt(i);

	if( sUserLimitCode.GetLength() > 2 )
		sUserLimitCode = sUserLimitCode.Mid(1);
	if( sRegCode.GetLength() > 2 )
		sRegCode = sRegCode.Mid(1);
	if( sManageCode.GetLength() > 2 )
		sManageCode = sManageCode.Mid(1);
	if( sShelfCode.GetLength() > 2 )
		sShelfCode = sShelfCode.Mid(1);
	
	pDM_Idx->SetCellData(_T("IBS_이용제한구분_코드"),sUserLimitCode,0);
	pDM_Idx->SetCellData(_T("IBS_등록구분_코드"),sRegCode,0);
	pDM_Idx->SetCellData(_T("IBS_관리구분_코드"),sManageCode,0);
	pDM_Idx->SetCellData(_T("IBS_청구_별치기호_코드"),sShelfCode,0);

	delete sArrayUserLimitCode;
	delete sArrayRegCode;
	delete sArrayManageCode;
	delete sArrayShelfCode;

	return 0;
}

BOOL CBO_CAT_PROC_0500_09::IsStringInArray( CStringArray * sArray, CString sCode )
{
	if( sArray == NULL || sCode.IsEmpty() )
		return TRUE;

	for( INT i = 0; i< sArray->GetSize(); i++ )
	{
		if ( sCode == sArray->GetAt(i) )
			return TRUE;
	}

	return FALSE;
}

CString CBO_CAT_PROC_0500_09::UpdateIndexQuery(CString sSpecies_key)
{
	if( sSpecies_key == _T("") )
		return _T("");

	CMap<CString, LPCTSTR, CString, LPCTSTR> mapFuncName3Fields;

	mapFuncName3Fields.SetAt(_T("PLACE_INFO"), _T("SHELF_LOC_CODE"));
	mapFuncName3Fields.SetAt(_T("USE_LIMIT_CODE"), _T("USE_LIMIT_CODE"));
	mapFuncName3Fields.SetAt(_T("MANAGE_CODE"), _T("MANAGE_CODE"));
	mapFuncName3Fields.SetAt(_T("REG_CODE"), _T("REG_CODE"));
	mapFuncName3Fields.SetAt(_T("IDX_VOL_TITLE"), _T("VOL_TITLE_INDEX"));
	mapFuncName3Fields.SetAt(_T("IDX_SE_SHELF_CODE"), _T("SEPARATE_SHELF_CODE"));
	mapFuncName3Fields.SetAt(_T("IDX_VOL"), _T("VOL_INDEX"));

	CString strQuery;
	CString strSetQuery;

	CString strTemp;

	strTemp.Format(_T("IDX_HOLD_PLACE = GET_BO_MAIN_SHELF_CODE(%s)"), sSpecies_key);
	strSetQuery += strTemp;

	strTemp.Format(_T(", MAIN_PLACE_INFO = GET_BO_MAIN_SHELF_CODE(%s)"), sSpecies_key);
	strSetQuery +=  strTemp;

	CString strField;
	CString strParam;
	POSITION pos = mapFuncName3Fields.GetStartPosition();
	while (pos)
	{
		mapFuncName3Fields.GetNextAssoc(pos, strField, strParam);
		
		strTemp.Format(_T(", %s = GET_BO_BOOK_INFO(%s, '%s')"), strField, sSpecies_key, strParam);
		strSetQuery += strTemp;
	}

	strQuery.Format(_T("UPDATE IDX_BO_TBL SET %s WHERE REC_KEY = %s;"), strSetQuery, sSpecies_key);
	return strQuery;
}



BOOL CBO_CAT_PROC_0500_09::ExeMultiVolSEPARATION()
{
	CESL_DataMgr *pDM_VolumeHold = FindDM(_T("DMUP_단행_권_권정보"));	
	
	if (pDM_VolumeHold->GetRowCount() == 0 )
	{
		m_strStreamMarc_obj = _T("");
		
		m_marcEditor_obj.Init(m_pInfo->pMarcMgr, &m_marc_obj);
		m_marcEditor_obj.GetEditCtrl()->SetWindowText(_T(""));
		
		GetDlgItem(IDC_bNEXT)->EnableWindow(FALSE);
		GetDlgItem(IDC_bPREV)->EnableWindow(FALSE);
		GetDlgItem(IDC_sNum)->SetWindowText(_T("0 / 0"));
		GetDlgItem(IDOK)->EnableWindow(FALSE);

		return 0;
	}

	GetDlgItem(IDOK)->EnableWindow(TRUE);

	pDM = FindDM(_T("DM_BO_CAT_PROC_0510_SPECIES"));
	if (!pDM) return FALSE;

	CString strWhere = _T("REC_KEY = ") + m_SelectedSRecKey;
	if (pDM->RefreshDataManager(strWhere) < 0)	return FALSE;

	pDM_COPY = FindDM(_T("DM_BO_CAT_PROC_0510_SPECIES_COPY"));
	if (!pDM_COPY) return FALSE;
	
	CString strTagCode, strErrMsg, strHelpMsg;
	CString strSpeciesKey, strVolSortNo;
	CString sPreCotrolNo;
	CString strControlNo;
	INT ids;
	
	CESL_DataMgr *pBookDM = FindDM( _T("DM_BO_CAT_PROC_0500_OBJ책") );
	
	pDM_COPY->FreeData();	
	for ( int i=0 ; i<pDM_VolumeHold->GetRowCount() ; i++ )
	{
		INT idx = DmRowCopy(pDM, 0 ,pDM_COPY);		
		pDM_VolumeHold->GetCellData(_T("BB_권일련번호"), i, strVolSortNo );	

		CString strUseLimitCode;
		strUseLimitCode = pDM_VolumeHold->GetCellData(_T("BB_이용제한구분_코드"), i );	
		if( !strUseLimitCode.IsEmpty() )
		{
			pDM_COPY->SetCellData(_T("BS_이용제한구분_코드"), strUseLimitCode, i);		
		}

		m_strStreamMarc_obj = pDM_COPY->GetCellData( _T("BS_MARC") , i);
		
		if(m_pInfo->pMarcMgr->Decoding(m_strStreamMarc_obj, &m_marc_obj))
		{
			POSITION pos;
			pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
			while (pos)
			{
				m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);
				UpdateData(FALSE);
			}
		}

		strWhere.Format(_T("BOOK_APPENDIX_FLAG = 'B' AND SPECIES_KEY = %s AND VOL_SORT_NO=%s"), m_SelectedSRecKey, strVolSortNo);
		
		if ( pBookDM->RefreshDataManager(strWhere) < 0 ) return FALSE;
		if ( !Set049Tag( &m_marc_obj, pBookDM, 1 ) ) return FALSE;		
	
		m_pInfo->pMarcMgr->GetItem( &m_marc_obj, _T("001*"), sPreCotrolNo );	
		if ( sPreCotrolNo != _T("") ) 
		{		
			CString strControlCode;
			if ( sPreCotrolNo.GetLength() >= 3 ) {
				strControlCode = sPreCotrolNo.Left(3);
			}
			else {
				AfxMessageBox( _T("제어자료구분이 없습니다.") );
				return FALSE;
			}

			CTime time = CTime::GetCurrentTime();
			CString strYear;
			strYear.Format( _T("%04d"), time.GetYear() );
			CString strQuery;
			CString strItem;
			strQuery.Format(_T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE MAKE_YEAR = '%s' AND DIVISION_VALUE = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE"), strYear, strControlCode);
			ids = pDM->GetOneValueQuery(strQuery, strItem);
			
			CString sControlNo;
			if (ids < 0)
			{
				strControlNo = _T("1");
				sControlNo.Format( _T("%s%s%05d"), strControlCode, strYear, _ttoi(strControlNo)+i );
				strQuery.Format(_T("SELECT CONTROL_NO FROM BO_SPECIES_TBL WHERE CONTROL_NO = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE"), sControlNo);
				ids = pDM->GetOneValueQuery(strQuery, strItem);
				if ( strItem != _T("") ) {
					AfxMessageBox( _T("제어번호가 중복됩니다. 마지막번호를 수정하세요.") );
					return FALSE;
				}
				
				CString strReckey;
				ids = pDM->MakeRecKey(strReckey);
				if (ids < 0) return FALSE;		
				strQuery.Format(_T("INSERT INTO CO_LAST_NUMBER_TBL (REC_KEY, PUBLISH_FORM_CODE, KIND_CODE, DIVISION_VALUE, MAKE_YEAR, LAST_NUMBER, FIRST_WORK, LAST_WORK, MANAGE_CODE) "
					_T("VALUES ('%s','CO','CON_NO','%s','%s','%s', '%s', '%s', UDF_MANAGE_CODE);"))
					, strReckey, strControlCode, strYear, strControlNo, GetWorkLogMsg(_T("종분리"),WORK_LOG), GetWorkLogMsg(_T("종분리"),WORK_LOG));			
			}
			else
			{
				strControlNo.Format(_T("%d"), _ttoi(strItem) + 1 + i);		
				sControlNo.Format( _T("%s%s%05s"), strControlCode, strYear, strControlNo );			
				strQuery.Format(_T("SELECT CONTROL_NO FROM BO_SPECIES_TBL WHERE CONTROL_NO = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE"), sControlNo);
				ids = pDM->GetOneValueQuery(strQuery, strItem);
				if ( strItem != _T("") ) {
					AfxMessageBox( _T("제어번호가 중복됩니다. 마지막번호를 수정세요.") );
					return FALSE;
				}
				strQuery.Format(_T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER = %s, LAST_WORK = '%s' WHERE DIVISION_VALUE = '%s' AND MAKE_YEAR = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE;"), 
					strControlNo, GetWorkLogMsg(_T("종분리"),WORK_LOG), strControlCode, strYear);			
			}

			if ( pBookDM->GetRowCount() > 1 )
			{
				for ( int i=1 ; i<pBookDM->GetRowCount() ; i++ )
				{
					pBookDM->DeleteRow(i);					
				}
				
			}
			if ( !SetVolumnInfo( &m_marc_obj, pBookDM, pDM_VolumeHold ) ) return FALSE;

			m_pInfo->pMarcMgr->DeleteField( &m_marc_obj, _T("001") );
			m_pInfo->pMarcMgr->InsertField( &m_marc_obj, _T("001") + sControlNo );			
		}
		m_pInfo->pMarcMgr->Encoding(&m_marc_obj, m_strStreamMarc_obj);
		pDM_COPY->SetCellData( _T("BS_MARC") , m_strStreamMarc_obj, i );

	}	

	if(m_pInfo->pMarcMgr->Decoding(m_strStreamMarc, &m_marc))
	{
		POSITION pos;
		pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
		while (pos)
		{
			m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);
			UpdateData(FALSE);
		}
	}

	CESL_DataMgr *pDM_0510;	
	CESL_DataMgr *pDmVol;

	pDmVol = FindDM( _T("DM_BO_CAT_PROC_0300_권") );
	pDM_0510 = FindDM(_T("DM_BO_CAT_PROC_0510"));

	CBO_SPECIES API(this);
	API.CONVERT_BOOK_to_VOLUME( pDM_0510, pDmVol );
	if ( !Set049Tag( &m_marc, pDM_0510, 1 ) ) return FALSE;

	if ( !SetVolumnInfo( &m_marc, pDmVol, pDM_0510 ) ) return FALSE;

	m_marcEditor.Init(m_pInfo->pMarcMgr, &m_marc);
	m_marcEditor.Display();
	
	m_marcEditor_obj.InitUserAlias();
	m_marcEditor_obj.AddUserAlias(_T("본표제"));
	m_marcEditor_obj.AddUserAlias(_T("저작자"));
	m_marcEditor_obj.AddUserAlias(_T("발행지"));
	m_marcEditor_obj.AddUserAlias(_T("발행자"));
	m_marcEditor_obj.AddUserAlias(_T("발행년"));
	m_marcEditor_obj.AddUserAlias(_T("입력날짜"));
	m_marcEditor_obj.AddUserAlias(_T("발행년1"));
	m_marcEditor_obj.AddUserAlias(_T("유형"));

	m_DividIndex = 0;
	ShowEditMarc();	
	
	return TRUE;
}

void CBO_CAT_PROC_0500_09::OnbPREV() 
{
	m_DividIndex = m_DividIndex-1;
	ShowEditMarc();	
}

void CBO_CAT_PROC_0500_09::OnbNEXT() 
{
	m_DividIndex = m_DividIndex + 1;
	ShowEditMarc();
}


INT CBO_CAT_PROC_0500_09::ShowEditMarc()
{
	CString strTagCode, strErrMsg, strHelpMsg;

	INT nRowCount;
	nRowCount = pDM_COPY->GetRowCount();
	
	if ( m_DividIndex == 0 ) GetDlgItem(IDC_bPREV)->EnableWindow(FALSE);
	else GetDlgItem(IDC_bPREV)->EnableWindow(TRUE);	

	if ( m_DividIndex == nRowCount-1 ) GetDlgItem(IDC_bNEXT)->EnableWindow(FALSE);
	else GetDlgItem(IDC_bNEXT)->EnableWindow(TRUE);	
	
	strTagCode.Format(_T("%d / %d"), m_DividIndex+1, nRowCount );
	GetDlgItem(IDC_sNum)->SetWindowText(strTagCode);

	m_strStreamMarc_obj = pDM_COPY->GetCellData( _T("BS_MARC") , m_DividIndex);
	
	if(m_pInfo->pMarcMgr->Decoding(m_strStreamMarc_obj, &m_marc_obj))
	{
		POSITION pos;
		pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
		while (pos)
		{
			m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);
			UpdateData(FALSE);
		}
	}

	m_marcEditor_obj.Init(m_pInfo->pMarcMgr, &m_marc_obj);
	m_marcEditor_obj.Display();
	m_marcEditor.Display();

	return 0;
}

void CBO_CAT_PROC_0500_09::OnbUPDATE() 
{
		
}

INT CBO_CAT_PROC_0500_09::OnVolumnSave()
{		
	CESL_DataMgr *pDM_IDX;
	CESL_DataMgr *pDM_IDX_COPY;
	CESL_DataMgr *pDM_0510_OBJECT;
	CESL_DataMgr *pDM_0510;
	CESL_DataMgr *pDM;
	
	CString sControlNo;
	CString sRecKeyDiv;
	CString s_Alias,s_Data,s_fName,s_fType,s_dType;
	CString strVolSortNo;
	
	CString strTagCode, strErrMsg, strHelpMsg;	
	pDM_IDX = FindDM(_T("DM_BO_CAT_PROC_0510_IDX"));
	if (!pDM_IDX) 
	{
		return -1000;
	}
	
	pDM = FindDM(_T("DM_BO_CAT_PROC_0510_SPECIES_COPY"));
	if (!pDM) 
	{		
		return -1000;
	}
	
	pDM_0510_OBJECT = FindDM(_T("DM_BO_CAT_PROC_0510_OBJECT"));
	
	CESL_DataMgr *pDM_VolumeHold = FindDM(_T("DMUP_단행_권_권정보"));	
	
	CString strWhere = _T("REC_KEY = ") + m_SelectedSRecKey;
	
	if (pDM_IDX->RefreshDataManager(strWhere) < 0)	return -1010;	
	pDM_IDX_COPY = FindDM(_T("DM_BO_CAT_PROC_0510_IDX_COPY"));
	if (!pDM_IDX_COPY) return -1020;
	int idx = DmRowCopy(pDM_IDX, 0 ,pDM_IDX_COPY);
	
	CString strQuery;
	INT i,j;
	
	for ( i=0 ; i<pDM_VolumeHold->GetRowCount() ; i++ )
	{
		INT ids = pDM->MakeRecKey( sRecKeyDiv );
		if ( 0 > ids ) return ids;
		ids = pDM->StartFrame();
		if ( ids < 0 ) 
		{			
			return ids;
		}
		
		m_strStreamMarc_obj = pDM->GetCellData( _T("BS_MARC") , i);
		
		if(m_pInfo->pMarcMgr->Decoding(m_strStreamMarc_obj, &m_marc_obj))
		{
			POSITION pos;
			pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
			while (pos)
			{
				m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);
				UpdateData(FALSE);
			}
		}
		
		pDM->InitDBFieldData();
		pDM->AddDBFieldData(_T("REC_KEY"), _T("STRING"), sRecKeyDiv);

		INT nCol = pDM->RefList.GetCount();
		
		for( j = 0 ; j< nCol ; j++)
		{
			INT ids =pDM->FindColumnFieldName( j , s_Alias , s_fName ,s_fType,s_dType,i,s_Data);
			if( !ids )
			{
				s_Data=_T("");
				pDM->GetCellData(s_Alias,i,s_Data);			
				if ( s_Alias == _T("BS_제어번호") ) 
				{
					CString sControlNo;
					m_pInfo->pMarcMgr->GetItem( &m_marc_obj, _T("001*"), sControlNo );
					s_Data = sControlNo;
				}
				if ( s_Alias != _T("BS_MARC") && s_Alias != _T("BS_종KEY") && s_Alias != _T("BS_SET_ISBN") ) 
				{
					pDM->AddDBFieldData(s_fName, _T("STRING"), s_Data);
				}
			}
		}
		pDM->AddDBFieldData(_T("APPLY_YN"), _T("STRING"), _T("Y"));	
		
		ids = pDM->MakeInsertFrame(_T("BO_SPECIES_TBL"));
		if ( ids < 0 ) 
		{			
			return -1010;
		}
		
		m_pInfo->pMarcMgr->GetItem( &m_marc_obj, _T("001*"), sControlNo );
		if ( sControlNo != _T("") ) 
		{
			CString strQuery;
			CString strItem;
			strQuery.Format(_T("SELECT CONTROL_NO FROM BO_SPECIES_TBL WHERE CONTROL_NO = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE"), sControlNo);
			ids = pDM->GetOneValueQuery(strQuery, strItem);
			if ( strItem != _T("") ) {
				AfxMessageBox( _T("제어번호가 중복됩니다. 마지막번호를 수정하고 다시 부여하세요.") );
				return 100;
			}
			INT iNum = _ttoi ( sControlNo.Right(5) );
			CString strYear = sControlNo.Mid(3,4);
			CString strControlCode = sControlNo.Left(3);
			if ( iNum == 1 ) {
				CString strReckey;
				ids = pDM->MakeRecKey(strReckey);		
				if (ids < 0) return ids;		
				strQuery.Format(_T("INSERT INTO CO_LAST_NUMBER_TBL (REC_KEY, PUBLISH_FORM_CODE, KIND_CODE, DIVISION_VALUE, MAKE_YEAR, LAST_NUMBER, FIRST_WORK, LAST_WORK, MANAGE_CODE) "
					_T("VALUES ('%s','CO','CON_NO','%s','%s','%s', '%s', '%s', UDF_MANAGE_CODE);"))
					, strReckey, strControlCode, strYear, sControlNo.Right(5), GetWorkLogMsg(_T("종분리"),WORK_LOG), GetWorkLogMsg(_T("종분리"),WORK_LOG));			
			}
			else {
				strQuery.Format(_T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER = %s, LAST_WORK = '%s' WHERE DIVISION_VALUE = '%s' AND MAKE_YEAR = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE;"), 
					sControlNo.Right(5), GetWorkLogMsg(_T("종분리"),WORK_LOG), strControlCode, strYear);			
			}
			ids = pDM->AddFrame(strQuery);
			if ( ids < 0 ) {
				AfxMessageBox( _T("종분리 마지막번호 업데이트 AddFrame 확인") );
				return ids;
			}
		}
		
		pDM_VolumeHold->GetCellData(_T("BB_권일련번호"), i, strVolSortNo );	
		strWhere.Format(_T("BOOK_APPENDIX_FLAG = 'B' AND SPECIES_KEY = %s AND VOL_SORT_NO=%s"), m_SelectedSRecKey, strVolSortNo);

		if ( pDM_0510_OBJECT->RefreshDataManager(strWhere) < 0 ) return FALSE;
		
		pDM_0510 = FindDM(_T("DM_BO_CAT_PROC_0510"));
		pDM_0510_OBJECT = FindDM(_T("DM_BO_CAT_PROC_0510_OBJECT"));
		INT iObjCount = pDM_0510_OBJECT->GetRowCount();
		
		MakeIndexClassifyCodeConcat(pDM_IDX_COPY, pDM_0510_OBJECT);
		
		pDM->InitDBFieldData();
		pDM->AddDBFieldData(_T("REC_KEY"), _T("STRING"), sRecKeyDiv);
		
		nCol = pDM_IDX_COPY->RefList.GetCount();
		
		for( j=0 ; j< nCol ; j++)
		{
			INT ids =pDM_IDX_COPY->FindColumnFieldName( j , s_Alias , s_fName ,s_fType,s_dType,0,s_Data);
			if( !ids )
			{
				s_Data=_T("");
				pDM_IDX_COPY->GetCellData(s_Alias,0,s_Data);
				
				if( s_Alias == _T("IBS_제어번호") )
				{
					CString sControlNo;
					m_pInfo->pMarcMgr->GetItem( &m_marc_obj, _T("001*"), sControlNo );
					s_Data = sControlNo;
				}
				if( s_Alias != _T("BS_MARC") && s_Alias != _T("IBS_REC_KEY") )
				{
					pDM->AddDBFieldData(s_fName, _T("STRING"), s_Data);
				}
			}
		}

		ids = pDM->MakeInsertFrame(_T("IDX_BO_TBL"));
		if ( ids < 0 ) 
		{
			AfxMessageBox(_T("MakeInsertFrame 를 확인하세요"));
			return 100;
		}
		
		MakeIndexClassifyCodeConcat(pDM_IDX,pDM_0510);
		
		pDM->InitDBFieldData();
		pDM->AddDBFieldData(_T("REC_KEY"), _T("STRING"), m_SelectedSRecKey);
		
		nCol = pDM_IDX->RefList.GetCount();
		
		for( j=0 ; j< nCol ; j++)
		{
			INT ids =pDM_IDX->FindColumnFieldName( j , s_Alias , s_fName ,s_fType,s_dType,0,s_Data);
			if( !ids )
			{
				s_Data=_T("");
				pDM_IDX->GetCellData(s_Alias,0,s_Data);
				if( s_Alias == _T("IBS_이용제한구분_코드") || 
					s_Alias == _T("IBS_등록구분_코드") || 
					s_Alias == _T("IBS_관리구분_코드") || 
					s_Alias == _T("IBS_청구_별치기호_코드") ) 
				{
					pDM->AddDBFieldData(s_fName, _T("STRING"), s_Data);
				}
			}
		}
		
		ids = pDM->MakeUpdateFrame( _T("IDX_BO_TBL"), _T("REC_KEY"), _T("NUMERIC"), m_SelectedSRecKey );
		if ( ids < 0 ) 
		{
			AfxMessageBox(_T("MakeInsertFrame 를 확인하세요"));
			return 100;
		}		
		
		INT iCh;	
		
		CString bRepresentYn = _T("N");
		for ( j = 1 ; j <= iObjCount ; j ++ ) 
		{
			CString sItem;
			pDM_0510_OBJECT->GetCellData( _T("BB_대표책여부") , j-1 , sItem );
			if ( sItem == _T("Y") ) 
			{
				bRepresentYn = _T("Y");
				break;
			}
		}
		
		for ( j = 1 ; j <= iObjCount ; j ++ ) 
		{
			CString sItem, sStatus, sRepresentBook;
			pDM_0510_OBJECT->GetCellData( _T("BB_책KEY") , j-1 , sItem );
			pDM_0510_OBJECT->GetCellData( _T("BB_자료상태") , j-1 , sStatus );
			pDM_0510_OBJECT->GetCellData( _T("BB_대표책여부") , j-1 , sRepresentBook );
			CString sQuery, sIdxUpdateQuery;
			if ( j == 1 && bRepresentYn == _T("N") ) 
			{
				sQuery.Format( _T("UPDATE BO_BOOK_TBL SET REPRESENT_BOOK_YN = 'Y', SPECIES_KEY = %s, LAST_WORK='%s' WHERE REC_KEY = '%s';") , sRecKeyDiv , GetWorkLogMsg(_T("종분리"),WORK_LOG), sItem );
				sIdxUpdateQuery.Format( _T("UPDATE IDX_BO_TBL SET WORKING_STATUS = '%s' WHERE REC_KEY = %s;"),sStatus,sRecKeyDiv);
			}
			else 
			{
				if( sRepresentBook == _T("Y") )
					sIdxUpdateQuery.Format( _T("UPDATE IDX_BO_TBL SET WORKING_STATUS = '%s' WHERE REC_KEY = %s;"),sStatus,sRecKeyDiv);
				sQuery.Format( _T("UPDATE BO_BOOK_TBL SET SPECIES_KEY = %s, LAST_WORK='%s' WHERE REC_KEY = '%s';") , sRecKeyDiv , GetWorkLogMsg(_T("종분리"),WORK_LOG), sItem );
			}
			iCh = pDM->AddFrame( sIdxUpdateQuery );
			iCh = pDM->AddFrame( sQuery );
			sQuery.Format(_T("UPDATE LS_WORK_T01 SET SPECIES_KEY = %s WHERE BOOK_KEY = %s;"), sRecKeyDiv, sItem);
			iCh = pDM->AddFrame( sQuery );
			if ( 0 > iCh ) return -1040;
			sQuery.Format(_T("UPDATE CO_LOAN_TBL SET SPECIES_KEY = %s WHERE BOOK_KEY = %s;"), sRecKeyDiv, sItem);
			iCh = pDM->AddFrame( sQuery );
			if ( 0 > iCh ) return -1040;

			CString strValue;
			sQuery.Format(_T("SELECT MIN(VALUE_1) FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='책두레시스템' AND VALUE_NAME='책두레사용여부' AND MANAGE_CODE = '%s'"),m_pInfo->MANAGE_CODE);
			iCh = pDM->GetOneValueQuery(sQuery,strValue);
			if ( 0 > iCh ) return -1040;
			if (strValue.Compare(_T("Y"))==0 )
			{
				sQuery.Format(_T("UPDATE ILL_TRANS_TBL SET SPECIES_KEY = %s WHERE BOOK_KEY = %s;"), sRecKeyDiv, sItem);
				iCh = pDM->AddFrame( sQuery );
				if ( 0 > iCh ) return -1040;
			}
		}
		
		if ( bRepresentYn == _T("Y") ) 
		{
			CESL_DataMgr *pDM_0510;
			pDM_0510 = FindDM(_T("DM_BO_CAT_PROC_0510"));
			CString sMainRecKey = pDM_0510->GetCellData( _T("BB_책KEY") , 0 );
			CString sQuery;
			sQuery.Format( _T("UPDATE BO_BOOK_TBL SET REPRESENT_BOOK_YN = 'Y', LAST_WORK='%s' WHERE REC_KEY = '%s';") ,GetWorkLogMsg(_T("종분리"),WORK_LOG), sMainRecKey );
			iCh = pDM->AddFrame( sQuery );
			if ( 0 > iCh ) return iCh;
		}
		m_pInfo->pMarcMgr->Encoding(&m_marc, m_strStreamMarc);
		m_pInfo->pMarcMgr->Encoding(&m_marc_obj, m_strStreamMarc_obj);

		pDM->InitDBFieldData();
		pDM->AddDBFieldData( _T("MARC"), _T("STRING"), m_strStreamMarc_obj, 1 );
		pDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), GetWorkLogMsg(_T("종분리"), WORK_LOG), 1 );	
		INT idsTmp = pDM->MakeUpdateFrame( _T("BO_SPECIES_TBL"), _T("REC_KEY"), _T("NUMERIC"), sRecKeyDiv );
		if ( 0 > idsTmp ) return idsTmp;
		
		pDM->InitDBFieldData();
		pDM->AddDBFieldData( _T("MARC"), _T("STRING"), m_strStreamMarc, 1 );
		pDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), GetWorkLogMsg(_T("종분리"), WORK_LOG), 1 );	
		idsTmp = pDM->MakeUpdateFrame( _T("BO_SPECIES_TBL"), _T("REC_KEY"), _T("NUMERIC"), m_SelectedSRecKey );
		if ( 0 > idsTmp ) return idsTmp;
		
		if(m_pInfo->GetSearchIndexType() == ECO_SEARCH)
		{
			CString strJJSQuery;
			strJJSQuery.Format(_T("ECO$P_MAKE_BO$I(%s);"), m_SelectedSRecKey);
			iCh = pDM->AddFrame(strJJSQuery);
			if ( 0 > iCh ) return iCh;
			strJJSQuery.Format(_T("ECO$P_MAKE_BO$I(%s);"), sRecKeyDiv);
			iCh = pDM->AddFrame(strJJSQuery);
			if ( 0 > iCh ) return iCh;			
		}
		else
		{
			
		}

		strQuery = UpdateIndexQuery(sRecKeyDiv);
		if( strQuery != _T("") )
		{
			ids = pDM->AddFrame(strQuery);
		}
		
		iCh = pDM->SendFrame();
		if ( iCh < 0 ) 
		{
			AfxMessageBox( _T("저장실패. 재시도 하십시오.") );
			return 100;
		}
	}

	INT iCh = pDM->EndFrame();
	INT ids;
	
	if ( 0 > iCh ) 
	{
		return iCh;
	}
	else
	{
		pDM->StartFrame();
		pDM->InitDBFieldData();
		
		CString strQuery;
		strQuery = UpdateIndexQuery(m_SelectedSRecKey);
		if( strQuery != _T("") )
		{
			ids = pDM->AddFrame(strQuery);
		}		
		pDM->SendFrame();
		pDM->EndFrame();
	}
	
	
	AfxMessageBox(_T("자료를 분리하였습니다."));
	
	CESL_DataMgr *pDM_PROC_0000;
	pDM_PROC_0000 = FindDM(m_strParentDMAlias);
	if ( pDM_PROC_0000 == NULL ) {
		AfxMessageBox ( _T("DM_BO_CAT_PROC_0000 error") );
		return 100;
	}
	
	CESL_DataMgr *pWhere;
	pWhere = FindDM( _T("DM_BO_CAT_PROC_WHERE") );
	if ( pWhere == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_PROC_WHERE ERROR") );
		return 100;
	}
	CString sWhere;
	pWhere->GetCellData( _T("조건절"), 0, sWhere );
	
	if (pDM_PROC_0000->RefreshDataManager(sWhere) < 0)	return -1050;
	
	pDM_PROC_0000->SORT( _T("시작등록번호"), FALSE );
	
	CESL_ControlMgr *pCM;
	pCM = FindCM(m_strParentCMAlias);
	if ( pCM == NULL ) {
		AfxMessageBox ( _T("CM_BO_CAT_PROC_0000 error") );
		return 100;
	}
	for ( i = 0 ; i < pDM_PROC_0000->GetRowCount() ; i++ ) {
		CString sCatalogStatus = pDM_PROC_0000->GetCellData( _T("BS_정리상태"), i );
		if ( sCatalogStatus == _T("0") ) {
			pDM_PROC_0000->SetCellData( _T("BS_정리상태"), _T("정리안됨"), i );
		}
		else if ( sCatalogStatus == _T("1") ) {
			pDM_PROC_0000->SetCellData( _T("BS_정리상태"), _T("정리됨"), i );
		}
	}
	CESL_Grid *pGrid = (CESL_Grid*)FindControl(m_strParentCMAlias , m_strParentGridAlias );
	
	pGrid->m_bProtectSelectColumn = TRUE;
	
	pCM->AllControlDisplay();
	
	pGrid->m_bProtectSelectColumn = FALSE;
	pGrid->SetReverse(pGrid->SelectGetHeadPosition());
	
	for( i = 0; i<pGrid->SelectGetHeadPosition(); i++ )
		pGrid->PostMessage(WM_VSCROLL, SB_LINEDOWN, 0);
	
	CDialog::OnCancel();
	
	return 0;
		
		
	return -1;
}

HBRUSH CBO_CAT_PROC_0500_09::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
