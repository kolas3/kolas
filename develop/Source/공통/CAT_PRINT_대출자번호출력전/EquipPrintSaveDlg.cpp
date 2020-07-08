// EquipPrintSaveDlg.cpp: implementation of the CEquipPrintSaveDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EquipPrintSaveDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEquipPrintSaveDlg::CEquipPrintSaveDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	m_nModeMONO_SERIAL = 0;
}

CEquipPrintSaveDlg::~CEquipPrintSaveDlg()
{

}

BOOL CEquipPrintSaveDlg::Create(CWnd *pParentWnd)
{
	EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;
}

INT CEquipPrintSaveDlg::Init()
{
	EFS_BEGIN

	if (!m_pDM_Parent || !m_pGrid_Parent|| m_strSpeciesKeyAlias.IsEmpty()) return -1;

	CString strBookKey;
	INT i ,j;
	
	// 멤버 정보 세팅
	m_pDM = FindDM(_T("DM_정리_출력_장비_책"));	

	if (!m_pDM ) return -1;	

	// 선정 리스트 체크한다.
	INT ids = -1;
	ids = m_pGrid_Parent->SelectMakeList();
	if (ids < 0) return ids;

	INT nCount = m_pGrid_Parent->SelectGetCount();
	if (nCount <= 0)
	{
		if ( m_arrStrSpeciesKey.GetSize() < 1 ) {
			m_strLastErrMsg = _T("선정된 자료가 없습니다.");
			return -1;
		}
	}

	// 선정 리스트를 돌면서 종키를 가져온다.	
	CStringArray arrStrSpeciesKey;

	INT nIdx = m_pGrid_Parent->SelectGetHeadPosition();
	//if (nIdx < 0) return -1;
	if (nIdx < 0 && m_arrStrSpeciesKey.GetSize() < 1) return -1;

	CString strSpeciesKey;
	while (nIdx >= 0)
	{
		// DM에서 종키를 가져온다.
		m_pDM_Parent->GetCellData(m_strSpeciesKeyAlias, nIdx, strSpeciesKey);
		if (!strSpeciesKey.IsEmpty())
			arrStrSpeciesKey.Add(strSpeciesKey);

		nIdx = m_pGrid_Parent->SelectGetNext();
	}
	
	// 출력자료 보관된 내용이 있으면 그 내용을 출력한다.
	INT iSwSave = 0;	// 출력자료 보관된 내용이 있으면 1, 없으면 0
	if ( m_arrStrSpeciesKey.GetSize() > 0 ) {
		arrStrSpeciesKey.RemoveAll();
		for ( i = 0 ; i < m_arrStrSpeciesKey.GetSize() ; i++ ) {
			arrStrSpeciesKey.Add( m_arrStrSpeciesKey.GetAt(i) );
		}
		//AfxMessageBox( _T("출력자료 보관된 내용을 출력합니다.") );
		iSwSave = 1;		
	}

	// 저장된 종키를 가지고 쿼리를 구성한다.	
	CString strOption = _T("I.REC_KEY IN (");
	nCount = arrStrSpeciesKey.GetSize();
	for ( i = 0; i < nCount; i++)
	{
		strOption += arrStrSpeciesKey.GetAt(i);
		if (i != nCount - 1)
			strOption += _T(", ");
	}	

	strOption += _T(")");

	// 단행 연속에 따라 테이블 이름을 및 쿼리를 변경한다.
	CString strFieldName;
	if (m_nModeMONO_SERIAL == 0)
	{
		m_pDM->TBL_NAME = _T("BO_BOOK_TBL B, IDX_BO_TBL I");
		strFieldName = _T("DECODE(BOOK_APPENDIX_FLAG, 'A', '부록')");
	}
	else
	{
		m_pDM->TBL_NAME = _T("SE_BOOK_TBL B, IDX_SE_TBL I");
		strFieldName = _T("DECODE(BOOK_TYPE, 'A', '부록')");
	}

	// 책부록에 대한 내용을 바꾸어준다.
	CESL_DataMgr::reference *pRef = NULL;
	POSITION pos = m_pDM->RefList.GetHeadPosition();
	while (pos)
	{
		pRef = (CESL_DataMgr::reference*)m_pDM->RefList.GetNext(pos);
		if (pRef && pRef->FIELD_ALIAS == _T("책부록"))
		{
			pRef->FIELD_NAME = strFieldName;
			break;
		}		
	}

 	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // 2003.08.23 이학중 수정
    // 복본추기한 내용의 원문책 정보를 보여 주지 않게 수정.  
    
	//strOption += _T("AND B.DUP_SPECIES_KEY IS NULL ") ;
	CESL_DataMgr *pDM_Where = FindDM(_T("DM_BO_CAT_PROC_WHERE"));
    CString bookWhere ;
	pDM_Where->GetCellData (_T("책조건절"),0,bookWhere) ;
    
	CString strFrom = m_pDM->TBL_NAME ;

	if(bookWhere.Find (_T("a."))>=0)
	{
		strFrom +=  _T(" , co_accession_rec_tbl a ") ;
		strOption = strOption + _T(" AND B.ACCESSION_REC_KEY = A.REC_KEY");
	}
	
	if(bookWhere.Find (_T("p."))>=0) 
	{
        strFrom +=  _T(" , BO_PURCHASE_SEQ_NO_TBL p  ") ;
	}
    
	m_pDM->TBL_NAME = strFrom ;
	//ids = m_pDM->RefreshDataManager(strOption);

    if ( iSwSave == 0 ) {	// 출력자료보관된 내용이 없으면 책조건절 포함
		strOption += _T(" and ") +bookWhere ;
	}
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	if ( iSwSave == 1 ) {
		CESL_DataMgr *pDmDeleteBook;
		pDmDeleteBook = FindDM(_T("DM_마지막번호"));
		if ( NULL == pDmDeleteBook ) {
			AfxMessageBox ( _T("pDmDeleteBook error") );
			return -1;
		}
		//삭제책KEY
		CString sDeleteBookKey, sDeleteBookKeyList;
		INT iDeleteCount = pDmDeleteBook->GetRowCount();
		for ( INT i = 0 ; i < iDeleteCount ; i++ ) {
			sDeleteBookKey = pDmDeleteBook->GetCellData( _T("삭제책KEY"), i );
			sDeleteBookKeyList = sDeleteBookKeyList + _T(",") + sDeleteBookKey;
		}
		
		if ( sDeleteBookKeyList != _T("") ) {
			sDeleteBookKeyList = sDeleteBookKeyList.Mid(1);
			strOption = strOption + _T(" AND B.REC_KEY NOT IN (") + sDeleteBookKeyList + _T(")");
		}
	}

	//strOption = strOption + _T(" AND I.REC_KEY = B.SPECIES_KEY AND B.REG_NO = 'EM0000000011'");
	ids = m_pDM->RefreshDataManager(strOption);
	if (ids < 0)
	{
		m_strLastErrMsg = _T("데이터베이스와 작업중에 에러가 발생했습니다.");
		return ids;
	}

	// 본관 삭제된 내용 제외
	for ( i = m_pDM->GetRowCount()-1 ; i >= 0 ; i-- ) {
		CString sBookKey = m_pDM->GetCellData( _T("BB_책KEY"), i );
		INT iBookCount = m_arrStrDeleteBookKey.GetSize();
		for ( INT j = 0 ; j < m_arrStrDeleteBookKey.GetSize() ; j++ ) {
			if ( sBookKey == m_arrStrDeleteBookKey.GetAt(j) ) {
				m_pDM->DeleteRow(i);
				break;
			}
		}
	}
	// 출력형태 및 청구기호를 세팅해 준다. (청구기호 : 별치기호 분류기호-도서기호-권책기호=복본기호)
	CString strCallNo;
	CString strItem;
	nCount = m_pDM->GetRowCount();
	for (i = 0; i < nCount; i++)
	{
		// 소정 수정(2004.10.08)
		// 청구기호 생성 Format 2(줄바꿔찍기)
		strCallNo = MakeCallNoPrintFormat2(i);

		// 청구기호
		ids = m_pDM->SetCellData(_T("청구기호"), strCallNo, i);

		// 출력형태
		ids = m_pDM->GetCellData(_T("레이블출력구분"), i, strItem);
		//if (strItem.IsEmpty())
		ids = m_pDM->SetCellData(_T("레이블출력구분"), _T("2"), i);
	}

	// 검색된 결과를 보관한다.
	CESL_DataMgr *m_pSaveDM;
	const INT nColCnt = 19;
	CString sColAliasData[nColCnt];
	CString sColAlias[nColCnt] =
	{
		_T("도서기호"), _T("분류기호"), _T("복본기호"), _T("레이블출력구분"), _T("등록번호"),
		_T("RFID_SERIAL"), _T("별치기호_코드"), _T("배가코드"), _T("권책기호"), _T("작업상태"), 
		_T("저작자"), _T("발행자"), _T("BS_종KEY"), _T("본표제"), _T("청구기호"), 
		_T("선정"), _T("삭제책KEY"), _T("책부록"), _T("BB_책KEY")
	};
	
	m_pSaveDM = FindDM(_T("DM_정리_출력_장비_책_보관"));
	nCount = m_pDM->GetRowCount();
	nIdx = m_pSaveDM->GetRowCount();

	CString strBookKey2;
	BOOL	bCheck;
	nIdx = m_pSaveDM->GetRowCount();
	for ( i=0 ; i<nCount ; i++ )
	{	
		// 중복조사, 이미 보관된 자료이면 SKIP
		m_pDM->GetCellData(_T("BB_책KEY"), i, strBookKey );
		bCheck = FALSE;

		for ( j=0 ; j<nIdx ; j++ )
		{
			m_pSaveDM->GetCellData(_T("BB_책KEY"), j, strBookKey2 );
			if ( strBookKey == strBookKey2 ) 
			{
				bCheck = TRUE;
				break;
			}
		}

		if ( bCheck ) continue;
		
		m_pDM->GetCellData(sColAlias, 19, sColAliasData, i );				
		ids = m_pSaveDM->InsertRow(-1);
		m_pSaveDM->SetCellData( sColAlias , 19, sColAliasData , nIdx );
		nIdx++;
	}

	nIdx = m_pSaveDM->GetRowCount();
	strItem.Format(_T("선정자료가 보관되었습니다.\n보관된 책수 : %d\n 전체 책수 : %d"),nCount, nIdx);
	AfxMessageBox(strItem);	

	return 0;

	EFS_END
	return -1;
}

