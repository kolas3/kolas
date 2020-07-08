// ESL_ControlEdit.cpp : implementation file
//

#include "stdafx.h"
#include "ESL_ControlEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "ESL_Mgr.h"
#include "efs.h"
#include <imm.h>
#include "LIB_F2KEY_SpecialChar.h"
#include "ConvertToHanjaDlg.h"
#include "ECO_FILE_API.h"
#include "ECO_FileIO.h"

#include "ESL_ControlMultiComboBox.h"

/////////////////////////////////////////////////////////////////////////////
// CESL_ControlEdit

CESL_ControlEdit::CESL_ControlEdit(CESL_Control_Element * ELEMENT)
{
	this->ELEMENT = ELEMENT;
	m_Validation = NULL;
	m_nHANENGMODE = ESLEDIT_MODE_NORMAL;

	m_FocusFlag = TRUE;
	m_ctlAutoList = NULL;
	m_bAutoListFocus = FALSE;
	m_bPasswordStyle = FALSE;
	m_bPasswordItem = FALSE;

	m_bKLRegnoMode = FALSE;
	m_pRegCodeCombo = NULL;
	m_nRegCodeComboMode = -1;
	m_nRegCodeLength = 0;
}

CESL_ControlEdit::~CESL_ControlEdit()
{	
	if ( m_ctlAutoList != NULL ) delete m_ctlAutoList;
}


