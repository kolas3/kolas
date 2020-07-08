// LIB_F2KEY_SpecialChar.cpp : implementation file
//

#include "stdafx.h"
#include "except.h"
#include "LIB_F2KEY_SpecialChar.h"
#include "LIB_F2KEY_SpecialCharZoom.h"
#include "ESL_Grid.h"
#include "efs.h"
#include "ECO_FileIO.h "
#include "ECO_FILE_API.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLIB_F2KEY_SpecialChar dialog
CLIB_F2KEY_SpecialCharZoom     m_ZoomChar;
CESL_Grid	  *m_pGrid;	

CLIB_F2KEY_SpecialChar::CLIB_F2KEY_SpecialChar(CWnd* pParent /*=NULL*/)
	: CDialog(CLIB_F2KEY_SpecialChar::IDD, pParent)
{

	//{{AFX_DATA_INIT(CLIB_F2KEY_SpecialChar)
    AppendData = _T("");
	//}}AFX_DATA_INIT

	m_schar.Class_Cnt = 0;
	m_schar.Char_Class = NULL;
    m_CurClassIdx = 0;
    m_CurSetIdx   = 0;

}

CLIB_F2KEY_SpecialChar::~CLIB_F2KEY_SpecialChar()   // standard constructor
{
	FreeAll( &m_schar );
}

VOID CLIB_F2KEY_SpecialChar::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN


	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLIB_F2KEY_SpecialChar)
	DDX_Control(pDX, IDC_eCHAR_SELECTED, m_eChar_Selected);
	DDX_Control(pDX, IDC_tCHAR_CLASS, m_tChar_Class);
	DDX_Control(pDX, IDC_cCHAR_SET, m_cChar_Set);
    DDX_Control(pDX, IDC_ZOOM_CHAR, m_ZoomChar);
	//}}AFX_DATA_MAP


	EFS_END
}


BEGIN_MESSAGE_MAP(CLIB_F2KEY_SpecialChar, CDialog)
	//{{AFX_MSG_MAP(CLIB_F2KEY_SpecialChar)
	ON_CBN_SELCHANGE(IDC_cCHAR_SET, OnSelchangecCHARSET)
	ON_NOTIFY(TCN_SELCHANGE, IDC_tCHAR_CLASS, OnSelchangetCHARCLASS)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLIB_F2KEY_SpecialChar message handlers

///////////////////////////////////////////////////////////////
// ChageByte
//
// [ PURPOSE ]
// $1. 상위바이트와 하위바이트의 순서를 바꾼다.
//
// [ MINISPEC ]
//
///////////////////////////////////////////////////////////////
USHORT CLIB_F2KEY_SpecialChar::ChageByte( USHORT code )
{

	USHORT low, high, result;

	low = 0x00FF & code;
	low <<= 8;

	high = 0xFF00 & code;
	high >>= 8;

/*
    low  = code << 8;
	high = code >> 8;
*/

	result = low | high;

	return result;

}

///////////////////////////////////////////////////////////////
// OnInitDialog
//
// [ PURPOSE ]
// $1. Dialog 초기화 함수이다.
//
// [ MINISPEC ]
// $1. Reference를 Road하여 구조체에 값을 넣는다.
// $2. Combo에 Data를 Insert한다.
// $3. TabCtrl의 항목을 Display한다.
// $4. 문자를 Display한다.
///////////////////////////////////////////////////////////////
BOOL CLIB_F2KEY_SpecialChar::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	
//  TC_ITEM TabCtrlItem;
    INT i, ids;

    // $1. Reference를 Road하여 구조체에 값을 넣는다.
    ids = LoadReference( &m_schar, _T("..\\cfg\\specialchar.cfg") );
	if(ids) {
		return FALSE;
	}

    // $2. Combo에 Data를 Insert한다.
    for ( i = 0; i < m_schar.Class_Cnt; i++ ) {
        m_cChar_Set.AddString( m_schar.Char_Class[i].Name );
    }
    m_cChar_Set.SetCurSel( m_CurClassIdx );

