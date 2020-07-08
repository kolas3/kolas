// TableFieldMapping.cpp : implementation file
//

#include "stdafx.h"
#include "TableFieldMapping.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTableFieldMapping dialog


CTableFieldMapping::CTableFieldMapping(CESL_Mgr* pParent /*=NULL*/, CString sMappingType )
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CTableFieldMapping)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_sMappingType = sMappingType;
	m_pMappingDM = NULL;

	InitMappingTable();
}

CTableFieldMapping::~CTableFieldMapping()
{
	delete m_pMappingDM;
}

VOID CTableFieldMapping::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTableFieldMapping)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CTableFieldMapping, CDialog)
	//{{AFX_MSG_MAP(CTableFieldMapping)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTableFieldMapping message handlers

INT CTableFieldMapping::InitMappingTable()
{
	EFS_BEGIN

	MakeMappingDM();
	
	MakeDMFromFile();

	return 0;


	EFS_END
	return -1;

}

INT CTableFieldMapping::MakeMappingDM()
{
	EFS_BEGIN

	/// 데이터 메니져 생성
	m_pMappingDM = new CESL_DataMgr();
	
	// DB CONNECTION 초기화
	m_pMappingDM->SetCONNECTION_INFO(CLocCommonAPI::GetDBConnection());	
	
	// 컬럼 구성하기
	m_pMappingDM->InitDataMgrRef( 3 );
	
	m_pMappingDM->SetDataMgrRef( 0 , _T("소스필드") , _T("") , _T("") , _T("") , _T("") );
	m_pMappingDM->SetDataMgrRef( 1 , _T("적용필드") , _T("") , _T("") , _T("") , _T("") );
	m_pMappingDM->SetDataMgrRef( 2 , _T("타입") , _T("") , _T("") , _T("") , _T("") );
	
	m_pMappingDM->FreeData();

	return 0;

	EFS_END
	return -1;

}

INT CTableFieldMapping::MakeDMFromFile()
{
	EFS_BEGIN

	INT ids;

	CString sFileName;
	sFileName = _MAPPING_FORDER _T("\\") + m_sMappingType + _T(".txt");

	CStdioFile fMappingFile;
	ids = fMappingFile.Open( sFileName , CFile::modeRead | CFile::shareDenyWrite | CFile::typeBinary);
	if( ids==0 )
	{
		CString sEMsg;
		sEMsg.Format( _T("[%s]파일명이 잘못되었습니다.\r\n올바른 경로인지 확인해 주십시오.") , sFileName );
		AfxMessageBox( sEMsg );
		return 1;
	}
	TCHAR cUni;
	fMappingFile.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fMappingFile.SeekToBegin();
	}

	// DM에 입력
	CString sLine;
	CArray < CString , CString > asDataList;


	INT nInsertIndex;
	CString sData;
    while ( fMappingFile.ReadString( sLine ) ) 
	{
        sLine.TrimRight(); sLine.TrimLeft();
		if( sLine.IsEmpty() ) continue;
		if( sLine.GetAt(0) == '.' ) continue;
		
		ids = CLocCommonAPI::MakeArrayList( sLine , _T(">") , asDataList );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("MakeStaticsMainInfoDM") );

		if( asDataList.GetSize() != 3 ) continue;

		m_pMappingDM->InsertRow(-1);

		nInsertIndex = m_pMappingDM->GetRowCount() - 1;

		sData = asDataList.GetAt(0); sData.TrimLeft(); sData.TrimRight();
		m_pMappingDM->SetCellData( _T("소스필드")  , sData, nInsertIndex );
		sData = asDataList.GetAt(1); sData.TrimLeft(); sData.TrimRight();
		m_pMappingDM->SetCellData( _T("적용필드")  , sData , nInsertIndex);		
		sData = asDataList.GetAt(2); sData.TrimLeft(); sData.TrimRight();
		m_pMappingDM->SetCellData( _T("타입")  , sData , nInsertIndex);		
		
	}
	
	fMappingFile.Close();

	return 0;


	EFS_END
	return -1;

}


CESL_DataMgr* CTableFieldMapping::GetMappingInfo()
{
	EFS_BEGIN

	return m_pMappingDM;

	EFS_END
	return NULL;

}