BEGIN_MESSAGE_MAP(CESL_ControlEdit, CEdit)
	//{{AFX_MSG_MAP(CESL_ControlEdit)
	ON_WM_KILLFOCUS()
	ON_WM_KEYDOWN()
	ON_WM_SETFOCUS()
	ON_WM_CREATE()
	ON_WM_CHAR()
	ON_CONTROL_REFLECT(EN_CHANGE, OnChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CESL_ControlEdit message handlers

INT CESL_ControlEdit::InitControl()
{
EFS_BEGIN

	Validationflag = 0;
	ControlStyleCheck(ELEMENT->CTRL_STYLE);
	return 0;

EFS_END
return -1;

}

INT CESL_ControlEdit::SetData(CString data)
{
	SetWindowText(data);
	return 0;
}

INT CESL_ControlEdit::SetData(INT data)
{
	CString tmp;
	tmp.Format(_T("%d"),data);
	SetWindowText(tmp);
	return 0;
}

INT CESL_ControlEdit::SetData(double data)
{
	CString tmp;
	tmp.Format(_T("%f"),data);
	SetWindowText(tmp);
	return 0;
}

INT CESL_ControlEdit::GetData(CString &data)
{
EFS_BEGIN

	GetWindowText(data);
	return 0;

EFS_END
return -1;

}

INT CESL_ControlEdit::GetData(INT *data)
{
EFS_BEGIN

	CString tmp;
	GetWindowText(tmp);
	*data = _ttoi(tmp.GetBuffer(0));
	return 0;

EFS_END
return -1;

}

INT CESL_ControlEdit::EditClear()
{
EFS_BEGIN

	if (ELEMENT->INIT_VALUE.GetLength() > 0) {
		CString strTmp = _T("");
		if (ELEMENT->INIT_VALUE == _T("SYSTEM_YEAR")) {
			CTime t = CTime::GetCurrentTime();
			strTmp.Format(_T("%04d"), t.GetYear());
			SetWindowText(strTmp);
		} 
		if (strTmp.GetLength() > 0) {
			SetWindowText(strTmp);
		} else {
			SetWindowText(ELEMENT->INIT_VALUE);
		}
	} else {
		SetWindowText(_T(""));
	}
	return 0;

EFS_END
return -1;

}

INT CESL_ControlEdit::ValidationCheck(TCHAR *data)
{
EFS_BEGIN

	if (data == NULL) return 0;
	TCHAR     ctmp[256];
	TCHAR    *TokenStr;
	CString tmp;

	if (_tcsclen(data) == 0 ) return -1;

	_tcscpy( ctmp, data );
	
	TokenStr = _tcstok( ctmp, _T("[") );
	tmp.Format(_T("%s"),TokenStr);
	if (tmp == _T("S") || tmp == _T("s"))
	{
		Validationflag = 1;
		TokenStr = _tcstok( NULL, _T("]") );
		if(INTCheck(TokenStr) <0)
		{
			MessageBox(_T("ValidationChecking error!!"), _T("Warnning!"), MB_ICONEXCLAMATION);
			SetReadOnly(TRUE);
			return -2;
		}
		LimitText(_ttoi(TokenStr));
	}
	else if (tmp == _T("I") || tmp == _T("i"))
	{
		Validationflag = 2;

		TokenStr = _tcstok( NULL, _T(":") );
		INTCheck(TokenStr);
		_stscanf(TokenStr, _T("%s"), from);

		TokenStr = _tcstok( NULL, _T("]") );
		_stscanf(TokenStr, _T("%s"), to);
		if(INTCheck(from) <0 || INTCheck(to) <0)
		{
			MessageBox(_T("ValidationChecking error!!"), _T("Warnning!"), MB_ICONEXCLAMATION);
			SetReadOnly(TRUE);
			return -2;
		}
	}
	return 0;	

EFS_END
return -1;

}

INT CESL_ControlEdit::INTCheck(TCHAR *data)
{
EFS_BEGIN

	INT    i_Len , i;

	if ( data == NULL ) return -1;

	i_Len = _tcsclen(data);
	if ( 0 == i_Len ) return 0;

	for (i = 0; i < i_Len ; i++) {
		if ( i_Len == 32 ) break;
		if ( data[i] < '0' || data[i] > '9' ) return -2;
	}
	return 0;

EFS_END
return -1;

}

INT CESL_ControlEdit::ControlStyleCheck(CString data)
{
EFS_BEGIN

	if (data.GetLength() == 0) return 0;
	data.MakeLower();
	if(data == _T("read_only"))
	{
		SetReadOnly(TRUE);
		return 1;
	} 	
	return 0;

EFS_END
return -1;

}

VOID CESL_ControlEdit::OnKillFocus(CWnd* pNewWnd) 
{
EFS_BEGIN

	CEdit::OnKillFocus(pNewWnd);
	
    if ( pNewWnd == NULL ) return;
	if(Validationflag == 2)
	{
		CString tmp;
		GetWindowText(tmp);
		if (tmp.GetLength() > 0) {
			if (_ttoi(tmp.GetBuffer(0)) < _ttoi(from) || _ttoi(tmp.GetBuffer(0)) > _ttoi(to))
			{
				tmp.Format(_T("정수 : %d ~ %d 값만 유효합니다."),_ttoi(from), _ttoi(to));
				MessageBox(tmp, _T("Warnning!"), MB_ICONINFORMATION);
				SetWindowText(_T(""));
				SetFocus();
				return;	
			}
		}
	}

EFS_END
}

INT CESL_ControlEdit::Display(INT index)
{
EFS_BEGIN

	if ( this->GetSafeHwnd() != NULL )
	{
		if ( (this->m_bPasswordStyle && m_bPasswordUse) || m_bPasswordItem ) 
		{
			SetPasswordChar('*');
			if ( m_bPasswordUse )
			{
				EnableWindow(FALSE);
			}
			else
			{
				EnableWindow(TRUE);				
			}
			
		}
	}
	CString link_data = ELEMENT->LINK_DATA;
	if (link_data.GetLength() == 0) return 0;
	CString data;
	if (link_data.GetAt(0) == ':') {
		INT mgrpos = link_data.Find('.', 1);
		if (mgrpos == -1) return -1;
		CESL_Mgr *pMgr = (CESL_Mgr*)ELEMENT->WorkWnd;
		pMgr->GetDataMgrData(link_data.Mid(1, mgrpos-1), link_data.Mid(mgrpos+1), data, index);

		SetData(data);
	}
	return 0;

EFS_END
return -1;

}

INT CESL_ControlEdit::SetHANENGMode(INT nMode)
{
	if (nMode == ESLEDIT_MODE_ENGLISH) {
		m_nHANENGMODE = ESLEDIT_MODE_ENGLISH;
		SetEnglishMode();
		return 0;
	}
	if (nMode == ESLEDIT_MODE_HANGUL) {
		m_nHANENGMODE = ESLEDIT_MODE_HANGUL;
		SetHangulMode();
		return 0;
	}
	m_nHANENGMODE = ESLEDIT_MODE_NORMAL;
	return 0;
}

INT CESL_ControlEdit::SetEnglishMode() 
{
    HIMC himc;
    DWORD dwConversion, dwSentence;
    himc = ImmGetContext(AfxGetMainWnd()->GetSafeHwnd());
    ImmGetConversionStatus(himc, &dwConversion, &dwSentence);
    ImmSetConversionStatus(himc, IME_CMODE_ALPHANUMERIC, dwSentence);
	
	return 0;
}

INT CESL_ControlEdit::SetHangulMode() 
{

    HIMC himc;
    DWORD dwConversion, dwSentence;
    himc = ImmGetContext(AfxGetMainWnd()->GetSafeHwnd());
    ImmGetConversionStatus(himc, &dwConversion, &dwSentence);
    if (!(dwConversion & IME_CMODE_HANGUL))
        ImmSetConversionStatus(himc, dwConversion | IME_CMODE_HANGUL, dwSentence);

	return 0;
}

INT CESL_ControlEdit::GetHanEngStatusItems()
{
    HWND hWnd	= GetSafeHwnd();
    HIMC hIMC	= ImmGetContext(hWnd);
    BOOL bOpen	= ImmGetOpenStatus(hIMC);
	ImmReleaseContext(hWnd,hIMC);

    if (bOpen) {
		return ESLEDIT_MODE_HANGUL;
	} else {
		return ESLEDIT_MODE_ENGLISH;
	}
}

INT CESL_ControlEdit::CheckAndForceStatus(VOID)
{
	if (m_nHANENGMODE != ESLEDIT_MODE_NORMAL) {
		INT nMODE = GetHanEngStatusItems();
		if (m_nHANENGMODE == ESLEDIT_MODE_ENGLISH && nMODE == ESLEDIT_MODE_HANGUL) SetEnglishMode();
		if (m_nHANENGMODE == ESLEDIT_MODE_HANGUL && nMODE == ESLEDIT_MODE_ENGLISH) SetHangulMode();		
	}
	return 0;
}

VOID CESL_ControlEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
    if(nChar==VK_HANGUL)
    {
		CheckAndForceStatus();
		return;
    }
	if (GetKeyState(VK_CONTROL) >= 0 && nChar == VK_F2) {
		InsertSpecialChar();
		return;
	}
	if (GetKeyState(VK_CONTROL) >= 0 && nChar == VK_F6) {
		ConvertWordKorToChi();
		return;
	}
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

VOID CESL_ControlEdit::OnSetFocus(CWnd* pOldWnd) 
{
	CEdit::OnSetFocus(pOldWnd);
	
	CheckAndForceStatus();
	CreateAutoList();	
}

VOID CESL_ControlEdit::InsertSpecialChar()
{
	EFS_BEGIN

	CLIB_F2KEY_SpecialChar sp;
	CString strSpecialChar;
	if (sp.DoModal() == IDOK)
	{
		sp.GetStringData(strSpecialChar);

		ReplaceSel(strSpecialChar, TRUE);
	}

	EFS_END
}

VOID CESL_ControlEdit::ConvertWordKorToChi()
{
	EFS_BEGIN

	INT crOldSel_cpMax, crOldSel_cpMin;
	GetSel(crOldSel_cpMin, crOldSel_cpMax);

	CString strLine;
	CString strSelText;
	CString strHangul;
	CString strBuf;
	GetWindowText(strBuf);
	if (crOldSel_cpMax == crOldSel_cpMin)
	{
		SetSel(LineIndex(), crOldSel_cpMax);
		strLine = strBuf.Mid(LineIndex(), crOldSel_cpMax-LineIndex());

		INT i = 0;
		for (i = strLine.GetLength() - 1; i >= 0; i--)
		{
			TCHAR ch = strLine.GetAt(i);
			if (ch == ' '  ||
				ch == '\0' ||
				ch == '('  ||
				ch == ')'  ||
				(_TUCHAR)ch < 0x80)
			{
				break;
			}
		}

		strSelText = strLine.Mid(i + 1);
	} else {
		strSelText = strBuf.Mid(crOldSel_cpMin, crOldSel_cpMax-crOldSel_cpMin);
	}

	_TUCHAR ch1 = 0;
	_TUCHAR ch2 = 0;
	for (INT i = strSelText.GetLength() - 1; i > 0; i--)
	{
		if ((_TUCHAR)strSelText.GetAt(i) >= 0x80)
		{
			ch1 = (_TUCHAR)strSelText.GetAt(i - 1);
			ch2 = (_TUCHAR)strSelText.GetAt(i);
			if( ch1 >= 0xCA && ch2 >= 0xA1 && ch1 <= 0xFD && ch2 <= 0xFE )
			{
				break;
			}
			i--;
		}
		else
		{
			break;
		}
	}

	strSelText = strSelText.Mid(i + 1);

	if (strSelText.IsEmpty()) return;

	CConvertToHanjaDlg dlg;
	dlg.m_strInput = strSelText;
	if (dlg.DoModal() == IDOK)
	{
		if (!dlg.m_strSelChi.IsEmpty())
		{
			SetSel(crOldSel_cpMax - strSelText.GetLength(), crOldSel_cpMax);
			ReplaceSel(dlg.m_strSelChi, TRUE);
		}
	}

	EFS_END
}

void CESL_ControlEdit::CreateAutoList()
{	
EFS_BEGIN	

	if ( m_ctlAutoList != NULL ) {return;}	
	if ( m_bAutoListFocus == TRUE ) {return;}
	if ( GetSafeHwnd() == NULL ) {return;}	

	CWnd	*EditWnd;
	UINT   nEditID;

	EditWnd = GetFocus();
	if ( EditWnd == NULL ) {return;}
	nEditID = EditWnd->GetDlgCtrlID();	

	if ( IsAutoList(nEditID) == FALSE ) return;	
	
	GetClientRect(&m_rect);
	ClientToScreen(m_rect);	
		
	m_rect.left -= 2;
	m_rect.top += m_rect.bottom - m_rect.top+2;
	m_rect.bottom = m_rect.top + 150; 	
	
	m_ctlAutoList = new CECO_ControlAutoList( this );
	if ( m_ctlAutoList == NULL ) {return;}
	m_ctlAutoList->Create(NULL, NULL, 0, m_rect, GetParent(), NULL, NULL);	
	m_ctlAutoList->MoveWindow(m_rect);
	m_ctlAutoList->SetResource( nEditID, this );
	m_ctlAutoList->Display();
	SetFocus();

EFS_END
}

BOOL CESL_ControlEdit::IsAutoList(INT nEditID)
{
	FILE	*fp;
	_TCHAR	szTemp[1024];
	INT		line_length;
	UINT	nResourceID;
	BOOL	bFlag;

	fp = FdkFopen( _T("..\\CFG\\AutoList.ini"), _T("r") );
	if ( fp == NULL ) {return FALSE;}
	
	line_length = 0;
	bFlag = FALSE;
	do {
		if (CECO_FileIO::next_line(fp, szTemp, &line_length) == -1) { break; }
		nResourceID = _ttoi(szTemp);
		if ( (INT)nResourceID == nEditID )
		{
			FdkFclose( fp );
			return TRUE;
			break;
		}		
	} while(1);
	FdkFclose( fp );

	return FALSE;
}

void CESL_ControlEdit::DeleteAutoList()
{
EFS_BEGIN
	if ( m_ctlAutoList != NULL && GetSafeHwnd() != NULL) 
	{
		CString strText;
		GetWindowText(strText);

		m_ctlAutoList->SetKeyWord(strText.GetBuffer(0));
		m_ctlAutoList->ReferenceWrite();
		delete m_ctlAutoList;
		m_ctlAutoList = NULL;
		m_bAutoListFocus = FALSE;
	}
EFS_END
}

int CESL_ControlEdit::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

BOOL CESL_ControlEdit::PreTranslateMessage(MSG* pMsg) 
{
	INT	s_CtrlKeyFlag, s_AltKeyFlag;
	CString str;
	
	if ( pMsg->message == WM_KEYDOWN )
	{	
		s_CtrlKeyFlag = ::GetKeyState(VK_CONTROL);
		s_AltKeyFlag  = ::GetKeyState(VK_MENU);

		switch ( pMsg->wParam )
		{
			case VK_DELETE :
				break;
			default:				
				CreateAutoList();			
		}	

		if ( !(s_CtrlKeyFlag < 0  || s_AltKeyFlag < 0) )
		{
			switch ( pMsg->wParam )
			{
				case VK_DOWN:

						if(m_ctlAutoList != NULL)
						{
							if ( m_ctlAutoList->m_ctlListBox.GetCurSel() == -1 ) m_ctlAutoList->PrevNextSelection(1);
							else m_ctlAutoList->PrevNextSelection(2);
							return TRUE;
						}
					break;
				case VK_UP:
					if(m_ctlAutoList != NULL)
						{	
							if ( m_ctlAutoList->GetSafeHwnd() != NULL ) m_ctlAutoList->PrevNextSelection(3);
							return TRUE;
					}
					break;
				case VK_LEFT:
					break;
				case VK_RIGHT:
					break;
				case VK_RETURN :
					{	
						if ( m_bAutoListFocus == TRUE )
						{							
							m_bAutoListFocus = FALSE;					
							m_ctlAutoList->ShowWindow(SW_HIDE);
							
							return TRUE;
						}
						else
						{
							if (m_bKLRegnoMode)
							{
								CString strText;
								GetWindowText(strText);

								INT nTextLength = strText.GetLength();
								if (nTextLength > 7) 
								{
									if (strText.GetAt(0) == '$')
									{
										INT nCheck = _ttoi(strText.Mid(1, 6));
										if (nCheck > 0)
										{
											if (m_pRegCodeCombo && m_pRegCodeCombo->GetSafeHwnd())
											{
												INT nRegCodeLength = 0;
												if (m_nRegCodeLength > 0)
												{
													nRegCodeLength = m_nRegCodeLength;
												}
												else
												{
													switch (m_nRegCodeComboMode)
													{
													case 0:
														{
															CComboBox* pCtrl = (CComboBox*)m_pRegCodeCombo;
															if (pCtrl->GetCount() > 0)
															{
																CString strRegCode;
																pCtrl->GetLBText(0, strRegCode);
																nRegCodeLength = strRegCode.GetLength();
															}
														}
														break;
													case 1:
														{
															CESL_ControlMultiComboBox* pCtrl = (CESL_ControlMultiComboBox*)m_pRegCodeCombo;
															if (pCtrl->GetRowCount() > 0)
															{
																CString strRegCode;
																pCtrl->GetData(strRegCode, 0);
																nRegCodeLength = strRegCode.GetLength();
															}
														}
														break;
													}
												}

												if (nRegCodeLength == 0)
												{
													SetWindowText(strText.Mid(7));
												}
												else
												{
													if (strText.Mid(7).GetLength() <= nRegCodeLength)
													{
														SetWindowText(strText.Mid(7));
													}
													else
													{
														CString strRegCode = strText.Mid(7, nRegCodeLength);

														switch (m_nRegCodeComboMode)
														{
														case 0:
															{
																CComboBox* pCtrl = (CComboBox*)m_pRegCodeCombo;
																if (pCtrl->GetCount() > 0)
																{
																	pCtrl->SelectString(0, strRegCode);
																}
															}
															break;
														case 1:
															{
																CESL_ControlMultiComboBox* pCtrl = (CESL_ControlMultiComboBox*)m_pRegCodeCombo;
																if (pCtrl->GetRowCount() > 0)
																{
																	pCtrl->SelectItem(strRegCode);
																}
															}
															break;
														}

														SetWindowText(strText.Mid(7+nRegCodeLength));
													}
												}
											}
											else
											{											
												SetWindowText(strText.Mid(7));
											}
											
											return GetParent()->PreTranslateMessage(pMsg);
										}
									}
								}
							}							
							
							return GetParent()->PreTranslateMessage(pMsg);
						}					
					}
					break;
				case VK_DELETE :
					{
						if ( m_bAutoListFocus == TRUE )	
						{
							m_ctlAutoList->DeleteAutoListKeyWord();						
							return TRUE;
						}
						else
						{
							GetParent()->PreTranslateMessage(pMsg);
							CreateAutoList();		
							return TRUE;

						}					
					}
					break;
				case VK_ESCAPE:
					{
						if ( m_bAutoListFocus == TRUE )	
						{
							m_bAutoListFocus = FALSE;					
							m_ctlAutoList->ShowWindow(SW_HIDE);		
							return TRUE;
						}
						else
						{
							return GetParent()->PreTranslateMessage(pMsg);
						}
					}
					break;
				case VK_BACK:
					break;
				case VK_TAB:
					break;
				case VK_F1:
					break;
				case VK_F2:
					break;
				case VK_F3:
					break;
				case VK_F4:
					break;
				case VK_F5:
					break;
				case VK_F6:
					break;
				case VK_F7:
					break;
				case VK_F8:
					break;
				case VK_F9:
					break;
				case VK_F10:
					break;
				case VK_F11:
					break;
				case VK_F12:
					break;
				case VK_HOME:
					break;
				case VK_END:
					break;
				case VK_INSERT:
					break;			
				default:			
					break;								
			}				
		}			
	}		

	return CEdit::PreTranslateMessage(pMsg);
}

void CESL_ControlEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CEdit::OnChar(nChar, nRepCnt, nFlags);
}

void CESL_ControlEdit::OnChange() 
{	
EFS_BEGIN

	CWnd* pParentDlg = (NULL == ELEMENT) ? NULL : ELEMENT->WorkWnd;
	if(NULL != pParentDlg && NULL != pParentDlg->GetSafeHwnd())
	{ 
		WPARAM wParam = MAKEWPARAM(this->GetDlgCtrlID(), EN_CHANGE);
		pParentDlg->SendMessage(WM_COMMAND, wParam, (LPARAM)pParentDlg->m_hWnd);
	}

	INT nStart;
	INT nEnd;
	CEdit::GetSel(nStart, nEnd);
	CString str;
	GetWindowText(str);	

	if ( m_bAutoListFocus == TRUE ) 
	{
		m_bAutoListFocus = FALSE;
		return;
	}

	if ( m_ctlAutoList != NULL ) 
	{
		CString	strKeyWord;
		GetWindowText(strKeyWord);
		m_ctlAutoList->SetKeyWord(strKeyWord.GetBuffer(0));
		m_ctlAutoList->SearchKeyWord();	
		{return;}
	}
	
EFS_END		
}