//REM▶     // $3. TabCtrl의 항목을 Display한다.
//REM▶     TabCtrlItem.mask = TCIF_TEXT;
//REM▶     for ( i = 0; i < m_schar.Char_Class[m_CurClassIdx].Set_Cnt; i++ ) {
//REM▶ 	    TabCtrlItem.pszText = m_schar.Char_Class[m_CurSetIdx].Char_Set[i].Name;
//REM▶ 	    m_tChar_Class.InsertItem( i, &TabCtrlItem );
//REM▶     }
    
    // $4. 문자를 Display한다.
    if( 0 != (ids = DisplayChar( m_CurClassIdx, m_CurSetIdx )) ) {
        ::MessageBox( NULL, _T("문자를 Display하다 ERROR가 발생했습니다."), _T("ERROR발생"), MB_OK );
    }
    
    return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE


	EFS_END
	return FALSE;

}

///////////////////////////////////////////////////////////////
// DisplayChar
//
// [ PURPOSE ]
// $1. Grid에 문자를 Display한다.
//
// [ MINISPEC ]
// $1. 1차원 Data를 Col/Row으로 나눈다.
// $2. Cell의 Height와 Width를 설정한다.
// $3. 문자 갯수에 맞게 Grid를 그린다. 
// $4. Grid의 Data를 Clear한다.
// $5. Grid에 tabItem에 해당하는 Data를 Display한다.
///////////////////////////////////////////////////////////////
INT CLIB_F2KEY_SpecialChar::DisplayChar(
    								INT Class_Idx,    // [ IN] : 항목의 Index
                                       INT Set_Idx       // [ IN] : 항목 Data의 Index
                                     ) 
{
	EFS_BEGIN
 
	CString strData;
	_TCHAR ch[3];
	INT ColCnt, RowCnt, CharCnt, i;

//    MEMORY_SET ( ch, 3 )
	
	ch[1] = '\0';
	
    // $1. 1차원 Data를 Col/Row으로 나눈다.
	CharCnt = m_schar.Char_Class[Class_Idx].Char_Set[Set_Idx].Char_Cnt;
	ColCnt = COL_MAX;
	RowCnt = (CharCnt / ColCnt) + (( CharCnt % ColCnt > 0 ) ? 1 : 0);

    // $3. 문자 갯수에 맞게 Grid를 그린다. 
    if ( RowCnt <= ROW_BASE_MAX ) {
        RowCnt = ROW_BASE_MAX + 1;
    }

	m_pGrid = (CESL_Grid*)GetDlgItem(IDC_COM_MSHFLEXGRID);
	//m_f1VCF = (CF1Book *)GetDlgItem( IDC_f1VCF );
	if ( m_pGrid == NULL ) return -1;

    m_pGrid->SetRows( RowCnt );
    m_pGrid->SetCols( 0, ColCnt );
	m_pGrid->Clear();

    // $2. Cell의 Height와 Width를 설정한다.
    for ( i = 0; i < RowCnt; i++ ) {
		m_pGrid->SetRowHeight(i , 350);
		//m_f1VCF->SetRowHeight( i+1, 350 );
	}
	for ( i = 0; i < ColCnt; i++ ) {
		m_pGrid->SetColWidth(i , 0, 350);
		//m_f1VCF->SetColWidth ( i+1, 353 );
	}

    // $4. Grid의 Data를 Clear한다.
	//m_f1VCF->ClearRange( -1, -1, -1, -1, 3 );

    // $5. Grid에 tabItem에 해당하는 Data를 Display한다.
	for ( i = 0; i < CharCnt; i++ ) {
		if ( m_schar.Char_Class[Class_Idx].Char_Set[Set_Idx].Char_Code[i] == 0x0000 ) continue;
		*(USHORT *)ch = m_schar.Char_Class[Class_Idx].Char_Set[Set_Idx].Char_Code[i];
//		m_f1VCF->SetTextRC( (i / ColCnt)+1, (i % ColCnt)+1, ch );

		m_pGrid->SetTextMatrix((i / ColCnt), (i % ColCnt), ch);
		//m_f1VCF->SetTextRC( (i / ColCnt)+1, (i % ColCnt)+1, ch );
	}

    // $6. 선택해있는 Sel의 Data를 크게보기에 Display한다.
	strData = m_pGrid->GetTextMatrix(m_pGrid->GetRow(), m_pGrid->GetCol());
	//strData = m_f1VCF->GetTextRC( m_f1VCF->GetRow(), m_f1VCF->GetCol() );
    m_ZoomChar.ViewChar( strData );

    return 0;


	EFS_END
	return -1;

}