//INT CEquipPrintSaveDlg::SetParentInfo(CESL_DataMgr *pDM_Parent, CESL_Grid *pGrid_Parent, CString strSpeciesKeyAlias)
INT CEquipPrintSaveDlg::SetParentInfo(CESL_DataMgr *pDM_Parent, CESL_Grid *pGrid_Parent, CString strSpeciesKeyAlias, CStringArray *arrStrSpeciesKey)
{
	EFS_BEGIN

	if (!pDM_Parent || !pGrid_Parent || strSpeciesKeyAlias.IsEmpty()) return -1;

	m_pDM_Parent = pDM_Parent;
	m_pGrid_Parent = pGrid_Parent;
	m_strSpeciesKeyAlias = strSpeciesKeyAlias;

	for ( INT i = 0 ; i < arrStrSpeciesKey->GetSize() ; i++ ) {
		m_arrStrSpeciesKey.Add( arrStrSpeciesKey->GetAt(i) );
	}	
	
	return 0;

	EFS_END
	return -1;
}


CString CEquipPrintSaveDlg::MakeCallNoPrintFormat2(INT idx )
{
	CString strCallNo = _T("");
	CString strItemData=_T("");
	INT nHeight;

	nHeight = 0;
	
	//임시로 만듬.. ㅠㅠ;; 헤더 컴파일 때문에.. 덴장..
	CString sQuery = _T("");
	CString sValue = _T("") ;
	sQuery = _T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE GROUP_1 = 'L' AND GROUP_2 = '정리' AND GROUP_3 = '공통' AND CLASS_ALIAS = '레이블권책기호설정'");
	m_pDM_Parent->GetOneValueQuery( sQuery , sValue );	
	sValue.TrimLeft(); sValue.TrimRight();

	INT ids = m_pDM->GetCellData(_T("별치기호_코드"), idx, strItemData);
	if (!strItemData.IsEmpty())
	{
		// 별치기호는 설명으로 바꾼다.
		m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("별치기호"), strItemData, strItemData);
		strCallNo = strItemData + _T("\r\n");		
	}
	
	ids = m_pDM->GetCellData(_T("분류기호"), idx, strItemData);
	if (!strItemData.IsEmpty())		
	{
		strCallNo +=  strItemData;		
	}
	
	ids = m_pDM->GetCellData(_T("도서기호"), idx, strItemData);
	if (!strItemData.IsEmpty())
	{
		strCallNo += _T("\r\n") + strItemData;
	}

	ids = m_pDM->GetCellData(_T("권책기호"), idx, strItemData);
	if (!strItemData.IsEmpty())
	{
		strCallNo += _T("\r\n") + sValue + strItemData;
	}

	ids = m_pDM->GetCellData(_T("복본기호"), idx, strItemData);
	if (!strItemData.IsEmpty())
	{
		strCallNo += _T("\r\nc.") + strItemData;
	}

	

	return strCallNo;
}