///////////////////////////////////////////////////////////////
// DisplayChar
//
// [ PURPOSE ]
// $1. Grid에 문자를 Display한다.
//
// [ MINISPEC ]
// $1. TabCtrl의 항목을 Display한다.
// $2. TabCtrl의 항목을 Display한다.
///////////////////////////////////////////////////////////////
VOID CLIB_F2KEY_SpecialChar::SetTabItemData( 
											SPECIAL_CHAR * schar,
                                          INT            CurClassIdx 
                                         )
{
	EFS_BEGIN
 
	TC_ITEM TabCtrlItem;
    TabCtrlItem.mask = TCIF_TEXT;

    // $1. TabCtrl의 항목을 Display한다.
    m_tChar_Class.DeleteAllItems();

    // $2. TabCtrl의 항목을 Display한다.
    for ( INT i = 0; i < schar->Char_Class[CurClassIdx].Set_Cnt; i++ ) {
	    TabCtrlItem.pszText = schar->Char_Class[CurClassIdx].Char_Set[i].Name;
	    m_tChar_Class.InsertItem( i, &TabCtrlItem );
    }
    m_tChar_Class.SetCurSel( 0 );


	EFS_END
}

BEGIN_EVENTSINK_MAP(CLIB_F2KEY_SpecialChar, CDialog)
    //{{AFX_EVENTSINK_MAP(CLIB_F2KEY_SpecialChar)
	ON_EVENT(CLIB_F2KEY_SpecialChar, IDC_COM_MSHFLEXGRID, -605 /* MouseDown */, OnMouseDownComMshflexgrid, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CLIB_F2KEY_SpecialChar, IDC_COM_MSHFLEXGRID, -604 /* KeyUp */, OnKeyUpComMshflexgrid, VTS_PI2 VTS_I2)
	ON_EVENT(CLIB_F2KEY_SpecialChar, IDC_COM_MSHFLEXGRID, -601 /* DblClick */, OnDblClickComMshflexgrid, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

///////////////////////////////////////////////////////////////
// OnSelchangecCHARSET
//
// [ PURPOSE ]
// $1. 바뀌는 Combo값에 따라서 값을 Display한다.
//
// [ MINISPEC ]
// $1. 선택된 Class값에 따라서 TabCtrl에 항목을 Display한다.
// $2. 항목에 해당하는 문자Data를 Display한다.
///////////////////////////////////////////////////////////////
VOID CLIB_F2KEY_SpecialChar::OnSelchangecCHARSET() 
{
	EFS_BEGIN


	// TODO: Add your control notification handler code here

    INT Set_Idx;

    // $1. TabCtrl의 항목을 Display한다.
    m_tChar_Class.DeleteAllItems();

    // $2. 선택된 Class값에 따라서 TabCtrl에 항목을 Display한다.
    m_CurClassIdx = m_cChar_Set.GetCurSel();
    SetTabItemData( &m_schar, m_CurClassIdx );

    // $3. 항목에 해당하는 문자Data를 Display한다.
    Set_Idx = m_schar.Char_Class[m_CurClassIdx].Cur_Idx;
    DisplayChar( m_CurClassIdx, 0 );


	EFS_END
}

///////////////////////////////////////////////////////////////
// OnSelchangetCHARCLASS
//
// [ PURPOSE ]
// $1. TabCtrl의 항목이 바뀌면 수행된다.
//
// [ MINISPEC ]
// $1. TabCtrl의 항목 Index를 구한다.
// $2. TabCtrl의 항목에 해당하는 문자Data를 Display한다.
///////////////////////////////////////////////////////////////
VOID CLIB_F2KEY_SpecialChar::OnSelchangetCHARCLASS(NMHDR* pNMHDR, LRESULT* pResult) 
{
	EFS_BEGIN

 
	INT CurSelIdx;

    // $1. TabCtrl의 항목 Index를 구한다.
    CurSelIdx = m_tChar_Class.GetCurSel();
    m_schar.Char_Class[m_CurClassIdx].Cur_Idx = CurSelIdx;

    // $2. TabCtrl의 항목에 해당하는 문자Data를 Display한다.
    DisplayChar( m_CurClassIdx, CurSelIdx );

	*pResult = 0;


	EFS_END
}

///////////////////////////////////////////////////////////////
// FreeAll
//
// [ PURPOSE ]
// $1. malloc한 구조체를 Free 시킨다.
//
// [ MINISPEC ]
///////////////////////////////////////////////////////////////
VOID CLIB_F2KEY_SpecialChar::FreeAll( SPECIAL_CHAR * schar )
{
	EFS_BEGIN

 
	INT i, j;

    m_CurClassIdx = 0;
    m_CurSetIdx   = 0;

    if ( NULL != schar->Char_Class ) {
        for ( i = 0; i < schar->Class_Cnt; i++ ) {
            if ( NULL != schar->Char_Class[i].Char_Set ) {
                for ( j = 0; j < schar->Char_Class[i].Set_Cnt; j++ ) {
                    if ( NULL != schar->Char_Class[i].Char_Set[j].Char_Code ) {
                        free( schar->Char_Class[i].Char_Set[j].Char_Code );
                    }
                }
                free( schar->Char_Class[i].Char_Set );
            }
        }
        free( schar->Char_Class );
    }


	EFS_END
}

///////////////////////////////////////////////////////////////
// GetArrData
//
// [ PURPOSE ]
// $1. 선택한 문자의 String Data를 Return한다.
//
// [ MINISPEC ]
///////////////////////////////////////////////////////////////
INT CLIB_F2KEY_SpecialChar::GetArrData( TCHAR ** arrData )
{
	EFS_BEGIN

 
	if ( NULL == arrData ) return -1;
    
    *arrData = (TCHAR *)malloc( sizeof(TCHAR)*(AppendData.GetLength()+sizeof(TCHAR)));
    if ( arrData == NULL ) return -2;

    _tcscpy( *arrData, AppendData.GetBuffer(0) );

    return 0;


	EFS_END
	return -1;

}


///////////////////////////////////////////////////////////////
// GetStringData
//
// [ PURPOSE ]
// $1. 선택한 문자의 String Data를 Return한다.
//
// [ MINISPEC ]
///////////////////////////////////////////////////////////////
VOID CLIB_F2KEY_SpecialChar::GetStringData( CString & strData )
{
	EFS_BEGIN

 
	strData = AppendData;


	EFS_END
}

//------------------------------------------------------------------//
// 작성일자       : 1999.1.11                                       //
// 작성자         : 백승우                                          // 
// RETURBN VALUE  :    0 (정상)                                     // 
//                   < 0 (비정상)                                   //
//                                                                  //   
// 함수설명       :                                                 //
//                   REFERENCE FILE에서 정보를 읽어와서             //  
//                   언어,구조,도형의 각각에 해당하는 문자 SET 값을 // 
//                   SPECIAL_CHAR 구조체에 저장한다.                //
//------------------------------------------------------------------//  
INT CLIB_F2KEY_SpecialChar::LoadReference(SPECIAL_CHAR* schar, TCHAR* pFileName)
{
	EFS_BEGIN


if(schar == NULL) {
		//	RETURN_INIT -11;
			return -11;
		}
		if(pFileName == NULL) {
		//	RETURN_INIT -11;
			return -11;
		}
		
		CECO_FileIO FileIO;
		FILE *fp;
		INT i, j, k,idx, line_length, ids, idd , x , y, ntemp;
		_TCHAR ctmp[30000], alias[1000], s_data[30000],tmp_alias[1000],tmp[30000];
		_TCHAR *TokenStr;

		/*
        MEMORY_SET ( ctmp     , 30000 )
        MEMORY_SET ( alias    ,  1000 )
        MEMORY_SET ( s_data   , 30000 )
        MEMORY_SET ( tmp_alias,  1000 )
        MEMORY_SET ( tmp      , 30000 )
		*/
		
		if ( (fp = FdkFopen(pFileName, _T("r"))) == NULL) {
//			Error_Print(this,-1,_T("FILE READ ERROR"), MB_OK);
//			{RETURN_INIT -1;}
			return -1;
		}
		
		ctmp[0] = alias[0] = s_data[0] = tmp_alias[0] = tmp[0] = _T('\0');
		schar->Class_Cnt = 0;
		schar->Char_Class = NULL;
		idx = ids = idd = x = y = k = i = j = ntemp = 0 ;
		
		if (FileIO.next_line(fp, ctmp, &line_length) == -1) {
			FdkFclose(fp);
//			Error_Print(this,ids,_T("FILE READ ERROR"),MB_OK);
//			{RETURN_INIT -2;}
			return -2;
		}
		
		while(1){
			FileIO.str_rtrim( ctmp, ctmp, _T(' ') );
			if ( _T('[') == ctmp[0] ) {
				
				FileIO.str_trimall( ctmp, ctmp, _T('[') );
				FileIO.str_trimall( ctmp, ctmp, _T(']') );
				FileIO.str_trimall( ctmp, ctmp, _T(' ') );
				FileIO.GetTagData(ctmp, alias, s_data);
				
				// _TCHAR CLASS COUNT 설정
				if(_tcsicmp(alias,_T("CHAR_CLASS_CNT")) == 0){
					schar->Class_Cnt = _ttoi(s_data);
					schar->Char_Class = (CHAR_CLASS*)malloc(sizeof(CHAR_CLASS) * schar->Class_Cnt);
					if(schar->Char_Class == NULL) {
						FdkFclose(fp);
//						{RETURN_INIT -3;}
						return -3;
					}
					
					// _TCHAR CLASS 초기화
					for( i =0 ; i < schar->Class_Cnt; i++){
						schar->Char_Class[i].Name[0] = _T('\0');
						schar->Char_Class[i].Set_Cnt = 0;
						schar->Char_Class[i].Char_Set = 0;
					}
				}
				
				// _TCHAR CLASS ALIAS 설정
				else if(_tcsicmp(alias,_T("CHAR_CLASS_ALIAS")) == 0){
					_stprintf(tmp_alias,_T("%s"),s_data);
					TokenStr = _tcstok( s_data, _T(",") );
					for ( i = 0; i < schar->Class_Cnt; i++ ) {
						if ( NULL != TokenStr ) {
							_stprintf(schar->Char_Class[i].Name,TokenStr);
							if ( i == schar->Class_Cnt - 1 ) break;
							TokenStr = _tcstok( NULL, _T(",") );
						} else break;
					}
				}
				
				// 읽어온 문자열이 _TCHAR SET CLASS의 NAME인 경우
				// _TCHAR SET 설정
				else if(_tcsicmp(alias,schar->Char_Class[ids].Name) == 0){
					
					if (FileIO.next_line(fp, ctmp, &line_length) == -1) break;
					FileIO.str_trimall( ctmp, ctmp, _T(' ') );
					FileIO.GetTagData(ctmp, alias, s_data);
					
					// _TCHAR SET CNT설정
					if(_tcsicmp(alias,_T("CHAR_SET_CNT")) == 0){
						schar->Char_Class[ids].Set_Cnt = _ttoi(s_data);
						schar->Char_Class[ids].Cur_Idx = 0;
						schar->Char_Class[ids].Char_Set = (CHAR_SET*)malloc(sizeof(CHAR_SET) * schar->Char_Class[ids].Set_Cnt);
						if(schar->Char_Class[ids].Char_Set == NULL) {
							for( j = 0; j < ids ;j++){
								free(schar->Char_Class[j].Char_Set);
							}
							FdkFclose(fp);
//							{RETURN_INIT -4;}
							return -4;
						}
						
						// 메모리를 할당한 _TCHAR SET을 초기화한다.
						for( i = 0 ; i < schar->Char_Class[ids].Set_Cnt; i++){
							schar->Char_Class[ids].Char_Set[i].Name[0] = _T('\0');
							schar->Char_Class[ids].Char_Set[i].Char_Cnt = 0;
							schar->Char_Class[ids].Char_Set[i].Char_Code = NULL;
						}
						
					}
					
					// _TCHAR SET ALIAS설정
					if (FileIO.next_line(fp, ctmp, &line_length) == -1) break;
					FileIO.str_trimall( ctmp, ctmp, _T(' ') );
					FileIO.GetTagData(ctmp, alias, s_data);
					
					if(_tcsicmp(alias,_T("CHAR_SET_ALIAS")) == 0){
						_stprintf(tmp_alias,_T("%s"),s_data);
						TokenStr = _tcstok( s_data, _T(",") );
						for ( idd = 0; idd < schar->Char_Class[ids].Set_Cnt; idd++ ) {
							if ( NULL != TokenStr ) {
								_stprintf(schar->Char_Class[ids].Char_Set[idd].Name,TokenStr);
								if ( idd == schar->Char_Class[ids].Set_Cnt - 1 ) break;
								TokenStr = _tcstok( NULL, _T(",") );
							} else break;
						}
						
					}
					ids++;
				}
				
				// 읽어온 문자열이 _TCHAR SEL CLASS의 _TCHAR DATA NAME과 일치하는 경우
				else if(_tcsicmp(alias,schar->Char_Class[x].Char_Set[y].Name) == 0){
					
					if (FileIO.next_line(fp, ctmp, &line_length) == -1) break;
					FileIO.str_trimall( ctmp, ctmp, _T(' ') );
					FileIO.GetTagData(ctmp, alias, s_data);
					
					// _TCHAR DATA CNT설정
					if(_tcsicmp(alias,_T("CHAR_CNT")) == 0){
						schar->Char_Class[x].Char_Set[y].Char_Cnt = _ttoi(s_data);
						schar->Char_Class[x].Char_Set[y].Char_Code = (unsigned short*)malloc(sizeof(unsigned short) * schar->Char_Class[x].Char_Set[y].Char_Cnt);
						if(schar->Char_Class[x].Char_Set[y].Char_Code == NULL) {
							for( i = 0 ; i < x ; i++){
								free(schar->Char_Class[i].Char_Set);
								for(j = y ; j < y ; y++){
									free(schar->Char_Class[i].Char_Set[j].Char_Code);
								}
							}
							FdkFclose(fp);
							//{RETURN_INIT -5;}
							return -5;
						}
					}
					if (FileIO.next_line(fp, ctmp, &line_length) == -1) break;
					FileIO.str_trimall( ctmp, ctmp, _T(' ') );
					FileIO.GetTagData(ctmp, alias, s_data);
					
					if(_tcsicmp(alias,_T("CHAR_DATA")) == 0){
						// _TCHAR DATA가 여러 라인에 걸쳐서 있는 경우
						// 마지막 라인에 _T(',') 이 있는지를 검사해서
						// _T(',')이 있으면 다음 라인을 읽어 온다.
						if(s_data[_tcsclen(s_data)-1] == _T(',')){
							_stprintf(ctmp,_T("%s"),s_data);
							i = j = 0;
							
							// 읽어온 문자열의 첫번째 값이 '['인 경우와 SPACE일때까지
							// 다음 라인을 읽어서 strtok한다.
							do{
								// _T(',')ㄱ를 가지고 strtok한다.
								TokenStr = _tcstok( ctmp, _T(",") );
								for ( k = i; k < schar->Char_Class[x].Char_Set[y].Char_Cnt; k++ ) {
									if ( NULL != TokenStr ) {
										j++;
										//FdkSscanf(TokenStr,_T("%x"),&ntemp);
										swscanf(TokenStr,_T("%x"),&ntemp);
										schar->Char_Class[x].Char_Set[y].Char_Code[k] = (unsigned short)ntemp;
										if( k == schar->Char_Class[x].Char_Set[y].Char_Cnt - 1 ) break;
										TokenStr = _tcstok( NULL, _T(",") );
									} else break;
								}
								FdkFgets(ctmp,30000,fp);
								FileIO.str_trimall( ctmp, ctmp, _T(' ') );
								i = j;
							}while((ctmp[0] != _T('[')) && (_tcsclen(ctmp) != 0));
						}
						
						// _TCHAR DATA가 한 라인에 있는 경우
						else{
							TokenStr = _tcstok( s_data, _T(",") );
							for ( k = 0; k < schar->Char_Class[x].Char_Set[y].Char_Cnt; k++ ) {
								if ( NULL != TokenStr ) {
									//FdkSscanf(TokenStr,_T("%x"),&ntemp);
									swscanf(TokenStr,_T("%x"),&ntemp);
									schar->Char_Class[x].Char_Set[y].Char_Code[k] = (unsigned short)ntemp;
									if ( k == schar->Char_Class[x].Char_Set[y].Char_Cnt - 1 ) break;
									TokenStr = _tcstok( NULL, _T(",") );
								} else break;
							}
						}
						y++;
					}
					if( y == schar->Char_Class[x].Set_Cnt){
						x++;
						y =0;
					}
				}
		}
		if (FileIO.next_line(fp, ctmp, &line_length) == -1) break;
	}
	
	FdkFclose(fp);


	EFS_END
	return 0;

}

VOID CLIB_F2KEY_SpecialChar::OnOK() 
{
	EFS_BEGIN


	m_eChar_Selected.GetWindowText( AppendData );
	CDialog::OnOK();


	EFS_END
}

VOID CLIB_F2KEY_SpecialChar::OnCancel() 
{
	EFS_BEGIN


	CDialog::OnCancel();


	EFS_END
}

VOID CLIB_F2KEY_SpecialChar::OnMouseDownComMshflexgrid(short Button, short Shift, long x, long y) 
{
	EFS_BEGIN

	// $3. Data를 크게보기Ctrl에 Display한다.
	m_pGrid = (CESL_Grid*)GetDlgItem(IDC_COM_MSHFLEXGRID);
	CString Data = m_pGrid->GetText();
    m_ZoomChar.ViewChar( Data );
	

	EFS_END
}

VOID CLIB_F2KEY_SpecialChar::OnKeyUpComMshflexgrid(short FAR* KeyCode, short Shift) 
{
	EFS_BEGIN

	CString Data;

	m_pGrid= (CESL_Grid*)GetDlgItem(IDC_COM_MSHFLEXGRID);
	if (m_pGrid== NULL) return;

	//m_f1VCF = (CF1Book *)GetDlgItem( IDC_f1VCF );
	//if ( m_f1VCF == NULL ) return;

    if ( *KeyCode == VK_LEFT  || *KeyCode == VK_UP   ||
         *KeyCode == VK_RIGHT || *KeyCode == VK_DOWN  ) 
    {
        //Data = m_f1VCF->GetText();
		Data = m_pGrid->GetText();
        m_ZoomChar.ViewChar( Data );
    }

    if ( *KeyCode == VK_SPACE ) {
        // $1. 선택된 Cell의 Data를 구한다.
        //Data = m_f1VCF->GetText();
		Data = m_pGrid->GetText();

        // $2. 선택문자 ctrl에 Data를 Append한다.
		m_eChar_Selected.GetWindowText( AppendData );
        m_eChar_Selected.SetWindowText( (LPCTSTR)AppendData );
        m_eChar_Selected.SendMessage( WM_KEYDOWN, VK_END, 0L );
    }
	

	EFS_END
}

VOID CLIB_F2KEY_SpecialChar::OnDblClickComMshflexgrid() 
{
	EFS_BEGIN

	m_pGrid= (CESL_Grid*)GetDlgItem(IDC_COM_MSHFLEXGRID);
	if (m_pGrid== NULL) return;
	//m_f1VCF = (CF1Book *)GetDlgItem( IDC_f1VCF );
	//if ( m_f1VCF == NULL ) return;

	// $1. 선택된 Cell의 Data를 구한다.
    //CString Data = m_f1VCF->GetText();
	CString Data = m_pGrid->GetText();

    // $2. 선택문자 ctrl에 Data를 Append한다.
	m_eChar_Selected.GetWindowText( AppendData );
    AppendData = AppendData + Data;
    m_eChar_Selected.SetWindowText( (LPCTSTR)AppendData );
    m_eChar_Selected.SendMessage( WM_KEYDOWN, VK_END, 0L );


	EFS_END
}

HBRUSH CLIB_F2KEY_SpecialChar::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;

}
